
// GdiPlusTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GdiPlusTest.h"
#include "GdiPlusTestDlg.h"
#include <ftlGdiPlus.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGdiPlusTestDlg dialog




CGdiPlusTestDlg::CGdiPlusTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGdiPlusTestDlg::IDD, pParent)
	, m_nFontHeight(10)
	, m_strPaint(_T("测试文字，有文本有ABC"))
	, m_strFontFamily(_T("宋体"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGdiPlusTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PAINT, m_staticPaint);
	DDX_Text(pDX, IDC_EDIT_FONT_HEIGHT, m_nFontHeight);
	DDX_Text(pDX, IDC_EDIT_STRING, m_strPaint);
	DDX_Text(pDX, IDC_EDIT_FONT_FAMILY, m_strFontFamily);
	//DDX_Control(pDX, IDC_LIST_DRAW_FORMAT, m_lstDrawFormat);
	DDX_Control(pDX, IDC_LIST_DRAW_FORMAT, m_lstDrawFormat);

	//DDX_Text(pDX, IDC_EDIT_STRING, )
}

BEGIN_MESSAGE_MAP(CGdiPlusTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_DRAW_STRING, &CGdiPlusTestDlg::OnBnClickedBtnDrawString)
END_MESSAGE_MAP()


// CGdiPlusTestDlg message handlers

BOOL CGdiPlusTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	m_lstDrawFormat.ModifyStyle(LVS_ICON,LVS_REPORT,0);
	m_lstDrawFormat.SetExtendedStyle(m_lstDrawFormat.GetExtendedStyle() | LVS_EX_CHECKBOXES);

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for(int i = 0; i< 1; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = TEXT("Formats");
		lvc.cx = 100;
		lvc.fmt = LVCFMT_LEFT;
		m_lstDrawFormat.InsertColumn(i,&lvc);
	}

	LV_ITEM lvi = {0};
	CString strThreadID;

	for (int i = 0; i < 5; i++)
	{
		lvi.mask = LVIF_TEXT | LVIF_STATE | LVIF_IMAGE;
		lvi.stateMask = LVIS_STATEIMAGEMASK;
		lvi.state = INDEXTOSTATEIMAGEMASK(2);
		lvi.iItem = i;
		lvi.iSubItem = 0;
		strThreadID.Format(TEXT("Item %d"), i);
		lvi.pszText = (LPTSTR)(LPCTSTR)strThreadID;
		//lvi.iImage = i;
		int nIndex = m_lstDrawFormat.InsertItem(&lvi);
		m_lstDrawFormat.SetCheck(nIndex,TRUE);
	}


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGdiPlusTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGdiPlusTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGdiPlusTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGdiPlusTestDlg::OnBnClickedBtnDrawString()
{
	//CStatic* pDrawWnd = (CStatic*)GetDlgItem(IDC_STATIC_PAINT);
	//if (pDrawWnd)
	//{
	//	CDC* pDC = pDrawWnd->GetWindowDC();
	//	pDrawWnd->ReleaseDC(pDC);
	//}
	UpdateData(TRUE);

	CDC* pDrawDC = m_staticPaint.GetWindowDC();
	{
		
		Status sts = Ok;

		Graphics graphic(pDrawDC->m_hDC);
		
		Gdiplus::Font font(m_strFontFamily, (REAL)m_nFontHeight);
		PointF ptStart(0, 0);
		StringFormat format;
		format.SetAlignment(StringAlignmentCenter);
		format.SetLineAlignment(StringAlignmentCenter);
		SolidBrush brush(Color::Red);

		CRect rcPaint;
		m_staticPaint.GetClientRect(rcPaint);
		RectF rcPaintGdiP(rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom);

		SolidBrush nullBrush(Color::Black);
		graphic.FillRectangle(&nullBrush, rcPaintGdiP);

		GDIPLUS_VERIFY(graphic.DrawString(m_strPaint, m_strPaint.GetLength(), &font, rcPaintGdiP, &format,  &brush));
	}

	m_staticPaint.ReleaseDC(pDrawDC);
}

void CGdiPlusTestDlg::OnOK()
{
	OnBnClickedBtnDrawString();
}
