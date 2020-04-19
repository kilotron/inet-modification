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



class Data extends FieldsChunk 
{
    chunkLength = B(78);
    
    unsigned char version = 0;
    unsigned char type = 0;
    int timeToLive = 8;
    bool moreFragments = false;
    B totalLength;
    Port_t portNumber1 = 0;
    Port_t portNumber2 = 0;
    simtime_t minimalPIDUpdateTime = 0;
    unsigned char PIDNum = 0;
    bool O = false;
    bool C = false;
    bool N = false;
    bool P = false;
    unsigned char RES = 0;
    unsigned char PIDIndex = 0;
    //reserved 8 bits
    uint16_t checksum = 0;
    SID sid;
    NID nid;
    MacAddress MAC;
    uint32_t offset = 0;
    B HeaderLength=B(78) ;
    
    long count = 0;
    PIDS pids;
    PID_t pidnr = 0;

    uint16_t MTU = 0;

    NID lastHop;
    bool comeFromSource;
    
    // PCCP字段，用于拥塞控制，在链路层修改
    // 值定义见CiTag.msg文件
    int congestionLevel = 0;
    
    //测试
    path trace;

    bool cluster;

    simtime_t routeLifetime;
    int routeMetric;
}