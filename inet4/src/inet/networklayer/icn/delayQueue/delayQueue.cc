/*
 * delayQueue.cc
 *
 *  Created on: 2019年12月20日
 *      Author: hiro
 */

#include "delayQueue.h"
#include "inet/networklayer/icn/color/Data_m.h"
#include "inet/networklayer/icn/color/Get_m.h"

namespace inet
{
void delayQueue::insert(Packet *pkt, TYPE type, simtime_t time, int forwardTimes, MacAddress mac)
{

    if (priQueue.size() == 0)
    {
        owner->cancelEvent(timer);
        owner->scheduleAt(time, timer);

        if (type == DATA)
            priQueue.insert(priQueue.begin(), pendPkt(pkt, DATA, time, forwardTimes));
        else if (type == GET)
            priQueue.insert(priQueue.begin(), pendPkt(pkt, GET, time, forwardTimes));
    }
    else
    {
        for (auto iter = priQueue.begin(); iter != priQueue.end(); iter++)
        {
            if (iter->sendtime < time)
                continue;
            else
            {
                if (iter == priQueue.begin())
                {
                    owner->cancelEvent(timer);
                    owner->scheduleAt(time, timer);
                }
                if (type == DATA)
                    priQueue.insert(iter, pendPkt(pkt, DATA, time, forwardTimes));
                else if (type == GET)
                    priQueue.insert(iter, pendPkt(pkt, GET, time, forwardTimes));
                break;
            }
        }
    }
}

Packet *delayQueue::popAtFront()
{
    auto begin = priQueue.begin();
    Packet *packet = begin->pkt;

    //如果收到的转发次数已经超过临界值，把packet资源释放返回空指针
    if (begin->tc < 1 && packet != nullptr)
    {
        begin->releasePkt();
        packet = nullptr;
    }

    //删除队首
    priQueue.erase(begin);

    //重新启动计时器
    if (priQueue.size() > 0)
        owner->scheduleAt(priQueue.begin()->sendtime, timer);
    return packet;
}

list<delayQueue::pendPkt>::iterator delayQueue::have(Packet *pkt)
{
    int type;
    Get *getPkt;
    Data *dataPkt;

    const auto &head = pkt->peekAtFront<Chunk>(B(78), 0);
    auto pointer = head.get();

    //转换为raw pointer
    Chunk *chunk = const_cast<Chunk *>(pointer);

    if ((getPkt = dynamic_cast<Get *>(chunk)) != nullptr)
    {
        type = 0;
    }
    else
    {
        dataPkt = dynamic_cast<Data *>(chunk);
        type = 1;
    }

    for (auto iter = priQueue.begin(); iter != priQueue.end(); iter++)
    {
        auto packet = iter->pkt;
        if (iter->type == GET && type == 0)
        {
            const auto &packetHead = packet->peekAtFront<Get>();
            if (getPkt->getSid() == packetHead->getSid() && getPkt->getSource() == packetHead->getSource() && getPkt->getNonce() == packetHead->getNonce())
            {
                return iter;
            }
        }
        else if (iter->type == DATA && type == 1)
        {
            const auto &packetHead = packet->peekAtFront<Data>();
            if (dataPkt->getSid() == packetHead->getSid())
            {
                return iter;
            }
        }
    }

    return priQueue.end();
}

bool delayQueue::check_and_delete(const SID &sid)
{
    for (auto iter = priQueue.begin(); iter != priQueue.end(); iter++)
    {
        auto packet = iter->pkt;
        if (iter->type == GET)
        {
            const auto &packetHead = packet->peekAtFront<Get>();
            if (sid == packetHead->getSid())
            {
                iter->releasePkt();
                iter->pkt = nullptr;
                return true;
            }
        }
    }
    return false;
}

bool delayQueue::check_and_decrease(Packet *pkt)
{
    auto iter = have(pkt);
    if (iter == priQueue.end())
    {
        return false;
    }
    else
    {
        iter->tc--;
        return true;
    }
}

void delayQueue::cancelDelayeForwarding(const SID &sid)
{
    for (auto iter = priQueue.begin(); iter != priQueue.end(); iter++)
    {
        if (iter->type == GET)
        {
            const auto &packetHead = iter->pkt->peekAtFront<Get>();
            if (packetHead->getSid() == sid)
            {
                priQueue.erase(iter);
            }
        }
    }
}

} // namespace inet
