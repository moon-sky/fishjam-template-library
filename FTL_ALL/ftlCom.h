///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlCom.h
/// @brief  Functional Template Library Base Header File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
/// @defgroup ftlcom ftl com function and class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FTL_COM_H
#define FTL_COM_H
#pragma once

#pragma TODO(Delete BSTR In COMStudy)
/*************************************************************************************************************
* INITGUID -- 包含 guiddef.h ？ 中定义的所有 GUID(使用库编译已经编译好的),更好的方式是直接包含 InitGuid.h
*
* 各编程语言的字符串表示
*   C++ 字符串是以null结束的ASCII或Unicode字符数组
*   Visual Basic字符串是一个ASCII字符数组加上表示长度的前缀。
*   Java字符串是以0结束的Unicode字符数组
* 
* COM的基本字符数据类型是OLECHAR，与平台无关的字符表示法，其C++体现即为BSTR
*   BSTR(Basic STRing) <== 带长度(BSTR指针的前4个字节,不包括结束NULL字符的字符串的字节数，如 SysAllocString(L"Hello")长度为10)的宽字符(16bit)数组，
*     可以存贮NULL字节进行分段，并且进行Marshal时更高效 -- 不需要扫描来确定字符串长度；
*     SysAllocString返回的指针指向BSTR的第一个字符(而不是BSTR在内存的第一个字节 -- 长度位置)，因此可直接用于大部分需要 LPCWSTR 或 const OLECHAR*  的地方，
*       但是不能用于 非const 的地方，否则可能照成错误
*     所有的BSTR都必须使用SysFreeString()释放,否则会造成内存泄漏。
*     使用规则（COM接口中推荐使用，但其他时候尽量避免）
*       1.COM接口定义(推荐) -- 可以不用自定义Marshal
*       2.不要在在IDL结构体中定义BSTR成员(避免使用) -- 会给结构体的复制和释放带来麻烦，最好直接使用最大长度的TCHAR数组
*       3.类的成员变量和非COM接口的函数参数不使用BSTR,局部变量要尽快释放
*       4.在对BSTR进行修改（包括创建和释放时），必须使用BSTR的专用函数 -- SysAllocString / SysAllocStringByteLen / SysFreeString / SysStringLen 等
*     转换
*       1.LPSTR   => BSTR  -- a.WCHAR wstr[MAX_WSTR_LEN]; MultiByteToWideChar(CP_ACP, 0, str, strlen(str)+1, wstr, xxx); BSTR bstr1 = ::SysAllocString(wstr);
*                             b._bstr_t("xxxx");
*       2.BSTR    => LPSTR -- a.WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)bstr,xxx); 
*                             b._bstr_t bstr1(bstr); const char* str = static_cast<const char*> bstr1;
*       3.BSTR    => LPWSTR(需要修改) -- UINT unLen = ::SysStringLen(bstr1); LPWSTR psz1 = new WCHAR[len+1]; ::_tcsncpy(psz1, bstr1, unLen);
*       4.BSTR    => LPCWSTR(不修改，只读)  -- LPCWSTR psz2 = static_cast<LPCWSTR>bstr2; 但注意：如果Free bstr2 后就不能再使用 psz2
*       5.CString => BSTR  -- BSTR bstrTest = str1.AllocSysString();  xxxxxx;  ::SysFreeString(bstrTest);  
*                          或 CComBSTR bstr1(static_cast<LPCTSTR>(str1));  不用手工释放
*       6.BSTR    => CString -- BSTR bstrTest = SysAllocString("XXX"); CString str1(bstrTest); SysFreeString(bstrTest); 
*                               问题：CString 是哪种?(ATL/WTL/MFC)
*       
*     辅助类: 
*       CComBSTR -- 只有一个m_str成员，常用于服务器程序，默认构造器将值设置为NULL，析构自动回收BSTR，也可用Empty手动释放，
*         通常需要把值设置为空串，避免客户程序使用未赋值的字串抛出异常，能接受ANSI，但只能输出UNICODE
*         CComBSTR bstrBad(NULL) -- 调用CComBSTR(int nSize)生成长度为0的字符串，等价于CComBSTR bstr2(L(“”))， CComBSTR::m_str 需要释放
*         CComBSTR bstrGood / CComBSTR bstrGood(static_cast< LPCOLESTR> (NULL))才调用 CComBSTR(LPCOLESTR pSrc) 把CComBSTR::m_str 生成NULL
*       _bstr_t -- 有引用计数，要求异常处理，适合于客户程序，提供了到wchar_t和char的转换输出，可以完成简单的BSTR字符串比较等操作
*       std::string/wstring -- 功能最完善，可移植性最好。推荐使用
*       comutil.h 在 _com_util 名称空间中定义了 ConvertStringToBSTR 和 ConvertBSTRToString 转换函数
*     注意：
*       1.Automation会cache BSTR使用的空间，会给发现问题带来困难 -- 因此，应该尽量减小BSTR的生存范围
*       2.把一个已经初始化好的CComBSTR的地址传给一个函数作为[out]参数会导致内存泄漏 -- 必须先调用Empty方法清空字符串的内容，
*         但对于相同的代码，如果参数类型是[in, out]，就不会有泄漏。因为被调函数应该会在复制之前，Free原有的串
*       3.把一个CComBSTR类型的类成员变量返回给传出[out]参数，需要用 Copy 或 Detach
*       4.避免用CComBSTR在内循环进行频繁字符串修改操作，BSTR的效率较低
*************************************************************************************************************/

#ifndef FTL_BASE_H
#  error ftlcom.h requires ftlbase.h to be included first
#endif

#include <exdispid.h>
#include <atlcomcli.h>
#include <atlsafe.h>

namespace FTL
{
//Class
    
//Function
    class CFComUtility
    {
    public:
        FTLINLINE static BOOL IsComponentRegistered(REFCLSID clsid);
        FTLINLINE static BOOL RegisterComponent(LPCTSTR pszFilePath,BOOL bRegister);
        FTLINLINE static HRESULT DisplayObjectPropertyPage(HWND hWndOwner,LPCOLESTR lpszCaption, IUnknown *pUnk);
    };

    FTLEXPORT class CFVariantInfo : public CFConvertInfoT<CFVariantInfo, const VARIANT&>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFVariantInfo);
    public:
        FTLINLINE explicit CFVariantInfo(const VARIANT& info);
        FTLINLINE virtual LPCTSTR ConvertInfo();
	public:
		FTLINLINE VOID GetTypeInfo(CFStringFormater& formaterType);
		FTLINLINE VOID GetValueInfo(CFStringFormater& formaterValue);
    };

	FTLEXPORT class CFPropVariantInfo : public CFConvertInfoT<CFPropVariantInfo, const PROPVARIANT&>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFPropVariantInfo);
	public:
		FTLINLINE explicit CFPropVariantInfo(const PROPVARIANT& info);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	public:
		FTLINLINE VOID GetTypeInfo(CFStringFormater& formaterType);
		FTLINLINE VOID GetValueInfo(CFStringFormater& formaterValue);
	};

    class CFSideBySide //从指定文件中产生COM对象(从sbs中拷贝)
    {
    public:
        FTLINLINE static HRESULT /*__stdcall*/ SbsCreateInstance(LPCTSTR szModule, REFCLSID rclsid, LPUNKNOWN pUnkOuter, 
            DWORD dwClsContext, REFIID riid, LPVOID FAR* ppv);
        FTLINLINE static HRESULT /*__stdcall*/ SbsCreateInstanceFromModuleHandle(HMODULE hModule, REFCLSID rclsid, LPUNKNOWN pUnkOuter,
            DWORD dwClsContext, REFIID riid, LPVOID FAR* ppv);
    };

    FTLEXPORT class CFIExplorerDispidInfo : public CFConvertInfoT<CFIExplorerDispidInfo, DISPID>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFIExplorerDispidInfo);
    public:
        FTLINLINE explicit CFIExplorerDispidInfo(DISPID id);
        FTLINLINE virtual LPCTSTR ConvertInfo();
    };

    //多线程之间传递COM接口的辅助类
    template<typename T>
    class QueryInterfaceForBackgroundThread
    {
    public:
        QueryInterfaceForBackgroundThread(IUnknown* pUnk);
        ~QueryInterfaceForBackgroundThread();
		T* GetBackThreadInterface();
        //operator T*();
    private:
        IStream*	m_pStream;
        T*	        m_pInterface;
    };

    class IInformationOutput
    {
    public:
        virtual HRESULT SetIndent(int nIndent) = 0;
		virtual int GetIndent() = 0;
        virtual HRESULT OutputInfoName(LPCTSTR pszInfoName) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszPrompInfo) = 0;
        virtual HRESULT OnOutput(LPCTSTR pszKey, LPCTSTR pValue) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, BSTR* pValue) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, VARIANT_BOOL value) = 0;
        virtual HRESULT OnOutput(LPCTSTR pszKey, LONG nValue) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, DWORD dwValue) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, DOUBLE fValue) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, int iValue) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, HWND hWnd) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, GUID* pGuid) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, VARIANT* pValue) = 0;
        virtual HRESULT OnOutput(LPCTSTR pszKey, LONG nTotal, LONG nIndex, VARIANT* pValue) = 0;
    };

    class CFOutputWindowInfoOutput : public IInformationOutput
    {
    public:
		FTLINLINE static CFOutputWindowInfoOutput* Instance();
        FTLINLINE CFOutputWindowInfoOutput();
        FTLINLINE ~CFOutputWindowInfoOutput();
        FTLINLINE virtual HRESULT SetIndent(int nIndent);
		FTLINLINE virtual int GetIndent();
        FTLINLINE virtual HRESULT OutputInfoName(LPCTSTR pszInfoName);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszPrompInfo);
        FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, LPCTSTR pValue);
        FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, BSTR* pValue);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, VARIANT_BOOL value);
        FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, LONG nValue);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, DWORD dwValue);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, DOUBLE fValue);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, int iValue);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, HWND hWnd);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, GUID* pGuid);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, VARIANT* pValue);
        FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, LONG nTotal, LONG nIndex, VARIANT* pValue);
    protected:
        int     m_nIndent;
        TCHAR*  m_pszIndentSpace;
        FTLINLINE virtual VOID    OutputIndentSpace();
    };

	template <typename T>
	class CFInterfaceDumperBase
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFInterfaceDumperBase);
	public:
		FTLINLINE explicit CFInterfaceDumperBase(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent);
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
        virtual ~CFInterfaceDumperBase();
	protected:
		IUnknown*           m_pObj;
		IInformationOutput*	m_pInfoOutput;
		int                 m_nIndent;
	};

}//namespace FTL

#endif //FTL_COM_H

#ifndef USE_EXPORT
#  include "ftlCom.hpp"
#endif