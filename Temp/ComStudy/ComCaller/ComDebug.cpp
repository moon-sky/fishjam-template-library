#include "StdAfx.h"
#include ".\comdebug.h"

CComDebug::CComDebug(void)
{
}

CComDebug::~CComDebug(void)
{
}

HRESULT CComDebug::RegisterCOMObject(LPCTSTR pszObjectPath, BOOL bRegister)
{
    HRESULT hr = E_FAIL;

    //typedef HRESULT (WINAPI * FREG)();
    //TCHAR szWorkPath[ MAX_PATH ];

    //::GetCurrentDirectory( sizeof(szWorkPath), szWorkPath );	// 保存当前进程的工作目录
    //::SetCurrentDirectory( 组件目录 );	// 切换到组件的目录，防止组件在装载的时候，需要同时加载一些必须依赖的DLL

    //HMODULE hDLL = ::LoadLibrary( 组件文件名 );	// 动态装载组件
    //if(hDLL)
    //{
    //    FREG lpfunc = (FREG)::GetProcAddress( hDLL, _T("DllRegisterServer") );	// 取得注册函数指针
    //    // 如果是反注册，可以取得"DllUnregisterServer"函数指针
    //    if ( lpfunc )	lpfunc();	// 执行注册。这里为了简单，没有判断返回值
    //    ::FreeLibrary(hDLL);
    //}

    //::SetCurrentDirectory(szWorkPath);	// 切换回原先的进程工作目录

    return hr;
}