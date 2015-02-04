// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	DoDataExchange(FALSE);

	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	return TRUE;
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

void CMainDlg::OnRefresh(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	m_RotObjects.ResetContent();

	HRESULT hr = E_FAIL;
	CComPtr<IRunningObjectTable>	spRunningObjectTable;
	COM_VERIFY(GetRunningObjectTable(0, &spRunningObjectTable));
	if (spRunningObjectTable)
	{
		CComPtr<IEnumMoniker>	spEnumMoniker;
		COM_VERIFY(spRunningObjectTable->EnumRunning(&spEnumMoniker));
		if (spEnumMoniker)
		{
			COM_VERIFY(spEnumMoniker->Reset());

			CComPtr<IBindCtx> spBindCtx;
			COM_VERIFY(CreateBindCtx(0, &spBindCtx));

			CComPtr<IMoniker> spMoniker;
			ULONG ulFetched = 0;
			while (S_OK == (hr = (spEnumMoniker->Next(1, &spMoniker, &ulFetched))))
			{
				FTLTRACE(TEXT("Enum Monitor 0x%x\n"), spMoniker);
				//CComBSTR bstrDisplayName;
				LPOLESTR pszDisplayName = NULL;
				COM_VERIFY(spMoniker->GetDisplayName(spBindCtx, NULL, &pszDisplayName));
				//CComPtr<IPropertyBag>	spPropertyBag;
				//COM_VERIFY(spMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&spPropertyBag));
				//if (SUCCEEDED(hr))
				//{
				//	CComVariant	varName;
				//	COM_VERIFY(spPropertyBag->Read(L"FriendlyName", &varName, NULL));
					if (SUCCEEDED(hr))
					{
						CString strInfo(pszDisplayName);
						m_RotObjects.AddString(strInfo);
						CoTaskMemFree(pszDisplayName);
					}
				//}
				spMoniker.Release();
			}
		}
	}
}
