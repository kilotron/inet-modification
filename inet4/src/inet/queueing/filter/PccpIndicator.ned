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

package inet.queueing.filter;

import inet.queueing.base.PacketFilterBase;

// This module add PccpDataQueueInd to packet.

simple PccpIndicator extends PacketFilterBase
{
    parameters:
//		 TODO 下面这些参数应该在实验完成后选出一个合适的值，设为默认值，
        double wq = default(0.02); // weight of the current queue length in the averaged queue length
        double g = default(0.2);	// weight of interest queue
        double CI_FREE = default(0.05);
        double CI_BUSY = default(0.3);
        double CI_CONG = default(0.6);
        double p0 = default(0.5);	// probability of CL=BUSY_1 when CI = CI_BUSY
        bool dataQueueOnly = default(false);
        @class(PccpIndicator); 
        
        @signal[dataQueueLength](type=long);
        @signal[pitLength](type=long);
        @signal[aveDataQueueLength](type=double);
        @signal[avePitLength](type=double);
        @signal[congestionIndex](type=double);
        
        @statistic[dataQueueLength](title="Data Queue Length"; record=max,vector);
        @statistic[pitLength](title="PIT Length"; record=max,vector);
        @statistic[aveDataQueueLength](title="Average Data Length"; record=max,vector);
        @statistic[avePitLength](title="Average PIT Length"; record=max,vector);
        @statistic[congestionIndex](title="Congestion Index"; record=max,vector);
}