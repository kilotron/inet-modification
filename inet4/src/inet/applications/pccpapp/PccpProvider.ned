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

package inet.applications.pccpapp;
import inet.applications.contract.IApp;


simple PccpProvider like IApp
{
    parameters:
        int port = default(8709);
        int pktLen = default(1000);
        int pktNum = default(100000);
        double startTime @unit(s) = default(3s); // send first ping at startTime
        @display("i=block/app");
        @lifecycleSupport;
        double stopOperationExtraTime @unit(s) = default(-1s);    // extra time after lifecycle stop operation finished
        double stopOperationTimeout @unit(s) = default(2s);    // timeout value for lifecycle stop operation

    gates:
        input in;
        input socketIn @labels(ITransportPacket/up);
        output socketOut @labels(ITransportPacket/down);
}
