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
        // TODO 在配置文件里改变wq默认值
        double wq = default(0.002); // weight of the current queue length in the averaged queue length
        @class(PccpIndicator);        
}