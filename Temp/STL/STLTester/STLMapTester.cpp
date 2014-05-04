#include "StdAfx.h"
#include "STLMapTester.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CSTLMapTester::testAddData()
{
#ifdef NEED_OPERATOR_FUNC
    typedef std::map<int, CMyTestData > IntTestDataMap;
    IntTestDataMap dataMap;
    
    //如果之前没有 insert 或通过 [] 进行设置，将使用默认构造设置其值 
    CMyTestData& defaultData = dataMap[3];
    CPPUNIT_ASSERT(defaultData.GetCurrentIndex() == -1);
    CPPUNIT_ASSERT(dataMap.size() == 1);

    dataMap[1] = 100; //刚开始时没有1对应的数据，插入
    CPPUNIT_ASSERT(dataMap.size() == 2);

    //需要访问 operator = 和 默认构造函数
    #if 0
    //以上代码等价于如下语句 -- 需要构造两个 CMyTestData 对象，效率低
       //用键1建立新映射入口和一个默认构造的值对象
        pair<IntTestDataMap::iterator, bool> result = dataMap.insert(IntTestDataMap::value_type(1, CMyTestData()));
        result.first->second = 100;  //赋值给新构造的值类型
    //此时应该用直截了当的insert调用来替换
        dataMap.insert(IntTestDataMap::value_type(1, 100)); 
    #endif 

    dataMap[1] = 200; //此时1已经对应了数据，更新，此时效率高
    CPPUNIT_ASSERT(dataMap.size() == 2);
    //等价于如下语句
    #if 0
        dataMap.insert(IntTestDataMap::value_type(1, 200)).first->second = 200;
    #endif

    dataMap[2] = 300; //没有 2 对应的数据，插入
    CPPUNIT_ASSERT(dataMap.size() == 3);
    
    IntTestDataMap::iterator iter = efficientAddOrUpdate(dataMap,4,500);  //高效的进行插入
    CPPUNIT_ASSERT(dataMap.size() == 4);

    efficientAddOrUpdate(dataMap,4,1000);  //高效的进行更新
    CPPUNIT_ASSERT(dataMap.size() == 4);

#endif
}

void CSTLMapTester::test_map_sort()
{
    std::map<char,char>   int_map;

    //插入 1到10之间的奇数
    for (int i = 1; i < 10; i+= 2)
    {
        int_map.insert(std::map<char,char>::value_type(i + '0', i+'0'));
    }

    //插入 2到10之间的偶数
    for(int i = 2; i < 10; i+= 2)
    {
        int_map.insert(std::map<char,char>::value_type(i + '0',i+'0'));
    }
    std::string strBuf;
    //std::copy(int_map.begin(),int_map.end(),std::back_inserter<std::string>(strBuf));
    //CPPUNIT_ASSERT(strBuf == "123456789");

}
