/*
 * PccpStateVariables.cc
 *
 *  Created on: Apr 11, 2020
 *      Author: kilotron
 */

#include "inet/applications/pccpapp/PccpStateVariables.h"

namespace inet {

PccpStateVariables::PccpStateVariables()
{
    rtsid_sendtime = 0;

    // Jacobson's alg: srtt must be initialized to 0, rttvar to a value which
    // will yield rto = 3s initially.
    srtt = 0;
    rttvar = 3.0 / 4.0;

    rexmit_timeout = 3;
    last_timeout_doubled_time = 0.0;

    window = 1.0; //删除
    num_continuous_congested = 0;
}

} // namespace inet
