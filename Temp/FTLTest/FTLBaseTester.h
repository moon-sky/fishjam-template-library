#pragma once

#include <cppunit/extensions/HelperMacros.h>


/************************************************************************
* 
************************************************************************/

class CFTLBaseTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CFTLBaseTester );
    CPPUNIT_TEST( test_LogPerformance );
    CPPUNIT_TEST( test_IsSameNumber );
    CPPUNIT_TEST( test_CFMemAllocator );
    CPPUNIT_TEST( test_CFStringFormater );
    CPPUNIT_TEST( test_CFBlockElapse );
    CPPUNIT_TEST( test_CFFastTrace );
    CPPUNIT_TEST( test_HandleCaseMacro );

    CPPUNIT_TEST_SUITE_END();

    void test_LogPerformance();
    void test_IsSameNumber();
    void test_CFMemAllocator();
    void test_CFStringFormater();
    void test_CFBlockElapse();
    void test_CFFastTrace();
    void test_HandleCaseMacro();

    DECLARE_DEFAULT_TEST_CLASS(CFTLBaseTester);
private:
    static unsigned int __stdcall BlockThreadFun(void* pParam);
};
