// ATLCom.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "ATLCom.h"

class CATLComModule : public CAtlDllModuleT< CATLComModule >
{
public :
	DECLARE_LIBID(LIBID_ATLComLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLCOM, "{BF5B28A0-CEF0-49B4-B7D8-DE5AC1471092}")
};

CATLComModule _AtlModule;


// DLL Entry Point
// DLL 入口点
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}


// Used to determine whether the DLL can be unloaded by OLE
// 用于确定 DLL 是否可由 OLE 卸载
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
// 返回一个类工厂以创建所请求类型的对象
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
// DllRegisterServer - 将项添加到系统注册表
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    // 注册对象、类型库和类型库中的所有接口
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - Removes entries from the system registry
// DllUnregisterServer - 将项从系统注册表中移除
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}
