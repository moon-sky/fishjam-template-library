// WowAHView.h : interface of the CWowAHView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CItemPageAnalyzeBase;

const int _nDispatchID = 1;


class CWowAHView : public CWindowImpl<CWowAHView, CAxWindow>
    , public IDispEventSimpleImpl<_nDispatchID, CWowAHView, &DIID_DWebBrowserEvents2>
{
public:
	CWowAHView();
	~CWowAHView();

	DECLARE_WND_SUPERCLASS(NULL, CAxWindow::GetWndClassName())

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//virtual void OnFinalMessage(HWND /*hWnd*/);

	BEGIN_MSG_MAP(CWowAHView)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
		MSG_WM_CLOSE(OnClose)
	END_MSG_MAP()

    // IDispatch events function info
    static _ATL_FUNC_INFO DocumentComplete2_Info;
	static _ATL_FUNC_INFO NavigateComplete2_Info;
	static _ATL_FUNC_INFO DownloadBegin_Info;

    BEGIN_SINK_MAP(CWowAHView)
        SINK_ENTRY_INFO(_nDispatchID, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnEventDocumentComplete, &DocumentComplete2_Info)
		SINK_ENTRY_INFO(_nDispatchID, DIID_DWebBrowserEvents2, DISPID_NAVIGATECOMPLETE2, OnNavigateComplete2, &NavigateComplete2_Info )
		SINK_ENTRY_INFO(_nDispatchID, DIID_DWebBrowserEvents2, DISPID_DOWNLOADBEGIN, OnDownloadBegin, &DownloadBegin_Info )
    END_SINK_MAP()

public:
	virtual STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
		DISPPARAMS FAR* pDispParams,VARIANT FAR* pVarResult,
		EXCEPINFO FAR* pExcepInfo, unsigned int FAR* puArgErr);

public:
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnClose();
	void OnDestroy();

public:
    HRESULT CheckElement();
	HRESULT SearchSpecialItem(const CString& strItemName);


    void __stdcall OnEventDocumentComplete(IDispatch* /*pDisp*/, VARIANT* URL);
	void __stdcall OnNavigateComplete2(IDispatch* /*pDisp*/, VARIANT* URL);
	void __stdcall OnDownloadBegin();

	CComPtr<IHTMLDocument3>	_GetDocument();
private:
	CItemPageAnalyzeBase*	m_pItemPageAnalyze;
};

