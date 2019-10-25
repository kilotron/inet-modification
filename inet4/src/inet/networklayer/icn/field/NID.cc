/*
 * NID.cc
 *
 *  Created on: 2019年10月17日
 *      Author: hiro
 */

#include "NID.h"

namespace inet{

NID& NID::operator=(std::array<uint64_t, 2> nid)
{
    memcpy(this->value.data(), nid.data(), 128);
    return *this;
}

NID::NID(std::array<uint64_t, 2> nid)
{
    memcpy(this->value.data(), nid.data(), 128);
}

std::string NID::str() const
{
    return std::string{value.data()};
}

void NID::print(std::ostream &out) const
{
    char buf[60];
    char *s = buf;
    for (int i = 0; i < 16; i++, s += 3)
        sprintf(s, "%2.2X-", value[i]);
    *(s - 1) = '\0';
    out << buf;
}

bool NID::operator<(const NID &nid)
{
    return strcmp(value.data(), nid.getNID().data()) < 0;
}

// inline std::ostream &operator<<(std::ostream &os, const NID &entry)
// {
//     entry.print(os);
//     return os;
// }

bool operator<(const NID &lhs, const NID &rhs)
{
    return strcmp(lhs.getNID().data(), rhs.getNID().data()) < 0;
}

bool operator==(const NID &lhs, const NID &rhs)
{
    return strcmp(lhs.getNID().data(), rhs.getNID().data())==0;
}

bool operator!=(const NID &lhs, const NID &rhs)
{
    return strcmp(lhs.getNID().data(), rhs.getNID().data()) != 0;
}
}


