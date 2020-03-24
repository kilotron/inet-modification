/*
 * ColorSocket.h
 *
 *  Created on: 2019年8月15日
 *      Author: hiro
 */

#ifndef INET_NETWORKLAYER_CONTRACT_COLOR_COLORSOCKET_H_
#define INET_NETWORKLAYER_CONTRACT_COLOR_COLORSOCKET_H_

#include "inet/common/INETDefs.h"
#include "inet/common/packet/Message.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/socket/ISocket.h"
#include "inet/common/Protocol.h"
#include "inet/networklayer/icn/field/NID.h"
#include "inet/networklayer/icn/field/SID.h"

namespace inet
{

class INET_API ColorSocket : public ISocket
{
    public:

    class INET_API ICallback
    {
      public:
        virtual ~ICallback() {}

        /**
         * Notifies about data arrival, packet ownership is transferred to the callee.
         */
        virtual void socketDataArrived(ColorSocket *socket, Packet *packet) = 0;

        /**
         * Notifies about error indication arrival, indication ownership is transferred to the callee.
         */
        // virtual void socketErrorArrived(ColorSocket *socket, Indication *indication) = 0;

        /**
         * Notifies about socket closed, indication ownership is transferred to the callee.
         */
        virtual void socketClosed(ColorSocket *socket) = 0;
    };
    enum State { CONNECTED, CLOSED};

    protected:

      int socketId;
      ICallback *cb = nullptr;
      void *userData = nullptr;
      cGate *gateToColor = nullptr;
      State sockState = CLOSED;

      ColorSocket::ICallback *callback = nullptr;
      cGate *outputGate = nullptr;
      bool bound = false;
      bool isOpen_ = false;
      const Protocol *l3Protocol = nullptr;

    public:
      ColorSocket(const Protocol* protocol, cGate *outputGate = nullptr);

      ~ColorSocket(){};

      void *getUserData() const { return userData; }

      void setUserData(void *userData) { this->userData = userData; }

      void setOutputGate(cGate *outputGate) { this->outputGate = outputGate; }

      void bind(const Protocol *protocol, const NID &nid, int localPort);

      void setCallback(ColorSocket::ICallback *callback);

      int getSocketId() const override { return socketId; }

      void sendGET(const SID &sid, int port, double inter);

      void CacheData(const SID &sid, cMessage *msg);

      void close();

      void destroy();

      const Protocol *getNetworkProtocol() const { return l3Protocol; };

      bool belongsToSocket(cMessage *msg) const;

      void processMessage(cMessage *msg) ;

      bool isOpen() const {return isOpen_;};

      void sendToOutput(cMessage *message);

      int generateSocketId(){ return getEnvir()->getUniqueNumber();}
};

}

#endif /* INET_NETWORKLAYER_CONTRACT_COLOR_COLORSOCKET_H_ */
