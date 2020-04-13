/*
 * SimpleApp.h
 *
 *  Created on: Mar 2, 2020
 *      Author: hiro
 */

#ifndef INET_APPLICATIONS_ICNAPP_SIMPLEAPP_H_
#define INET_APPLICATIONS_ICNAPP_SIMPLEAPP_H_

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
class INET_API SimpleApp : public ApplicationBase, public ColorSocket::ICallback
{
    public:
        struct SimRecorder
        {
            SimpleApp * owner;

            int multiConsumer;

            int index;
            std::map<SID, simtime_t> Delays;
            std::vector<double> delayArray;

            B throughput = B(0);
            int GetSendNum = 0;
            int GetRecvNum = 0;

            int DataSendNum = 0;
            int DataRecvNum =0;
            simtime_t delay = 0;

            void ConsumerPrint(std::ostream &os);

            void ProviderPrint(std::ostream &os);
        };

    private:
        NID nid;
        int destIndex;
        int requestNum;
        int rngNum;
        
        int localPort;
        
        simtime_t stopTime;
        long long content = 0;
        

        ColorSocket *currentSocket = nullptr;
        int pid = 0;
        cMessage *timer = nullptr;    // to schedule the next Ping request
        cMessage *start = nullptr;
        SimRecorder Recorder;

        std::string path;

    public:
        simtime_t startTime;
        double sendInterval;
        
        SimpleApp(){};
        ~SimpleApp(){};
        int getPid() const {return pid;}

        virtual void initialize(int stage) override;
        virtual int numInitStages() const override { return NUM_INIT_STAGES; }
        virtual void handleMessageWhenUp(cMessage *msg) override;
        virtual void handleSelfMessage(cMessage *msg);
        virtual void finish() override;
        virtual void refreshDisplay() const override;

        void sendRequest(const SID &sid);

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





#endif /* INET_APPLICATIONS_ICNAPP_SIMPLEAPP_H_ */
