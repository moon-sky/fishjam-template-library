// GuiPage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "GdiPage.h"
#include <afxwin.h>
#include <ftlGdi.h>
#include "CheckComboBox.h"

// CGdiPage dialog

IMPLEMENT_DYNAMIC(CGdiPage, CPropertyPage)

CGdiPage::CGdiPage()
	: CPropertyPage(CGdiPage::IDD)
{

}

CGdiPage::~CGdiPage()
{
}

void CGdiPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STOCK_OBJECT, m_cmbStockObject);

}

BEGIN_MESSAGE_MAP(CGdiPage, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_STOCK_OBJECT, &CGdiPage::OnCbnSelchangeComboStockObject)
END_MESSAGE_MAP()

BOOL CGdiPage::OnInitDialog()
{
	UpdateData(FALSE);

	_InitControls();


	return TRUE;
}
// CGdiPage 消息处理程序

BOOL CGdiPage::_InitControls()
{
	BOOL bRet = TRUE;

	ADD_COMBO_VALUE_STRING(m_cmbStockObject, WHITE_BRUSH | HOLLOW_BRUSH);
	ADD_COMBO_VALUE_STRING(m_cmbStockObject, LTGRAY_BRUSH);
	ADD_COMBO_VALUE_STRING(m_cmbStockObject, GRAY_BRUSH);
	ADD_COMBO_VALUE_STRING(m_cmbStockObject, DKGRAY_BRUSH);
	ADD_COMBO_VALUE_STRING(m_cmbStockObject, BLACK_BRUSH);
	ADD_COMBO_VALUE_STRING(m_cmbStockObject, NULL_BRUSH);
	ADD_COMBO_VALUE_STRING(m_cmbStockObject, WHITE_PEN);
	ADD_COMBO_VALUE_STRING(m_cmbStockObject, BLACK_PEN);
	ADD_COMBO_VALUE_STRING(m_cmbStockObject, NULL_PEN);
	ADD_COMBO_VALUE_STRING(m_cmbStockObject, OEM_FIXED_FONT);
	ADD_COMBO_VALUE_STRING(m_cmbStockObject, ANSI_FIXED_FONT);
	ADD_COMBO_VALUE_STRING(m_cmbStockObject, ANSI_VAR_FONT);
	ADD_COMBO_VALUE_STRING(m_cmbStockObject, SYSTEM_FONT);
	ADD_COMBO_VALUE_STRING(m_cmbStockObject, DEVICE_DEFAULT_FONT);
	ADD_COMBO_VALUE_STRING(m_cmbStockObject, DEFAULT_PALETTE);
	ADD_COMBO_VALUE_STRING(m_cmbStockObject, SYSTEM_FIXED_FONT);
	ADD_COMBO_VALUE_STRING(m_cmbStockObject, DEFAULT_GUI_FONT);
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN2K)
	ADD_COMBO_VALUE_STRING(m_cmbStockObject, DC_BRUSH);
	ADD_COMBO_VALUE_STRING(m_cmbStockObject, DC_PEN);
#endif

	m_cmbStockObject.SetCurSel( 0 );
	return bRet;
}
void CGdiPage::OnCbnSelchangeComboStockObject()
{
	BOOL bRet = FALSE;
	int nCurSel = m_cmbStockObject.GetCurSel();
	if (nCurSel != -1)
	{
		int nSelStockObj = (int)m_cmbStockObject.GetItemData(nCurSel);
		HGDIOBJ hObject = GetStockObject(nSelStockObj);

		FTL::CFGdiObjectInfoDump objInfoDumper;
		API_VERIFY(objInfoDumper.GetGdiObjectInfo(hObject));

		CString strSelStockObj;
		m_cmbStockObject.GetLBText(nCurSel, strSelStockObj);
		FTLTRACE(TEXT("StockObject:%s -- %s\n"), strSelStockObj, objInfoDumper.GetGdiObjectInfoString());
		FTL::FormatMessageBox(m_hWnd, strSelStockObj, MB_OK, TEXT("%s"), 
			objInfoDumper.GetGdiObjectInfoString());
	}
}
