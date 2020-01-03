/*
 * NID.cc
 *
 *  Created on: 2019年10月17日
 *      Author: hiro
 */

#include "NID.h"

const NID& NID::operator=(const NID &other)
{
    for (int i = 0; i < 4;i++)\
    {
        value[i] = other.getNID()[i];
    }
    return *this;
}

bool NID::isDefault()
{
    for (int i = 0; i < 4; i++)
    {
        if(value[i]!=0)
            return false;
    }
    return true;
}

void NID::setNID(std::array<uint64_t, 2> nid)
{
    memcpy(this->value.data(), nid.data(), 16);
}

std::string NID::str() const
{
    char temp[17];
    memcpy(temp, value.data(), 16);
    temp[16] = '\0';
    return std::string{temp};
}

void NID::print(std::ostream &out) const
{
    char temp[17];
    memcpy(temp, value.data(), 16);
    temp[16] = '\0';

    char buf[60];
    char *s = buf;
    for (int i = 0; i < 16; i++, s += 3)
        sprintf(s, "%2.2X-", temp[i]);
    *(s - 1) = '\0';
    out << buf;
}

bool operator<(const NID &lhs, const NID &nid)
{
     return ArrayCmp(lhs.value,nid.value);
}

bool operator!=(const NID &lhs, const NID &rhs){
    return lhs.value != rhs.value;
}

bool operator==(const NID &lhs, const NID &rhs)
{
    return lhs.value == rhs.value;
}

std::ostream &operator<<(std::ostream &os, const NID &entry)
{
    entry.print(os);
    return os;
}





