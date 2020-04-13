/*
 * SimpleCluster.cc
 *
 *  Created on: 2019年6月24日
 *      Author: hiro
 */

#include "inet/networklayer/icn/cluster/SimpleCluster.h"
#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/dissector/ProtocolDissector.h"
#include "inet/common/packet/dissector/ProtocolDissectorRegistry.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/networklayer/icn/cluster/SimpleClusterPacket_m.h"
#include "inet/common/bloomfilter/hash/MurmurHash3.h"
#include "inet/common/packet/chunk/Chunk.h"


#include <fstream>
#include <array>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <time.h>

namespace inet
{

Define_Module(SimpleCluster);

int SimpleCluster::change = 0;
double SimpleCluster::proTime = 0;
BloomFilter<std::array<Word, 4>> SimpleCluster::nidFilter{};

void SimpleCluster::initialize(int stage)
{
    OperationalBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL)
    {

        int ID = getContainingNode(this)->getId();
        std::array<uint64_t, 2> hashValue;
        MurmurHash3_x64_128(&ID, sizeof(int), 0, hashValue.data());

        neighbors = new std::set<int>;
        nodeIndex = getContainingNode(this)->getIndex();
        nid.setNID(hashValue);
        nid.test = nodeIndex;

        nidFilter.Insert(nid.getNID());


        wait = new cMessage("wait");
        startTimer = new cMessage("start");
        hello = new cMessage("hello");
        collect = new cMessage("collect");
        retry = new cMessage("retry");
        iniTimer = new cMessage("init over");
        prehead = new cMessage("become head");

        neighborsClear = new cMessage("neighbors clear");

        waitingTime = par("waitingTime").doubleValue();
        PreHeadChange = waitingTime;
        startTime = par("startTime").doubleValue();
        collectingTime = par("collectTime").doubleValue();
        interval = par("interval").doubleValue();
        retryTimes = par("retry").intValue();
        helloTime = par("hello").doubleValue();

        
        path = par("path").stdstringValue();
        filename = path+par("recordFile").stdstringValue();
        inifile = path+par("iniFile").stdstringValue();

        state = INI;

        maxNeighbor = nid;
    }
    else if (stage == INITSTAGE_NETWORK_LAYER)
    {
        scheduleAt(startTime, startTimer);
        registerService(Protocol::simplecluster, gate("ifIn"), SP_INDICATION);
        registerService(Protocol::simplecluster, gate("ifIn"), SP_CONFIRM);
        registerProtocol(Protocol::simplecluster, gate("ifOut"), SP_REQUEST);
        registerProtocol(Protocol::simplecluster, gate("ifOut"), SP_RESPONSE);
    }
}

InterfaceEntry *SimpleCluster::chooseInterface(const char *interfaceName)
{
    //得到指向转发表的指针
    auto name = getContainingNode(this)->getFullPath();
    name = name + ".interfaceTable";
    auto path = name.c_str();
    cModule *mod = this->getModuleByPath(path);
    ift = dynamic_cast<IInterfaceTable *>(mod);
    //    IInterfaceTable *test = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);

    InterfaceEntry *ie = nullptr;

    if (strlen(interfaceName) > 0)
    {
        ie = ift->findInterfaceByName(interfaceName);
        if (ie == nullptr)
            throw cRuntimeError("Interface \"%s\" does not exist", interfaceName);
    }

    return ie;
}

void SimpleCluster::finish()
{
    cancelAndDelete(wait);
    cancelAndDelete(startTimer);
    cancelAndDelete(hello);
    cancelAndDelete(collect);
    cancelAndDelete(retry);
    cancelAndDelete(iniTimer);
    cancelAndDelete(neighborsClear);
    cancelAndDelete(prehead);

    recorder(filename);
    topology(1, path + "all_edge.txt");
    topology(0, path + "edge.txt");

    if(nodeIndex==0)
    {
        std::ofstream outfile;
        outfile.open("authTime.txt", std::ofstream::app);
        outfile<<proTime<<",";
        outfile.close();
    }



    if (getContainingNode(this)->getIndex() == 0)
        std::cout
            << "change number is: " << change << endl;
    delete neighbors;
}

SimpleCluster::~SimpleCluster()
{
    
}

SimpleCluster::SimpleCluster() : nid(), maxNeighbor(), path(), filename(), position(), inifile()
{

}

void SimpleCluster::handleMessageWhenUp(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        handleSelfMessage(msg);
    }
    else
    {
        Packet *packet = check_and_cast<Packet *>(msg);
        auto head = packet->peekAtFront<Chunk>(B(18), Chunk::PF_ALLOW_ALL);
        auto pointer = head.get();

        //转换为raw pointer
        Chunk *chunk = const_cast<Chunk *>(pointer);
        if (dynamic_cast<SimpleClusterPacket *>(chunk))
        {
            processClusterPacket(packet);
        }
        else
        {
            processAuthPacket(packet);
        }

    }
}

void SimpleCluster::handleStartOperation(LifecycleOperation *operation)
{
    ASSERT(clusterTable.table.empty());
    ie5 = chooseInterface("wlan0");

    std::ofstream outfile;
    if (getParentModule()->getParentModule()->getIndex() == 0)
    {
        outfile.open(filename, std::ofstream::app);
        outfile << endl;
        outfile.close();

        outfile.open(inifile, std::ofstream::app);
        outfile << endl;
        outfile.close();
    }
}

void SimpleCluster::handleStopOperation(LifecycleOperation *operation)
{
    flush();
}

void SimpleCluster::handleCrashOperation(LifecycleOperation *operation)
{
    flush();
}

void SimpleCluster::flush()
{
    clusterTable.table.clear();
}

void SimpleCluster::refreshDisplay() const
{
    OperationalBase::refreshDisplay();

    // std::stringstream os;

    // os << "size:" << arpCache.size() << " sent:" << numRequestsSent << "\n"
    //    << "repl:" << numRepliesSent << " fail:" << numFailedResolutions;

    // getDisplayString().setTagArg("t", 0, os.str().c_str());
}

void SimpleCluster::processClusterPacket(Packet *packet)
{
    ASSERT(isUp() && ie5 != nullptr);
    const auto &head = packet->popAtFront<SimpleClusterPacket>();
    const auto &type = head->getType();
    //    std::cout<<"index is:"<<getParentModule()->getParentModule()->getIndex()<<endl;
    cancelEvent(iniTimer);
    switch (type)
    {
    case INIT:
    {
        auto id = head->getNid();
        int temp = head->getNodeIndex();
        neighbors->insert(temp);

        // cancelEvent(wait);

        maxNeighbor = maxNeighbor < id ? id : maxNeighbor;
        if(nodeIndex == 1)
        {

        }
        break;
    }

    case CALL:
    {
    
        clusterHeads.insert(head->getNodeIndex());

        if (state == INI)
        {
            becomeMember();
            cancelEvent(retry);
            cancelEvent(collect);
            // std::cout << nodeIndex << "become member" << endl;
        }
        else if (state == MEMBER)
        {
            if(clusterHeads.size()>1)
                becomeGateway();
            scheduleWait();
        }
        else if (state == GATEWAY)
        {
            if(clusterHeads.size() == 1)
                becomeMember();
            scheduleWait();
        }           
        else if(state == PREHEAD)
        {
            
            if (nodeIndex < head->getNodeIndex())
            {

                becomeMember();
            }
                
        }
        else if(state == HEAD)
        {
            
        }
        else
        {
            throw cRuntimeError("unknown node state");
        }
        
        ClusterEntry entry(head->getNid(), head->getMAC(), simTime());
        clusterTable.table.insert(entry);

        break;
    }
    case ACK:
    {
        break;
    }
    }
   delete packet;
}

void SimpleCluster::processAuthPacket(Packet *packet)
{
    ASSERT(isUp() && ie5 != nullptr);
    const auto &head = packet->popAtFront<AuthPacket>();

    auto sig = head->getSignature();
    auto otherNid = head->getNid();
    auto rsa = head->getPublicKey();
    auto type = head->getType();
    clock_t start, end;
    start = clock();
    if (nidFilter.Check(otherNid.getNID()))
    {
        unsigned char *encode = new unsigned char[RSA_size(rsa)];

        memset(encode, 0, RSA_size(rsa));
        memcpy(encode, sig, RSA_size(rsa));

        unsigned char *decode = new unsigned char[RSA_size(rsa)];
        memset(decode, 0, RSA_size(rsa));
        RSA_public_decrypt(RSA_size(rsa), encode, decode, rsa, RSA_PKCS1_PADDING);

        if(type==0)
            sendAuth(1,head->getTime());
        else
        {
            proTime += 1000*(simTime().dbl() -head->getTime());
            std::cout<<proTime<<" ms"<<endl;
        }
        end = clock();
        proTime += 1000 * static_cast<double>(end - start)/CLOCKS_PER_SEC;

    }
    else
    {
        std::cout << "deny!" << endl;
    }

    delete packet;
}

void SimpleCluster::handleSelfMessage(cMessage *msg)
{
    if (msg == startTimer)
    {
        if(simTime()>0)
        {
            sendAuth(0,simTime().dbl());
        }
        else
        {
            sendCluster(INIT);
            scheduleAt(iniTime, iniTimer);
            scheduleAt(simTime() + collectingTime, collect);
            scheduleRetry();
        }     
    }
    else if (msg == collect)
    {
        cancelEvent(retry);
        if (isMax())
        {
            becomeHead();
        }
        else
        {
//            becomeMember();
            scheduleWait();        
        }
            
        recorder(inifile);
    }
    else if (msg == retry)
    {

        if (retryTimes > 1)
            scheduleRetry();
    }
    else if (msg == wait)
    {
        if(state == INI || simTime()<1)
        {
            becomePrehead();
        } 
        else
        {
            std::cout<< simTime()<<endl;
            becomeHead();
        }        
    }
    else if (msg == hello)
    {
        scheduleHello();
    }
    else if(msg == iniTimer)
    {
        
        // becomeHead();
    }
    else if (msg == neighborsClear)
    {
        clusterHeads.clear();
        scheduleAt(simTime() + nbClearInterval, neighborsClear);
    }
    else if(msg == prehead)
    {
        becomeHead();
    }
}

bool SimpleCluster::isMax()
{
    return nid == maxNeighbor;
}

void SimpleCluster::sendCluster(PacketType type)
{
    
    //发送端口的mac地址
    auto src = ie5->getMacAddress();

    //新数据包
    Packet *packet = new Packet("init");

    //分簇协议报文头部
    const auto &clusterMsg = makeShared<SimpleClusterPacket>();
    clusterMsg->setType(type);
    clusterMsg->setNid(nid);
    // clusterMsg->setSignature(e());
    clusterMsg->setMAC(src);
    clusterMsg->setNodeIndex(nodeIndex);
    
    //数据包添加头部
    packet->insertAtFront(clusterMsg);

    //为数据包加上标签设置源地址和广播
    auto macAddrReq = packet->addTag<MacAddressReq>();
    macAddrReq->setSrcAddress(src);
    macAddrReq->setDestAddress(MacAddress::BROADCAST_ADDRESS);

    //添加协议标签
    packet->addTag<PacketProtocolTag>()->setProtocol(&Protocol::simplecluster);

    //添加接口标签
    packet->addTag<InterfaceReq>()->setInterfaceId(ie5->getInterfaceId());

    send(packet, "ifOut");
}

void SimpleCluster::sendHeadAdvertise()
{
    auto src = ie5->getMacAddress();
    Packet *packet = new Packet("hello");
    const auto &clusterMsg = makeShared<SimpleClusterPacket>();
    clusterMsg->setType(CALL);
    clusterMsg->setNid(nid);

    clusterMsg->setMAC(src);
    packet->insertAtFront(clusterMsg);

    auto macAddrReq = packet->addTag<MacAddressReq>();
    macAddrReq->setSrcAddress(src);
    macAddrReq->setDestAddress(MacAddress::BROADCAST_ADDRESS);

    packet->addTag<PacketProtocolTag>()->setProtocol(&Protocol::simplecluster);
    packet->addTag<InterfaceReq>()->setInterfaceId(ie5->getInterfaceId());

    send(packet, "ifOut");
}

void SimpleCluster::sendAuth(int type, double time)
{
    RSA *rsa = RSA_new();
    auto publicKey = fopen("/home/hiro/keys/rsa_public_key0.pem", "r");
    PEM_read_RSA_PUBKEY(publicKey, &rsa, NULL, NULL);
    fclose(publicKey);

    auto privateKey = fopen("/home/hiro/keys/rsa_private_key0.pem", "r");
    PEM_read_RSAPrivateKey(privateKey, &rsa, NULL, NULL);
    char *source = "zeusNet";
    unsigned char *encode = new unsigned char[RSA_size(rsa)];
    memset(encode, 0, RSA_size(rsa));
    if (RSA_private_encrypt(strlen(source), (unsigned char *)source, encode, rsa, RSA_PKCS1_PADDING) < 0)
    std::cout << "error" << endl;

    auto src = ie5->getMacAddress();
    Packet *packet = new Packet("auth");
    const auto &authMsg = makeShared<AuthPacket>();

    authMsg->setNid(nid);
    authMsg->setPublicKey(rsa);

    authMsg->setSignature(encode);
    authMsg->setType(type);
    authMsg->setTime(time);
    packet->insertAtFront(authMsg);

    auto test = authMsg->getSignature();
    unsigned char *decode = new unsigned char[RSA_size(rsa)];
    memset(decode, 0, RSA_size(rsa));
    RSA_public_decrypt(RSA_size(rsa), test, decode, rsa, RSA_PKCS1_PADDING);



    auto macAddrReq = packet->addTag<MacAddressReq>();
    macAddrReq->setSrcAddress(src);
    macAddrReq->setDestAddress(MacAddress::BROADCAST_ADDRESS);

    packet->addTag<PacketProtocolTag>()->setProtocol(&Protocol::simplecluster);
    packet->addTag<InterfaceReq>()->setInterfaceId(ie5->getInterfaceId());

    send(packet, "ifOut");
}

void SimpleCluster::scheduleHello()
{
    cancelEvent(hello);
    sendCluster(CALL);
    simtime_t time = uniform(0, helloTime);
    scheduleAt(simTime() + time, hello);
}

void SimpleCluster::scheduleWait()
{
    cancelEvent(wait);

    scheduleAt(simTime() + waitingTime, wait);
}

void SimpleCluster::scheduleRetry()
{

    if (retryTimes > 0)
    {
        retryTimes--;
        sendCluster(INIT);
        simtime_t time = uniform(0, interval);
        scheduleAt(simTime() + time, retry);
    }
}

void SimpleCluster::becomeMember()
{
    cancelEvent(prehead);
    cancelEvent(neighborsClear);
    scheduleAt(simTime() + nbClearInterval, neighborsClear);
    cancelEvent(hello);
    scheduleWait();

    getContainingNode(this)->bubble("member!");

//    std::cout << simTime() << " member" << endl;
    state = MEMBER;
}

void SimpleCluster::becomePrehead()
{
    cancelEvent(wait);
    scheduleAt(simTime() + PreHeadChange, prehead);
    state = PREHEAD;
//    std::cout << simTime() << " prehead" << endl;
    scheduleHello();
}

void SimpleCluster::becomeHead()
{
    cancelEvent(wait);

    getParentModule()->getParentModule()->bubble("I'm cluster head!");
    state = HEAD;

    if (simTime() > collectingTime + waitingTime)
        change++;

    scheduleHello();
}

void SimpleCluster::becomeGateway()
{
    cancelEvent(neighborsClear);
    scheduleAt(simTime() + nbClearInterval, neighborsClear);
    cancelEvent(hello);
    scheduleWait();
    getParentModule()->getParentModule()->bubble("I'm Gateway");
    state = GATEWAY;
}

void SimpleCluster::recorder(std::string filename)
{
    std::ofstream outfile;
    outfile.open(filename, std::ofstream::app);
    int index = getParentModule()->getParentModule()->getIndex();

    if(state == HEAD)
        outfile << 1;
    else if(state == GATEWAY)
        outfile << 2;
    else
        outfile << 0;
    outfile.close();

    outfile.open(path+"position.txt", std::ofstream::app);
    auto positon = getPosition();
    outfile << positon.getX() << ", " << positon.getY() << endl;
    outfile.close();
}
    

void SimpleCluster::topology(bool all, std::string filename)
{
    std::ofstream outfile;
    
    if (state == HEAD || all)
    {
        outfile.open(filename, std::ofstream::app);
        for (auto iter : *neighbors)
        {
            outfile << nodeIndex << ", " << iter << endl;
        }
        outfile.close();
    }
}

Coord SimpleCluster::getPosition()
{
    cModule *host = getContainingNode(this);
    IMobility *mobility = check_and_cast<IMobility *>(host->getSubmodule("mobility"));
    return mobility->getCurrentPosition();
}

const ICluster::ClusterTable SimpleCluster::getClusterHead()
{
    return clusterTable;
}

bool SimpleCluster::isHead()
{
    return state == HEAD;
}

bool SimpleCluster::isPreHead()
{
    return state == PREHEAD;
}

bool SimpleCluster::isGateWay()
{
    return state == GATEWAY;
}



} // namespace inet
