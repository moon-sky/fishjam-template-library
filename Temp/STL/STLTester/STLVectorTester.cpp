#include "StdAfx.h"
#include "STLVectorTester.h"
#include "MyTestData.h"

#include <vector>
using std::vector;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CSTLVectorTester::test_assign()
{
    //当处理标准序列容器时，应用单元素成员函数比完成同样目的的区间成员函数需要更多地内存分配，更频繁地拷贝对象，而且/或者造成多余操作
    //inserter,front_inserter
    //copy(v2.begin() + v2.size() / 2, v2.end(), back_inserter(v1)); //也可以用 copy 函数？

#ifdef NEED_OPERATOR_FUNC
    //注意：vector中保存元素时，需要调用 operator=，而list则不需要
    vector<CMyTestData> myTestVectorSrc;
    vector<CMyTestData> myTestVectorTarget;

    for (int i = 0; i< 5; i++)
    {
        myTestVectorSrc.push_back(CMyTestData(i));
    }

    CPPUNIT_ASSERT(myTestVectorSrc.size() == 5);
    CPPUNIT_ASSERT(myTestVectorTarget.empty());

    myTestVectorTarget.assign(myTestVectorSrc.begin() + myTestVectorSrc.size() /2 , myTestVectorSrc.end());
    CPPUNIT_ASSERT(myTestVectorTarget.size() == 3);
    CPPUNIT_ASSERT(myTestVectorSrc.size() == 5);
#else
    vector<int> myTestVectorSrc;
    vector<int> myTestVectorTarget;
    for (int i = 0; i< 5; i++)
    {
        myTestVectorSrc.push_back(i);
    }

    CPPUNIT_ASSERT(myTestVectorSrc.size() == 5);
    CPPUNIT_ASSERT(myTestVectorTarget.empty());

    myTestVectorTarget.assign(myTestVectorSrc.begin() + myTestVectorSrc.size() /2 , myTestVectorSrc.end());
    CPPUNIT_ASSERT(myTestVectorTarget.size() == 3); 
    CPPUNIT_ASSERT(myTestVectorSrc.size() == 5); //保持原有的不变？
#endif
}

void CSTLVectorTester::test_reserve()
{
    vector<int> intVector;
    CPPUNIT_ASSERT( 0 == intVector.capacity());  //刚初始化时使用默认值 -- VC的默认值是 0
    CPPUNIT_ASSERT( 0 == intVector.size());

    intVector.reserve(10);
    CPPUNIT_ASSERT( 10 == intVector.capacity());
    
    CPPUNIT_ASSERT( intVector.empty() );
    CPPUNIT_ASSERT( 0 == intVector.size() );

    for (int i = 0; i< 5; i++)
    {
        intVector.push_back(i);
    }

    CPPUNIT_ASSERT( 10 == intVector.capacity());
    CPPUNIT_ASSERT( 5 == intVector.size());

    for (int i = 5; i< 20; i++)
    {
        intVector.push_back(i);
    }
    CPPUNIT_ASSERT( intVector.capacity() >= 20);  //分配内存后VC实现中默认翻倍(测试出是 22)
    CPPUNIT_ASSERT( 20 == intVector.size());

}

struct BigMemoryStruct
{
    DWORD buf[1000];
};
void CSTLVectorTester::test_vector_memory()
{
    std::vector<BigMemoryStruct*>   vectAddress;    //保存每一个元素的地址
    std::vector<BigMemoryStruct>    vectBigMemorys;

    //保留4个的空间
    vectBigMemorys.reserve(4);
    vectBigMemorys.push_back(BigMemoryStruct());
    BigMemoryStruct* pFirstBigMemoryStruct = &vectBigMemorys[0];
    vectAddress.push_back(pFirstBigMemoryStruct);

    DWORD dwCount = 1;
    while (&vectBigMemorys[0] == pFirstBigMemoryStruct && dwCount < 10)
    {
        vectBigMemorys.push_back(BigMemoryStruct());

        //必然是连续空间
        CPPUNIT_ASSERT(((BYTE*)(&vectBigMemorys[dwCount]) - (BYTE*)(&vectBigMemorys[dwCount-1])) == sizeof(BigMemoryStruct));

        vectAddress.push_back(&vectBigMemorys[dwCount]);
        dwCount++;
    }
    CPPUNIT_ASSERT(dwCount <= 5);   //前面保留了4个，在第5个的时候push_back时就会重新分配内存，并进行拷贝
    for (DWORD dwIndex = 0; dwIndex < dwCount - 1; dwIndex++)
    {
        //vectBigMemorys中的元素移动了位置，已经不是原来的了
        CPPUNIT_ASSERT(&vectBigMemorys[dwIndex] != vectAddress[dwIndex]);
    }
    CPPUNIT_ASSERT(&vectBigMemorys[dwCount-1] == vectAddress[dwCount-1]);   //由于第5个发生在拷贝内存之后，因此保存的是新地址

}