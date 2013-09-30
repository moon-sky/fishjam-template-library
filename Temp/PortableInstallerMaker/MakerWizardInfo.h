#pragma once

static LPCTSTR s_allFiles = _T("*.*");

class IMakerWizardFindFileCB
{
public:
    virtual bool OnBeginFindFiles(void) = 0;
    virtual void OnEndFindFiles(void) = 0;
    virtual bool OnBeginDirectorySearch(LPCTSTR directory) = 0;
    virtual void OnEndDirectorySearch(LPCTSTR directory) = 0;

    virtual void OnFileFound(LPCTSTR directory, LPWIN32_FIND_DATA findFileData) = 0;
};

// Enumerations
enum MakerWizardOutputType
{
    eOutput_Clipboard  = 0,
    eOutput_SendEMail  = 1,
    eOutput_SaveToFile = 2,

    eOutput_First      = eOutput_Clipboard,
    eOutput_Last       = eOutput_SaveToFile,
};
enum MakerWizardOutputFileEncoding
{
    eEncoding_ASCII = 0,
    eEncoding_UCS2  = 1,
    eEncoding_UTF8  = 2,

    eEncoding_First = eEncoding_ASCII,
    eEncoding_Last  = eEncoding_UTF8,
};
enum MakerWizardQuerySiblingNotifiations
{
    eQuerySibling_ParametersFileChanged = 0,
};

class CMakerWizardInfo
{
protected:
    // Typedefs
    typedef CMakerWizardInfo thisClass;

    // Member variables
    CString m_path, m_filter;
    bool m_showWelcome, m_recurse;

    MakerWizardOutputType m_outputType;
    CString m_outputFileName;
    MakerWizardOutputFileEncoding m_outputFileEncoding;

public:
    // Constructor/Destructor
    CMakerWizardInfo();
    virtual ~CMakerWizardInfo();

    // General methods
    // Set
    bool SetShowWelcome(bool showWelcome);
    bool SetPath(LPCTSTR path);
    bool SetRecurse(bool recurse);
    bool SetFilter(LPCTSTR filter);
    bool SetOutputType(MakerWizardOutputType outputType);
    bool SetOutputTypeByDisplayName(LPCTSTR typeDisplayName);
    bool SetOutputFileName(LPCTSTR outputFileName);
    bool SetOutputFileEncoding(MakerWizardOutputFileEncoding outputFileEncoding);
    bool SetOutputFileEncodingByDisplayName(LPCTSTR encodingDisplayName);

    // Get
    bool GetShowWelcome(void) const;
    CString GetPath(void) const;
    bool GetRecurse(void) const;
    CString GetFilter(void) const;
    MakerWizardOutputType GetOutputType(void) const;
    CString GetOutputTypeDisplayName(void) const;
    CString GetOutputFileName(void) const;
    MakerWizardOutputFileEncoding GetOutputFileEncoding(void) const;
    CString GetOutputFileEncodingDisplayName(void) const;

    // Static methods
    static bool IsValidOutputType(DWORD outputType);
    static bool IsValidOutputFileEncoding(DWORD outputFileEncoding);
    static CString GetOutputTypeDisplayName(MakerWizardOutputType outputType);
    static CString GetOutputFileEncodingDisplayName(MakerWizardOutputFileEncoding outputFileEncoding);
    static bool GetOutputTypeForDisplayName(LPCTSTR typeDisplayName, MakerWizardOutputType& outputType);
    static bool GetOutputFileEncodingForDisplayName(LPCTSTR encodingDisplayName, MakerWizardOutputFileEncoding& outputFileEncoding);

    // File List
    int FindFiles(IMakerWizardFindFileCB* callback) const;

    // Page Navigation
    int FindPreviousPage(int pageDialogId) const;
    int FindNextPage(int pageDialogId) const;

    // Help
    void ShowHelp(int pageDialogId, int controlId = 0);
    void ShowContextHelp(LPHELPINFO helpInfo);

    // FinishWizard
    bool FinishWizard(HWND hWndParent);

protected:
    // Implementation methods

    int FindFiles(IMakerWizardFindFileCB* callback, LPCTSTR directory, LPCTSTR filter, bool recurse) const;
    bool FinishWizard_CopyToClipboard(HWND hWndParent);
    bool FinishWizard_SendEMail(HWND hWndParent);
    bool FinishWizard_SaveToFile(HWND hWndParent);
};

class CMakerWizardInfoRef
{
protected:
    // Data members
    CMakerWizardInfo* m_pMakerWizardInfo;

public:
    // Constructors
    CMakerWizardInfoRef(CMakerWizardInfo* pMakerWizardInfo = NULL) : 
      m_pMakerWizardInfo(pMakerWizardInfo)
      {
      }

      // Public methods
      CMakerWizardInfo* GetMakerWizardInfo(void)
      {
          return m_pMakerWizardInfo;
      }
      void SetMakerWizardInfo(CMakerWizardInfo* pMakerWizardInfo)
      {
          m_pMakerWizardInfo = pMakerWizardInfo;
      }
};

class CMakerWizardFindFile_BuildString :
    public CMakerWizardInfoRef,
    public IMakerWizardFindFileCB
{
protected:
    // Typedefs
    typedef CMakerWizardInfoRef baseClass;

    // Data members
    CString m_output;

public:
    // IMakerWizardFindFileCB
    virtual bool OnBeginFindFiles(void) { return true; }
    virtual void OnEndFindFiles(void) { }
    virtual bool OnBeginDirectorySearch(LPCTSTR /*directory*/) { return true; }
    virtual void OnEndDirectorySearch(LPCTSTR /*directory*/) { }

    virtual void OnFileFound(LPCTSTR directory, LPWIN32_FIND_DATA findFileData);

    // Public methods
    const CString& GetOutputString() const
    {
        return m_output;
    }
};

class CMakerWizardFindFile_SaveToFile :
    public CMakerWizardInfoRef,
    public IMakerWizardFindFileCB
{
protected:
    // Typedefs
    typedef CMakerWizardInfoRef baseClass;

    // Data members
    CString m_outputFileName;
    HANDLE m_hFile;
    MakerWizardOutputFileEncoding m_outputFileEncoding;
    bool m_succeeded;
    CString m_failureReason;

public:
    // Constructor/Destructor
    CMakerWizardFindFile_SaveToFile() :
      m_hFile(INVALID_HANDLE_VALUE),
          m_outputFileEncoding(eEncoding_ASCII),
          m_succeeded(false)
      {
      }

      ~CMakerWizardFindFile_SaveToFile()
      {
          if(m_hFile != INVALID_HANDLE_VALUE)
          {
              ::CloseHandle(m_hFile);
              m_hFile = INVALID_HANDLE_VALUE;
          }
      }

      // IMakerWizardFindFileCB
      virtual bool OnBeginFindFiles(void);
      virtual void OnEndFindFiles(void);
      virtual bool OnBeginDirectorySearch(LPCTSTR /*directory*/) { return true; }
      virtual void OnEndDirectorySearch(LPCTSTR /*directory*/) { }

      virtual void OnFileFound(LPCTSTR directory, LPWIN32_FIND_DATA findFileData);

      // Public methods
      bool Succeeded(void) const
      {
          return m_succeeded;
      }
      CString GetFailureReason(void) const
      {
          return m_failureReason;
      }
};
