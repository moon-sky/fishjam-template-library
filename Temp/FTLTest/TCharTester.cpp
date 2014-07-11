#include "StdAfx.h"
#include "TCharTester.h"

#define MIN_BUFFER_SIZE     5
#define MAX_BUFFER_SIZE     15
#define TEST_STRING         TEXT("abcde12345")
#define SHORT_TEST_STRING   TEXT("abc")
#define TEST_STRING_LENGTH  10

#pragma warning(disable : 4996) //This function or variable may be unsafe.

void CTCharTester::test_tcscpy()
{
    TCHAR bufMin[MIN_BUFFER_SIZE] = {0};
    errno_t eRet = 0;
    TCHAR* pChatRet = NULL;

    //注意：本语句会造成程序崩溃 -- 而且无法用异常机制捕获
    //  调试版本弹出对话框 -- "Buffer is too small"
    //ResetBuf(bufMin,_countof(bufMin));
    // _tcscpy_s(bufMin, _countof(bufMin) , TEST_STRING);

    //"abc\0" -- Buf足够大，完整拷贝字符串，而且返回0表示正确
    ResetBuf(bufMin,_countof(bufMin));
    eRet = _tcsncpy_s(bufMin, _countof(bufMin), SHORT_TEST_STRING, _TRUNCATE);
    CPPUNIT_ASSERT(0 == eRet);
    CPPUNIT_ASSERT(_tcscmp(bufMin,TEXT("abc")) == 0);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == TEXT('*'));  //超过范围的字符不会变

    
    //"abcd\0" -- _tcsncpy_s 自动填充结束NULL
    ResetBuf(bufMin,_countof(bufMin));
    eRet = _tcsncpy_s(bufMin,_countof(bufMin),TEST_STRING,_TRUNCATE);
    CPPUNIT_ASSERT(STRUNCATE == eRet);
    CPPUNIT_ASSERT(_tcscmp(bufMin,TEXT("abcd")) == 0);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == TEXT('\0'));


    //"abcd*" -- _tcsncpy 不会自动填充结束NULL
    ResetBuf(bufMin,_countof(bufMin));
    pChatRet = _tcsncpy(bufMin,TEST_STRING,_countof(bufMin) - 1);
    CPPUNIT_ASSERT(pChatRet == bufMin);     //返回目的字符串的地址
    CPPUNIT_ASSERT(_tcsncmp(bufMin,TEXT("abcd"), MIN_BUFFER_SIZE - 1) == 0);
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == TEXT('*'));  //没有自动填充最后的NULL
    bufMin[MIN_BUFFER_SIZE - 1]  = 0;  //需要手工填充确保结束NULL
    CPPUNIT_ASSERT(bufMin[MIN_BUFFER_SIZE - 1] == TEXT('\0'));
}

#pragma warning(default : 4996)