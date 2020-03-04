/*
 * ColorSocket.cc
 *
 *  Created on: 2019年8月15日
 *      Author: hiro
 */

#include "inet/applications/common/SocketTag_m.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/packet/Message.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/contract/color/ColorSocket.h"
#include "inet/networklayer/contract/color/ColorSocketCommand_m.h"

namespace inet{

ColorSocket::ColorSocket(const Protocol* protocol, cGate *outputGate) :
l3Protocol(protocol),
socketId(getEnvir()->getUniqueNumber()),
outputGate(outputGate)
{
}

void ColorSocket::bind(const Protocol *protocol, const NID &nid)
{
    ASSERT(!bound);
    ASSERT(l3Protocol != nullptr);
    auto *command = new ColorSocketBindCommand();
    command->setProtocol(protocol);
    command->setNid(nid);
    auto request = new Request("bind", COLOR_C_BIND);
    request->setControlInfo(command);
    sendToOutput(request);
    bound = true;
    isOpen_ = true;
}

void ColorSocket::sendToOutput(cMessage *message)
{
    if (!outputGate)
        throw cRuntimeError("ColorSocket: setOutputGate() must be invoked before the socket can be used");
    auto& tags = getTags(message);
    tags.addTagIfAbsent<DispatchProtocolReq>()->setProtocol(l3Protocol);
    tags.addTagIfAbsent<SocketReq>()->setSocketId(socketId);
    check_and_cast<cSimpleModule *>(outputGate->getOwnerModule())->send(message, outputGate);
}

void ColorSocket::setCallback(ColorSocket::ICallback *callback)
{
    this->callback = callback;
}

void ColorSocket::sendGET(const SID &sid)
{
    ASSERT(bound);
    ASSERT(l3Protocol != nullptr);
    auto *command = new ColorSocketSendGetCommand();
    command->setSid(sid);
    auto request = new Request("bind", COLOR_C_SEND_GET);
    request->setControlInfo(command);
    sendToOutput(request);
}

void ColorSocket::close()
{
    ASSERT(bound);
    auto *command = new ColorSocketCloseCommand();
    auto request = new Request("close", COLOR_C_CLOSE);
    request->setControlInfo(command);
    sendToOutput(request);
}

void ColorSocket::destroy()
{
    ASSERT(bound);
    auto command = new ColorSocketDestroyCommand();
    auto request = new Request("destroy", COLOR_C_DESTROY);
    request->setControlInfo(command);
    sendToOutput(request);
}

bool ColorSocket::belongsToSocket(cMessage *msg) const
{
    auto& tags = getTags(msg);
    int msgSocketId = tags.getTag<SocketInd>()->getSocketId();
    return socketId == msgSocketId;
}

void ColorSocket::processMessage(cMessage *msg)
{
    ASSERT(belongsToSocket(msg));
    if (callback)
        callback->socketDataArrived(this, check_and_cast<Packet *>(msg));
    else
        delete msg;
}

}

