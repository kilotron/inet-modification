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

#ifndef __INET_MODIFICATION_PCCPPROVIDER_H_
#define __INET_MODIFICATION_PCCPPROVIDER_H_

#include "inet/common/INETDefs.h"
#include "inet/applications/base/ApplicationBase.h"
#include "inet/common/lifecycle/LifecycleOperation.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/networklayer/contract/color/ColorSocket.h"
#include "inet/networklayer/icn/field/SID.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"

namespace inet {

class INET_API PccpProvider : public ApplicationBase, public ColorSocket::ICallback
{
    private:
        // parameters
        int localPort; // not used now
        int pktLen;
        int pktNum;
        simtime_t startTime;

        int nodeIndex;
        NID nid;

        ColorSocket *currentSocket = nullptr;

        cMessage *start = nullptr;
        long long content = 0;

    public:

        PccpProvider(){};
        ~PccpProvider(){};

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

#endif
