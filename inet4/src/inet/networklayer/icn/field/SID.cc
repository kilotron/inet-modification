/*
 * SID.cc
 *
 *  Created on: 2019年10月17日
 *      Author: hiro
 */

#include "SID.h"

namespace inet
{
std::string SID::str() const
{
    std::string result(nidHeader.getNID().data()) ;
    result += sidTail.data();
    return result;
}

void SID::print(std::ostream &out) const
{
    nidHeader.print(out);
    char buf[30];
    char *s = buf;
    for (int i = 0; i < 16; i++, s += 3)
        sprintf(s, "%2.2X-", sidTail[i]);
    *(s - 1) = '\0';
    out << buf;
}

bool operator<(const SID &lhs, const SID &rhs)
{
    if (lhs.getNidHead() != rhs.getNidHead())
    {
        return lhs.getNidHead() < rhs.getNidHead();
    }
    else
    {
        return strcmp(lhs.getSidTail().data(), rhs.getSidTail().data());
    }
}

bool operator==(const SID &lhs, const SID &rhs)
{
    return lhs.getNidHead() == rhs.getNidHead() && \
    strcmp(lhs.getSidTail().data(), rhs.getSidTail().data()) == 0;
}

bool operator!=(const SID &lhs, const SID &rhs)
{
    return !(lhs == rhs);
}

} // namespace inet


