/*
 * colorCacheTable.h
 *
 *  Created on: 2019年7月5日
 *      Author: hiro
 */

#ifndef INET_NETWORKLAYER_ICN_CACHETABLE_COLORCACHETABLE_H_
#define INET_NETWORKLAYER_ICN_CACHETABLE_COLORCACHETABLE_H_

#include "inet/common/INETDefs.h"
#include "inet/common/lifecycle/ILifecycle.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/icn/field/dataType.h"
#include "inet/networklayer/icn/field/compare.h"
#include  <iostream>

#include<map>
#include<memory>
#include "contentBlock.h"
/*
缓存表采用三级目录结构，每个SID对应一个block，每个block中有多个chunk，chunk用链表的形式组织起来，每个chunk中可以存放多个packet
这样对于有序的数据流可以提供局部完整的缓存，每个节点中对应的缓存可能是整体不完整，但是每个chunk中是完整的，在AD-HOC网络中可以
利用多路径回传提高效率和冗余性，对于一个SID对应一个DATA包的情况可以退化到一个block中单个chunk的情况
 */
namespace inet{

class ContentBlock;
using std::shared_ptr;
class INET_API ColorCacheTable: public cSimpleModule,protected cListener, public ILifecycle
{
    private:
        //缓存表的数据核心，一个SID对应一个block
        typedef std::map<SID_t,shared_ptr<ContentBlock>> CacheTable;
        CacheTable table;

        //缓存表的父节点指针
        cModule* owner;

        //为每个缓存条目维护一个计时器，计时器记录生存时间，生存时间为0时删除缓存，也可以为之后的缓存替换策略提供支持
        typedef std::map<cMessage*,SID_t> timerTable;
        timerTable timers;

        //网络层的MTU
        unsigned mtu;

        //节点初始化是否完成
        bool isNodeUp;

        //缓存大小
        B size;

        //剩余缓存大小
        B remain;
        
    public:
        ColorCacheTable(){};
        ~ColorCacheTable(){};

        //原则上缓存表禁止进行拷贝和赋值
        ColorCacheTable(const ColorCacheTable&) = delete;
        ColorCacheTable& operator = (const ColorCacheTable) = delete;

        void setOwner(cModule* own){owner = own;};

        //打印缓存表信息
        void printCacheTable(std::ostream & out);

        //根据SID返回对应的block
        shared_ptr<ContentBlock> getBlock(SID_t sid);
        
        //创建block，并相应的创建计时器
        shared_ptr<ContentBlock> CreateBlock(SID_t sid);

        //存入packet
        void CachePacket(SID_t sid,Packet* packet);

        //移除缓存
        void RemoveBlock(shared_ptr<ContentBlock> block);

        //检查此数据包是否已经在缓存中 
        bool hasThisPacket(Packet *packet,SID_t sid);

    protected:
        void finish() override;
        virtual int numInitStages() const override { return NUM_INIT_STAGES; }
        virtual void initialize(int stage) override;

        virtual cModule *getHostModule(){return owner;}

        virtual void handleMessage(cMessage *) override;
        
        virtual void receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject *details) override;

        virtual bool handleOperationStage(LifecycleOperation *operation, IDoneCallback *doneCallback) override;
};


}



#endif /* INET_NETWORKLAYER_ICN_CACHETABLE_COLORCACHETABLE_H_ */
