#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <ftlFile.h>

/************************************************************************
* 
************************************************************************/

class CFTLFileTester : public CPPUNIT_NS::TestFixture, 
    public ICopyDirCallback
{
public:

    CPPUNIT_TEST_SUITE( CFTLFileTester );
    CPPUNIT_TEST( test_CreateFile );
    CPPUNIT_TEST( test_DirectoryCopier );

    CPPUNIT_TEST_SUITE_END();

    void test_CreateFile();
    void test_DirectoryCopier();

    DECLARE_DEFAULT_TEST_CLASS(CFTLFileTester);


    //ICopyDirCallback
    virtual VOID OnBegin(LONGLONG nTotalSize, LONG nFileCount) { }
    virtual VOID OnCopyFile(LPCTSTR pszSrcFile, LPCTSTR pszTargetFile, LONG nIndex, LONGLONG nFileSize, LONGLONG nCopiedSize) { }
    virtual VOID OnError(LPCTSTR pszSrcFile, LPCTSTR pszTargetFile, DWORD dwError) { }
    virtual VOID OnEnd(BOOL bSuccess, LONGLONG nTotalCopiedSize, LONG nCopiedFileCount) { }

private:
    FTL::CFDirectoryCopier  m_DirCopier;
};
