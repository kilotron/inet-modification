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

namespace inet;

cplusplus  {{
    #include <omnetpp.h>
    #include <stdint.h>
//  	#include "inet/mobility/contract/IMobility.h"
    #include "../field/dataType.h"
}}

enum PacketType
{
    INIT = 0;
    CALL = 1;
    ACK = 2;
}

class NID_t
{
    @existingClass;
    @opaque;
}

struct Position
{
    double x;
    double y;
    double z;
}

class SimpleClusterPacket extends FieldsChunk
{
    int nodeIndex;

    int neighbors;
    chunkLength = B(18);
    PacketType type;
    MacAddress MAC; 
    NID_t NID;
    Position position;
    unsigned long signature;
}

cplusplus(SimpleClusterPacket) {{
  
}}
