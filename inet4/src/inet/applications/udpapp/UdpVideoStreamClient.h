//
// Copyright (C) 2005 Andras Varga
//
// Based on the video streaming app of the similar name by Johnny Lai.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_UDPVIDEOSTREAMCLI_H
#define __INET_UDPVIDEOSTREAMCLI_H

#include "inet/common/INETDefs.h"

#include "inet/applications/base/ApplicationBase.h"
#include "inet/common/packet/Packet.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"

namespace inet {

/**
 * A "Realtime" VideoStream client application.
 *
 * Basic video stream application. Clients connect to server and get a stream of
 * video back.
 */
class INET_API UdpVideoStreamClient : public ApplicationBase, public UdpSocket::ICallback
{
  public:
    struct SimRecorder
    {
        UdpVideoStreamClient *owner;

        int multiConsumer;

        int index;
        std::vector<double> delayArray;

        B throughput = B(0);
        int GetSendNum = 0;
        int GetRecvNum = 0;

        int DataSendNum = 0;
        int DataRecvNum =0;
        simtime_t delay = 0;
        simtime_t last = 0;

        void ConsumerPrint(std::ostream &os);

        void ProviderPrint(std::ostream &os);
    };
  protected:

    SimRecorder Recorder;
    // state
    UdpSocket socket;
    cMessage *selfMsg = nullptr;
    cMessage *timer = nullptr;

  protected:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void finish() override;
    virtual void handleMessageWhenUp(cMessage *msg) override;

    virtual void requestStream();
    virtual void receiveStream(Packet *msg);

    // ApplicationBase:
    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;

    virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;
    virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
    virtual void socketClosed(UdpSocket *socket) override;

  public:
    double startTime;
    double sendInterval;
    double pktNum;
    std::string path;
    UdpVideoStreamClient() { }
    virtual ~UdpVideoStreamClient() { cancelAndDelete(selfMsg); }
};

} // namespace inet

#endif // ifndef __INET_UDPVIDEOSTREAMCLI_H

