/*
 * ColorFragBuf.cc
 *
 *  Created on: 2019年9月3日
 *      Author: hiro
 */

#include <stdlib.h>
#include <string.h>
#include"inet/networklayer/icn/color/ColorFragBuf.h"
#include"inet/networklayer/icn/field/dataType.h"

namespace inet{

ColorFragBuf::ColorFragBuf()
{
//    bufs.clear();
}

ColorFragBuf::~ColorFragBuf()
{
    flush();
}

void ColorFragBuf::flush()
{
    for (auto i = bufs.begin(); i != bufs.end(); ++i)
        delete i->second.packet;
    bufs.clear();
}

Packet *ColorFragBuf::addFragment(Packet *packet, simtime_t now)
{
    auto test = packet->getDataLength().get()/8;
    const auto& DataHead = packet->peekAtFront<Data>(B(78),0);
    auto sid = DataHead->getSID().getInt();
    auto i = bufs.find(sid);

    DatagramBuffer *curBuf = nullptr;
    if (i == bufs.end()) {
        // this is the first fragment of that datagram, create reassembly buffer for it
        curBuf = &bufs[sid];
        i = bufs.find(sid);
    }
    else {
        // use existing buffer
        curBuf = &(i->second);
    }
    
//    ASSERT(DataHead->getTotalLength() >= DataHead->getLength());
    B bytes = DataHead->getTotalLength() - DataHead->getHeaderLength();
    /*
     * *
     */
    auto hL = B(DataHead->getHeaderLength()).get();
    auto len = bytes.get();
    auto datalen = packet->getDataLength().get() / 8;

    curBuf->buf.replace(B(DataHead->getOffset()), packet->peekDataAt(B(DataHead->getHeaderLength()), bytes));
    if(!DataHead->getMoreFragments()){
        curBuf->buf.setExpectedLength(B(DataHead->getOffset()) + bytes);
    }
    if (DataHead->getOffset() == 0 || curBuf->packet == nullptr) {
        delete curBuf->packet;
        curBuf->packet = packet;
    }
    else {
        delete packet;
    }

    if (curBuf->buf.isComplete())
    {
        std::string pkName(curBuf->packet->getName());
        std::size_t found = pkName.find("-frag-");
        if (found != std::string::npos)
            pkName.resize(found);
        auto hdr = Ptr<Data>(curBuf->packet->peekAtFront<Data>()->dup());
        Packet *pk = curBuf->packet;
        pk->setName(pkName.c_str());
        pk->removeAll();
        const auto& payload = curBuf->buf.getReassembledData();
        hdr->setTotalLength(hdr->getHeaderLength() + payload->getChunkLength());
        hdr->setOffset(0);
        hdr->setMoreFragments(false);
        pk->insertAtFront(hdr);
        pk->insertAtBack(payload);
        bufs.erase(i);
        return pk;
    }
    else
    {
        curBuf->lastupdate = now;
        return nullptr;
    }
    
}

void ColorFragBuf::purgeStaleFragments(simtime_t lastupdate)
{
    // this method shouldn't be called too often because iteration on
    // an std::map is *very* slow...
    for (auto i = bufs.begin(); i != bufs.end(); ) {
        // if too old, remove it
        DatagramBuffer& buf = i->second;
        if (buf.lastupdate < lastupdate) {
                      
            if (buf.packet != nullptr)
                
            // delete
            auto oldi = i++;

        }
        else {
            ++i;
        }
    }
}

void ColorFragBuf::clear()
{
    bufs.clear();
}

}
