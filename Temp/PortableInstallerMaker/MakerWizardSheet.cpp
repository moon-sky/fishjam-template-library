#include "StdAfx.h"
#include "MakerWizardSheet.h"

CMakerWizardSheet::CMakerWizardSheet(CMakerWizardInfo* pMakerWizardInfo, UINT uStartPage, HWND hWndParent) :
baseClass(_T("Maker Wizard"), 
          IDB_WIZ97_HEADER, 
          IDB_WIZ97_WATERMARK, uStartPage, hWndParent),
infoRefClass(pMakerWizardInfo)
{
    BOOL bRet = FALSE;
    m_pageWelcome.SetMakerWizardInfo(pMakerWizardInfo);
    m_pageChooseSetupFile.SetMakerWizardInfo(pMakerWizardInfo);
    m_pageSetupProgress.SetMakerWizardInfo(pMakerWizardInfo);
    m_pageSetupResult.SetMakerWizardInfo(pMakerWizardInfo);
    m_pageCompletion.SetMakerWizardInfo(pMakerWizardInfo);

    API_VERIFY(AddPage(m_pageWelcome));
    API_VERIFY(AddPage(m_pageChooseSetupFile));
    API_VERIFY(AddPage(m_pageSetupProgress));
    API_VERIFY(AddPage(m_pageSetupResult));
    API_VERIFY(AddPage(m_pageCompletion));

    m_psh.dwFlags &= ~PSH_HASHELP;
}

LRESULT CMakerWizardSheet::OnHelp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    // We get here when the user hits F1 while on a page,
    // or uses the "What's This" button then clicks on a control.
    // We can also handle WM_HELP on the page for the cases
    // when a control on the dialog has focus.  If the page doesn't handle WM_HELP,
    // then the sheet is given a chance to handle it (and we end up here).

    LPHELPINFO helpInfo = (LPHELPINFO)lParam;
    if(helpInfo)
    {
        if(helpInfo->dwContextId != 0)
        {
            // If dwContextId is set, then the control with
            // focus has a help context ID, so we'll show context help.
            m_pMakerWizardInfo->ShowContextHelp(helpInfo);
        }
        else
        {
            int currentIndex = this->GetActiveIndex();
            if(currentIndex >= 0)
            {
                int pageDialogId = this->IndexToId(currentIndex);
                if(pageDialogId != 0)
                {
                    m_pMakerWizardInfo->ShowHelp(pageDialogId, helpInfo->iCtrlId);
                }
            }
        }
    }

    return 0;
}
