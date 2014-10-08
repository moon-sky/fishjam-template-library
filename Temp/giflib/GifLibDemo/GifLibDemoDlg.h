
// GifLibDemoDlg.h : header file
//

#pragma once

#include "../gifUtil/gifUtil.h"

// CGifLibDemoDlg dialog
class CGifLibDemoDlg : public CDialog, 
    public IGifMakerCallback,
    public IGifParserCallback
{
// Construction
public:
	CGifLibDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GIFLIBDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


public:
    //IGifMakerCallback
    virtual VOID OnBeginWriteGif();
    virtual VOID OnWriteGifFrame(INT nIndex, INT nTotal);
    virtual VOID OnEndWriteGif();

    //IGifParserCallback
    virtual VOID OnGetGifInfo(INT nWidth, INT nHeight, DWORD_PTR callbackData);
    virtual BOOL OnParseGifFrame(INT nIndex, const GifControlInfo& gifControlInfo, const RECT& rcFrame, BYTE* pBmpBuffer, INT nLength, DWORD_PTR callbackData);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();
    BOOL _OverlayMouseToScreen(HDC hdc, LPRECT lpRect);
    afx_msg void OnBnClickedButton2();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedBtnTimerClip();
    afx_msg void OnBnClickedBtnSingleColorQuantizer();
    afx_msg void OnBnClickedBtnGifParser();

private:
    IGifMaker*    m_pTimerGifMaker;
    int m_nClipIndex;
    DWORD m_dwTicket;
    int m_nTargetBpp;
    INT m_nParserScreenWidth;
    INT m_nParserScreenHeight;
};
