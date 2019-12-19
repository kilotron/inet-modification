/*
 * colorChunk.h
 *
 *  Created on: 2019年7月8日
 *      Author: hiro
 */

#ifndef INET_NETWORKLAYER_ICN_CACHETABLE_COLORCHUNK_H_
#define INET_NETWORKLAYER_ICN_CACHETABLE_COLORCHUNK_H_

#include "inet/common/INETDefs.h"

#include "inet/common/packet/Packet.h"

#include <vector>
#include <list>
#include "colorCacheTable.h"

namespace inet{
    class INET_API colorChunk
    {
        private:
            unsigned mtu = 2304;
            unsigned index;

            //chunk的大小，每次插入数据包后更新
            B size;

            //一个chunk存放的packet数量
            unsigned packet_num;

            //用vector存放packets
            std::vector<Packet*> packets;
            void caculateSize();
        public:
            colorChunk(unsigned m, unsigned index, unsigned num):mtu(m),index(index),packet_num(num)
            {
                for(int i=0;i<packet_num;i++)
                {
                    packets.push_back(nullptr);
                }
            }
            
            ~colorChunk(){
                for(auto iter:packets)
                {
                    delete iter;
                }
            }
            bool isIntegrity();
            void setIndex(unsigned i);
            unsigned getIndex();
            void fillPacket(Packet*,int);
            B getSize();
            Packet* getPacket(unsigned i);

            const std::vector<Packet*> getPackets(){return packets;}
    };
}



#endif /* INET_NETWORKLAYER_ICN_CACHETABLE_COLORCHUNK_H_ */
