#include "StdAfx.h"
#include "ScriptErrHandler.h"

CScriptErrHandler::CScriptErrHandler(void)
{
	CString strJavaScriptCode = TEXT("function fnOnError(msg,url,lineno){alert('script error:\\n\\nURL:'+url")
		TEXT("+'\\n\\nMSG:'+msg +'\\n\\nLine:'+lineno+'\\n\\nframes:' + window.frames.length);return true;}window.onerror=fnOnError;");
	//屏蔽的脚本,可以改进为从文本里读取
	m_bstrScript = strJavaScriptCode.AllocSysString();
}

CScriptErrHandler::~CScriptErrHandler(void)
{
	SysFreeString(m_bstrScript);
}

HRESULT CScriptErrHandler::ShieldCurrPage(CComPtr<IHTMLDocument2> &spDoc)
{
	HRESULT hr = E_FAIL;

	CComPtr<IHTMLWindow2>  spIhtmlwindow2;
	COM_VERIFY(spDoc->get_parentWindow(&spIhtmlwindow2));
	if(spIhtmlwindow2 != NULL)
	{
		CComBSTR bstrLanguage(TEXT("JavaScript"));
		CComVariant varRet;

		//把window.onerror函数插入入当前页面中去
		COM_VERIFY(spIhtmlwindow2->execScript(m_bstrScript, bstrLanguage, &varRet));
	}
	return hr;
}

HRESULT CScriptErrHandler::ShieldAllChildPages(CComPtr<IHTMLDocument2> &parentDoc)
{
	HRESULT hr = E_FAIL;
	COM_VERIFY(WalkAllChildPages(parentDoc));
	return hr;
}

HRESULT CScriptErrHandler::WalkAllChildPages(CComPtr<IHTMLDocument2> &parentDoc)
{
	HRESULT hr = E_FAIL;
	CComPtr<IHTMLFramesCollection2> spFramesCol;
	COM_VERIFY(parentDoc->get_frames(&spFramesCol));
	if(SUCCEEDED(hr) && spFramesCol != NULL)
	{
		long lSize = 0;
		COM_VERIFY(spFramesCol->get_length(&lSize));
		for(long i=0; i<lSize; i++)
		{
			CComVariant varIndex(i);
			CComVariant varItem;
			COM_VERIFY(spFramesCol->item(&varIndex, &varItem));
			if(varItem.pdispVal != NULL)
			{
				CComQIPtr<IHTMLWindow2> spChildWindow = varItem.pdispVal;
				if (spChildWindow)
				{
					CComPtr<IHTMLDocument2> spChildDocument;
					COM_VERIFY(spChildWindow->get_document(&spChildDocument));
					if(spChildDocument)
					{
						COM_VERIFY(ShieldCurrPage(spChildDocument));
						COM_VERIFY(WalkAllChildPages(spChildDocument));
					}
				}
			}
		}
	}
	return hr;
}
