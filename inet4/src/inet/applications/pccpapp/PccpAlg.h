/*
 * PccpAlg.h
 *
 *  Created on: Apr 11, 2020
 *      Author: kilotron
 */

#ifndef INET_APPLICATIONS_PCCPAPP_PCCPALG_H_
#define INET_APPLICATIONS_PCCPAPP_PCCPALG_H_

#include "inet/common/INETDefs.h"
#include "inet/applications/pccpapp/PccpStateVariables.h"
#include "inet/applications/pccpapp/PccpSendQueue.h"

namespace inet {
namespace pccp {

/**
 * Includes basic algorithms: retransmission, congestion window adjustment.
 */
class INET_API PccpAlg : public cObject
{
protected:
    PccpStateVariables state;
    PccpSendQueue sendQueue;

public:
    PccpAlg();

    /**
     * Performs retransmission and increases RTO
     */
    void processRexmitTimer(cMessage *timer);

    /**
     * Schedules retransmission timer and starts round-trip time measurement.
     */
    void requestSent(const SID& sid);

    /**
     * Finishes round-trip time measurement, cancel retransmission timer and adjust
     * the congestion window.
     */
    void dataReceived(const SID& sid);

    /**
     * Update state vars with new measured RTT value. Passing two simtime_t's
     * will allow rttMeasurementComplete() to do calculations.
     */
    void rttMeasurementComplete(simtime_t timeSent, simtime_t timeReceived);
};

} // namespace pccp
} // namespace inet

#endif /* INET_APPLICATIONS_PCCPAPP_PCCPALG_H_ */
