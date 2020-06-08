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
        
        // 指定发送请求的速率，useFreq = true时使用sendFreq参数，useFreq = false时使用sendInterval参数
        bool useFreq;
        volatile double sendInterval = default(1);    // 发送get的间隔，单位/s 	
        volatile int sendFreq = default(1);		// 发送get的频率，单位packet/s
        
        double startTime @unit(s) = default(3s); // send first request
        double stopTime @unit(s) = default(-1s); // time to finish sending, negative values mean forever
        
        @display("i=block/app");
        @lifecycleSupport;
        double stopOperationExtraTime @unit(s) = default(-1s);    // extra time after lifecycle stop operation finished
        double stopOperationTimeout @unit(s) = default(2s);    // timeout value for lifecycle stop operation
        
        bool congestionControlEnabled = default(true);	// 是否开启拥塞控制
        int initialWindowSize = default(10);				// 拥塞控制初始窗口
        int maxRexmitLimit = default(12);				// 最大重传次数，0则不重传
        
        // 调节拥塞窗口的参数
        int n0 = default(5);
        double k0 = default(0.5);
        
		// TODO 增加窗口调节算法后，需要在相应位置发射下面的信号
		// 增加新的信号后，也需要在ini文件中修改
		@signal[rto](type=simtime_t);
		@signal[srtt](type=simtime_t);
		@signal[rttvar](type=simtime_t);
		@signal[window](type=double);
		@signal[effectiveWindow](type=long);
		@signal[rexmit](type=long);		// type is indifferent, count # of retransmissions
		@signal[dataRcvd](type=long);	// type is indifferent, count # of received Data
		@signal[getSent](type=long);	// type is indifferent, count # of sent Get
		@signal[congestionLevel](type=long);
		@signal[timeout](type=long);
		@signal[maxRexmit](type=long);
		@signal[dataRcvdTime](type=simtime_t);
		
		@statistic[rto](title="retransmission timeout"; unit=s; record=vector);
		@statistic[srtt](title="smoothed round-trip time"; unit=s; record=vector);
		@statistic[rttvar](title="rtt variance"; unit=s; record=vector);
		@statistic[window](title="congestion window";record=vector);
		@statistic[effectiveWindow](title="effective window"; record=vector);
		@statistic[rexmit](source=count(rexmit);title="retransmission"; record=last,vector);
		@statistic[dataRcvd](source=count(dataRcvd);title="data received"; record=last,vector);
		@statistic[getSent](source=count(getSent);title="get sent";record=last,vector);
		@statistic[congestionLevel](title="Congestion Level"; record=vector);
		@statistic[timeout](source=count(timeout); title="Timeout"; record=vector);
		@statistic[maxRexmit](source=count(maxRexmit); title="Number of retransmission that exceeds maximum value"; record=last, vector);
		@statistic[dataRcvdTime](source=dataRcvdTime; title="Last time received time"; record=last);
	gates:
	    input in;
	    input socketIn @labels(ITransportPacket/up);
	    output socketOut @labels(ITransportPacket/down);
}
