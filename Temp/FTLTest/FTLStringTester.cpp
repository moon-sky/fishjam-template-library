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

void CFTLStringTester::test_UnicodeHardCodeString()
{
    //字符串转换时没有正确设置编码集(nDrive的一个bug) -- 造成文件名错误
    //  TODO: 两种文字看起来差不多，似乎和Unicode编码相关?
    //酪車樂洛烙串珞狼祿 <== \uF919\uF902\uF914\uF915\uF916\uF905\uF917\uF92B\uF93C
    //酪車樂洛烙串珞狼祿 <== \u916A\u8ECA\u6A02\u6D1B\u70D9\u4E32\u73DE\u72FC\u797F

    //两种硬编码字符串的声明方式
    wchar_t pSrc1[] = { L'\uF919', L'\uF902', L'\uF914', L'\uF915', L'\uF916', L'\uF905', L'\uF917', L'\uF92B', L'\uF93C', '\0' }; 
    wchar_t pSrc2[] = { L"\u916A\u8ECA\u6A02\u6D1B\u70D9\u4E32\u73DE\u72FC\u797F" }; 

    FTLTRACE(TEXT("pSrc1=%s\n"), pSrc1);
    FTLTRACE(TEXT("pSrc2=%s\n"), pSrc2);
}

