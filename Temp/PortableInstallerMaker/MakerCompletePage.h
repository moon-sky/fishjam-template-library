#pragma once

#include "resource.h"
#include "MakerWizardInfo.h"

class CMakerCompletePage :
    public CWizard97ExteriorPageImpl<CMakerCompletePage>,
    public CMakerWizardInfoRef
{
protected:
    // Typedefs
    typedef CMakerCompletePage thisClass;
    typedef CWizard97ExteriorPageImpl<CMakerCompletePage> baseClass;

    // Data members
    CFont m_fontSummary;
    //CRichEditCtrl m_editSummary;

public:
    // Constructors
    CMakerCompletePage(_U_STRINGorID title = (LPCTSTR)NULL) :
      baseClass(title)
      { }

      // Message Handlers
      enum { IDD = IDD_PAGE_COMPLETE };
      BEGIN_MSG_MAP(thisClass)
          MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)

          CHAIN_MSG_MAP(baseClass)
      END_MSG_MAP()

      LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

      // Helpers
      void InitializeFont(void);
      void InitializeControls(void);
      void InitializeValues(void);
      void UpdateSummary(void);

      // Overrides from base class
      int OnSetActive();
      int OnWizardBack();
      int OnWizardFinish();
      void OnHelp();
};
