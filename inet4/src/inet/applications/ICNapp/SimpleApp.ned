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

package inet.applications.ICNapp;

import inet.applications.contract.IApp;

//
simple SimpleApp like IApp
{
    parameters:
        int port = default(8709);
        int destAddr = default(0); // destination address(es), separated by spaces, "*" means all IPv4/IPv6 interfaces in entire simulation
        int requestNum = default(100000);
        string RSTpath = default("");
//        string srcAddr = default(""); // source address (useful with multi-homing)
        // int packetSize @unit(B) = default(56B); // of ping payload, in bytes
        volatile double sendInterval = default(1); // time to wait between pings (can be random)

        double startTime @unit(s) = default(3s); // send first ping at startTime
        double stopTime @unit(s) = default(-1s); // time to finish sending, negative values mean forever
 
        bool continuous = default(false);  // whether to continuously ping the destinations in a round-robin fashion
//        bool printPing = default(false); // log to stdout
//        string crcMode @enum("declared", "computed") = default("declared");
//        string networkProtocol = default(""); // uses specified network protocol for communication; uses the address-specified default network protocol when the parameter is empty
        @display("i=block/app");
        @lifecycleSupport;
        double stopOperationExtraTime @unit(s) = default(-1s);    // extra time after lifecycle stop operation finished
        double stopOperationTimeout @unit(s) = default(2s);    // timeout value for lifecycle stop operation
//        @signal[rtt](type=simtime_t);
//        @signal[numLost](type=long);
//        @signal[numOutOfOrderArrivals](type=long);
//        @signal[pingTxSeq](type=long);
//        @signal[pingRxSeq](type=long);

    gates:
        input pingIn;
        input socketIn @labels(ITransportPacket/up);
        output socketOut @labels(ITransportPacket/down);
}