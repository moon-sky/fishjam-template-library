#ifndef FTL_WIN_INTERFACE_DETECT_HPP
#define FTL_WIN_INTERFACE_DETECT_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlWinIntDetect.h"
#endif


namespace FTL
{
	LPCTSTR CFWinInterfaceDetectUtil::GetSystemStateFlags(CFStringFormater& formater, DWORD nFlags, LPCTSTR pszDivide /* = TEXT */)
	{
        DWORD oldFlags = nFlags;

		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_UNAVAILABLE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_SELECTED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_FOCUSED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_PRESSED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_CHECKED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_MIXED, pszDivide); //STATE_SYSTEM_INDETERMINATE
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_READONLY, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_HOTTRACKED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_DEFAULT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_EXPANDED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_COLLAPSED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_BUSY, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_FLOATING, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_MARQUEED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_ANIMATED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_INVISIBLE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_OFFSCREEN, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_SIZEABLE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_MOVEABLE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_SELFVOICING, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_FOCUSABLE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_SELECTABLE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_LINKED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_TRAVERSED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_MULTISELECTABLE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_EXTSELECTABLE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_ALERT_LOW, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_ALERT_MEDIUM, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_ALERT_HIGH, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, nFlags, STATE_SYSTEM_PROTECTED, pszDivide);

        FTLASSERT( 0 == nFlags);
        if (0 != nFlags)
        {
            FTLTRACEEX(FTL::tlWarning, TEXT("%s:GetSystemStateFlags Not Complete, total=0x%x, remain=0x%x\n"),
                __FILE__LINE__, oldFlags, nFlags);
        }

        return formater.GetString();
	}


	HRESULT CFEnumVARIANTDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("EnumVARIANT")));
		if (m_pObj)
		{
			CComQIPtr<IEnumVARIANT>     spEnumVariant(m_pObj);
			if (spEnumVariant)
			{
				COM_VERIFY(spEnumVariant->Reset());

				ULONG nFetched = 0;
				CComVariant varChild;
				//do 
				//{
				//	hr = spEnumVariant->Next(1, &varChild, &nFetched);
				//	//if (SUCCEEDED(hr) && varChild)
				//	//{
				//	//	//varChild
				//	//}

				//} while (TRUE);
			}
		}
		return hr;
	}

	HRESULT CFAccessibleDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("Accessible")));
		if (m_pObj)
		{
			CComQIPtr<IAccessible>     spAccessible(m_pObj);
			if (spAccessible)
			{
				CComVariant varSelfIndex((long)CHILDID_SELF, VT_I4);

				CComPtr<IDispatch> spDispParent;
				hr = spAccessible->get_accParent(&spDispParent);
				//if (spDispParent)
				//{
				//	COM_DETECT_INTERFACE_FROM_LIST(spDispParent);
				//}

				CComBSTR bstrName;
				hr = spAccessible->get_accName(varSelfIndex, &bstrName);
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Name"), &bstrName));

				CComBSTR bstrDescription;
				hr = spAccessible->get_accDescription(varSelfIndex, &bstrDescription);
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Description"), &bstrDescription));

				CComBSTR bstrValue;
				hr = spAccessible->get_accValue(varSelfIndex, &bstrValue);
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Value"), &bstrValue));

				CComVariant varState;
				hr = spAccessible->get_accState(varSelfIndex, &varState);
				if (SUCCEEDED(hr))
				{
					if (VT_I4 == varState.vt)
					{
						//STATE_SYSTEM_UNAVAILABLE =>  STATE_SYSTEM_ALERT_HIGH
						CFStringFormater formaterState;
						formaterState.Format(TEXT("(null)"));
						COM_VERIFY(pInfoOutput->OnOutput(TEXT("State String"), 
							CFWinInterfaceDetectUtil::GetSystemStateFlags(formaterState, varState.lVal)));
					}
				}
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("State"), &varState));

				long nLeft = 0;
				long nTop = 0;
				long nWidth = 0;
				long nHeight = 0;
				hr = spAccessible->accLocation(&nLeft, &nTop, &nWidth, &nHeight, varSelfIndex);

				CFStringFormater strLocationFormater;
				strLocationFormater.Format(TEXT("(%d,%d)-(%d,%d), %dx%d"), nLeft, nTop, nLeft + nWidth, nTop + nHeight,
					nWidth, nHeight);
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Location"), strLocationFormater.GetString()));

				CComVariant varRole;
				hr = spAccessible->get_accRole(varSelfIndex, &varRole);
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Role"), &varRole));

				long nChildCount = 0;
				spAccessible->get_accChildCount(&nChildCount);
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("ChildCount"), nChildCount));

				CComQIPtr<IEnumVARIANT>	spChildEnumVariant(spAccessible);
				if (spChildEnumVariant)
				{
					spChildEnumVariant->Reset();
				}

				for (long nChildIndex = 1; nChildIndex <= nChildCount; nChildIndex++)
				{
					COM_VERIFY(pInfoOutput->OnOutput(TEXT("Child Index"), nChildIndex ));

					ULONG nFetchCount = 0;
					// Get next child

					CComVariant varChildIndex(nChildIndex, VT_I4);
					if (spChildEnumVariant)
					{
						spChildEnumVariant->Next(1, &varChildIndex, &nFetchCount);
					}

					CComPtr<IDispatch> spDispChild;
					// Get IDispatch interface for the child
					if (varChildIndex.vt == VT_I4)
					{
						hr = spAccessible -> get_accChild(varChildIndex, &spDispChild);
					}
					else
					{
						spDispChild = varChildIndex.pdispVal;
					}
					CComPtr<IAccessible> spChildAccessible;

					if (spDispChild)
					{
						spDispChild->QueryInterface(IID_IAccessible, (void**)&spChildAccessible);
					}

					if (spDispChild)
					{
						CFAccessibleDumper childDumper(spDispChild, pInfoOutput, m_nIndent + 2);
					}
				}
			}
		}
		return hr;
	}
}

#endif //FTL_WIN_INTERFACE_DETECT_HPP
