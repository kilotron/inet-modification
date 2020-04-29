/*
 * colorRoutingTable.cc
 *
 *  Created on: 2019年7月5日
 *      Author: hiro
 */

#include "colorRoutingTable.h"

#include "inet/common/INETUtils.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/Simsignals.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/networklayer/contract/IInterfaceTable.h"
#include <memory>

namespace inet
{

Define_Module(ColorRoutingTable);

ColorRoutingTable::~ColorRoutingTable()
{
}

void ColorRoutingTable::initialize(int stage)
{
    cSimpleModule::initialize(stage);

    if (stage == INITSTAGE_LOCAL)
    {
        cModule *host = getContainingNode(this);
        forwarding = par("forwarding");
    }
}

void ColorRoutingTable::handleMessage(cMessage *msg)
{
}

void ColorRoutingTable::CreateEntry(const NID &dest, const NID &nextHop, const MacAddress &mac, const simtime_t &ttl, int interFace, double linkQ)
{
    auto range = routingTable.equal_range(dest);
   
   //先检查路由表中是否有完全相同的表项，若有在生存期更长的情况下更新生存期
    for (auto iter = range.first; iter != range.second; iter++)
    {
       if((iter->second->getNextHop() == nextHop && iter->second->getNextMac() == mac))
       {
           if (ttl + simTime() > iter->second->getLifeTime())
           {
               iter->second->setLifeTime(ttl + simTime());
           }

           return;
       }
       
        //对于链路质量差的不插入条目
       if (iter->second->getLinkQlt() >= linkQ)
       {
           return;
       }
    }

    //创建并插入新的路由表项
    auto route = std::make_shared<Croute>(nextHop, mac, ttl + simTime(), interFace, linkQ);
    routingTable.insert({dest, route});
}

void ColorRoutingTable::printRoutingTable(std::ostream &out)
{
}

cModule *ColorRoutingTable::getHostModule()
{
    return findContainingNode(this);
}

shared_ptr<Croute> ColorRoutingTable::findRoute(NID dest)
{
    auto range = routingTable.equal_range(dest);
    auto result = routingTable.end();
    /*返回链路质量最好路径对应的表项，并且在每次查找时检查路由表项是否过期
    如果过期则应该删除表项，没有对应路径时返回空指针*/    
    for (auto iter = range.first; iter != range.second; iter++)
    {
        if (iter->second->getLifeTime() > simTime())
        {
            if (result == routingTable.end() || result->second->getLinkQlt() < iter->second->getLinkQlt())
                result = iter;
        }  
        else
        {
            routingTable.erase(iter);
            break;
        }
    }
    if (result == routingTable.end())
        return nullptr;
    else
        return result->second;
}



void ColorRoutingTable::removeEntry(NID nid)
{
    routingTable.erase(nid);
}

} // namespace inet
