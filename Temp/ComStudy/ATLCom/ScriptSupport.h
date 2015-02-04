#ifndef SCRIPT_SUPPORT_H
#define SCRIPT_SUPPORT_H
#pragma once
#include <ActivScp.h>

/****************************************************************************************************
* 脚本(JScript、VBScript等)的应用很广泛 -- HTML、ASP、VBA、安装程序、XML解析、Shell批处理等；
* 
* 在EXE中增加 VBScript、JavaScript 等脚本的支持(使用脚本引擎,如 VBScript、JScript) -- 如：
*   通过脚本解析绘制任意平面函数：http://www.vckbase.com/document/viewdoc/?id=1462
*   在MFC应用程序中加入脚本支持： http://support.microsoft.com/kb/221992/en-us
*
* 当前的Windows平台提供了5种引擎(在 CoCreateInstance(IActiveScript) 时提供)
*   脚本引擎                ProgID              CLSID 
* 1.VBScript                VBScript            {B54F3741-5B07-11CF-A4B0-00AA004A55E8} 
* 2.VBScript encoding       VBScript.Encode     {B54F3743-5B07-11cf-A4B0-00AA004A55E8} 
* 3.JScript                 JScript             {F414C260-6AC0-11CF-B6D1-00AA00BBBB58} 
* 4.JScript encoding        JScript.Encode      {F414C262-6AC0-11CF-B6D1-00AA00BBBB58} 
* 5.XMLScript               XML                 {989D1DC0-B162-11D1-B6EC-D27DDCF9A923}

* 1.实现 IActiveScriptSite 接口来建立执行脚本的主机--脚本引擎会通过该接口获取交互的自动化对象指针；
*   a.GetItemInfo -- 脚本引擎调用该方法来获得任何不认识的对象信息，需要返回自动化对象IDispatch的接口指针，
*     提供这个对象的类型库和IUnknown的接口指针
* 2.实现自动化接口 IDispatch，创建和脚本交互的自动化对象。提供后绑定的方法和属性，进行脚本函数的扩展(可以通过 call XXX 的方式调用)
* 3.包含 <activscp.h> 文件，创建 IActiveScript 接口；
*   a.通过 SetScriptSite 设置自己实现的 ScriptSite 接口
*   b.通过 AddNamedItem 告诉脚本引擎创建的 Item对象，并使其可见，
*   c.通过 SetScriptState( SCRIPTSTATE_CONNECTED ) 调用运行脚本
* 4.通过 IActiveScript 获取 IActiveScriptParse 接口
*   a.通过 ParseScriptText 提供脚本文本
*
* The VBScript engine parses the text in the script for you and when it encounters a method call 
*   or property reference, it delegates the implementation to your provided interfaces.
*
* 运行顺序：
*   spActiveScript.CoCreateInstance( L"VBScript" ); -- 创建(VBScript)脚本引擎，并获得其接口指针
*     =>IActiveScript::QueryInterface(IActiveScriptParse) -- 获得脚本引擎解析的接口指针
*       =>IActiveScript::SetScriptSite(IActiveScriptSite)
*         =>IActiveScriptParse::InitNew -- 初始化脚本引擎的解析对象
*           =>spActiveScript->AddNamedItem(L"MyObject", SCRIPTITEM_ISVISIBLE | SCRIPTITEM_ISSOURCE); 
*             指定本次使用引擎的项目名称 和 标志信息
*             =>IActiveScriptParse::ParseScriptText -- 提交需要运行的脚本文本
*               =>IActiveScriptSite::GetItemInfo    -- 回调取得与脚本交互的自动化组件的类型库和 IUnknown 指针
*                 =>IActiveScript::SetScriptState(SCRIPTSTATE_CONNECTED) -- 设置为运行状态，开始运行
*                   =>脚本中可以通过 call XXX 的方式调用 Site 提供的方法进行通知 -- Invoke方式调用
*                     => IActiveScript::Close() -- 关闭引擎，释放接口指针
*
* 实现对用户输入表达式的运算:
*   编译型语言 -- 需要对表达式进行扫描，去括号，按照运算符的优先级生成2叉树，然后遍历该树生成逆波兰表达式，
*                 再然后通过栈的方法进行运算。
*   解释性语言 -- 
****************************************************************************************************/

template<typename T>
class ATL_NO_VTABLE IActiveScriptSiteImpl : public IActiveScriptSite
{
public:
    //脚本引擎在准备执行脚本程序的时候，调用该函数来询问脚本所使用的语言环境，
    //如果返回 E_NOTIMPL,那么引擎就会使用当前系统默认使用的语言
    STDMETHOD(GetLCID)(LCID *plcid) 
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteImpl::GetLCID\n"));
        return E_NOTIMPL;
    }

    //脚本引擎执行前调用这个函数，获得类型库(IDL文件的二进行形式，保存有函数的参数信息告诉脚本引擎回调的方法)
    //和用于以后获取IDispatch接口(用于通过Invoke执行脚本中的函数)的IUnknown指针。
    //pstrName -- 用于脚本引擎对象区别处理多个脚本项目，给出相应的IUnknown和ITypeInfo 的指针，
    //            其值通过 IActiveScript::AddNamedItem 方法指定。
    STDMETHOD(GetItemInfo)(LPCOLESTR pstrName,DWORD dwReturnMask,IUnknown **ppiunkItem,ITypeInfo **ppti)
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteImpl::GetItemInfo\n"));
        if(ppti)
        {	//取得类型信息库，供脚本进行函数解析用
            *ppti = NULL;
            if(dwReturnMask & SCRIPTINFO_ITYPEINFO)
            {
                CComPtr< ITypeLib > sptLib;
                _ASSERT(FALSE);
                //注意，此处需要换成通用的方法
                ::LoadTypeLib( L"FunMap.tlb", &sptLib );	//装载自身的类型库
                if( sptLib )
                    sptLib->GetTypeInfo( 0, ppti );
            }
        }

        if(ppiunkItem)
        {	//取得 IUnknown 指针，供 VBScript 将来的函数调用
            *ppiunkItem = NULL;
            if(dwReturnMask & SCRIPTINFO_IUNKNOWN)
            {
                _ASSERT(FALSE); //此处也需要换成通用的方法
                if ( 0 == _wcsicmp( L"MyObject", pstrName ) )
                {
                    //以前的代码是制作Active控件的
                    //pT->ControlQueryInterface( IID_IUnknown, (LPVOID *)ppiunkItem );
                    pT->QueryInterface(IID_IUnknown,(LPVOID *)ppiunkItem);
                }
            }
        }
        return E_NOTIMPL;
    }

    //脚本引擎需要通过唯一的一个字符串在适当的时候保存和装入文档的状态,比如在IE中调用记事本编辑HTML源文件。
    //如果返回 E_NOTIMPL，则脚本引擎默认同步使用文档
    STDMETHOD(GetDocVersionString)(BSTR *pbstrVersion)
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteImpl::GetDocVersionString\n"));
        return E_NOTIMPL;
    }

    //脚本引擎执行结束后，在OnStateChange 之前调用这个函数，同时 SCRIPTSTATE_INITIALIZED 已经设置完成
    //参数pvarResult中传递脚本的执行结果，如果为NULL表示脚本没有执行结果。
    //参数pexecpinfo为NULL表示脚本执行没有错误，否则你可以从这个结构中取得发生异常的 具体信息。
    STDMETHOD(OnScriptTerminate)(const VARIANT *pvarResult,const EXCEPINFO *pexcepinfo)
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteImpl::OnScriptTerminate\n"));
        return E_NOTIMPL;
    }

    //脚本引擎在执行脚本过程中，当状态发生改变的时候，调用该函数。信息参考 IActiveScript::GetScriptState
    STDMETHOD(OnStateChange)(SCRIPTSTATE ssScriptState)
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteImpl::OnStateChange\n"));
        return S_OK;
    }

    //当脚本发生错误时调用（如用户输入了不正确的函数式，或脚本执行时出错）
    STDMETHOD(OnScriptError)(IActiveScriptError *pscripterror)
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteImpl::OnScriptError\n"));
        CComBSTR err,msg;
        EXCEPINFO ei = {0};
        HRESULT hr = E_FAIL;
        hr = pscripterror->GetSourceLineText( &err );
        hr = pscripterror->GetExceptionInfo(&ei);
        return S_OK;
    }

    //脚本开始执行时调用
    STDMETHOD(OnEnterScript)(void)
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteImpl::OnEnterScript\n"));
        return S_OK;
    }

    //脚本执行完毕时调用
    STDMETHOD(OnLeaveScript)(void)
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteImpl::OnLeaveScript\n"));
        return S_OK;
    }
};

//使脚本实现与用户的界面交互，脚本引擎会通过IActiveScriptSite::QueryInterface() 来查询这个接口并使用它。
template<typename T>
class ATL_NO_VTABLE IActiveScriptSiteWindowImpl : public IActiveScriptSiteWindow
{
public:
    STDMETHOD(GetWindow)(HWND *phwnd)
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteWindowImpl::GetWindow\n"));
        return E_NOTIMPL;
    }

    STDMETHOD(EnableModeless)(BOOL fEnable)
    {
        T* pT = static_cast<T*>(this);
        ATLTRACE2(atlTraceControls,2,_T("IActiveScriptSiteWindowImpl::EnableModeless\n"));
        return E_NOTIMPL;
    }
};

#endif //SCRIPT_SUPPORT_H