#pragma once

#include "resource.h"
#include "MakerWizardInfo.h"

class CMakerSetupResultPage :
    public CWizard97ExteriorPageImpl<CMakerSetupResultPage>,
    public CWinDataExchange<CMakerSetupResultPage>,
    public CMakerWizardInfoRef
{
protected:
    // Typedefs
    typedef CMakerSetupResultPage thisClass;
    typedef CWizard97ExteriorPageImpl<CMakerSetupResultPage> baseClass;

    // Data members
    CTreeViewCtrlEx m_treeSetupChangeResult;
public:
    // Constructors
    CMakerSetupResultPage(_U_STRINGorID title = (LPCTSTR)NULL);

    // Message Handlers
    enum { IDD = IDD_PAGE_SETUP_RESULT };

    BEGIN_DDX_MAP(thisClass)
        DDX_CONTROL_HANDLE(IDC_TREE_SETUP_CHANGE_LIST, m_treeSetupChangeResult)
    END_DDX_MAP()

    BEGIN_MSG_MAP(thisClass)
        MSG_WM_INITDIALOG(OnInitDialog)
        CHAIN_MSG_MAP(baseClass)
    END_MSG_MAP()

    BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);

    // Helper methods
    void InitializeControls(void);
    void InitializeValues(void);
    bool StoreValues(void);

    // Overrides from base class
    int OnSetActive();
    int OnWizardNext();
    void OnHelp();
    BOOL _IsFilterFile(LPCTSTR pszFilePath);
    BOOL PathIsTempPath(LPCTSTR pszFilePath);

};


