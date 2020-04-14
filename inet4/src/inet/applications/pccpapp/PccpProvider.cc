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

#include "PccpProvider.h"

namespace inet {

Define_Module(PccpProvider);

void PccpProvider::initialize(int stage)
{
    ApplicationBase::initialize(stage);
    if(stage == INITSTAGE_LOCAL){
        localPort = par("port").intValue();
        pktLen = par("pktLen").intValue();
        pktNum = par("pktNum").intValue();
        startTime = par("startTime").doubleValue();

        start = new cMessage("start");
        nodeIndex = getParentModule()->getIndex();
        std::array<uint64_t, 2> hashValue;
        MurmurHash3_x64_128(&nodeIndex, sizeof(int), 0, hashValue.data());
        nid.setNID(hashValue);
        nid.test = nodeIndex;
    }
}

void PccpProvider::handleMessageWhenUp(cMessage *msg){
    if (msg->isSelfMessage())
        handleSelfMessage(msg);
    else {
        currentSocket->processMessage(msg);
    }
    if (operationalState == State::STOPPING_OPERATION )
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void PccpProvider::handleSelfMessage(cMessage *msg)
{
    if(msg == start) {
        while(content < pktNum) {
            generateAndCacheData({nodeIndex, content});
            content++;
        }
        delete start;
        start = nullptr;
    }
}

void PccpProvider::finish()
{
    ApplicationBase::finish();
}

void PccpProvider::refreshDisplay() const
{
    ApplicationBase::refreshDisplay();
}

void PccpProvider::generateAndCacheData(const SID &sid){

    Packet* pkt = new Packet("data");
    auto payload = makeShared<ByteCountChunk>(B(pktLen));
    pkt->insertAtBack(payload);
    currentSocket->CacheData(sid,pkt);
}

bool PccpProvider::isEnabled()
{
    return true;
}

void PccpProvider::handleStartOperation(LifecycleOperation *operation)
{
    if (isEnabled()) {
        currentSocket = new ColorSocket(&Protocol::color, gate("socketOut"));
        currentSocket->bind(&Protocol::color,nid, localPort);
        currentSocket->setCallback(this);
        scheduleAt(startTime, start);
    }
}

void PccpProvider::handleStopOperation(LifecycleOperation *operation)
{
}

void PccpProvider::handleCrashOperation(LifecycleOperation *operation)
{
}

void PccpProvider::socketDataArrived(ColorSocket *socket, Packet *packet)
{
    // will never arrive
}

void PccpProvider::socketClosed(ColorSocket *socket)
{
   delete currentSocket;
   currentSocket = nullptr;
}

} //namespace inet
