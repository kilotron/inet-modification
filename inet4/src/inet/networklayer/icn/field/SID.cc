/*
 * SID.cc
 *
 *  Created on: 2019年10月17日
 *      Author: hiro
 */

#include "SID.h"

SID::SID(int nid, long long Lsid) : nidHeader(nid)
{
    MurmurHash3_x64_128(&Lsid, sizeof Lsid, 0, sidTail.data());
    sidTail[4] = 0;
    test = Lsid;
}

std::string SID::str() const
{
    std::string result(nidHeader.str());

}

SID::SID(const SID& other):nidHeader(other.nidHeader), sidTail(other.sidTail),test(other.test)
{
    
    
}

bool operator<(const SID &lhs, const SID &rhs)
{
    // if (lhs.nidHeader == rhs.nidHeader)
    // {
    //     return ArrayCmp(lhs.sidTail, rhs.sidTail);
    // }
    // else
    //     return lhs.nidHeader < rhs.nidHeader;
    return ArrayCmp(lhs.sidTail, rhs.sidTail);
}

bool operator==(const SID &lhs, const SID &rhs)
{
    return lhs.nidHeader == rhs.nidHeader && lhs.sidTail == rhs.sidTail;
}

bool operator!=(const SID &lhs, const SID &rhs)
{
    return !(lhs == rhs);
}
