/*
 * PccpSendQueue.cc
 *
 *  Created on: Apr 11, 2020
 *      Author: kilotron
 */

#include "inet/applications/pccpapp/PccpSendQueue.h"
#include <iostream>
namespace inet {

Register_Class(PccpSendQueue);

PccpSendQueue::PccpSendQueue()
{
}

PccpSendQueue::~PccpSendQueue()
{
    for (auto it = sidToTimerMap.begin(); it != sidToTimerMap.end(); ++it) {
        delete it->second;  // delete timers.
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
    return sidToTimerMap.find(sid)->second;
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
    return sid;
}

void PccpSendQueue::discard(const SID& sid)
{
    cMessage *timer = sidToTimerMap.find(sid)->second;
    sidToTimerMap.erase(sid);
    timerToSidMap.erase(timer);
    sidRexmitCount.erase(sid);
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

int PccpSendQueue::getSentRequestCount()
{
    return sidToTimerMap.size();
}

} // namespace inet


