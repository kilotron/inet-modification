/*
 * PccpIndicator.cc
 *
 *  Created on: Apr 17, 2020
 *      Author: kilotron
 */
#include "PccpIndicator.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/ProtocolGroup.h"
#include "inet/common/ModuleAccess.h"
#include "inet/networklayer/common/L3Tools.h"
#include "inet/networklayer/common/PitTag_m.h"
#include "inet/networklayer/icn/color/Data_m.h"
#include <iostream>

#ifdef WITH_IEEE80211
#include "inet/linklayer/ieee80211/mac/Ieee80211Frame_m.h"
//#include "inet/linklayer/ethernet/Ethernet.h"
#endif // #ifdef WITH_ETHERNET

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

void PccpIndicator::setPccpCi(Packet *packet, PccpCiCode ci)
{
    auto protocol = packet->getTag<PacketProtocolTag>()->getProtocol();
    if (protocol == &Protocol::ieee80211Mac) {
#if defined(WITH_IEEE80211)
        packet->trim();
        auto header = packet->removeAtFront<ieee80211::Ieee80211MacHeader>();
        auto fcs = packet->removeAtBack<ieee80211::Ieee80211MacTrailer>(B(4));
        auto data = removeNetworkProtocolHeader<Data>(packet);
        data->setCongestionIndication(ci);
        // TODO: recalculate IP header checksum
        //insertNetworkProtocolHeader(packet, Protocol::color, data);
        packet->insertAtFront(header);
        packet->insertAtBack(fcs);
        packet->addTagIfAbsent<PacketProtocolTag>()->setProtocol(protocol);
    }
#else
        throw cRuntimeError("IEEE 802.11 feature is disabled");
#endif // #if defined(WITH_ETHERNET)
}

bool PccpIndicator::matchesPacket(Packet *packet)
{
    int queueLength = collection->getNumPackets();
    int queueCapacity = collection->getMaxNumPackets();
    std::cout << "in matchesPacket:QL=" << queueLength << ",Qc=" << queueCapacity << endl;
    // 如果有PitTag则这个packet是DATA，不是GET
    auto pitInd = packet->removeTagIfPresent<PitInd>();
    if (pitInd) {
        std::cout << "pitL=" << pitInd->getPitLength() << ",pitC=" << pitInd->getPitCapacity() << endl;
        setPccpCi(packet, BUSY_1);
    }
    avg = (1 - wq) * avg + wq * queueLength;
    auto ind = packet->addTag<PccpDataQueueInd>();
    ind->setQueueLength(avg);
    ind->setQueueCapacity(queueCapacity);
    return true;
}

} // namespace queueing
} // namespace inet
