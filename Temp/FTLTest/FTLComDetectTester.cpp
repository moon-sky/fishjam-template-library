#include "StdAfx.h"
#include "FTLComDetectTester.h"

#include <FTLComDetect.h>

void CFTLComDetectTester::setUp()
{
    HRESULT hr = E_FAIL;
    COM_VERIFY(CoInitialize(NULL));

    COM_VERIFY(m_spShellBrower.CoCreateInstance(CLSID_ShellDesktop));
}

void CFTLComDetectTester::tearDown()
{
    m_spShellBrower.Release();
    CoUninitialize();
}

void CFTLComDetectTester::test_ComDetectIIDFromList()
{

}

void CFTLComDetectTester::test_ComDetectIIDFromRegister()
{
}

void CFTLComDetectTester::test_ComDetectInterfaceFromList()
{
#ifdef FTL_DEBUG
    FTLTRACE(TEXT("%s(%d) : Begin Detect Interface %s( 0x%p ) From List\n"),
        TEXT(__FILE__),__LINE__,TEXT("spShellBrower"),m_spShellBrower);\
        DWORD dwCount = FTL::CFComDetect::CoDetectInterfaceFromList(m_spShellBrower,GUID_NULL,FTL::CFComDetect::cdtInterface);
    CPPUNIT_ASSERT(dwCount >= 2);  //至少实现了 IUnknown 和 IShellFolder 两个接口
#endif 
}

void CFTLComDetectTester::test_ComDetectInterfaceFromRegister()
{
#ifdef FTL_DEBUG
    FTLTRACE(TEXT("%s(%d) : Begin Detect Interface %s( 0x%p ) From Register\n"),
        TEXT(__FILE__),__LINE__,TEXT("spShellBrower"),m_spShellBrower);\
        DWORD dwCount = FTL::CFComDetect::CoDetectInterfaceFromRegister(m_spShellBrower,IID_NULL, 
        FTL::CFComDetect::cdtInterface);
    CPPUNIT_ASSERT(dwCount >= 2);  //至少实现了 IUnknown 和 IShellFolder 两个接口
#endif 
}

void CFTLComDetectTester::test_ComDetectMonikerBindToObjectFromList()
{
#ifdef FTL_DEBUG
    //FTLTRACE(TEXT("%s(%d) : Begin Detect BindToObject %s( 0x%p ) From List\n"),
    //    TEXT(__FILE__),__LINE__,TEXT("spShellBrower"),m_spShellBrower);\
    //    DWORD dwCount = FTL::CFComDetect::CoDetectInterfaceFromList(m_spShellBrower,GUID_NULL,FTL::CFComDetect::cdtMonikerBind);
    //CPPUNIT_ASSERT(dwCount >= 2);  //至少实现了 IUnknown 和 IShellFolder 两个接口
#endif 

}