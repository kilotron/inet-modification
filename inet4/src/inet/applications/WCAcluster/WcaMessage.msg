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
import inet.networklayer.contract.ipv4.Ipv4Address;
import inet.linklayer.common.MacAddress;

namespace inet;

enum WcaMessageType
{
	Wca_Hello_1=1;
	Wca_Hello_2=2;
	Wca_Hello_3=3;
	Wca_Request_1=4;
	Wca_Ack_1=5;
	Wca_Ack_2=6;
	Wca_Ack_3=7;   
}


struct location
{
    double x;
    double y;
    double z;
}

struct velocity
{
    double x;
    double y;
    double z;
}

class WcaMessage extends FieldsChunk
{
    WcaMessageType messageType; // message type

    int clusterID;
    int sourceId; // the Id of the source node

    velocity v;// the velocity of the node x,y,z
    location p;// the location of the node x,y,z
    double weight;// the weight of the node
}
