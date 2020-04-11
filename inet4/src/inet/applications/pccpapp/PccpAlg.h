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
namespace pccp {

class PccpApp;

/**
 * Includes basic algorithms: retransmission, congestion window adjustment.
 */
class INET_API PccpAlg : public cObject, public ColorSocket::ICallback
{
protected:
    PccpStateVariables state;
    PccpSendQueue sendQueue;

public:
    PccpApp *pccpApp;    // app module, set by the app

    PccpAlg();
    ~PccpAlg();

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

    //INetworkSocket::ICallback:
    virtual void socketDataArrived(ColorSocket *socket, Packet *packet) override;
    virtual void socketClosed(ColorSocket *socket) override;

    // interface between PccpApp and PccpAlg
    void sendRequest(const SID &sid, int localPort, double sendInterval);
};

} // namespace pccp
} // namespace inet

#endif /* INET_APPLICATIONS_PCCPAPP_PCCPALG_H_ */
