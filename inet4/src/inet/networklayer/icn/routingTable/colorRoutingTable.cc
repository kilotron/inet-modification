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

namespace inet{
    Define_Module(ColorRoutingTable);

    ColorRoutingTable::~ColorRoutingTable()
    {
        for(auto &i : timers)
            delete i.first;
    }

    void ColorRoutingTable::initialize(int stage)
    {
        cSimpleModule::initialize(stage);

        if(stage == INITSTAGE_LOCAL)
        {
            cModule *host = getContainingNode(this);
            host->subscribe(interfaceCreatedSignal, this);
            host->subscribe(interfaceDeletedSignal, this);
            host->subscribe(interfaceStateChangedSignal, this);
            host->subscribe(interfaceConfigChangedSignal, this);
            host->subscribe(interfaceIpv4ConfigChangedSignal, this);

            //得到指向转发表的指针
            auto name = getParentModule()->getParentModule()->getFullPath();
            name=name+".interfaceTable";
            auto path = name.c_str();
            cModule *mod = this->getModuleByPath(path);
            ift = dynamic_cast<IInterfaceTable *>(mod);

            forwarding = par("forwarding");
        }
        else if(stage == INITSTAGE_STATIC_ROUTING)
        {
            cModule *node = findContainingNode(this);
            NodeStatus *nodeStatus = node ? check_and_cast_nullable<NodeStatus *>(node->getSubmodule("status")) : nullptr;
            isNodeUp = !nodeStatus || nodeStatus->getState() == NodeStatus::UP;
        }
    }

    bool ColorRoutingTable::handleOperationStage(LifecycleOperation *operation, IDoneCallback *doneCallback)
    {
        Enter_Method_Silent(); 
        int stage = operation->getCurrentStage();
        if (dynamic_cast<ModuleStartOperation *>(operation)) {
            if (static_cast<ModuleStartOperation::Stage>(stage) == ModuleStartOperation::STAGE_NETWORK_LAYER) {
                // read routing table file (and interface configuration)
                
            }
            else if (static_cast<ModuleStartOperation::Stage>(stage) == ModuleStartOperation::STAGE_TRANSPORT_LAYER) {
            
                isNodeUp = true;
            }
        }
        else if (dynamic_cast<ModuleStopOperation *>(operation)) {
            if (static_cast<ModuleStopOperation::Stage>(stage) == ModuleStopOperation::STAGE_NETWORK_LAYER) {
                
                isNodeUp = false;
            }
        }
        else if (dynamic_cast<ModuleCrashOperation *>(operation)) {
            if (static_cast<ModuleCrashOperation::Stage>(stage) == ModuleCrashOperation::STAGE_CRASH) {
                
                isNodeUp = false;
            }
        }
        return true;
    }

    void ColorRoutingTable::receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject *details)
    {
        if (getSimulation()->getContextType() == CTX_INITIALIZE)
        return; // ignore notifications during initialize

        Enter_Method_Silent();
        printSignalBanner(signalID, obj, details);
    }

    void ColorRoutingTable::handleMessage(cMessage * msg)
    {
        auto it = timers.find(msg);
        removeEntry(it->second);
        delete msg;
    }

    shared_ptr<Croute> ColorRoutingTable::CreateEntry(SID sid, NID nid, simtime_t t)
    {
        shared_ptr<Croute> route = std::make_shared<Croute>(nid,sid,this,t);
        cMessage* msg = new cMessage();

        table.insert(std::make_pair(sid,route));
        RT[route] = sid;
        timers[msg] = route;
        Rtimers[route] = msg;
        return route;
    }

    void ColorRoutingTable::printRoutingTable(std::ostream & out)
    {
        for(auto &iter : table)
        {
            out<<iter.second->str();
        }
    }

    cModule *ColorRoutingTable::getHostModule()
    {
        return findContainingNode(this);
    }

    shared_ptr<Croute> ColorRoutingTable::findMachEntry(SID sid)
    {
        auto pair = table.find(sid);
        return pair->second;
    }

    void ColorRoutingTable::removeEntry(shared_ptr<Croute> croute)
    {
        auto rtEntry = RT.find(croute);
        auto range = table.equal_range(rtEntry->second);
        

        for(auto head = range.first;head!=range.second;head++)
        {
            if(head->second == croute)
                table.erase(head->first);
        }

        RT.erase(croute);
        rmFromTimers(croute);
    }

    void ColorRoutingTable::removeEntry(SID sid)
    {
        auto range = table.equal_range(sid);

        for(auto head = range.first;head!=range.second;head++)
        {
            RT.erase(head->second);
            rmFromTimers(head->second);
        }
        table.erase(sid);
    }

    void ColorRoutingTable::rmFromTimers(shared_ptr<Croute> croute)
    {
        auto timerPair = Rtimers.find(croute);
        timers.erase(timerPair->second);
        delete timerPair->second;
        Rtimers.erase(timerPair->first);
    }

    shared_ptr<Croute> ColorRoutingTable::findRoute(SID sid)
    {
        return table.find(sid)->second;
    }
}


