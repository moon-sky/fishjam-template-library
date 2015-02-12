// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#include <statreg.cpp>
#endif

#include <atlimpl.cpp>

#include <ftlConfigDetect.h>


BOOL CALLBACK MyTranslateWndClassProc(LPCTSTR pszOriClassName, LPTSTR pszNewClass, UINT nLength)
{
    if (0 == lstrcmpi(pszOriClassName, TEXT("EDITQUOTE"))){
        lstrcpyn(pszNewClass, TEXT("Edit"), nLength);
        return TRUE;
    }
    else if(0 == lstrcmpi(pszOriClassName, TEXT("MOTLEYFOOLTOOLBAR"))){
        lstrcpyn(pszNewClass, TOOLBARCLASSNAME, nLength);
    }
    return FALSE;
}

class CTmpSetup{
public:
    CTmpSetup(){
        SetTranslateWndClassProc(MyTranslateWndClassProc);
    }
    ~CTmpSetup(){
        SetTranslateWndClassProc(NULL);
    }
};

CTmpSetup g_tmpSetup;