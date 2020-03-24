/*
 * SimpleProvider.cc
 *
 *  Created on: Mar 24, 2020
 *      Author: hiro
 */


#include <algorithm>
#include <fstream>

#include "inet/applications/ICNapp/SimpleProvider.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/Protocol.h"
#include "inet/common/ProtocolGroup.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "inet/networklayer/common/EchoPacket_m.h"
#include "inet/networklayer/common/InterfaceEntry.h"
#include "inet/networklayer/common/IpProtocolId_m.h"
#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/networklayer/icn/field/NID.h"
#include "inet/applications/common/SocketTag_m.h"

namespace inet
{
Define_Module(SimpleProvider);



void SimpleProvider::initialize(int stage)
{
    ApplicationBase::initialize(stage);
    if(stage == INITSTAGE_LOCAL){
        pktLen = par("pktLen").intValue();
        pktNum = par("pktNum").intValue();
 
        startTime = par("startTime").doubleValue();

        localPort = par("port").intValue();

        start = new cMessage("start");

        nodeIndex = getParentModule()->getIndex();
        std::array<uint64_t, 2> hashValue;
        MurmurHash3_x64_128(&nodeIndex, sizeof(int), 0, hashValue.data());
        nid.setNID(hashValue);
        nid.test = nodeIndex;
    }
}

void SimpleProvider::handleMessageWhenUp(cMessage *msg){
    if (msg->isSelfMessage())
        handleSelfMessage(msg);
    else {
        currentSocket->processMessage(msg);
    }
    if (operationalState == State::STOPPING_OPERATION )
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void SimpleProvider::handleSelfMessage(cMessage *msg)
{
    if(msg == start)
    {
        while(content < pktNum)
        {
            generateAndCacheData({nodeIndex, content});
            content++;
        }
    }
}

void SimpleProvider::socketDataArrived(ColorSocket *socket, Packet *packet)
{

    // Recorder.delayArray.push_back((simTime().dbl() - Recorder.Delays[sid].dbl()) * 1000);
    // std::cout << "receive the packet successfully!" << endl;
    // std::cout << "delay is " << (simTime().dbl() - Recorder.Delays[sid].dbl()) * 1000 << " ms" << endl;

    // //移除Delays中的此sid对应的delay
    // Recorder.Delays.erase(sid);

    //统计吞吐量
}

void SimpleProvider::socketClosed(ColorSocket *socket)
{

   delete currentSocket;
   currentSocket = nullptr;
}

void SimpleProvider::handleStartOperation(LifecycleOperation *operation)
{
    if (isEnabled())
    {
        currentSocket = new ColorSocket(&Protocol::color, gate("socketOut"));
        currentSocket->bind(&Protocol::color,nid, localPort);
        currentSocket->setCallback(this);
        scheduleAt(startTime, start);
    }

}

void SimpleProvider::refreshDisplay() const
{
    ApplicationBase::refreshDisplay();

}

void SimpleProvider::generateAndCacheData(const SID &sid){

    Packet* pkt = new Packet("data");
    auto payload = makeShared<ByteCountChunk>(B(pktLen));
    pkt->insertAtBack(payload);
    currentSocket->CacheData(sid,pkt);
}

bool SimpleProvider::isEnabled()
{
    return true;
}

void SimpleProvider::finish()
{
    ApplicationBase::finish();

}

void SimpleProvider::handleStopOperation(LifecycleOperation *operation)
{

}

void SimpleProvider::handleCrashOperation(LifecycleOperation *operation)
{

}


}

