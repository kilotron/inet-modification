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

typedef char Byte;

namespace inet{
class NID
{
    private:
        std::array<Byte, 16> value;
    public:
        NID() { value.fill(0); }

        template<typename T>
        NID(T &param);

        NID(std::array<uint64_t, 2> nid);

        bool operator==(const NID &rhs) { return value == rhs.getNID(); }
        bool operator!=(const NID &rhs) { return value != rhs.getNID(); }

        std::array<Byte, 16> getNID() const { return value; }

        NID &operator=(std::array<uint64_t, 2> nid);

        std::string str() const;

        void print(std::ostream &out) const;

        bool operator<(const NID &nid);
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

} // namespace inet

#endif /* INET_NETWORKLAYER_ICN_FIELD_NID_H_ */
