#pragma once

#include "MakerWizardInfo.h"

class CMakerWizard
{
protected:
    // Data members
    CMakerWizardInfo m_makerWizardInfo;
public:
    // Constructors
    CMakerWizard();
    ~CMakerWizard(void);

    // General Methods
    bool ExecuteWizard();

    void InitializeDefaultValues();
    void StoreDefaultValues();

    bool GetStringValue(ATL::CRegKey& regKey, LPCTSTR valueName, CString& value);
    bool GetBoolValue(ATL::CRegKey& regKey, LPCTSTR valueName, bool& value);

    bool SetStringValue(ATL::CRegKey& regKey, LPCTSTR valueName, LPCTSTR value);
    bool SetBoolValue(ATL::CRegKey& regKey, LPCTSTR valueName, bool value);

};
