#include "stdafx.h"
#include "viewhexedit.h"

CViewHexEdit::CViewHexEdit(void)
{
}

CViewHexEdit::~CViewHexEdit(void)
{
}


BOOL CViewHexEdit::PreTranslateMessage(MSG* pMsg) 
{ 
    // ÆÁ±ÎÓÒ¼ü²Ëµ¥ 
    if (pMsg->message == WM_RBUTTONUP) { 
        return TRUE; 
    } 
    
    if((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
        (pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST)) 
    { 
        return FALSE; 
    } 
    
    // give HexEditAx Control a chance to translate this message
    return (BOOL)SendMessage(WM_FORWARDMSG, 0, (LPARAM)pMsg); 
} 

LRESULT CViewHexEdit::OnWmCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{ 
    uMsg, wParam, lParam, bHandled; 
	HRESULT hr = E_FAIL;

    CREATESTRUCT * pCreateStruct = reinterpret_cast<CREATESTRUCT *>(lParam); 

    LRESULT lRet = S_OK; 
    lRet = DefWindowProc(uMsg, wParam, lParam); 

    { 
        CComPtr<IUnknown> punkCtrl; 
        CComVariant v; 

        OLECHAR szGUID[MAX_PATH] = { 0 }; 
        StringFromGUID2(__uuidof(HexEditor::HexEdit), szGUID, ARR_SIZE(szGUID)); 

        // Create the AX host window. And Create the HexEdit control using its GUID. 
        HWND hHost = m_wndAxHost.Create ( m_hWnd, rcDefault, szGUID, WS_CHILD | WS_VISIBLE , 
            0, ID_DISPATCH_HEXEDIT ); 

		hr = m_wndAxHost.QueryControl(&m_spHexEdit);
		ATLASSERT( SUCCEEDED(hr) );

        // Begin sinking events 
        AtlAdviseSinkMap ( this, true ); 
    } 

    bHandled = FALSE; 

    return lRet; 
} 

LRESULT CViewHexEdit::OnWmDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
{
    uMsg, wParam, lParam, bHandled; 

    // Stop sinking events 
    AtlAdviseSinkMap ( this, false ); 

    m_wndAxHost.DestroyWindow(); 
	m_spHexEdit.Release();

    bHandled = FALSE; 

    return S_OK; 
} 

LRESULT CViewHexEdit::OnWmSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
{ 
    uMsg, wParam, lParam, bHandled; 

    m_wndAxHost.SetWindowPos(NULL, 0, 0, LOWORD(lParam), HIWORD(lParam), SWP_NOZORDER); 

    return S_OK; 
} 

LRESULT CViewHexEdit::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{ 
    BOOL& bShow = (BOOL&)wParam;

    bHandled = FALSE; 

    return S_OK;  // Let the system set the focus
} 

BOOL CViewHexEdit::SetReadOnly(BOOL bReadOnly) 
{ 
    HRESULT hr = E_FAIL; 
    if (m_spHexEdit) 
    { 
        hr = m_spHexEdit->put_ReadOnly(bReadOnly); 
    } 
    return SUCCEEDED(hr); 
} 

BOOL CViewHexEdit::SetData(const VARIANT & vData) 
{ 
    HRESULT hr = E_FAIL; 
    if (m_spHexEdit) 
    { 
        hr = m_spHexEdit->put_Data(vData); 
    } 
    return SUCCEEDED(hr); 
} 


//void __stdcall CViewHexEdit::SagePreviewOnClose() 
//{ 
//    ATLTRACE(_T("=================CViewSrcMonitor::SagePreviewOnClose=================\n")); 
//} 

//void __stdcall CViewHexEdit::SagePreviewOnPreviewReady() 
//{  
//    ATLTRACE(_T("=================CViewSrcMonitor::SagePreviewOnPreviewReady=================\n")); 
//} 

//void __stdcall CViewHexEdit::ShImgVwOnError() 
//{ 
//    ATLTRACE(_T("=================CViewSrcMonitor::ShImgVwOnError=================\n")); 
//} 

//void __stdcall CViewHexEdit::ShImgVwOnBestFitPress() 
//{ 
//    ATLTRACE(_T("=================CViewSrcMonitor::ShImgVwOnBestFitPress=================\n")); 
//} 

//void __stdcall CViewHexEdit::ShImgVwOnActualSizePress() 
//{ 
//    ATLTRACE(_T("=================CViewSrcMonitor::ShImgVwOnActualSizePress=================\n")); 
//} 
