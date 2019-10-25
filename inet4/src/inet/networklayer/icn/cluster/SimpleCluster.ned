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

package inet.networklayer.icn.cluster;

simple SimpleCluster like ICluster
{
    parameters:
        string interfaceTableModule="";
        double waitingTime @unit(s) = default(0.5s);
        double startTime @unit(s) = default(0s);
        double collectTime @unit(s) = default(0.5s);
        double interval @unit(s) = default(0.1s);
        double hello @unit(s) = default(0.5s);
        string recordFile = default("clusterRecord.txt");
        string iniFile = default("ini.txt");
        
        int retry;
        @display("i=block/layer");
        
        @lifecycleSupport;
        double stopOperationExtraTime @unit(s) = default(-1s);    // extra time after lifecycle stop operation finished
        double stopOperationTimeout @unit(s) = default(2s);    // timeout value for lifecycle stop operation
    gates:    	
        input ifIn @labels(clusterPacket,Ieee802Ctrl);
        output ifOut @labels(clusterPacket,Ieee802Ctrl);
}