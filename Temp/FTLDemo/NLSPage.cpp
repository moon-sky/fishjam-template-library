// NLSPage.cpp : 实现文件
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "NLSPage.h"
#include "ftlNLS.h"
#include "ftlConversion.h"

// CNLSPage 对话框
static CNLSPage* g_NlsPage = NULL;

IMPLEMENT_DYNAMIC(CNLSPage, CPropertyPage)

CNLSPage::CNLSPage()
	: CPropertyPage(CNLSPage::IDD)
{
}

CNLSPage::~CNLSPage()
{

}

void CNLSPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CODEPAGES, m_cmbCodePages);
}


BEGIN_MESSAGE_MAP(CNLSPage, CPropertyPage)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_CODEPAGE_SUPPORTED, &CNLSPage::OnBnClickedRadioCodepageSupported)
	ON_BN_CLICKED(IDC_RADIO_CODEPAGE_INSTALLED, &CNLSPage::OnBnClickedRadioCodepageInstalled)
END_MESSAGE_MAP()

BOOL CNLSPage::OnInitDialog()
{
	__super::OnInitDialog();

	//UpdateData(FALSE);

	_InitControls();

	return TRUE;
}

void CNLSPage::OnDestroy()
{
	g_NlsPage = NULL;
	__super::OnDestroy();
}

BOOL CALLBACK CNLSPage::_InnerCodePageEnumProc(LPTSTR lpCodePageString)
{
	if (g_NlsPage)
	{
		g_NlsPage->m_cmbCodePages.AddString(lpCodePageString);
	}
	
	return TRUE;
}

BOOL CNLSPage::_InitControls()
{
	BOOL bRet = TRUE;
	g_NlsPage = this;
	

	API_VERIFY(_RefreshCodePages(0));
	return bRet;
}

BOOL CNLSPage::_RefreshCodePages(DWORD dwFlags)
{
	BOOL bRet = FALSE;
	m_cmbCodePages.ResetContent();
	API_VERIFY(EnumSystemCodePages(_InnerCodePageEnumProc, dwFlags));

	return bRet;
}

void CNLSPage::OnBnClickedRadioCodepageSupported()
{
	_TestConverion();
	//枚举系统中支持的代码页
	//_RefreshCodePages(CP_SUPPORTED);
}

void CNLSPage::OnBnClickedRadioCodepageInstalled()
{
	//枚举系统中已安装的代码页
	_RefreshCodePages(CP_INSTALLED);
}

BOOL CNLSPage::_TestConverion()
{
	BOOL bRet = FALSE;
	CString strSource = TEXT("κλμξενφēéèīǖù");

	FTL::CFConversion conv2Utf8(CP_UTF8, 0);
	//conv2Utf8.SetDefaultCharForWC2MB(("x"));
	LPSTR pszUTF8 = conv2Utf8.TCHAR_TO_UTF8(strSource);

	FTL::CFConversion conv2TCHAR(CP_UTF8, 0);
	LPTSTR pszTCHAR = conv2TCHAR.UTF8_TO_TCHAR(pszUTF8);
	FTLTRACE(TEXT("src=%s, dst=%s\n"), strSource, pszTCHAR);

	FTLASSERT(strSource.Compare(pszTCHAR) == 0);

	return bRet;
}