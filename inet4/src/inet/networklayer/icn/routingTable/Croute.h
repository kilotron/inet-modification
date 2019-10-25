/*
 * Croute.h
 *
 *  Created on: 2019年7月5日
 *      Author: hiro
 */

#ifndef INET_NETWORKLAYER_ICN_ROUTINGTABLE_CROUTE_H_
#define INET_NETWORKLAYER_ICN_ROUTINGTABLE_CROUTE_H_
#include "inet/common/INETDefs.h"
#include "inet/networklayer/icn/field/dataType.h"
#include <string>
#include "colorRoutingTable.h"


namespace inet{

class ColorRoutingTable;
class Croute: public cObject
{
    private:
        NID_t nextHop;
        SID_t sid;
        ColorRoutingTable* routingTable;
        int metric; //ad-hoc网络中暂时用不到
        simtime_t timeToLive;
    public:
        Croute(NID_t next, SID_t s, ColorRoutingTable* table, simtime_t t, int m=1):cObject(),nextHop(next),\
        sid(s),routingTable(table),timeToLive(t),metric(m){}
        ~Croute(){}

        void setRoutingTable(ColorRoutingTable* t){routingTable=t;}
        ColorRoutingTable* getRoutingTable(){return routingTable;}
        std::string str();
        void setSid(SID_t s){sid = s;}

        SID_t getSid(){return sid;}
        void setNextHop(NID_t n){nextHop = n;}
        NID_t getNextHop(){return nextHop;}
        void setMetric(int m){metric = m;}
        int getMetric(){return metric;}
};
}



#endif /* INET_NETWORKLAYER_ICN_ROUTINGTABLE_CROUTE_H_ */
