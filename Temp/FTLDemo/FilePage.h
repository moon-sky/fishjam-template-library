#pragma once

#include <ftlFile.h>
// CFilePage dialog

class CFilePage : public CPropertyPage
    , public ICopyDirCallback
{
	DECLARE_DYNAMIC(CFilePage)

public:
	CFilePage();
	virtual ~CFilePage();

// Dialog Data
	enum { IDD = IDD_PAGE_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnFileChooseCopySrcDir();
    afx_msg void OnBnClickedBtnFileChooseCopyDstDir();
    afx_msg void OnBnClickedBtnFileFindSourceDir();
    afx_msg void OnBnClickedBtnFileStartCopyDir();
    afx_msg void OnBnClickedBtnFileStopCopyDir();
    virtual BOOL OnInitDialog();
protected:
    CString     m_strCopySrcDir;
    CString     m_strCopyDstDir;

    CFDirectoryCopier   m_DirCopier;

    //ICopyDirCallback
    virtual VOID OnBeginPrepareSourceFiles(LPCTSTR pszSrcDir, LPCTSTR pszDstDir);
    virtual VOID OnBegin(LONGLONG nTotalSize, LONG nFileCount);
    virtual VOID OnCopyFile(LPCTSTR pszSrcFile, LPCTSTR pszTargetFile, LONG nIndex, LONGLONG nFileSize, LONGLONG nCopiedSize);
    virtual VOID OnEnd(BOOL bSuccess, LONGLONG nTotalCopiedSize, LONG nCopiedFileCount);
    virtual VOID OnError(LPCTSTR pszSrcFile, LPCTSTR pszTargetFile, DWORD dwError);
};
