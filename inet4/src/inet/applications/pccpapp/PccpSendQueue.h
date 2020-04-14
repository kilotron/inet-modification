/*
 * PccpSendQueue.h
 *
 *  Created on: Apr 11, 2020
 *      Author: kilotron
 */

#ifndef INET_APPLICATIONS_PCCPAPP_PCCPSENDQUEUE_H_
#define INET_APPLICATIONS_PCCPAPP_PCCPSENDQUEUE_H_

#include "inet/common/INETDefs.h"
#include "inet/networklayer/icn/field/SID.h"
#include <map>
#include <queue>
#include <vector>

namespace inet {

/**
 * Send queue that manages app request.
 * request sid, retransmission timer and retransmission count are managed by this class.
 */
class INET_API PccpSendQueue : public cObject
{
private:
    std::map<SID, cMessage*> sidToTimerMap;
    std::map<cMessage*, SID> timerToSidMap;
    std::map<SID, int> sidRexmitCount;
    std::queue<SID> unsentSidQueue;

public:
    PccpSendQueue();
    ~PccpSendQueue();

    /**
     * Inserts in the queue the request the user wants to send. Returns false if
     * the queue is full.
     */
    bool enqueueRequest(const SID &sid);

    /**
     * Pre-condition: sid is in the queue and the corresponding request has been
     * sent once at least.
     */
    cMessage *findRexmitTimer(const SID &sid);

    /**
     * Pre-condition: rexmitTimer is obtained by calling findRexmitTimer().
     * Returns the SID associated with rexmitTimer.
     */
    SID& findSID(cMessage *rexmitTimer);

    /**
     * Returns true if there are unsent requests in the queue.
     */
    bool hasUnsentSID();

    /**
     * Pre-conditions: hasUnsentSID() returns true
     * Returns one request sid. The request should be sent by the caller after calling the method.
     */
    SID popOneUnsentSID();

    /**
     * Tells the queue that the data of the request sid is received, so it can be
     * removed from the queue.
     */
    void discard(const SID& sid);

    /**
     * Pre-condition: sid is in the queue and the corresponding request has been
     * sent at least once.
     * Returns the number of retransmissions of the request sid.
     */
    int getRexmitCount(const SID& sid);

    /**
     * Pre-condition: sid is in the queue and the corresponding request has been
     * sent at least once.
     * Increase the number of retransmissions of the request sid by 1.
     */
    void increaseRexmitCount(const SID& sid);

    /**
     * Returns the number of the requests that has been sent at least once.
     */
    int getSentRequestCount();

    /**
     * Returns all the timers in this queue.
     */
     void getAllTimers(std::vector<cMessage *>& v);
};

} // namespace inet



#endif /* INET_APPLICATIONS_PCCPAPP_PCCPSENDQUEUE_H_ */
