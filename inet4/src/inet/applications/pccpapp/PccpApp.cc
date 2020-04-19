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

#include "PccpApp.h"
#include "inet/networklayer/common/SidTag_m.h"
#include <iostream>

namespace inet {

Define_Module(PccpApp);

simsignal_t PccpApp::rtoSignal = registerSignal("rto");
simsignal_t PccpApp::srttSignal = registerSignal("srtt");
simsignal_t PccpApp::rttvarSignal = registerSignal("rttvar");
simsignal_t PccpApp::windowSignal = registerSignal("window");
simsignal_t PccpApp::effectiveWindowSignal = registerSignal("effectiveWindow");
simsignal_t PccpApp::rexmitSignal = registerSignal("rexmit");
simsignal_t PccpApp::dataRcvdSignal = registerSignal("dataRcvd");
simsignal_t PccpApp::getSentSignal = registerSignal("getSent");

PccpApp::PccpApp() {
    pccpAlg = new PccpAlg();
    pccpAlg->pccpApp = this;
};

PccpApp::~PccpApp() {
    delete pccpAlg;
};

void PccpApp::initialize(int stage)
{
    ApplicationBase::initialize(stage);
    if(stage == INITSTAGE_LOCAL){
        localPort = par("port").intValue();
        destIndex = par("destAddr").intValue();
        requestNum = par("requestNum").intValue();
        path=par("RSTpath").stdstringValue();
        sendInterval = par("sendInterval").doubleValue();
        startTime = par("startTime").doubleValue();
        stopTime = par("stopTime").doubleValue();

        timer = new cMessage("sendGET");
        start = new cMessage("start");

        int nodeIndex = getParentModule()->getIndex();
        std::array<uint64_t, 2> hashValue;
        MurmurHash3_x64_128(&nodeIndex, sizeof(int), 0, hashValue.data());
        nid.setNID(hashValue);
        nid.test = nodeIndex;
    }
}

void PccpApp::handleMessageWhenUp(cMessage *msg){
    if (msg->isSelfMessage()) {
        handleSelfMessage(msg);  // including rexmit timer
    }
    else {
        currentSocket->processMessage(msg);
    }
    if (operationalState == State::STOPPING_OPERATION )
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void PccpApp::handleSelfMessage(cMessage *msg)
{
    if(msg == start)
    {
        sendRequest({destIndex,content});
        scheduleAt(simTime()+sendInterval, timer);
        delete start;
        start = nullptr;
    }
    else if(msg == timer)
    {
        if(content < requestNum)
        {
            content++;
            sendRequest({destIndex,content});
            scheduleAt(simTime()+sendInterval, timer);
        }
        else cancelEvent(timer);
    }
    else if (strcmp(msg->getName(), "REXMIT") == 0) {
        pccpAlg->processRexmitTimer(msg);
    }
}

void PccpApp::finish()
{
    ApplicationBase::finish();
    if (timer->isScheduled()) {
        cancelEvent(timer);
    }
    delete timer;
}

void PccpApp::refreshDisplay() const
{
    ApplicationBase::refreshDisplay();
}

void PccpApp::sendRequest(const SID &sid)
{
    pccpAlg->sendRequest(sid, localPort, sendInterval);
}

// interface between PccpApp and PccpAlg
void PccpApp::maxRexmit(const SID& sid)
{
    // TODO 定义最大超时次数行为
}

void PccpApp::dataArrived(Packet *packet)
{
    // TODO 在这里统计
    std::cout << "data arrived, time=" << simTime() << endl;
    delete packet;
}

void PccpApp::scheduleTimeout(cMessage *timer, simtime_t timeout)
{
    scheduleAt(simTime() + timeout, timer);
}

bool PccpApp::isEnabled()
{
    return destIndex >= 0;
}

// Lifecycle methods

void PccpApp::handleStartOperation(LifecycleOperation *operation)
{
    if (isEnabled())
    {
        currentSocket = new ColorSocket(&Protocol::color, gate("socketOut"));
        currentSocket->bind(&Protocol::color, nid, localPort);
        currentSocket->setCallback(pccpAlg);
        scheduleAt(startTime, start);
    }
}

void PccpApp::handleStopOperation(LifecycleOperation *operation)
{
}

void PccpApp::handleCrashOperation(LifecycleOperation *operation)
{
}

} //namespace inet
