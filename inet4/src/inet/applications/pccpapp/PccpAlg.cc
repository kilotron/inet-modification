/*
 * PccpAlg.cc
 *
 *  Created on: Apr 11, 2020
 *      Author: kilotron
 */

#include "inet/applications/pccpapp/PccpAlg.h"

namespace inet {
namespace pccp {

#define MAX_REXMIT_COUNT 12
#define MAX_REXMIT_TIMEOUT 240
#define MIN_REXMIT_TIMEOUT 1.0

PccpAlg::PccpAlg()
{
}

void PccpAlg::processRexmitTimer(cMessage *timer)
{
    // Abort retransmission after max 12 retries.
    SID sid = sendQueue.findSID(timer);
    sendQueue.increaseRexmitCount(sid);
    if (sendQueue.getRexmitCount(sid) > MAX_REXMIT_COUNT) {
        EV << "Retransmission count exceeds " << MAX_REXMIT_COUNT << ", aborting.\n";
        // TODO 通知App超过最大重传次数
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

    EV << " to " << state.rexmit_timeout << "s, and cancelling RTT measurement\n";
    // TODO 重新schedule timer

    // cancel round-trip time measurement
    state.rtsid = nullptr;
    state.rtsid_sendtime = 0;

    // TODO 重传数据
}

void PccpAlg::requestSent(const SID& sid)
{
    // if retransmission timer not running, schedule it
    cMessage *timer = sendQueue.findRexmitTimer(sid);

    // start round-trip time measurement (if not already running)
    if (state.rtsid_sendtime == 0) {
        // remember this sid and when it was sent
        state.rtsid = &sid;
        state.rtsid_sendtime = simTime();
        EV << "Starting rtt measurement on sid=" << sid.str() << "\n";
    }

    // TODO 启动超时计时器
}

void PccpAlg::dataReceived(const SID& sid)
{
    if ( state.rtsid_sendtime != 0 && *(state.rtsid) == sid ) {
        EV << "Round-trip time measured on sid=" << sid.str() << ": "
           << floor((simTime() - state.rtsid_sendtime) * 1000 + 0.5) << "ms\n";
        // update RTT variables with new value
        rttMeasurementComplete(state.rtsid_sendtime, simTime());

        // measurement finished
        state.rtsid = nullptr;
        state.rtsid_sendtime = 0;
    }

    // cancel retransmission timer
    cMessage *timer = sendQueue.findRexmitTimer(sid);
    // TODO 取消timer

    // remove the request from sendQueue
    sendQueue.discard(sid);

    // TODO 调整拥塞窗口，发送新请求（如果可以的话）
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

    EV << "Measured RTT=" << (newRTT * 1000) << "ms, updated SRTT=" << (state.srtt * 1000)
       << "ms, new RTO=" << (rto * 1000) << "ms\n";
}

} // namespace pccp
} // namespace inet
