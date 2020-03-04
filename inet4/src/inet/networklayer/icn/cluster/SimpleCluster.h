/*
 * SimpleCluster.h
 *
 *  Created on: 2019年6月24日
 *      Author: hiro
 */

#ifndef INET_NETWORKLAYER_ICN_CLUSTER_SIMPLECLUSTER_H_
#define INET_NETWORKLAYER_ICN_CLUSTER_SIMPLECLUSTER_H_

#include "inet/common/INETDefs.h"
#include "inet/mobility/contract/IMobility.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/lifecycle/OperationalBase.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/packet/Packet.h"
#include "inet/linklayer/common/MacAddress.h"
#include "inet/networklayer/icn/cluster/SimpleClusterPacket_m.h"
#include "inet/common/bloomfilter/BloomFilter.h"
#include "inet/networklayer/icn/cluster/ICluster.h"
#include <random>
#include <string>


#include<set>

namespace inet{


//提供一个简单的分簇协议根据节点的NID大小选出簇头，保证每个节点要么是簇头，要么有一个簇头为它服务
class INET_API SimpleCluster : public OperationalBase, public ICluster
{
    enum ClusterState
    {
        INI,
        PREHEAD,
        HEAD,
        MEMBER,
        GATEWAY
    };

    static BloomFilter<std::array<Word, 4>> nidFilter;

private:
    //节点NID
    NID nid;

    //随机数产生器
    // std::default_random_engine e;

    //节点状态
    ClusterState state = INI;

    //节点位置
    Coord position;

    //NID最大的邻居节点
    NID maxNeighbor;

    //邻居集合
    std::set<int> *neighbors;

    std::set<int> clusterHeads;

    //检查自己是否是NID最大的节点
    bool isMax();

    //各类计时器
    cMessage *wait = nullptr;
    cMessage *startTimer = nullptr;
    cMessage *hello = nullptr;
    cMessage *collect = nullptr;
    cMessage *retry = nullptr;
    cMessage *iniTimer = nullptr;

    cMessage *neighborsClear = nullptr;
    cMessage *prehead = nullptr;

    //计时器触发间隔
    double startTime = 0;
    double interval = 0;
    double helloTime = 0.5;
    double waitingTime = 0;
    double collectingTime = 0;
    double iniTime = 1;
    double nbClearInterval = 0.5;
    double PreHeadChange = 0;
    unsigned retryTimes = 0;

    //测试
    //节点的索引
    int nodeIndex = 0;

    //记录文件名
    std::string path;
    std::string filename;
    std::string inifile;

    //簇头的切换次数
    static int change;
    static double proTime;

protected:
    //data members

    InterfaceEntry *ie24 = nullptr; //2.4GHz interface
    InterfaceEntry *ie5 = nullptr;  //5GHz interface

    IInterfaceTable *ift = nullptr;
    ClusterTable clusterTable;

protected:
    //functions
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void refreshDisplay() const override;
    virtual void finish() override;

    virtual void handleMessageWhenUp(cMessage *msg) override;

    //        virtual bool handleOperationStage(LifecycleOperation *operation, IDoneCallback *doneCallback) override;
    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;

    virtual bool isInitializeStage(int stage) override { return stage == INITSTAGE_NETWORK_LAYER; }
    virtual bool isModuleStartStage(int stage) override { return stage == ModuleStartOperation::STAGE_NETWORK_LAYER; }
    virtual bool isModuleStopStage(int stage) override { return stage == ModuleStopOperation::STAGE_NETWORK_LAYER; }

public:
    //返回簇头
    std::set<int> &getHeads() override { return clusterHeads; }

    //返回节点所有的簇头
    const ClusterTable getClusterHead() override;

    //判断自己是否是簇头
    bool isHead() override;

    bool isPreHead();

    //is gateway node?
    bool isGateWay() override;

    //选择接口
    InterfaceEntry *chooseInterface(const char *interfaceName);

    //刷新显示
    void flush();

    //处理分簇协议的数据包
    void processClusterPacket(Packet *packet);

    //处理接入认证数据包
    void processAuthPacket(Packet *packet);

    //处理各个计时器
    void handleSelfMessage(cMessage *msg);

    //exchange information between neighbors, for clustering
    void sendCluster(PacketType type);

    //发送认证消息,0是初始消息，1是回复消息,time用来记录传输时间
    void sendAuth(int type, double time);

    //通告自己成为簇头
    void sendHeadAdvertise(); //

    void scheduleHello();
    void scheduleRetry();
    void scheduleWait();

    Coord getPosition();

    //成为预簇头
    void becomePrehead();

    //成为簇头
    void becomeHead();

    //退化成簇成员
    void becomeMember();

    void becomeGateway();

    //记录输出分簇信息到文件
    void recorder(std::string filename);

    //拓扑信息
    void topology(bool all, std::string filename);

    friend std::ostream &operator<<(std::ostream &os, const SimpleCluster::ClusterEntry &entry);

public:
    SimpleCluster();
    ~SimpleCluster();
};

std::ostream& operator<<(std::ostream& os, const SimpleCluster::ClusterEntry& entry)
{
    os  <<  "header NID is: " << entry.clusterhead << "macAddr is:  "<< entry.headAddr<<"last update time is:   "<< entry.lastUpdate << endl;
    return os;
}

}
#endif /* INET_NETWORKLAYER_ICN_CLUSTER_SIMPLECLUSTER_H_ */
