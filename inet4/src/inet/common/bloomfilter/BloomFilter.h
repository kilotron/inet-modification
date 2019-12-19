/*
 * BloomFilter.h
 *
 *  Created on: 2018��9��17��
 *      Author: hiro
 */

#ifndef INET_APPLICATIONS_FAP_BLOOMFILTER_H_
#define INET_APPLICATIONS_FAP_BLOOMFILTER_H_
#include<vector>
#include<functional>
#include<string>
#include"./hash/MurmurHash3.h"

namespace inet{


template<class T>
class BloomFilter{

    using  Position = size_t;
    private:
        
        unsigned m;
        unsigned k;

        Position Hash_fun(size_t h);
        // std::vector<Position> generate(const T &item);
        unsigned int FilterSignature;
        static std::hash<std::string> hashfunc1;

    public:
        std::vector<bool> Filter;
        std::vector<int8_t> Counter;
        BloomFilter():m(512),k(3),Filter(512,0),Counter(512,0){}
        BloomFilter(unsigned m, unsigned k):Filter(m,0),m(m),k(k){}
        BloomFilter(unsigned m, unsigned k, uint32_t value);
        BloomFilter(BloomFilter& F);
        unsigned getM(){return m;}
        unsigned getK(){return k;}
        unsigned getM() const{return m;}
        unsigned getK() const{return k;}
        std::array<uint64_t, 2> hash(const T &data, std::size_t len);

        void setK(unsigned i){k = i;}
        void Insert(const T &item);
        //check if item has been inserted into filter, yes return true, no return false
        bool Check(const T &item);
        // void updateSignature();
        unsigned int getSignature();
        void ResizeFilter(int m);
        void DeleteItem(const T &item);
        void convert(std::string value);
        std::string invert();
        BloomFilter& operator=(std::string value);
        BloomFilter& operator=(const BloomFilter& BF);
        template<class U>
        friend std::ostream& operator<<(std::ostream& out, BloomFilter<U>& BF);
        BloomFilter operator+(BloomFilter& BF);

        uint64_t nthHash(uint8_t n,
                        uint64_t hashA,
                        uint64_t hashB,
                        uint64_t filterSize) {
    return (hashA + n * hashB) % filterSize;}
};

template<class T>
BloomFilter<T>::BloomFilter(unsigned m,unsigned k, uint32_t value)
{
    this->m = m;
    this->k = k;
    Filter = std::vector<int>(m,0);
    
}

template<class T>
std::ostream& operator<<(std::ostream& out, BloomFilter<T>& BF)
{
    std::string filter ="";
    for(auto i=BF.Filter.begin();i!=BF.Filter.end();i++)
    {
        filter= filter + std::to_string(static_cast<int>(*i));
    }
    out<<filter;
    return out;
}

template<class T>
std::hash<std::string>  BloomFilter<T>::hashfunc1;

template<class T>
BloomFilter<T>::BloomFilter(BloomFilter<T> &F)
{
    m = F.getM();
    k = F.getK();
    Filter = F.Filter;
    Counter = F.Counter;
}

template<class T>
std::array<uint64_t, 2> BloomFilter<T>::hash(const T& data,std::size_t len)
{
  std::array<uint64_t, 2> hashValue;
  MurmurHash3_x64_128(&data, len, 0, hashValue.data());

  return hashValue;
}

template<class T>
void BloomFilter<T>::ResizeFilter(int m)
{
    Filter.assign(m,false);
}

// template<class T>
// std::vector<Position> BloomFilter<T>::generate(const T &item)
// {
//     std::vector<Position> p;
//     std::vector<size_t> h;
    
//     h.push_back(hashfunc1(item));
//     for(int i = 0;i < k - 1;i++)
//     {
//         h.push_back(hashfunc2(h.back()));
//     }
//     for(auto iter = h.begin();iter != h.end();iter++)
//     {
//         p.push_back(Hash_fun(*iter));
//     }
//     return p;
// }

template<class T>
void BloomFilter<T>::convert(std::string value)
{
    for(int i = 0; i<value.size();i++)
    {
        Filter[i]=(int)(value[i]-'0');
    }
}

template<class T>
void BloomFilter<T>::Insert(const T& item)
{
    auto hashValues = hash(item, sizeof(T));
    for(int i=0;i<k;i++)
    {
        Filter[nthHash(i,hashValues[0],hashValues[1],m)]=1;

    }
    // auto value = generate(item);
    // for(auto iter = value.begin();iter != value.end();iter++)
    // {
    //    Filter[*iter] = 1;
    //    Counter[*iter] += 1;
    // }
}

template<class T>
void BloomFilter<T>::DeleteItem(const T& item)
{
    auto value = generate(item);
    for(auto iter=value.begin();iter != value.end();iter++)
    {
        if(--Counter[*iter]);
        else Filter[*iter]=0; 
    }
}

template<class T>
bool BloomFilter<T>::Check(const T &item)
{
    // auto value = generate(item);
    bool result = true;
    auto hashValues = hash(item, sizeof(T));
    for(int i=0;i<k;i++)
    {
        if(Filter[nthHash(i,hashValues[0],hashValues[1],m)]==0)
        {
            result=false;
            break;
        }
    }
    // for(auto iter = value.begin();iter != value.end();iter++)
    // {
    //     if(Filter[*iter] == 0)
    //     {
    //         result = false;
    //         break;
    //     }
    // }
    return result;
}

template<class T>
std::string BloomFilter<T>::invert()
{
    std::string result="";
    for(auto i = Filter.begin();i!=Filter.end();i++){
        result=result + std::to_string(static_cast<int>(*i));
    }
    return result;
}

template<class T>
unsigned int BloomFilter<T>::getSignature()
{
    // unsigned int  result = std::stoi(invert()) + k*m - k;
    auto value = invert();
    auto result = hashfunc1(value)+k*m;
//    std::array<uint64_t, 2> hashValue;
//    MurmurHash3_x64_128(value, sizeof(std::string), 0, hashValue.data());
//    unsigned int  result = hashValue[0]+k*m - k;;
    return result;  
}

template<class T>
BloomFilter<T>& BloomFilter<T>::operator=(std::string value)
{
    for(int i = 0; i<value.size();i++)
    {
        Filter[i]=(int)(value[i]-'0');
    }
    return *this;
} 

template<class T>
BloomFilter<T>& BloomFilter<T>::operator=(const BloomFilter& BF)
{
    m = BF.getM();
    k = BF.getK();
    Filter = BF.Filter;
    Counter = BF.Counter;
    return *this;
}

template<class T>
BloomFilter<T> BloomFilter<T>::operator+(BloomFilter& BF)
{
    BloomFilter<T> result=*this;
    if(this->getM()!= BF.getM()||this->getK()!=BF.getK())
    {
        ;
    }
    else{       
        for(int i=0;i<m;i++)
        {
            result.Filter[i] = this->Filter[i] | BF.Filter[i];
        }
    }
    return result;
}

}

#endif /* INET_APPLICATIONS_FAP_BLOOMFILTER_H_ */
