/*
 * PccpStateVariables.cc
 *
 *  Created on: Apr 11, 2020
 *      Author: kilotron
 */

#include "inet/applications/pccpapp/PccpStateVariables.h"

namespace inet {
namespace pccp {

PccpStateVariables::PccpStateVariables()
{
    rtsid = nullptr;
    rtsid_sendtime = 0;

    // Jacobson's alg: srtt must be initialized to 0, rttvar to a value which
    // will yield rto = 3s initially.
    srtt = 0;
    rttvar = 3.0 / 4.0;

    rexmit_timeout = 3.0;

    // TODO 目前是固定窗口。需要改为随拥塞状态改变的窗口
    window = 100;
}

} // namespace pccp
} // namespace inet
