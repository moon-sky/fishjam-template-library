#include "StdAfx.h"
#include "FTLSystemTester.h"
#include <ftlSystem.h>
#include <vector>

void CFTLSystemTester::test_IntelIsLittleSystem()
{
    //一般都是Intel平台
    CPPUNIT_ASSERT(CFSystemUtil::IsLittleSystem());
}