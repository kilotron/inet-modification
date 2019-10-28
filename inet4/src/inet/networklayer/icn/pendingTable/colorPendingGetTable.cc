/*
 * colorPendingGetTable.cc
 *
 *  Created on: 2019年7月5日
 *      Author: hiro
 */

#include "colorPendingGetTable.h"

namespace inet
{
Define_Module(colorPendingGetTable);

void colorPendingGetTable::finish()
{
    for (auto &iter : timers)
    {
        cancelAndDelete(iter.first);
        sids.erase(iter.second);
    }
}

colorPendingGetTable::~colorPendingGetTable()
{
    
}

void colorPendingGetTable::initialize(int stage)
{
    cSimpleModule::initialize(stage);
}

void colorPendingGetTable::handleMessage(cMessage *msg)
{
    //定时器触发，删除对应的pit 表项
    auto sid = timers.find(msg)->second;
    RemoveEntry(sid);
    //删除定时器消息本身a
    cancelEvent(msg);
}

void colorPendingGetTable::PrintPIT(std::ostream &out)
{
    for (auto iter = table.begin(); iter != table.end();)
    {
        auto range = table.equal_range(iter->first);
        out << "SID is: " << iter->first.str() << endl;
        out << "NIDs is: ";
        for (auto it = range.first; it != range.second; it++)
            out << it->second.getNid().str() << "  ";
        out << endl;
        out << "TTL is: " << iter->second.getTTL();
        iter = range.second;
    }
}

const colorPendingGetTable::Entry& colorPendingGetTable::createEntry(const SID_t &sid, const NID_t &nid,
                                                              const MacAddress &mac, simtime_t t, int type, bool is_consumer)
{
    Enter_Method("createEntry()");
    //首先根据信息生成pit表项
    PITentry nt(nid, t, mac, type, is_consumer);

    //检查这条pit表项是否已经存在（收到重复的get包），如果已经存在直接返回已存在的表项
    auto range = table.equal_range(sid);
    for (auto iter = range.first; iter != range.second; iter++)
    {
        if (nt.getMac() == iter->second.getMac())
            return *iter;
    }

    //在pit table中插入表项
    auto entry = std::make_pair(sid, nt);
    AddPITentry(entry);

    //生成定时器，如果当前sid对应的定时器已经存在则刷新定时器的时间
    auto timer = sids.find(sid);
    if (timer != sids.end())
    {
        cancelEvent(timer->second);
        scheduleAt(simTime() + t, timer->second);
    }
    //不存在，生成新的定时器
    else
    {
        cMessage *newTimer = new cMessage("timer");
        timers[newTimer] = sid;
        sids[sid] = newTimer;
        scheduleAt(simTime() + t, newTimer);
    }

    return entry;
}

void colorPendingGetTable::AddPITentry(const colorPendingGetTable::Entry &entry)
{
    table.insert(entry);
}

void colorPendingGetTable::RemoveEntry(const SID_t &sid)
{
    auto pointer = sids.find(sid)->second;
    timers.erase(pointer);
    cancelAndDelete(pointer);

    table.erase(sid);
    sids.erase(sid);
}

colorPendingGetTable::EntrysRange colorPendingGetTable::findPITentry(const SID_t &sid)
{
    return table.equal_range(sid);
}

bool colorPendingGetTable::isConsumer(const SID_t &sid)
{
    auto range = table.equal_range(sid);
    for (auto iter = range.first; iter != range.second; iter++)
    {
        if (iter->second.isConsumer())
            return true;
    }
    return false;
}
} // namespace inet
