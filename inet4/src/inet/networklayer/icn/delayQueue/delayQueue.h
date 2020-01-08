/*
 * delayQueue.h
 *
 *  Created on: 2019年12月20日
 *      Author: hiro
 */

#ifndef INET_NETWORKLAYER_ICN_DELAYQUEUE_DELAYQUEUE_H_
#define INET_NETWORKLAYER_ICN_DELAYQUEUE_DELAYQUEUE_H_

#include <list>

#include "inet/common/INETDefs.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/lifecycle/ILifecycle.h"
#include "inet/networklayer/icn/color/colorfields_m.h"
#include "inet/linklayer/common/MacAddress_m.h"

namespace inet
{

using std::list;

class delayQueue
{
public:
    struct pendPkt
    {
        Packet *pkt;
        TYPE type;
        simtime_t sendtime;
        int tc;
        MacAddress nextHop;

        pendPkt(Packet *pkt, TYPE type, simtime_t sendtime, int tc , MacAddress mac = MacAddress::BROADCAST_ADDRESS) : pkt(pkt), type(type), sendtime(sendtime), tc(tc), nextHop(mac) {}

        void releasePkt() { 
            delete pkt;
            pkt = nullptr;
        }
    };

private:
    list<pendPkt> priQueue;
    cMessage *timer;
    cSimpleModule *owner;

public:
    delayQueue(cMessage *timer, cSimpleModule *owner) : priQueue(), timer(timer), owner(owner) {}

    delayQueue() : priQueue(), timer(nullptr), owner(nullptr) {}

    //设置计时器和延迟队列的拥有者
    void setTimerAndOwner(cMessage *t, cSimpleModule *o) { 
        timer = t;
        owner = o;
    }

    //新插入一个包，和延迟发送的时间, 侦听发送上限
    void insert(Packet *pkt, TYPE type, simtime_t time, int forwardTimes = 1, MacAddress mac = MacAddress::BROADCAST_ADDRESS);

    //弹出延迟队列的队首元素
    Packet *popAtFront();

    //检查延迟队列中是否有此包，若有返回指向迭代器，若无返回end迭代器
    list<delayQueue::pendPkt>::iterator have(Packet *pkt);

    //检查延迟队列中是否有此包，并且计数器递减
    bool check_and_decrease(Packet *pkt);

 
};

} // namespace inet
#endif /* INET_NETWORKLAYER_ICN_DELAYQUEUE_DELAYQUEUE_H_ */
