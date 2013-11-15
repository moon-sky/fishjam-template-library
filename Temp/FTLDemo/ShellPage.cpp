// ShellPage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "ShellPage.h"
#include <ftlShell.h>

// CShellPage dialog

IMPLEMENT_DYNAMIC(CShellPage, CPropertyPage)

CShellPage::CShellPage()
	: CPropertyPage(CShellPage::IDD)
{

}

CShellPage::~CShellPage()
{
}

void CShellPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CShellPage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_SHELL_EXECUTE, &CShellPage::OnBnClickedBtnShellExecute)

END_MESSAGE_MAP()

BOOL CShellPage::OnInitDialog()
{
    BOOL bRet = TRUE;
    return bRet;
}
// CShellPage 消息处理程序

void CShellPage::OnBnClickedBtnShellExecute()
{
    HRESULT hr = S_OK;
	CString strFilePath;
	GetDlgItemText(IDC_EDIT_SHELL_FILE, strFilePath);
	if (strFilePath.IsEmpty())
	{
		AfxMessageBox(IDS_SHELL_NEED_INPUT_FILEPATH);
		return;
	}

    //CComPtr<IShellBrowser> spShellBrowser;
    //CComPtr<IShellFolder> spDesktop;
    //COM_VERIFY(::SHGetDesktopFolder(&spDesktop));
    //if (spDesktop)
    {
        LPITEMIDLIST pidl = NULL;
        //ULONG attributes = 0;
        SFGAOF sfgaofIn = 0, sfgaofOut = 0;

		PTSTR pszBuf = strFilePath.GetBuffer();
        COM_VERIFY(SHParseDisplayName(pszBuf, NULL, &pidl, sfgaofIn, &sfgaofOut));
		strFilePath.ReleaseBuffer();

        //COM_VERIFY(spDesktop->ParseDisplayName(m_hWnd, NULL, 
        //        (LPTSTR)(LPCTSTR)strSelectFile,  
        //    NULL, &pidl, &attributes));
        if (SUCCEEDED(hr))
        {

			//打开目录并选中指定的文件
            COM_VERIFY(SHOpenFolderAndSelectItems(pidl, 0, NULL, 0));
        }
    }
}