#include "StdAfx.h"
#include "FTLBaseTester.h"
#include <ftlcrashhandler.h>


void CFTLBaseTester::test_LogPerformance()
{
#ifdef FTL_DEBUG
    DECLARE_MYTEST_DATA_COUNT_CHECKER(constructorChecker, ctConstructCount, 1, __FILE__, __LINE__);
#else 
    //Release
	DECLARE_MYTEST_DATA_COUNT_CHECKER(constructorChecker, ctConstructCount, 0, __FILE__, __LINE__);
#endif 
    FTLTRACE(TEXT("test_LogPerformance Dummy Data=%d\n"), CMyTestData(1).GetCurrentIndex());
}

template<typename E, typename A, typename D>
BOOL IsSameNumber_Bad(const E& expected, const A& actual, const D& delta)
{
    //虽然可以自动进行类型转换，但转换后会产生误差，无法判断
    if (FTL_ABS(expected - actual) <= delta)
    {
        return TRUE;
    }
    return FALSE;
}

void CFTLBaseTester::test_IsSameNumber()
{
    CPPUNIT_ASSERT(IsSameNumber(10.0,10.1,1.0));
    CPPUNIT_ASSERT(IsSameNumber(10.0,10.1,0.1)); //可以正确判断数据
    CPPUNIT_ASSERT(FALSE == IsSameNumber(10.0,10.1,0.09));

    CPPUNIT_ASSERT(IsSameNumber (10.0001,10.0001, 0.0000001));
    CPPUNIT_ASSERT(IsSameNumber (10.0001,10.0002, 0.0001));
    CPPUNIT_ASSERT(!IsSameNumber(10.0001,10.0002, 0.00001));


    int     iNum = 10;
    float   fNum = 10.1f;

    double  dNum = 1.0;
    CPPUNIT_ASSERT(IsSameNumber_Bad(iNum,fNum, dNum));

    dNum = 0.01;
    CPPUNIT_ASSERT(!IsSameNumber_Bad(iNum,fNum, dNum));

    dNum = 0.001;
    CPPUNIT_ASSERT(!IsSameNumber_Bad(iNum,fNum, dNum));


    dNum = 0.11;
    CPPUNIT_ASSERT(IsSameNumber_Bad(iNum,fNum, dNum));

    dNum = 0.1;
    //以下由于类型转换，不能正确判断 -- 实际上应该是相同的数据？
    CPPUNIT_ASSERT(FALSE == IsSameNumber((double)iNum,(double)fNum, dNum));
    CPPUNIT_ASSERT(FALSE == IsSameNumber_Bad(iNum,fNum, dNum));
}

void CFTLBaseTester::test_CFMemAllocator()
{
    {
		DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountCheck, ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(consturctCountCheck, ctConstructCount, 10, __FILE__, __LINE__);

        CFMemAllocator<CMyTestData, 10, matNew> localTestData;
        CPPUNIT_ASSERT(localTestData.GetCount() == 10);
        

        CFMemAllocator<TCHAR> localString;
        CPPUNIT_ASSERT(localString.GetCount() == DEFAULT_MEMALLOCATOR_FIXED_COUNT);

        StringCchCopy((TCHAR*)localString,256,TEXT("fishjam"));
        CPPUNIT_ASSERT(_tcscmp(localString,TEXT("fishjam")) == 0);
    }
}

void CFTLBaseTester::test_CFStringFormater()
{
    HRESULT hr = E_FAIL;

    FTL::CFStringFormater formater(20);
    COM_VERIFY(formater.Format(TEXT("Num:%d,String: %s\n"),100,TEXT("test String")));
    CPPUNIT_ASSERT(_tcscmp(TEXT("Num:100,String: test String\n"),formater) == 0);

    COM_VERIFY(formater.AppendFormat(TEXT("Append String is %s\n"),TEXT("This is Append String")));
    CPPUNIT_ASSERT(_tcscmp(TEXT("Num:100,String: test String\nAppend String is This is Append String\n"),formater) == 0);    
}

unsigned int CFTLBaseTester::BlockThreadFun(void* /*pParam*/)
{
    FUNCTION_BLOCK_TRACE(100 * 1000 * 1000);
    Sleep(150);
    return 0;
}


//验证BlockTrace，会在OutPut窗口输出两个不同线程的 Enter -> Leave 信息（注意：不同线程的 Level 是互不干扰的）
void CFTLBaseTester::test_CFBlockElapse()
{
    FUNCTION_BLOCK_TRACE(50 * 1000 * 1000);
    {
        HANDLE hThread = (HANDLE)_beginthreadex(NULL,0,BlockThreadFun,NULL,0,NULL);
        WaitForSingleObject(hThread,INFINITE);
        FUNCTION_BLOCK_NAME_TRACE(TEXT("In test_CFBlockElapse Sub Scope"),100 * 1000 * 1000);
        Sleep(315);
    }
}

void CFTLBaseTester::test_CFFastTrace()
{
    FTL::CFFastTrace::GetInstance().WriteLogInfoEx(FTL::tlDetail,
        TEXT("test from CPP Unit, %d\n"),GetCurrentThreadId());
}

void CFTLBaseTester::test_HandleCaseMacro()
{
    FTL::CFStringFormater formater;
    DWORD dwWindowStype = WS_CHILD | WS_VISIBLE;

    HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwWindowStype, WS_POPUP, TEXT(","));
    HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwWindowStype, WS_CHILD, TEXT(","));
    HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwWindowStype, WS_VISIBLE, TEXT(","));

    CPPUNIT_ASSERT(0 == lstrcmp(formater.GetString(), TEXT("WS_CHILD,WS_VISIBLE,")));
    CPPUNIT_ASSERT(0 == dwWindowStype);
}