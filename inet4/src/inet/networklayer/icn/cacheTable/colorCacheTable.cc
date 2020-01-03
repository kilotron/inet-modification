/*
 * colorCacheTable.cc
 *
 *  Created on: 2019年7月5日
 *      Author: hiro
 */

#include "colorCacheTable.h"

#include "inet/common/INETUtils.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/Simsignals.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/lifecycle/NodeStatus.h"

#include<algorithm>

namespace inet
{
Define_Module(ColorCacheTable);

void ColorCacheTable::finish()
{
    //FINISH时删除所有计时器的指针
    for (auto iter = timers->begin(); iter != timers->end(); iter++)
    {
        delete iter->first;
    }
    timers->clear();
    //清理所有缓存
    for_each(table.begin(), table.end(), [](std::pair<SID, shared_ptr<ContentBlock>> entry) {
        entry.second->flush();
    });

//    delete table;
    delete timers;
}

void ColorCacheTable::initialize(int stage)
{
    cSimpleModule::initialize(stage);
    if (stage == INITSTAGE_LOCAL)
    {
        owner = getContainingNode(this);
        mtu = par("mtu").intValue();
//        table = new CacheTable;
        timers = new timerTable;
    }
    else if (stage == INITSTAGE_NETWORK_LAYER)
    {
        cModule *node = findContainingNode(this);
        NodeStatus *nodeStatus = node ? check_and_cast_nullable<NodeStatus *>(node->getSubmodule("status")) : nullptr;
        isNodeUp = !nodeStatus || nodeStatus->getState() == NodeStatus::UP;
    }
}

void ColorCacheTable::handleMessage(cMessage *timer)
{
    //计时器时间到后删除对应表项
    //找到对应的sid
    auto sid = timers->find(timer)->second;

    //找到对应的block
    auto block = table.find(sid)->second;

    remain += block->GetSize();

    table.erase(sid);
    timers->erase(timer);
    cancelAndDelete(timer);
}

void ColorCacheTable::receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject *details)
{
    if (getSimulation()->getContextType() == CTX_INITIALIZE)
        return; // ignore notifications during initialize

    Enter_Method_Silent();
    printSignalBanner(signalID, obj, details);
}

bool ColorCacheTable::handleOperationStage(LifecycleOperation *operation, IDoneCallback *doneCallback)
{
    Enter_Method_Silent();
    int stage = operation->getCurrentStage();
    if (dynamic_cast<ModuleStartOperation *>(operation))
    {
    }
    else if (dynamic_cast<ModuleStopOperation *>(operation))
    {
        if (static_cast<ModuleStopOperation::Stage>(stage) == ModuleStopOperation::STAGE_NETWORK_LAYER)
        {
        }
    }
    else if (dynamic_cast<ModuleCrashOperation *>(operation))
    {
        if (static_cast<ModuleCrashOperation::Stage>(stage) == ModuleCrashOperation::STAGE_CRASH)
        {
        }
    }
    return true;
}

void ColorCacheTable::printCacheTable(std::ostream &out)
{
    for (auto iter = table.begin(); iter != table.end(); iter++)
    {
        out << iter->second->str() << endl;
    }
}

shared_ptr<ContentBlock> ColorCacheTable::getBlock(SID sid)
{
    auto result = table.find(sid);
    if (result != table.end())
        return result->second;
    else
        return nullptr;
}

shared_ptr<ContentBlock> ColorCacheTable::CreateBlock(SID sid)
{
    //创建表项
    shared_ptr<ContentBlock> block = std::make_shared<ContentBlock>(sid, mtu);
    table[sid] = block;

    //创建计时器
    cMessage *timer = new cMessage("timer");
    (*timers)[timer] = sid;
    return block;
}

void ColorCacheTable::CachePacket(SID sid, Packet *packet)
{

    auto iter = table.find(sid);
    if (iter != table.end())
    {
        //已有block直接插入
        iter->second->InsterPacket(packet);
    }
    else
    {
        //没有就创建block后插入
        CreateBlock(sid);
        iter = table.find(sid);

        if (iter == table.end())
            std::cout << "end!" << endl;
        auto ptr = iter->second;
        iter->second->InsterPacket(packet);
    }
}

bool ColorCacheTable::hasThisPacket(Packet *packet, SID sid)
{
    auto iter = table.find(sid);
    if (iter == table.end())
    {
        return false;
    }
    else
    {
        return iter->second->hasThisPacket(packet);
    }
}
} // namespace inet
