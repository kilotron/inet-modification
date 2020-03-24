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
import inet.common.Protocol;
import inet.networklayer.icn.color.colorfields;

namespace inet;

enum ColorCommandCode
{
 	 COLOR_C_BIND   = 1;
 	 COLOR_C_SEND_GET = 2;
     COLOR_C_CLOSE = 3;
     COLOR_C_DESTROY = 4;
     COLOR_C_CACHE_DATA = 5; 
};

enum ColorStatusInd {
    COLOR_I_DATA = 0;
    COLOR_I_SOCKET_CLOSED = 1;
};

class ColorSocketCommandBase extends cObject
{
    
}

class ColorSocketBindCommand extends ColorSocketCommandBase
{
    const Protocol *protocol;
    int localPort;
    NID nid;
}

class ColorSocketSendGetCommand extends ColorSocketCommandBase
{
	SID sid;
    int localPort;
    double Inter;   
}

class ColorSocketCacheDataCommand extends ColorSocketCommandBase
{
	SID sid;
    cMessage *pkt; 
}

class ColorSocketCloseCommand extends ColorSocketCommandBase
{
}

class ColorSocketDestroyCommand extends ColorSocketCommandBase
{
}
