/*
 * Color.h
 *
 *  Created on: 2019年7月1日
 *      Author: hiro
 */

#ifndef INET_NETWORKLAYER_ICN_COLOR_COLOR_H_
#define INET_NETWORKLAYER_ICN_COLOR_COLOR_H_

#include <list>
#include <map>
#include <set>
#include <fstream>

#include "../cacheTable/colorCacheTable.h"
#include "../cacheTable/colorCacheTable.h"
#include "../cacheTable/colorChunk.h"
#include "../pendingTable/colorPendingGetTable.h"
#include "../routingTable/colorRoutingTable.h"
#include "../routingTable/Croute.h"
#include "inet/common/INETDefs.h"
#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/lifecycle/OperationalBase.h"
#include "inet/common/packet/Message.h"
#include "inet/networklayer/contract/INetworkProtocol.h"
#include "inet/networklayer/icn/cluster/ICluster.h"
#include "inet/networklayer/icn/color/ColorFragBuf.h"
#include <openssl/rsa.h>


namespace inet{

    class ColorCacheTable;
    class colorPendingGetTable;
    class ColorRoutingTable;

    class INET_API color : public OperationalBase, public INetworkProtocol, public IProtocolRegistrationListener, public cListener
    {
        public:
            struct SimRcorder
            {
                color *owner;

                bool multiConsumer;

                int index;
                std::map<SID_t, simtime_t> Delays;

                B throughput = B(0);
                int GetSendNum = 0;
                int GetRecvNum = 0;

                int DataSendNum = 0;
                int DataRecvNum =0;
                simtime_t delay = 0;

                void ConsumerPrint(std::ostream &os);

                void ProviderPrint(std::ostream &os);
            };

        private:
            //节点索引
            int nodeIndex;

            //转发表
            IInterfaceTable *ift = nullptr;

            //缓存表
            ColorCacheTable *ct = nullptr;

            //PIT表
            colorPendingGetTable *pit = nullptr;

            //路由表
            ColorRoutingTable *rt = nullptr;

            //最大传输单元
            int mtu;

            //上层协议族
            std::set<const Protocol *> upperProtocols;

            //节点标示NID
            NID_t nid;

            //最大跳数
            int hopLimit;

            //2.4GHz端口
            InterfaceEntry * ie24;

            //5GHz
            InterfaceEntry * ie5;

            //分簇协议模块
            ICluster* clusterModule;

            //数据包重组buffer
            ColorFragBuf* ResemBuffer;

            cMessage* testTimer;

            cMessage* testGet;
            simtime_t testget;

            cMessage* testData;
            simtime_t testdata;

            //测试变量
            simtime_t startTime;
            int Cindex;
            int Pindex;

            std::map<SID_t, simtime_t> Delays;
            B throuput;
            int sendNum;
            int recvNum;
            double sentInterval;
            simtime_t delay;

            SimRcorder testModule;

            enum class SendMode
            {
                EqualInterval = 1,
                UniformDisInterval = 2,
                ExpDisInterval = 3
            };

        protected:
            virtual void finish() override;
            /**
             * IProtocolRegistrationListener methods
             */
            virtual void handleRegisterService(const Protocol& protocol, cGate *out, ServicePrimitive servicePrimitive) override;
            virtual void handleRegisterProtocol(const Protocol& protocol, cGate *in, ServicePrimitive servicePrimitive) override;
            /**
             * OperationalBase methods
             */
            virtual void refreshDisplay() const override;
            virtual int numInitStages() const override { return NUM_INIT_STAGES; }
            virtual void initialize(int stage) override;
            virtual void handleMessageWhenUp(cMessage *msg) override;
            /**
             * ILifecycle methods
             */
            virtual bool isInitializeStage(int stage) override { return stage == INITSTAGE_NETWORK_LAYER; }
            virtual bool isModuleStartStage(int stage) override { return stage == ModuleStartOperation::STAGE_NETWORK_LAYER; }
            virtual bool isModuleStopStage(int stage) override { return stage == ModuleStopOperation::STAGE_NETWORK_LAYER; }
            virtual void handleStartOperation(LifecycleOperation *operation) override;
            virtual void handleStopOperation(LifecycleOperation *operation) override;
            virtual void handleCrashOperation(LifecycleOperation *operation) override;
            
            /// cListener method
            virtual void receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject *details) override;
        public:
            color(){}

            ~color();


            //处理GET包
            void handleGetPacket(Packet* packet);

            //处理DATA包
            void handleDataPacket(Packet* packet);

            //为GET包寻找路由
            shared_ptr<Croute> findRoute(SID_t sid);

            //对收到的DATA包进行缓存
            void CachePacket(SID_t sid, Packet* packet);
            
            //在缓存中寻找内容
            shared_ptr<ContentBlock> findContentInCache(SID_t sid);

            //创建路由条目
            shared_ptr<Croute> createRoute(SID_t sid, simtime_t ttl);

            //创建PIT条目
            void createPIT(const SID_t& sid, const NID_t& nid, const MacAddress& mac,simtime_t t);

            //在PIT表中找到上一跳的信息
            colorPendingGetTable::EntrysRange findPITentry(SID_t sid);

            //对上层来的包封装, type==0代表GET包， type==1代表DATA包
            void encapsulate(Packet *packet, int type, SID_t sid);

            void encapsulate(Packet *packet, int type, int portSelf, int portDest);

            //对下层来的包解封装
            void decapsulate(Packet *packet, SID_t sid);

            //将数据包发往指定端口
            void sendDatagramToOutput(Packet *packet, int nic);

            //处理来自上层的数据包
            void handlePacketFromHL(Packet *packet);

            //处理来自低层的数据包    
            void handleIncomingDatagram(Packet *packet);

            //绑定无线网卡端口
            InterfaceEntry *chooseInterface(const char* interfaceName);

            //得到数据包到达的接口
            const InterfaceEntry *getSourceInterface(Packet *packet);

            //节点作为consumer测试发包，转发路由机制，直接在网络层产生GET包发送
            void testSend(SID_t sid);

            //节点作为provider创建内容包，测试中一个GET包对应一个DATA包
            void testProvide(SID_t sid, B dataSize);

            //通过T控制发送get包的时间间隔， mode选择时间间隔的具体分布形式，0均匀分布，1指数分布
            void scheduleGet(simtime_t t, SendMode mode);

            //测试记录
            void record();

            //分片转发
            void FragmentAndSend(Packet* packet);

            //分片存储
            void FragmentAndStore(Packet* packet, SID_t sid);

            //产生GET包头
            const inet::Ptr<inet::Get> GetHead(SID_t sid);

            //产生Data包头
            const inet::Ptr<inet::Data> DataHead(SID_t sid);
    };
}



#endif /* INET_NETWORKLAYER_ICN_COLOR_COLOR_H_ */
