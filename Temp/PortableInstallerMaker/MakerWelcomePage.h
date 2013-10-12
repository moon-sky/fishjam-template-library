#pragma once
#include "resource.h"
#include "MakerWizardInfo.h"

class CMakerWelcomePage :
    public CWizard97ExteriorPageImpl<CMakerWelcomePage>,
    public CWinDataExchange<CMakerWelcomePage>,
    public CDialogResize<CMakerWelcomePage>,
    public CMakerWizardInfoRef
{
protected:
    // Typedefs
    typedef CMakerWelcomePage thisClass;
    typedef CWizard97ExteriorPageImpl<CMakerWelcomePage> baseClass;

    // Data members
    //CButton m_buttonSkipWelcome;
    bool m_allowWelcomeToHide;

public:
    // Constructors
    CMakerWelcomePage(_U_STRINGorID title = (LPCTSTR)NULL);

    // Message Handlers
    enum { IDD = IDD_PAGE_WELCOME };

    BEGIN_DLGRESIZE_MAP(thisClass)
        DLGRESIZE_CONTROL(IDC_STATIC_WELCOME, DLSZ_MOVE_Y|DLSZ_MOVE_X)
    END_DLGRESIZE_MAP()

    BEGIN_DDX_MAP(thisClass)
    END_DDX_MAP()

    BEGIN_MSG_MAP(thisClass)
      MSG_WM_INITDIALOG(OnInitDialog)
      //MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      CHAIN_MSG_MAP(CDialogResize)
      CHAIN_MSG_MAP(baseClass)
    END_MSG_MAP()

    //LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);

    // Helper methods
    void InitializeControls(void);
    void InitializeValues(void);
    bool StoreValues(void);

    // Overrides from base class
    int OnSetActive();
    int OnWizardNext();
    void OnHelp();
};

