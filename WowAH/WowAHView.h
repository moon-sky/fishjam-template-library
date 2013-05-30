// WowAHView.h : interface of the CWowAHView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CItemPageAnalyzeBase;
class CWowItemManager;

const int _nDispatchID = 1;


class CWowAHView : public CWindowImpl<CWowAHView, CAxWindow>
    , public IDispEventSimpleImpl<_nDispatchID, CWowAHView, &DIID_DWebBrowserEvents2>
{
public:
	CWowAHView();
	~CWowAHView();

	HRESULT Navigate(const CString& strURL);

	enum ParsePageType
	{
		pptInvalid = -1,

		pptSellerBrowse,	//购买 -- 浏览拍卖
		pptCreateMyAuction,	//出售 -- 开始拍卖
		pptMyBit,			//竞标 -- 我的竞标
		pptMyAuction,		//拍卖 -- 我的拍卖

		pptCount,		//页面总数
	};
	DECLARE_WND_SUPERCLASS(NULL, CAxWindow::GetWndClassName())

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//virtual void OnFinalMessage(HWND /*hWnd*/);

	BEGIN_MSG_MAP_EX(CWowAHView)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_DESTROY(OnDestroy)
		MSG_WM_CLOSE(OnClose)
		COMMAND_ID_HANDLER_EX(ID_AH_GOTOPAGE, OnGotoPage)
		COMMAND_ID_HANDLER_EX(ID_AH_PARSE_SELLER_BROWSE, OnParseSellerBrowse)
		COMMAND_ID_HANDLER_EX(ID_AH_PARSE_CREATE_MY_AUCTION, OnParseCreateMyAuction)
		COMMAND_ID_HANDLER_EX(ID_AH_PARSE_MY_BID, OnParseMyBid)
		COMMAND_ID_HANDLER_EX(ID_AH_PARSE_MY_AUCTION, OnParseMyAuction)
		COMMAND_ID_HANDLER_EX(ID_AH_SEARCH_SPECIAL_ITEM, OnSearchSpecialItem)
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
	ParsePageType	m_ParsePageType;
public:
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnClose();
	void OnDestroy();

	void OnGotoPage(UINT uNotifyCode, int nID, CWindow wndCtl);
	
	void OnParseSellerBrowse(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnParseCreateMyAuction(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnParseMyBid(UINT uNotifyCode, int nID, CWindow wndCtl);
	void OnParseMyAuction(UINT uNotifyCode, int nID, CWindow wndCtl);

	void OnSearchSpecialItem(UINT uNotifyCode, int nID, CWindow wndCtl);
public:
	HRESULT SearchSpecialItem(const CString& strItemName);


	void _OpenLocalFileAndParse(ParsePageType pageType);
    void __stdcall OnEventDocumentComplete(IDispatch* /*pDisp*/, VARIANT* URL);
	void __stdcall OnNavigateComplete2(IDispatch* /*pDisp*/, VARIANT* URL);
	void __stdcall OnDownloadBegin();

	CComPtr<IHTMLDocument3>	_GetDocument();
private:
	CItemPageAnalyzeBase*	m_pItemPageAnalyzes[pptCount];
	CWowItemManager*		m_pWowItemManager;
};

