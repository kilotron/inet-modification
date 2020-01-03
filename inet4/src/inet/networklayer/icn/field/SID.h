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


using Name = std::array<Word, 5>;
class SID
{
    private:
        NID nidHeader;
        Name sidTail;
        int test;
    public:
        //默认构造函数
        SID() : nidHeader() { sidTail.fill(0); }

        //模板构造函数
        template <typename T, typename C>
        SID(const T &param, const C &content);

        // template <>
        // SID::SID(const int &index, const int &content);

        //转换为字符串
        std::string str() const;

        // //输出到给定输出流
        // void print(std::ostream &out) const;

        friend bool operator<(const SID &lhs, const SID &rhs);

        friend bool operator==(const SID &lhs, const SID &rhs);

        friend bool operator!=(const SID &lhs, const SID &rhs);

        const NID &getNidHead() const { return nidHeader; }

        const Name &getSidTail() const { return sidTail; }
};

// inline std::ostream &operator<<(std::ostream &os, const SID &sid)
// {
    
// }

template <typename T, typename C>
SID::SID(const T &param, const C &content) : nidHeader(param), test(content)
{
    char out1[16];
   
    MurmurHash3_x64_128(&content, sizeof content, 0, out1);
    memcpy(sidTail.data(), out1, 16);
    sidTail[4] = 0;
    // MurmurHash3_x64_128(&content, sizeof content, 1, out2);

    // memcpy(sidTail.data()+16, out2, 4);
}

// template <>
// SID::SID(const int &index, const int &content) : nidHeader(index), test(content)
// {
//     char out1[16];

//     MurmurHash3_x64_128(&content, sizeof content, 0, out1);
//     memcpy(sidTail.data(), out1, 16);
// }



#endif /* INET_NETWORKLAYER_ICN_FIELD_SID_H_ */
