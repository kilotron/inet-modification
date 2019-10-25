/*
 * colorChunk.cc
 *
 *  Created on: 2019年7月8日
 *      Author: hiro
 */
#include "colorChunk.h"

#include "inet/networklayer/icn/color/Data_m.h"

namespace inet{
    void colorChunk::setIndex(unsigned i)
    {
        index = i;
    }

    unsigned colorChunk::getIndex()
    {
        return index;
    }

    void colorChunk::fillPacket(Packet* packet, int i)
    {
        // auto dataHead =packet->peekAtFront<Data>();
        // index = dataHead->getOffset()/mtu + 1;
        packets[i] = packet->dup();

    }

    B colorChunk::getSize()
    {
        return size;
    }

    Packet* colorChunk::getPacket(unsigned i)
    {
        Packet* packet = new Packet(*packets[i]);
        return packet;
    }

    bool colorChunk::isIntegrity()
    {
        for(auto iter = packets.begin();iter!=packets.end();iter++)
        {
            if((*iter)->getByteLength()==0)
                return false;
        }
        return true;
    }
}


