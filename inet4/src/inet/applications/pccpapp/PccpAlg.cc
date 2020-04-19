/*
 * PccpAlg.cc
 *
 *  Created on: Apr 11, 2020
 *      Author: kilotron
 */

#include "inet/applications/pccpapp/PccpAlg.h"
#include "inet/networklayer/common/SidTag_m.h"
#include "inet/networklayer/common/ClTag_m.h"
#include <iostream>
#include <vector>

namespace inet {

#define MAX_REXMIT_COUNT 12
#define MAX_REXMIT_TIMEOUT 240
#define MIN_REXMIT_TIMEOUT 1.0

PccpAlg::PccpAlg()
{
}

PccpAlg::~PccpAlg()
{
    std::vector<cMessage *> v;
    sendQueue.getAllTimers(v);
    for (auto it = v.begin(); it != v.end(); ++it) {
        cMessage *timer = *it;
        if (timer->isScheduled()) {
            pccpApp->cancelEvent(timer);
        }
    }
}

void PccpAlg::processRexmitTimer(cMessage *timer)
{
    pccpApp->emit(PccpApp::rexmitSignal, 1); // the second parameter can be any value
    std::cout << "retrans...";
    // Abort retransmission after max 12 retries.
    SID sid = sendQueue.findSID(timer);
    sendQueue.increaseRexmitCount(sid);
    if (sendQueue.getRexmitCount(sid) > MAX_REXMIT_COUNT) {
        EV << "Retransmission count exceeds " << MAX_REXMIT_COUNT << ", aborting.\n";
        pccpApp->maxRexmit(sid); // Tell app retransmission count exceeds MAX_REXMIT_COUNT.
        return;
    }

    EV << "Performing retransmission #" << sendQueue.getRexmitCount(sid)
            << "; increasing RTO from " << state.rexmit_timeout << "s ";

    // Karn's algorithm is implemented below:
    //  (1) don't measure RTT for retransmitted packets.
    //  (2) RTO should be doubled after retransmission ("exponential back-off")
    state.rexmit_timeout += state.rexmit_timeout;
    if (state.rexmit_timeout > MAX_REXMIT_TIMEOUT) {
        state.rexmit_timeout = MAX_REXMIT_TIMEOUT;
    }
    std::cout << "timeout=" << state.rexmit_timeout << endl;
    pccpApp->emit(PccpApp::rtoSignal, state.rexmit_timeout);

    EV << " to " << state.rexmit_timeout << "s, and canceling RTT measurement\n";

    // cancel round-trip time measurement
    state.rtsid_sendtime = 0;

    retransmitRequest(sid);
}

void PccpAlg::retransmitRequest(const SID& sid)
{
    cMessage *timer = sendQueue.findRexmitTimer(sid);
    pccpApp->currentSocket->sendGET(sid, pccpApp->localPort, pccpApp->sendInterval);
    pccpApp->scheduleTimeout(timer, state.rexmit_timeout);
    sendQueue.increaseRexmitCount(sid);
    pccpApp->emit(PccpApp::getSentSignal, 1);
}

void PccpAlg::sendRequestsToSocket()
{
    int effectiveWindow = int(state.window) - sendQueue.getSentRequestCount();
    while (effectiveWindow-- > 0 && sendQueue.hasUnsentSID()) {
        SID sidToSend = sendQueue.popOneUnsentSID();
        pccpApp->currentSocket->sendGET(sidToSend, pccpApp->localPort, pccpApp->sendInterval);
        requestSent(sidToSend);
    }
    pccpApp->emit(PccpApp::effectiveWindowSignal, effectiveWindow + 1);
}

void PccpAlg::requestSent(const SID& sid)
{
    pccpApp->emit(PccpApp::getSentSignal, 1);
    // if retransmission timer not running, schedule it
    cMessage *timer = sendQueue.findRexmitTimer(sid);
    pccpApp->scheduleTimeout(timer, state.rexmit_timeout);

    // start round-trip time measurement (if not already running)
    if (state.rtsid_sendtime == 0) {
        // remember this sid and when it was sent
        state.rtsid = sid;
        state.rtsid_sendtime = simTime();
        EV << "Starting rtt measurement.\n";
    }
}

void PccpAlg::dataReceived(const SID& sid, Packet *packet)
{
    pccpApp->emit(PccpApp::dataRcvdSignal, 1);
    if ( state.rtsid_sendtime != 0 && state.rtsid == sid ) {
        EV << "Round-trip time measured: "
           << floor((simTime() - state.rtsid_sendtime) * 1000 + 0.5) << "ms\n";
        // update RTT variables with new value
        rttMeasurementComplete(state.rtsid_sendtime, simTime());

        // measurement finished
        state.rtsid_sendtime = 0;
    }

    // cancel retransmission timer
    cMessage *timer = sendQueue.findRexmitTimer(sid);
    pccpApp->cancelEvent(timer);

    // remove the request from sendQueue
    sendQueue.discard(sid);

    // 调整拥塞窗口，发送新请求（如果可以的话）
    PccpClCode congestionLevel = packet->removeTag<ClInd>()->getCongestionLevel();

    if (congestionLevel != PccpClCode::CONGESTED) {
        state.num_continuous_congested = 0;
    } else {
        state.num_continuous_congested++;
    }

    if (congestionLevel == PccpClCode::FREE) {
        state.window += 1;
    } else if (congestionLevel == PccpClCode::BUSY_1) {
        state.window += 1 / state.window;
    } else if (congestionLevel == PccpClCode::BUSY_2) {
        state.window -= 1 / state.window;
    } else { // congestionLevel == PccpClCode::CONGESTED
        /* k: decrease factor, n: number of continuous congested packets
         * k = k0 if n >= n0
         * k = 1 - (1 - k0) * n / n0 if n < n0
         */
        double k;
        if (state.num_continuous_congested >= pccpApp->n0) {
            k = pccpApp->k0;
        } else {
            k = 1 - (1 - pccpApp->k0) * state.num_continuous_congested / pccpApp->k0;
        }
        state.window *= k;
    }
    pccpApp->emit(PccpApp::windowSignal, state.window);
    sendRequestsToSocket();
    std::cout << "window=" << state.window << endl;
    // notify app
    pccpApp->dataArrived(packet);
}

void PccpAlg::rttMeasurementComplete(simtime_t timeSent, simtime_t timeReceived)
{
    // update smoothed RTT estimate (srtt) and variance (rttvar)
    const double g = 0.125;    // 1 / 8; (1 - alpha) where alpha == 7 / 8;
    simtime_t newRTT = timeReceived - timeSent;
    simtime_t err = newRTT - state.srtt;

    // srtt(t+1) = srtt(t) * alpha + newRTT * (1 - alpha)
    // rttvar(t+1) = alpha * rttvar(t) + abs(err) * (1 - alpha)
    state.srtt += g * err;
    state.rttvar += g * (fabs(err) - state.rttvar);

    // assign RTO a new value
    simtime_t rto = state.srtt + 4 * state.rttvar;
    if (rto > MAX_REXMIT_TIMEOUT)
        rto = MAX_REXMIT_TIMEOUT;
    if (rto < MIN_REXMIT_TIMEOUT)
        rto = MIN_REXMIT_TIMEOUT;
    state.rexmit_timeout = rto;

    pccpApp->emit(PccpApp::srttSignal, state.srtt);
    pccpApp->emit(PccpApp::rttvarSignal, state.rttvar);
    pccpApp->emit(PccpApp::rtoSignal, state.rexmit_timeout);
    EV << "Measured RTT=" << (newRTT * 1000) << "ms, updated SRTT=" << (state.srtt * 1000)
       << "ms, new RTO=" << (rto * 1000) << "ms\n";
}

//INetworkSocket::ICallback
// socket bind is set by PccpApp
void PccpAlg::socketDataArrived(ColorSocket *socket, Packet *packet)
{
    auto sidInd = packet->getTag<SidInd>();
    SID sid = sidInd->getSid();
    dataReceived(sid, packet);
}

void PccpAlg::socketClosed(ColorSocket *socket)
{
   delete socket;
   pccpApp->currentSocket = nullptr;
}

// interface between PccpApp and PccpAlg
void PccpAlg::sendRequest(const SID &sid, int localPort, double sendInterval)
{
    //pccpApp->currentSocket->sendGET(sid, localPort, sendInterval);
    sendQueue.enqueueRequest(sid);
    sendRequestsToSocket();
}

} // namespace inet
