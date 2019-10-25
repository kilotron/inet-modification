/*
 * SID.h
 *
 *  Created on: 2019年10月17日
 *      Author: hiro
 */

#ifndef INET_NETWORKLAYER_ICN_FIELD_SID_H_
#define INET_NETWORKLAYER_ICN_FIELD_SID_H_

#include "NID.h"
#include<string>
#include <string.h>
#include <array>
#include <iostream>
#include "inet/common/bloomfilter/hash/MurmurHash3.h"

namespace inet
{
using Name = std::array<Byte, 4>;
class SID
{
    private:
        NID nidHeader;
        Name sidTail;
    public:
        //默认构造函数
        SID() : nidHeader() { sidTail.fill(0); }

        //模板构造函数
        template <typename T, typename C>
        SID(const T &param, const C &content);

        //转换为字符串
        std::string str() const;

        //输出到给定输出流
        void print(std::ostream &out) const;

        bool operator<(const SID &sid);

        const NID &getNidHead() const { return nidHeader; }

        const Name &getSidTail() const { return sidTail; }
};

// inline std::ostream &operator<<(std::ostream &os, const SID &sid)
// {
    
// }

template <typename T, typename C>
SID::SID(T const &param, const C &content):nidHeader(param)
{
    char out[128];
    MurmurHash3_x64_128(&content, sizeof content, 0, out);
    memcpy(sidTail.data(), out, 48);
}

bool operator<(const SID &lhs, const SID &rhs);

bool operator==(const SID &lhs, const SID &rhs);

bool operator!=(const SID &lhs, const SID &rhs);
} // namespace inet

#endif /* INET_NETWORKLAYER_ICN_FIELD_SID_H_ */
