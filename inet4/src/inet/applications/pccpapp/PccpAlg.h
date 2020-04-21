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
#include "inet/applications/pccpapp/PccpApp.h"

namespace inet {

class PccpApp;

/**
 * Includes basic algorithms: retransmission, congestion window adjustment.
 */
class INET_API PccpAlg : public cObject, public ColorSocket::ICallback
{
    friend class PccpApp;

protected:
    PccpStateVariables state;
    PccpSendQueue sendQueue;
    PccpApp *pccpApp;    // app module, set by the app

private:
    /**
     * Send all of the requests in sendQueue if possible.
     */
    void sendRequestsToSocket();

    /**
     * Schedules retransmission timer and starts round-trip time measurement.
     */
    void requestSent(const SID& sid, bool isRexmit);

    /**
     * Retransmit GET, reset timer and increase rexmit count.
     */
    void retransmitRequest(const SID& sid);

    /**
     * Finishes round-trip time measurement, cancel retransmission timer and adjust
     * the congestion window.
     */
    void dataReceived(const SID& sid, Packet *packet);

    /**
     * Update state vars with new measured RTT value. Passing two simtime_t's
     * will allow rttMeasurementComplete() to do calculations.
     */
    void rttMeasurementComplete(simtime_t timeSent, simtime_t timeReceived);

public:

    PccpAlg();
    ~PccpAlg();

    // interface between PccpApp and PccpAlg
    void sendRequest(const SID &sid, int localPort, double sendInterval);

    /**
     * Performs retransmission and increases RTO
     */
    void processRexmitTimer(cMessage *timer);

    //INetworkSocket::ICallback:
    virtual void socketDataArrived(ColorSocket *socket, Packet *packet) override;
    virtual void socketClosed(ColorSocket *socket) override;
};

} // namespace inet

#endif /* INET_APPLICATIONS_PCCPAPP_PCCPALG_H_ */
