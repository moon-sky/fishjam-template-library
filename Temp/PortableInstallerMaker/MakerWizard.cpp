#include "StdAfx.h"
#include "MakerWizard.h"
#include "MakerWizardSheet.h"


LPCTSTR g_lpcstrPrefRegKey = _T("Software\\Naver\\PortableInstallerMaker");

CMakerWizard::CMakerWizard(void)
{
}

CMakerWizard::~CMakerWizard(void)
{
}



bool CMakerWizard::ExecuteWizard()
{
    // You could also pass in parameters here
    // (or something generic like a name/value map).

    bool success = false;

    this->InitializeDefaultValues();

    CMakerWizardSheet wizard(&m_makerWizardInfo, 0, NULL);
    //wizard.SetActivePage(1);
    wizard.m_psh.dwFlags &= ~(PSH_NOAPPLYNOW | PSH_HASHELP);
    INT_PTR result = wizard.DoModal();
    API_ASSERT(result >= 0);

    if(result == IDOK)
    {
        // You could either do the work here, or in
        // OnWizardFinish in the completion page class.
        success = true;

        this->StoreDefaultValues();
    }

    return success;
}

void CMakerWizard::InitializeDefaultValues()
{
    //*
    bool showWelcome = true;
    CString defaultPath;
    bool defaultRecurse = true;
    CString defaultFilter;
    MakerWizardOutputType outputType = eOutput_Clipboard;
    CString outputFileName;
    MakerWizardOutputFileEncoding outputFileEncoding = eEncoding_ASCII;

    CRegKeyEx regKey;
    LONG result = regKey.Open(HKEY_CURRENT_USER, g_lpcstrPrefRegKey);
    if(result == ERROR_SUCCESS)
    {
        this->GetBoolValue(regKey, _T("showWelcome"), showWelcome);
        this->GetStringValue(regKey, _T("path"), defaultPath);
        this->GetBoolValue(regKey, _T("recurse"), defaultRecurse);
        this->GetStringValue(regKey, _T("filter"), defaultFilter);

        CString outputTypeDisplayName;
        if(this->GetStringValue(regKey, _T("outputType"), outputTypeDisplayName))
        {
            CMakerWizardInfo::GetOutputTypeForDisplayName(outputTypeDisplayName, outputType);

            // NOTE: We could have it so that "outputFileName" and "outputFileEncoding"
            //  were only looked for if the last output type were eOutput_SaveToFile.
            //  However, in case a previous run had done eOutput_SaveToFile, we'll
            //  load up what's there.

            //if(outputType == eOutput_SaveToFile)

            this->GetStringValue(regKey, _T("outputFileName"), outputFileName);

            CString outputFileEncodingDisplayName;
            if(this->GetStringValue(regKey, _T("outputFileEncoding"), outputFileEncodingDisplayName))
            {
                CMakerWizardInfo::GetOutputFileEncodingForDisplayName(outputFileEncodingDisplayName, outputFileEncoding);
            }
        }
    }
    regKey.Close();

    if(defaultPath.IsEmpty())
    {
        ::GetCurrentDirectory(MAX_PATH, defaultPath.GetBuffer(MAX_PATH+1));
        defaultPath.ReleaseBuffer();
    }
    if(defaultFilter.IsEmpty())
    {
        defaultFilter = _T("*.*");
    }

    m_makerWizardInfo.SetShowWelcome(showWelcome);
    m_makerWizardInfo.SetPath(defaultPath);
    m_makerWizardInfo.SetRecurse(defaultRecurse);
    m_makerWizardInfo.SetFilter(defaultFilter);

    m_makerWizardInfo.SetOutputType(outputType);

    m_makerWizardInfo.SetOutputFileName(outputFileName);
    m_makerWizardInfo.SetOutputFileEncoding(outputFileEncoding);
    //*/
}

void CMakerWizard::StoreDefaultValues()
{
    ///*
    bool showWelcome = m_makerWizardInfo.GetShowWelcome();
    CString path = m_makerWizardInfo.GetPath();
    bool recurse = m_makerWizardInfo.GetRecurse();
    CString filter = m_makerWizardInfo.GetFilter();
    MakerWizardOutputType outputType = m_makerWizardInfo.GetOutputType();
    CString outputTypeDisplayName = m_makerWizardInfo.GetOutputTypeDisplayName();

    CRegKeyEx regKey;
    LONG result = regKey.Open(HKEY_CURRENT_USER, g_lpcstrPrefRegKey);
    if(result != ERROR_SUCCESS)
    {
        result = regKey.Create(HKEY_CURRENT_USER, g_lpcstrPrefRegKey);
    }
    if(result == ERROR_SUCCESS)
    {
        this->SetBoolValue(regKey, _T("showWelcome"), showWelcome);
        this->SetStringValue(regKey, _T("path"), path);
        this->SetBoolValue(regKey, _T("recurse"), recurse);
        this->SetStringValue(regKey, _T("filter"), filter);

        // NOTE: For enumerations, we could either store the display name
        //  or the enumeration value.  Which ever one you choose to store,
        //  for future versions you either need to ensure that value 
        //  never changes, or you need to do a conversion.  To do a conversion,
        //  one way is to store a "schema version" number so that readers
        //  know what you're written.
        //
        //  We'll choose to store the display name so that the enumeration
        //  values can change (to change their order perhaps).
        this->SetStringValue(regKey, _T("outputType"), outputTypeDisplayName);
        if(outputType == eOutput_SaveToFile)
        {
            CString outputFileName = m_makerWizardInfo.GetOutputFileName();
            CString outputFileEncodingDisplayName = m_makerWizardInfo.GetOutputFileEncodingDisplayName();

            this->SetStringValue(regKey, _T("outputFileName"), outputFileName);
            this->SetStringValue(regKey, _T("outputFileEncoding"), outputFileEncodingDisplayName);
        }
        else
        {
            // Since "outputFileName" and "outputFileEncoding" are used with
            //  eOutput_SaveToFile, we could technically delete them if a previous
            //  run had stored them.  But we'll leave them in as defaults
            //  for future runs in case they switch back.
            //regKey.DeleteValue(_T("outputFileName"));
            //regKey.DeleteValue(_T("outputFileEncoding"));
        }
    }
    regKey.Close();
    //*/
}

bool CMakerWizard::GetStringValue(CRegKeyEx& regKey, LPCTSTR valueName, CString& value)
{
    bool success = false;

    DWORD cchValue = 0;
    LONG result = regKey.QueryStringValue(valueName, NULL, &cchValue);
    if((result == ERROR_SUCCESS) && (cchValue > 0))
    {
        regKey.QueryStringValue(valueName, value.GetBuffer(cchValue+1), &cchValue);
        value.ReleaseBuffer();
        success = true;
    }

    return success;
}

bool CMakerWizard::GetBoolValue(ATL::CRegKey& regKey, LPCTSTR valueName, bool& value)
{
    bool success = false;

    DWORD dwValue = 0;
    LONG result = regKey.QueryDWORDValue(valueName, dwValue);
    if(result == ERROR_SUCCESS)
    {
        value = (dwValue != 0);
        success = true;
    }

    return success;
}

bool CMakerWizard::SetStringValue(ATL::CRegKey& regKey, LPCTSTR valueName, LPCTSTR value)
{
    return (ERROR_SUCCESS == regKey.SetStringValue(valueName, value, REG_SZ));
}

bool CMakerWizard::SetBoolValue(ATL::CRegKey& regKey, LPCTSTR valueName, bool value)
{
    return (ERROR_SUCCESS == regKey.SetDWORDValue(valueName, (value ? 1 : 0)));
}

