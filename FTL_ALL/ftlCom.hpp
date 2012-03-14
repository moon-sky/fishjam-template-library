///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlCom.h
/// @brief  Functional Template Library Base Header File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
/// @defgroup ftlcom ftl com function and class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FTL_COM_HPP
#define FTL_COM_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlCom.h"
#endif

//#include <ExDisp.h>
#include <exdispid.h>
#include <atlcomcli.h>

namespace FTL
{
    BOOL CFComUtility::IsComponentRegistered(REFCLSID clsid)
    {
        BOOL bRet = FALSE;
        HRESULT hr = E_FAIL;
        IUnknown *pUnk = NULL;
        ::SetLastError(ERROR_SUCCESS);
        COM_VERIFY(::CoCreateInstance(clsid,NULL,CLSCTX_ALL,IID_IUnknown,(void**)&pUnk));
        if (SUCCEEDED(hr))
        {
            bRet = TRUE;
            pUnk->Release();
            pUnk = NULL;
        }
        else
        {
            ::SetLastError(hr);
        }
        return bRet;
    }

    BOOL CFComUtility::RegisterComponent(LPCTSTR pszFilePath,BOOL bRegister)
    {
        BOOL bRet = FALSE;
        HRESULT hr = E_FAIL;
        typedef HRESULT (WINAPI* REGISTER_FUNC)(void);
        REGISTER_FUNC pfnReg = NULL;

        TCHAR strOldDir[MAX_PATH] = {0};
        TCHAR strNewDir[MAX_PATH] = {0};
        //LPTSTR pszFileName = NULL;
        ::SetLastError(ERROR_SUCCESS);

        API_VERIFY(GetCurrentDirectory(_countof(strOldDir),strOldDir));
        COM_VERIFY(StringCchCopy(strNewDir,_countof(strNewDir),pszFilePath));
        LPTSTR pLastSpalsh = _tcsrchr(strNewDir,TEXT('\\'));
        if (pLastSpalsh != NULL)
        {
            *pLastSpalsh = TEXT('\0');
            API_VERIFY(SetCurrentDirectory(strNewDir));
            if (bRet)
            {
                HMODULE hModule = ::LoadLibrary(pszFilePath);
                if (hModule)
                {
                    if (bRegister)
                    {
                        pfnReg = (REGISTER_FUNC)::GetProcAddress(hModule,"DllRegisterServer");
                    }
                    else
                    {
                        pfnReg = (REGISTER_FUNC)::GetProcAddress(hModule,"DllUnregisterServer");
                    }

                    if (pfnReg)
                    {
                        HRESULT hr =E_FAIL;
                        COM_VERIFY(pfnReg());
                        bRet = SUCCEEDED(hr);
                    }
                    ::FreeLibrary(hModule);
                }
                ::SetCurrentDirectory(strOldDir);
            }
        }
        return bRet;
    }

    //显示对象的属性页,需要实现ISpecifyPropertyPages接口 -- 如Filter
    HRESULT CFComUtility::DisplayObjectPropertyPage(HWND hWndOwner,
        LPCOLESTR lpszCaption, IUnknown *pUnk)
    {
        CHECK_POINTER_READABLE_DATA_RETURN_VALUE_IF_FAIL(pUnk,sizeof(IUnknown),E_POINTER);
        HRESULT hr = E_FAIL;
        ISpecifyPropertyPages* pProp = NULL;
        COM_VERIFY_EXCEPT1(pUnk->QueryInterface(IID_ISpecifyPropertyPages,
            (void**)&pProp),E_NOINTERFACE);
        if (SUCCEEDED(hr))
        {
            CAUUID caGuid = { 0 };
            COM_VERIFY(pProp->GetPages(&caGuid));
            COM_VERIFY(OleCreatePropertyFrame(hWndOwner,0,0,lpszCaption,1,&pUnk,
                caGuid.cElems,caGuid.pElems,0,0,NULL));
            CoTaskMemFree(caGuid.pElems);
            SAFE_RELEASE(pProp);
        }
        return hr;
    }
    __declspec(selectany) CModulesHolder g_modulesHolder;

    HRESULT /*__stdcall*/ CFSideBySide::SbsCreateInstance(LPCTSTR szModule, REFCLSID rclsid, LPUNKNOWN pUnkOuter, 
        DWORD dwClsContext, REFIID riid, LPVOID FAR* ppv)
    {
        HRESULT hr = S_OK;
        HMODULE hModule = ::LoadLibraryEx(szModule, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
        if (NULL == hModule)
        {
            DWORD dwLastError = ::GetLastError();
            return HRESULT_FROM_WIN32(dwLastError);
        }

        hr = SbsCreateInstanceFromModuleHandle(hModule,rclsid,pUnkOuter,dwClsContext,riid,ppv);

        return hr;
    }

    HRESULT /*__stdcall*/ CFSideBySide::SbsCreateInstanceFromModuleHandle(HMODULE hModule, REFCLSID rclsid, LPUNKNOWN pUnkOuter,
        DWORD dwClsContext, REFIID riid, LPVOID FAR* ppv)
    {
        HRESULT hr = E_FAIL;
        //BOOL bRet = FALSE;
        UNREFERENCED_PARAMETER(dwClsContext); // --如果不需要这个参数，那么外部就不应该传入

        if (g_modulesHolder.Find(hModule))
        {
            g_modulesHolder.Erase(hModule);
        }

        typedef HRESULT (_stdcall *DLLGETCLASSOBJECT_FUNC)(REFCLSID, REFIID, void**);
        DLLGETCLASSOBJECT_FUNC pfDllGetClassObject = NULL;

        pfDllGetClassObject = (DLLGETCLASSOBJECT_FUNC)::GetProcAddress(hModule, "DllGetClassObject");
        if (NULL == pfDllGetClassObject)
        {
            DWORD dwLastError = GetLastError();
            SAFE_FREE_LIBRARY(hModule);
            return HRESULT_FROM_WIN32(dwLastError);
        }
        IClassFactory* pFactory = NULL;
        COM_VERIFY((*pfDllGetClassObject)(rclsid, IID_IClassFactory, (void**)&pFactory));
        if (FAILED(hr))
        {
            SAFE_FREE_LIBRARY(hModule);
            return hr;
        }
        hr = pFactory->CreateInstance(pUnkOuter, riid, (void**)ppv);
        SAFE_RELEASE(pFactory);

        g_modulesHolder.Insert(hModule);


        return hr;
    }

    CFIExplorerDispidInfo::CFIExplorerDispidInfo(DISPID id) : CFConvertInfoT<CFIExplorerDispidInfo ,DISPID>(id)
    {
    }

    LPCTSTR CFIExplorerDispidInfo::ConvertInfo()
    {
        if (NULL == m_bufInfo[0])
        {
            switch (m_Info)
            {
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_BEFORENAVIGATE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_NAVIGATECOMPLETE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_STATUSTEXTCHANGE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_QUIT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_DOWNLOADCOMPLETE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_COMMANDSTATECHANGE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_DOWNLOADBEGIN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_NEWWINDOW);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_PROGRESSCHANGE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_WINDOWMOVE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_WINDOWRESIZE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_WINDOWACTIVATE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_PROPERTYCHANGE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_TITLECHANGE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_TITLEICONCHANGE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_FRAMEBEFORENAVIGATE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_FRAMENAVIGATECOMPLETE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_FRAMENEWWINDOW);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_BEFORENAVIGATE2);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_NEWWINDOW2);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_NAVIGATECOMPLETE2);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_ONQUIT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_ONVISIBLE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_ONTOOLBAR);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_ONMENUBAR);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_ONSTATUSBAR);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_ONFULLSCREEN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_DOCUMENTCOMPLETE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_ONTHEATERMODE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_ONADDRESSBAR);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_WINDOWSETRESIZABLE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_WINDOWCLOSING);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_WINDOWSETLEFT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_WINDOWSETTOP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_WINDOWSETWIDTH);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_WINDOWSETHEIGHT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_CLIENTTOHOSTWINDOW);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_SETSECURELOCKICON);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_FILEDOWNLOAD);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_NAVIGATEERROR);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_PRIVACYIMPACTEDSTATECHANGE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_NEWWINDOW3);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_VIEWUPDATE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_SETPHISHINGFILTERSTATUS);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_WINDOWSTATECHANGED);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_PRINTTEMPLATEINSTANTIATION);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_PRINTTEMPLATETEARDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_UPDATEPAGESTATUS);
                //HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_WINDOWREGISTERED);
                //HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DISPID_WINDOWREVOKED);
            default:
                StringCchPrintf(m_bufInfo,_countof(m_bufInfo),TEXT("Unknown Id:%d"),m_Info);
                break;
            }
        }
        return m_bufInfo;
    }

    template<typename T>
    QueryInterfaceForBackgroundThread<T>::QueryInterfaceForBackgroundThread(IUnknown* pUnk)
        :m_pStream(NULL)
        ,m_pInterface(NULL)
    {
        HRESULT hr = E_FAIL;
        COM_VERIFY(CoMarshalInterThreadInterfaceInStream(__uuidof(T), pUnk, &m_pStream));
    }

    template<typename T>
    QueryInterfaceForBackgroundThread<T>::~QueryInterfaceForBackgroundThread()
    {
        SAFE_RELEASE(m_pStream);
        SAFE_RELEASE(m_pInterface);
    }

    //template<typename T>
    //operator T* QueryInterfaceForBackgroundThread<T>::() 
    //{ 
    //    if (!m_pInterface)
    //    {
    //        HRESULT hr = E_FAIL;
    //        COM_VERIFY(CoGetInterfaceAndReleaseStream(m_pStream, __uuidof(T), (VOID**)&m_pInterface));
    //        //Because the function will Release IStream , so this just assign NULL
    //        m_pStream = NULL;
    //    }
    //    return m_pInterface;
    //}

    CFOutputWindowInfoOutput::CFOutputWindowInfoOutput()
    {
        m_nIndent = 2;
        m_pszIndentSpace = NULL;
    }

    CFOutputWindowInfoOutput::~CFOutputWindowInfoOutput()
    {
        SAFE_DELETE_ARRAY(m_pszIndentSpace);
    }

    HRESULT CFOutputWindowInfoOutput::SetIndent(int nIndent)
    {
        m_nIndent = nIndent;
        SAFE_DELETE_ARRAY(m_pszIndentSpace);
        m_pszIndentSpace  = new TCHAR[m_nIndent + 1];
        for (int i = 0; i < m_nIndent; i++)
        {
            m_pszIndentSpace[i] = ' ';
        }
        m_pszIndentSpace[m_nIndent] = '\0';

        return S_OK;
    }

    VOID CFOutputWindowInfoOutput::OutputIndentSpace()
    {
        FTLTRACE(TEXT("%s"), m_pszIndentSpace);
    }

    HRESULT CFOutputWindowInfoOutput::OutputInfoName(LPCTSTR pszInfoName)
    {
        OutputIndentSpace();
        FTLTRACE(TEXT("<<%s>>:\n"), pszInfoName);
        return S_OK;
    }

    HRESULT CFOutputWindowInfoOutput::OnOutput(LPCTSTR pszKey, LPCTSTR pValue)
    {
        USES_CONVERSION;
        if (pszKey && pValue)
        {
            OutputIndentSpace();
            FTLTRACE(TEXT("  Key=%s,Value=%s\n"), pszKey, pValue);
            return S_OK;
        }
        return S_FALSE;
    }


    HRESULT CFOutputWindowInfoOutput::OnOutput(LPCTSTR pszKey, BSTR* pValue)
    {
        USES_CONVERSION;
        if (pszKey && pValue)
        {
            OutputIndentSpace();
            FTLTRACE(TEXT("  Key=%s,Value=%s\n"), pszKey, OLE2CT(*pValue));
            return S_OK;
        }
        return S_FALSE;
    }

    HRESULT CFOutputWindowInfoOutput::OnOutput(LPCTSTR pszKey, long nValue)
    {
        if (pszKey)
        {
            OutputIndentSpace();
            FTLTRACE(TEXT("  Key=%s,Value=%d\n"), pszKey, nValue);
            return S_OK;
        }
        return S_FALSE;
    }

	HRESULT CFOutputWindowInfoOutput::OnOutput(LPCTSTR pszKey, HWND hWnd)
	{
		if (pszKey)
		{
			BOOL bRet = FALSE;
			OutputIndentSpace();
			API_VERIFY(::IsWindow(hWnd));
			if (bRet)
			{
				TCHAR szClassName[64] = {0};
				API_VERIFY(0 != ::GetClassName(hWnd,szClassName, _countof(szClassName)));
				TCHAR szWindowText[64] = {0};
				API_VERIFY(0 != ::GetWindowText(hWnd, szWindowText, _countof(szWindowText)));
				RECT rcWindow = {0};
				API_VERIFY(::GetWindowRect(hWnd, &rcWindow));
				
				FTL::CFStringFormater formater;
				formater.Format(TEXT("rcWindow=(%d,%d)-(%d,%d), %dx%d, ClassName=%s, WindowText=%s"),
					rcWindow.left, rcWindow.top, rcWindow.right, rcWindow.bottom,
					rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top,
					szClassName, szWindowText);
				FTLTRACE(TEXT("  Key=%s,Value=%d\n"), pszKey, formater.GetString());

				return S_OK;
			}
		}
		return S_FALSE;
	}

    HRESULT CFOutputWindowInfoOutput::OnOutput(LPCTSTR pszKey, VARIANT* pValue)
    {
        USES_CONVERSION;
        if (pszKey && pValue)
        {
#pragma TODO(VARIANT is Safe Array)
            OutputIndentSpace();
            if (VT_BSTR == (VT_BSTR & pValue->vt))
            {
                ATL::CComVariant varString(*pValue);
                varString.ChangeType(VT_BSTR);
                FTLTRACE(TEXT("  Key=%s,Value=%s\n"), pszKey, OLE2CT(varString.bstrVal));
            }
            else
            {
                FTLTRACE(TEXT("  Key=%s,Value=NULL\n"), pszKey);
            }
            return S_OK;
        }
        return S_FALSE;
    }

    HRESULT CFOutputWindowInfoOutput::OnOutput(LPCTSTR pszKey, long nTotal, long nIndex, VARIANT* pValue)
    {
        USES_CONVERSION;
        if (pszKey && pValue)
        {
            OutputIndentSpace();
            if (VT_ARRAY == pValue->vt)
            {
                FTLASSERT(FALSE);
            }
            else
            {
                ATL::CComVariant var(*pValue);
                var.ChangeType(VT_BSTR);
                FTLTRACE(TEXT("  Key=%s,[%d in %d] Value=%s\n"), pszKey, nIndex, nTotal, OLE2CT(var.bstrVal));
            }
            return S_OK;
        }
        return S_FALSE;
    }

	template <typename T>
	CFInterfaceDumperBase<T>::CFInterfaceDumperBase(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
		:m_pObj(pObj)
		,m_nIndent(nIndent)
	{
		if(m_pObj)
        {
            m_pObj->AddRef();
        }
        T* pT = static_cast<T*>(this);
		pInfoOutput->SetIndent(nIndent);
		pT->GetObjInfo(pInfoOutput);
	}

	template <typename T>
	HRESULT CFInterfaceDumperBase<T>::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		FTLASSERT(FALSE);
		return S_FALSE;
	}

    template <typename T>
    CFInterfaceDumperBase<T>::~CFInterfaceDumperBase()
    {
        SAFE_RELEASE(m_pObj);
    }
} //namespace FTL

#endif //FTL_COM_HPP