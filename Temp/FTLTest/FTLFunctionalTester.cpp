#include "StdAfx.h"
#include "FTLFunctionalTester.h"
#include <list>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <limits>
#include "ftlFunctional.h"

//int (*func)(const C::key_type& pre, const C::key_type& want, const C::key_type& nex),

void CFTLFunctionalTester::test_PairDeleter()
{
    typedef std::map<int, CMyTestData*> MyTestDataContainer;
    MyTestDataContainer dataContainer;
    LONG lOldCount = CMyTestData::GetTestDataCount();

    for (int i = 0; i < 5; i++)
    {
        dataContainer.insert( std::pair<int,CMyTestData*>( i, new CMyTestData(i)));
    }
    CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() - 5 == lOldCount);

    for_each(dataContainer.begin(), dataContainer.end(), PairDeleter< int,CMyTestData*>());
    dataContainer.clear();

    CPPUNIT_ASSERT( CMyTestData::GetTestDataCount() == lOldCount);
}

void CFTLFunctionalTester::test_Split()
{
    std::list<tstring> tokens1;
    tstring inputString1 = _T("parent1/parent2/parent3");
    FTL::Split(inputString1, _T("/"), false, tokens1);

    CPPUNIT_ASSERT( tokens1.size() == 3);
    std::vector<tstring> tokenVec1(tokens1.begin(), tokens1.end());
    CPPUNIT_ASSERT( tokenVec1[0] == _T("parent1"));
    CPPUNIT_ASSERT( tokenVec1[1] == _T("parent2"));
    CPPUNIT_ASSERT( tokenVec1[2] == _T("parent3"));

    tokens1.clear();
    FTL::Split(inputString1, TEXT("/"), true, tokens1);

    CPPUNIT_ASSERT( tokens1.size() == 3);
    std::vector<tstring> tokenVec2(tokens1.begin(), tokens1.end());
    CPPUNIT_ASSERT( tokenVec2[0] == _T("parent1"));
    CPPUNIT_ASSERT( tokenVec2[1] == _T("/parent2"));
    CPPUNIT_ASSERT( tokenVec2[2] == _T("/parent3"));

}

//比较哪个整数更接近pWant
struct CompareInt
{
#pragma push_macro("max") 
#undef max
    CompareInt(int maxDiff = std::numeric_limits<int>::max())
    {
        m_maxDiff = maxDiff;
    }

    int operator()(const int* pPre, const int* pWant, const int* pNext)
    {
        int dif1 = m_maxDiff;
        int dif2 = m_maxDiff;
        if (pPre || pNext)
        {
            if (pPre)
            {
                dif1 = abs(*pWant - *pPre);
            }
            if(pNext)
            {
                dif2 = abs(*pNext-*pWant);
            }

            if( dif1 <= dif2 && dif1 <= m_maxDiff)
            {
                return -1;
            }
            else if(dif2 <= m_maxDiff)
            {
                return 1;
            }
        }
        return 0;
    }
private:
    int m_maxDiff;
};
#pragma pop_macro("max") 

void CFTLFunctionalTester::test_find_nearest()
{
    test_find_nearest_set_int();
    test_find_nearest_map_int();
    test_find_nearest_set_struct();
    test_find_nearest_empty();
}

void CFTLFunctionalTester::test_find_nearest_set_int()
{
    typedef std::set<int> IntSetContainer;
    bool bResult = false;
    
    IntSetContainer container; //setContainer
    // 0, 3, 6, 9, 12, 15
    for (int i = 0; i <= 15; i += 3)
    {
        container.insert(i);
    }
    
    CPPUNIT_ASSERT(container.size() == 6);
    
    int want;
    IntSetContainer::iterator nearestIter = container.end();

    want = -1;
    nearestIter = container.end();
    bResult = find_nearest(container, want, CompareInt(), nearestIter);
    CPPUNIT_ASSERT(bResult);
    CPPUNIT_ASSERT(*nearestIter == 0);

    want = 4;
    nearestIter = container.end();
    bResult = find_nearest(container, want, CompareInt(), nearestIter);
    CPPUNIT_ASSERT(bResult);
    CPPUNIT_ASSERT(*nearestIter == 3);


    want = 5;
    nearestIter = container.end();
    bResult = find_nearest(container, want, CompareInt(), nearestIter);
    CPPUNIT_ASSERT(bResult);
    CPPUNIT_ASSERT(*nearestIter == 6);

    want = 6;
    nearestIter = container.end();
    bResult = find_nearest(container, want, CompareInt(), nearestIter);
    CPPUNIT_ASSERT(bResult);
    CPPUNIT_ASSERT(*nearestIter == 6);

    want = 20;
    nearestIter = container.end();
    bResult = find_nearest(container, want, CompareInt(), nearestIter);
    CPPUNIT_ASSERT(bResult);
    CPPUNIT_ASSERT(*nearestIter == 15);
}

void CFTLFunctionalTester::test_find_nearest_map_int()
{
    typedef std::map<int, int> IntMapContainer;
    bool bResult = false;

    IntMapContainer container; //setContainer
    // [0,0], [3,30], [6,60], [9,90], [12,120], [15,150]
    for (int i = 0; i <= 15; i += 3)
    {
        container[i] = i * 10;
    }

    CPPUNIT_ASSERT(container.size() == 6);

    int want;
    IntMapContainer::iterator nearestIter = container.end();

    want = -1;
    nearestIter = container.end();
    bResult = find_nearest(container, want, CompareInt(), nearestIter);
    CPPUNIT_ASSERT(bResult);
    CPPUNIT_ASSERT(nearestIter->first == 0);
    CPPUNIT_ASSERT(nearestIter->second == 0);

    want = 4;
    nearestIter = container.end();
    bResult = find_nearest(container, want, CompareInt(), nearestIter);
    CPPUNIT_ASSERT(bResult);
    CPPUNIT_ASSERT(nearestIter->first == 3);
    CPPUNIT_ASSERT(nearestIter->second == 30);


    want = 5;
    nearestIter = container.end();
    bResult = find_nearest(container, want, CompareInt(), nearestIter);
    CPPUNIT_ASSERT(bResult);
    CPPUNIT_ASSERT(nearestIter->first == 6);
    CPPUNIT_ASSERT(nearestIter->second == 60);

    want = 6;
    nearestIter = container.end();
    bResult = find_nearest(container, want, CompareInt(), nearestIter);
    CPPUNIT_ASSERT(bResult);
    CPPUNIT_ASSERT(nearestIter->first == 6);
    CPPUNIT_ASSERT(nearestIter->second == 60);

    want = 20;
    nearestIter = container.end();
    bResult = find_nearest(container, want, CompareInt(), nearestIter);
    CPPUNIT_ASSERT(bResult);
    CPPUNIT_ASSERT(nearestIter->first == 15);
    CPPUNIT_ASSERT(nearestIter->second == 150);
}

//呼叫信息的KEY，要找到callID相同，时间在5秒以内的
struct CallInfo
{
    int callID;     //呼叫ID
    int tickets;    //呼叫时间
    CallInfo()
    {
        callID = -1;
        tickets = -1;
    }
    CallInfo(int callID, int tickets)
    {
        this->callID = callID;
        this->tickets = tickets;
    }
    bool operator < (const CallInfo & other) const
    {
        COMPARE_MEM_LESS(callID, other);
        COMPARE_MEM_LESS(tickets, other);
        return false;
    }

};
struct CompareCallInfo
{
    CompareCallInfo(int maxDiff)
    {
        m_maxDiff = maxDiff;
    }
    int operator()(const CallInfo* pPre, const CallInfo* pWant, const CallInfo* pNext)
    {
#pragma push_macro("max") 
#undef max
        int dif1 = std::numeric_limits<int>::max();
        int dif2 = std::numeric_limits<int>::max();
#pragma pop_macro("max")

        if (pPre && pPre->callID == pWant->callID)
        {
            dif1 = abs(pWant->tickets - pPre->tickets);
        }
        if(pNext && pNext->callID == pWant->callID)
        {
            dif2 = abs(pNext->tickets - pWant->tickets);
        }

        if( dif1 <= dif2 && dif1 <= m_maxDiff)
        {
            return -1;
        }
        else if(dif2 <= m_maxDiff)
        {
            return 1;
        }
        return 0;
    }
    int     m_maxDiff;
};
void CFTLFunctionalTester::test_find_nearest_set_struct()
{
    typedef std::set<CallInfo> CallInfoSetContainer;
    CallInfoSetContainer    container;

    //[0,0],[0,3]
    //[1,10],[1,13]
    //[2,20],[2,23]
    //[3,30],[3,33]
    //[4,40],[4,43]
    //[5,50],[5,53]
    for (int i = 0; i <= 5; i++)
    {
        container.insert(CallInfo(i, i * 10));
        container.insert(CallInfo(i, i * 10 + 3));
    }

    CPPUNIT_ASSERT(container.size() == 12);

    bool bResult = false;
    CallInfo want;
    CallInfoSetContainer::iterator nearestIter = container.end();
    
    want.callID = 1;
    want.tickets = 11;
    nearestIter = container.end();
    bResult = find_nearest(container, want, CompareCallInfo(5), nearestIter);
    CPPUNIT_ASSERT(bResult);
    CPPUNIT_ASSERT(nearestIter->callID == 1);
    CPPUNIT_ASSERT(nearestIter->tickets == 10);

    want.callID = 1;
    want.tickets = 14;
    nearestIter = container.end();
    bResult = find_nearest(container, want, CompareCallInfo(5), nearestIter);
    CPPUNIT_ASSERT(bResult);
    CPPUNIT_ASSERT(nearestIter->callID == 1);
    CPPUNIT_ASSERT(nearestIter->tickets == 13);

    want.callID = 3;
    want.tickets = 30;
    nearestIter = container.end();
    bResult = find_nearest(container, want, CompareCallInfo(5), nearestIter);
    CPPUNIT_ASSERT(bResult);
    CPPUNIT_ASSERT(nearestIter->callID == 3);
    CPPUNIT_ASSERT(nearestIter->tickets == 30);

    //因为超过范围(大于5)，无法找到
    want.callID = 3;
    want.tickets = 39;
    nearestIter = container.end();
    bResult = find_nearest(container, want, CompareCallInfo(5), nearestIter);
    CPPUNIT_ASSERT(!bResult);
    CPPUNIT_ASSERT(container.end() == nearestIter );


    //因为callID不同，所以无法找到
    want.callID = 6;
    want.tickets = 10;
    nearestIter = container.end();
    bResult = find_nearest(container, want, CompareCallInfo(5), nearestIter);
    CPPUNIT_ASSERT(!bResult);
    CPPUNIT_ASSERT(container.end() == nearestIter );
}


void CFTLFunctionalTester::test_find_nearest_empty()
{
    typedef std::set<int> IntSetContainer;
    IntSetContainer    container;
    CPPUNIT_ASSERT(container.size() == 0);

    bool bResult = false;
    int want = 0;
    IntSetContainer::iterator nearestIter = container.end();

    want = 10;
    nearestIter = container.end();
    bResult = find_nearest(container, want, CompareInt(10), nearestIter);
    CPPUNIT_ASSERT(!bResult);
}

void CFTLFunctionalTester::test_binarystream()
{
    binarystream    outStream;
    outStream << "fishjam";
    for (int i = -10; i < 10; i++)
    {
        outStream << (u_int32_t)i;
    }
    outStream << (u_int8_t)'\0';
    outStream << (u_int16_t)1616;
    outStream << (u_int32_t)3232323232;
    LARGE_INTEGER ll64;
    ll64.HighPart = 0x12345678;
    ll64.LowPart = 0x9ABCDEF0;

    outStream << *(u_int64_t*)&ll64;

    const std::string strOutput = outStream.str();
    size_t len = strOutput.length();
    
    CPPUNIT_ASSERT(106 == len);  //4(长度) + 7(fishjam) + 4 * 20 + 1(a) + 2(1616) + 4(3232323232) + 8(0x123456789ABCDEF0)
    char *pBuf = new char[len];
    memcpy(pBuf, strOutput.c_str(), len);
    
    binarystream inStream(pBuf, len );
    delete [] pBuf;

    std::string strInput = "";
    inStream >> strInput;
    CPPUNIT_ASSERT(strInput == "fishjam");

    int iValue[20] = {0};
    for (int i = -10; i < 10; i++)
    {
        inStream >> *(u_int32_t*)&iValue[i+10];
        CPPUNIT_ASSERT(i == iValue[i+10] );
    }
    
    unsigned char uc = 0;
    inStream >> uc;
    CPPUNIT_ASSERT('\0' == uc);

    unsigned short us = 0;
    inStream >> us;
    CPPUNIT_ASSERT(1616 == us);

    unsigned int ui = 0;
    inStream >> ui;
    CPPUNIT_ASSERT(3232323232 == ui);

    unsigned long long ull64 = 0;
    inStream >> ull64;
    CPPUNIT_ASSERT((LONGLONG)ull64 == ll64.QuadPart);
}
