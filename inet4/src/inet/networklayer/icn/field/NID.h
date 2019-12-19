/*
 * NID.h
 *
 *  Created on: 2019年10月17日
 *      Author: hiro
 */

#ifndef INET_NETWORKLAYER_ICN_FIELD_NID_H_
#define INET_NETWORKLAYER_ICN_FIELD_NID_H_

#include <string>
#include <string.h>
#include <array>
#include <iostream>
#include "inet/common/bloomfilter/hash/MurmurHash3.h"

using Word = uint32_t;

void MurmurHash3_x64_128 ( const void * key, int len, uint32_t seed, void * out );

template <unsigned long N>
bool ArrayCmp(const std::array<Word, N> &lhs, const std::array<Word, N> &rhs)
{
    int i = 0;
    while(i<N)
    {
        if(lhs[i]!=rhs[i])
        {
            return lhs[i]<rhs[i];
        }

        i++;
    }
    return false;
}

class NID
{
    private:
        std::array<Word, 4> value;
        int test;

    public:
        NID() { value.fill(0); }

        template<typename T>
        NID(T &param);

        const NID &operator=(const NID& other);

        // NID(std::array<uint64_t, 2> nid);

        friend bool operator==(const NID &lhs,const NID &rhs);
        friend bool operator!=(const NID &lhs,const NID &rhs);
        friend bool operator<(const NID &lhs,const NID &nid);

        std::array<Word, 4> getNID() const { return value; }

        void setNID(std::array<uint64_t, 2> nid);

        // NID &operator=(std::array<uint64_t, 2> nid);

        // NID &operator=(const NID &nid);

        std::string str() const;

        void print(std::ostream &out) const;


};

template<typename T>
NID::NID(T &param)
{
    MurmurHash3_x64_128(&param, sizeof param, 0, value.data());
}

// inline std::ostream &operator<<(std::ostream &os, const NID &entry);

bool operator<(const NID &lhs, const NID &rhs);

bool operator==(const NID &lhs, const NID &rhs);

bool operator!=(const NID &lhs, const NID &rhs);

std::ostream &operator<<(std::ostream &os, const NID &entry);

#endif /* INET_NETWORKLAYER_ICN_FIELD_NID_H_ */
