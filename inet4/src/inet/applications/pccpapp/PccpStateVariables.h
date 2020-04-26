/*
 * PccpStateVariables.h
 *
 *  Created on: Apr 11, 2020
 *      Author: kilotron
 */

#ifndef INET_APPLICATIONS_PCCPAPP_PCCPSTATEVARIABLES_H_
#define INET_APPLICATIONS_PCCPAPP_PCCPSTATEVARIABLES_H_

#include "inet/common/INETDefs.h"
#include "inet/networklayer/icn/field/SID.h"

namespace inet{

/**
 * State Variables for Pccp
 */
class INET_API PccpStateVariables : public cObject
{
public:
    PccpStateVariables();

    // round-trip time measurements
    SID rtsid;    // SID for RTT measurement(meaningless if RTT measurement is not running)
    simtime_t rtsid_sendtime;    // time when rtsid was sent(0 if RTT measurement is not running)

    // round-trip time estimation
    simtime_t srtt;    // smoothed round-trip time
    simtime_t rttvar;  // variance of round-trip time

    simtime_t rexmit_timeout;   // current retransmission timeout
    simtime_t last_timeout_doubled_time;// last time when timeout occurs
    simtime_t last_cong_rcvd; // last time when a packet is marked congested

    double window;    // congestion window

    int num_continuous_congested;
};

} // namespace inet

#endif /* INET_APPLICATIONS_PCCPAPP_PCCPSTATEVARIABLES_H_ */
