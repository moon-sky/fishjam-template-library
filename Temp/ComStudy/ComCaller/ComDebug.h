#pragma once

/************************************************************************
* 使用 IMallocSpy 可以监视内存的使用
* 
* 定义宏
*   _ATL_DEBUG_QI <== 在DEBUG窗口输出QueryInterface()调用情况
*   _ATL_DEBUG_INTERFACES <== 在DEBUG窗口输出接口使用情况（包括接口引用数），注意：MS实现似乎有Bug
* 
* 工具：
*   DCOMCNFG.exe
*   RpcSs.exe -- 远程创建组件时使用？
************************************************************************/
class CComDebug
{
public:
    CComDebug(void);
    ~CComDebug(void);

    //用于注册和反注册指定的COM组件
    HRESULT RegisterCOMObject(LPCTSTR pszObjectPath, BOOL bRegister);
};
