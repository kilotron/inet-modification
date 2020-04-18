/*
 * color.cc
 *
 *  Created on: 2019年12月27日
 *      Author: hiro
 */

#include <iostream>
#include <algorithm>

#include "inet/applications/common/SocketTag_m.h"
#include "inet/networklayer/icn/color/color.h"
#include "inet/common/bloomfilter/hash/MurmurHash3.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/networklayer/icn/color/Data_m.h"
#include "inet/networklayer/icn/color/Get_m.h"
#include "inet/networklayer/icn/color/AppData_m.h"
#include "inet/networklayer/common/L3Tools.h"
#include "inet/networklayer/contract/color/ColorSocketCommand_m.h"
#include "inet/networklayer/common/SidTag_m.h"
#include "inet/networklayer/common/PitTag_m.h"

namespace inet
{

using std::string;
Define_Module(colorCluster);

void colorCluster::SimRecorder::ConsumerPrint(std::ostream &os)
{

    os << "index:    " << index << endl;
    os << "sendNum: " << GetSendNum << endl;
    os << "recvNum: " << DataRecvNum << endl;
    if (GetSendNum != 0)
        os << "trans ratio: " << 100 * DataRecvNum / GetSendNum << "%" << endl;
    os << "send Interval: " << owner->sentInterval << "s" << endl;
    os << "Throughput: " << throughput.get() * 8 / ((simTime().dbl() - owner->testget.dbl()) * 1000 * 1000) << " Mbps" << endl;
    os << "Average delay: ";
    double sum = 0;
    if(delayArray.size()>0)
    {
        std::for_each(delayArray.begin(), delayArray.end(), [&sum](double value) { sum += value; });
        os << sum / delayArray.size() << " ms" << endl;
    }
    else
        os << 0 << "ms" << endl;

    os << endl;
}

void colorCluster::SimRecorder::ProviderPrint(std::ostream &os)
{
    os << "index:    " << index << endl;
    os << "DataSendNum: " << DataSendNum << endl;
    os << "GetRecvNum: " << GetRecvNum << endl;
    os << endl;
}

colorCluster::~colorCluster()
{
}

void colorCluster::finish()
{

    record();
//    ct->clear();
    cancelAndDelete(testGet);
    cancelAndDelete(testData);
    cancelAndDelete(delayer24);

}

void colorCluster::initialize(int stage)
{
    OperationalBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL)
    {
        nodeIndex = getParentModule()->getParentModule()->getIndex();
        std::array<uint64_t, 2> hashValue;
        MurmurHash3_x64_128(&nodeIndex, sizeof(int), 0, hashValue.data());
        nid.setNID(hashValue);
        nid.test = nodeIndex;

        testTimer = nullptr;
        testGet = new cMessage("testGet");
        testData = new cMessage("testData");

        testget = 2 + 1.0 / (nodeIndex % 10 + 1);
        testdata = 1;

        testModule.owner = this;
        testModule.index = nodeIndex;
        testModule.multiConsumer = par("multi").intValue();


        ResemBuffer = new ColorFragBuf();
        //得到指向转发表的指针
        auto name = getParentModule()->getParentModule()->getFullPath();
        name = name + ".interfaceTable";
        auto path = name.c_str();
        cModule *mod = this->getModuleByPath(path);
        ift = dynamic_cast<IInterfaceTable *>(mod);

        //得到指向路由表的指针
        name = getParentModule()->getFullPath();
        name = name + ".routingTable";
        path = name.c_str();
        mod = this->getModuleByPath(path);
        rt = dynamic_cast<ColorRoutingTable *>(mod);

        //得到指向缓存表的指针
        name = getParentModule()->getFullPath();
        name = name + ".CacheTable";
        path = name.c_str();
        mod = this->getModuleByPath(path);
        ct = dynamic_cast<ColorCacheTable *>(mod);

        //得到指向缓存表的指针
        name = getParentModule()->getFullPath();
        name = name + ".PendingTable";
        path = name.c_str();
        mod = this->getModuleByPath(path);
        pit = dynamic_cast<colorPendingGetTable *>(mod);

        //得到指向分簇模块的指针
        name = getParentModule()->getParentModule()->getFullPath();
        name = name + ".cluster";
        path = name.c_str();
        mod = this->getModuleByPath(path);
        clusterModule = dynamic_cast<ICluster *>(mod);

        mtu = par("mtu").intValue();
        hopLimit = par("hopLimit").intValue();

        getDelayTime = par("getDelayTime").doubleValue();
        dataDelayTime = par("dataDelayTime").doubleValue();
        TC = par("TC").intValue();

        getProdelay = par("GetProcDelay").doubleValue();
        dataProdelay = par("DataProcDelay").doubleValue();

        delayer24 = new cMessage("delayer24");
        delay_queue24.setTimerAndOwner(delayer24, this);


        flood = par("flood").boolValue();
        unicast = par("unicast").boolValue();
        routeLifeTime = par("routeLifeTime").doubleValue();

        registerService(Protocol::color, gate("transportIn"), gate("queueIn"));
        registerProtocol(Protocol::color, gate("queueOut"), gate("transportOut"));
    }
}

InterfaceEntry *colorCluster::chooseInterface(const char *interfaceName)
{
    //得到指向转发表的指针
    InterfaceEntry *ie = nullptr;
    if (strlen(interfaceName) > 0)
    {
        ie = ift->findInterfaceByName(interfaceName);
        if (ie == nullptr)
            throw cRuntimeError("Interface \"%s\" does not exist", interfaceName);
    }
    else
    {
        // there should be exactly one non-loopback interface that we want to configure
        for (int i = 0; i < ift->getNumInterfaces(); i++)
        {
            InterfaceEntry *current = ift->getInterface(i);
            if (!current->isLoopback())
            {
                if (ie)
                    throw cRuntimeError("Multiple non-loopback interfaces found");
                ie = current;
            }
        }
        if (!ie)
            throw cRuntimeError("No non-loopback interface");
    }

    return ie;
}

void colorCluster::handleRegisterService(const Protocol &protocol, cGate *out, ServicePrimitive servicePrimitive)
{
    Enter_Method("handleRegisterService");
}

void colorCluster::handleRegisterProtocol(const Protocol &protocol, cGate *in, ServicePrimitive servicePrimitive)
{
    //对上层协议提供注册服务
    Enter_Method("handleRegisterProtocol");
    if (in->isName("transportIn"))
        upperProtocols.insert(&protocol);
}

void colorCluster::refreshDisplay() const
{
    OperationalBase::refreshDisplay();
}

void colorCluster::handleStartOperation(LifecycleOperation *operation)
{
    //默认wlan0是2.4GHz， wlan1是5GHz
    ie = chooseInterface("wlan0");


    //测试，一个节点作为内容源一个作为请求者发送get包
    if (testModule.multiConsumer == 1)
    {
        if (nodeIndex % Cindex == 0)
            scheduleAt(testget, testGet);

    }
    else if (testModule.multiConsumer == 0)
    {
        if (nodeIndex == Cindex)
            scheduleAt(testget, testGet);

    }
    else if (testModule.multiConsumer == 2)
    {
        // std::cout << "sch" << endl;
        if (nodeIndex - Cindex >= 0 && nodeIndex - Cindex <= 3)
        {
            scheduleAt(testget, testGet);
        }

    }
}

void colorCluster::handleStopOperation(LifecycleOperation *operation)
{
    // TODO: stop should send and wait pending packets
    cancelAndDelete(testGet);
    cancelAndDelete(testData);
}

void colorCluster::handleCrashOperation(LifecycleOperation *operation)
{
    cancelAndDelete(testGet);
    cancelAndDelete(testData);
}

void colorCluster::handleMessageWhenUp(cMessage *msg)
{
    //数据包来自上层
    if (msg->arrivedOn("transportIn"))
    {
        if (auto request = dynamic_cast<Request *>(msg))
            handleRequest(request);
        else handlePacketFromHL(check_and_cast<Packet *>(msg));
    }

    //数据包来自链路层
    else if (msg->arrivedOn("queueIn"))
    {
        EV_INFO << "Received " << msg << " from network.\n";
        handleIncomingDatagram(check_and_cast<Packet *>(msg));
    }

    //自消息，定时器
    else if (msg->isSelfMessage())
    {
        
        if (msg == delayer24)
        {
            //表明延迟发送的报文计时器计时完成
            auto packet = delay_queue24.popAtFront();
            if (packet != nullptr)
            {
                // std::cout << nodeIndex << "  delay forward(intercluster) at  " << simTime() << "     " << endl;
                sendDatagramToOutput(packet->dup(), 24);
                delete packet;
            }
        }
    }
    else
        throw cRuntimeError("message arrived on unknown gate '%s'", msg->getArrivalGate()->getName());
}

void colorCluster::handleIncomingDatagram(Packet *packet)
{

    //对链路层传来的数据包进行操作，先取得数据包的头部
    const auto &head = packet->peekAtFront<Chunk>(B(78), 0);
    auto pointer = head.get();

    //转换为raw pointer
    Chunk *chunk = const_cast<Chunk *>(pointer);

    //通过动态类型转换判断数据包的类型
    if (dynamic_cast<Data *>(chunk))
    {
        //处理data包
        handleDataPacket(packet);
    }
    else if (dynamic_cast<Get *>(chunk))
    {
        //处理get包
        handleGetPacket(packet);
    }
    else
    {
        throw cRuntimeError("unknow message type");
    }
}

void colorCluster::handleDataPacket(Packet *packet)
{
    //取出头部
    const auto head = packet->peekAtFront<Data>();
    const SID &headSid = head->getSid();
    auto ie = getSourceInterface(packet);



    //mac信息
    auto macInfo = packet->getTag<MacAddressInd>();

    // std::cout << "data received, index: " << getParentModule()->getParentModule()->getIndex() << endl;
    //测试信息
    // if (nodeIndex == Cindex)
    // {
    // }

    auto newPacket = packet->dup();
    //检查是否是请求者
    if (pit->isConsumer(headSid))
    {
        //调试信息
        std::cout << "one packet received" << endl;
        std::cout << "off set is " << head->getOffset() << endl;

        std::for_each(head->getTrace().begin(), head->getTrace().end(), [](const int &n) { std::cout << n << "->"; });
        std::cout << nodeIndex << endl;

        //如果本节点正是发出get包的源节点在进行数据包重组验证完整性后，解包给上层协议
        auto fullPacket = ResemBuffer->addFragment(packet->dup(), simTime());
        if (fullPacket != nullptr)
        {
            auto dataHead = fullPacket->peekAtFront<Data>();
            auto localPort = dataHead->getPortNumber2();
               
            decapsulate(fullPacket, headSid);

            // 该数据包对应的SID
            auto sidInd = fullPacket->addTag<SidInd>();
            sidInd->setSid(headSid);
            std::cout << "in HandleData:CI=" << (head->getCongestionIndication()) << endl;

            auto sd = socketsByPortMap.find(localPort);
            if(sd == socketsByPortMap.end())
            {
                throw cRuntimeError("No socket on that local port");
            }
            else
            {
                sendUp(fullPacket,sd->second);
            }
            
        }

        //首先移除PIT表中consumer的对应表项， PIT表项仅为consumer时不触发转发
        auto range = findPITentry(headSid);
        auto iter = range.first;
        while (iter != range.second)
        {
            if (iter->second.isConsumer())
            {
                pit->RemoveEntry(iter++);
            }
            else
                iter++;
        }
    }

    createRoute(head->getLastHop(), head->getLastHop(), macInfo->getSrcAddress(), routeLifeTime, 24, head->getTimeToLive());
    if (head->getComeFromSource())
    {

        createRoute(head->getSid().getNidHead(), head->getLastHop(), macInfo->getSrcAddress(), routeLifeTime, 24, head->getTimeToLive());
            

    }
//    if( head->getRouteMetric() !=0 )
//    {
//        createRoute(head->getSid().getNidHead(), head->getLastHop(), macInfo->getSrcAddress(), head->getRouteLifetime(), 24, head->getRouteMetric());
//    }

    //查看PIT表是否有此SID记录
    if (pit->hasThisSid(headSid))
    {
        // std::cout << "data received, PIT hit, index: " << nodeIndex << " at " << simTime() << endl;

        //转发原则与NDN相同，get包从哪个端口来，data包就往哪个端口回
        //指示data包应该发往那个端口，hz5 == true发往5GHz端口，hz24 == true发往2.4GHz端口
        bool hz5 = false;
        bool hz24 = false;
        MacAddress nexthop;
        //PIT表中一个SID可能对应多条表项
        auto range = findPITentry(headSid);
        for (auto iter = range.first; iter != range.second; iter++)
        {
            nexthop = iter->second.getMac();
            auto route = rt->findRoute(iter->second.getNid());
            if(route != nullptr)
                nexthop = route->getNextMac();
                
            if (iter->second.getType() == 24)
            {
                //来自2.4GHz端口
                hz24 = true;
                continue;
            }
            if (iter->second.getType() == 5)
            {
                //来自5GHz端口
                hz5 = true;
            }
        }
        if (isForwarder() && !ct->hasThisPacket(packet, headSid))
        {
            //测试信息
            //                        std::cout << "data received, index: " << getParentModule()->getParentModule()->getIndex() << endl;
            
            //移除padding
            if (head->getTotalLength() < newPacket->getDataLength())
            {
                newPacket->setBackOffset(newPacket->getFrontOffset() + head->getTotalLength());
            }

            //处理数据包
            newPacket->trim();
            const auto &newHead = newPacket->removeAtFront<Data>();

            //用作缓存的数据包，清空trace,重置TTL
            auto CacheHead = makeShared<Data>(*newHead->dup());
            CacheHead->setTimeToLive(8);
            CacheHead->getTraceForUpdate().clear();
            CacheHead->getTraceForUpdate().push_back(nodeIndex);
            CacheHead->setComeFromSource(false);
            CacheHead->setRouteMetric(0);

            newPacket->clearTags();
            //            newPacket->trim();
            //TTL-1， 测试trace加入本节点
            newHead->setTimeToLive(newHead->getTimeToLive() - 1);
            newHead->setLastHop(nid);
            auto metric = newHead->getRouteMetric();
            if(metric != 0)
                newHead->setRouteMetric(metric - 1);
            auto trace = newHead->getTrace();
            newHead.get()->getTraceForUpdate().push_back(nodeIndex);

            //            std::for_each(newHead->getTrace().begin(), newHead->getTrace().end(), [](const int &n) { std::cout << n << "->"; });
            //            std::cout << endl;
            //重新插入头部
            auto CachePacket = newPacket->dup();
            CachePacket->insertAtFront(CacheHead);

            newPacket->insertAtFront(newHead);

            //debug检查长度是否和复制前的packet一致
            //            auto ll2 = newPacket->getDataLength().get() / 8;

            //            testpacket->insertAtFront(newHead);
            //将数据包存入缓存
            auto block = findContentInCache(headSid);
            if (block == nullptr)
            {
                block = ct->CreateBlock(headSid);
            }
            block->InsterPacket(CachePacket);

            //确定数据包的回传端口
            if (hz24 == 1)
            {
                //数据包发往2.4GHz端口
                //                if(nodeIndex!=43)
                // if (delay_queue24.check_and_decrease(packet) == false)
                //     delay_queue24.insert(newPacket->dup(), DATA, simTime() + uniform(0, dataDelayTime), TC);
                sendDatagramToOutput(newPacket->dup(),24,nexthop);
            }


            //在一个get包就对应一个data包的情况中，缓存转发后移除PIT条目
            pit->RemoveEntry(headSid);
//            delay_queue24.cancelDelayeForwarding(headSid);
        }
    }
    else
    {
        //PIT中没有对应条目时不做任何操作
    }

    //删除两个数据包，避免内存泄露
    delete packet;
    delete newPacket;
}

void colorCluster::handleGetPacket(Packet *packet)
{
    //根据GET包头部内容进行相应操作
    packet->trim();

    //mac地址信息
    auto macInfo = packet->getTag<MacAddressInd>();
    //取出头部
    const auto &head = packet->removeAtFront<Get>();
    auto ie = getSourceInterface(packet);
    //信号强度，之后用作设置延时定时器

    auto headSID = head->getSid();
    auto isLastHopHead = head->getCluster();
    auto port = head->getPortNumber1();

    //检查ttl，ttl小于等于0丢弃
    if (head->getTimeToLive() < 0)
    {
        delete packet;
        return;
    }

    //簇头可以插入两个端口的路由信息，簇成员由于在2.4GHz上只侦听不发送，所以只在路由表中只插入来自5GHz端口的信息
    //检查
    if (isForwarder())
    {
        //测试信息
         std::cout << nodeIndex << "  " << simTime() << "     ";
         std::for_each(head->getTrace().begin(), head->getTrace().end(), [](const int &n) { std::cout << n << "->"; });
         std::cout << endl;


      
        createRoute(head->getSource(), head->getLastHop(), macInfo->getSrcAddress(), routeLifeTime, 24, head->getTimeToLive());
        createRoute(head->getLastHop(), head->getLastHop(), macInfo->getSrcAddress(), routeLifeTime, 24, head->getTimeToLive());


    }

    auto route = rt->findRoute(headSID.getNidHead());

    //首先在缓存中查找,节点成为潜在转发者的条件是缓存中没有该数据或者请求端指定了要从源端获取数据
    if (findContentInCache(headSID) == nullptr || (head->getMustBeFresh() && nid != headSID.getNidHead()))
    {
        simtime_t ttl = 1;
        //先查看路由表中,是否有对应表项，若有设置下一跳后直接转发，pit增加相应表项
        //只有簇头才进行转发，添加PIT等操作
        if(nodeIndex == 28)
        {
            std::cout<<28<<endl;
        }

        bool forward =  isLastHopHead && isGateway() || isHead();
        

        if (forward || route != nullptr)
        {
            
            head->setCluster(isHead());
            if (route != nullptr && flood == false && !pit->hasThisSid(headSID))
            {
                // std::cout << nodeIndex << "  " << simTime() << "     ";
                // std::for_each(head->getTrace().begin(), head->getTrace().end(), [](const int &n) { std::cout << n << "->"; });
                // std::cout << endl;

                if(head->getNexthop() == nid || head->getNexthop().isDefault())
                {
                    if (ie == ie)
                        pit->createEntry(head->getSid(), head->getSource(), macInfo->getSrcAddress(), ttl, 24, head->getNonce());
                    else
                        pit->createEntry(head->getSid(), head->getSource(), macInfo->getSrcAddress(), ttl, 5, head->getNonce());

                    packet->clearTags();

                    head->setTimeToLive(head->getTimeToLive() - 1);
                    head.get()->getTraceForUpdate().push_back(nodeIndex);
                    head->setLastHop(nid);
                    head->setNexthop(route->getNextHop());
                    head->setMAC(ie->getMacAddress());
                    packet->insertAtFront(head);

                    // sendDatagramToOutput(packet,24,route->getNextMac());
                    sendDatagramToOutput(packet->dup(),24);

                    delete packet;
                    return;
                }
            }
            //没有路由表项，需要广播探测路径
            if ((head->getNexthop().isDefault() && route == nullptr) || flood == true)
            {
                head->setTimeToLive(head->getTimeToLive() - 1);
                head.get()->getTraceForUpdate().push_back(nodeIndex);
                head->setLastHop(nid);

                Packet *newPacket = packet->dup();
                newPacket->clearTags();
                newPacket->insertAtFront(head);

                // std::cout<<nodeIndex<<endl;
                if (!pit->hasThisSid(headSID))
                {
                    if (delay_queue24.check_and_decrease(newPacket) == false)
                        delay_queue24.insert(newPacket->dup(), GET, simTime() + uniform(0, getDelayTime), TC);
                    // delay_queue5.insert(newPacket->dup(), GET, simTime() + uniform(0, getDelayTime), TC);
                }
                else
                {
                    delay_queue24.check_and_decrease(newPacket);
                    if(nodeIndex == 28)
                    {
                        std::cout<<28<<endl;
                    }
                }

                delete newPacket;
                //添加新条目

                if (ie == ie)
                    pit->createEntry(head->getSid(), head->getSource(), macInfo->getSrcAddress(), ttl, 24, head->getNonce());
                else
                    pit->createEntry(head->getSid(), head->getSource(), macInfo->getSrcAddress(), ttl, 5, head->getNonce());
            }
        }
    }
    //缓存中存在，根据数据包的入网卡不同进行不同的操作
    else
    {
        createRoute(head->getSource(), head->getLastHop(), macInfo->getSrcAddress(), routeLifeTime, 24, head->getTimeToLive());
        createRoute(head->getLastHop(), head->getLastHop(), macInfo->getSrcAddress(), routeLifeTime, 24, head->getTimeToLive());

        //仅为了测试使用，处理逻辑是如果缓存中有就回传SID对应的所有数据包
        std::for_each(head->getTrace().begin(), head->getTrace().end(), [](const int &n) { std::cout << n << "->"; });
        std::cout << nodeIndex;
        std::cout << "  content found!"
                  << "at " << simTime() << endl;

        auto lists = findContentInCache(headSID)->GetList();
        int nic = (ie == ie) ? 24 : 5;


        for (const auto &P : lists)
        {
            if (P != nullptr)
            {
                if (!pit->servedForThisGet(headSID, head->getNonce()))
                    testModule.GetRecvNum++;
                pit->SetServed(headSID);
                pit->createEntry(headSID, head->getSource(), head->getMAC(), simtime_t(1), nic, head->getNonce());

                testModule.DataSendNum++;
                auto newP = P->dup();

                
                auto forwardHead = newP->removeAtFront<Data>();
                forwardHead->setPortNumber2(port);
                if(route != nullptr)
                {
                    forwardHead->setRouteMetric(static_cast<int>(route->getLinkQlt()));
                    forwardHead->setRouteLifetime(route->getLifeTime());
                }
                newP->insertAtFront(forwardHead);

                sendDatagramToOutput(newP, 24, macInfo->getSrcAddress());
   
            }
        }
    }
    
    delete packet;
}

//下面函数只是对几个表操作的简单封装
shared_ptr<Croute> colorCluster::findRoute(const NID& nid)
{
    return rt->findRoute(nid);
}

void colorCluster::createRoute(const NID &dest, const NID &nextHop, const MacAddress &mac, const simtime_t &ttl, int interFace, double linkQ)
{
    return rt->CreateEntry(dest, nextHop, mac, ttl, interFace, linkQ);
}

void colorCluster::createPIT(const SID &sid, const NID &nid, const MacAddress &mac, simtime_t ttl)
{
    pit->createEntry(sid, nid, mac, ttl);
}

colorPendingGetTable::EntrysRange colorCluster::findPITentry(const SID& sid)
{
    return pit->findPITentry(sid);
}

void colorCluster::CachePacket(const SID &sid, Packet *packet)
{
    ct->CachePacket(sid, packet);
}

shared_ptr<ContentBlock> colorCluster::findContentInCache(const SID &sid)
{
    return ct->getBlock(sid);
}

const Ptr<inet::Get> colorCluster::GetHead(const SID &sid)
{
    auto route = rt->findRoute(sid.getNidHead());

    const auto &get = makeShared<Get>();
    get->setVersion(0);
    get->setTimeToLive(hopLimit);
    get->setMTU(mtu);
    get->setSid(sid);
    get->setSource(nid);
    get->setLastHop(nid);
    get->setMAC(ie->getMacAddress());
    get->setNonce(this->getRNG(0)->intRand());
    if (isHead())
        get->setCluster(true);
    else
        get->setCluster(false);

    if(route != nullptr && flood == false)
    {
        get->setNexthop(route->getNextHop());
    }

    return get;
}

const Ptr<inet::Data> colorCluster::DataHead(const SID &sid)
{
    const auto &data = makeShared<Data>();
    data->setVersion(0);
    data->setTimeToLive(hopLimit);
    data->setMTU(mtu);
    data->setSid(sid);
    data->setNid(nid);
    data->setLastHop(nid);

    data->setMAC(ie->getMacAddress());


    return data;
}

void colorCluster::encapsulate(Packet *packet, int type, const SID& sid)
{
    //封装数据包，加上color的头部, 0是get包，1是data包
    if (type == 0)
    {

        auto get = GetHead(sid);
        get->getTraceForUpdate()
            .push_back(nodeIndex);

        //添加报文头
        packet->insertAtFront(get);
    }
    if (type == 1)
    {

        auto data = DataHead(sid);

        //测试
        data->setTotalLength(B(packet->getByteLength()));
        data->getTraceForUpdate().push_back(nodeIndex);
        data->setComeFromSource(true);

        packet->insertAtFront(data);
    }
}

void colorCluster::encapsulate(Packet *packet, int type, const SID& sid, int port)
{
    //重载，添加端口号
    //封装数据包，加上color的头部, 0是get包，1是data包
    if (type == 0)
    {

        auto get = GetHead(sid);
        get->getTraceForUpdate()
            .push_back(nodeIndex);
        get->setPortNumber1(port);

        //添加报文头
        packet->insertAtFront(get);
    }
    if (type == 1)
    {

        auto data = DataHead(sid);

        //测试
        data->setTotalLength(B(packet->getByteLength()));
        data->getTraceForUpdate().push_back(nodeIndex);
        data->setComeFromSource(true);
        data->setPortNumber2(port);

        packet->insertAtFront(data);
    }
}

void colorCluster::decapsulate(Packet *packet,  const SID& sid)
{
    //解封装数据包record
    testModule.DataRecvNum++;

    testModule.delayArray.push_back((simTime().dbl() - testModule.Delays[sid].dbl()) * 1000);
    std::cout << "receive the packet successfully!" << endl;
    std::cout << "delay is " << (simTime().dbl() - testModule.Delays[sid].dbl()) * 1000 << " ms" << endl;

    //移除Delays中的此sid对应的delay
    testModule.Delays.erase(sid);

    //统计吞吐量
    testModule.throughput += B(packet->getByteLength());
    packet->trim();
    packet->removeAtFront<Data>();
}

void colorCluster::sendDatagramToOutput(Packet *packet, int nic, const MacAddress &mac)
{
    //对数据包添加tag后发送到下层
    ASSERT((nic == 24));
    if (nic == 24)
    {
        EV_INFO << "Sending " << packet << " to output interface = "
                << "ie" << endl;
        packet->removeTagIfPresent<MacAddressReq>();

        auto src = ie->getMacAddress();
        auto macAddrReq = packet->addTag<MacAddressReq>();
        macAddrReq->setSrcAddress(src);
        macAddrReq->setDestAddress(mac);

        packet->removeTagIfPresent<InterfaceReq>();
        packet->addTagIfAbsent<InterfaceReq>()->setInterfaceId(ie->getInterfaceId());

        packet->removeTagIfPresent<PacketProtocolTag>();
        packet->addTagIfAbsent<PacketProtocolTag>()->setProtocol(&Protocol::color);

        packet->removeTagIfPresent<DispatchProtocolInd>();
        packet->addTagIfAbsent<DispatchProtocolInd>()->setProtocol(&Protocol::color);

        //通过动态类型转换判断数据包的类型
        const auto &head = packet->peekAtFront<Chunk>(B(78), 0);
        auto pointer = head.get();
        Chunk *chunk = const_cast<Chunk *>(pointer);
        if (dynamic_cast<Data *>(chunk)) {
            auto pitInd = packet->addTag<PitInd>();
            pitInd->setPitLength(pit->getLength());
            pitInd->setPitCapacity(pit->getCapacity());
        }
        send(packet, "queueOut");
    }
}



void colorCluster::handlePacketFromHL(Packet *packet)
{
    //todo implement later
}

const InterfaceEntry *colorCluster::getSourceInterface(Packet *packet)
{
    auto tag = packet->findTag<InterfaceInd>();
    return tag != nullptr ? ift->getInterfaceById(tag->getInterfaceId()) : nullptr;
}

void colorCluster::FragmentAndStore(Packet *packet, const SID &sid)
{
    const auto &data = makeShared<Data>();
    int headerLength = data->getHeaderLength().get();
    int payloadLength = packet->getByteLength();
    int fragmentLength = ((mtu - headerLength) / 8) * 8;
    ASSERT(fragmentLength > 0);

    if (headerLength + payloadLength < mtu)
    {
        encapsulate(packet, 1, sid);
        const auto &testhead = packet->peekAtFront<Chunk>(B(78), 0);
        CachePacket(sid, packet->dup());
    }
    else
    {
        string fragMsgName = packet->getName();
        fragMsgName += "-frag-";

        for (int offset = 0; offset < payloadLength;)
        {
            bool lastFragment = (offset + fragmentLength >= payloadLength);

            int thisFragmentLength = lastFragment ? payloadLength - offset : fragmentLength;

            string curFragName = fragMsgName + std::to_string(offset);
            if (lastFragment)
                curFragName += "-last";
            Packet *fragment = new Packet(curFragName.c_str());

            ASSERT(fragment->getByteLength() == 0);
            auto fraghdr = staticPtrCast<Data>(data->dupShared());
            const auto &fragData = packet->peekDataAt(B(offset), B(thisFragmentLength));
            ASSERT(fragData->getChunkLength() == B(thisFragmentLength));
            fragment->insertAtBack(fragData);

            auto head = DataHead(sid);
            if (!lastFragment)
                head->setMoreFragments(true);

            head->setOffset(offset);
            head->setTotalLength(B(headerLength + thisFragmentLength));
            head->getTraceForUpdate().push_back(getParentModule()->getParentModule()->getIndex());

            fragment->insertAtFront(head);

            //            auto datalen = fragment->getDataLength().get();

            CachePacket(sid, fragment);
            offset += thisFragmentLength;
        }

    }
    delete packet;
}

void colorCluster::FragmentAndSend(Packet *packet)
{
}

void colorCluster::testSend(const SID &sid)
{
    Packet *packet = new Packet("getPacket");

    encapsulate(packet, 0, sid);
    auto mac = ie->getMacAddress();
    pit->createEntry(sid, nid, mac, simtime_t(5), 24, 0, false, true);
    sendDatagramToOutput(packet, 24);
    testModule.Delays[sid] = simTime();


    testModule.GetSendNum++;

    std::cout << endl;
    delete packet;
}

void colorCluster::sendGET(const SID &sid, int port)
{
    Packet *packet = new Packet("getPacket");

    encapsulate(packet, 0, sid, port);
    auto mac = ie->getMacAddress();
    pit->createEntry(sid, nid, mac, simtime_t(5), 24, 0, false, true);
    sendDatagramToOutput(packet, 24);
    testModule.Delays[sid] = simTime();
    testModule.GetSendNum++;
}

void colorCluster::testProvide(const SID &sid, const B &dataSize)
{
    auto playload = makeShared<AppData>();
    playload->setChunkLength(dataSize);
    Packet *packet = new Packet("DATA");

    packet->insertAtBack(playload);
    FragmentAndStore(packet, sid);

    //    ct->CachePacket(1024, packet);
}
void colorCluster::cacheData(const SID &sid, Packet *packet)
{
    FragmentAndStore(packet, sid);
}

void colorCluster::scheduleGet(simtime_t t, SendMode mode)
{
    //不同的发包模式
    cancelEvent(testGet);
    switch (mode)
    {

    //等间隔t发送
    case SendMode::EqualInterval:
    {
        scheduleAt(simTime() + t, testGet);
    }
    break;

    //发送间隔服从均值为t的均匀分布
    case SendMode::UniformDisInterval:
    {
        scheduleAt(simTime() + uniform(0, t), testGet);
    }
    break;

    //发送间隔服从均值为t的指数分布
    case SendMode::ExpDisInterval:
    {
        scheduleAt(simTime() + exponential(t), testGet);
    }
    break;

    default:
        break;
    }
}

void colorCluster::receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject *details)
{
}

void colorCluster::record()
{
    std::ofstream outfile;


    if (nodeIndex == Cindex)
    {
        auto fileName = cSimulation::getActiveEnvir()->getConfigEx()->getActiveConfigName() + string("_Consumer.txt");
        outfile.open("./simu/" + fileName, std::ofstream::app);
        testModule.ConsumerPrint(outfile);
        outfile.close();
        // outfile.open(cSimulation::getActiveEnvir()->getConfigEx()->getActiveConfigName() + string("_") + std::to_string(getDelayTime) + "_" + std::to_string(dataDelayTime) + "_" + std::to_string(flood) + string("_delays.txt"), std::ofstream::app);
        // outfile << nodeIndex << ": ";
        // std::for_each(testModule.delayArray.begin(), testModule.delayArray.end(), [&outfile](double time) { outfile << time << ","; });
        // outfile << endl;
        // outfile << endl;
    }

}

bool colorCluster::isForwarder()
{
    return clusterModule->isHead() || clusterModule->isPreHead() || clusterModule->isGateWay();
}

void  colorCluster::handleRequest(Request *request)
{
    auto ctrl = request->getControlInfo();
    if (ctrl == nullptr)
        throw cRuntimeError("Request '%s' arrived without controlinfo", request->getName());
    else if(ColorSocketBindCommand *command = dynamic_cast<ColorSocketBindCommand *>(ctrl))
    {
        int socketId = request->getTag<SocketReq>()->getSocketId();
        SocketDescriptor *descriptor = new SocketDescriptor(socketId, command->getProtocol()->getId(), command->getNid(),command->getLocalPort());
        socketsByPortMap[command->getLocalPort()] = descriptor;
        socketIdToSocketDescriptor[socketId] = descriptor;
        delete request;
    }
    else if(ColorSocketSendGetCommand *command = dynamic_cast<ColorSocketSendGetCommand *>(ctrl))
    {
        Cindex = nodeIndex;
        sentInterval = command->getInter();
        sendGET(command->getSid(), command->getLocalPort());
        delete request;
    }
    else if(ColorSocketCacheDataCommand *command = dynamic_cast<ColorSocketCacheDataCommand *>(ctrl))
    {
        auto msg = const_cast<cMessage *>(command->getPkt());
        auto pkt = dynamic_cast<Packet *>(msg);
        cacheData(command->getSid(), pkt);
        delete request;
    }
}

void colorCluster::sendUp(Packet *pkt, SocketDescriptor *sd)
{
    pkt->removeTagIfPresent<DispatchProtocolReq>();
    pkt->addTagIfAbsent<SocketInd>()->setSocketId(sd->socketId);
    send(pkt,"transportOut");
}

INetfilter::IHook::Result colorCluster::datagramPreRoutingHook(Packet *datagram)
{
    return INetfilter::IHook::ACCEPT;
}


INetfilter::IHook::Result colorCluster::datagramForwardHook(Packet *datagram)
{
    return INetfilter::IHook::ACCEPT;
}


INetfilter::IHook::Result colorCluster::datagramPostRoutingHook(Packet *datagram)
{
    return INetfilter::IHook::ACCEPT;
}


INetfilter::IHook::Result colorCluster::datagramLocalInHook(Packet *datagram)
{
    return INetfilter::IHook::ACCEPT;
}


INetfilter::IHook::Result colorCluster::datagramLocalOutHook(Packet *datagram)
{
    return INetfilter::IHook::ACCEPT;
}

void colorCluster::registerHook(int priority, IHook *hook)
{

}


void colorCluster::unregisterHook(IHook *hook)
{

}


void colorCluster::dropQueuedDatagram(const Packet *datagram) 
{

}


void colorCluster::reinjectQueuedDatagram(const Packet *datagram)
{
    
}

} // namespace inet
