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

namespace inet {
namespace pccp {

Define_Module(PccpApp);

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
    if (msg->isSelfMessage())
        handleSelfMessage(msg);
    else {
        currentSocket->processMessage(msg); // TODO
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
}

void PccpApp::finish()
{
    ApplicationBase::finish();
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

} // namespace pccp
} //namespace inet
