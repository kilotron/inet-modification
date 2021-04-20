//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "PccpBurstApp.h"
#include "inet/networklayer/common/SidTag_m.h"
#include <iostream>

namespace inet {

Define_Module(PccpBurstApp);

PccpBurstApp::PccpBurstApp() {
}

PccpBurstApp::~PccpBurstApp() {
}

void PccpBurstApp::initialize(int stage)
{
    PccpApp::initialize(stage);
    if(stage == INITSTAGE_LOCAL){
//        transNum = par("transNum");
//        sleepTime = par("sleepTime").doubleValue();
//        appIndex = getIndex();
//        localPort += appIndex;
    }
}

double PccpBurstApp::getSendInterval()
{
    const double offset = -3.0; // start after t = 3.0
    double time = simTime().dbl() + offset;
    const double FREQ_LOW = 125; // 1Mbps
    const double FREQ_HIGH = 500; // 8Mbps
    double frequency;
    double interval;
//    burst
//    if ((time >= 30 && time < 32) || (time >= 62 && time < 64) || (time >= 94 && time < 96)) {
//        frequency = FREQ_HIGH;
//    } else {
//        frequency = FREQ_LOW;
//    }
//    high to low
//    if ((time < 30 )) {
//        frequency = FREQ_HIGH;
//    } else {
//        frequency = FREQ_LOW;
//    }
    frequency = 2000;
    interval = 1.0 / frequency;
    return exponential(interval);
}

void PccpBurstApp::handleSelfMessage(cMessage *msg)
{
    if(msg == start)
    {
        sendRequest({destIndex, content});
        scheduleAt(simTime() + getSendInterval(), timer);
        delete start;
        start = nullptr;
    }
    else if(msg == timer)
    {
        if(content < requestNum - 1)    // msg==start时已经发过1个
        {
            content++;
            sendRequest({destIndex,content});
            scheduleAt(simTime() + getSendInterval(), timer);
        }
        else cancelEvent(timer);
    }
    else if (strcmp(msg->getName(), "REXMIT") == 0) {
        pccpAlg->processRexmitTimer(msg);
    }
}

} /* namespace inet */
