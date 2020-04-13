/*
 * Color.h
 *
 *  Created on: 2019年7月1日
 *      Author: hiro
 */

#ifndef INET_NETWORKLAYER_ICN_COLOR_COLOR_H_
#define INET_NETWORKLAYER_ICN_COLOR_COLOR_H_

#include <list>
#include <queue>
#include <map>
#include <set>
#include <fstream>

#include "../cacheTable/colorCacheTable.h"
#include "../cacheTable/colorChunk.h"
#include "../pendingTable/colorPendingGetTable.h"
#include "../routingTable/colorRoutingTable.h"
//#include "../routingTable/Croute.h"
#include "inet/common/INETDefs.h"
#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/lifecycle/OperationalBase.h"
#include "inet/common/packet/Message.h"
#include "inet/networklayer/contract/INetworkProtocol.h"
#include "inet/networklayer/icn/cluster/ICluster.h"
#include "inet/networklayer/icn/color/ColorFragBuf.h"
#include "inet/networklayer/icn/field/NID.h"
#include "inet/networklayer/icn/field/SID.h"
#include "inet/networklayer/icn/delayQueue/delayQueue.h"
#include "inet/networklayer/contract/INetfilter.h"



namespace inet{

    class ColorCacheTable;
    class colorPendingGetTable;
    class ColorRoutingTable;

    class INET_API colorCluster : public OperationalBase, public INetworkProtocol, public NetfilterBase, public IProtocolRegistrationListener, public cListener
    {
        public:
            struct SimRecorder
            {
                colorCluster *owner;

                int multiConsumer;

                int index;
                std::map<SID, simtime_t> Delays;
                std::vector<double> delayArray;

                B throughput = B(0);
                int GetSendNum = 0;
                int GetRecvNum = 0;

                int DataSendNum = 0;
                int DataRecvNum =0;
                simtime_t delay = 0;

                void ConsumerPrint(std::ostream &os);

                void ProviderPrint(std::ostream &os);
            };

            struct SocketDescriptor
            {
                int socketId = -1;
                int protocolId = -1;
                NID nid;
                int localPort;
                

                SocketDescriptor(int socketId, int protocolId, const NID& nid ,int port)
                        : socketId(socketId), protocolId(protocolId), nid(nid),localPort(port) {}
            };

            class GETidentifier
            {
                public:
                    SID sid;
                    NID requester;
                    simtime_t servedTime;
                    GETidentifier(const SID& sid, const NID& nid, simtime_t ST) : sid(sid), requester(nid), servedTime(ST){}
                    bool operator == (const GETidentifier& other) const
                    {
                        return sid == other.sid && requester == other.requester;
                    }
            };
            
            class GETidentifierComparor
            {
                bool operator ()(const GETidentifier& lhs, const GETidentifier& rhs)
                {
                    if (lhs.sid == rhs.sid)
                        return lhs.requester < rhs.requester;
                    else return lhs.sid < rhs.sid;
                }
            };

            enum class SendMode
            {
                EqualInterval = 1,
                UniformDisInterval = 2,
                ExpDisInterval = 3
            };


        private:
            //节点索引
            int nodeIndex;

            std::map<GETidentifier, simtime_t, GETidentifierComparor> getTable;

            std::map<int, SocketDescriptor *> socketIdToSocketDescriptor;

            std::map<int, SocketDescriptor *> socketsByPortMap;

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
            NID nid;

            //最大跳数
            int hopLimit;

            //端口
            InterfaceEntry * ie;


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
            int Cindex = 10000;


            double sentInterval;
   
            SimRecorder testModule;

            delayQueue delay_queue24;
            cMessage* delayer24;


            double getDelayTime;
            double dataDelayTime;
            int TC;

            //处理时延,两种包的处理时延不同
            double getProdelay;
            double dataProdelay;

            bool flood = true;
            bool unicast;
            double routeLifeTime;

        protected:
            virtual void
            finish() override;
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
        
            /**
             * called before a packet arriving from the network is routed
             */
            IHook::Result datagramPreRoutingHook(Packet *datagram);

            /**
             * called before a packet arriving from the network is delivered via the network
             */
            IHook::Result datagramForwardHook(Packet *datagram);

            /**
             * called before a packet is delivered via the network
             */
            IHook::Result datagramPostRoutingHook(Packet *datagram);

            /**
             * called before a packet arriving from the network is delivered locally
             */
            IHook::Result datagramLocalInHook(Packet *datagram);

            /**
             * called before a packet arriving locally is delivered
             */
            IHook::Result datagramLocalOutHook(Packet *datagram);

        public:
            /**
             * registers a Hook to be executed during datagram processing
             */
            virtual void registerHook(int priority, IHook *hook) override;

            /**
             * unregisters a Hook to be executed during datagram processing
             */
            virtual void unregisterHook(IHook *hook) override;

            /**
             * drop a previously queued datagram
             */
            virtual void dropQueuedDatagram(const Packet *datagram) override;

            /**
             * re-injects a previously queued datagram
             */
            virtual void reinjectQueuedDatagram(const Packet *datagram) override;

            colorCluster(){}

            ~colorCluster();


            //处理GET包
            void handleGetPacket(Packet* packet);

            //处理DATA包
            void handleDataPacket(Packet* packet);

            //为GET包寻找路由
            shared_ptr<Croute> findRoute(const NID &sid);

            //对收到的DATA包进行缓存
            void CachePacket(const SID &sid, Packet* packet);
            
            //在缓存中寻找内容
            shared_ptr<ContentBlock> findContentInCache(const SID &sid);

            //创建路由条目
            void createRoute(const NID &dest, const NID &nextHop, const MacAddress &mac, const simtime_t &ttl, int interFace, double linkQ);

            //创建PIT条目
            void createPIT(const SID& sid, const NID& nid, const MacAddress& mac,simtime_t t);

            //在PIT表中找到上一跳的信息
            colorPendingGetTable::EntrysRange findPITentry(const SID &sid);

            //对上层来的包封装, type==0代表GET包， type==1代表DATA包
            void encapsulate(Packet *packet, int type, const SID& sid);

            void encapsulate(Packet *packet, int type, const SID& sid, int port);
      
            //对下层来的包解封装
            void decapsulate(Packet *packet, const SID& sid);

            void cacheData(const SID &sid, Packet *packet);

            //将数据包通过指定端口发送
            void sendDatagramToOutput(Packet *packet, int nic, const MacAddress& mac = MacAddress::BROADCAST_ADDRESS);

            //将数据包发往下一跳，通过nid指定下一跳
            // void sendOutToNode(Packet* pakcet, NID nid);

            //处理来自上层的数据包
            void handlePacketFromHL(Packet *packet);

            //处理来自低层的数据包    
            void handleIncomingDatagram(Packet *packet);

            //绑定无线网卡端口
            InterfaceEntry *chooseInterface(const char* interfaceName);

            //得到数据包到达的接口
            const InterfaceEntry *getSourceInterface(Packet *packet);

            //节点作为consumer测试发包，转发路由机制，直接在网络层产生GET包发送
            void testSend(const SID &sid);

            void sendGET(const SID &sid, int port);

            //节点作为provider创建内容包，测试中一个GET包对应一个DATA包
            void testProvide(const SID &sid, const B& dataSize);

            //通过T控制发送get包的时间间隔， mode选择时间间隔的具体分布形式，0均匀分布，1指数分布
            void scheduleGet(simtime_t t, SendMode mode);

            //测试记录
            void record();

            //分片转发
            void FragmentAndSend(Packet* packet);

            //分片存储
            void FragmentAndStore(Packet* packet, const SID& sid);

            //产生GET包头
            const Ptr<inet::Get> GetHead(const SID &sid);

            //产生Data包头
            const Ptr<inet::Data> DataHead(const SID &sid);

            void sendUp(Packet *pkt, SocketDescriptor *sd);

            //判断是否是
            bool isForwarder();

            bool isHead(){return clusterModule->isHead() || clusterModule->isPreHead();}

            bool isGateway(){return clusterModule->isGateWay();}

            void handleRequest(Request *request);

            // void handleDelayPkt(Packet *pkt, int seq=0, simtime_t delay=0);

    };

 
}



#endif /* INET_NETWORKLAYER_ICN_COLOR_COLOR_H_ */
