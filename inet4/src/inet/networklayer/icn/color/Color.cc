/*
 * Color.cc
 *
 *  Created on: 2019年7月1日
 *      Author: hiro
 */

#include <iostream>
#include <algorithm>

#include "inet/networklayer/icn/color/Color.h"
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

namespace inet
{

Define_Module(color);

void color::SimRcorder::ConsumerPrint(std::ostream &os)
{

    os << "index:    " << index << endl;
    os << "sendNum: " << GetSendNum << endl;
    os << "recvNum: " << DataRecvNum << endl;
    if (GetSendNum != 0)
        os << "trans ratio: " << 100 * DataRecvNum / GetSendNum << "%" << endl;
    os << "send Interval: " << owner->sentInterval << "s" << endl;
    os << "Throughput: " << throughput * 8 / ((simTime().dbl() - owner->testget.dbl()) * 1000 * 1000) << " Mbps" << endl;
    os << endl;
    
}

void color::SimRcorder::ProviderPrint(std::ostream &os)
{
    os << "index:    " << index << endl;
    os << "DataSendNum: " << DataSendNum << endl;
    os << "GetRecvNum: " << GetRecvNum << endl;
    os << endl;
}

color::~color()
{
}

void color::finish()
{

    record();
    cancelAndDelete(testGet);
    cancelAndDelete(testData);
}

void color::initialize(int stage)
{
    OperationalBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL)
    {
        nodeIndex = getParentModule()->getParentModule()->getIndex();
        std::array<uint64_t, 2> hashValue;
        MurmurHash3_x64_128(&nodeIndex, sizeof(int), 0, hashValue.data());
        nid.setNID(hashValue);
        testTimer = nullptr;
        testGet = new cMessage("testGet");
        testData = new cMessage("testData");

        testget = 2 + 1.0 / (nodeIndex % 10 + 1);
        testdata = 1;

        testModule.owner = this;
        testModule.index = nodeIndex;
        testModule.multiConsumer = par("multi").intValue();

        Cindex = cSimulation::getActiveSimulation()->getSystemModule()->par("Cindex").intValue();
        Pindex = cSimulation::getActiveSimulation()->getSystemModule()->par("Pindex").intValue();
        sentInterval = cSimulation::getActiveSimulation()->getSystemModule()->par("sentInterval").doubleValue();

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
        name = getParentModule()->getFullPath();
        name = name + ".cluster";
        path = name.c_str();
        mod = this->getModuleByPath(path);
        clusterModule = dynamic_cast<ICluster *>(mod);

        mtu = par("mtu").intValue();
        hopLimit = par("hopLimit").intValue();

        getProdelay = par("GetProcDelay").doubleValue();
        dataProdelay = par("DataProcDelay").doubleValue();

        registerService(Protocol::color, gate("transportIn"), gate("queueIn"));
        registerProtocol(Protocol::color, gate("queueOut"), gate("transportOut"));

    }
}

InterfaceEntry *color::chooseInterface(const char *interfaceName)
{
    //得到指向转发表的指针
    InterfaceEntry *ie = nullptr;
    if (strlen(interfaceName) > 0)
    {
        ie = ift->getInterfaceByName(interfaceName);
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

void color::handleRegisterService(const Protocol &protocol, cGate *out, ServicePrimitive servicePrimitive)
{
    Enter_Method("handleRegisterService");
}

void color::handleRegisterProtocol(const Protocol &protocol, cGate *in, ServicePrimitive servicePrimitive)
{
    //对上层协议提供注册服务
    Enter_Method("handleRegisterProtocol");
    if (in->isName("transportIn"))
        upperProtocols.insert(&protocol);
}

void color::refreshDisplay() const
{
    OperationalBase::refreshDisplay();
}

void color::handleStartOperation(LifecycleOperation *operation)
{
    //默认wlan0是2.4GHz， wlan1是5GHz
    ie24 = chooseInterface("wlan0");
    ie5 = chooseInterface("wlan1");

    //测试，一个节点作为内容源一个作为请求者发送get包
    if (testModule.multiConsumer == 1)
    {
        if (nodeIndex % Cindex == 0)
            scheduleAt(testget, testGet);
        else if (nodeIndex == Pindex)
            scheduleAt(testdata, testData);
    }
    else if (testModule.multiConsumer == 0)
    {
        if (nodeIndex == Cindex)
            scheduleAt(testget, testGet);
        else if (nodeIndex == Pindex)
            scheduleAt(testdata, testData);
    }
    else if (testModule.multiConsumer == 2)
    {
        // std::cout << "sch" << endl;
        if (nodeIndex - Cindex >= 0 && nodeIndex - Cindex <= 3)
        {
            scheduleAt(testget, testGet);
        }
        else if (nodeIndex == Pindex)
            scheduleAt(testdata, testData);
    }
}

void color::handleStopOperation(LifecycleOperation *operation)
{
    // TODO: stop should send and wait pending packets
    cancelAndDelete(testGet);
    cancelAndDelete(testData);
}

void color::handleCrashOperation(LifecycleOperation *operation)
{
    cancelAndDelete(testGet);
    cancelAndDelete(testData);
}

void color::handleMessageWhenUp(cMessage *msg)
{
    //数据包来自上层
    if (msg->arrivedOn("transportIn"))
    { //TODO packet->getArrivalGate()->getBaseId() == transportInGateBaseId
        handlePacketFromHL(check_and_cast<Packet *>(msg));
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
        static int requestIndex = 0;
        if (msg == testGet)
        {
            if (testModule.multiConsumer == 0)
            {
                testSend({Pindex,requestIndex++ % 2});
                scheduleGet(sentInterval, SendMode::UniformDisInterval);
            }
            else if(testModule.multiConsumer == 1)
            {
                if(nodeIndex % Cindex == 0)
                    testSend({Pindex,requestIndex++ % 2});
                scheduleGet(sentInterval, SendMode::UniformDisInterval);
            }
            else
            {
                //请求的SID与当前时间相关
                int temp = ceil(uniform(simTime() - 2, simTime() + 2).dbl());
                int sid = temp < 0 ? 0 : temp;
                // std::cout << "req sid: " << sid << "interval: " << sentInterval << endl;
                testSend({Pindex,sid});
                scheduleGet(sentInterval, SendMode::UniformDisInterval);
            }
        }
        else if (msg == testData)
        {
            for (int i = 0; i < 200; i++)
            {
                testProvide({Pindex,i}, B(2000));
            }
        }
        else
        {
            //表明延迟发送的报文计时器计时完成
            auto pair = PendingPkts.find(msg);
            if (pair == PendingPkts.end())
            {
                throw cRuntimeError("unknown selfmessage '%s'", msg->getArrivalGate()->getName());
            }
            else
            {
                if (pair->second.type == Pendingpkt::GET)
                {

                }
                else
                {
                    
                }
                PendingPkts.erase(msg);
                cancelAndDelete(msg);
            }
        }
    }
    else
        throw cRuntimeError("message arrived on unknown gate '%s'", msg->getArrivalGate()->getName());
}

void color::handleIncomingDatagram(Packet *packet)
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

void color::handleDataPacket(Packet *packet)
{
    //取出头部
    const auto head = packet->peekAtFront<Data>(B(78), 0);
    const SID &headSid = head->getSid();

    auto newPacket = packet->dup();

    //测试信息
    if (nodeIndex == Cindex)
    {
    }

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
            decapsulate(fullPacket, headSid);
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

        if (!pit->hasThisSid(headSid))
        {
            pit->RemoveEntry(headSid);
        }
    }

    //查看PIT表是否有此SID记录
    if (pit->hasThisSid(headSid))
    {
        //        std::cout << "data received, index: " << getParentModule()->getParentModule()->getIndex() << endl;

        //转发原则与NDN相同，get包从哪个端口来，data包就往哪个端口回
        //指示data包应该发往那个端口，hz5==true发往5GHz端口，hz24==true发往2.4GHz端口
        bool hz5 = false;
        bool hz24 = false;

        //PIT表中一个SID可能对应多条表项
        auto range = findPITentry(headSid);
        for (auto iter = range.first; iter != range.second; iter++)
        {

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
        if (isHead() && !ct->hasThisPacket(packet, headSid))
        {
            //测试信息
            //                        std::cout << "data received, index: " << getParentModule()->getParentModule()->getIndex() << endl;

            if (head->getTimeToLive() < 1)
            {
                delete packet;

                return;
            }

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

            newPacket->clearTags();
            //            newPacket->trim();
            //TTL-1， 测试trace加入本节点
            newHead->setTimeToLive(newHead->getTimeToLive() - 1);
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
                sendDatagramToOutput(newPacket->dup(), 24);
            }
            if (hz5 == 1)
            {
                //数据包发往5GHz端口
                //                std::cout << "send back" << endl;
                sendDatagramToOutput(newPacket->dup(), 5);
            }

            //在一个get包就对应一个data包的情况中，缓存转发后移除PIT条目
            pit->RemoveEntry(headSid);
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

void color::handleGetPacket(Packet *packet)
{

    if (nodeIndex == Pindex)
    {
        testModule.GetRecvNum++;
    }
    //根据GET包头部内容进行相应操作
    packet->trim();
    const auto &head = packet->removeAtFront<Get>();

    //检查ttl，ttl小于等于1丢弃
    if (head->getTimeToLive() < 1)
    {
        delete packet;
        return;
    }

    //    if (isHead())
    // std::cout << nodeIndex <<"  "<< head->getNexthop() << endl;

    auto headSID = head->getSid();
    //缓存中存在，根据数据包的入网卡不同进行不同的操作
    auto ie = getSourceInterface(packet);
    //首先在缓存中查找
    if (findContentInCache(headSID) == nullptr)
    {

        //如果是簇头才添加PIT表项，再进行转发
        if (isHead() && (head->getNexthop() == -1 || head->getNexthop() == nodeIndex))
        {
            head->setTimeToLive(head->getTimeToLive() - 1);
            head.get()->getTraceForUpdate().push_back(nodeIndex);
            head->setNexthop(-1);
            Packet *newPacket = packet->dup();
            newPacket->clearTags();
            newPacket->insertAtFront(head);

            // std::cout<<nodeIndex<<endl;
            if (!pit->hasThisSid(headSID))
            {
                sendDatagramToOutput(newPacket->dup(), 24);
                sendDatagramToOutput(newPacket->dup(), 5);
            }

            delete newPacket;
            //添加新条目
            simtime_t ttl = 0.01;

            if (ie == ie24)
                pit->createEntry(head->getSid(), head->getSource(), head->getMAC(), ttl, 24);
            else
                pit->createEntry(head->getSid(), head->getSource(), head->getMAC(), ttl, 5);
        }
    }
    else
    {
        //仅为了测试使用，处理逻辑是如果缓存中有就回传SID对应的所有数据包

        //        std::cout << "content found!" << endl;

        //        std::for_each(head->getTrace().begin(), head->getTrace().end(), [](const int &n) { std::cout << n << "->"; });
        // std::cout << nodeIndex;
        // std::cout << endl;

        auto lists = findContentInCache(headSID)->GetList();
        int nic = (ie == ie24) ? 24 : 5;

        testModule.DataSendNum++;

        for (const auto &P : lists)
        {
            if (P != nullptr)
            {
                auto newP = P->dup();
                //                std::cout << "sent data packet" << endl;
                if (!isHead())
                    sendDatagramToOutput(newP, 5);
                else
                    sendDatagramToOutput(newP, nic);
            }
        }
    }
    //    ;
    delete packet;
}

//下面函数只是对几个表操作的简单封装
shared_ptr<Croute> color::findRoute(SID sid)
{
    return rt->findMachEntry(sid);
}

shared_ptr<Croute> color::createRoute(SID sid, simtime_t ttl)
{
    return rt->CreateEntry(sid, nid, ttl);
}

void color::createPIT(const SID &sid, const NID &nid, const MacAddress &mac, simtime_t ttl)
{
    pit->createEntry(sid, nid, mac, ttl);
}

colorPendingGetTable::EntrysRange color::findPITentry(SID sid)
{
    return pit->findPITentry(sid);
}

void color::CachePacket(SID sid, Packet *packet)
{
    ct->CachePacket(sid, packet);
}

shared_ptr<ContentBlock> color::findContentInCache(SID sid)
{
    return ct->getBlock(sid);
}

const Ptr<inet::Get> color::GetHead(SID sid)
{
    const auto &get = makeShared<Get>();
    get->setVersion(0);
    get->setTimeToLive(hopLimit);
    get->setMTU(mtu);
    get->setSid(sid);
    get->setLastHop(nid);
    get->setMAC(ie24->getMacAddress());

    return get;
}

const Ptr<inet::Data> color::DataHead(SID sid)
{
    const auto &data = makeShared<Data>();
    data->setVersion(0);
    data->setTimeToLive(5);
    data->setMTU(mtu);
    data->setSid(sid);
    data->setNid(nid);
    data->setMAC(ie5->getMacAddress());

    return data;
}

void color::encapsulate(Packet *packet, int type, SID sid)
{
    //封装数据包，加上color的头部, 0是get包，1是data包
    if (type == 0)
    {

        auto get = GetHead(sid);
        auto headsSet = clusterModule->getHeads();
        if (headsSet.size() > 1)
        {
            get->setNexthop(*headsSet.begin());
        } //测试
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

        packet->insertAtFront(data);
    }
}

void color::encapsulate(Packet *packet, int type, int portSelf, int portDest)
{
    //重载，添加端口号
}

void color::decapsulate(Packet *packet, SID sid)
{
    //解封装数据包record
    testModule.DataRecvNum++;

    std::cout << "receive the packet successfully!" << endl;
    std::cout << "delay is " << (simTime() - testModule.Delays[sid]).format(SIMTIME_MS) << " ms" << endl;

    //移除Delays中的此sid对应的delay
    testModule.Delays.erase(sid);

    //统计吞吐量
    testModule.throughput += B(packet->getByteLength());
    delete packet;
}

void color::sendDatagramToOutput(Packet *packet, int nic)
{
    //对数据包添加tag后发送到下层
    ASSERT((nic == 24) || (nic == 5));
    if (nic == 24)
    {
        EV_INFO << "Sending " << packet << " to output interface = "
                << "ie24" << endl;
        packet->removeTagIfPresent<MacAddressReq>();

        auto src = ie24->getMacAddress();
        auto macAddrReq = packet->addTag<MacAddressReq>();
        macAddrReq->setSrcAddress(src);
        macAddrReq->setDestAddress(MacAddress::BROADCAST_ADDRESS);

        packet->removeTagIfPresent<InterfaceReq>();
        packet->addTagIfAbsent<InterfaceReq>()->setInterfaceId(ie24->getInterfaceId());

        packet->removeTagIfPresent<PacketProtocolTag>();
        packet->addTagIfAbsent<PacketProtocolTag>()->setProtocol(&Protocol::color);

        packet->removeTagIfPresent<DispatchProtocolInd>();
        packet->addTagIfAbsent<DispatchProtocolInd>()->setProtocol(&Protocol::color);

        send(packet, "queueOut");
    }
    else if (nic == 5)
    {
        EV_INFO << "Sending " << packet << " to output interface = "
                << "ie5" << endl;
        auto src = ie5->getMacAddress();

        packet->removeTagIfPresent<MacAddressReq>();
        auto macAddrReq = packet->addTagIfAbsent<MacAddressReq>();
        macAddrReq->setSrcAddress(src);
        macAddrReq->setDestAddress(MacAddress::BROADCAST_ADDRESS);

        packet->removeTagIfPresent<InterfaceReq>();
        packet->addTagIfAbsent<InterfaceReq>()->setInterfaceId(ie5->getInterfaceId());

        packet->removeTagIfPresent<PacketProtocolTag>();
        packet->addTagIfAbsent<PacketProtocolTag>()->setProtocol(&Protocol::color);


        send(packet, "queueOut");
    }
}

void color::handlePacketFromHL(Packet *packet)
{
    //todo implement later
}

const InterfaceEntry *color::getSourceInterface(Packet *packet)
{
    auto tag = packet->findTag<InterfaceInd>();
    return tag != nullptr ? ift->getInterfaceById(tag->getInterfaceId()) : nullptr;
}

void color::FragmentAndStore(Packet *packet, SID sid)
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
        std::string fragMsgName = packet->getName();
        fragMsgName += "-frag-";

        for (int offset = 0; offset < payloadLength;)
        {
            bool lastFragment = (offset + fragmentLength >= payloadLength);

            int thisFragmentLength = lastFragment ? payloadLength - offset : fragmentLength;

            std::string curFragName = fragMsgName + std::to_string(offset);
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

        auto lists = findContentInCache(sid)->GetList();
        for (auto p : lists)
        {
            const auto &testHeader = p->peekAtFront<Data>(B(78), 0);
        }
    }
    delete packet;
}

void color::FragmentAndSend(Packet *packet)
{
}

void color::testSend(SID sid)
{
    Packet *packet = new Packet("getPacket");

    if (isHead())
    {
        encapsulate(packet, 0, sid);
        auto mac = ie24->getMacAddress();
        pit->createEntry(sid, nid, mac, simtime_t(5), 24, true);
        sendDatagramToOutput(packet->dup(), 24);
        pit->createEntry(sid, nid, mac, simtime_t(5), 5, true);
        sendDatagramToOutput(packet->dup(), 5);
    }
    else
    {
        encapsulate(packet, 0, sid);
        auto mac = ie5->getMacAddress();
        auto entry = pit->createEntry(sid, nid, mac, simtime_t(5), 5, true);

        sendDatagramToOutput(packet->dup(), 5);
    }
    testModule.Delays[sid] = simTime();
    // delay = SimTime();

    testModule.GetSendNum++;
   
    std::cout << endl;
    //    std::for_each(clusterModule->getHeads().begin(), clusterModule->getHeads().end(), [](const int &n) { std::cout << n << " "; });
//    std::cout << "send get packet" <<simTime()<< endl;
    delete packet;
}

void color::testProvide(SID sid, B dataSize)
{
    auto playload = makeShared<AppData>();
    playload->setChunkLength(dataSize);
    Packet *packet = new Packet();

    packet->insertAtBack(playload);
    FragmentAndStore(packet, sid);

    //    ct->CachePacket(1024, packet);
}

void color::scheduleGet(simtime_t t, SendMode mode)
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

void color::receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject *details)
{
}

void color::record()
{
    std::ofstream outfile;

    if (testModule.multiConsumer == 1)
    {
        if (Cindex == 0 ||nodeIndex % Cindex == 0)
        {
            outfile.open(cSimulation::getActiveEnvir()->getConfigEx()->getActiveConfigName() + std::string("_") + std::to_string(sentInterval) + std::string("_Consumer.txt"), std::ofstream::app);
            testModule.ConsumerPrint(outfile);
            outfile.close();
        }
        else if (nodeIndex == Pindex)
        {
            outfile.open(cSimulation::getActiveEnvir()->getConfigEx()->getActiveConfigName() + std::string("_") + std::to_string(sentInterval) + std::string("_Provider.txt"), std::ofstream::app);
            testModule.ProviderPrint(outfile);
            outfile.close();
        }
    }
    else if(testModule.multiConsumer == 0)
    {
        if (nodeIndex == Cindex)
        {
            outfile.open(cSimulation::getActiveEnvir()->getConfigEx()->getActiveConfigName() + std::string("_Consumer.txt"), std::ofstream::app);
            testModule.ConsumerPrint(outfile);
            outfile.close();
        }
        else if (nodeIndex == Pindex)
        {
            outfile.open(cSimulation::getActiveEnvir()->getConfigEx()->getActiveConfigName() + std::string("_Provider.txt"), std::ofstream::app);
            testModule.ProviderPrint(outfile);
            outfile.close();
        }
    }
    else if(testModule.multiConsumer == 2)
    {
        if(nodeIndex - Cindex >= 0 && nodeIndex - Cindex <= 3)
        {
            outfile.open(cSimulation::getActiveEnvir()->getConfigEx()->getActiveConfigName() + std::string("_Consumer.txt"), std::ofstream::app);
            testModule.ConsumerPrint(outfile);
            outfile.close();
        }
        else if(nodeIndex == Pindex)
        {
            outfile.open(cSimulation::getActiveEnvir()->getConfigEx()->getActiveConfigName() + std::string("_Provider.txt"), std::ofstream::app);
            testModule.ProviderPrint(outfile);
            outfile.close();
        }
    }

}

bool color::isHead()
{
    return clusterModule->isHead() || clusterModule->isPreHead();
}

} // namespace inet
