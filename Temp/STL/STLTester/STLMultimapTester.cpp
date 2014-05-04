#include "StdAfx.h"
#include "STLMultimapTester.h"
#include <map>
#include <string>
#include <ostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CSTLMultimapTester::test_multimap()
{
    typedef std::multimap<int , std::string>    Int2StringMap;
    typedef std::pair <int, std::string> Int_String_Pair;

    Int2StringMap intStringMap;
    CPPUNIT_ASSERT( intStringMap.empty() );
    CPPUNIT_ASSERT( intStringMap.size() == 0);

    intStringMap.insert(Int_String_Pair (1, "one"));
    intStringMap.insert(Int_String_Pair (1, "一" ));
    intStringMap.insert(Int_String_Pair (1, "壹" ));
    CPPUNIT_ASSERT( intStringMap.size() == 3);

    intStringMap.insert(Int_String_Pair( 0, "zero"));
    CPPUNIT_ASSERT( intStringMap.size() == 4);

    intStringMap.insert(Int_String_Pair( 2, "two"));
    CPPUNIT_ASSERT( intStringMap.size() == 5);

    //输出 multimap 中的key,可以发现Key有重复,且按照默认顺序进行了排序
    std::ostringstream os;
    for (Int2StringMap::iterator iter = intStringMap.begin(); 
        iter != intStringMap.end();
        ++iter)
    {
        os << iter->first << " ";
    }
    const std::string& strInfoKey = os.str();
    CPPUNIT_ASSERT( strInfoKey == "0 1 1 1 2 ");

    os.str("");
    //获取所有key都为 1 的项
    std::pair <Int2StringMap::const_iterator, Int2StringMap::const_iterator> iterPair;
    iterPair = intStringMap.equal_range(1);
    for (Int2StringMap::const_iterator iter = iterPair.first;
        iter != iterPair.second;
        ++iter)
    {
        os << iter->second << " ";
    }
    const std::string& strInfoValue = os.str();
    CPPUNIT_ASSERT( strInfoValue == "one 一 壹 ");

    Int2StringMap::iterator iter100 = intStringMap.find(100);
    CPPUNIT_ASSERT(iter100 == intStringMap.end());
}