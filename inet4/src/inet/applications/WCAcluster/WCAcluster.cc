#include "inet/applications/WCAcluster/WCAcluster.h"

#include <math.h>
#include <utility>
#include <string.h>
#include <map>
#include <stdio.h>
#include <omnetpp.h>
#include <float.h>
#include <limits.h>
#include "inet/mobility/contract/IMobility.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/common/Simsignals.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/networklayer/ipv4/Ipv4InterfaceData.h"
#include "inet/applications/dhcp/DhcpClient.h"
#include "inet/applications/dhcp/DhcpMessage_m.h"
#include "inet/applications/WCAcluster/WCAcluster.h"
#include "inet/applications/WCAcluster/WcaMessage_m.h"
#include "inet/power/contract/IEpEnergyStorage.h"
#include "inet/power/generator/AlternatingEpEnergyGenerator.h"

namespace inet {

Define_Module(WCAcluster);

WCAcluster::WCAcluster()
{

}

WCAcluster::~WCAcluster()
{
    cancelAndDelete(startTimer);
    cancelAndDelete(timerH1);
    cancelAndDelete(timerH2);
    cancelAndDelete(timerH3);
    cancelAndDelete(timerR1);
    cancelAndDelete(timerWR1);
    cancelAndDelete(timerA123);
    cancelAndDelete(timerT0);
    cancelAndDelete(timerT1);
    cancelAndDelete(timerT2);
    cancelAndDelete(timerT3);
    cancelAndDelete(timerT4);
    cancelAndDelete(timerT5);
    cancelAndDelete(timerT6);
}

void WCAcluster::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        

        //id=getHost();
        num_neighbour=0;
        num_member=0;
        selfwe=0;
        // WCA UDP ports
        Port = 2233;
        // get the routing table to update and subscribe it to the blackboard
        startTimer=new cMessage("Starting",START_WCA);
        timerH1=new cMessage("H1 timer",WAIT_INIT);
        timerH2=new cMessage("H2 timer",WAIT_WEIGHT);
        timerH3=new cMessage("H3 timer",H3);
        timerR1=new cMessage("R1 timer",R1);
        timerWR1=new cMessage("WR1 timer",WAIT_R1);
        timerA123=new cMessage("A123 timer",A123);
        timerT0=new cMessage("T0 timer",T0);
        timerT1=new cMessage("T1 timer",T1);
        timerT2=new cMessage("T2 timer",T2);
        timerT3=new cMessage("T3 timer",T3);
        timerT4=new cMessage("T4 timer",T4);
        timerT5=new cMessage("T5 timer",T5);
        timerT6=new cMessage("T6 timer",T6);

        host_state=INI;

        WATCH(host_state);
        WATCH(head_id);
        WATCH(num_member);
        WATCH(num_neighbour);
        WATCH(selfwe);
        WATCH(id);
        WATCH(minid);
        WATCH(minwe);

        FILE *stream;
        stream = fopen("res.txt", "w+");
        fclose(stream);

        startTime=par("startTime");

    }
    else if (stage == INITSTAGE_APPLICATION_LAYER) {
        // get the hostname
        host = getContainingNode(this);
        // for a wireless interface subscribe the association event to start the WCA protocol
        host->subscribe(l2AssociatedSignal, this);
        host->subscribe(interfaceDeletedSignal, this);
        socket1.setOutputGate(gate("socketOut"));
        socket1.setCallback(this);

    }
}

void WCAcluster::finish()
{
    cancelEvent(startTimer);
    cancelEvent(timerH1);
    cancelEvent(timerH2);
    cancelEvent(timerH3);
    cancelEvent(timerR1);
    cancelEvent(timerWR1);
    cancelEvent(timerA123);
    cancelEvent(timerT0);
    cancelEvent(timerT1);
    cancelEvent(timerT2);
    cancelEvent(timerT3);
    cancelEvent(timerT4);
    cancelEvent(timerT5);
    cancelEvent(timerT6);
}


InterfaceEntry *WCAcluster::chooseInterface()
{
    IInterfaceTable *ift = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);
    const char *interfaceName = par("interface");
    InterfaceEntry *ie = nullptr;

    if (strlen(interfaceName) > 0) {
        ie = ift->getInterfaceByName(interfaceName);
        if (ie == nullptr)
            throw cRuntimeError("Interface \"%s\" does not exist", interfaceName);
    }
    else {
        // there should be exactly one non-loopback interface that we want to configure
        for (int i = 0; i < ift->getNumInterfaces(); i++) {
            InterfaceEntry *current = ift->getInterface(i);
            if (!current->isLoopback()) {
                if (ie)
                    throw cRuntimeError("Multiple non-loopback interfaces found, please select explicitly which one you want to configure via DHCP");
                ie = current;
            }
        }
        if (!ie)
            throw cRuntimeError("No non-loopback interface found to be configured via DHCP");
    }

/*    if (!ie->ipv4Data()->getIPAddress().isUnspecified())
        throw cRuntimeError("Refusing to start DHCP on interface \"%s\" that already has an IP address", ie->getInterfaceName());*/
    return ie;
}

void WCAcluster::handleMessageWhenUp(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        handleTimer(msg);
    }
    else if (msg->arrivedOn("socketIn")) {
        socket1.processMessage(msg);
    }
    else
        throw cRuntimeError("Unknown incoming gate: '%s'", msg->getArrivalGate()->getFullName());
}

void WCAcluster::socketDataArrived(UdpSocket *socket, Packet *packet)
{
    // process incoming packet
    processWCAMessage(packet);
    delete packet;
}

void WCAcluster::socketErrorArrived(UdpSocket *socket, Indication *indication)
{
    EV_WARN << "Ignoring UDP error report " << indication->getName() << endl;
    delete indication;
}

void WCAcluster::socketClosed(UdpSocket *socket_)
{
    if (operationalState == State::STOPPING_OPERATION && !socket1.isOpen())
        startActiveOperationExtraTimeOrFinish(par("stopOperationExtraTime"));
}

void WCAcluster::handleTimer(cMessage *msg)
{
    int category = msg->getKind();

    if(category==START_WCA){
        openSocket();
        scheduleTimerH1();
        getvel();
        getloc();
        sendhello_1();
        simtime_t t=uniform(0.1*h1.dbl(),0.9*h1.dbl());
        scheduleTimerT0(t);
        EV_DETAIL << "Send hello_1 " << endl;
    }
    else if(category==T0){//send hello_1
        getvel();
        getloc();
        sendhello_1();
        EV_DETAIL << "Send hello_1 again " << endl;
    }
    else if(category==WAIT_INIT){
        if(host_state==INI || host_state==IDLE){
            if(num_neighbour>0){
                scheduleTimerH2();
                simtime_t t=uniform(0.1*h2.dbl(),0.9*h2.dbl());
                scheduleTimerT1(t);
                minid=id;
                minwe=DBL_MAX;///////////////////
            }
            else{
                scheduleTimerH1();
                simtime_t t=uniform(0.1*h1.dbl(),0.9*h1.dbl());
                scheduleTimerT0(t);
                members.clear();
            }
        }
        else if(host_state==MEMBER){//join some head
            simtime_t t=uniform(0.5*a123.dbl(),0.8*a123.dbl());
            scheduleTimerT5(t);
            scheduleTimerA123();
            geth3=false;
        }
        else{
            EV_WARN << id << " : WAIT INIT for error state " << host_state << endl;
        }
    }
    else if(category==T1){//send hello_2
        cal_weight();
        if(minwe>selfwe){
            minwe=selfwe;///////////////
            minid=id;//the min weight and id is itself on start
        }
        sendhello_2();
        EV_DETAIL << "Send hello_2 " << endl;
    }
    else if(category==WAIT_WEIGHT){
        if(host_state==INI || host_state==IDLE){
            if(minid==id){
                  scheduleTimerR1();//wait ack_1,after request_1
                  simtime_t t=uniform(0.01*r1.dbl(),0.3*r1.dbl());
                  scheduleTimerT2(t);
                  host_state=PREHEAD;
                  num_member=num_neighbour;
                    }
            else{
                  minid=id;
                  minwe=selfwe;
                  scheduleTimerWR1();//wait request_1
                  simtime_t t=uniform(0.5*wr1.dbl(),0.8*wr1.dbl());
                  scheduleTimerT3(t);
                  host_state=PREMEMBER;
                  EV_DETAIL << "Wait request_1 " << endl;
                    }
        }
        else if(host_state==MEMBER){//join some head
            simtime_t t=uniform(0.5*a123.dbl(),0.8*a123.dbl());
            scheduleTimerT5(t);
            scheduleTimerA123();
            geth3=false;
        }
        else{
            EV_WARN << id << " : WAIT WEIGHT for error state " << host_state << endl;
        }
    }
    else if(category==T2){//send request_1
        sendrequest_1();
        EV_DETAIL << "Send request_1 " << endl;
    }
    else if(category==T3){//send ack_1
        if(minid!=id){
            head_id=minid;
            sendack_1();
            EV_DETAIL << "Send ack_1 " << endl;
        }
        else{
            head_id=minid;
            EV_WARN << id << " : no head send request " << host_state << endl;
        }
    }
    else if(category==T4){//send hello_3
        sendhello_3();
        EV_DETAIL << "Send hello_3 " << endl;
    }
    else if(category==T5){//send ack_2
        if(geth3){
            sendack_2();
            EV_DETAIL << "Send ack_2 " << endl;
            }
    }
    else if(category==T6){//send ack_3
        if(geth3){
           scheduleTimerA123();
           sendack_3();
           host_state=MEMBER;
           EV_DETAIL << "Send ack_2 " << endl;
           }
    }
    else if(category==R1){//wait ack_1,after request_1
        if(host_state==PREHEAD){
            nodes::iterator iter;
            for(iter=members.begin();iter!=members.end();iter++){
                if(!iter->second.isin){
                    num_member--;
                    members.erase(iter);
                }
            }
            double remain=cal_re_energy();//0.1J 20% energy remain can not be a head
            if(num_member<=(ideal_num+ideal_num/2) && num_member>=(ideal_num-ideal_num+1) && remain > 0.1){//number of member between 0.5 ideal and 1.5 ideal
                host_state=HEAD;
                changetoheadw();//change capacity
                bubble("head");
                scheduleTimerH3();
                simtime_t t=uniform(0.01*h3.dbl(),0.3*h3.dbl());
                scheduleTimerT4(t);
                head_id=id;
                nodes::iterator iter;
                for(iter = members.begin(); iter != members.end(); iter++) {
                    iter->second.isin=false;
                }
                time_head=simTime();
                ////output result
                FILE *stream;
                stream = fopen("res.txt", "a+");
                fprintf(stream, "#1 id%d become a head, number of member : %d \n",id,num_member);//become cluster
                fclose(stream);
            }
            else{
                host_state=IDLE;
                scheduleTimerH1();
                simtime_t t=uniform(0.1*h1.dbl(),0.9*h1.dbl());
                scheduleTimerT0(t);
                members.clear();
                num_neighbour=0;
                geth3=false;
            }
        }
        else{
            EV_WARN << id << " : R1 for error state " << host_state << endl;
        }
    }
    else if(category==WAIT_R1){
        if(host_state==PREMEMBER){
            if(head_id!=id){
                scheduleTimerA123();
                host_state=MEMBER;
                simtime_t t=uniform(0.5*wr1.dbl(),0.8*wr1.dbl());
                scheduleTimerT5(t);
                geth3=false;
            }
            else{
                host_state=IDLE;
                scheduleTimerH1();
                simtime_t t=uniform(0.1*h1.dbl(),0.9*h1.dbl());
                scheduleTimerT0(t);
                members.clear();
                num_neighbour=0;
                geth3=false;
            }
        }
        else{
            EV_WARN << id << " : WAIT REQUEST_1 for error state " << host_state << endl;
        }
    }
    else if(category==H3){
        double remain=cal_re_energy();//0.1J 20% energy remain can not be a head /0.1J
        if(host_state==HEAD){
            nodes::iterator iter;
            for(iter = members.begin(); iter != members.end(); iter++){
                if(iter->second.isin==false){
                    members.erase(iter);
                    num_member--;
                    }
                }
            if(num_member<=0 || remain <= 0.1){
                host_state=IDLE;
                changetomemw();//change capacity
                scheduleTimerH1();
                simtime_t t=uniform(0.1*h1.dbl(),0.9*h1.dbl());
                scheduleTimerT0(t);
                members.clear();
                num_neighbour=0;
                time_head=simTime()-time_head;
                all_headt+=time_head.dbl();
                geth3=false;
                ////output result
                FILE *stream;
                stream = fopen("res.txt", "a+");
                fprintf(stream, "#2 id%d head is over, last : %f s \n",id,time_head.dbl());//cluster over
                fclose(stream);
                }
            else{
                for(iter = members.begin();iter != members.end(); iter++){
                    iter->second.isin=false;
                    }
                scheduleTimerH3();
                simtime_t t=uniform(0.01*h3.dbl(),0.3*h3.dbl());
                scheduleTimerT4(t);
                /////output result
                }
            }
        else{
            EV_WARN << id << " : H3 for error state " << host_state << endl;
            }
    }
    else if(category==A123){
        if(host_state==MEMBER){
            if(!geth3){
                host_state=IDLE;
                cancelEvent(timerA123);
                scheduleTimerH1();
                simtime_t t=uniform(0.1*h1.dbl(),0.9*h1.dbl());
                scheduleTimerT0(t);
                members.clear();
                num_neighbour=0;
                //output result
                FILE *stream;
                stream = fopen("res.txt", "a+");
                fprintf(stream, "#4 id%d leave the cluster from head id: %d over\n",id,head_id);//member leave the cluster
                fclose(stream);
            }
            else{
                simtime_t t=uniform(0.5*a123.dbl(),0.8*a123.dbl());
                scheduleTimerT5(t);
                scheduleTimerA123();
                geth3=false;
            }
        }
        else{
            EV_WARN << id << " : A123 for error state " << host_state << endl;
        }
    }


}

void WCAcluster::processWCAMessage(Packet *packet)
{
    ASSERT(isUp());
    const auto& WCAMsg = packet->peekAtFront<WcaMessage>();

    // check that the packet arrived on the interface we are supposed to serve
    /*int inputInterfaceId = packet->getTag<InterfaceInd>()->getInterfaceId();
    if (inputInterfaceId != ie->getInterfaceId()) {
        EV_WARN << "WCA message arrived on a different interface, dropping\n";
        delete packet;
        return;
        }
    */
    // check the type code
    if (WCAMsg->getMessageType() == Wca_Hello_1) {
            if(host_state==INI || host_state==IDLE){
                int getid=WCAMsg->getClusterID();
                nodes::iterator iter;
                iter=members.find(getid);
                if(iter == members.end()){//no this node in the members
                    EV << "get id: " <<  getid << endl;
                    node mem;
                    mem.id=getid;
                    mem.vel=WCAMsg->getV();
                    mem.loc=WCAMsg->getP();
                    mem.weight=WCAMsg->getWeight();
                    mem.isin=false;
                    members.insert(nodes::value_type(getid, mem));
                    num_neighbour++;
                }
                else{
                    iter->second.vel=WCAMsg->getV();
                    iter->second.loc=WCAMsg->getP();
                    iter->second.weight=WCAMsg->getWeight();
                    iter->second.isin=false;
                }
            }
    }
    else if(WCAMsg->getMessageType() == Wca_Hello_2){
        if(host_state==INI || host_state==IDLE){
                int getid=WCAMsg->getClusterID();
                double getwe=WCAMsg->getWeight();
                if(getwe<minwe){
                    minwe=getwe;
                    minid=getid;
                }
            }
    }
    else if(WCAMsg->getMessageType() == Wca_Hello_3){
        if(host_state==MEMBER){
                int getid=WCAMsg->getClusterID();
                if(head_id==getid){
                    geth3=true;
                }
            }
        else if(host_state==IDLE){
            int getid=WCAMsg->getClusterID();
            head_id=getid;
            geth3=true;
            simtime_t t=uniform(0.01*a123.dbl(),0.2*a123.dbl());
            scheduleTimerT6(t);
            cancelEvent(timerT0);
            cancelEvent(timerT1);
            ////output result
            FILE *stream;
            stream = fopen("res.txt", "a+");
            fprintf(stream, "#3 id%d join the cluster, head id: %d \n",id,head_id);//idle join the cluster
            fclose(stream);
            }
    }
    else if(WCAMsg->getMessageType() == Wca_Request_1){
        if(host_state==PREMEMBER){
            int getid=WCAMsg->getClusterID();
            double getwe=WCAMsg->getWeight();
            if(minwe>getwe){
                minwe=getwe;
                minid=getid;
                }
            }
    }
    else if(WCAMsg->getMessageType() == Wca_Ack_1){
        if(host_state==PREHEAD){
            int aimid=WCAMsg->getSourceId();
            if(aimid==id){
                int getid=WCAMsg->getClusterID();
                nodes::iterator iter;
                iter=members.find(getid);
                if(iter==members.end()){//not in neighbor
                    node mem;
                    mem.id=getid;
                    mem.vel=WCAMsg->getV();
                    mem.loc=WCAMsg->getP();
                    mem.weight=WCAMsg->getWeight();
                    mem.isin=true;
                    members.insert(nodes::value_type(getid, mem));
                    num_member++;
                }
                else{//in neighbor
                    iter->second.vel=WCAMsg->getV();
                    iter->second.loc=WCAMsg->getP();
                    iter->second.weight=WCAMsg->getWeight();
                    iter->second.isin=true;
                    }
                }
            }
    }
    else if(WCAMsg->getMessageType() == Wca_Ack_2){
        if(host_state==HEAD){
            int aimid=WCAMsg->getSourceId();
            if(aimid==id){
                int getid=WCAMsg->getClusterID();
                nodes::iterator iter;
                iter=members.find(getid);
                if(iter != members.end()){
                    iter->second.isin=true;
                    }
                else{
                    EV_WARN << id <<": there is no member "<< getid << endl;
                }
            }
        }
    }
    else if(WCAMsg->getMessageType() == Wca_Ack_3){
        if(host_state==HEAD){
            int aimid=WCAMsg->getSourceId();
            if(aimid==id){
                int getid=WCAMsg->getClusterID();
                nodes::iterator iter;
                iter=members.find(getid);
                if(iter != members.end()){
                    EV_WARN << id <<": there has been member "<< getid << endl;
                        }
                else{
                        node mem;
                        mem.id=getid;
                        mem.vel=WCAMsg->getV();
                        mem.loc=WCAMsg->getP();
                        mem.weight=WCAMsg->getWeight();
                        mem.isin=true;
                        members.insert(nodes::value_type(getid, mem));
                        num_member++;
                        }
                    }
           }
     }
}


void WCAcluster::sendToUdp(Packet *msg, int srcPort, const L3Address& destAddr, int destPort)
{
    EV_INFO << "Sending packet " << msg << endl;
    msg->addTagIfAbsent<InterfaceReq>()->setInterfaceId(ie->getInterfaceId());
    socket1.sendTo(msg, destAddr, destPort);
}

void WCAcluster::openSocket()
{

    socket1.bind(Port);
    socket1.setBroadcast(true);
    EV_INFO << "WCA bound to port " << Port << "." << endl;
}

void WCAcluster::handleStartOperation(LifecycleOperation *operation)
{
    simtime_t start = std::max(startTime, simTime());
    ie = chooseInterface();

    Ipv4InterfaceData *ipv4data = ie->findProtocolData<Ipv4InterfaceData>();
    Ipv4Address ipadress=ipv4data->getIPAddress();
    if (ipv4data == nullptr)
            throw cRuntimeError("interface %s is not configured for IPv4", ie->getFullName());
    id=ipadress.getInt();
    id=id&0xff;
    //id=intuniform(1, 1000000);


    scheduleAt(start, startTimer);
}



void WCAcluster::handleStopOperation(LifecycleOperation *operation)
{




    // TODO: Client should send DHCPRELEASE to the server. However, the correct operation
    // of WCA does not depend on the transmission of DHCPRELEASE messages.

    socket1.close();
    delayActiveOperationFinish(par("stopOperationTimeout"));
}

void WCAcluster::handleCrashOperation(LifecycleOperation *operation)
{
        cancelAndDelete(startTimer);
        cancelAndDelete(timerH1);
        cancelAndDelete(timerH2);
        cancelAndDelete(timerH3);
        cancelAndDelete(timerR1);
        cancelAndDelete(timerWR1);
        cancelAndDelete(timerA123);
        cancelAndDelete(timerT0);
        cancelAndDelete(timerT1);
        cancelAndDelete(timerT2);
        cancelAndDelete(timerT3);
        cancelAndDelete(timerT4);
        cancelAndDelete(timerT5);
        cancelAndDelete(timerT6);


    if (operation->getRootModule() != getContainingNode(this))     // closes socket1 when the application crashed only
        socket1.destroy();         //TODO  in real operating systems, program crash detected by OS and OS closes sockets of crashed programs.
}


void WCAcluster::sendhello_1()
{
    Packet *packet = new Packet("Wca_Hello_1");
    const auto& hello_1 = makeShared<WcaMessage>();
    hello_1->setMessageType(Wca_Hello_1);
    hello_1->setChunkLength(B(80));
    hello_1->setClusterID(id);
    hello_1->setSourceId(id);
    hello_1->setV(selfvel);
    hello_1->setP(selfloc);
    hello_1->setWeight(selfwe);

    packet->insertAtBack(hello_1);
    EV_INFO << "Sending wca hello_1." << endl;
    sendToUdp(packet, Port, Ipv4Address::ALLONES_ADDRESS, Port);
}

void WCAcluster::sendhello_2()
{
    Packet *packet = new Packet("Wca_Hello_2");
    const auto& hello_2 = makeShared<WcaMessage>();
    hello_2->setMessageType(Wca_Hello_2);
    hello_2->setChunkLength(B(80));
    hello_2->setClusterID(id);
    hello_2->setSourceId(id);
    hello_2->setV(selfvel);
    hello_2->setP(selfloc);
    hello_2->setWeight(selfwe);

    packet->insertAtBack(hello_2);
    EV_INFO << "Sending wca hello_2." << endl;
    sendToUdp(packet, Port, Ipv4Address::ALLONES_ADDRESS, Port);
}

void WCAcluster::sendhello_3()
{
    Packet *packet = new Packet("Wca_Hello_3");
    const auto& hello_3 = makeShared<WcaMessage>();
    hello_3->setMessageType(Wca_Hello_3);
    hello_3->setChunkLength(B(80));
    hello_3->setClusterID(id);
    hello_3->setSourceId(id);
    hello_3->setV(selfvel);
    hello_3->setP(selfloc);
    hello_3->setWeight(selfwe);

    packet->insertAtBack(hello_3);
    EV_INFO << "Sending wca hello_3." << endl;
    sendToUdp(packet, Port, Ipv4Address::ALLONES_ADDRESS, Port);
}

void WCAcluster::sendrequest_1()
{
    Packet *packet = new Packet("Wca_Request_1");
    const auto& request_1 = makeShared<WcaMessage>();
    request_1->setMessageType(Wca_Request_1);
    request_1->setChunkLength(B(80));
    request_1->setClusterID(id);
    request_1->setSourceId(id);
    request_1->setV(selfvel);
    request_1->setP(selfloc);
    request_1->setWeight(selfwe);

    packet->insertAtBack(request_1);
    EV_INFO << "Sending wca request_1." << endl;
    sendToUdp(packet, Port, Ipv4Address::ALLONES_ADDRESS, Port);
}

void WCAcluster::sendack_1()
{
    Packet *packet = new Packet("Wca_Ack_1");
    const auto& ack_1 = makeShared<WcaMessage>();
    ack_1->setMessageType(Wca_Ack_1);
    ack_1->setChunkLength(B(80));
    ack_1->setClusterID(id);
    ack_1->setSourceId(head_id);
    ack_1->setV(selfvel);
    ack_1->setP(selfloc);
    ack_1->setWeight(selfwe);

    packet->insertAtBack(ack_1);
    EV_INFO << "Sending wca ack_1." << endl;
    sendToUdp(packet, Port, Ipv4Address::ALLONES_ADDRESS, Port);
}

void WCAcluster::sendack_2()
{
    Packet *packet = new Packet("Wca_Ack_2");
    const auto& ack_2 = makeShared<WcaMessage>();
    ack_2->setMessageType(Wca_Ack_2);
    ack_2->setChunkLength(B(80));
    ack_2->setClusterID(id);
    ack_2->setSourceId(head_id);
    ack_2->setV(selfvel);
    ack_2->setP(selfloc);
    ack_2->setWeight(selfwe);

    packet->insertAtBack(ack_2);
    EV_INFO << "Sending wca ack_2." << endl;
    sendToUdp(packet, Port, Ipv4Address::ALLONES_ADDRESS, Port);
}

void WCAcluster::sendack_3()
{
    Packet *packet = new Packet("Wca_Ack_3");
    const auto& ack_3 = makeShared<WcaMessage>();
    ack_3->setMessageType(Wca_Ack_3);
    ack_3->setChunkLength(B(80));
    ack_3->setClusterID(id);
    ack_3->setSourceId(head_id);
    ack_3->setV(selfvel);
    ack_3->setP(selfloc);
    ack_3->setWeight(selfwe);

    packet->insertAtBack(ack_3);
    EV_INFO << "Sending wca ack_3." << endl;
    sendToUdp(packet, Port, Ipv4Address::ALLONES_ADDRESS, Port);
}

void WCAcluster::scheduleTimerT0(simtime_t t)
{
    cancelEvent(timerT0);
    scheduleAt(simTime() + t, timerT0);
}

void WCAcluster::scheduleTimerT1(simtime_t t)
{
    cancelEvent(timerT1);
    scheduleAt(simTime() + t, timerT1);
}

void WCAcluster::scheduleTimerT2(simtime_t t)
{
    cancelEvent(timerT2);
    scheduleAt(simTime() + t, timerT2);
}

void WCAcluster::scheduleTimerT3(simtime_t t)
{
    cancelEvent(timerT3);
    scheduleAt(simTime() + t, timerT3);
}

void WCAcluster::scheduleTimerT4(simtime_t t)
{
    cancelEvent(timerT4);
    scheduleAt(simTime() + t, timerT4);
}

void WCAcluster::scheduleTimerT5(simtime_t t)
{
    cancelEvent(timerT5);
    scheduleAt(simTime() + t, timerT5);
}

void WCAcluster::scheduleTimerT6(simtime_t t)
{
    cancelEvent(timerT6);
    scheduleAt(simTime() + t, timerT6);
}

void WCAcluster::scheduleTimerH1()
{
    cancelEvent(timerH1);
    scheduleAt(simTime() + h1, timerH1);
}

void WCAcluster::scheduleTimerH2()
{
    cancelEvent(timerH2);
    scheduleAt(simTime() + h2, timerH2);
}

void WCAcluster::scheduleTimerH3()
{
    cancelEvent(timerH3);
    scheduleAt(simTime() + h3, timerH3);
}

void WCAcluster::scheduleTimerR1()
{
    cancelEvent(timerR1);
    scheduleAt(simTime() + r1, timerR1);
}

void WCAcluster::scheduleTimerWR1()
{
    cancelEvent(timerWR1);
    scheduleAt(simTime() + wr1, timerWR1);
}

void WCAcluster::scheduleTimerA123()
{
    cancelEvent(timerA123);
    scheduleAt(simTime() + a123, timerA123);
}

void WCAcluster::cal_weight()
{
    double degree=cal_degree_diff();
    double remain=cal_re_energy();
    if(remain>0.1){
        selfwe=c1*degree+c2*cal_simi_velocity()+c3*cal_ave_distance()+10*c4*remain;
    }
    else{
        selfwe=DBL_MAX;
    }
}

double WCAcluster::cal_simi_velocity()
{
    getvel();
    double v=0;
    double vel;
    nodes::iterator iter;
    for(iter=members.begin();iter!=members.end();iter++){
        vel=sqrt(pow(fabs(iter->second.vel.x-selfvel.x),2)+pow(fabs(iter->second.vel.y-selfvel.y),2)+pow(fabs(iter->second.vel.z-selfvel.z),2));
        v+=vel;
    }
    if(num_neighbour==0){
            return 0;
        }
    v/=num_neighbour;
    //v=sqrt(pow(selfvel.x,2)+pow(selfvel.y,2)+pow(selfvel.z,2));//the last algorithm
    return v;
}

int WCAcluster::cal_degree_diff()
{
    return abs(num_neighbour-ideal_num);
}

double WCAcluster::cal_ave_distance()
{
    getloc();
    double d=0;
    double dist;
    nodes::iterator iter;
    for(iter=members.begin();iter!=members.end();iter++){
        dist=sqrt(pow(fabs(iter->second.loc.x-selfloc.x),2)+pow(fabs(iter->second.loc.y-selfloc.y),2)+pow(fabs(iter->second.loc.z-selfloc.z),2));
        d+=dist;
    }
    if(num_neighbour==0){
        return 0;
        }
    d/=num_neighbour;
    return d;
}

double WCAcluster::cal_re_energy()
{
    cModule *host = getContainingNode(this);
    power::IEpEnergyStorage *energy = check_and_cast<power::IEpEnergyStorage *>(host->getSubmodule("energyStorage"));
    auto remain = energy->getResidualEnergyCapacity();
    return remain.get();
    //return all_headt;//the last algorithm
}

void WCAcluster::getvel()
{
    cModule *host = getContainingNode(this);
    IMobility *mobility = check_and_cast<IMobility *>(host->getSubmodule("mobility"));
    auto _velocity = mobility->getCurrentVelocity();
    selfvel.x=_velocity.x;
    selfvel.y=_velocity.y;
    selfvel.z=_velocity.z;
}

void WCAcluster::getloc()
{
    cModule *host = getContainingNode(this);
    IMobility *mobility = check_and_cast<IMobility *>(host->getSubmodule("mobility"));
    auto position = mobility->getCurrentPosition();
    selfloc.x=position.x;
    selfloc.y=position.y;
    selfloc.z=position.z;
}

void WCAcluster::changetoheadw()
{
    cModule *host = getContainingNode(this);
 //   power::AlternatingEpEnergyGenerator *energy = check_and_cast<power::AlternatingEpEnergyGenerator *>(host->getSubmodule("energyGenerator"));
//    energy->setsPowerGeneration(headw);
}

void WCAcluster::changetomemw()
{
    cModule *host = getContainingNode(this);
 //   power::AlternatingEpEnergyGenerator *energy = check_and_cast<power::AlternatingEpEnergyGenerator *>(host->getSubmodule("energyGenerator"));
//    energy->setsPowerGeneration(memw);
}

} // namespace inet


