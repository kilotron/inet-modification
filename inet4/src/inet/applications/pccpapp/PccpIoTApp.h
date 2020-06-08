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

#ifndef INET_APPLICATIONS_PCCPAPP_PCCPIOTAPP_H_
#define INET_APPLICATIONS_PCCPAPP_PCCPIOTAPP_H_

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

class INET_API PccpIoTApp : public PccpApp
{
    friend class PccpAlg;

protected:
    // parameters
    int transNum;
    simtime_t sleepTime;
    int appIndex;

public:
    PccpIoTApp();
    ~PccpIoTApp();
    virtual void initialize(int stage) override;
    virtual void handleSelfMessage(cMessage *msg);
};

} /* namespace inet */

#endif /* INET_APPLICATIONS_PCCPAPP_PCCPIOTAPP_H_ */
