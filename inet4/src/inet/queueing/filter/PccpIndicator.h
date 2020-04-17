/*
 * PccpIndicator.h
 *
 *  Created on: Apr 17, 2020
 *      Author: kilotron
 */

#ifndef INET_QUEUEING_FILTER_PCCPINDICATOR_H_
#define INET_QUEUEING_FILTER_PCCPINDICATOR_H_

#include "inet/common/packet/Packet.h"
#include "inet/queueing/base/PacketFilterBase.h"
#include "inet/queueing/contract/IPacketCollection.h"
#include "inet/linklayer/common/PccpDataQueueTag_m.h"

namespace inet {
namespace queueing {

class INET_API PccpIndicator : public PacketFilterBase
{
protected:
    double wq;
    double avg = 0.0;
    IPacketCollection *collection = nullptr; // linklayer queu

protected:
    virtual void initialize(int stage) override;
    virtual bool matchesPacket(Packet *packet) override;
};

} // namespace queueing
} // namespace inet

#endif /* INET_QUEUEING_FILTER_PCCPINDICATOR_H_ */
