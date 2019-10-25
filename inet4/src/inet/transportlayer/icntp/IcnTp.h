/*
 * IcnTp.h
 *
 *  Created on: 2019年9月3日
 *      Author: hiro
 */

#ifndef INET_TRANSPORTLAYER_ICNTP_ICNTP_H_
#define INET_TRANSPORTLAYER_ICNTP_ICNTP_H_

#include <vector>

#include "inet/common/Protocol.h"
#include "inet/common/lifecycle/OperationalBase.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/packet/Message.h"
#include "inet/common/INETDefs.h"
#include "inet/common/packet/Message.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/socket/ISocket.h"
#include "inet/transportlayer/common/TransportPseudoHeader_m.h"
#include "inet/common/packet/ReassemblyBuffer.h"

namespace inet{

class InterfaceEntry;

class INET_API IcnTp : public OperationalBase
{
public:
    IcnTp();
    ~IcnTp();
protected:
    virtual void refreshDisplay() const override;
    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;
    virtual bool isInitializeStage(int stage) override { return stage == INITSTAGE_TRANSPORT_LAYER; }
    virtual bool isModuleStartStage(int stage) override { return stage == ModuleStartOperation::STAGE_TRANSPORT_LAYER; }
    virtual bool isModuleStopStage(int stage) override { return stage == ModuleStopOperation::STAGE_TRANSPORT_LAYER; }

    virtual void initialize(int stage) override;
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void handleMessageWhenUp(cMessage *msg) override;
};

}



#endif /* INET_TRANSPORTLAYER_ICNTP_ICNTP_H_ */
