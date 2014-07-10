#include "StdAfx.h"
#include "WinNLSTester.h"

void CWinNLSTester::setUp()
{
    ZeroMemory(&m_systemtime,sizeof(m_systemtime));
    //1979/03/30 12:15:30:001
    m_systemtime.wYear = 1979;
    m_systemtime.wMonth = 03;
    m_systemtime.wDay = 30;
    m_systemtime.wDayOfWeek = 5;   //÷‹ŒÂ£¨Sunday is 0, Monday is 1

    m_systemtime.wHour = 12;
    m_systemtime.wMinute = 15;
    m_systemtime.wSecond = 30;
    m_systemtime.wMilliseconds = 001;
}

void CWinNLSTester::tearDown()
{

}

void CWinNLSTester::test_GetDateFormat()
{
    TCHAR buf[MAX_PATH] = {0};
    ::GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &m_systemtime, NULL, buf, _countof(buf));
    FTLTRACE(TEXT("GetDateFormat is %s\n"),buf);
    CPPUNIT_ASSERT((_tcscmp(buf, TEXT("1979-3-30")) == 0) || (_tcscmp(buf, TEXT("1979/3/30")) == 0));

	::GetDateFormat(LOCALE_USER_DEFAULT, 0, &m_systemtime, TEXT("yyyy-MM-dd"), buf, _countof(buf));
	FTLTRACE(TEXT("GetDateFormat with Format is %s\n"), buf);
	CPPUNIT_ASSERT(_tcscmp(buf, TEXT("1979-03-30")) == 0);
}

void CWinNLSTester::test_GetNumberFormat()
{
	//GetNumberFormat(LOCALE_USER_DEFAULT, )
	//GetNumberFormat ∫Û Format
}