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

import inet.linklayer.common.MacAddress;
import inet.networklayer.icn.color.colorfields;

namespace inet;


//
// TODO generated message class
//
class Croute extends cObject
{
    
    NID nextHop;
    MacAddress nextMac;
    simtime_t lifeTime;
    int interFace;
    double linkQlt;
}

cplusplus(Croute) {{
  public:
    Croute(NID next, MacAddress mac, simtime_t time, int interFace = 0, double link= 1.0): cObject(), nextHop(next), nextMac(mac), lifeTime(time), interFace(interFace), linkQlt(link){}

    //比较是否已有相同的路由条目
    bool comparator(Croute& c1, Croute& c2)
    {
        if(c1.nextHop == c2.nextHop && c1.nextMac == c2.nextMac)
            return true;
        else return false;
    }
}}
