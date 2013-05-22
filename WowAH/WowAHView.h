// WowAHView.h : interface of the CWowAHView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

const int _nDispatchID = 1;

class CWowAHView : public CWindowImpl<CWowAHView, CAxWindow>
    , public IDispEventSimpleImpl<_nDispatchID, CWowAHView, &DIID_DWebBrowserEvents2>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CAxWindow::GetWndClassName())

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CWowAHView)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
	END_MSG_MAP()

    // IDispatch events function info
    static _ATL_FUNC_INFO DocumentComplete2_Info;


    BEGIN_SINK_MAP(CWowAHView)
        SINK_ENTRY_INFO(_nDispatchID, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnEventDocumentComplete, &DocumentComplete2_Info)
    END_SINK_MAP()

public:
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
    void OnDestroy();

public:
    HRESULT CheckElement();
    HRESULT CheckDocument(CComQIPtr<IHTMLDocument3>& spDoc);

    void __stdcall OnEventDocumentComplete(IDispatch* /*pDisp*/, VARIANT* URL);
};

