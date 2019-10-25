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
        virtual void socketErrorArrived(ColorSocket *socket, Indication *indication) = 0;

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

    
};

}

#endif /* INET_NETWORKLAYER_CONTRACT_COLOR_COLORSOCKET_H_ */
