#pragma once

class CScriptErrHandler
{
public:
	CScriptErrHandler(void);
	~CScriptErrHandler(void);

	HRESULT ShieldCurrPage(CComPtr<IHTMLDocument2> &spDoc);
	HRESULT ShieldAllChildPages(CComPtr<IHTMLDocument2> &parentDoc);
	HRESULT WalkAllChildPages(CComPtr<IHTMLDocument2> &parentDoc);
private:
	BSTR m_bstrScript;
};
