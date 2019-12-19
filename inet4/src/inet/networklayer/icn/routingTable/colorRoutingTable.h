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
#include "Croute.h"
#include "inet/networklayer/icn/field/compare.h"

namespace inet{

class Croute;
using std::shared_ptr;
class ColorRoutingTable : public cSimpleModule, protected cListener, public ILifecycle
{
    private:
        //转发表
        IInterfaceTable *ift = nullptr;

        //路由表, 每个SID可能对应多条路径
        typedef std::multimap<SID, std::shared_ptr<Croute>>  Table;
        typedef std::map<shared_ptr<Croute>,SID> ReverseTable;
        //为每个路由表项维护一个生存周期值
        typedef std::map<cMessage*,shared_ptr<Croute>> timerTable;
        typedef std::map<shared_ptr<Croute>, cMessage*> RTimerTable;
        
        Table table;
        ReverseTable RT;
        timerTable timers;
        RTimerTable Rtimers;

        bool isNodeUp;
        //是否开启转发
        bool forwarding = false;

        void rmFromTimers(shared_ptr<Croute> croute);

        ColorRoutingTable(const ColorRoutingTable&);
        ColorRoutingTable& operator = (const ColorRoutingTable);
    public:
        ColorRoutingTable(){}
        ~ColorRoutingTable();
        //创建一条路由表向
        shared_ptr<Croute> CreateEntry(SID sid, NID nid, simtime_t t );

        //打印路由表
        void printRoutingTable(std::ostream & out);

        //返回主机节点指针
        cModule *getHostModule();

        //通过SID查找路由表
        shared_ptr<Croute> findMachEntry(SID sid);

        //通过route指针删除路由表项
        void removeEntry(shared_ptr<Croute> croute);

        //通过SID删除路由表项
        void removeEntry(SID sid);
    protected:
        virtual void handleMessage(cMessage *)override;
        virtual int numInitStages() const override { return NUM_INIT_STAGES; }
        virtual void initialize(int stage) override;
        virtual void receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject *details) override;
        virtual bool handleOperationStage(LifecycleOperation *operation, IDoneCallback *doneCallback) override;

        shared_ptr<Croute> findRoute(SID sid);
};
}



#endif /* INET_NETWORKLAYER_ICN_ROUTINGTABLE_COLORROUTINGTABLE_H_ */
