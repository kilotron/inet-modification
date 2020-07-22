/*
 * colorPendingGettable->cc
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
    for (auto &iter : *timers)
    {
        cancelAndDelete(iter.first);
        sids->erase(iter.second);
    }
    delete timers;
    delete table;
    delete sids;
}

colorPendingGetTable::~colorPendingGetTable()
{
    
}

void colorPendingGetTable::initialize(int stage)
{
    cSimpleModule::initialize(stage);
    if (stage == INITSTAGE_LOCAL)
    {
        table = new std::multimap<SID, PITentry>;
        timers = new timerTable;
        sids = new std::map<SID, cMessage *>;

        //得到指向路由表的指针
        auto name = getParentModule()->getFullPath();
        name = name + ".routingTable";
        auto path = name.c_str();
        auto mod = this->getModuleByPath(path);
        rt = dynamic_cast<ColorRoutingTable *>(mod);
    }
}

void colorPendingGetTable::handleMessage(cMessage *msg)
{
    //定时器触发，删除对应的pit 表项
    auto sid = timers->find(msg)->second;
    // rt->removeEntry(sid.getNidHead());
    RemoveEntry(sid);
    //删除定时器消息本身a
    cancelEvent(msg);
//    if (getParentModule()->getParentModule()->getIndex() == 1) {
//        int i;
//        i=0;
//    }
//    std::cout << "t=" << simTime() << ", node " << getParentModule()->getParentModule()->getIndex()
//            <<  ", pit timer expires, length=" << getLength() << endl;
}

void colorPendingGetTable::PrintPIT(std::ostream &out)
{
    for (auto iter = table->begin(); iter != table->end();)
    {
        auto range = table->equal_range(iter->first);
//        out << "SID is: " << iter->first.str() << endl;
        out << "NIDs is: ";
        for (auto it = range.first; it != range.second; it++)
            out << it->second.getNid().str() << "  ";
        out << endl;
        out << "TTL is: " << iter->second.getTTL();
        iter = range.second;
    }
}

bool colorPendingGetTable::haveEntry(const SID& sid, long nonce)
{
    auto iter = table->find(sid);
    if(iter != table->end() && iter->second.getNonce() == nonce)
        return true;
    else return false;
    
}

const colorPendingGetTable::Entry &colorPendingGetTable::createEntry(const SID &sid, const NID &nid,
                                                                     const MacAddress &mac, simtime_t t, int type, long Nonce, bool served, bool is_consumer)
{
//    if (getParentModule()->getParentModule()->getIndex() == 1) {
//            int i;
//            i=0;
//            std::cout << "timer=" << t << endl;
//        }
    Enter_Method("createEntry()");
    //首先根据信息生成pit表项
    PITentry nt(nid, t, mac, type, Nonce, served, is_consumer);

    //检查这条pit表项是否已经存在（收到重复的get包），如果已经存在直接返回已存在的表项
    auto range = table->equal_range(sid);
    for (auto iter = range.first; iter != range.second; iter++)
    {
        //每次插入前检查相同SID对应的条目是否已被服务过（数据包回传），是的情况下删除服务过的条目
        if (Nonce == iter->second.getNonce())
            return *iter;
  
    }

    //在pit table中插入表项
    auto entry = std::make_pair(sid, nt);
    AddPITentry(entry);

    //生成定时器，如果当前sid对应的定时器已经存在则刷新定时器的时间
    auto timer = sids->find(sid);
    if (timer != sids->end())
    {
        cancelEvent(timer->second);
        scheduleAt(simTime() + t, timer->second);
    }
    //不存在，生成新的定时器
    else
    {
        cMessage *newTimer = new cMessage("timer");
        (*timers)[newTimer] = sid;
        (*sids)[sid] = newTimer;
        scheduleAt(simTime() + t, newTimer);
    }

    return entry;
}

void colorPendingGetTable::AddPITentry(const colorPendingGetTable::Entry &entry)
{
    table->insert(entry);
}

void colorPendingGetTable::RemoveEntry(const SID &sid)
{
    auto pointer = sids->find(sid)->second;
    timers->erase(pointer);
    cancelAndDelete(pointer);

    table->erase(sid);
    sids->erase(sid);
}

void colorPendingGetTable::RemoveEntry(const std::multimap<SID, PITentry>::iterator iter)
{
    table->erase(iter);
}

colorPendingGetTable::EntrysRange colorPendingGetTable::findPITentry(const SID &sid)
{
    return table->equal_range(sid);
}

bool colorPendingGetTable::isConsumer(const SID &sid)
{
    auto range = table->equal_range(sid);
    for (auto iter = range.first; iter != range.second; iter++)
    {
        if (iter->second.isConsumer())
            return true;
    }
    return false;
}

bool colorPendingGetTable::servedForThisGet(const SID& sid, unsigned long Nonce)
{
    auto range = table->equal_range(sid);
    for (auto iter = range.first; iter != range.second; iter++)
    {
        if(iter->second.getNonce() == Nonce && iter->second.getServed())
            return true;
    }
    return false;
}

void colorPendingGetTable::SetServed(const SID &sid)
{
    auto range = table->equal_range(sid);
    for (auto iter = range.first; iter != range.second; iter++)
    {
            iter->second.setServed(true);
    }
}
} // namespace inet
