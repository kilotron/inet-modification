/*
 * ContentBlock.h
 *
 *  Created on: 2019年7月5日
 *      Author: hiro
 */

#ifndef INET_NETWORKLAYER_ICN_COLOR_CACHETABLE_ContentBlock_H_
#define INET_NETWORKLAYER_ICN_COLOR_CACHETABLE_ContentBlock_H_

#include "inet/common/INETDefs.h"
#include "inet/networklayer/common/InterfaceEntry.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/icn/field/SID.h"

#include<vector>
#include<list>

#include "colorCacheTable.h"
#include "colorChunk.h"
namespace inet{
    class ColorCacheTable;
    class colorChunk;
    
    class INET_API ContentBlock : public cObject
    {
        private:
            //拥有这个block的缓存表
            ColorCacheTable* cct = nullptr;
            
            //block对应的SID
            SID sid;

            //每个chunk的大小
            unsigned chunksize=7500;

            //chunk的数量
            unsigned int num=5;

            //网络层的MTU
            unsigned int mtu=2304;

            //生存时间
            simtime_t lifeTime;
            
            //整个block的大小
            B size;
            
            //用list存放chunk
            // std::list<colorChunk* > chunks;

            
            void CaculateSize();

        public:
            std::list<Packet *> packets;

            ContentBlock(SID S, unsigned mtu=2304):sid(S),mtu(mtu){};
            ContentBlock():mtu(2304){};
            ~ContentBlock();

            //创建新的chunk
            colorChunk* CreateNewChunk();

            //插入数据包
            void InsterPacket(Packet* );

            //移除chunk
            void RemoveChunk(colorChunk* );

            void SetCacheTable(ColorCacheTable* table){cct = table;};

            //检查chunk的完整性
            bool CheckIntegrity(colorChunk* );

            //检查此数据包是否已经被缓存
            bool hasThisPacket(Packet *packet);

            //返回chunk的list
            // const std::list<colorChunk* >& GetChunkList(){return chunks;}
            std::list<Packet *> GetList() { return packets; }

            unsigned GetNum(){return num;}
            B GetSize(){return size;}

            //将block转换为字符信息
            std::string str();

            //清除数据包
            void flush();
    };
}




#endif /* INET_NETWORKLAYER_ICN_COLOR_CACHETABLE_ContentBlock_H_ */
