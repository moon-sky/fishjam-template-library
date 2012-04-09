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

/*************************************************************************************************************
* INITGUID -- 包含 guiddef.h ？ 中定义的所有 GUID(使用库编译已经编译好的),更好的方式是直接包含 InitGuid.h
*************************************************************************************************************/

#ifndef FTL_BASE_H
#  error ftlcom.h requires ftlbase.h to be included first
#endif

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

    FTLEXPORT class CFVariantInfo : public CFConvertInfoT<CFVariantInfo, VARIANT>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFVariantInfo);
    public:
        FTLINLINE explicit CFVariantInfo(VARIANT info);
        FTLINLINE virtual LPCTSTR ConvertInfo();
	private:
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
        //operator T*();
    private:
        IStream*	m_pStream;
        T*	        m_pInterface;
    };

    class IInformationOutput
    {
    public:
        virtual HRESULT SetIndent(int nIndent) = 0;
        virtual HRESULT OutputInfoName(LPCTSTR pszInfoName) = 0;
        virtual HRESULT OnOutput(LPCTSTR pszKey, LPCTSTR pValue) = 0;
        virtual HRESULT OnOutput(LPCTSTR pszKey, BSTR* pValue) = 0;
        virtual HRESULT OnOutput(LPCTSTR pszKey, long nValue) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, HWND hWnd) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, GUID* pGuid) = 0;
		virtual HRESULT OnOutput(LPCTSTR pszKey, VARIANT* pValue) = 0;
        virtual HRESULT OnOutput(LPCTSTR pszKey, long nTotal, long nIndex, VARIANT* pValue) = 0;
    };

    class CFOutputWindowInfoOutput : public IInformationOutput
    {
    public:
        FTLINLINE CFOutputWindowInfoOutput();
        FTLINLINE ~CFOutputWindowInfoOutput();
        FTLINLINE virtual HRESULT SetIndent(int nIndent);
        FTLINLINE virtual HRESULT OutputInfoName(LPCTSTR pszInfoName);
        FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, LPCTSTR pValue);
        FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, BSTR* pValue);
        FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, long nValue);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, HWND hWnd);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, GUID* pGuid);
		FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, VARIANT* pValue);
        FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, long nTotal, long nIndex, VARIANT* pValue);
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
		int                 m_nIndent;
	};

}//namespace FTL

#endif //FTL_COM_H

#ifndef USE_EXPORT
#  include "ftlCom.hpp"
#endif