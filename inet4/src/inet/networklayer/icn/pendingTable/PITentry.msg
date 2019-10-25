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
import inet.common.packet.chunk.Chunk;
import inet.linklayer.common.MacAddress;
import inet.networklayer.icn.color.colorfields;


namespace inet;


class PITentry extends cObject
{
    NID_t nid;
    simtime_t TTL;
    MacAddress mac;
    
    //24 for 2.4GHz, 5 for 5GHz
    int type;

    //判断是否是GET包的发出者，即Data包是否已经返回到请求发起者
    bool isConsumer;
}

cplusplus(PITentry) {{
  public:
    PITentry(NID_t nid, simtime_t t, MacAddress mac, int type=5,bool isconsumer=false): cObject(), nid(nid), TTL(t), mac(mac),type(type),isConsumer_(isconsumer){}    
}}