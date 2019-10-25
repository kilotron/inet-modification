/*
 * dataType.h
 *
 *  Created on: 2016��5��25��
 *      Author: lhyi2
 */

#ifndef DATATYPE_H_
#define DATATYPE_H_

#include <string>
#include <string.h>
#include <array>
#include <sstream>
#include <iostream>

typedef char Byte;

class nid_type
{
private:
    //char *NID = NULL;
    std::array<Byte, 16> NID;
    bool unset = true;
    int oriint = 0;

public:
    nid_type() { this->NID.fill(0); }

public:
    bool isUnSet() { return unset; }
    std::array<Byte, 16> getNID() const { return this->NID; }
    int getint() const { return oriint; }
    bool operator==(const nid_type &rhs) { return this->NID == rhs.NID; }
    bool operator!=(const nid_type &rhs) { return this->NID != rhs.NID; }

    nid_type &operator=(std::array<uint64_t, 2> nid)
    {

        memcpy(this->NID.data(), nid.data(), 128);
        unset = false;
        return *this;
    }

    std::string str() const
    {
        // std::string result(""); //= std::string(this->NID.data());
        // for(int i = 0; i < 16; i++){
        //     result = result + ch2str(NID[i]);
        // }
        // return result;
        char buf[60];
        char *s = buf;
        for (int i = 0; i < 16; i++, s += 3)
            sprintf(s, "%2.2X-", NID[i]);
        *(s - 1) = '\0';
        return std::string(buf);
    }

    std::string ch2str(char ch) const
    {
        char result[6] = {'\0'};
        sprintf(result, "%d.", ch);
        return std::string(result);
    }

    bool operator<(const nid_type &data) { return str() < data.str(); }
};
typedef nid_type NID_t;

inline std::ostream &operator<<(std::ostream &os, const NID_t &entry)
{

    os << entry.str();
    return os;
}

inline bool operator<(const NID_t &lhs, const NID_t &rhs)
{
    return strcmp(lhs.getNID().data(), rhs.getNID().data()) < 0;
}

typedef std::array<Byte, 4> L_SIDType;
class sid_type
{
private:
    //there are two parts in SID
    //the former 16 bytes : NID of provider or leave blank
    //the last 20 bytes : the hash of content or assigned by NIDs
    //std::array<char,36> SID;
    NID_t NID;
    L_SIDType L_SID;
    int oriint = 0;

public:
    sid_type() {}

public:
    /*decltype(SID)*/
    L_SIDType getLSID() const { return this->L_SID; }

    bool operator==(const sid_type &rhs) { return L_SID == rhs.getLSID(); }
    bool operator!=(const sid_type &rhs) { return L_SID != rhs.getLSID(); }

    sid_type operator++()
    {
        for (int i = 0; i <= 8; ++i)
        {
            char pre = L_SID[i];
            L_SID[i]++;

            if (pre < L_SID[i])
            {
                break;
            }
        }

        return *this;
    }
    sid_type &operator=(int32_t sid)
    {
        oriint = sid;

        char bytecont = (char)(sid & 0x0F);
        this->L_SID[0] = bytecont;
        sid >>= 8;
        bytecont = (char)(sid & 0x0F);
        this->L_SID[1] = bytecont;
        sid >>= 8;
        bytecont = (char)(sid & 0x0F);
        this->L_SID[2] = bytecont;
        sid >>= 8;
        bytecont = (char)(sid & 0x0F);
        this->L_SID[3] = bytecont;

        return *this;
    }

    sid_type(int32_t sid)
    {
        oriint = sid;

        char bytecont = (char)(sid & 0x0F);
        this->L_SID[0] = bytecont;
        sid >>= 8;
        bytecont = (char)(sid & 0x0F);
        this->L_SID[1] = bytecont;
        sid >>= 8;
        bytecont = (char)(sid & 0x0F);
        this->L_SID[2] = bytecont;
        sid >>= 8;
        bytecont = (char)(sid & 0x0F);
        this->L_SID[3] = bytecont;
    }

    sid_type &operator=(const char *sid)
    {
        //        memcpy(this->L_SID.data(), sid, strlen(sid));
        memcpy(this->L_SID.data(), sid, 20);
        return *this;
    }

    bool setNID(NID_t nid)
    {
        //memcpy(this->NID.data(), nid.getNID().data(), 16);
        this->NID = nid;
        return true;
    }

    NID_t getNID() const
    {
        return NID;
    }

    bool setLSID(char *LSID)
    {
        memcpy(this->L_SID.data(), LSID, 20);
        return true;
    }

    std::string str() const
    {
        char buf[30];
        char *s = buf;
        for (int i = 0; i < 16; i++, s += 3)
            sprintf(s, "%2.2X-", L_SID[i]);
        *(s - 1) = '\0';
        return NID.str() + std::string(buf);
    }

    std::string ch2str(char ch) const
    {
        char result[6] = {'\0'};
        sprintf(result, "%d.", ch);
        return std::string(result);
    }

    bool totalEqual(const sid_type &rhs) { return (this->L_SID == rhs.L_SID) && (this->NID == rhs.NID); }
    int getInt() const { return oriint; }
};
typedef sid_type SID_t;

inline std::ostream &operator<<(std::ostream &os, const SID_t &entry)
{
    os << entry.str();
    return os;
}

inline bool operator<(const SID_t &lhs, const SID_t &rhs)
{
    //    std::string lhsdata = lhs.getNID().getNID().data() + std::string(lhs.getLSID().data());
    //    std::string rhsdata = rhs.getNID().getNID().data() + std::string(rhs.getLSID().data());

    return (lhs.getInt() < rhs.getInt());
}

typedef uint32_t PID_t;
//typedef uint32_t SID_t;
typedef uint32_t ASID_t;

typedef int16_t Port_t;

typedef double metric_t;
typedef double UpdataPeriod_t;

typedef std::string LocalIdentifier_t;

typedef uint32_t SK;
typedef uint32_t SHK;

#endif /* DATATYPE_H_ */
