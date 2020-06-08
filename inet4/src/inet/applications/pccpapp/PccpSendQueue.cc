/*
 * PccpSendQueue.cc
 *
 *  Created on: Apr 11, 2020
 *      Author: kilotron
 */

#include "inet/applications/pccpapp/PccpSendQueue.h"
#include <iostream>
namespace inet {

TimeoutRequest::TimeoutRequest(const SID& sid, cMessage *timer, int rexmit_count, simtime_t first_trans_time)
{
    this->sid = sid;
    this->timer = timer;
    this->rexmit_count = rexmit_count;
    this->first_trans_time = first_trans_time;
}

Register_Class(PccpSendQueue);

PccpSendQueue::PccpSendQueue()
{
}

PccpSendQueue::~PccpSendQueue()
{
    for (auto it = sidToTimerMap.begin(); it != sidToTimerMap.end(); ++it) {
        delete it->second;  // delete timers. timers are canceled by PccpAlg
    }
}

bool PccpSendQueue::enqueueRequest(const SID& sid)
{
    unsentSidQueue.push(sid);
    // TODO: 限制队列长度，目前没有限制，直接返回true表示请求已经加入队列。
    return true;
}

cMessage *PccpSendQueue::findRexmitTimer(const SID& sid)
{
    auto it = sidToTimerMap.find(sid);
    if (it == sidToTimerMap.end()) {
        return nullptr; // 不在窗口内，或已达到最大重传次数被丢弃，返回null表示不需要取消计时器。
    } else {
        return it->second;
    }
}

SID& PccpSendQueue::findSID(cMessage *rexmitTimer)
{
    return timerToSidMap.find(rexmitTimer)->second;
}

bool PccpSendQueue::hasUnsentSID()
{
    return !unsentSidQueue.empty();
}

SID PccpSendQueue::popOneUnsentSID()
{
    SID sid = unsentSidQueue.front();
    unsentSidQueue.pop();
    cMessage *timer = new cMessage("REXMIT");
    sidToTimerMap.insert(std::pair<SID, cMessage*>(sid, timer));
    timerToSidMap.insert(std::pair<cMessage*, SID>(timer, sid));
    sidRexmitCount.insert(std::pair<SID, int>(sid, 0));
    firstTransTime.insert(std::pair<SID, simtime_t>(sid, simTime()));
    return sid;
}

void PccpSendQueue::discard(const SID& sid)
{
    cMessage *timer = sidToTimerMap.find(sid)->second;
    sidToTimerMap.erase(sid);
    timerToSidMap.erase(timer);
    sidRexmitCount.erase(sid);
    firstTransTime.erase(sid);
    delete timer;
}

int PccpSendQueue::getRexmitCount(const SID& sid)
{
    return sidRexmitCount[sid];
}

void PccpSendQueue::increaseRexmitCount(const SID& sid)
{
    sidRexmitCount[sid] = sidRexmitCount[sid] + 1;
}

simtime_t PccpSendQueue::getFirstTransTime(const SID& sid)
{
    return firstTransTime[sid];
}

int PccpSendQueue::getSentRequestCount()
{
    return sidToTimerMap.size();
}

void PccpSendQueue::getAllTimers(std::vector<cMessage *>& v)
{
    for (auto it = sidToTimerMap.begin(); it != sidToTimerMap.end(); ++it) {
        v.push_back(it->second);
    }
}

void PccpSendQueue::enqueueRexmit(const SID& sid)
{
    cMessage *timer = findRexmitTimer(sid);
    rexmitQueue.push(TimeoutRequest(sid, timer, getRexmitCount(sid), getFirstTransTime(sid)));
    sidToTimerMap.erase(sid);
    timerToSidMap.erase(timer);
    sidRexmitCount.erase(sid);
    firstTransTime.erase(sid);
}

bool PccpSendQueue::hasUnsentRexmit()
{
    return !rexmitQueue.empty();
}

SID PccpSendQueue::popOneUnsentRexmit()
{
    TimeoutRequest r = rexmitQueue.front();
    rexmitQueue.pop();
    sidToTimerMap.insert(std::pair<SID, cMessage*>(r.sid, r.timer));
    timerToSidMap.insert(std::pair<cMessage*, SID>(r.timer, r.sid));
    sidRexmitCount.insert(std::pair<SID, int>(r.sid, r.rexmit_count));
    firstTransTime.insert(std::pair<SID, simtime_t>(r.sid, r.first_trans_time));
    return r.sid;
}

} // namespace inet


