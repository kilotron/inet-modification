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

#include "PccpIoTApp.h"
#include "inet/networklayer/common/SidTag_m.h"
#include <iostream>

namespace inet {

Define_Module(PccpIoTApp);

PccpIoTApp::PccpIoTApp() {
}

PccpIoTApp::~PccpIoTApp() {
}

void PccpIoTApp::initialize(int stage)
{
    PccpApp::initialize(stage);
    if(stage == INITSTAGE_LOCAL){
        transNum = par("transNum");
        sleepTime = par("sleepTime").doubleValue();
        appIndex = getIndex();
        localPort += appIndex;
    }
}

void PccpIoTApp::handleSelfMessage(cMessage *msg)
{
    if(msg == start)
    {
        sendRequest({destIndex, content + requestNum * appIndex});
        scheduleAt(simTime() + exponential(sendInterval), timer);
        delete start;
        start = nullptr;
    }
    else if(msg == timer)
    {
        if(content < requestNum - 1)    // msg==start时已经发过1个
        {
            content++;
            sendRequest({destIndex,content + requestNum * appIndex});
            if ((content % transNum) == (transNum - 1)) { // sleep
                scheduleAt(simTime() + sleepTime, timer);
            } else {
                scheduleAt(simTime() + exponential(sendInterval), timer);
            }
        }
        else cancelEvent(timer);
    }
    else if (strcmp(msg->getName(), "REXMIT") == 0) {
        pccpAlg->processRexmitTimer(msg);
    }
}

} /* namespace inet */
