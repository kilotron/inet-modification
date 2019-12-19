/*
 * ContentBlock.cc
 *
 *  Created on: 2019年7月5日
 *      Author: hiro
 */
#include "contentBlock.h"

#include "inet/networklayer/icn/color/Data_m.h"
#include <sstream>
#include <algorithm>

namespace inet
{

colorChunk *ContentBlock::CreateNewChunk()
{
    colorChunk *chunk = new colorChunk(mtu, 0, 5);
    // chunks.push_back(chunk);
    // CaculateSize();
    return chunk;
}

void ContentBlock::InsterPacket(Packet *packet)
{
    //根据报头中的偏移量来插入数据包
    // auto dataHead =packet->peekAtFront<Data>(B(78),Chunk::PF_ALLOW_SERIALIZATION);
    // int seq = dataHead->getOffset();

    // seq = seq/mtu + 1;
    // int index = seq/chunksize + 1;
    // for(auto iter=chunks.begin();iter!=chunks.end();iter++)
    // {
    //     if((*iter)->getIndex()==index)
    //     {
    //         (*iter)->fillPacket(packet,seq % chunksize - 1);
    //         return;
    //     }
    // }

    packets.push_back(packet);
    //不存在对应的chunk就新建后插入
    // auto chunk = CreateNewChunk();
    // chunk->setIndex(index);
    // chunk->fillPacket(packet,seq % chunksize);
    // chunks.push_back(chunk);
    num++;
}

void ContentBlock::RemoveChunk(colorChunk *chunk)
{
    // if(chunk!=nullptr)
    // {
    //     chunks.remove(chunk);
    //     delete chunk;
    // }
}

bool ContentBlock::CheckIntegrity(colorChunk *chunk)
{
    return chunk->isIntegrity();
}

std::string ContentBlock::str()
{
    std::string result(" ");
    return result;
}

ContentBlock::~ContentBlock()
{
}

bool ContentBlock::hasThisPacket(Packet *packet)
{
    auto function = [packet](Packet *p) -> bool {
        const auto &head1 = p->peekAtFront<Data>();
        const auto &head2 = packet->peekAtFront<Data>();
        if (head1->getOffset() == head2->getOffset() && head1->getTotalLength() == head2->getTotalLength())
            return true;
        else
            return false;
    };

    if (find_if(packets.begin(), packets.end(), function) == packets.end())
        return false;
    else
        return true;
}

void ContentBlock::flush()
{
    for_each(packets.begin(), packets.end(), [](Packet* p) {
        Packet *temp = p;
        delete temp;
        p = nullptr;
    });
}
} // namespace inet
