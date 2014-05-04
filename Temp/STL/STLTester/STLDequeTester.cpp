#include "StdAfx.h"
#include "STLDequeTester.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct BigMemoryStruct
{
    DWORD buf[1000];
};

void CSTLDequeTester::test_deque_memory()
{
    std::deque<BigMemoryStruct*>   dequeAddress;    //保存每一个元素的地址
    std::deque<BigMemoryStruct>    dequeBigMemorys;

    dequeBigMemorys.push_back(BigMemoryStruct());
    BigMemoryStruct* pFirstBigMemoryStruct = &dequeBigMemorys[0];
    dequeAddress.push_back(pFirstBigMemoryStruct);

    DWORD dwCount = 1;
    
    while (&dequeBigMemorys[0] == pFirstBigMemoryStruct && dwCount < 10)
    {
        dequeBigMemorys.push_back(BigMemoryStruct());

        DWORD dwOffset = ((BYTE*)(&dequeBigMemorys[dwCount]) - (BYTE*)(&dequeBigMemorys[dwCount-1]));
        TRACE(TEXT("test_deque_memory,dwCount = %d, dwOffset = %d, &dequeBigMemorys[%d] = %p\n"),
            dwCount,dwOffset, dwCount,&dequeBigMemorys[dwCount]);
        dequeAddress.push_back(&dequeBigMemorys[dwCount]);
        dwCount++;
    }
    CPPUNIT_ASSERT(dwCount == 10);//deque 采用了链表结构，不会出现内存拷贝的问题。
    for (DWORD dwIndex = 0; dwIndex < dwCount - 1; dwIndex++)
    {
        //在没有删除的情况下，每一个元素都在初始放入时的地址
        CPPUNIT_ASSERT(&dequeBigMemorys[dwIndex] == dequeAddress[dwIndex]);
    }

    //删除首尾的元素，其他元素的位置不会进行调整
    dequeBigMemorys.erase(dequeBigMemorys.begin());     //删除第一个
    dequeBigMemorys.erase(dequeBigMemorys.end() - 1);   //删除最后一个
    
    for (DWORD dwIndex = 0; dwIndex < 10 -2 ; dwIndex++)
    {
        CPPUNIT_ASSERT(&dequeBigMemorys[dwIndex] == dequeAddress[dwIndex + 1]);
    }


    //如果删除中间的元素：会将从头到删除位置前一个的元素向后拷贝，并释放首元素的内存
    dequeBigMemorys.erase(dequeBigMemorys.begin() + 3);     //删除第三个
    for (DWORD dwIndex = 0; dwIndex < 10 - 3; dwIndex++)
    {
        CPPUNIT_ASSERT(&dequeBigMemorys[dwIndex] == dequeAddress[dwIndex + 2]);
    }

}