/*
 * SimpleProvider.h
 *
 *  Created on: Mar 24, 2020
 *      Author: hiro
 */

#ifndef INET_APPLICATIONS_ICNAPP_SIMPLEPROVIDER_H_
#define INET_APPLICATIONS_ICNAPP_SIMPLEPROVIDER_H_

#include "inet/common/INETDefs.h"
#include "inet/applications/base/ApplicationBase.h"
#include "inet/common/Protocol.h"
#include "inet/common/lifecycle/LifecycleOperation.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/socket/SocketMap.h"
#include "inet/networklayer/common/L3Address.h"
#include "inet/networklayer/contract/INetworkSocket.h"
#include "inet/networklayer/contract/color/ColorSocket.h"
#include "inet/networklayer/icn/field/SID.h"

namespace inet
{
class INET_API SimpleProvider : public ApplicationBase, public ColorSocket::ICallback
{

    private:
        NID nid;
        int pktLen;
        int pktNum;
        int nodeIndex;

        int localPort;

        simtime_t stopTime;


        ColorSocket *currentSocket = nullptr;
        int pid = 0;

        cMessage *start = nullptr;
        long long content = 0;

    public:
        simtime_t startTime;

        SimpleProvider(){};
        ~SimpleProvider(){};
        int getPid() const {return pid;}

        virtual void initialize(int stage) override;
        virtual int numInitStages() const override { return NUM_INIT_STAGES; }
        virtual void handleMessageWhenUp(cMessage *msg) override;
        virtual void handleSelfMessage(cMessage *msg);
        virtual void finish() override;
        virtual void refreshDisplay() const override;

        void generateAndCacheData(const SID &sid);

        bool isEnabled();

        // Lifecycle methods
        virtual void handleStartOperation(LifecycleOperation *operation) override;
        virtual void handleStopOperation(LifecycleOperation *operation) override;
        virtual void handleCrashOperation(LifecycleOperation *operation) override;

        //INetworkSocket::ICallback:
        virtual void socketDataArrived(ColorSocket *socket, Packet *packet) override;
        virtual void socketClosed(ColorSocket *socket) override;

};
} // namespace inet




#endif /* INET_APPLICATIONS_ICNAPP_SIMPLEPROVIDER_H_ */
