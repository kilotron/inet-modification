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

#ifndef __INET_MODIFICATION_PCCPAPP_H_
#define __INET_MODIFICATION_PCCPAPP_H_

#include "inet/common/INETDefs.h"
#include "inet/applications/base/ApplicationBase.h"
#include "inet/common/Protocol.h"
#include "inet/common/lifecycle/LifecycleOperation.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/networklayer/contract/color/ColorSocket.h"
#include "inet/networklayer/icn/field/SID.h"
#include "inet/applications/pccpapp/PccpAlg.h"

namespace inet {

class PccpAlg;

class INET_API PccpApp : public ApplicationBase
{
    friend class PccpAlg;

  private:

    // parameters
    int localPort;
    int destIndex;
    int requestNum;
    std::string path;
    double sendInterval;
    simtime_t startTime;
    simtime_t stopTime;

    NID nid;
    long long content = 0;
    ColorSocket *currentSocket = nullptr;

    cMessage *timer = nullptr;    // to schedule the next Ping request
    cMessage *start = nullptr;

    PccpAlg *pccpAlg;

    // statistics, see PccpApp.ned for more details
    static simsignal_t rtoSignal;
    static simsignal_t srttSignal;
    static simsignal_t windowSignal;
    static simsignal_t effectiveWindowSignal;
    static simsignal_t rexmitSignal;
    static simsignal_t dataRcvdSignal;
    static simsignal_t getSentSignal;

  public:
    PccpApp();
    ~PccpApp();

    virtual void initialize(int state) override;
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void handleSelfMessage(cMessage *msg);
    virtual void finish() override;
    virtual void refreshDisplay() const override;
    void sendRequest(const SID &sid);

    // interface between PccpApp and PccpAlg
    void maxRexmit(const SID& sid);
    void dataArrived(Packet *packet);
    void scheduleTimeout(cMessage *timer, simtime_t timeout);

    bool isEnabled(); // TODO what's this

    // Lifecycle methods
    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;
};

} //namespace inet

#endif /* __INET_MODIFICATION_PCCPAPP_H_ */
