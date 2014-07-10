#pragma once

#include <cppunit/extensions/HelperMacros.h>

/***************************************************************************************************
*  追踪器(Tracers) -- 一个用户自定义的类，可作为待测试的模板的模板参数，恰好符合待测试模板的要求，可以报告出
*    在它自身上进行的操作的具体方法和顺序。
***************************************************************************************************/

class CTemplateTester : public CPPUNIT_NS::TestFixture
{

    CPPUNIT_TEST_SUITE( CTemplateTester );
    CPPUNIT_TEST( test_fun_template_overload );
    CPPUNIT_TEST( test_tracer );
    CPPUNIT_TEST_SUITE_END();

    void test_fun_template_overload();

    //测试排序算法的追踪器，如追踪 std::sort 怎么比较
    void test_tracer();

    DECLARE_DEFAULT_TEST_CLASS(CTemplateTester);
};