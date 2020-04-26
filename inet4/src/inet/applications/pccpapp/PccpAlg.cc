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

#define MAX_REXMIT_TIMEOUT 1.0
#define MIN_REXMIT_TIMEOUT 0.5
#define VERY_LARGE_WINDOW 2e+9

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

void PccpAlg::initializeState()
{
    state.window = pccpApp->initialWindowSize;
    if (!pccpApp->congestionControlEnabled) {
        state.window = VERY_LARGE_WINDOW;
    }
}

void PccpAlg::processRexmitTimer(cMessage *timer)
{
    pccpApp->emit(PccpApp::timeoutSignal, 1);
    std::cout << "timeout!";
    SID sid = sendQueue.findSID(timer);

    // cancel round-trip time measurement if RTT measurement is running
    // and RTT of this request sid is being measured.
    if (state.rtsid_sendtime != 0 && state.rtsid == sid) {
        state.rtsid_sendtime = 0;
    }

    // Abort retransmission after max 12 retries.
    sendQueue.increaseRexmitCount(sid);
    if (sendQueue.getRexmitCount(sid) > pccpApp->maxRexmitLimit) {
        sendQueue.discard(sid);
        EV << "Retransmission count exceeds " << pccpApp->maxRexmitLimit << ", aborting.\n";
        pccpApp->maxRexmit(sid); // Tell app retransmission count exceeds MAX_REXMIT_COUNT.
        sendRequestsToSocket();  // if there are no newly arrived requests
        return;
    }

    EV << "Performing retransmission #" << sendQueue.getRexmitCount(sid);

    // Karn's algorithm is implemented below:
    //  (1) don't measure RTT for retransmitted packets.
    //  (2) RTO should be doubled after retransmission ("exponential back-off")
    // *(3) RTO is doubled only once in one RTO
    if ( simTime() > state.last_timeout_doubled_time + state.rexmit_timeout ) {
        EV << "; increasing RTO from " << state.rexmit_timeout << "s ";
        state.rexmit_timeout += state.rexmit_timeout;
        if (state.rexmit_timeout > MAX_REXMIT_TIMEOUT) {
            state.rexmit_timeout = MAX_REXMIT_TIMEOUT;
        }
        state.last_timeout_doubled_time = simTime();
        pccpApp->emit(PccpApp::rtoSignal, state.rexmit_timeout);
        EV << " to " << state.rexmit_timeout << "s, and canceling RTT measurement\n";
    }

    retransmitRequest(sid);
}

void PccpAlg::retransmitRequest(const SID& sid)
{
    int effectiveWindow = int(state.window) - sendQueue.getSentRequestCount();
    if (effectiveWindow < 0) { // 等于0时可重传，小于零说明窗口缩小，不能重传
        sendQueue.enqueueRexmit(sid);
        return;
    }
    cMessage *timer = sendQueue.findRexmitTimer(sid);
    pccpApp->currentSocket->sendGET(sid, pccpApp->localPort, pccpApp->sendInterval);
    pccpApp->scheduleTimeout(timer, state.rexmit_timeout);
    //sendQueue.increaseRexmitCount(sid);
    pccpApp->emit(PccpApp::getSentSignal, 1);
    pccpApp->emit(PccpApp::rexmitSignal, 1); // the second parameter can be any value
}

// 窗口改变或有app数据到达时调用
void PccpAlg::sendRequestsToSocket()
{
    int effectiveWindow = int(state.window) - sendQueue.getSentRequestCount();
//    pccpApp->emit(PccpApp::effectiveWindowSignal, effectiveWindow);
    // 先检查重传队列
    while (effectiveWindow > 0 && sendQueue.hasUnsentRexmit()) {
        SID sidToSend = sendQueue.popOneUnsentRexmit();
        pccpApp->currentSocket->sendGET(sidToSend, pccpApp->localPort, pccpApp->sendInterval);
        requestSent(sidToSend, true);
        effectiveWindow--;
    }

    while (effectiveWindow > 0 && sendQueue.hasUnsentSID()) {
        SID sidToSend = sendQueue.popOneUnsentSID();
        pccpApp->currentSocket->sendGET(sidToSend, pccpApp->localPort, pccpApp->sendInterval);
        requestSent(sidToSend, false);
        effectiveWindow--;
    }
    pccpApp->emit(PccpApp::effectiveWindowSignal, effectiveWindow + 1);
}

void PccpAlg::requestSent(const SID& sid, bool isRexmit)
{
    pccpApp->emit(PccpApp::getSentSignal, 1);
    if (isRexmit) {
        pccpApp->emit(PccpApp::rexmitSignal, 1);
    }

    // if retransmission timer not running, schedule it
    cMessage *timer = sendQueue.findRexmitTimer(sid);
    pccpApp->scheduleTimeout(timer, state.rexmit_timeout);

    // start round-trip time measurement (if not already running)
    if (state.rtsid_sendtime == 0 && !isRexmit) {
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
    if (timer != nullptr) {
        pccpApp->cancelEvent(timer);
        // remove the request from sendQueue
        sendQueue.discard(sid);
    }

    // 调整拥塞窗口，发送新请求（如果可以的话）
    PccpClCode congestionLevel = packet->removeTag<ClInd>()->getCongestionLevel();
    pccpApp->emit(PccpApp::congestionLevelSignal, int(congestionLevel));

    if (congestionLevel != PccpClCode::CONGESTED) {
        state.num_continuous_congested = 0;
    } else {
        state.num_continuous_congested++;
    }

    if (congestionLevel == PccpClCode::FREE) {
        state.window += 1 / state.window;
    } else if (congestionLevel == PccpClCode::BUSY_1) {
        //state.window += 1 / state.window; // 保持不变
    } else if (congestionLevel == PccpClCode::BUSY_2) {
        state.window -= 1 / state.window;
    } else { // congestionLevel == PccpClCode::CONGESTED
        if (simTime() > state.last_cong_rcvd + state.srtt) {
            state.last_cong_rcvd = simTime();
            state.window *= 0.5;
        }
    }
    if (state.window < 1.0) {
        state.window = 1.0;
    }
    if (!pccpApp->congestionControlEnabled) {
        state.window = VERY_LARGE_WINDOW;
    }
    pccpApp->emit(PccpApp::windowSignal, state.window);
    sendRequestsToSocket();

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
