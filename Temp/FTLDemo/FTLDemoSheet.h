#pragma once

// CFTLDemoSheet

class CFTLDemoSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CFTLDemoSheet)

public:
	CFTLDemoSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CFTLDemoSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CFTLDemoSheet();

protected:
    //CBasePage           m_BasePage;
 //   CCrackPage          m_CrackPage;
 //   CCrashHandlerPage   m_CrashHandlerPage;
 //   CGuiPage            m_GuiPage;
 //   CMemPage            m_MemPage;
 //   CNetPage            m_NetPage;
 //   CNLSPage            m_NLSPage;
 //   CPipelinePage       m_PipelinePage;
 //   CShellPage          m_ShellPage;
 //   CSystemPage         m_SystemPage;
 //   CThreadPage         m_ThreadPage;
 //   CVistaPage          m_VistaPage;
 //   CWindowPage         m_WindowPage;
protected:
    BOOL    InitSheet();
	BOOL    UninitSheet();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDestroy();
};


