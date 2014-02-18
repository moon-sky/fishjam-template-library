// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CChildFrame : public CMDIChildWindowImpl<CChildFrame>
{
public:
	CChildFrame();
    virtual ~CChildFrame();

    BOOL SetImagePath(const CString& strImagePath);

    DECLARE_FRAME_WND_CLASS(NULL, IDR_MDICHILD)


	virtual void OnFinalMessage(HWND /*hWnd*/);

	BEGIN_MSG_MAP(CChildFrame)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_FORWARDMSG(OnForwardMsg)
        COMMAND_ID_HANDLER_EX(ID_DRAW_EMPTY, OnSelectDrawEmpty)
        COMMAND_ID_HANDLER_EX(ID_DRAW_WALL, OnSelectDrawWall)
        COMMAND_ID_HANDLER_EX(ID_DRAW_ELEVATOR, OnSelectDrawElevator)
        COMMAND_ID_HANDLER_EX(ID_TOOLS_OPTIONS, OnOptinos)

		CHAIN_MSG_MAP(CMDIChildWindowImpl<CChildFrame>)
	END_MSG_MAP()

    int OnCreate(LPCREATESTRUCT lpCreateStruct);
    BOOL OnForwardMsg(LPMSG Msg, DWORD nUserData);
    void OnSelectDrawEmpty(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnSelectDrawWall(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnSelectDrawElevator(UINT uNotifyCode, int nID, CWindow wndCtl);
    void OnOptinos(UINT uNotifyCode, int nID, CWindow wndCtl);
private:
    CMapMakerView m_view;
};
