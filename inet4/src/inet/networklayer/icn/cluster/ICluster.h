/*
 * ICluster.h
 *
 *  Created on: 2019年7月5日
 *      Author: hiro
 */

#ifndef INET_NETWORKLAYER_ICN_CLUSTER_ICLUSTER_H_
#define INET_NETWORKLAYER_ICN_CLUSTER_ICLUSTER_H_

#include "inet/common/INETDefs.h"
#include "inet/common/ModuleAccess.h"
#include "inet/linklayer/common/MacAddress.h"
#include "inet/networklayer/icn/field/NID.h"

//Cluster protocol interface, for implement different cluster algorithm
namespace inet{


//分簇协议的接口类，可以根据不同的策略派生出不同的分簇算法，允许一个节点有多个簇头
class INET_API ICluster
{
    protected:


        static const simsignal_t clusterInitiatedSignal;
        static const simsignal_t clusterCompletedSignal;
        static const simsignal_t clusterFailedSignal;

        //record nid, mac, last update time for the clusterhead
        struct ClusterEntry
        {

            NID clusterhead;
            MacAddress headAddr;
            simtime_t lastUpdate;
            ClusterEntry(NID n, MacAddress mac, simtime_t update):clusterhead(n),headAddr(mac),lastUpdate(update){}

            bool operator<(const struct ClusterEntry& entry) const
            {
                return this->clusterhead < entry.clusterhead;
            }
        };

        //a node can have several cluster heads
        struct ClusterTable
        {
            bool isHead = 0;
            std::set<ClusterEntry> table;
        };


    public:
        virtual std::set<int> &getHeads()=0;
        virtual const ClusterTable getClusterHead() = 0;
        virtual bool isHead() = 0;
        virtual bool isPreHead() { return true; };
};
}


#endif /* INET_NETWORKLAYER_ICN_CLUSTER_ICLUSTER_H_ */
