/*
 * PccpIndicator.cc
 *
 *  Created on: Apr 17, 2020
 *      Author: kilotron
 */
#include "PccpIndicator.h"
#include "inet/common/ModuleAccess.h"

namespace inet {
namespace queueing {

Define_Module(PccpIndicator);

void PccpIndicator::initialize(int stage)
{
    PacketFilterBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        wq = par("wq");
        if (wq < 0.0 || wq > 1.0)
            throw cRuntimeError("Invalid value for wq parameter: %g", wq);
        auto outputGate = gate("out");
        collection = findConnectedModule<IPacketCollection>(outputGate);
        // 获得链路层队列
        if (collection == nullptr) {
            collection = getModuleFromPar<IPacketCollection>(par("collectionModule"), this);
        }
    }
}

bool PccpIndicator::matchesPacket(Packet *packet)
{
    int queueLength = collection->getNumPackets();
    int queueCapacity = collection->getMaxNumPackets();
    avg = (1 - wq) * avg + wq * queueLength;
    auto ind = packet->addTag<PccpDataQueueInd>();
    ind->setQueueLength(avg);
    ind->setQueueCapacity(queueCapacity);
    return true;
}

} // namespace queueing
} // namespace inet
