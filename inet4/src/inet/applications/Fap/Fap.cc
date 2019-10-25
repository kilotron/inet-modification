//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "inet/applications/Fap/Fap.h"

#include"math.h"
#include"utility"
#include <string.h>
#include"inet/mobility/contract/IMobility.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/common/Simsignals.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/networklayer/ipv4/Ipv4InterfaceData.h"
#include"FAPmessage_m.h"
#include"./hash/MurmurHash3.h"
#include <ctime>
//#include "inet/common/packet/chunk/Chunk.h"


namespace inet {
Define_Module(Fap);
//
Fap::Fap()
{}

Fap::~Fap()
{
    cancelAndDelete(timerTL);
    cancelAndDelete(timerTP);
    cancelAndDelete(timerTW);
    cancelAndDelete(timerTH);
    cancelAndDelete(timerts);
    cancelAndDelete(timerTM);
    cancelAndDelete(timerTF);
    cancelAndDelete(startTimer);
}

void Fap::initialize(int stage)
{
    ApplicationBase::initialize(stage);
    if(stage==INITSTAGE_LOCAL)
    {
        timerTL = new cMessage("TL");
        timerTP = new cMessage("TP");
        timerTW = new cMessage("TW");
        timerTH = new cMessage("TH");
        timerTC = new cMessage("TC");
        // timerTS = new cMessage("TS");
        timerts = new cMessage("ts");
        timerTM = new cMessage("TM");
        timerTF = new cMessage("TF");
        Random = new cMessage("random");
        buffer = new FAPhead();
        startTimer = new cMessage("start Timer");
        AreqTimer =  new cMessage("AREQ timer");
        startTime = par("startTime").doubleValue();
        timerTS= new cMessage("TS");

        irt = getModuleFromPar<IIpv4RoutingTable>(par("routingTableModule"), this);
        EV<<"APP INITSTAGE LOCAL"<<endl;
    }
    else if(stage == INITSTAGE_APPLICATION_LAYER)
    {
        Port = 1024;
        nodeCounter = 0;
        cModule *host = getContainingNode(this);
        selectedAddr.set("0.0.0.1");
        host->subscribe(l2AssociatedSignal, this);
        host->subscribe(interfaceDeletedSignal, this);
        socket.setCallback(this);
        socket.setOutputGate(gate("socketOut"));

        //clear record file
        if(getParentModule()->getId()==10)
        {
            std::string filename ="selectedip.txt";
            outfile.open(filename, std::ofstream::out);
            time_t now = time(0);
            outfile<<ctime(&now);
            outfile.close();

            num = cSimulation::getActiveSimulation()->getSystemModule()->par("numHosts").intValue();

            std::string filename2 ="/home/hiro/PycharmProjects/FAP/data/iniRecord/"+std::to_string(num)+"record.csv";
            outfile.open(filename2, std::ofstream::out);
//            time_t now = time(0);
            outfile<<ctime(&now);
            outfile.close();

            std::string filename3 ="record2.txt";
            outfile.open(filename3, std::ofstream::out);
//            time_t now = time(0);
            outfile<<ctime(&now);
            outfile.close();

            std::string filename4 ="/home/hiro/PycharmProjects/FAP/data/mergeRecord/"+std::to_string(num)+"merge.csv";
            outfile.open(filename4, std::ofstream::out);
//            time_t now = time(0);
            outfile<<ctime(&now);
            outfile.close();

            std::string filename5 ="/home/hiro/PycharmProjects/FAP/data/mergeRecord/"+std::to_string(num)+"merge_log.csv";
            outfile.open(filename5, std::ofstream::out);
//            time_t now = time(0);
            outfile<<ctime(&now);
            outfile.close();
        }

        // NodeStatus *nodeStatus = dynamic_cast<NodeStatus *>(host->getSubmodule("status"));

        // isOperational = (!nodeStatus) || nodeStatus->getState() == NodeStatus::UP;
        EV<<"operational is "<<isOperational<<endl;
        HostState = IDLE;
    }
}

void Fap::finish()
{
    cancelEvent(timerTL);
    cancelEvent(timerTP);
    cancelEvent(timerTW);
    // cancelEvent(timerTW2);
    cancelEvent(timerTH);
    cancelEvent(timerTC);
    cancelEvent(timerts);
    cancelEvent(timerTM);
    cancelEvent(timerTF);
    cancelEvent(startTimer);
    cancelEvent(AreqTimer);
    cancelEvent(timerTC);
    cancelEvent(timerTS);
//    std::string filename ="record2.txt";
//    outfile.open(filename, std::ofstream::app);
//    outfile<<simTime()<<endl;
//    outfile<< ie->getProtocolData<Ipv4InterfaceData>()->getIPAddress()<<endl;
//    outfile<<nodeCounter<<endl;
//    outfile<<endl;
//    outfile.close();
}

uint32_t Fap::generateIPADDR()
{
    uint32_t addr = ADDRPool.prefix.getInt();
    uint32 randNum = uniform(1,static_cast<int>(pow(2,(32-ADDRPool.prefixLength))));
    addr += randNum;
    return addr;
}


void Fap::handleMessageWhenUp(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        handleTimer(msg);
    }
    else if (msg->arrivedOn("socketIn")) {
        socket.processMessage(msg);
    }
    else
        throw cRuntimeError("Unknown incoming gate: '%s'", msg->getArrivalGate()->getFullName());
}

void Fap::socketDataArrived(UdpSocket *socket, Packet *packet)
{
    prossesFAPmessage(packet);
    delete packet;
}


void Fap::prossesFAPmessage(Packet *packet)
{
    ASSERT(isUp() && ie != nullptr);
    const auto& head =packet->popAtFront<FAPhead>();
    const auto& type = head->getFapType();

    switch(type){
        case HELLO :
        {  
            EV_INFO<<"Receive a HELLO message"<<endl;        
            if(HostState == WAIT && WaitLock == false)
            {
                cancelEvent(timerTL);
                WaitLock = true;
                Ipv4Address dest = head->getSource();
                sendAF(dest,1,0);
            }
            else if(HostState == STEADY)
            {
                 if(head->getSignature1() != getFilterSignature() && MergeLock==false)
                {
                    auto t1 = head->getSignature1();
                    auto t2 =getFilterSignature();
                    scheduleTP();
                    MergeLock=true;
                    Ipv4Address dest = head->getSource();
                    if(dest.getInt()<ie->getProtocolData<Ipv4InterfaceData>()->getIPAddress().getInt())
                        sendAF(dest,0,0);
                }
            }
            break;
        }

       case AREQ :
       {
//           cancelEvent(timerTW);
//           scheduleTW();
           EV_INFO<<"Receive an AREQ message"<<endl;
            if(HostState == WAIT)
            {
                ChangeToIni();
            }
            else if(HostState == INI )
            {
                cancelEvent(timerTL);

                auto ipAddr = head->getSource().getInt();
                /*ip address collision*/
                if(ipAddr == selectedAddr.getInt()&&head->getIDnumber()!=xID)
                {
                    cancelEvent(timerTC);
                    cancelEvent(AreqTimer);
                    scheduleTC();
                    selectedAddr.set("0.0.0.1");
                }
                /*record  the ip address has been selected*/
                else{
                    auto record = IpAddrRecord.find(ipAddr);
                    if(record!=IpAddrRecord.end()&&record->second!= head->getIDnumber())
                    {
                       IpAddrRecord.erase(ipAddr);
                    }
                    else if(record==IpAddrRecord.end())
                    {
                        // cancelEvent(timerTW);
                        // scheduleTW();
                        std::pair<uint32_t,unsigned int>  Item{ipAddr,head->getIDnumber()};
                        IpAddrRecord.insert(Item);
//                        sendAREQ(0,head->getSource(),head->getIDnumber());//flood AREQ message
                    }
                }
            }
            else if(HostState == STEADY){
                auto ipAddr = head->getSource().getInt();
                auto record = IpAddrRecord.find(ipAddr);
                cancelEvent(timerTS);
                scheduleAt(simTime()+TS, timerTS);
                if(record!=IpAddrRecord.end()&&record->second!= head->getIDnumber())
                {
                    IpAddrRecord.erase(ipAddr);
                }
                else if(record==IpAddrRecord.end())
                {
                    cancelEvent(timerTS);
                    scheduleAt(simTime()+TS, timerTS);
                    std::pair<uint32_t,unsigned int>  Item{ipAddr,head->getIDnumber()};
                    IpAddrRecord.insert(Item);
                    sendAREQ(0,head->getSource(),head->getIDnumber());
                }

            }
            else if(HostState == MERGE)
            {
                auto ipAddr = head->getSource().getInt();
                auto record = IpAddrRecord.find(ipAddr);
                if(record!=IpAddrRecord.end()&&record->second!= head->getIDnumber())
                {
                    IpAddrRecord.erase(ipAddr);
                }
                else
                {
                    cancelEvent(timerTS);
                    scheduleAt(simTime()+TS, timerTS);
                    std::pair<uint32_t,unsigned int>  Item{ipAddr,head->getIDnumber()};
                    IpAddrRecord.insert(Item);
                    sendAREQ(0,head->getSource(),head->getIDnumber());
                }
            }
            break;
       }

        case AF:
        {
            EV_INFO<<"Receive an AF message"<<endl;
            /*if host's state is WAIT, no mater whether it has sent AF, take the same process*/
            if(HostState == WAIT && head->getI() && head->getR())
            {
                HostState = INI;
                nodeCounter=head->getCounter()+1;
                UpdateFilter(head->getFilterM(),head->getFilterK(),head->getFilterValue());
                ChangeToIni();
            }
            else if(head->getI()&&!head->getR())
            {
                sendAF(Ipv4Address::ALLONES_ADDRESS,1,1);
            }
            else if(HostState == STEADY && head->getSignature1() != getFilterSignature()&&!head->getI())
            {

                unsigned int temp1 = head->getSignature1();
                unsigned int temp2 = getFilterSignature();

                buffer->setFilterValue(head->getFilterValue());
                buffer->setFilterM(head->getFilterM());
                buffer->setFilterK(head->getFilterK());
                buffer->setSignature1(head->getSignature1());
                buffer->setCounter(head->getCounter());
                
                
                if(!head->getR())
                {
                    HostState = MERGE;
                    cancelEvent(timerTH);
                    sendAF(Ipv4Address::ALLONES_ADDRESS,0,1);
                    // scheduleTF();
                    if(head->getCounter()>nodeCounter)
                    {
                        sendPartition(0,xID,head->getFilterValue(),head->getFilterM(),head->getFilterK(),head->getSignature1(),head->getCounter());
                        buffer->setM(0);
                    }
                    else
                    {
                        sendPartition(1,xID,head->getFilterValue(),head->getFilterM(),head->getFilterK(),head->getSignature1(),head->getCounter());
                        buffer->setM(1);
                    }

                    BloomFilter<uint32_t> f2(head->getFilterM(),head->getFilterK());
                    f2.convert(head->getFilterValue());
                    filter = filter+f2;
                    scheduleTM();
                    nodeCounter += head->getCounter();

                    merge_record();
                }

                else 
                {
                    HostState = MERGE;
                    cancelEvent(timerTH);

                    if(head->getCounter()>=nodeCounter)
                    {
                        sendPartition(0,xID,head->getFilterValue(),head->getFilterM(),head->getFilterK(),head->getSignature1(),head->getCounter());
                        buffer->setM(0);
                    }
                    else
                    {
                        sendPartition(1,xID,head->getFilterValue(),head->getFilterM(),head->getFilterK(),head->getSignature1(),head->getCounter());
                        buffer->setM(1);
                    }

                    BloomFilter<uint32_t> f2(head->getFilterM(),head->getFilterK());
                    f2.convert(head->getFilterValue());
                    filter = filter+f2;
                    scheduleTM();
                    nodeCounter += head->getCounter(); 

                    merge_record();                
                }
//                scheduleTF();
            }
            else {
                return;
            }
            break;
        }

        case PARTITION:
        {
            EV_INFO<<"Receive a PARTITION message"<<endl;
            if(HostState == STEADY&& head->getSignature1() == getFilterSignature() && FilterLock==false)
            {
                if(FilterLock==true)
                {
                    ;
                }
                HostState = MERGE;
                scheduleTP();
                FilterLock=true;
                cancelEvent(timerTH);
                scheduleTM();
                nodeCounter += head->getCounter();
                if(head->getM()==0)
                {
                    BloomFilter<uint32_t> f2(head->getFilterM(),head->getFilterK());
                    f2.convert(static_cast<std::string>(head->getFilterValue()));
                    filter = filter+f2;
//                    nodeCounter += head->getCounter();
                    if(f2.Check(selectedAddr.getInt()))
                    {
                        HostState = INI;
                        InitialConfigure2();
                    }
                }
                else
                {
                    BloomFilter<uint32_t> f2(head->getFilterM(),head->getFilterK());
                    f2.convert(static_cast<std::string>(head->getFilterValue()));
                    filter = filter+f2;
                }
                merge_record();
            }
            break;
        }
    }
}

bool Fap::checkIdentical(std::deque<std::shared_ptr< BloomFilter<uint32_t>>> Q,unsigned int Sig)
{
    for(auto iter = Q.begin();iter != Q.end();iter++)
    {
        if((*iter)->getSignature()==Sig)
            return true;
    }
    return false;
}

void Fap::socketClosed(UdpSocket *socket_)
{
     if (operationalState == State::STOPPING_OPERATION && !socket.isOpen())
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void Fap::openSocket()
{
    if(!ie)
        throw cRuntimeError("Interface to listen does not exist. aborting");
    
    socket.bind(Port);
    socket.setBroadcast(true);
    socket.setTimeToLive(1);
    EV_INFO << "Fap bound to port " << Port << endl;
}

bool Fap::CheckFilterSignature(unsigned int S)
{
    return getFilterSignature()==S;
}

void Fap::ChooseIpAddr()
{
    selectedAddr.set(generateIPADDR());
    while(IpAddrRecord.find(selectedAddr.getInt())!=IpAddrRecord.end()|| filter.Check(selectedAddr.getInt()))
    {
        selectedAddr.set(generateIPADDR());
    }
}

void Fap::SetIpAddr(uint32_t addr)
{
    ie->getProtocolData<Ipv4InterfaceData>()->setIPAddress(Ipv4Address(addr));
    ie->getProtocolData<Ipv4InterfaceData>()->setNetmask(ADDRPool.prefix);
   
    insertADDR(addr);
    irt->setRouterId(Ipv4Address(addr));
    std::string banner = "Got IP " + selectedAddr.str();
    EV<<banner<<endl;

//    Ipv4Route *iroute = nullptr;
//    for (int i = 0; i < irt->getNumRoutes(); i++) {
//        Ipv4Route *e = irt->getRoute(i);
//        iroute=e;
//    }

    EV_INFO << "The requested IP addr is assigned successfully" <<endl;
}

void Fap::UpdateFilter(int m,int k,std::string f)
{
    filter.setK(k);
    filter.ResizeFilter(m);
    filter.convert(f);
}

InterfaceEntry *Fap::chooseInterface()
{
    std::string name=par("interfaceTableModule");
    IInterfaceTable *ift = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);
    IInterfaceTable *test = ift;
    const char *interfaceName = par("interface");
    InterfaceEntry *ie = nullptr;
    std::cout<<*interfaceName<<endl;
    if (strlen(interfaceName) > 0) {
        ie = ift->getInterfaceByName(interfaceName);
        if (ie == nullptr)
            throw cRuntimeError("Interface \"%s\" does not exist", interfaceName);
    }
    else {
        // there should be exactly one non-loopback interface that we want to configure
        for (int i = 0; i < ift->getNumInterfaces(); i++) {
            InterfaceEntry *current = ift->getInterface(i);
            if (!current->isLoopback()) {
                if (ie)
                    throw cRuntimeError("Multiple non-loopback interfaces found, please select explicitly which one you want to configure via DHCP");
                ie = current;
            }
        }
        if (!ie)
            throw cRuntimeError("No non-loopback interface found to be configured via DHCP");
    }

/*    if (!ie->ipv4Data()->getIPAddress().isUnspecified())
        throw cRuntimeError("Refusing to start DHCP on interface \"%s\" that already has an IP address", ie->getInterfaceName());*/
    return ie;
}

void Fap::handleTimer(cMessage *msg)
{
    if(msg == timerTL){

        ChangeToIni();
    }
    else if(msg == timerTP)
    {
//        scheduleTP();
        MergeLock = false;
        FilterLock=false;
    }
    else if(msg == timerTW)
    {
        SetIpAddr(selectedAddr.getInt());//set ip address

        num = cSimulation::getActiveSimulation()->getSystemModule()->par("numHosts").intValue();
        std::string filename ="/home/hiro/PycharmProjects/FAP/data/iniRecord/"+std::to_string(num)+"record.csv";
        outfile.open(filename, std::ofstream::app);
        outfile<<simTime()<<","<< ie->getProtocolData<Ipv4InterfaceData>()->getIPAddress()<<",";
//        unsigned int temp = getFilterSignature();
//        outfile<<xID<<endl;
//        outfile<<"IPmap is: "<<endl;
        for(auto map_i = IpAddrRecord.cbegin();map_i != IpAddrRecord.cend();map_i++)
        {
            insertADDR(map_i->first);
//            Ipv4Address temp(map_i->first);
//            outfile<<temp<<"    "<<map_i->second<<endl;
            nodeCounter++;              
        }


        outfile<<nodeCounter<<","<<filter.getSignature()<<endl;

        outfile.close();
        EV_INFO<<"init state over, the network has "<<nodeCounter<<" nodes"<<endl;

        HostState = STEADY;
        IpAddrRecord.clear();
        scheduleTH();
    }
    else if(msg == timerTH)
    {
        if(HostState == STEADY)
        {
            scheduleTH();
            sendHello();
        }
        else cancelEvent(timerTH);
    }
    else if(msg == timerTC)
    {
        cancelEvent(timerTC);
        cancelEvent(AreqTimer);
        cancelEvent(timerTW);
        ChooseIpAddr();
        if(selectedAddr.getInt()==1)
            EV<<"erro"<<endl;
        scheduleAt(simTime(),AreqTimer);
        scheduleTW();
    }
    else if(msg == timerTS)
    {
        for(auto map_i = IpAddrRecord.begin();map_i != IpAddrRecord.end();map_i++)
        {
            insertADDR(map_i->first);
            nodeCounter++;                
        }
        IpAddrRecord.clear();
    }
    else if(msg == timerTM)
    {

        for(auto map_i = IpAddrRecord.cbegin();map_i != IpAddrRecord.cend();map_i++)
        {
            insertADDR(map_i->first);            
        }
        // outfile1.close();
        num = cSimulation::getActiveSimulation()->getSystemModule()->par("numHosts").intValue();
        std::string filename ="/home/hiro/PycharmProjects/FAP/data/mergeRecord/"+std::to_string(num)+"merge.csv";
        outfile.open(filename, std::ofstream::app);
        outfile<<simTime()<<","<<getParentModule()->getId()<<","<< ie->getProtocolData<Ipv4InterfaceData>()->getIPAddress()<<",";
        unsigned int temp = getFilterSignature();
        outfile<<nodeCounter<<","<<filter<<","<<filter.getSignature();
        outfile<<endl;
        outfile.close();

        EV_INFO<<"init state over, the network has "<<nodeCounter<<" nodes"<<endl;
        HostState = STEADY;
        IpAddrRecord.clear();
        scheduleTH();
    }
    else if(msg == timerTF)
    {
        NT--;
        if(NT>=0){
            simtime_t PartitionTime = uniform(0,0.3);
            scheduleAt(simTime()+PartitionTime,timerTF);           
//            sendPartition(buffer->getM(),xID,buffer->getFilterValue(),buffer->getFilterM(),buffer->getFilterK();buffer->getFilterSignature1(),buffer->getCounter());
        }
    }     
    else if(msg == startTimer){
        cancelEvent(startTimer);
        openSocket();
        scheduleAt(simTime()+TL,timerTL);
        HostState = WAIT;
    }
    else if(msg == AreqTimer)
    {
        NF--;
        if(NF>=0){
            AreqTime = uniform(0,0.4);
            scheduleAt(simTime()+AreqTime,AreqTimer);
            if(selectedAddr.getInt()==1)
                EV<<"erro"<<endl;
            sendAREQ(0);
        }       
    }
    else if(msg == timerTS){
        for(auto map_i = IpAddrRecord.cbegin();map_i != IpAddrRecord.cend();map_i++)
        {
            insertADDR(map_i->first);
            nodeCounter++;              
        }
    } 
}

void Fap::InitialConfigure()
{
    ChooseIpAddr();
    std::string filename ="selectedip.txt";
    outfile.open(filename, std::ofstream::app);
    outfile<< selectedAddr<<endl;
    outfile.close();
    scheduleAt(simTime(),AreqTimer);
    scheduleTW();
}

void Fap::InitialConfigure2()
{
    ChooseIpAddr();
    std::string filename ="selectedip.txt";
    outfile.open(filename, std::ofstream::app);
    outfile<< selectedAddr<<endl;
    outfile.close();
    scheduleAt(simTime(),AreqTimer);
    
}

unsigned int Fap::getFilterSignature()
{

    return filter.getSignature();
}

void Fap::ChangeToIni()
{
    HostState = INI;
    InitialConfigure();
}

const char *Fap::getStateName(hostState state)
{
    switch (state) {
#define CASE(X)    case X: \
        return #X;
        CASE(IDLE);
        CASE(WAIT);
        CASE(INI);
        CASE(STEADY);
        CASE(MERGE);
        /*CASE(SELECTING);
        CASE(REQUESTING);
        CASE(BOUND);
        CASE(RENEWING);
        CASE(REBINDING);*/

        default:
            return "???";
#undef CASE
    }
}

bool Fap::checkRepetition(uint32_t addr)
{
    return filter.Check(addr);
}

void Fap::insertADDR(uint32_t addr)
{
    filter.Insert(addr);
}

void Fap::receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject *details)
{
    Enter_Method_Silent();
    printSignalBanner(signalID, obj, details);

    // host associated. link is up. change the state to init.
    if (signalID == l2AssociatedSignal) {
        InterfaceEntry *associatedIE = check_and_cast_nullable<InterfaceEntry *>(obj);
        if (associatedIE && ie == associatedIE /*&& hostState != IDLE*/) {
            EV_INFO << "Interface associated, starting Fap." << endl;
            
        }
    }
    else if (signalID == interfaceDeletedSignal) {
        if (isUp())
            throw cRuntimeError("Reacting to interface deletions is not implemented in this module");
    }
}

void Fap::generateXID()
{
    xID = intuniform(0, RAND_MAX);
}

void Fap::sendToUdp(Packet *msg, int srcPort, const L3Address& destAddr, int destPort)
{
    EV_INFO << "Sending packet: " << msg << "." << endl;
    msg->addTagIfAbsent<InterfaceReq>()->setInterfaceId(ie->getInterfaceId());
    socket.sendTo(msg, destAddr, destPort);
}

void Fap::sendHello()
{
    Packet *packet = new Packet("Fap.HELLO");
    const auto& hello = makeShared<FAPhead>();

    hello->setChunkLength(B(20));
    hello->setSource(selectedAddr);
    unsigned int temp = getFilterSignature();
    hello->setSignature1(getFilterSignature());
    hello->setFapType(HELLO);
    packet->insertAtBack(hello);
    
//    cModule *host = getContainingNode(this);
//    IMobility *mobility = check_and_cast<IMobility *>(host->getSubmodule("mobility"));
//    auto position = mobility->getCurrentPosition();
//    auto velocity = mobility->getCurrentVelocity();

    EV_INFO << "Sending HELLO." << endl;
    sendToUdp(packet,Port,Ipv4Address::ALLONES_ADDRESS,Port);
}

void Fap::sendAREQ(bool R)
{
    Packet *packet = new Packet("Fap.AREQ");
    const auto& areq = makeShared<FAPhead>();
    areq->setChunkLength(B(100));
    areq->setR(R);
    areq->setSource(selectedAddr);
    areq->setIDnumber(xID);
//    areq->setSignature1(getFilterSignature());
    areq->setFapType(AREQ);
    packet->insertAtBack(areq);
    

    EV_INFO << "Sending AREQ." << endl;
    sendToUdp(packet,Port,Ipv4Address::ALLONES_ADDRESS,Port);
}

void Fap::sendAREQ(bool R, Ipv4Address seletct, unsigned int ID)
{
    Packet *packet = new Packet("Fap.AREQ");
    const auto& areq = makeShared<FAPhead>();
    areq->setChunkLength(B(100));
    areq->setR(R);
    areq->setSource(seletct);
    areq->setIDnumber(ID);
//    areq->setSignature1(getFilterSignature());
    areq->setFapType(AREQ);
    packet->insertAtBack(areq);
    

    EV_INFO << "Forwarding AREQ." << endl;
    sendToUdp(packet,Port,Ipv4Address::ALLONES_ADDRESS,Port);
}

void Fap::sendAF(Ipv4Address dest,bool I,bool R)
{
    Packet *packet = new Packet("Fap.AF");
    const auto& af = makeShared<FAPhead>();
    std::cout<<dest<<endl;
    af->setI(I);
    af->setR(R);
    af->setChunkLength(B(100));
    af->setCounter(nodeCounter);
    af->setSource(selectedAddr);
    af->setDest(dest);
    if(R==0&&I==1);
    else
    {
        af->setFilterValue(filter.invert().c_str());
        af->setFilterM(filter.getM());
        af->setFilterK(filter.getK());
        af->setSignature1(getFilterSignature());
    }    
    af->setFapType(AF);
   
    packet->insertAtBack(af);

    EV_INFO << "Sending AF." << endl;
    sendToUdp(packet,Port,Ipv4Address::ALLONES_ADDRESS,Port);
}

void Fap::sendPartition(bool m, int seq, std::string value, int filter_m, int filter_k, unsigned int filter_sig,unsigned counter)
{
    Packet *packet = new Packet("Fap.PARTITION");
    const auto& partition = makeShared<FAPhead>();
    partition->setFapType(PARTITION);
    partition->setCounter(counter);
    partition->setM(m);
    partition->setSeq(seq);
    partition->setSource(selectedAddr);
    partition->setFilterValue(value.c_str());
    partition->setFilterM(filter_m);
    partition->setFilterK(filter_k);
    partition->setSignature1(getFilterSignature());
    partition->setSignature2(filter_sig);
    partition->setChunkLength(B(100));

    packet->insertAtBack(partition);
    EV_INFO << "Sending PARTITION." << endl;
    sendToUdp(packet,Port,Ipv4Address::ALLONES_ADDRESS,Port);
}


void Fap::socketErrorArrived(UdpSocket *socket, Indication *indication)
{
    EV_WARN << "Ignoring UDP error report " << indication->getName() << endl;
    delete indication;
}




void Fap::handleStopOperation(LifecycleOperation *operation)
{
    cancelEvent(timerTL);
    cancelEvent(timerTP);
    cancelEvent(timerTW);
    cancelEvent(timerTH);
    cancelEvent(timerTC);

    cancelEvent(timerts);
    cancelEvent(timerTM);
    cancelEvent(timerTF);
    ie = nullptr;
    socket.close();
    delayActiveOperationFinish(par("stopOperationTimeout"));
}

void Fap::handleStartOperation(LifecycleOperation *operation)
{
    simtime_t start = std::max(startTime, simTime());
    ie = chooseInterface();
    scheduleAt(start, startTimer);
    // openSocket();
//    generateXID();
    xID= this->getParentModule()->getIndex();
    macAddress = ie->getMacAddress();    
}

void Fap::handleCrashOperation(LifecycleOperation *operation)
{
    cancelEvent(timerTL);
    cancelEvent(timerTP);
    cancelEvent(timerTW);
    cancelEvent(timerTH);
    cancelEvent(timerTC);
    cancelEvent(timerts);
    cancelEvent(timerTM);
    cancelEvent(timerTF);
    ie = nullptr;

    if (operation->getRootModule() != getContainingNode(this))     // closes socket when the application crashed only
        socket.destroy();   
}

void Fap::merge_record()
{
    num = cSimulation::getActiveSimulation()->getSystemModule()->par("numHosts").intValue();
    std::string filename ="/home/hiro/PycharmProjects/FAP/data/mergeRecord/"+std::to_string(num)+"merge_log.csv";
    outfile.open(filename, std::ofstream::app);
    outfile<<simTime()<<","<<getParentModule()->getId()<<","<< ie->getProtocolData<Ipv4InterfaceData>()->getIPAddress()<<",";
    unsigned int temp = getFilterSignature();
    outfile<<nodeCounter<<","<<filter<<","<<filter.getSignature();
    outfile<<endl;
    outfile.close();
}

void Fap::scheduleTL(){cancelEvent(timerTL);scheduleAt(simTime()+TL,timerTL);}
void Fap::scheduleTP(){cancelEvent(timerTP); scheduleAt(simTime()+TP,timerTP);}
void Fap::scheduleTW(){cancelEvent(timerTW);scheduleAt(simTime()+TW,timerTW);}
// void Fap::scheduleTW(){scheduleAt(simTime()+TW,timerTW2);}
void Fap::scheduleTM(){cancelEvent(timerTM);scheduleAt(simTime()+TM,timerTM);}
void Fap::scheduleTH(){
    cancelEvent(timerTH);
    simtime_t x = uniform(TH-0.5,TH+0.5);
    scheduleAt(simTime()+x,timerTH);
    }
void Fap::scheduleTC(){cancelEvent(timerTC);scheduleAt(simTime()+TC,timerTC);}
// void Fap::scheduleTS(){scheduleAt(TS,timerTS);}
void Fap::schedulets(){cancelEvent(timerts);scheduleAt(simTime()+ts,timerts);}
void Fap::scheduleTF(){cancelEvent(timerTF);scheduleAt(simTime()+TF,timerTF);}
void Fap::scheduleRandom()
{
    cancelEvent(Random);
    simtime_t r = uniform(0,0.2);
    scheduleAt(simTime()+r,Random);
}


} //namespace
