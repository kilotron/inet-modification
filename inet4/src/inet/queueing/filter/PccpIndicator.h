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
#include "inet/networklayer/common/ClTag_m.h"

namespace inet {
namespace queueing {

class INET_API PccpIndicator : public PacketFilterBase
{
protected:
    // parameters
    double wq;  // weight of the current queue length in the averaged queue length
    double g;    // weight of data queue occupancy
    double CI_FREE;
    double CI_BUSY;
    double CI_CONG;
    double p0;
    double avgDataQueueLength = 0.0;
    double avgPitLength = 0.0;
    double ci = 0.0; // congestion index
    bool dataQueueOnly;
    IPacketCollection *collection = nullptr; // linklayer queue
    simtime_t q_time;

    // statistics
    static simsignal_t dataQueueLengthSignal;
    static simsignal_t pitLengthSignal;
    static simsignal_t aveDataQueueLengthSignal;
    static simsignal_t avePitLengthSignal;
    static simsignal_t congestionIndexSignal;
protected:
    virtual void initialize(int stage) override;

    /**
     * 从packet中获得PIT的信息，并把当前的拥塞等级写到Packet中。
     */
    virtual bool matchesPacket(Packet *packet) override;

    /**
     * 队列空时设置q_time
     */
    virtual void pushOrSendPacket(Packet *packet, cGate *gate, IPassivePacketSink *consumer) override;

    /**
     * 如果此节点拥塞等级更高，则把packet中的拥塞等级字段设为cl。
     */
    static void setPccpCl(Packet *packet, PccpClCode cl);

    /**
     * 检查模块参数。如果参数parVal不在parMin与parMax之间，则抛出异常。
     */
    static void checkParameters(double parVal, const char* parName, double parMin, double parMax);

    /**
     * 更新avgDataQueueLength与avgPitLength与ci。参数pitLength与pitCapacity是PIT当前长度与容量。
     * 此函数在收到上层数据包时调用。
     */
    void updateAverageQueueLengthAndCI(int pitLength, int pitCapacity);

    /**
     * 根据CI（congestion index）计算得出CL(congestion level)
     */
    PccpClCode calculateCongestionLevel();
};

} // namespace queueing
} // namespace inet

#endif /* INET_QUEUEING_FILTER_PCCPINDICATOR_H_ */
