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
import inet.common.INETDefs;
import inet.common.TagBase;

namespace inet;

// 此Tag向链路层提供PIT的占用情况，添加到Data包中（color.cc: sendDatagramToOutput()），
// queueLength是当前的PIT长度，不是平均值
class PitInd extends TagBase
{
    int pitLength;	// current number of packets in PIT
    int pitCapacity;// maximum allowed number of packets in PIT
}