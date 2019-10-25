//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __INET_FAP_H_
#define __INET_FAP_H_

#include <map>
#include<vector>
#include<memory>
#include<queue>
#include<fstream>
#include "inet/common/INETDefs.h"


#include "BloomFilter.h"
#include"./hash/MurmurHash3.h"
#include "inet/applications/base/ApplicationBase.h"
#include "inet/networklayer/common/InterfaceTable.h"
#include "inet/linklayer/common/MacAddress.h"
#include "inet/networklayer/arp/ipv4/Arp.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "inet/networklayer/ipv4/Ipv4RoutingTable.h"
#include"inet/networklayer/contract/ipv4/Ipv4Address.h"
#include"FAPmessage_m.h"





/**
 * TODO - Generated class
 */

namespace inet{
class INET_API Fap : public ApplicationBase, public cListener, public UdpSocket::ICallback
{
  protected:

    class IpAddrPool{
      public:
        Ipv4Address prefix;
        int prefixLength;

        IpAddrPool():prefix("192.168.0.0"),prefixLength(24){}
        IpAddrPool(Ipv4Address p, int l):prefix(p),prefixLength(l){}
    };

  private:
    enum hostState{IDLE,WAIT,INI,STEADY,MERGE,DEPART};
    IpAddrPool ADDRPool;
    int num;//for clear record file
    int Port = -1;

    Ipv4Address selectedAddr;

    unsigned int xID;
    //record received ip addresses within AREQ
    std::map<uint32_t,unsigned int> IpAddrRecord;
    //record how many hosts in this group
    unsigned nodeCounter;

    UdpSocket socket;    // UDP socket for client-server communication
    // UdpSocket socketOut;   //UDP socket for client-server communication
    simtime_t startTime;    // application start time
    MacAddress macAddress;    // client's MAC address
    cModule *host = nullptr;    // containing host module (@networkNode)
    InterfaceEntry *ie = nullptr;    // interface to configure
    IIpv4RoutingTable *irt = nullptr;    // routing table to update
    bool isOperational = false;


    BloomFilter<uint32_t> filter; //filter of Fap
    // std::deque< std::shared_ptr< BloomFilter<uint32_t>> > OldFilters;
    // std::deque< std::shared_ptr< BloomFilter<uint32_t>> > NewFilters;
    hostState HostState ;
    Ipv4Route *route = nullptr;    // last added route
    
    bool WaitLock = false;//to lock and waiting for only one AF
    bool MergeLock = false;
    bool FilterLock = false;

    const simtime_t TL = 1.0; //MAX.time listening to the medium
    const simtime_t TP = 3.0; //Partition merging min.interval
    const simtime_t TW = 1.2; //AREQ waiting time
    const simtime_t TR = 0.3; //Message replication interval
    const simtime_t TH = 1.0; //Hello time
    const simtime_t TC = 0.3; //Waiting time before changing address
    const simtime_t TS = 0.5; //Generated-filter-signature storage time
    const simtime_t ts = 3.0; //Received-filter-signature storage time
    const simtime_t TM = 2.5; //Merge change to Steady
    const simtime_t TF = 0.1; //Max time waiting for a filter
    simtime_t AreqTime;//generate a random interval for repetitive sending AREQ

    //for record
    simtime_t MergeTime;

    cMessage *Random = nullptr;
    cMessage *timerTL = nullptr;
    cMessage *timerTP = nullptr;
    cMessage *timerTW = nullptr;
    // cMessage *timerTW2 = nullptr;
    cMessage *timerTH = nullptr;
    cMessage *timerTC = nullptr;
    // cMessage *timerTS = nullptr;
    cMessage *timerts = nullptr;
    cMessage *timerTM = nullptr;
    cMessage *timerTF = nullptr;
    cMessage *startTimer = nullptr;
    cMessage *AreqTimer=nullptr;
    cMessage *timerTS =nullptr;

    FAPhead *buffer = nullptr;
    int NF = 3;//Transmission times of a AREQ
    unsigned int NT = 2;//Neighbor reply threshold

  protected:
    virtual void initialize(int stage)override;
    virtual void finish() override;
//    virtual void handleMessage(cMessage *msg) override;
    virtual void handleMessageWhenUp(cMessage *msg) override;

    virtual void scheduleTL();
    virtual void scheduleTP();
    virtual void scheduleTW();
    virtual void scheduleTH();
    virtual void scheduleTC();
    virtual void schedulets();
    virtual void scheduleTM();
    virtual void scheduleTF();
    virtual void scheduleRandom();

    static const char *getStateName(hostState state);

    /*
         * Opens a UDP socket for client-server communication.
         */
    virtual void openSocket();
    
    //for WAIT--->INI
    virtual void InitialConfigure();
    //for MERGE--->INI
    virtual void InitialConfigure2();
    /*
    *generate a random ip address
    */
    virtual uint32_t generateIPADDR();

    virtual void SetIpAddr(uint32_t addr);
    /*
    *insert ip address
    */
    virtual void insertADDR(uint32_t addr);

    virtual void prossesFAPmessage(Packet *packet);

    /*check whether IP address has been inserted into filer*/
    virtual bool checkRepetition(uint32_t addr);

    virtual bool checkIdentical(std::deque<std::shared_ptr< BloomFilter<uint32_t> >> Q,unsigned int Sig);

    virtual void ChooseIpAddr();

    virtual void UpdateFilter(int m, int k, std::string f);

    virtual bool CheckFilterSignature(unsigned int S);

    virtual void generateXID();
    
    /*
     * Performs state changes and requests according to the timer expiration events
     * (e. g. retransmits DHCPREQUEST after the WAIT_ACK timeout expires).
     */
    virtual void handleTimer(cMessage *msg);

    /*
     * Signal handler for cObject, override cListener function.
     */
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject *details) override;

    /*
     * Performs UDP transmission.
     */
    virtual void sendToUdp(Packet *msg, int srcPort, const L3Address& destAddr, int destPort);

    /*
     * send hello message
     */
    virtual void sendHello();
    /*
     * send AREQ message V1
     */
    virtual void sendAREQ(bool R);
    /*
    * send AREQ message V2
    */
    virtual void sendAREQ(bool R, Ipv4Address seletct, unsigned int XID);
    /*
    *send AF message
    */
    virtual void sendAF(Ipv4Address dest,bool I,bool R);
    /*
    *send Partition message
    */ 
    virtual void sendPartition(bool m, int seq, const std::string value, int filter_m, int filter_k, unsigned int filter_sig, unsigned counter);
    
    virtual unsigned int getFilterSignature();
    
    virtual InterfaceEntry * chooseInterface();



    virtual void ChangeToIni();

    //UdpSocket::ICallback methods
    virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;
    virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
    virtual void socketClosed(UdpSocket *socket) override;

    // Lifecycle methods
    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;


    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
  public:
    std::ofstream outfile;
    void merge_record();
    Fap() ;
    virtual ~Fap();
};
}
#endif
