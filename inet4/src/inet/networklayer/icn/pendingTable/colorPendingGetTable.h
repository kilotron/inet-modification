/*
 * colorPendingGetTable.h
 *
 *  Created on: 2019年7月5日
 *      Author: hiro
 */

#ifndef INET_NETWORKLAYER_ICN_PENDINGTABLE_COLORPENDINGGETTABLE_H_
#define INET_NETWORKLAYER_ICN_PENDINGTABLE_COLORPENDINGGETTABLE_H_

#include "inet/common/INETDefs.h"



#include <map>
#include <iostream>
#include "PITentry_m.h"


namespace inet{

class PITentry;

class INET_API colorPendingGetTable: public cSimpleModule
{
    private:
        using timerTable = std::map<cMessage *, SID>;
        std::map<SID,cMessage*>* sids;

        //PIT表的核心，一个SID可能对应多个GET包
        std::multimap<SID,PITentry>* table;

        //为每条记录分配一个自消息用作定时器
        timerTable* timers;

        colorPendingGetTable(const colorPendingGetTable& pit){};
        colorPendingGetTable& operator = (const colorPendingGetTable pit);

    protected:
        virtual int numInitStages() const override { return NUM_INIT_STAGES; }
        virtual void initialize(int stage) override;
        virtual void handleMessage(cMessage *message) override;

        void finish() override;

    public:
        colorPendingGetTable():cSimpleModule(){};
        //一个GET包到来时进行记录，包括上一跳的nid，一个PIT条目的生存时间
        //PIT中的一条记录
        using Entry = std::pair<SID, PITentry>;

        /*一个SID可能对应多条记录，因此在PIT查表时将multimap中对应查询结果
        的头尾两个迭代器作为pair返回 */
        using EntrysRange = std::pair<std::multimap<SID, PITentry>::iterator,
                                    std::multimap<SID, PITentry>::iterator>;

        ~colorPendingGetTable();
        
        //打印PIT
        void PrintPIT(std::ostream & out);
        
        //根据SID，NID，生存时间创建一个PIT表项
        const colorPendingGetTable::Entry& createEntry(const SID& sid, const NID& nid, const MacAddress& mac,simtime_t t, int type = 5, unsigned long Nonce = 0, bool is_consumer = false);
        
        //将表项添加到表中
        void AddPITentry(const Entry& entry);

        //根据SID移除表项
        void RemoveEntry(const SID& sid);

        //根据迭代器移除表项
        void RemoveEntry(const std::multimap<SID, PITentry>::iterator);

        //返回SID对应的表项
        EntrysRange findPITentry(const SID &sid);

        //判断是否是GET包的请求者
        bool isConsumer(const SID& sid);

        bool servedForThisGet(const SID &sid, unsigned long Nonce);

        //查询PIT中是否有关于这个SID的条目
        bool hasThisSid(const SID& sid){return !(table->find(sid)==table->end());}

        std::multimap<SID,PITentry>::iterator getTableEnd(){return table->end();}
};

}


#endif /* INET_NETWORKLAYER_ICN_PENDINGTABLE_COLORPENDINGGETTABLE_H_ */
