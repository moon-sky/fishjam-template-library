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
#include <atlsafe.h>

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

	CFVariantInfo::CFVariantInfo(VARIANT info) : CFConvertInfoT<CFVariantInfo, VARIANT>(info)
	{
	}

	VOID CFVariantInfo::GetTypeInfo(CFStringFormater& formaterType)
	{
		CFStringFormater formaterTypeEx;
		formaterTypeEx.Format(TEXT(""));	//make sure empty string (not null)

		VARTYPE varType = m_Info.vt;

		HANDLE_COMBINATION_VALUE_TO_STRING(formaterTypeEx,varType, VT_RESERVED, "|");
		HANDLE_COMBINATION_VALUE_TO_STRING(formaterTypeEx,varType, VT_BYREF, "|");
		HANDLE_COMBINATION_VALUE_TO_STRING(formaterTypeEx,varType, VT_ARRAY, "|");
		HANDLE_COMBINATION_VALUE_TO_STRING(formaterTypeEx,varType, VT_VECTOR, "|");

		FTLASSERT((varType & VT_TYPEMASK) == varType && TEXT("just remain type"));
		varType &= VT_TYPEMASK;

		TCHAR szType[32] = {0};
		switch(varType)
		{
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_EMPTY);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_NULL);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_I2);
			//HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_I4);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_R4);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_R8);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_CY);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_DATE);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_BSTR);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_DISPATCH);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_ERROR);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_BOOL);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_VARIANT);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_UNKNOWN);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_DECIMAL);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_I1);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_UI1);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_UI2);
			//HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_UI4);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_I8);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_UI8);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_INT);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_UINT);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_VOID);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_HRESULT);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_PTR);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_SAFEARRAY);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_CARRAY);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_USERDEFINED);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_LPSTR);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_LPWSTR);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_RECORD);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_INT_PTR|VT_I4);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_UINT_PTR|VT_UI4);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_FILETIME);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_BLOB);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_STREAM);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_STORAGE);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_STREAMED_OBJECT);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_STORED_OBJECT);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_BLOB_OBJECT);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_CF);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_CLSID);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_VERSIONED_STREAM);
			HANDLE_CASE_TO_STRING(szType,_countof(szType),VT_BSTR_BLOB);
		default:
			StringCchPrintf(szType,_countof(szType),TEXT("Unknown Type:%d"),varType);
			break;
		}
		formaterType.Format(TEXT("%s%s"), formaterTypeEx.GetString(), szType);
	}

	VOID CFVariantInfo::GetValueInfo(CFStringFormater& formaterValue)
	{
        //有 V_BSTR 等宏辅助操作
		VARTYPE varType = V_VT(&m_Info);

		if (
			(VT_ARRAY == (VT_ARRAY & varType ))||    //ATL::CComSafeArray
			(VT_VECTOR == (VT_VECTOR & varType))||
			(VT_BYREF == (VT_BYREF & varType))
			)
		{
			if (VT_ARRAY == (VT_ARRAY & varType))
			{
				if (NULL != V_ARRAY(&m_Info) && VT_BSTR == V_VT(&m_Info))
				{
					ATL::CComSafeArray<VARIANT> varArray(V_ARRAY(&m_Info));
					LONG nLowerBound = varArray.GetLowerBound();
					LONG nUpperBound = varArray.GetUpperBound();
					formaterValue.AppendFormat(TEXT("[%d-%d]"), nLowerBound, nUpperBound);

					for (LONG nIndex = nLowerBound; nIndex < nUpperBound; ++nIndex)
					{
						VARIANT& varItem = varArray.GetAt(nIndex);
						if (V_VT(&varItem) == VT_BSTR)
						{
							formaterValue.AppendFormat(TEXT("%s,"), COLE2T(V_BSTR(&varItem)));
						}
						else
						{
							FTLASSERT(!TEXT("NOT BSTR"));
						}
					}
				}
				else
				{
					formaterValue.Format(TEXT("NULL BSTR ARRAY"));
				}
			}
			else
			{
				FTLASSERT(!TEXT("VT_VECTOR OR VT_BYREF"));
			}
		}
		else
		{
			varType &= VT_TYPEMASK;  //Get Type

			switch(varType)
			{
			case VT_EMPTY:
				formaterValue.Format(TEXT("<Empty>")); //just empty
				break;
			case VT_NULL:
				formaterValue.Format(TEXT("<Null>")); //just Null
				break;
			case VT_I2:
				formaterValue.Format(TEXT("%d"), V_I2(&m_Info));
				break;
			case VT_I4:
			case VT_INT:
				//case VT_INT_PTR:
				formaterValue.Format(TEXT("%d"), V_INT(&m_Info));
				break;
			case VT_R4:
				formaterValue.Format(TEXT("%f"), V_R4(&m_Info));
				break;
			case VT_R8:
			case VT_DATE:
				formaterValue.Format(TEXT("%e"), V_R8(&m_Info));
				break;
			case VT_CY:
				formaterValue.Format(TEXT("%d-%d"), V_CY(&m_Info).Hi, V_CY(&m_Info).Lo);
				break;
			case VT_BSTR:
				formaterValue.Format(TEXT("%s"), COLE2T(V_BSTR(&m_Info)));
				break;
			case VT_DISPATCH:
			case VT_UNKNOWN:
				formaterValue.Format(TEXT("0x%p"), V_UNKNOWN(&m_Info));
				break;
			case VT_ERROR:
				{
					LONG nError = V_ERROR(&m_Info);
					formaterValue.Format(TEXT("0x%x(%d),%s"), nError, nError, CFAPIErrorInfo(nError).ConvertInfo());
					break;
				}
			case VT_BOOL:
				formaterValue.Format(TEXT("%s"), VARIANT_FALSE == V_BOOL(&m_Info) ? TEXT("FALSE") : TEXT("TRUE") );
				break;
			case VT_VARIANT:
			case VT_DECIMAL:
				FTLASSERT(FALSE);
				break;
			case VT_UI4:
				formaterValue.Format(TEXT("%d"), V_UI4(&m_Info));
				break;
			default:
				formaterValue.Format(TEXT("WARNING,TODO: %lld"), V_I8(&m_Info));
				//FTLASSERT(FALSE);
				break;
			}
		}
	}

	LPCTSTR CFVariantInfo::ConvertInfo()
	{
		if (NULL == m_bufInfo[0])
		{
			//VARIANT m_Info;

			//Get VARIANT Type
			FTL::CFStringFormater formaterType;
			GetTypeInfo(formaterType);

			//Get Variant Value
			FTL::CFStringFormater formaterValue;
			GetValueInfo(formaterValue);

			StringCchPrintf(m_bufInfo, _countof(m_bufInfo),TEXT("Type = %s, Value = %s"),
				formaterType.GetString(),formaterValue.GetString());
		}
		return m_bufInfo;
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

    template<typename T>
	T* QueryInterfaceForBackgroundThread<T>::GetBackThreadInterface()
    { 
        if (!m_pInterface)
        {
            HRESULT hr = E_FAIL;
            COM_VERIFY(CoGetInterfaceAndReleaseStream(m_pStream, __uuidof(T), (VOID**)&m_pInterface));
            //Because the function will Release IStream , so this just assign NULL
            m_pStream = NULL;
        }
        return m_pInterface;
    }

	CFOutputWindowInfoOutput* CFOutputWindowInfoOutput::Instance()
	{
		static CFOutputWindowInfoOutput s_instance;
		return &s_instance;
	}

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

	HRESULT CFOutputWindowInfoOutput::OnOutput(LPCTSTR pszPrompInfo)
	{
		if (pszPrompInfo)
		{
			OutputIndentSpace();
			FTLTRACE(TEXT("  --%s:--\n"), pszPrompInfo);
			return S_OK;
		}
		return S_FALSE;
	}

    HRESULT CFOutputWindowInfoOutput::OnOutput(LPCTSTR pszKey, LPCTSTR pValue)
    {
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

	HRESULT CFOutputWindowInfoOutput::OnOutput(LPCTSTR pszKey, VARIANT_BOOL value)
	{
		if (pszKey)
		{
			OutputIndentSpace();
			FTLTRACE(TEXT("  Key=%s,Value=%s\n"), pszKey, VARIANT_FALSE == value ? TEXT("FALSE") : TEXT("TRUE"));
			return S_OK;
		}
		return S_FALSE;
	}

    HRESULT CFOutputWindowInfoOutput::OnOutput(LPCTSTR pszKey, LONG nValue)
    {
        if (pszKey)
        {
            OutputIndentSpace();
            FTLTRACE(TEXT("  Key=%s,Value=%d\n"), pszKey, nValue);
            return S_OK;
        }
        return S_FALSE;
    }

	HRESULT CFOutputWindowInfoOutput::OnOutput(LPCTSTR pszKey, DWORD dwValue)
	{
		if (pszKey)
		{
			OutputIndentSpace();
			FTLTRACE(TEXT("  Key=%s,Value=%d\n"), pszKey, dwValue);
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
				FTLTRACE(TEXT("  Key=%s,Value=\" %s \"\n"), pszKey, formater.GetString());

				return S_OK;
			}
		}
		return S_FALSE;
	}
	HRESULT CFOutputWindowInfoOutput::OnOutput(LPCTSTR pszKey, GUID* pGuid)
	{
		if (pszKey && pGuid)
		{
			OutputIndentSpace();
			TCHAR szGuidString[40] = {0};
			int nRet = StringFromGUID2(*pGuid, szGuidString, _countof(szGuidString));
			API_ASSERT(0 != nRet);
			if (0 != nRet)
			{
				FTLTRACE(TEXT(" Key=%s,Value=\" %s \"\n"), pszKey, szGuidString);
			}
			return S_OK;			
		}
		return S_FALSE;
	}

    HRESULT CFOutputWindowInfoOutput::OnOutput(LPCTSTR pszKey, VARIANT* pValue)
    {
        if (pszKey && pValue)
        {
            OutputIndentSpace();
			FTL::CFVariantInfo varInfo(*pValue);

			FTLTRACE(TEXT("  Key=%s, Value=\" %s \"\n"), pszKey, varInfo.GetConvertedInfo());
            return S_OK;
        }
        return S_FALSE;
    }

    HRESULT CFOutputWindowInfoOutput::OnOutput(LPCTSTR pszKey, LONG nTotal, LONG nIndex, VARIANT* pValue)
    {
        if (pszKey && pValue)
        {
            OutputIndentSpace();
			FTL::CFVariantInfo varInfo(*pValue);
            FTLTRACE(TEXT("  Key=%s,[%d in %d] Value=\" %s \"\n"), pszKey, nIndex, nTotal, varInfo.GetConvertedInfo());
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