#pragma once

#include "resource.h"
#include "MakerWizardInfo.h"

class CMakerSetupProgressPage :
    public CWizard97ExteriorPageImpl<CMakerSetupProgressPage>,
    public CWinDataExchange<CMakerSetupProgressPage>,
    public CMakerWizardInfoRef
{
protected:
    // Typedefs
    typedef CMakerSetupProgressPage thisClass;
    typedef CWizard97ExteriorPageImpl<CMakerSetupProgressPage> baseClass;

    // Data members
public:
    // Constructors
    CMakerSetupProgressPage(_U_STRINGorID title = (LPCTSTR)NULL);

    // Message Handlers
    enum { IDD = IDD_PAGE_SETUP_PROGRESS };

    BEGIN_DDX_MAP(thisClass)
        //DDX_TEXT(IDC_EDIT_SETUP_FILE_PATH, m_strSetupFilePath)
    END_DDX_MAP()

    BEGIN_MSG_MAP(thisClass)
        MSG_WM_INITDIALOG(OnInitDialog)
        CHAIN_MSG_MAP(baseClass)
    END_MSG_MAP()

    BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
    void OnBtnChooseSetupFile(UINT uNotifyCode, int nID, CWindow wndCtl);

    // Helper methods
    void InitializeControls(void);
    void InitializeValues(void);
    bool StoreValues(void);

    // Overrides from base class
    int OnSetActive();
    int OnWizardNext();
    void OnHelp();
};


