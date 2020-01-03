/*
 * colorRoutingTable.h
 *
 *  Created on: 2019年7月5日
 *      Author: hiro
 */

#ifndef INET_NETWORKLAYER_ICN_ROUTINGTABLE_COLORROUTINGTABLE_H_
#define INET_NETWORKLAYER_ICN_ROUTINGTABLE_COLORROUTINGTABLE_H_

#include <vector>

#include "inet/common/INETDefs.h"
#include "inet/common/lifecycle/ILifecycle.h"
#include "inet/networklayer/common/InterfaceEntry.h"
#include "inet/networklayer/icn/field/SID.h"

#include<map>
#include<memory>
#include<iostream>
#include "Croute_m.h"


namespace inet{

class Croute;
using std::shared_ptr;
using std::unique_ptr;
class ColorRoutingTable : public cSimpleModule
{
    private:
        //转发表
        // IInterfaceTable *ift = nullptr;

        //路由表, 路由表依然以主机为中心建立，每个NID可能对应多条路径
        using Table = std::multimap<NID, std::shared_ptr<Croute>>;
        // typedef std::map<shared_ptr<Croute>,SID> ReverseTable;
        // //为每个路由表项维护一个生存周期值
        // typedef std::map<cMessage*,shared_ptr<Croute>> timerTable;
        // typedef std::map<shared_ptr<Croute>, cMessage*> RTimerTable;
        
        Table routingTable;
        // ReverseTable RT;
        // timerTable timers;
        // RTimerTable Rtimers;

        bool isNodeUp;
        //是否开启转发
        bool forwarding = false;

        ColorRoutingTable(const ColorRoutingTable&);
        ColorRoutingTable& operator = (const ColorRoutingTable);
    public:
        ColorRoutingTable(){}
        ~ColorRoutingTable();
        //创建一条路由表向
        void CreateEntry(NID dest, NID nextHop, MacAddress mac, simtime_t ttl, int interFace, double linkQ );

        //打印路由表
        void printRoutingTable(std::ostream & out);

        //返回主机节点指针
        cModule *getHostModule();

        //通过SID查找路由表
        shared_ptr<Croute> findRoute(NID dest);

        //通过NID删除路由表项
        void removeEntry(NID nid);
    protected:
        virtual void handleMessage(cMessage *)override;
        virtual int numInitStages() const override { return NUM_INIT_STAGES; }
        virtual void initialize(int stage) override;
        

};
}



#endif /* INET_NETWORKLAYER_ICN_ROUTINGTABLE_COLORROUTINGTABLE_H_ */
