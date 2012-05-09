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

#include <list>
//#include <string>
//#include <functional>
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
}//namespace FTL

#endif //FTL_FUNCTIONAL_H