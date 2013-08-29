// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"
#include "../ComicService/ComicService_i.c"
#include "../ComicHelper/ComicHelper.h"

//#include <ftlComDetect.h>
//#include <ftlWindow.h>
//#include <ftlDebug.h>

CMainDlg::CMainDlg()
{
    m_bHookInDll = FALSE;
    m_hWndNextViewer = NULL;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // center the dialog on the screen
    CenterWindow();

    // set icons
    HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
    SetIcon(hIconSmall, FALSE);

    CoInitialize(NULL);
    return TRUE;
}

void CMainDlg::OnDestroy()
{
    ::ChangeClipboardChain(m_hWnd, m_hWndNextViewer);
    if (m_hWndNextViewer)
    {
        m_hWndNextViewer = NULL;
    }

    if (m_bHookInDll)
    {
        m_bHookInDll = FALSE;
        DisableWindowProtected(m_hWnd);
    }
    if (m_spComicService)
    {
        m_spComicService.Release();
    }
    CoUninitialize();
}


LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
    dlg.DoModal();
    return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // TODO: Add validation code 
    EndDialog(wID);
    return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EndDialog(wID);
    return 0;
}

void CMainDlg::OnBtnInitService(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    HRESULT hr = E_FAIL;
    COM_VERIFY(m_spComicService.CoCreateInstance(CLSID_ComicServiceObj, NULL, CLSCTX_LOCAL_SERVER));
    if (m_spComicService)
    {
        //COM_DETECT_INTERFACE_FROM_REGISTER(m_spComicService);
    }
    BOOL bEnabled = SUCCEEDED(hr);
    ::EnableWindow(GetDlgItem(IDC_BTN_INIT_SERVICE), !bEnabled);
    ::EnableWindow(GetDlgItem(IDC_BTN_SERVICE_HOOK), bEnabled);
    ::EnableWindow(GetDlgItem(IDC_BTN_SERVICE_UNHOOK), bEnabled);
}

void CMainDlg::OnBtnServiceHook(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    HRESULT hr = E_FAIL;
    //CComQIPtr<IComicServiceObj> spComicServiceOb(m_spComicService);
    if (m_spComicService)
    {
        COM_VERIFY(m_spComicService->ProtectWnd((OLE_HANDLE)m_hWnd, RGB(255, 0, 0), CComBSTR(TEXT("Copy Protected"))));
    }
}

void CMainDlg::OnBtnServiceUnHook(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    HRESULT hr = E_FAIL;
    //CComQIPtr<IComicServiceObj> spComicServiceOb(m_spComicService);
    if (m_spComicService)
    {
        COM_VERIFY(m_spComicService->UnProtectWnd((OLE_HANDLE)m_hWnd));
    }
}

void CMainDlg::OnBtnFinService(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    if (m_spComicService)
    {
        m_spComicService.Release();
        ::EnableWindow(GetDlgItem(IDC_BTN_INIT_SERVICE), TRUE);
        ::EnableWindow(GetDlgItem(IDC_BTN_SERVICE_HOOK), FALSE);
        ::EnableWindow(GetDlgItem(IDC_BTN_SERVICE_UNHOOK), FALSE);
        ::EnableWindow(GetDlgItem(IDC_BTN_FIN_SERVICE), FALSE);
    }
}

void CMainDlg::OnBtnDllHook(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    m_bHookInDll = TRUE;
    EnableWindowProtected(GetCurrentProcessId(), m_hWnd);
}

void CMainDlg::OnBtnDllUnHook(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    DisableWindowProtected(m_hWnd);
    m_bHookInDll = FALSE;
}


void CMainDlg::OnBtnEnableClipboardViewer(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    BOOL bRet = FALSE;
    if (NULL == m_hWndNextViewer)
    {
        m_hWndNextViewer = ::SetClipboardViewer(m_hWnd);
        API_VERIFY_EXCEPT1(m_hWndNextViewer != NULL, ERROR_SUCCESS);
    }
}

void CMainDlg::OnBtnDisableClipboardViewer(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    ::ChangeClipboardChain(m_hWnd, m_hWndNextViewer);
    if (m_hWndNextViewer)
    {
        m_hWndNextViewer = NULL;
    }
}

void CMainDlg::OnChangeCbChain(CWindow wndRemove, CWindow wndAfter)
{
    if (wndRemove == m_hWndNextViewer) 
    {
        m_hWndNextViewer = wndAfter; 
    }
    // Otherwise, pass the message to the next link. 
    else if (m_hWndNextViewer != NULL) 
    {
        SetMsgHandled(FALSE);
        //::SendMessage(m_hWndNextViewer, WM_CHANGECBCHAIN, (WPARAM)wndRemove.m_hWnd, (LPARAM)wndAfter.m_hWnd); 
    }
}

void CMainDlg::OnDrawClipboard()
{
    BOOL bRet = FALSE;
    //HWND hWndOwner = GetClipboardOwner();
    //if (hWndOwner)
    {
        //FTL::CFWindowDumpInfo dump(hWndOwner);
        //FTLTRACE(TEXT("%s\n"), dump.GetConvertedInfo());
        //TCHAR szClassName[FTL_MAX_CLASS_NAME_LENGTH] = {0};
        //GetClassName(hWndOwner, szClassName, _countof(szClassName));
        //if (lstrcmpi(_T("#32769"), szClassName) == 0)
        {
            //Desktop
            API_VERIFY(OpenClipboard());
            if (bRet)
            {
#if 1
                int nItem = 0;
                CString strFormat;
                UINT uFormat = 	EnumClipboardFormats(0); 
                //TCHAR szFormatName[80] = {0};
                while (uFormat) 
                { 
                    if (uFormat == CF_BITMAP || uFormat == CF_DIB || uFormat == CF_DIBV5)
                    {
                        FTLTRACE(TEXT("Will EmptyClipboard %d\n"), uFormat);
                        API_VERIFY(EmptyClipboard());
                        break;
                    }
                    uFormat = EnumClipboardFormats(uFormat); 
                }
#endif 
                API_VERIFY(CloseClipboard());
            }

        }
    }

}
LRESULT CMainDlg::OnBnClickedBtnInlineHook(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    HookApi();
    return 0;
}

LRESULT CMainDlg::OnBnClickedBtnInlineUnhook(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    UnHookApi();
    return 0;
}

LRESULT CMainDlg::OnBnClickedBtnDllHook(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    return 0;
}
