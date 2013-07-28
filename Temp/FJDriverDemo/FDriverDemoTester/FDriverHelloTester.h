#pragma once

/************************************************************************
* 
************************************************************************/

#ifdef ENABLE_CPPUNIT

#include <cppunit/extensions/HelperMacros.h>
#include "FDriverDemoController.h"

class CFDriverHelloTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CFDriverHelloTester );

    CPPUNIT_TEST( test_DriverHelloWorld );

    CPPUNIT_TEST_SUITE_END();

    void test_DriverHelloWorld();

public:
    virtual void setUp();
    virtual void tearDown();
private:
    CFDriverDemoController      m_FDriverDemoController;
    //DECLARE_DEFAULT_TEST_CLASS(CFDriverHelloTester);
};

#endif //ENABLE_CPPUNIT