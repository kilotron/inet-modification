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

#define TOSTRING(x) #x

#ifdef WITH_IEEE80211
#include "inet/linklayer/ieee80211/mac/Ieee80211Frame_m.h"
#include "inet/linklayer/ieee8022/Ieee8022LlcHeader_m.h"
#endif // #ifdef WITH_ETHERNET

namespace inet {
namespace queueing {

Define_Module(PccpIndicator);

simsignal_t PccpIndicator::dataQueueLengthSignal = registerSignal("dataQueueLength");
simsignal_t PccpIndicator::pitLengthSignal = registerSignal("pitLength");
simsignal_t PccpIndicator::aveDataQueueLengthSignal = registerSignal("aveDataQueueLength");
simsignal_t PccpIndicator::avePitLengthSignal = registerSignal("avePitLength");
simsignal_t PccpIndicator::congestionIndexSignal = registerSignal("congestionIndex");

void PccpIndicator::checkParameters(double parVal, const char* parName, double parMin, double parMax)
{
    // 不需要精确比较
    if (parVal < parMin || parVal > parMax) {
        throw cRuntimeError("Invalid value for %s parameter: %g", parName, parVal);
    }
}

void PccpIndicator::initialize(int stage)
{
    PacketFilterBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        wq = par("wq");
        g = par("g");
        CI_FREE = par("CI_FREE");
        CI_BUSY = par("CI_BUSY");
        CI_CONG = par("CI_CONG");
        p0 = par("p0");
        algorithm = par("algorithm").stdstringValue();
        checkParameters(wq, TOSTRING(wq), 0.0, 1.0);
        checkParameters(g, TOSTRING(g), 0.0, 1.0);
        checkParameters(p0, TOSTRING(p0), 0.0, 1.0);
        checkParameters(CI_FREE, TOSTRING(CI_FREE), 0.0, 1.0);
        checkParameters(CI_BUSY, TOSTRING(CI_BUSY), 0.0, 1.0);
        checkParameters(CI_CONG, TOSTRING(CI_CONG), 0.0, 1.0);
        if ( !(CI_FREE < CI_BUSY && CI_BUSY < CI_CONG) ) {
            throw cRuntimeError("Invalid value for CI_FREE=%g, CI_BUSY=%g, CI_CONG=%g", CI_FREE, CI_BUSY, CI_CONG);
        }

        // 获得链路层队列
        auto outputGate = gate("out");
        collection = findConnectedModule<IPacketCollection>(outputGate);
        if (collection == nullptr) {
            collection = getModuleFromPar<IPacketCollection>(par("collectionModule"), this);
        }
    }
}

/** cl: congestion level, PccpClCode is defined in ClTag.msg
 *     FREE = 0;
 *     BUSY_1 = 1;
 *     BUSY_2 = 2;
 *     CONGESTED = 3;
 * */
void PccpIndicator::setPccpCl(Packet *packet, PccpClCode cl)
{
    auto protocol = packet->getTag<PacketProtocolTag>()->getProtocol();
    if (protocol == &Protocol::ieee80211Mac) {
#if defined(WITH_IEEE80211)
        packet->trim();
        auto macHeader = packet->removeAtFront<ieee80211::Ieee80211MacHeader>();
        auto llcHeader = packet->removeAtFront<Ieee8022LlcHeader>();
        auto fcs = packet->removeAtBack<ieee80211::Ieee80211MacTrailer>(B(4));
        auto data = removeNetworkProtocolHeader<Data>(packet);
        // congestion indication cannot be decreased by an intermediate node.
        if (cl > data->getCongestionLevel()) {
            data->setCongestionLevel(cl);
        }
        // 下面的操作与removeNetworkProtocolHeader相反，但省略了NetworkProtocolInd
        // 因为icn里没有用NetworkProtocolInd
        packet->addTagIfAbsent<PacketProtocolTag>()->setProtocol(protocol);
        packet->insertAtFront(data);
        packet->insertAtFront(llcHeader);
        packet->insertAtFront(macHeader);
        packet->insertAtBack(fcs);
    }
#else
        throw cRuntimeError("IEEE 802.11 feature is disabled");
#endif // #if defined(WITH_ETHERNET)
}

bool PccpIndicator::matchesPacket(Packet *packet)
{
    // 如果有PitTag,则这个packet是DATA，不是GET
    auto pitInd = packet->removeTagIfPresent<PitInd>();
    if (pitInd == nullptr) {
        //emit(PccpIndicator::dataQueueLengthSignal, collection->getNumPackets());
        return true;
    }
    updateAverageQueueLengthAndCI(pitInd->getPitLength(), pitInd->getPitCapacity());
    PccpClCode cl = calculateCongestionLevel();
    setPccpCl(packet, cl);
    return true;
}

void PccpIndicator::updateAverageQueueLengthAndCI(int pitLength, int pitCapacity)
{
    int dataQLength = collection->getNumPackets();
    int dataQCapacity = collection->getMaxNumPackets();
    if (dataQLength > 0) {
        avgDataQueueLength = (1 - wq) * avgDataQueueLength + wq * dataQLength;
    } else {
        // 假设链路带宽16Mpbs, 一个packet大小是1KB，则需要0.0005s传输时间
        double m = SIMTIME_DBL(simTime() - q_time) / 0.0005;
        avgDataQueueLength = pow(1 - wq, m) * avgDataQueueLength;
    }

    avgPitLength = (1 - wq) * avgPitLength + wq * pitLength;

    if (algorithm == "ECP") {
        ci = avgPitLength / pitCapacity;
    } else if (algorithm == "CCS") {
        ci = (avgDataQueueLength) / dataQCapacity;
    } else if (algorithm == "PCCP"){
        ci = (1 - g) * avgDataQueueLength / dataQCapacity + g * min(avgPitLength / dataQCapacity, 1);
    } else {
        std::cout << "Undefined congestion control algorithm" << std::endl;
    }
//    std::cout << "update CI: QL=" << dataQLength << ",QC=" << dataQCapacity
//            << ",avgDQ=" << avgDataQueueLength << ",pitL=" << pitLength
//            << ",pitC=" << pitCapacity << ",avgPL=" << avgPitLength
//            << ",CI=" << ci << endl;
    emit(PccpIndicator::dataQueueLengthSignal, dataQLength);
    emit(PccpIndicator::pitLengthSignal, pitLength);
    emit(PccpIndicator::aveDataQueueLengthSignal, avgDataQueueLength);
    emit(PccpIndicator::avePitLengthSignal, avgPitLength);
    emit(PccpIndicator::congestionIndexSignal, ci);
}

/**
 * CL计算方法：
 * 1. CI <= CI_FREE, CL = FREE
 * 2. CI >= CI_CONG, CL = CONGESTED
 * 3. CI_FREE < CI <= CI_BUSY, P(CL=BUSY_1) = 1 - (1 - p0) * (CI - CI_FREE) / (CI_BUSY - CI_FREE)
 * 4. CI_BUSY < CI < CI_CONG, P(CL=BUSY_1) = p0 * (CI_CONG - CI) / (CI_CONG - CI_BUSY)
 * P(CL=BUSY_2) = 1 - P(CL=BUSY_1)
 */
PccpClCode PccpIndicator::calculateCongestionLevel()
{
    if (ci <= CI_FREE) {
        return PccpClCode::FREE;
    }
    else if (ci >= CI_CONG) {
        return PccpClCode::CONGESTED;
    }
    else if (CI_FREE < ci && ci <= CI_BUSY) {
        // 先采用简化方式去掉概率计算
        return PccpClCode::BUSY_1;
//        double r = dblrand();
//        double p = 1 - (1 - p0) * (ci - CI_FREE) / (CI_BUSY - CI_FREE);
//        return (r <= p) ? PccpClCode::BUSY_1 : PccpClCode::BUSY_2;
    }
    else { // CI_BUSY < ci < CI_CONG
        return PccpClCode::BUSY_2;
//        double r = dblrand();
//        double p = p0 * (CI_CONG - ci) / (CI_CONG - CI_BUSY);
//        return (r <= p) ? PccpClCode::BUSY_1 : PccpClCode::BUSY_2;
    }
}

void PccpIndicator::pushOrSendPacket(Packet *packet, cGate *gate, IPassivePacketSink *consumer)
{
    PacketFilterBase::pushOrSendPacket(packet, gate, consumer);
    // Set the time stamp q_time when the queue gets empty.
    const int queueLength = collection->getNumPackets();
    if (queueLength == 0)
        q_time = simTime();
}

} // namespace queueing
} // namespace inet
