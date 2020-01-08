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
import inet.common.ProtocolTag;
import inet.common.packet.chunk.Chunk;
import inet.linklayer.common.MacAddress;
import inet.networklayer.icn.color.colorfields;

namespace inet;




class Get extends FieldsChunk
{
    chunkLength = B(78);

    NID nexthop;
    
    //AppControlInfo controlInfo;
    uint16_t version = 0;
    unsigned char type = 0;
    int timeToLive = 2;
    uint16_t totalLength = 0;
    //32bits

    Port_t portNumber1 = 0;
    Port_t portNumber2 = 0;
    //32bits

    //collect the PID update period of the source node
    simtime_t minimalPIDUpdateTime = 0;
    unsigned char PIDSNum = 0;
    bool O = false;
    bool Q = false;
    bool K = false;
    unsigned char RES;
    //32bits

    uint16_t MTU = 0;
    uint16_t checksum = 0;
    //32bits

    SID sid;
    //160bits

    NID source;
    NID lastHop;
    //256bits

    MacAddress MAC;
    //48bits

    uint32_t offset = 0;
    //32bits

    uint32_t length = 0;
    //32bits

    uint16_t lengthOfQoSRequirement = 0;
    string QosRequirements; 
    uint16_t lengthOfPublicKey = 0;
    string publicKey;
    //暂时32bits
    
    PIDS pids;
    
    bool MustBeFresh = false;
    simtime_t IntrestLifeTime;
    
    unsigned long Nonce;
    
    //测试
    path trace;
}