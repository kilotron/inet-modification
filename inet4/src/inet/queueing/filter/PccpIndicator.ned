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
        double wq = default(0.002); // weight of the current queue length in the averaged queue length
        double g = default(0.5);	// weight of data queue occupancy
        double CI_FREE = default(0.6);
        double CI_BUSY = default(0.8);
        double CI_CONG = default(0.9);
        double p0 = default(0.5);	// probability of CL=BUSY_1 when CI = CI_BUSY
        @class(PccpIndicator);     
}