#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

class CFTLComDetectTester : public CPPUNIT_NS::TestFixture
{
public:
    virtual void setUp();
    virtual void tearDown();

    CPPUNIT_TEST_SUITE( CFTLComDetectTester );

    CPPUNIT_TEST( test_ComDetectIIDFromList);
    CPPUNIT_TEST( test_ComDetectIIDFromRegister);
    CPPUNIT_TEST( test_ComDetectInterfaceFromList);
    CPPUNIT_TEST( test_ComDetectInterfaceFromRegister);
    CPPUNIT_TEST( test_ComDetectMonikerBindToObjectFromList );

    CPPUNIT_TEST_SUITE_END();

    void test_ComDetectIIDFromList();
    void test_ComDetectIIDFromRegister();
    void test_ComDetectInterfaceFromList();
    void test_ComDetectInterfaceFromRegister();
    void test_ComDetectMonikerBindToObjectFromList();

    //DECLARE_DEFAULT_TEST_CLASS(CFTLComDetectTester);
private:
    CComPtr<IUnknown> m_spShellBrower;
};
