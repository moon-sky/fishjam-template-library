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
