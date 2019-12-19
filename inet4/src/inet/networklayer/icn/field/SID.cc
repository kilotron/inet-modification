/*
 * SID.cc
 *
 *  Created on: 2019年10月17日
 *      Author: hiro
 */

#include "SID.h"

std::string SID::str() const{
    std::string result(nidHeader.str());

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
