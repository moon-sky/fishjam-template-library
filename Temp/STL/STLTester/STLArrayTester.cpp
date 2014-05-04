#include "StdAfx.h"
#include "STLArrayTester.h"
#include <array>

void CSTLArrayTester::test_array()
{
	//模版数据 和 编译时常量
	const size_t MAX_COUNT = 10;
	size_t iMaxCount = MAX_COUNT;

	typedef std::tr1::array<int, MAX_COUNT>	MyArrayContainer;
	//typedef std::tr1::array<int, MaxCount>	MyArrayContainer; -- 无法编译过(大小在编译时无法确定)
	
	//可以类似普通数组一样进行初始化
	MyArrayContainer myArray = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	//大小在编译时确定
	CPPUNIT_ASSERT(myArray.size() == MAX_COUNT);
	CPPUNIT_ASSERT(myArray.max_size() == myArray.size());

	CPPUNIT_ASSERT(myArray[0] == 0 );
	CPPUNIT_ASSERT(*myArray.rbegin() == 9);
	CPPUNIT_ASSERT(myArray.empty() == false);

	//data 返回数组第一个元素地址
	CPPUNIT_ASSERT(myArray.data() == &myArray[0]);

//#if SUPPORT_CPP2011
//	//for_each(myArray.begin(), myArray.end(),[](int n)
//	//	{ CPPUNIT_ASSERT(n == myArray[n]);} 
//	//);
//#endif //SUPPORT_CPP2011
}