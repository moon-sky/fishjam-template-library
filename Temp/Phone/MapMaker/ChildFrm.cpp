// ChildFrm.cpp : implementation of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MapMakerView.h"
#include "ChildFrm.h"
#include "OptionsDlg.h"
#include <atlpath.h>

CChildFrame::CChildFrame()
{

}

CChildFrame::~CChildFrame()
{

}

void CChildFrame::OnFinalMessage(HWND /*hWnd*/)
{
	delete this;
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL, WS_EX_CLIENTEDGE);
	// replace with appropriate values for the app
	//m_view.SetScrollSize(1024, 768);
    
    SetMsgHandled(FALSE);
	return 1;
}

BOOL CChildFrame::OnForwardMsg(LPMSG pMsg, DWORD nUserData)
{
	if(CMDIChildWindowImpl<CChildFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return m_view.PreTranslateMessage(pMsg);
}

BOOL CChildFrame::SetImagePath(const CString& strImagePath)
{
    return m_view.SetImagePath(strImagePath);
}


void CChildFrame::OnOptinos(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    COptionsDlg dlgOptions;
    dlgOptions.m_nTileHeight = m_view.m_nGridHeight;
    dlgOptions.m_nTileWidth = m_view.m_nGridWidth;
    dlgOptions.m_nTranspant = m_view.m_nTranspant;
    if (dlgOptions.DoModal() == IDOK)
    {
        if (dlgOptions.m_nTranspant != m_view.m_nTranspant)
        {
            m_view.m_nTranspant = dlgOptions.m_nTranspant;
            m_view.Invalidate();
        }
        if (
            (dlgOptions.m_nTileHeight != m_view.m_nGridHeight)
            || (dlgOptions.m_nTileWidth != m_view.m_nGridWidth)
            )
        {
            m_view.SetTileParams(dlgOptions.m_nTileWidth, dlgOptions.m_nTileHeight);
        }
        
    }
}

void CChildFrame::OnSelectDrawEmpty(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    m_view.SetDrawToolType(dttEmpty);
    UISetCheck(ID_DRAW_EMPTY, TRUE);
}

void CChildFrame::OnSelectDrawWall(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    m_view.SetDrawToolType(dttWall);
    UISetCheck(ID_DRAW_WALL, TRUE);
}
void CChildFrame::OnSelectDrawElevator(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    m_view.SetDrawToolType(dttElevator);
    UISetCheck(ID_DRAW_ELEVATOR, TRUE);
}

void CChildFrame::OnClearDrawTool(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    if (MessageBox(TEXT("确定清空全部表格为空地状态？"), TEXT("提示"), MB_OKCANCEL)
        == IDOK)
    {
        m_view.ResetTileGrids(dttEmpty);
    }
}
void CChildFrame::OnSetAllDrawTool(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    if (MessageBox(TEXT("确定设置全部表格为不可通过状态？"), TEXT("提示"), MB_OKCANCEL)
        == IDOK)
    {
        m_view.ResetTileGrids(dttWall);
    }
}

void CChildFrame::OnFileExportMap(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    BOOL bRet = FALSE;
    CString STR_MAP_TEXT_APPENDIX = _T("_txt.txt");
    CPath path(m_view.GetImagePath());
    path.RemoveExtension();
    path.m_strPath += STR_MAP_TEXT_APPENDIX;

    CString strFilter = _T("Map Text Files(*.txt)|*.txt||");
    strFilter.Replace(TEXT('|'), TEXT('\0'));

    CFileDialog dlgSave(FALSE, TEXT(".txt"), path.m_strPath, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        strFilter);
    if (dlgSave.DoModal() == IDOK)
    {
        API_VERIFY(m_view.SaveMapInfo(dlgSave.m_szFileName));
    }
}