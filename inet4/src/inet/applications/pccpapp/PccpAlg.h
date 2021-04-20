/*
 * PccpAlg.h
 *
 *  Created on: Apr 11, 2020
 *      Author: kilotron
 */

#ifndef INET_APPLICATIONS_PCCPAPP_PCCPALG_H_
#define INET_APPLICATIONS_PCCPAPP_PCCPALG_H_

#include "inet/common/INETDefs.h"
#include "inet/applications/pccpapp/PccpStateVariables.h"
#include "inet/applications/pccpapp/PccpSendQueue.h"
#include "inet/applications/pccpapp/PccpApp.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace inet {

class PccpApp;

class PccpRecorder
{
private:
    int numMaxRexmit;
    int numRexmit;
    int numDataRcvd;
    bool started;
    double startRcvTime; // in seconds
    double lastRcvTime; // in seconds

    // delay < 5ms
    int numDataRcvdLessDelayed;
    bool startedLessDelayed;
    double startRcvTimeLessDelayed; // in seconds
    double lastRcvTimeLessDelayed; // in seconds

    std::ostringstream bandwidth_result; // each line: time bandwidth
    std::ostringstream data_rcvd;
    std::ostringstream rexmit_result;
    std::ostringstream packet_loss_result;
    std::ostringstream delay_result;
    std::ostringstream data_rcvd_less_delayed; // data received, delay of which is less than 100ms

public:
    std::string pccpinfo; // information will be written to file
    PccpRecorder() {
        numMaxRexmit = 0;
        numRexmit = 0;
        numDataRcvd = 0;
        started = false;
        startRcvTime = 0.0;
        lastRcvTime = 0.0;

        numDataRcvdLessDelayed = 0;
        startedLessDelayed = false;
        startRcvTimeLessDelayed = 0.0;
        lastRcvTimeLessDelayed = 0.0;

        bandwidth_result << "Bandwidth Result" << std::endl;
        data_rcvd << "DataRcvd Result" << std::endl;
        rexmit_result << "Rexmit Result" << std::endl;
        packet_loss_result << "Packet Loss Result" << std::endl;
        delay_result << "Delay Result" << std::endl;
        data_rcvd_less_delayed << "DataRcvdLessDelayed Result" << std::endl;
    };
    ~PccpRecorder() {
        outputToFile();
    };
    void increase(std::string name) {
        double time = simTime().dbl();
        if (name == "numRexmit") {
            numRexmit++;
            rexmit_result << time - 3 << std::endl;
        } else if (name == "numMaxRexmit") {
            numMaxRexmit++;
            packet_loss_result << time - 3 << std::endl;
        } else if (name == "numDataRcvd") {
            if (!started) {
                started = true;
                startRcvTime = time;
            }
            numDataRcvd++;
            data_rcvd << time - 3 << std::endl;
            lastRcvTime = time;
        } else if (name == "numDataRcvdLessDelayed") {
            if (!startedLessDelayed) {
                startedLessDelayed = true;
                startRcvTimeLessDelayed = time;
            }
            numDataRcvdLessDelayed++;
            data_rcvd_less_delayed << time - 3 << std::endl;
            lastRcvTimeLessDelayed = time;
        } else {
            std::cout << "PccpAlg.h: recorder increase error" << std::endl;
        }
    }
    void addDelay(double d) {
        delay_result << d << std::endl;
    }
    void outputToFile() {
        double throughput = (numDataRcvd * 1000) / (lastRcvTime - startRcvTime); // KB/s
        throughput = throughput / 1e6 * 8; // Mbps
        double throughput_less_delayed = (numDataRcvdLessDelayed * 1000) / (lastRcvTimeLessDelayed - startRcvTimeLessDelayed);
        throughput_less_delayed = throughput_less_delayed / 1e6 * 8;
        std::ostringstream oss;
        oss << pccpinfo << endl;
        oss << "numMaxRexmit " << numMaxRexmit << endl;
        oss << "numRexmit " << numRexmit << endl;
        oss << "numDataRcvd " << numDataRcvd << endl;
        oss << "throughput " << throughput << endl;
        oss << "throughput_less_delayed " << throughput_less_delayed << endl;

        bandwidth_result << "END" << std::endl;
        data_rcvd << "END" << std::endl;
        rexmit_result << "END" << std::endl;
        packet_loss_result << "END" << std::endl;
        delay_result << "END" << std::endl;
        data_rcvd_less_delayed << "END" << std::endl;
        oss << data_rcvd.str();
        oss << rexmit_result.str();
        oss << packet_loss_result.str();
        oss << delay_result.str();
        oss << data_rcvd_less_delayed.str();

        std::string s = oss.str();
        std::ofstream outfile;
        outfile.open("/home/zeusnet/pccp/xtoponoburstresultfreq=.txt", std::ios::app);
        outfile << s;
        outfile.close();
    };

};

/**
 * Includes basic algorithms: retransmission, congestion window adjustment.
 */
class INET_API PccpAlg : public cObject, public ColorSocket::ICallback
{
    friend class PccpApp;

protected:
    PccpStateVariables state;
    PccpSendQueue sendQueue;
    PccpApp *pccpApp;    // app module, set by the app
    PccpRecorder recorder;

private:
    /**
     * Send all of the requests in sendQueue if possible.
     */
    void sendRequestsToSocket();

    /**
     * Schedules retransmission timer and starts round-trip time measurement.
     */
    void requestSent(const SID& sid, bool isRexmit);

    /**
     * Retransmit GET, reset timer and increase rexmit count.
     */
    void retransmitRequest(const SID& sid);

    /**
     * Finishes round-trip time measurement, cancel retransmission timer and adjust
     * the congestion window.
     */
    void dataReceived(const SID& sid, Packet *packet);

    /**
     * Update state vars with new measured RTT value. Passing two simtime_t's
     * will allow rttMeasurementComplete() to do calculations.
     */
    void rttMeasurementComplete(simtime_t timeSent, simtime_t timeReceived);

public:

    PccpAlg();
    ~PccpAlg();

    // interface between PccpApp and PccpAlg
    void sendRequest(const SID &sid, int localPort, double sendInterval);

    /**
     * Performs retransmission and increases RTO
     */
    void processRexmitTimer(cMessage *timer);

    /**
     * Initialize window size and congestionControlEnabled.
     * This method should be called by pccpApp.
     */
    void initializeState();

    //INetworkSocket::ICallback:
    virtual void socketDataArrived(ColorSocket *socket, Packet *packet) override;
    virtual void socketClosed(ColorSocket *socket) override;

    void setInfo(std::string algoinfo, int sizeinfo);
};

} // namespace inet

#endif /* INET_APPLICATIONS_PCCPAPP_PCCPALG_H_ */
