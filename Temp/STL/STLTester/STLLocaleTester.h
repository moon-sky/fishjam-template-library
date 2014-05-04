#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* C++中的区域设置是一个std::locale类型的对象，可以建立它并传给函数，就像其他任何对象一样
*   std::locale L = std::locale::classic(); //建立一个表示通常区域的区域设置对象
* C++里的区域设置分成多个方面（facet），每个方面处理一个国际化的不同方向，函数std::use_facet从区域设置对象中提取一个特定的方面
*    std::use_facet<numpunct<char_type> >(loc:decimal_point);  -- 语法不全，可能有错
*    local::global(local(" ", local::ctype)); -- 将字符处理部分...
*    
* collate 方面的存在正好封装了排序的细节 
* tolower的参数和返回值类型是int，但除非这个int是EOF，它的值必须能表现为一个unsigned char。
* static_cast<unsigned char>(c1));// char c1;
* 
************************************************************************/
class CSTLLocaleTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLLocaleTester );
    //CPPUNIT_TEST( testAddData );
    CPPUNIT_TEST_SUITE_END();

    //void testAddData();


    DECLARE_DEFAULT_TEST_CLASS(CSTLLocaleTester);
};
