///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlFunctional.h
/// @brief  Functional Template Library Functional Header File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FTL_FUNCTIONAL_H
#define FTL_FUNCTIONAL_H
#pragma once

#include <iosfwd>
#include <list>
#include <sstream>
#include "ftlTypes.h"
#include <WinSock2.h>
namespace FTL
{
    template<typename InputIterator,typename OutputIterator,typename Predicate>
    OutputIterator copy_if(InputIterator begin,InputIterator end,OutputIterator destBegin, Predicate p)
    {
        while (begin != end) 
        {
            if (p(*begin))*destBegin++ = *begin;
            ++begin;
        }
        return destBegin;
    }

    template <typename T>
    struct UnreferenceLess : public std::binary_function<T, T, bool>
    {
        bool operator()(const T& _Left, const T& _Right) const
        {
            return (*_Left < *_Right);
        }
    };

    template <typename T>
    struct ObjecteDeleter
    {
        void operator()(const T& ptr) const
        {
            delete ptr;
        }
    };

    //null deleter, can use to handle pair key
    template <typename T>
    struct NullDeleter
    {
        void operator()(const T& /*ptr*/) const
        {
            //Do Nothing
        }
    };

    //usage: for_each(map.begin(), map.end(), PairDeleter...) 
    template <
        typename KEY,
        typename VALUE,
        typename KeyDeleter = NullDeleter<KEY>,
        typename ValueDeleter = ObjecteDeleter<VALUE>
    >
    struct PairDeleter
    {
        void operator()(const std::pair<KEY,VALUE>& pa) const
        {
            KeyDeleter()(pa.first);
            ValueDeleter()(pa.second);
        }
    };

    template <class K>
    const K& ftl_Kfn(const K& Val)
    {
        return (Val);
    }

    template <class K, class V>
    const K& ftl_Kfn(const std::pair<const K,V> & Val)
    {
        return (Val.first);
    }

    template <class K>
    const K& ftl_Vfn(const K& Val)
    {
        return (Val);
    }

    template <class K, class V>
    const V& ftl_Vfn(const std::pair<const K,V> & Val)
    {
        return (Val.second);
    }

    template<typename MapType,typename KeyArgType,typename ValueArgtype>
    typename MapType::iterator efficientAddOrUpdate(MapType& m,const KeyArgType& k, const ValueArgtype& v)
    {
        typename MapType::iterator Ib = m.lower_bound(k);

        if(Ib != m.end() && !(m.key_comp()(k, Ib->first)))
        {
            Ib->second = v;
            return Ib;
        }
        else
        {
            typedef typename MapType::value_type MVT;
            return m.insert(Ib, MVT(k, v));
        }
    }

    //find nearest item in set or map
    // F must be functor like this:
    //   int operator()(const key_type* pPre, const key_type* pWant, const key_type* pNext)
    //   return value:less 0 indicate pPre is nearer , equal 0 indicate none is nearlier, big 0 indicate pNext is nearer
    template<typename C, typename F>
    bool find_nearest(const C& container, 
        const typename C::key_type& want, 
        F func,
        typename C::const_iterator& retIter)
    {
        if(container.empty())
        {
            return false;
        }

        retIter = container.end();
        bool bResult = false;

        typename C::const_iterator nextIter = container.lower_bound(want);
        typename C::const_iterator preIter = nextIter;

        int retCmp = 0; 
        const typename C::key_type* pPreValue = NULL;
        const typename C::key_type* pNextValue = NULL;

        if (nextIter != container.end())
        {
            if(!container.key_comp()(want, ftl_Kfn(*nextIter)))
            {
                bResult = true;
                retIter = nextIter;
                return bResult;
            }

            preIter--;
            if(preIter != container.end())
            {
                pPreValue = &ftl_Kfn(*preIter);
            }
            pNextValue = &ftl_Kfn(*nextIter);
        }
        else
        {
            nextIter = (--container.end());
            pNextValue = &ftl_Kfn(*nextIter);
        }

        retCmp = func(pPreValue,&want,pNextValue);
        if(retCmp < 0)
        {
            bResult = true;
            retIter = preIter;
        }
        else if(retCmp > 0)
        {
            bResult = true;
            retIter = nextIter;
        }

        return bResult;
    }

    template<typename T>
    FTLINLINE T& ToRef(T * p)
    {
        FTLASSERT(NULL != p);
        return *p;
    }

    template<typename T>
    T& ToRef(const std::auto_ptr<T>& sp)
    {
        FTLASSERT(NULL != sp.get());
        return *(sp.get());
    }

    template<typename T>
    T* ToPtr(T &obj)
    {
        return &obj;
    }

    FTLINLINE size_t Split(const std::string& text, 
        const std::string& delimiter,
        bool bWithDelimeter,
        std::list<std::string>& tokens)
    {
        size_t len = text.length();
        size_t start = text.find_first_not_of(delimiter); //找到第一个不是分隔符的
        size_t stop = 0;
        while ( (start >=0) && (start < len))
        {
            stop = text.find_first_of(delimiter, start); //找到这之后的第一个分隔符
            if( (stop < 0) || stop > len)
            {
                stop = len;
            }
            if (bWithDelimeter && start > 0)
            {
                tokens.push_back(text.substr(start - 1, stop - start + 1));
            }
            else
            {
                tokens.push_back(text.substr(start, stop - start));
            }
            start = text.find_first_not_of(delimiter, stop + 1);
        }
        return tokens.size();
    }

    FTLINLINE int GetRandomArray(int from, int to, std::vector<int>& result)
    {
        FTLASSERT(from <= to);
        int size = to - from + 1;
        if (size <= 0)
        {
            return 0;
        }

        std::vector<int> tmpVector;
        tmpVector.resize(size);

        //初始化
        for (int i = from; i <= to; i++)
        {
            tmpVector[i - from] = i;
        }
        result.resize(size);

        //time_t now = 0;
        srand(GetTickCount());

        for (int i = 0; i < size; i++)
        {
            int index = rand() % (size-i);
            result[i] = tmpVector[index];
            tmpVector[index] = tmpVector[size - i - 1];
        }
        return size;
    }
    // usage std::generate(intVect.begin(), intVect.end(), sequence_generator<int>(1,1));
    template<typename T>
    struct sequence_generator
    {
    public:
        sequence_generator(const T& _start, const T& _step)
            :m_start(_start)
            ,m_step(_step)
        {

        }
        T operator()()
        {
            T e = m_start;
            m_start += m_step;
            return e;
        }
    private:
        T m_start;
        T m_step;
    };
    template <typename InType, typename OutType, typename Fun>
    struct sequence_generator_ex
    {
    public:
        sequence_generator_ex(const InType& _start, const InType& _step)
            :m_start(_start)
            ,m_step(_step)
        {

        }
        OutType operator()()
        {
            OutType e = Fun(m_start);
            m_start+= m_step;
            return e;
        }
    private:
        InType m_start;
        InType m_step;
    };
    FTLINLINE int LuhnCalc(const std::string& strInput)
    {
        UNREFERENCED_PARAMETER(strInput);
        FTLASSERT(FALSE);
        return 0;
    }
    class binarystream
    {
    public:
        explicit binarystream(std::ios_base::openmode mode = std::ios_base::out |std::ios_base::in)
            : m_stream(mode) {}
        explicit binarystream(const std::string &str,  
            std::ios_base::openmode mode = std::ios_base::out|std::ios_base::in)  
            : m_stream(str, mode) {}  
        explicit binarystream(const char *str, size_t size,
            std::ios_base::openmode mode = std::ios_base::out|std::ios_base::in)
            : m_stream(std::string(str, size), mode) {}

        binarystream &operator>>(std::string &str)
        {
            u_int32_t length;
            *this >> length;
            if (eof())
            {
                return *this;
            }
            if (length == 0) {
                str.clear();
                return *this;
            }
            std::vector<char> buffer(length);
            m_stream.read(&buffer[0], length);
            if (!eof())
            {
                str.assign(&buffer[0], length);
            }
            return *this;
        }
        binarystream &operator>>(u_int8_t &u8)
        {
            m_stream.read((char *)&u8, 1);
            return *this;
        }
        binarystream &operator>>(u_int16_t &u16)
        {
            u_int16_t temp;
            m_stream.read((char *)&temp, 2);
            if (!eof())
            {
                u16 = ntohs(temp);
            }
            return *this;
        }
        binarystream &operator>>(u_int32_t &u32)
        {
            u_int32_t temp;
            m_stream.read((char *)&temp, 4);
            if (!eof())
            {
                u32 = ntohl(temp);
            }
            return *this;
        }
        binarystream &operator>>(u_int64_t &u64)
        {
            u_int32_t lower, upper;
            *this >> lower >> upper;
            if (!eof())
            {
                u64 = static_cast<u_int64_t>(lower) | (static_cast<u_int64_t>(upper) << 32);
            }
            return *this;
        }

        binarystream &operator<<(const std::string &str)
        {
            u_int32_t length = (u_int32_t)(str.length());
            *this <<  length;
            m_stream.write(str.c_str(), length);
            return *this;
        }
        binarystream &operator<<(u_int8_t u8)
        {
            m_stream.write((const char*)&u8, 1);
            return *this;
        }
        binarystream &operator<<(u_int16_t u16)
        {
            u16 = htons(u16);
            m_stream.write((const char*)&u16, 2);
            return *this;
        }
        binarystream &operator<<(u_int32_t u32)
        {
            u32 = htonl(u32);
            m_stream.write((const char*)&u32, 4);
            return *this;
        }
        binarystream &operator<<(u_int64_t u64)
        {
            // write 64-bit ints as two 32-bit ints, so we can byte-swap them easily
            u_int32_t lower = static_cast<u_int32_t>(u64 & 0xFFFFFFFF);
            u_int32_t upper = static_cast<u_int32_t>(u64 >> 32);
            *this << lower << upper;
            return *this;
        }

        bool eof() const { return m_stream.eof(); }
        void clear() { m_stream.clear(); }
        std::string str() const { return m_stream.str(); }
        void str(const std::string &s){ m_stream.str(s); }
    
        // Seek both read and write pointers to the beginning of the stream.
        void rewind(){
            m_stream.seekg (0, std::ios::beg);
            m_stream.seekp (0, std::ios::beg);
        }
    private:
        std::stringstream m_stream;
    };
    template <typename T>
    class BloomFilter
    {
    public:
    };
}//namespace FTL

#endif //FTL_FUNCTIONAL_H