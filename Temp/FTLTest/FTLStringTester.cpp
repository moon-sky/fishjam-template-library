#include "StdAfx.h"
#include "FTLStringTester.h"
#include <ftlString.h>

void CFTLStringTester::test_IsMachMask()
{
    


    LPCTSTR pszName = TEXT("This is a same string");
    CPPUNIT_ASSERT(FTL::CFStringUtil::IsMatchMask(pszName, TEXT("*")));
    CPPUNIT_ASSERT(FTL::CFStringUtil::IsMatchMask(pszName, TEXT("This is * string")));
    CPPUNIT_ASSERT(FTL::CFStringUtil::IsMatchMask(pszName, TEXT("This is ? same s??ing")));
    CPPUNIT_ASSERT(FTL::CFStringUtil::IsMatchMask(pszName, TEXT("*string")));
    CPPUNIT_ASSERT(FTL::CFStringUtil::IsMatchMask(pszName, TEXT("* is * string")));
    CPPUNIT_ASSERT(!FTL::CFStringUtil::IsMatchMask(pszName, TEXT("* String")));
}

void CFTLStringTester::test_UpperLower()
{
    //注意：字符串函数操作时，最好使用Unicode版本，否则国际化时可能出现问题

    TCHAR buf[] = _T("some 中华人民共和国 INFO");

    //使用CRT函数
    LPTSTR pUpperResult = _tcsupr(buf);
    CPPUNIT_ASSERT(StrCmp(pUpperResult, _T("SOME 中华人民共和国 INFO")) == 0);

    //使用微软API
    CharLower(buf);
    CPPUNIT_ASSERT(StrCmp(buf, _T("some 中华人民共和国 info")) == 0);

    CharUpperBuff(buf, _countof(buf) - 3); //特意不转换最后的 "fo\0" 共3个字符来测试(包括最后的NULL结束符)
    CPPUNIT_ASSERT(StrCmp(buf, _T("SOME 中华人民共和国 INfo")) == 0);

    //使用MFC中的CString 和 ATL 中的 CAtlString
    CString strMFC = buf;
    strMFC.MakeLower();
    CPPUNIT_ASSERT(StrCmp(strMFC, _T("some 中华人民共和国 info")) == 0);

    CAtlString strAtl = buf;
    strAtl.MakeUpper();
    CPPUNIT_ASSERT(StrCmp(strAtl, _T("SOME 中华人民共和国 INFO")) == 0);


}