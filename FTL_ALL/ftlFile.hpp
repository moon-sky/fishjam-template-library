#ifndef FTL_FILE_HPP
#define FTL_FILE_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlfile.h"
#endif

namespace FTL
{
    CFConsoleFile::CFConsoleFile()
    {
        AllocConsole(); 
        int hCrun;
        hCrun = _open_osfhandle((long )GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT); 
        FILE* hFile  = _fdopen(hCrun, "w" ); 
        // use default stream buffer 
        setvbuf(hFile, NULL, _IONBF, 0); 
        *stdout = *hFile; 
        //test  
        //_cprintf("test console by _cprintf/n", 0); 
        //std::cout << "test console by std::out/n"; 
    }
    CFConsoleFile::~CFConsoleFile()
    {
        FreeConsole(); 
    }

	//-----------------------------------------------------------------------------
	// Construction/Destruction
	//-----------------------------------------------------------------------------
	CFFile::CFFile()
	{
		m_hFile = INVALID_HANDLE_VALUE;
	}

	CFFile::CFFile(HANDLE hFile)
	{
		m_hFile = hFile;
	}

	CFFile::~CFFile()
	{
		Close();
	}

	//-----------------------------------------------------------------------------
	// Constructs a duplicate CFFile object for a given file.
	//-----------------------------------------------------------------------------
	CFFile* CFFile::Duplicate() const
	{
		_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));

		CFFile * pFile = new CFFile();

		HANDLE hFile;
		if (!::DuplicateHandle(::GetCurrentProcess(), m_hFile,
			::GetCurrentProcess(), &hFile, 0, FALSE, DUPLICATE_SAME_ACCESS))
		{
			delete pFile;
			CFFileException ex((long)::GetLastError());
			throw ex;
		}

		pFile->m_hFile = hFile;
		_ASSERT(pFile->m_hFile);

		return pFile;
	}

	//-----------------------------------------------------------------------------
	// Creates or opens a file
	//-----------------------------------------------------------------------------
	void CFFile::Open(LPCTSTR pszFileName,			// file name 
		DWORD dwAccess,				// access mode
		DWORD dwShareMode,				// share mode
		LPSECURITY_ATTRIBUTES lpSA,	// poiner to SECURITY_ATTRIBUTES
		DWORD dwCreationDisposition,	// how to create
		DWORD dwAttributes,			// file attributes
		HANDLE hTemplateFile)			// handle to template file
	{
		//_ASSERT(!strFileName.IsEmpty());				// file name should not be empty
		//_ASSERT(strFileName.GetLength() <= MAX_PATH);	// the file name is limited to 
		// MAX_PATH charactes

		m_hFile = NULL;
		m_strFileName.Empty();

		// Retrieve the full path and file name 
		TCHAR szTemp[MAX_PATH];
		LPTSTR lpszFilePart;

		if (!::GetFullPathName(pszFileName, MAX_PATH, szTemp, &lpszFilePart))
		{
			_ASSERT(FALSE);
			m_strFileName = pszFileName;
		}

		m_strFileName = szTemp;

		// Attempt file creation
		HANDLE hFile = ::CreateFile(m_strFileName, 
			dwAccess, 
			dwShareMode, 
			lpSA,
			dwCreationDisposition, 
			dwAttributes, 
			hTemplateFile);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			CFFileException ex((long)::GetLastError());
			throw ex;
		}

		// See if the file already exists and if it does move the file pointer 
		// to the end of the file
		if (::GetLastError() == ERROR_ALREADY_EXISTS)
			::SetFilePointer(hFile, 0, 0, FILE_END);

		m_hFile = hFile;
	}

	//-----------------------------------------------------------------------------
	// Reads data from a file associated with the CFFile object, 
	// starting at the position indicated by the file pointer.
	//-----------------------------------------------------------------------------
	DWORD CFFile::Read(void* lpBuf, DWORD nCount, LPOVERLAPPED lpOverlapped)
	{
		_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));

		// Avoid a null read operation, since the behavior of a null
		// read operation depends on the underlying file system
		if (nCount == 0)
			return 0;   

		_ASSERT(lpBuf);

		// Read data from the file
		DWORD dwRead;
		if (!::ReadFile(m_hFile, lpBuf, nCount, &dwRead, lpOverlapped))
		{
			CXFileException ex((long)::GetLastError());
			throw ex;
		}

		return dwRead;
	}

	//-----------------------------------------------------------------------------
	// Writes data from a buffer to the file associated with the CFFile object
	//-----------------------------------------------------------------------------
	void CFFile::Write(const void* lpBuf, DWORD nCount)
	{
		_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));
		_ASSERT(lpBuf);

		// Avoid a null write operation, since the behavior of a null
		// write operation depends on the underlying file system
		if (nCount == 0)
			return;

		// Write data to a file
		DWORD dwWritten;
		if (!::WriteFile(m_hFile, lpBuf, nCount, &dwWritten, NULL))
		{
			CXFileException ex((long)::GetLastError());
			throw ex;
		}
	}

	//-----------------------------------------------------------------------------
	// Writes "\r\n" combination at the end of the line
	//-----------------------------------------------------------------------------
	void CFFile::WriteEndOfLine()
	{
		_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));

		DWORD dwWritten;
		CString strCRLF = TEXT("\r\n");	// \r\n at the end of each line

		// Append CR-LF pair 
		if (!::WriteFile(m_hFile, strCRLF, strCRLF.GetLength(), &dwWritten, NULL))
		{
			CXFileException ex((long)::GetLastError());
			throw ex;
		}
	}

	//-----------------------------------------------------------------------------
	// Repositions the pointer in a previously opened file.
	//-----------------------------------------------------------------------------
	DWORD CFFile::Seek(LONG lOff, UINT nFrom)
	{
		FTLASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));
		FTLASSERT(nFrom == begin || nFrom == end || nFrom == current);
		FTLASSERT(begin == FILE_BEGIN && end == FILE_END && current == FILE_CURRENT);

		DWORD dwNew = ::SetFilePointer(m_hFile, lOff, NULL, (DWORD)nFrom);
		API_ASSERT((DWORD)-1 != dwNew);
	
		return dwNew;
	}

	//-----------------------------------------------------------------------------
	// Obtains the current value of the file pointer, which can be used in 
	// subsequent calls to Seek.
	//-----------------------------------------------------------------------------
	DWORD CFFile::GetPosition() const
	{
		_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));

		DWORD dwPos = ::SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT);
		API_ASSERT(dwPos  != (DWORD)-1);
		return dwPos;
	}

	//-----------------------------------------------------------------------------
	// Flushes any data yet to be written
	//-----------------------------------------------------------------------------
	BOOL CFFile::Flush()
	{
		if (NULL == m_hFile || INVALID_HANDLE_VALUE == m_hFile)
		{
			return TRUE;
		}

		BOOL bRet = FALSE;
		API_VERIFY(::FlushFileBuffers(m_hFile));
		return bRet;
	}

	//-----------------------------------------------------------------------------
	// Closes the file associated with CFFile object and makes the file 
	// unavailable for reading or writing.
	//-----------------------------------------------------------------------------
	BOOL CFFile::Close()
	{
		BOOL bRet = TRUE;
		if (INVALID_HANDLE_VALUE != m_hFile )
		{
			API_VERIFY(::CloseHandle(m_hFile));
			m_hFile = INVALID_HANDLE_VALUE;
		}
		m_strFileName.Empty();
		return bRet;
	}

	//-----------------------------------------------------------------------------
	// Locks a range of bytes in an open file, throwing an exception if the file is 
	// already locked. Locking bytes in a file prevents access to those bytes by other processes.
	//-----------------------------------------------------------------------------
	BOOL CFFile::LockRange(DWORD dwPos, DWORD dwCount)
	{
		FTLASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));
		BOOL bRet = FALSE;

		API_VERIFY(::LockFile(m_hFile, dwPos, 0, dwCount, 0));
		return bRet;
	}

	//-----------------------------------------------------------------------------
	// Unlocks a range of bytes in an open file. 
	//-----------------------------------------------------------------------------
	BOOL CFFile::UnlockRange(DWORD dwPos, DWORD dwCount)
	{
		FTLASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));
		BOOL bRet = FALSE;
		API_VERIFY(::UnlockFile(m_hFile, dwPos, 0, dwCount, 0));
		return bRet;
	}

	//-----------------------------------------------------------------------------
	// Changes the length of the file.
	//-----------------------------------------------------------------------------
	BOOL CFFile::SetLength(DWORD dwNewLen)
	{
		FTLASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));
		BOOL bRet = FALSE;
		Seek(dwNewLen, CFFile::begin);

		// Move the end-of-file (EOF) position for the file to the current position 
		// of the file pointer. 
		API_VERIFY(::SetEndOfFile(m_hFile));
		return bRet;
	}

	//-----------------------------------------------------------------------------
	// Obtains the current logical length of the file in bytes
	//-----------------------------------------------------------------------------
	DWORD CFFile::GetLength() const
	{
		CFFile * pFile = (CFFile*)this;

		return pFile->SeekToEnd();
	}

	//-----------------------------------------------------------------------------
	// Renames the specified file. Directories cannot be renamed
	//-----------------------------------------------------------------------------
	BOOL CFFile::Rename(CString strOldName, CString strNewName)
	{
		BOOL bRet = FALSE;
		API_VERIFY(::MoveFile(strOldName, strNewName));
		return bRet;
	}

	//-----------------------------------------------------------------------------
	// Deletes the specified file.
	//-----------------------------------------------------------------------------
	BOOL CFFile::Remove(CString strFileName)
	{
		BOOL bRet = FALSE;
		API_VERIFY(::DeleteFile(strFileName));
		return bRet;
	}

	//-----------------------------------------------------------------------------
	// Sets the value of the file pointer to the end of the file.
	//-----------------------------------------------------------------------------
	DWORD CFFile::SeekToEnd()
	{ 
		return Seek(0, CFFile::end); 
	}

	//-----------------------------------------------------------------------------
	// Sets the value of the file pointer to the beginning of the file.
	//-----------------------------------------------------------------------------
	DWORD CFFile::SeekToBegin()
	{ 
		return Seek(0, CFFile::begin);
	}

	//-----------------------------------------------------------------------------
	// Sets the full file path of the selected file, for example, if the path of a 
	// file is not available when a CFFile object is constructed, call SetFilePath to 
	// provide it.
	// SetFilePath does not open the file or create the file; it simply associates the 
	// CFFile object with a path name, which can then be used.
	//-----------------------------------------------------------------------------
	BOOL CFFile::SetFilePath(CString strNewName)
	{
		FTLASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));

		// Make sure it's a file path
		int nPos = strNewName.Find(TEXT(":\\"));
		if (nPos == -1)
		{
			FTLASSERT(FALSE);	
		}

		m_strFileName = strNewName;
	}

	//-----------------------------------------------------------------------------
	// Retrieves the full file path of the selected file.
	//-----------------------------------------------------------------------------
	CString CFFile::GetFilePath() const
	{
		_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));
		_ASSERT(!m_strFileName.IsEmpty());		// file name should not be empty

		return m_strFileName;
	}

	//-----------------------------------------------------------------------------
	// Retrieves the filename of the selected file.
	//-----------------------------------------------------------------------------
	CString CFFile::GetFileName() const
	{
		_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));
		_ASSERT(!m_strFileName.IsEmpty());		// file name should not be empty

		// Always capture the complete file name including extension
		LPTSTR lpszTemp = (LPTSTR)(LPCTSTR)(m_strFileName);
		for (LPCTSTR lpsz = m_strFileName; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
		{
			// Remember last directory/drive separator
			if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
				lpszTemp = const_cast<LPTSTR>(_tcsinc(lpsz));
		}

		return CString(lpszTemp);
	}

	//-----------------------------------------------------------------------------
	// CFFile does not support direct buffering
	//-----------------------------------------------------------------------------
	UINT CFFile::GetBufferPtr(UINT nCommand, UINT nCount, void** ppBufStart, void** ppBufMax)
	{
		_ASSERT(nCommand == bufferCheck);

		return 0;   // no support
	}

    CFStructuredStorageFile::CFStructuredStorageFile()
    {
        m_pRootStg = NULL;
        m_pCurrentStg = NULL;
    }

    virtual CFStructuredStorageFile::~CFStructuredStorageFile()
    {
        Close();
    }

    HRESULT CFStructuredStorageFile::CreateDocFile(LPCTSTR pszFilePath, 
        DWORD mode /* = STGM_CREATE | STRUCTURED_STORAGE_FILE_DEFAULT_MODE*/)
    {
        CHECK_POINTER_RETURN_VALUE_IF_FAIL(pszFilePath,E_POINTER);
        HRESULT hr = E_FAIL;

        FTLASSERT(NULL == m_pRootStg);
        if (m_pRootStg)
        {
            return HRESULT_FROM_WIN32(ERROR_INVALID_STATE);
        }

        USES_CONVERSION;
        COM_VERIFY(::StgCreateDocfile(CT2COLE(pszFilePath),mode,0,&m_pRootStg));
        if (SUCCEEDED(hr))
        {
            StorageData *sd = new StorageData;
            sd->Stg = m_pRootStg;
            sd->ParentStg = NULL;
            m_pCurrentStg = sd;
        }
        return hr;
    }
    HRESULT CFStructuredStorageFile::OpenDocFile(LPCTSTR pszFilePath,
        DWORD mode/* = STRUCTURED_STORAGE_FILE_DEFAULT_MODE*/)
    {
        HRESULT hr = E_FAIL;
        FTLASSERT(NULL == m_pRootStg);

        if (m_pRootStg)
        {
            return HRESULT_FROM_WIN32(ERROR_INVALID_STATE);
        }
        COM_VERIFY(::StgOpenStorage(CT2COLE(pszFilePath),NULL,
            mode,NULL,0,&m_pRootStg));

        if (SUCCEEDED(hr))
        {
            StorageData *sd = new StorageData;
            sd->Stg = m_pRootStg;
            sd->ParentStg = NULL;
            m_pCurrentStg = sd;
        }
        return hr;
    }

    void CFStructuredStorageFile::Close()
    {
        HRESULT hr = S_OK;
        if (m_pRootStg)
        {
            // Release all pointers in the open tree:
            while (S_OK == ExitStorage())
                ;

            m_pCurrentStg->Stg->Release();
            delete m_pCurrentStg;

            m_pCurrentStg = NULL;
            m_pRootStg = NULL;
        }
    }

    IStorage* CFStructuredStorageFile::Attach(IStorage* pNews)
    {
        FTLASSERT(pNews != NULL);

        // store the old root storage:
        IStorage* old = m_pRootStg;
        while (S_OK == ExitStorage())
            ;	

        // set up the new one:
        m_pRootStg = pNews;
        m_pCurrentStg = new StorageData;
        m_pCurrentStg->ParentStg = NULL;
        m_pCurrentStg->Stg = pNews;

        // return the old one:
        return old;
    }

    IStorage* CFStructuredStorageFile::Detach()
    {
        while (S_OK == ExitStorage())
            ;

        IStorage* rtn = m_pRootStg;
        m_pCurrentStg = NULL;
        m_pRootStg = NULL;
        return rtn;
    }

    // storage-level access:

    HRESULT CFStructuredStorageFile::CreateStorage(LPCTSTR pName,BOOL bEnter/* = TRUE*/, 
        DWORD mode/* = STRUCTURED_STORAGE_FILE_DEFAULT_MODE*/)
    {
        HRESULT hr = E_NOTIMPL;
        FTLASSERT(NULL != m_pRootStg);
        FTLASSERT(NULL != m_pCurrentStg);

        USES_CONVERSION;
        IStorage* pNewStorage;
        COM_VERIFY(m_pCurrentStg->Stg->CreateStorage(T2COLE(pName), mode, 0, 0, &pNewStorage));
        if (SUCCEEDED(hr))
        {
            if (!bEnter)
            {
                pNewStorage->Release();
            }
            else
            {
                StorageData* pSD = new StorageData;
                pSD->ParentStg = m_pCurrentStg;
                pSD->Stg = pNewStorage;
                m_pCurrentStg = pSD;
            }
        }
        return hr;
    }

    HRESULT CFStructuredStorageFile::EnterStorage(LPCTSTR pName,DWORD mode/* = STRUCTURED_STORAGE_FILE_DEFAULT_MODE*/)
    {
        FTLASSERT(m_pCurrentStg);
        FTLASSERT(m_pRootStg);

        HRESULT hr = E_NOTIMPL;
        IStorage* pStg = NULL;
        USES_CONVERSION;

        COM_VERIFY(m_pCurrentStg->Stg->OpenStorage(T2COLE(pName), NULL, mode, NULL, 0, &pStg));
        if(SUCCEEDED(hr))
        {
            StorageData* pSD = new StorageData;
            pSD->ParentStg = m_pCurrentStg;
            pSD->Stg = pStg;

            m_pCurrentStg = pSD;
        }
        return hr;
    }

    HRESULT CFStructuredStorageFile::ExitStorage()
    {
        FTLASSERT(m_pCurrentStg);
        FTLASSERT(m_pRootStg);

        if (m_pCurrentStg->ParentStg)
        {
            m_pCurrentStg->Stg->Release();
            StorageData* pSD = m_pCurrentStg->ParentStg;
            delete m_pCurrentStg;
            m_pCurrentStg = pSD;
            return S_OK;
        }
        else
        {
            return HRESULT_FROM_WIN32(ERROR_NO_MORE_ITEMS); // no storage to exit out of without closing the file
        }
    }

    //bool CreateStream(const CString & name, COleStreamFile &sf, DWORD mode = CFile::modeReadWrite | CFile::shareExclusive);
    //bool OpenStream(const CString & name, COleStreamFile &sf, DWORD mode = CFile::modeReadWrite | CFile::shareExclusive);

    HRESULT CFStructuredStorageFile::CreateStream(LPCTSTR pName,IStream** ppChildStream,
        DWORD mode /*= STRUCTURED_STORAGE_FILE_DEFAULT_MODE*/)
    {
        FTLASSERT(m_pCurrentStg);
        FTLASSERT(m_pRootStg);

        HRESULT hr = E_FAIL;
        *ppChildStream = NULL;
        if (m_pCurrentStg->Stg)
        {
            COM_VERIFY(m_pCurrentStg->Stg->CreateStream(CT2COLE(pName),mode,0,0,ppChildStream));
        }
        return hr;
    }

    HRESULT CFStructuredStorageFile::OpenStream(LPCTSTR pName,IStream** ppChildStream,
        DWORD mode /* = STRUCTURED_STORAGE_FILE_DEFAULT_MODE*/)
    {
        FTLASSERT(m_pCurrentStg);
        FTLASSERT(m_pRootStg);

        HRESULT hr = E_FAIL;
        *ppChildStream = NULL;
        if (m_pCurrentStg->Stg)
        {
            COM_VERIFY(m_pCurrentStg->Stg->OpenStream(CT2COLE(pName),NULL,mode,0,ppChildStream));
        }
        return hr;
    }

    HRESULT CFStructuredStorageFile::DestroyElement(LPCTSTR pName)
    {
        FTLASSERT(m_pCurrentStg);
        HRESULT hr = E_NOTIMPL;
        USES_CONVERSION;
        COM_VERIFY(m_pCurrentStg->Stg->DestroyElement(T2COLE(pName)));
        return hr;
    }

    // status info:
    IStorage* CFStructuredStorageFile::GetRootStorage() const
    {
        FTLASSERT(m_pRootStg);
        return m_pRootStg;
    }

    IStorage* CFStructuredStorageFile::GetCurrentStorage() const
    {
        FTLASSERT(m_pCurrentStg);
        return m_pCurrentStg->Stg;
    }
    BOOL CFStructuredStorageFile::IsOpen() const
    {
        BOOL bRet = (NULL != m_pRootStg);
        return  bRet;
    }
    //CString GetPath(const CString & SepChar) const
    //{
    //    ASSERT(m_pCurrentStg && m_bOpen);
    //    StorageData* pSD = m_pCurrentStg;
    //    // loop through each storage in the tree and concatenate names along with the
    //    // separator character:
    //    CString strPath;
    //    while (pSD->ParentStg)
    //    {
    //        STATSTG sg;
    //        pSD->Stg->Stat(&sg, STATFLAG_DEFAULT);
    //        CString strTemp = sg.pwcsName;
    //        CoTaskMemFree((void *)sg.pwcsName);
    //        strPath = strTemp + SepChar + strPath;
    //        pSD = pSD->ParentStg; // up a level for next interation
    //    }
    //    strPath = m_strFilename + SepChar + strPath;
    //    return strPath;
    //}

    //CString GetFilename() const;
}

#endif //FTL_FILE_HPP