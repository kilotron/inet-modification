import inet.common.INETDefs;
import inet.common.packet.chunk.Chunk;
import inet.networklayer.contract.ipv4.Ipv4Address;
import inet.linklayer.common.MacAddress;

namespace inet;

enum FAPmsgType
{
    HELLO = 1;
    AREQ =2;
    AF =3;
    PARTITION =4;
}


class FAPhead extends FieldsChunk
{
    FAPmsgType FapType;
    bool R;
   	bool I;
   	bool M;
   	int counter;
   	int seq;
    Ipv4Address Source;
    Ipv4Address Dest;
    unsigned int IDnumber;
    string filterValue;
    unsigned int filterM;
    unsigned int filterK;
    
    unsigned long Signature1;
    unsigned long Signature2;
}