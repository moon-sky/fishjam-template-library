#pragma once

#include <cppunit/extensions/HelperMacros.h>

/******************************************************************************************************************
* vector -- 连续存储的元素(其内存连续，因此在增加时可能会出现内存拷贝的问题，但可以和数组互换）
* vector<bool> -- 伪容器?
******************************************************************************************************************/

class CSTLVectorTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLVectorTester );
    
    CPPUNIT_TEST( test_assign );
    CPPUNIT_TEST( test_reserve );
    
    CPPUNIT_TEST( test_vector_memory );
    CPPUNIT_TEST_SUITE_END();

    void test_assign();
    void test_reserve();

    //测试Vector的内存分配
    void test_vector_memory();

    DECLARE_DEFAULT_TEST_CLASS(CSTLVectorTester);
};
