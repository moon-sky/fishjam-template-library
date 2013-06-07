#ifndef FTL_WEB_INTERFACE_DETECT_HPP
#define FTL_WEB_INTERFACE_DETECT_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlWebInterfaceDetect.h"
#endif


namespace FTL
{
	HRESULT CFWebBrowserUtil::FindChildElement(IHTMLElement* pParent, 
		FindElementMethod method, LPCTSTR pszName, long nStartIndex, RecursiveWay recurWay,
		IHTMLElement** ppFoundElement)
	{
		CHECK_POINTER_RETURN_VALUE_IF_FAIL(pParent, E_POINTER);
		CHECK_POINTER_WRITABLE_DATA_RETURN_VALUE_IF_FAIL(ppFoundElement, sizeof(IHTMLElement*), E_POINTER);

		HRESULT hr = E_FAIL;

		CComPtr<IDispatch> spDispChildred;
		COM_VERIFY(pParent->get_children(&spDispChildred));
		CComQIPtr<IHTMLElementCollection>	spElemCollection(spDispChildred);
		if (spElemCollection)
		{
			long nLength = 0;
			COM_VERIFY(spElemCollection->get_length(&nLength));
			for (long nIndex = nStartIndex; nIndex < nLength && (NULL == *ppFoundElement); ++nIndex)
			{
				CComVariant varIndex(nIndex);
				CComPtr<IDispatch> spDispChild;
				COM_VERIFY(spElemCollection->item(varIndex, varIndex, &spDispChild));
				CComQIPtr<IHTMLElement> spChildElement = spDispChild;
				if (spChildElement)
				{
					//CFHTMLElementDumper dumper(spChildElement, CFOutputWindowInfoOutput::Instance(), 0, nIndex);
					CComBSTR	bstrValue;
					switch (method)
					{
					case femById:
						COM_VERIFY(spChildElement->get_id(&bstrValue));
						break;
					case femByTag:
						COM_VERIFY(spChildElement->get_tagName(&bstrValue));
						break;
					case femByClass:
						break;
					}
					if (bstrValue)
					{
						CAtlString strValue = bstrValue;
						if (strValue.Compare(pszName) == 0)
						{
							//bFound
							*ppFoundElement = spChildElement.Detach();
							hr = S_OK;
							//跳出 for 循环的 break
							break;
						}
					}

					if (NULL == *ppFoundElement && recurWay == rwDepthFirst)
					{
						//深度优先递归
						hr = FindChildElement(spChildElement,  method, pszName, nStartIndex, recurWay, ppFoundElement);
					}
				}
			}

			if (NULL == *ppFoundElement && recurWay == rwBreadthFirst)
			{
				//广度优先遍历
				for (long nIndex = nStartIndex; nIndex < nLength && (NULL == *ppFoundElement); ++nIndex)
				{
					CComVariant varIndex(nIndex);
					CComPtr<IDispatch> spDispChild;
					COM_VERIFY(spElemCollection->item(varIndex, varIndex, &spDispChild));
					CComQIPtr<IHTMLElement> spChildElement = spDispChild;
					if (spChildElement)
					{
						hr = FindChildElement(spChildElement,  method, pszName, nStartIndex, recurWay, ppFoundElement);
					}
				}
			}
		}

		//没有找到对象，并且错误码是正确 
		if (NULL == *ppFoundElement && ( SUCCEEDED(hr)))
		{
			hr = HRESULT_FROM_WIN32(ERROR_NOT_FOUND);
		}

		return hr;
	}

	HRESULT CFWebBrowserDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;

		//可以QI到(>=37个)
		//	IPersistStreamInit/IPersistStream/IPersistStorage/IPersistPropertyBag/IPersistHistory/
		//	IViewObject2/IDataObject/IOleInPlaceObject/IOleInPlaceActiveObject
		//	IWebBrowser2/IWebBrowserApp/IWebBrowserPriv2/IServiceProvider/IProvideClassInfo2
		//	IEFrameAuto/IUrlHistoryNotify/ITargetEmbedding/IXMicTestMode
		//	ITargetNotify/IConnectionPointContainer/IOleCommandTarget/ISecMgrCacheSeedTarget
		//	等
		//连接点有：DWebBrowserEvents2/DWebBrowserEvents/IPropertyNotifySink
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("WebBrowser")));
		if (m_pObj)
		{
			CComQIPtr<IWebBrowser>     spWebBrowser(m_pObj);
			if (spWebBrowser)
			{
				CComPtr<IDispatch> spDispDocument;
				COM_VERIFY(spWebBrowser->get_Document(&spDispDocument));

				CComQIPtr<IHTMLDocument, &IID_IHTMLDocument> spDocument ( spDispDocument );
				if (spDocument)
				{
					CFHTMLDocumentDumper htmlDocumentDumper(spDocument, pInfoOutput, m_nIndent + 2);
				}
			}
		}
		return hr;
	}


	HRESULT CFHTMLDocumentDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;

		//可以QI到(>=289个)
		//	IPersistFile/DataSource/IPersistMoniker/IDispatchEx
		//	IOleDocument/IOleDocumentView/IOleInPlaceActiveObject/IParseDisplayName/IOleControl
		//	IOleItemContainer/DispDOMEvent/HTMLWindowEvents3/IDocumentEvent
		//	IHTMLDocument7/IDocumentRange/DispHTMLStyle
		//	DispHTMLWindow2/DispHTMLBody/DispHTMLAnchorElement/DispHTMLDDElement/DispHTMLIFrame
		//	DispHTMLTable/DispHTMLFrameBase/DispHTMLHistory/DispHTMLStyleSheetRulesCollection
		//	DispCPlugins/DispHTMLLocation/DispHTMLScreen
		//	DispDOMParser/IObjectWithSite/IObjectIdentity
		//	等
		//连接点有：IPropertyNotifySink/HTMLDocumentEvents4/ITridentEventSink
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("HTMLDocument")));
		if (m_pObj)
		{
			CComQIPtr<IHTMLDocument>	spHTMLDocument(m_pObj);
			if (spHTMLDocument)
			{
				CComPtr<IDispatch> spDispScript;
				COM_VERIFY(spHTMLDocument->get_Script(&spDispScript));
				//COM_DETECT_INTERFACE_FROM_REGISTER(spDispScript);
				//COM_DETECT_INTERFACE_FROM_LIST(spDispScript);
			}

			CComQIPtr<IHTMLDocument2>	spHTMLDocument2(m_pObj);
			if (spHTMLDocument2)
			{
				CComPtr<IHTMLWindow2>	spHTMLWindow;
				COM_VERIFY(spHTMLDocument2->get_parentWindow(&spHTMLWindow));
				if (spHTMLWindow)
				{
					CFHTMLWindowDumper	htmlWindowDumper(spHTMLWindow, pInfoOutput, m_nIndent);
				}

				CComPtr<IHTMLElement>	spBodyHtmlElement;
				COM_VERIFY(spHTMLDocument2->get_body(&spBodyHtmlElement));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("HTMLDocument Body")));
				if (spBodyHtmlElement)
				{
					CFHTMLElementDumper	bodyHtmlElementDumper(spBodyHtmlElement, pInfoOutput, m_nIndent + 2);
				}

				CComPtr<IHTMLElementCollection>	spHtmlElementCollection;
				COM_VERIFY(spHTMLDocument2->get_all(&spHtmlElementCollection));
				if (spHtmlElementCollection)
				{
					CFHTMLElementCollectionDumper	htmlElementCollectionDumper(spHtmlElementCollection, 
						pInfoOutput, m_nIndent + 2);
				}
			}

			//CComQIPtr<IHTMLDocument6>     spHTMLDocument6(m_pObj);
			//if (spHTMLDocument6)
			//{
			//	
			//}

		}
		return hr;
	}

	HRESULT CFHTMLWindowDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		//可以QI到()
		//	等
		//连接点有：
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("HTMLWindow2")));
		if (m_pObj)
		{
			CComQIPtr<IHTMLWindow2>	spHTMLWindow2(m_pObj);
			if (spHTMLWindow2)
			{
				CComBSTR bstrName;
				COM_VERIFY(spHTMLWindow2->get_name(&bstrName));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("HTML Window Name"), &bstrName));


				CComPtr<IHTMLEventObj>	spHtmlEventObj;
				COM_VERIFY(spHTMLWindow2->get_event(&spHtmlEventObj));
				if (spHtmlEventObj)
				{
					CFHTMLEventObjDumper htmlEventObjDumper(spHtmlEventObj, pInfoOutput, m_nIndent + 2);
				}
			}
		}
		return hr;

	}

	HRESULT CFHTMLEventObjDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		//可以QI到()
		//	等
		//连接点有：
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("HTMLEventObj")));
		if (m_pObj)
		{
			CComQIPtr<IHTMLEventObj>	spHtmlEventObj(m_pObj);
			if (spHtmlEventObj)
			{
				
			}
		}
		return hr;
	}


	HRESULT CFHTMLStyleDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		//可以QI到()
		//	等
		//连接点有：
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("HTMLStyle")));
		if (m_pObj)
		{
			CComQIPtr<IHTMLStyle>	spHtmlStyle(m_pObj);
			if (spHtmlStyle)
			{
				CComBSTR	bstrBackGround;
				COM_VERIFY(spHtmlStyle->get_background(&bstrBackGround));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("background"), &bstrBackGround));

				CComBSTR	bstrTextAlign;
				COM_VERIFY(spHtmlStyle->get_textAlign(&bstrTextAlign));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("textAlign"), &bstrTextAlign));
			}
		}
		return hr;
	}

	HRESULT CFHTMLElementDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		//可以QI到()
		//	等
		//连接点有：
		if (m_nParam != INVALID_INTERFACE_DUMPER_PARAM)
		{
			CFStringFormater strNameFormater;
			strNameFormater.Format(TEXT("HTMLElement %d"), (long)m_nParam);
			COM_VERIFY(pInfoOutput->OutputInfoName(strNameFormater.GetString()));
		}
		else
		{
			COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("HTMLElement")));
		}
		if (m_pObj)
		{
			CComQIPtr<IHTMLElement>		spHTMLElement(m_pObj);
			if (spHTMLElement)
			{
				CComBSTR	bstrTagName;
				COM_VERIFY(spHTMLElement->get_tagName(&bstrTagName));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("TagName"), &bstrTagName));

				long nLeft = 0, nTop = 0, nWidth = 0, nHeight = 0;
				COM_VERIFY(spHTMLElement->get_offsetLeft(&nLeft));
				COM_VERIFY(spHTMLElement->get_offsetTop(&nTop));
				COM_VERIFY(spHTMLElement->get_offsetWidth(&nWidth));
				COM_VERIFY(spHTMLElement->get_offsetHeight(&nHeight));
				FTL::CFStringFormater	strFormaterOffset;
				COM_VERIFY(strFormaterOffset.Format(TEXT("(%d,%d)-(%d,%d), %dx%d"),
					nLeft, nTop, nLeft + nWidth, nTop + nHeight,
					nWidth, nHeight));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Offset"), strFormaterOffset.GetString()));

#if 1
				CComBSTR	bstrToString;
				COM_VERIFY(spHTMLElement->toString(&bstrToString));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("toString"), &bstrToString));

				//Inner 不含本Tag的内容，Outer 包含本Tag和其内部的内容
				//HTML  带HTML标志的全文本， Text 只包含内容
				//总结：OuterHTML 信息最全
				CComBSTR	bstrInnerHTML;
				COM_VERIFY(spHTMLElement->get_innerHTML(&bstrInnerHTML));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("InnerHTML"), &bstrInnerHTML));

				CComBSTR	bstrInnerText;
				COM_VERIFY(spHTMLElement->get_innerText(&bstrInnerText));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("InnerText"), &bstrInnerText));

				CComBSTR	bstrOuterHTML;
				COM_VERIFY(spHTMLElement->get_outerHTML(&bstrOuterHTML));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("OuterHTML"), &bstrOuterHTML));

				CComBSTR	bstrOuterText;
				COM_VERIFY(spHTMLElement->get_outerHTML(&bstrOuterText));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("OuterText"), &bstrOuterText));

				CComBSTR	bstrClassName;
				COM_VERIFY(spHTMLElement->get_className(&bstrClassName));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("ClassName"), &bstrClassName));

				CComBSTR	bstrTitle;
				COM_VERIFY(spHTMLElement->get_title(&bstrTitle));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Title"), &bstrTitle));

				CComBSTR	bstrID;
				COM_VERIFY(spHTMLElement->get_id(&bstrID));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Id"), &bstrID));

				CComBSTR	bstrLang;
				COM_VERIFY(spHTMLElement->get_lang(&bstrLang));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("lang"), &bstrLang));

				CComBSTR	bstrLanguage;
				COM_VERIFY(spHTMLElement->get_language(&bstrLanguage));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("language"), &bstrLanguage));
#endif

#if 0
				CComPtr<IHTMLStyle>		spHtmlStyle;
				COM_VERIFY(spHTMLElement->get_style(&spHtmlStyle));
				if (spHtmlStyle)
				{
					CFHTMLStyleDumper	styleDumper(spHtmlStyle, pInfoOutput, m_nIndent + 2);
				}
#endif 
			}
		}
		return hr;
	}

	HRESULT CFHTMLElementCollectionDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;

		//可以QI到()
		//	等
		//连接点有：
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("HTMLElementCollection")));
		if (m_pObj)
		{
			CComQIPtr<IHTMLElementCollection>	spHTMLElementCollection(m_pObj);
			if (spHTMLElementCollection)
			{
				long nLength = 0;
				COM_VERIFY(spHTMLElementCollection->get_length(&nLength));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("HTML Element Collection Length"), nLength));

				for (long nIndex = 0; nIndex < nLength; nIndex++)
				{
					CComPtr<IDispatch>	spDispItem;
					CComVariant varIndex(nIndex);
					COM_VERIFY(spHTMLElementCollection->item(varIndex, varIndex, &spDispItem));
					CComQIPtr<IHTMLElement> spElementItem = spDispItem;
					if (spElementItem)
					{
						CFHTMLElementDumper htmlElementDumper(spElementItem, pInfoOutput, m_nIndent + 2, nIndex);
					}
				}
			}
		}
		return hr;
	}


	HRESULT CFHTMLTableDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		//可以QI到()
		//	等
		//连接点有：
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("HTMLTable")));
		if (m_pObj)
		{
			CComQIPtr<IHTMLTable>	spTable(m_pObj);
			if (spTable)
			{
				CComBSTR	bstrFrame;
				COM_VERIFY(spTable->get_frame(&bstrFrame));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("frame"), &bstrFrame));

				long nCols = 0;
				COM_VERIFY(spTable->get_cols(&nCols));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("cols"), nCols));

				CComPtr<IHTMLElementCollection>	spRows;
				COM_VERIFY(spTable->get_rows(&spRows));
				if (spRows)
				{
					long nRows = 0;
					COM_VERIFY(spRows->get_length(&nRows));
					COM_VERIFY(pInfoOutput->OnOutput(TEXT("rows Count"), nRows));
					if (nRows > 0)
					{
						CFHTMLElementCollectionDumper	spRowsDumper(spRows, pInfoOutput, m_nIndent + 2);
					}
				}
			}
		}
		return hr;
	}

	HRESULT CFHTMLTableRowDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		//可以QI到()
		//	等
		//连接点有：
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("HTMLTableRow")));
		if (m_pObj)
		{
			CComQIPtr<IHTMLTableRow>	spTableRow(m_pObj);
			if (spTableRow)
			{
				long nRowIndex = 0;
				COM_VERIFY(spTableRow->get_rowIndex(&nRowIndex));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("rowIndex"), nRowIndex));
			}
		}
		return hr;
	}
}

//#include <MsHTML.h>

#endif //FTL_WEB_INTERFACE_DETECT_HPP
