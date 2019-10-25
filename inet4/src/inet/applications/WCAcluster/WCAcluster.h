/*
 * WCAcluster.h
 *
 *  Created on: 2019年4月16日
 *      Author: hiro
 */

#ifndef INET_APPLICATIONS_WCACLUSTER_WCACLUSTER_H_
#define INET_APPLICATIONS_WCACLUSTER_WCACLUSTER_H_

#include <map>
#include "inet/common/INETDefs.h"

#include "inet/applications/base/ApplicationBase.h"
#include "inet/applications/dhcp/DhcpMessage_m.h"
#include "inet/networklayer/arp/ipv4/Arp.h"
#include "inet/networklayer/common/InterfaceTable.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "inet/applications/WCAcluster/WcaMessage_m.h"


namespace inet {

/**
 * Implements a DHCP server. See NED file for more details.
 */
class INET_API WCAcluster : public ApplicationBase, public cListener, public UdpSocket::ICallback
{
  protected:
    enum HostState{
      IDLE,INI,PREHEAD,PREMEMBER,HEAD,MEMBER
    };
    
    // WCA timer types
     enum TimerType {
        START_WCA,WAIT_INIT,WAIT_WEIGHT,WAIT_R1,R1,A123,H3,T0,T1,T2,T3,T4,T5,T6
     };

    struct node{
        int id;
        double weight;
        velocity vel;
        location loc;
        bool isin;
    };

    typedef std::map<int, node> nodes;
    nodes members;
    //weight para
    const int ideal_num = 5;
    const double c1 = 0.5;
    const double c2 = 0.25;
    const double c3 = 0.2;
    const double c4 = 0.05;

    int num_neighbour;//the number of the neighbour node

    int num_member;//the number of the members of the head

    int head_id;//the id of head
    int id;//self id

    velocity selfvel;
    location selfloc;
    double selfwe;
    //min weight and its id
    double minwe;
    int minid;

    bool geth3=false;

    HostState host_state;

    int Port = -1;
    int HeadPort = -1;
    int MemberPort = -1;
    
    /* Set by management, see WCAcluster NED file. */
    
    cMessage *startTimer = nullptr;    //start
    cMessage* timerH1;      //wait init,after hello_1
    cMessage* timerH2;      //wait weight,after hello_2
    cMessage* timerH3;      //wait ack_2,after hello_3
    cMessage* timerR1;  //wait ack_1 3 or ack_3,after request_1
    cMessage* timerWR1; //wait request_1
    cMessage* timerA123;//wait hello_3,after ack123
    cMessage* timerT0;  //temporary timer send hello_1
    cMessage* timerT1;  //temporary timer send hello_2
    cMessage* timerT2;  //temporary timer send request_1
    cMessage* timerT3;  //temporary timer send ack_1
    cMessage* timerT4;  //temporary timer send hello_3
    cMessage* timerT5;  //temporary timer send ack_2
    cMessage* timerT6;  //temporary timer send ack_3

    simtime_t h1=0.5;//  2
    simtime_t h2=0.5;//  2
    simtime_t h3=0.5;//  2
    simtime_t r1=0.5;//  2
    simtime_t wr1=0.5;// 2
    simtime_t a123=0.5;//2
    simtime_t time_head=0;

    double all_headt=0;

    double headw=-0.004;//the capacity of head //units:W
    double memw=-0.002;//the capacity of member of idle//units:W

    UdpSocket socket1;    // UDP socket for initialization phase
    UdpSocket socket2;    //UDP socket for cluster head
    UdpSocket socket3;    //UDP socket for cluster member
    simtime_t startTime;    // application start time

    cModule *host = nullptr;    // containing host module (@networkNode)
 // interface to configure

  InterfaceEntry *ie = nullptr;
    
    //analysis

  protected:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;
    virtual void finish() override;
    /*
     * Opens a UDP socket for client-server communication.
     */
    virtual void openSocket();

     virtual InterfaceEntry *chooseInterface();
    /*
     * Implements the server's state machine.
     */
    virtual void processWCAMessage(Packet *packet);

    virtual void handleTimer(cMessage *msg);
    
    /*
         * Performs UDP transmission.
    */
    virtual void sendToUdp(Packet *msg, int srcPort, const L3Address& destAddr, int destPort);

    //UdpSocket::ICallback methods
    virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;
    virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
    virtual void socketClosed(UdpSocket *socket) override;

    /*
     * Signal handler for cObject, override cListener function.
     */
//    virtual void receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject *details) override;



    // Lifecycle methods
    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;

    //calculate weights
    virtual void cal_weight();//calculate node's weight
    virtual double cal_simi_velocity();//calculate similar velocity
    virtual int cal_degree_diff();//calculate degree difference
    virtual double cal_ave_distance();//calculate average distance
    virtual double cal_re_energy();//calculate remain energy

    virtual void changetoheadw();//change capacity to head mode
    virtual void changetomemw();//change capacity to member or idle mode

    //get v and l
    virtual void getvel();//get self velocity
    virtual void getloc();//get self location

    //send message
    virtual void sendhello_1();
    virtual void sendhello_2();
    virtual void sendhello_3();
    virtual void sendrequest_1();
    virtual void sendack_1();
    virtual void sendack_2();
    virtual void sendack_3();

    //schedule timer
    virtual void scheduleTimerH1();
    virtual void scheduleTimerH2();
    virtual void scheduleTimerH3();
    virtual void scheduleTimerR1();
    virtual void scheduleTimerWR1();
    virtual void scheduleTimerA123();
    virtual void scheduleTimerT0(simtime_t t);
    virtual void scheduleTimerT1(simtime_t t);
    virtual void scheduleTimerT2(simtime_t t);
    virtual void scheduleTimerT3(simtime_t t);
    virtual void scheduleTimerT4(simtime_t t);
    virtual void scheduleTimerT5(simtime_t t);
    virtual void scheduleTimerT6(simtime_t t);

  public:
    WCAcluster();
    virtual ~WCAcluster();
};

} // namespace inet


#endif /* INET_APPLICATIONS_WCACLUSTER_WCACLUSTER_H_ */
