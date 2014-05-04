#include "StdAfx.h"
#include "STLTesterUtil.h"

#include <string>
#include <deque>
#include <list>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


LONG g_forEachCount = 0;

void PrintStringItem (std::string& StringToPrint) 
{
    g_forEachCount++;
    TRACE("stringToPrint = %s\n",StringToPrint.c_str());
}

void PrintIntItem (int& intToPrint) 
{
    g_forEachCount++;
    TRACE("%d,",intToPrint);
}

void PrintMyTestData(CMyTestData & myDataToPrint)
{
    g_forEachCount++;
    TRACE(_T("myTestDataIndex = %d\n"),myDataToPrint.GetCurrentIndex());
}

bool CountFailure(int value)
{
    return (value < 60);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

