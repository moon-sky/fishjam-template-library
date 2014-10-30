// GuiPage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "GdiPage.h"
#include <afxwin.h>
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
    DDX_Control(pDX, IDC_STATIC_DRAW_TARGET, m_staticDrawTarget);
}

BEGIN_MESSAGE_MAP(CGdiPage, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_STOCK_OBJECT, &CGdiPage::OnCbnSelchangeComboStockObject)
    ON_BN_CLICKED(IDC_BTN_CANVAS_DRAW, &CGdiPage::OnBnClickedBtnCanvasDraw)
    ON_BN_CLICKED(IDC_BTN_CANVAS_ATTACH_BMP, &CGdiPage::OnBnClickedBtnCanvasAttachBmp)
    ON_BN_CLICKED(IDC_BTN_BITBLT_CANVAS, &CGdiPage::OnBnClickedBtnBitbltCanvas)
    ON_BN_CLICKED(IDC_BTN_GDI_UTIL_GETHDC_PROPERTY, &CGdiPage::OnBnClickedBtnGdiUtilGethdcProperty)
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


void CGdiPage::OnBnClickedBtnCanvasAttachBmp()
{
    BOOL bRet = FALSE;
    //CString strFilter = _T("Bmp Files(*.bmp)|*.bmp|All Files(*.*)|*.*||");
    //CFileDialog dlg(TRUE, TEXT(".BMP"), NULL, 0, strFilter, this);
    //if (dlg.DoModal() == IDOK)
    {
        m_canvas.Release();
        API_VERIFY(m_canvas.AttachBmpFile(TEXT("F:\\Fujie\\FJCODE_GOOGLE\\Temp\\FTLTest\\LargeBitmap.bmp"))); //dlg.GetPathName()
    }
}

void CGdiPage::OnBnClickedBtnCanvasDraw()
{
    BOOL bRet = FALSE;

    CWindowDC dcTarget(&m_staticDrawTarget);
    CRect rcTarget;
    m_staticDrawTarget.GetClientRect(&rcTarget);
    m_canvas.Draw(dcTarget.GetSafeHdc(), rcTarget.left, rcTarget.top, rcTarget.right, rcTarget.bottom,
        NULL, true);
}


void CGdiPage::OnBnClickedBtnBitbltCanvas()
{
    BOOL bRet = FALSE;
    CWindowDC dcTarget(&m_staticDrawTarget);
    
    API_VERIFY(::BitBlt(dcTarget.GetSafeHdc(), 0, 0, m_canvas.GetWidth(), m_canvas.GetHeight(), 
        m_canvas.GetCanvasDC(), 0, 0, SRCCOPY));
}

void CGdiPage::OnBnClickedBtnGdiUtilGethdcProperty()
{
    BOOL bRet = FALSE;
    FTL::HDCProperty hdcProperty;
    HWND hWndDesktop = ::GetDesktopWindow();
    HDC hdcDesktop = ::GetDC(hWndDesktop);
    if (hdcDesktop)
    {
        API_VERIFY(FTL::CFGdiUtil::GetHDCProperty(hdcDesktop, &hdcProperty));
        FTLTRACE(TEXT("hdc Desktop=%s\n"), hdcProperty.GetPropertyString(HDC_PROPERTY_GET_ALL));

        ::ReleaseDC(hWndDesktop, hdcDesktop);
    }
    
}
