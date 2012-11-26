// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__B57241F7_EE1F_416A_9475_84CECA52B793__INCLUDED_)
#define AFX_CHILDFRM_H__B57241F7_EE1F_416A_9475_84CECA52B793__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000 

#include "viewfiledetail.h" 

#define IDC_ORGANIZE    1000 
#define IDC_HEXED_HOST  1001 

class CEmptyWindow : public CWindowImpl<CEmptyWindow> 
{ 
public: 
    BEGIN_MSG_MAP(CEmptyWindow) 
        MESSAGE_HANDLER(WM_CREATE, OnWmCreate) 
        MESSAGE_HANDLER(WM_PAINT, OnWmPaint) 
    END_MSG_MAP() 
 
    LRESULT OnWmCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
    { 
        uMsg, wParam, lParam, bHandled; 
        bHandled = FALSE; 
        return 0; 
    } 

    LRESULT OnWmPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
    { 
        CPaintDC dc(m_hWnd); 

        dc.SelectFont(AtlGetDefaultGuiFont()); 

        RECT rc = { 0 }; 
        GetClientRect(&rc); 
        LPCTSTR str = _T("No Data"); 
        dc.DrawText(str, lstrlen(str), &rc, DT_VCENTER|DT_CENTER|DT_SINGLELINE); 
        return 0; 
    } 
}; 


class CChildFrame 
    : public CMDIChildWindowImpl<CChildFrame> 
    , public CSplitterImpl< CChildFrame , true > 
{ 
    typedef CSplitterImpl< CChildFrame , true >   splitterClass; 

    static HRESULT _RecursionExtractData(LPCTSTR lpszRootPath, HTREEITEM hCurrItem, CTreeViewCtrlEx * pTreeView); 

public:
    DECLARE_FRAME_WND_CLASS(NULL, IDR_MDICHILD) 

    CChildFrame(CUpdateUIBase* pUpdate); 
    CUpdateUIBase * m_pUpdateUI; 


    CTreeViewCtrlExWindow m_wndCatalog; 

    CViewHexEdit m_wndHexEdit; 
    // CEmptyWindow m_wndEmpty; 
    CViewFileDetail m_wndFileDetails; 

    virtual void OnFinalMessage(HWND); 
    void UpdateLayout(BOOL bResizeBars=TRUE); 

    BEGIN_MSG_MAP(CChildFrame) 
        CHAIN_MSG_MAP(splitterClass) 
        MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg) 
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground) 
        MESSAGE_HANDLER(WM_CREATE, OnCreate) 
        MESSAGE_HANDLER(WM_CUSTOM_POSTCREATE, OnCustomPostCreate) 
        MESSAGE_HANDLER(WM_CUSTOM_ACTIVE_ITEM, OnCustomActiveItem) 
        MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
        MESSAGE_HANDLER(WM_MDIACTIVATE,     OnWmMDIActivate) 

		COMMAND_ID_HANDLER(ID_FILE_EXTRACTDATA, OnFileExtractData) 
		COMMAND_ID_HANDLER(ID_FILE_ADD_STG, OnFileAddStorage) 
		COMMAND_ID_HANDLER(ID_FILE_ADD_STM, OnFileAddStream) 
		COMMAND_ID_HANDLER(ID_FILE_DEL_NODE, OnFileDelNode) 

        NOTIFY_HANDLER(IDC_ORGANIZE, TVN_DELETEITEM,    OnTvnDeleteItem) 
        NOTIFY_HANDLER(IDC_ORGANIZE, TVN_SELCHANGED,    OnTvnSelChanged) 

        CHAIN_CLIENT_COMMANDS() 
        CHAIN_MSG_MAP(CMDIChildWindowImpl<CChildFrame>) 
        REFLECT_NOTIFICATIONS() 
    END_MSG_MAP() 
    
    CTreeItem m_pData; 

    TCHAR m_szFileName[MAX_PATH]; 
    void SetFileName(LPCTSTR lpszFileName); 

    BOOL OpenCompositeDocument(LPCTSTR lpszFileName); 

    LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
    LRESULT OnForwardMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
    LRESULT OnCustomPostCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
    LRESULT OnCustomActiveItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
	LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
    LRESULT OnWmMDIActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 

	LRESULT OnFileExtractData(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled); 
	LRESULT OnFileAddStorage(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled); 
	LRESULT OnFileAddStream(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled); 
	LRESULT OnFileDelNode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled); 

    LRESULT OnTvnDeleteItem(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled); 
    LRESULT OnTvnSelChanged(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled); 

};


#endif // !defined(AFX_CHILDFRM_H__B57241F7_EE1F_416A_9475_84CECA52B793__INCLUDED_)
