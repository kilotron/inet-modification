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

simple PccpApp like IApp
{
    parameters:
        int port = default(8709);
        int destAddr = default(0);
        int requestNum = default(100000);
        string RSTpath = default("");
        volatile double sendInterval = default(1);
        
        double startTime @unit(s) = default(3s); // send first request
        double stopTime @unit(s) = default(-1s); // time to finish sending, negative values mean forever
        
        @display("i=block/app");
        @lifecycleSupport;
        double stopOperationExtraTime @unit(s) = default(-1s);    // extra time after lifecycle stop operation finished
        double stopOperationTimeout @unit(s) = default(2s);    // timeout value for lifecycle stop operation
		
		// TODO 增加窗口调节算法后，需要在相应位置发射下面的信号
		// 增加新的信号后，也需要在ini文件中修改
		@signal[rto](type=simtime_t);
		@signal[srtt](type=simtime_t);
		@signal[window](type=long);
		@signal[effectiveWindow](type=long);
		@signal[rexmit](type=long);		// type is indifferent, count # of retransmissions
		@signal[dataRcvd](type=long);	// type is indifferent, count # of received Data
		@signal[getSent](type=long);	// type is indifferent, count # of sent Get
		
		@statistic[rto](title="retransmission timeout"; unit=s; record=vector);
		@statistic[srtt](title="smoothed round-trip time"; unit=s; record=vector);
		@statistic[window](title="congestion window";record=vector);
		@statistic[effectiveWindow](title="effective window"; record=vector);
		@statistic[rexmit](source=count(rexmit);title="retransmission"; record=last,vector);
		@statistic[dataRcvd](source=count(dataRcvd);title="data received"; record=last,vector);
		@statistic[getSent](source=count(getSent);title="get sent";record=last,vector);
	gates:
	    input in;
	    input socketIn @labels(ITransportPacket/up);
	    output socketOut @labels(ITransportPacket/down);
}
