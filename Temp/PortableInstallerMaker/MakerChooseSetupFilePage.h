#pragma once

#include "resource.h"
#include "MakerWizardInfo.h"

class CMakerChooseSetupFilePage :
    public CWizard97ExteriorPageImpl<CMakerChooseSetupFilePage>,
    public CWinDataExchange<CMakerChooseSetupFilePage>,
    public CMakerWizardInfoRef
{
protected:
    // Typedefs
    typedef CMakerChooseSetupFilePage thisClass;
    typedef CWizard97ExteriorPageImpl<CMakerChooseSetupFilePage> baseClass;

    // Data members
    CString     m_strSetupFilePath;

public:
    // Constructors
    CMakerChooseSetupFilePage(_U_STRINGorID title = (LPCTSTR)NULL);

    // Message Handlers
    enum { IDD = IDD_PAGE_CHOOSE_SETUP_FILE };

    BEGIN_DDX_MAP(thisClass)
        DDX_TEXT(IDC_EDIT_SETUP_FILE_PATH, m_strSetupFilePath)
    END_DDX_MAP()

    BEGIN_MSG_MAP(thisClass)
        MSG_WM_INITDIALOG(OnInitDialog)
        COMMAND_ID_HANDLER_EX(IDC_BTN_CHOOSE_SETUP_FILE, OnBtnChooseSetupFile)
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


