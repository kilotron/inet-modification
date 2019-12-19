/*
 * ColorFragBuf.h
 *
 *  Created on: 2019年9月3日
 *      Author: hiro
 */

#ifndef INET_NETWORKLAYER_ICN_COLOR_COLORFRAGBUF_H_
#define INET_NETWORKLAYER_ICN_COLOR_COLORFRAGBUF_H_


#include <map>
#include "inet/common/packet/Packet.h"
#include "inet/common/packet/ReassemblyBuffer.h"
#include "inet/networklayer/icn/color/Data_m.h"
#include "inet/networklayer/icn/color/Get_m.h"
#include "inet/networklayer/icn/field/SID.h"


namespace inet{

//包重组缓冲区 ，参考了IPV4和IPV6的实现
class INET_API ColorFragBuf
{
    protected:
        //数据包重组缓冲区的数据结构
        struct DatagramBuffer
        {
            ReassemblyBuffer buf;    // reassembly buffer
            Packet *packet = nullptr;          // the packet
            simtime_t lastupdate;    // last time a new fragment arrived
        };

        //多个buffer通过SID唯一标识
        typedef std::map<SID,DatagramBuffer> Buffers;

        Buffers bufs;

    public:
        //构造函数
        ColorFragBuf();

        //析构函数
        ~ColorFragBuf();

        /*将数据包插入buffer中如果buffer中的所有分片能重组成完整的数据
        包就返回该数据包，否则返回一个空指针*/
        Packet *addFragment(Packet *packet, simtime_t now);

        //删除超时的所有数据包
        void purgeStaleFragments(simtime_t lastupdate);
        
        //清除所有数据包
        void flush();

        void clear();
};
}



#endif /* INET_NETWORKLAYER_ICN_COLOR_COLORFRAGBUF_H_ */
