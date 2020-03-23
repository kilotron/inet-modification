/*
 * SimleApp.h
 *
 *  Created on: Mar 2, 2020
 *      Author: hiro
 */
#include <algorithm>
#include <fstream>

#include "inet/applications/ICNapp/SimpleApp.h"
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
Define_Module(SimpleApp);

void SimpleApp::SimRecorder::ConsumerPrint(std::ostream &os)
{

    os << "index:    " << index << endl;
    os << "sendNum: " << GetSendNum << endl;
    os << "recvNum: " << DataRecvNum << endl;
    if (GetSendNum != 0)
        os << "trans ratio: " << 100 * DataRecvNum / GetSendNum << "%" << endl;
    os << "send Interval: " << owner->sendInterval << "s" << endl;
    os << "Throughput: " << throughput.get() * 8 / ((simTime().dbl() - owner->startTime.dbl()) * 1000 * 1000) << " Mbps" << endl;
//    os << "Average delay: ";
    double sum = 0;
//    if(delayArray.size()>0)
//    {
//        std::for_each(delayArray.begin(), delayArray.end(), [&sum](double value) { sum += value; });
//        os << sum / delayArray.size() << " ms" << endl;
//    }
//    else
//        os << 0 << "ms" << endl;

    os << endl;
}

void SimpleApp::SimRecorder::ProviderPrint(std::ostream &os)
{
    os << "index:    " << index << endl;
    os << "DataSendNum: " << DataSendNum << endl;
    os << "GetRecvNum: " << GetRecvNum << endl;
    os << endl;
}

void SimpleApp::initialize(int stage)
{
    ApplicationBase::initialize(stage);
    if(stage == INITSTAGE_LOCAL){
        destIndex = par("destAddr").intValue();
        requestNum = par("requestNum").intValue();
        sendInterval = par("sendInterval").doubleValue();
        startTime = par("startTime").doubleValue();
        stopTime = par("stopTime").doubleValue();
        timer = new cMessage("sendGET");
        localPort = par("port").intValue();
        path=par("RSTpath").stdstringValue();
        Recorder.owner = this;
        start = new cMessage("start");

        int nodeIndex = getParentModule()->getIndex();
        std::array<uint64_t, 2> hashValue;
        MurmurHash3_x64_128(&nodeIndex, sizeof(int), 0, hashValue.data());
        nid.setNID(hashValue);
        nid.test = nodeIndex;
    }
}

void SimpleApp::handleMessageWhenUp(cMessage *msg){
    if (msg->isSelfMessage())
        handleSelfMessage(msg);
    else {
        currentSocket->processMessage(msg);
    }
    if (operationalState == State::STOPPING_OPERATION )
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void SimpleApp::handleSelfMessage(cMessage *msg)
{
    static long long content = 0;
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

void SimpleApp::socketDataArrived(ColorSocket *socket, Packet *packet)
{
    Recorder.DataRecvNum++;

    // Recorder.delayArray.push_back((simTime().dbl() - Recorder.Delays[sid].dbl()) * 1000);
    // std::cout << "receive the packet successfully!" << endl;
    // std::cout << "delay is " << (simTime().dbl() - Recorder.Delays[sid].dbl()) * 1000 << " ms" << endl;

    // //移除Delays中的此sid对应的delay
    // Recorder.Delays.erase(sid);

    //统计吞吐量
    Recorder.throughput += B(packet->getByteLength());
}

void SimpleApp::socketClosed(ColorSocket *socket)
{

   delete currentSocket;
   currentSocket = nullptr;
}

void SimpleApp::handleStartOperation(LifecycleOperation *operation)
{
    if (isEnabled())
    {
        currentSocket = new ColorSocket(&Protocol::color, gate("socketOut"));
        currentSocket->bind(&Protocol::color,nid, localPort);
        currentSocket->setCallback(this);
        scheduleAt(startTime, start);
    }
        
}

void SimpleApp::refreshDisplay() const
{
    ApplicationBase::refreshDisplay();

}

void SimpleApp::sendRequest(const SID &sid){

    currentSocket->sendGET(sid, localPort);
    Recorder.GetSendNum++;
}

bool SimpleApp::isEnabled()
{
    return destIndex >= 0;
}

void SimpleApp::finish()
{
    ApplicationBase::finish();
    std::ofstream outfile;
    auto fileName = cSimulation::getActiveEnvir()->getConfigEx()->getActiveConfigName() + std::string("_Consumer.txt");
    outfile.open(path + fileName, std::ofstream::app);
    Recorder.ConsumerPrint(outfile);
    outfile.close();
}

void SimpleApp::handleStopOperation(LifecycleOperation *operation)
{

}

void SimpleApp::handleCrashOperation(LifecycleOperation *operation)
{

}


}


