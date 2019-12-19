/*
 * Croute.h
 *
 *  Created on: 2019年7月5日
 *      Author: hiro
 */

#ifndef INET_NETWORKLAYER_ICN_ROUTINGTABLE_CROUTE_H_
#define INET_NETWORKLAYER_ICN_ROUTINGTABLE_CROUTE_H_
#include "inet/common/INETDefs.h"
#include "inet/networklayer/icn/field/SID.h"
#include "inet/networklayer/icn/field/NID.h"
#include <string>
#include "colorRoutingTable.h"


namespace inet{

class ColorRoutingTable;
class Croute: public cObject
{
    private:
        NID nextHop;
        SID sid;
        ColorRoutingTable* routingTable;
        int metric; //ad-hoc网络中暂时用不到
        simtime_t timeToLive;
    public:
        Croute(NID next, SID s, ColorRoutingTable* table, simtime_t t, int m=1):cObject(),nextHop(next),\
        sid(s),routingTable(table),metric(m),timeToLive(t){}
        ~Croute(){}

        void setRoutingTable(ColorRoutingTable* t){routingTable=t;}
        ColorRoutingTable* getRoutingTable(){return routingTable;}
//        std::string str();
        void setSid(SID s){sid = s;}

        SID getSid(){return sid;}
        void setNextHop(NID n){nextHop = n;}
        NID getNextHop(){return nextHop;}
        void setMetric(int m){metric = m;}
        int getMetric(){return metric;}
};
}



#endif /* INET_NETWORKLAYER_ICN_ROUTINGTABLE_CROUTE_H_ */
