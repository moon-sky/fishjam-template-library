#include "StdAfx.h"
#include "StdioTester.h"


void CStdioTester::test_string_func()
{
    char buf[100] = "100 200";
    int iNum1 = 0;
    int iNum2 = 0;
    char cChar = 'a';

    int nRet = -1;
    nRet = sscanf(buf, "%d %d %c", &iNum1, &iNum2, &cChar);

    //返回成功获取到的个数
    CPPUNIT_ASSERT( 2 == nRet );
    CPPUNIT_ASSERT( 100 == iNum1);
    CPPUNIT_ASSERT( 200 == iNum2);
    CPPUNIT_ASSERT( 'a' == cChar);

    long lNum1 = 0;
    nRet = sscanf(buf, "%ld", &lNum1);
    CPPUNIT_ASSERT(1 == nRet);
    CPPUNIT_ASSERT(100 == lNum1);

}