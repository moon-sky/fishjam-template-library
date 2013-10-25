#ifndef FTL_FILE_HPP
#define FTL_FILE_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlfile.h"
#endif
#include "ftlConversion.h"

#include <atlpath.h>

namespace FTL
{
    LPCTSTR CFFileUtil::GetFileDesiredAccessFlagsString(FTL::CFStringFormater& formater, DWORD dwDesiredAccess, LPCTSTR pszDivide /* = TEXT("|") */)
    {
        DWORD oldDesiredAccess = dwDesiredAccess;

        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, FILE_READ_DATA, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, FILE_WRITE_DATA, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, FILE_APPEND_DATA, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, FILE_READ_EA, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, FILE_WRITE_EA, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, FILE_EXECUTE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, FILE_DELETE_CHILD, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, FILE_READ_ATTRIBUTES, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, FILE_WRITE_ATTRIBUTES, pszDivide);

        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, DELETE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, READ_CONTROL, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, WRITE_DAC, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, WRITE_OWNER, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, SYNCHRONIZE, pszDivide);

        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, ACCESS_SYSTEM_SECURITY, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, MAXIMUM_ALLOWED, pszDivide);


        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, GENERIC_READ, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, GENERIC_WRITE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, GENERIC_EXECUTE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, GENERIC_ALL, pszDivide);

        FTLASSERT( 0 == dwDesiredAccess);
        //HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwDesiredAccess, XXXXXXXXX, pszDivide);
        if (0 != dwDesiredAccess)
        {
            FTLTRACEEX(FTL::tlWarning, TEXT("%s:Get File Desired AccessFlags String Not Complete, total=0x%08x, remain=0x%08x\n"),
                __FILE__LINE__, oldDesiredAccess, dwDesiredAccess);
        }
        return formater.GetString();
    }

    BOOL CFFileUtil::DumpAllAPISystemFolderPath(DWORD dwSHGetFolderPathFlags/* = SHGFP_TYPE_DEFAULT */)
    {
        BOOL bRet = FALSE;
        HRESULT hr = E_FAIL;
        TCHAR szFolderPath[MAX_PATH] = {0};

        typedef std::pair<int, LPCTSTR> CSIDL2NameInfoPair;
        CSIDL2NameInfoPair checkClsids[] = {
            std::make_pair(CSIDL_DESKTOP, TEXT("CSIDL_DESKTOP")),           //%USERPROFILE%\Desktop
            std::make_pair(CSIDL_INTERNET, TEXT("CSIDL_INTERNET")),         //E_FAIL
            std::make_pair(CSIDL_PROGRAMS, TEXT("CSIDL_PROGRAMS")),         //%APPDATA%\Microsoft\Windows\Start Menu\Programs
            std::make_pair(CSIDL_CONTROLS, TEXT("CSIDL_CONTROLS")),         //E_FAIL
            std::make_pair(CSIDL_PRINTERS, TEXT("CSIDL_PRINTERS")),         //E_FAIL
            std::make_pair(CSIDL_PERSONAL, TEXT("CSIDL_PERSONAL")),         //%USERPROFILE%\Documents
            std::make_pair(CSIDL_FAVORITES, TEXT("CSIDL_FAVORITES")),       //%USERPROFILE%\Favorites
            std::make_pair(CSIDL_STARTUP, TEXT("CSIDL_STARTUP")),           //%APPDATA%\Microsoft\Windows\Start Menu\Programs\StartUp
            std::make_pair(CSIDL_RECENT, TEXT("CSIDL_RECENT")),             //%APPDATA%\Microsoft\Windows\Recent
            std::make_pair(CSIDL_SENDTO, TEXT("CSIDL_SENDTO")),             //%APPDATA%\Microsoft\Windows\SendTo
            std::make_pair(CSIDL_BITBUCKET, TEXT("CSIDL_BITBUCKET")),       //E_FAIL
            std::make_pair(CSIDL_STARTMENU, TEXT("CSIDL_STARTMENU")),       //%APPDATA%\Microsoft\Windows\Start Menu
            std::make_pair(CSIDL_MYDOCUMENTS, TEXT("CSIDL_MYDOCUMENTS")),   //%USERPROFILE%\Documents
            std::make_pair(CSIDL_MYMUSIC, TEXT("CSIDL_MYMUSIC")),           //%USERPROFILE%\Music
            std::make_pair(CSIDL_MYVIDEO, TEXT("CSIDL_MYVIDEO")),           //%USERPROFILE%\Videos
            std::make_pair(CSIDL_DESKTOPDIRECTORY, TEXT("CSIDL_DESKTOPDIRECTORY")), //%USERPROFILE%\Desktop
            std::make_pair(CSIDL_DRIVES, TEXT("CSIDL_DRIVES")),             //E_FAIL
            std::make_pair(CSIDL_NETWORK, TEXT("CSIDL_NETWORK")),           //E_FAIL
            std::make_pair(CSIDL_NETHOOD, TEXT("CSIDL_NETHOOD")),           //%APPDATA%\Microsoft\Windows\Network Shortcuts
            std::make_pair(CSIDL_FONTS, TEXT("CSIDL_FONTS")),               //%SystemRoot%\Fonts
            std::make_pair(CSIDL_TEMPLATES, TEXT("CSIDL_TEMPLATES")),       //%APPDATA%\Microsoft\Windows\Templates
            std::make_pair(CSIDL_COMMON_STARTMENU, TEXT("CSIDL_COMMON_STARTMENU")), //%ProgramData%\Microsoft\Windows\Start Menu
            std::make_pair(CSIDL_COMMON_PROGRAMS, TEXT("CSIDL_COMMON_PROGRAMS")),   //%ProgramData%\Microsoft\Windows\Start Menu\Programs
            std::make_pair(CSIDL_COMMON_STARTUP, TEXT("CSIDL_COMMON_STARTUP")),     //%ProgramData%\Microsoft\Windows\Start Menu\Programs\StartUp
            std::make_pair(CSIDL_COMMON_DESKTOPDIRECTORY, TEXT("CSIDL_COMMON_DESKTOPDIRECTORY")),   //%PUBLIC%\Desktop
            std::make_pair(CSIDL_APPDATA, TEXT("CSIDL_APPDATA")),           //%APPDATA%
            std::make_pair(CSIDL_PRINTHOOD, TEXT("CSIDL_PRINTHOOD")),       //%APPDATA%\Microsoft\Windows\Printer Shortcuts
            std::make_pair(CSIDL_LOCAL_APPDATA, TEXT("CSIDL_LOCAL_APPDATA")),   //%LOCALAPPDATA%
            std::make_pair(CSIDL_ALTSTARTUP, TEXT("CSIDL_ALTSTARTUP")),     //%APPDATA%\Microsoft\Windows\Start Menu\Programs\StartUp
            std::make_pair(CSIDL_COMMON_ALTSTARTUP, TEXT("CSIDL_COMMON_ALTSTARTUP")),   //%ProgramData%\Microsoft\Windows\Start Menu\Programs\StartUp
            std::make_pair(CSIDL_COMMON_FAVORITES, TEXT("CSIDL_COMMON_FAVORITES")), //%USERPROFILE%\Favorites

            std::make_pair(CSIDL_INTERNET_CACHE, TEXT("CSIDL_INTERNET_CACHE")), //%LOCALAPPDATA%\Microsoft\Windows\Temporary Internet Files
            std::make_pair(CSIDL_COOKIES, TEXT("CSIDL_COOKIES")),           //%APPDATA%\Microsoft\Windows\Cookies
            std::make_pair(CSIDL_HISTORY, TEXT("CSIDL_HISTORY")),           //%LOCALAPPDATA%\Microsoft\Windows\History
            std::make_pair(CSIDL_COMMON_APPDATA, TEXT("CSIDL_COMMON_APPDATA")), //%ProgramData%
            std::make_pair(CSIDL_WINDOWS, TEXT("CSIDL_WINDOWS")),               //%SystemRoot%
            std::make_pair(CSIDL_SYSTEM, TEXT("CSIDL_SYSTEM")),                 //%SystemRoot%\System32
            std::make_pair(CSIDL_PROGRAM_FILES, TEXT("CSIDL_PROGRAM_FILES")),   //%ProgramFiles(x86)% -- 32
            std::make_pair(CSIDL_MYPICTURES, TEXT("CSIDL_MYPICTURES")),         //%USERPROFILE%\Pictures
            std::make_pair(CSIDL_PROFILE, TEXT("CSIDL_PROFILE")),               //%USERPROFILE%
            std::make_pair(CSIDL_SYSTEMX86, TEXT("CSIDL_SYSTEMX86")),           //%SystemRoot%\SysWOW64
            std::make_pair(CSIDL_PROGRAM_FILESX86, TEXT("CSIDL_PROGRAM_FILESX86")), //%ProgramFiles(x86)%
            std::make_pair(CSIDL_PROGRAM_FILES_COMMON, TEXT("CSIDL_PROGRAM_FILES_COMMON")),     //%CommonProgramFiles(x86)%
            std::make_pair(CSIDL_PROGRAM_FILES_COMMONX86, TEXT("CSIDL_PROGRAM_FILES_COMMONX86")),   //%CommonProgramFiles(x86)%
            std::make_pair(CSIDL_COMMON_TEMPLATES, TEXT("CSIDL_COMMON_TEMPLATES")), //%ProgramData%\Microsoft\Windows\Templates
            std::make_pair(CSIDL_COMMON_DOCUMENTS, TEXT("CSIDL_COMMON_DOCUMENTS")), //%PUBLIC%\Documents
            std::make_pair(CSIDL_COMMON_ADMINTOOLS, TEXT("CSIDL_COMMON_ADMINTOOLS")),   //%ProgramData%\Microsoft\Windows\Start Menu\Programs\Administrative Tools
            std::make_pair(CSIDL_ADMINTOOLS, TEXT("CSIDL_ADMINTOOLS")),                 //%APPDATA%\Microsoft\Windows\Start Menu\Programs\Administrative Tools
            std::make_pair(CSIDL_CONNECTIONS, TEXT("CSIDL_CONNECTIONS")),       //E_FAIL
            std::make_pair(CSIDL_COMMON_MUSIC, TEXT("CSIDL_COMMON_MUSIC")),     //%PUBLIC%\Music
            std::make_pair(CSIDL_COMMON_PICTURES, TEXT("CSIDL_COMMON_PICTURES")),   //%PUBLIC%\Pictures
            std::make_pair(CSIDL_COMMON_VIDEO, TEXT("CSIDL_COMMON_VIDEO")),     //%PUBLIC%\Videos
            std::make_pair(CSIDL_RESOURCES, TEXT("CSIDL_RESOURCES")),           //%SystemRoot%\resources
            std::make_pair(CSIDL_RESOURCES_LOCALIZED, TEXT("CSIDL_RESOURCES_LOCALIZED")),   //%SystemRoot%\resources\0804 -- ÖÐÎÄ
            std::make_pair(CSIDL_COMMON_OEM_LINKS, TEXT("CSIDL_COMMON_OEM_LINKS")), //%ProgramData%\OEM Links
            std::make_pair(CSIDL_CDBURN_AREA, TEXT("CSIDL_CDBURN_AREA")),           //%LOCALAPPDATA%\Microsoft\Windows\Burn\Burn
            std::make_pair(CSIDL_COMPUTERSNEARME, TEXT("CSIDL_COMPUTERSNEARME")),   //E_FAIL

            //std::make_pair(xxxxxx, TEXT("xxxxx")),
        };

        FTLTRACEEX(tlTrace, TEXT("SHGetFolderPath Folders are:\n"));
        for (int i = 0; i <_countof(checkClsids); i++)
        {
            COM_VERIFY_EXCEPT1(SHGetFolderPath(NULL, checkClsids[i].first, NULL, dwSHGetFolderPathFlags, szFolderPath), E_FAIL);
            if (SUCCEEDED(hr))
            {
                FTLTRACEEX(tlTrace, TEXT("%s => %s\n"), checkClsids[i].second, szFolderPath);
            }
            else
            {
                FTLTRACEEX(tlWarning, TEXT("SHGetFolderPath Get %s Failed(0x%x)\n"), checkClsids[i].second, hr);
            }
        }
        
        API_VERIFY(0 != GetTempPath(_countof(szFolderPath), szFolderPath));
        if (bRet)
        {
            FTLTRACEEX(tlTrace, TEXT("GetTempPath => %s\n"), szFolderPath);
        }

        return bRet;
    }

 	TextFileEncoding CFFileUtil::GetTextFileEncoding(LPCTSTR pszFilePath)
	{
		BOOL bRet = FALSE;
		TextFileEncoding encoding = tfeError;
		if (pszFilePath)
		{
			BYTE header[3] = {0};
			HANDLE hFile = ::CreateFile(pszFilePath, GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			API_VERIFY(hFile != INVALID_HANDLE_VALUE);

			if (hFile != INVALID_HANDLE_VALUE)
			{
				DWORD dwFileSize = ::GetFileSize( hFile, NULL );
				DWORD dwReadSize = FTL_MIN(sizeof(header), dwFileSize);
				API_VERIFY(ReadFile(hFile, header, dwReadSize, &dwReadSize, NULL));
				CloseHandle(hFile);

				if (bRet)
				{
					//read success
					if (memcmp(TEXT_FILE_HEADER_UTF8, header, sizeof(TEXT_FILE_HEADER_UTF8)) == 0)
					{
						encoding = tfeUTF8;
					}
					else if(memcmp(TEXT_FILE_HEADER_UNICODE, header, sizeof(TEXT_FILE_HEADER_UNICODE)) == 0)
					{
						encoding = tfeUnicode;
					}
					else if(memcmp(TEXT_FILE_HEADER_UNICODE_BIG_ENDIAN, header, sizeof(TEXT_FILE_HEADER_UNICODE_BIG_ENDIAN)) == 0)
					{
						encoding = tfeUnicodeBigEndian;
					}
					else
					{
						encoding = tfeUnknown;
					}
				}
			}
		}
		return encoding;
	}

	HANDLE CFFileUtil::CreateLocalWriteFile(__inout LPTSTR pszFilePath, DWORD dwMaxSize, CreateLocalFileFlags flags)
	{
		DWORD dwCreationDisposition = 0;
		switch (flags)
		{
		case clfCreateIfNotExist:
			dwCreationDisposition = CREATE_NEW;
			break;
		case clfAutoRename:
			dwCreationDisposition = CREATE_NEW;
			break;
		case clfOverwrite:
			dwCreationDisposition = CREATE_ALWAYS;
			break;
		}

		HANDLE hLocalFile = ::CreateFile(pszFilePath, 
			GENERIC_READ | GENERIC_WRITE, 
			0, 
			NULL,
			dwCreationDisposition, 
			FILE_ATTRIBUTE_NORMAL, 
			NULL);
		if (INVALID_HANDLE_VALUE == hLocalFile && clfAutoRename == flags)
		{
			//Can not create File,
			TCHAR szLocalFilePath[_MAX_PATH] = {0};
			TCHAR szDrive[_MAX_DRIVE]  = {0};
			TCHAR szDir[_MAX_DIR] = {0};
			TCHAR szFileName[_MAX_FNAME] = {0};
			TCHAR szExt[_MAX_EXT] = {0};

			_tsplitpath_s(pszFilePath, szDrive, szDir, szFileName, szExt);

			int nIndex = 1;
			CFStringFormater strFileName;
			while ((nIndex <= 999) && (INVALID_HANDLE_VALUE == hLocalFile))
			{
				strFileName.Format(TEXT("%s(%d)"), szFileName, nIndex);
				_tmakepath_s(szLocalFilePath, szDrive, szDir, strFileName.GetString(), szExt);

				hLocalFile = ::CreateFile(szLocalFilePath, 
					GENERIC_WRITE, 
					0, 
					NULL,
					CREATE_NEW, 
					FILE_ATTRIBUTE_NORMAL, 
					NULL);
				if (INVALID_HANDLE_VALUE != hLocalFile)
				{
					//change file path
					StringCchCopy(pszFilePath, dwMaxSize, szLocalFilePath);
					break;
				}
				nIndex++;
			}
		}

		return hLocalFile;
	}
#if 0
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
#endif 

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
			//CFFileException ex((long)::GetLastError());
			//throw ex;
			return NULL;
		}

		pFile->m_hFile = hFile;
		_ASSERT(pFile->m_hFile);

		return pFile;
	}

	//-----------------------------------------------------------------------------
	// Creates or opens a file
	//-----------------------------------------------------------------------------
	BOOL CFFile::Create(LPCTSTR pszFileName,			// file name 
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
			//CFFileException ex((long)::GetLastError());
			//throw ex;
			return FALSE;
		}

		// See if the file already exists and if it does move the file pointer 
		// to the end of the file
		if (::GetLastError() == ERROR_ALREADY_EXISTS)
			::SetFilePointer(hFile, 0, 0, FILE_END);

		m_hFile = hFile;
		return TRUE;
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
		DWORD dwRead = 0;
		if (!::ReadFile(m_hFile, lpBuf, nCount, &dwRead, lpOverlapped))
		{
			//CXFileException ex((long)::GetLastError());
			//throw ex;
			FTLASSERT(FALSE);
		}

		return dwRead;
	}

	//-----------------------------------------------------------------------------
	// Writes data from a buffer to the file associated with the CFFile object
	//-----------------------------------------------------------------------------
	BOOL CFFile::Write(const void* lpBuf, DWORD nCount, DWORD* pdwWritten)
	{
		_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));
		_ASSERT(lpBuf);

		// Avoid a null write operation, since the behavior of a null
		// write operation depends on the underlying file system
		if (nCount == 0)
		{
			return TRUE;
		}
        DWORD dwWritten = 0;
        DWORD* pLocalWritten = pdwWritten;
        if (!pLocalWritten)
        {
            pLocalWritten = &dwWritten;
        }
		// Write data to a file
		if (!::WriteFile(m_hFile, lpBuf, nCount, pLocalWritten, NULL))
		{
			//CXFileException ex((long)::GetLastError());
			//throw ex;
			return FALSE;
		}
		return TRUE;
	}

	//-----------------------------------------------------------------------------
	// Writes "\r\n" combination at the end of the line
	//-----------------------------------------------------------------------------
	BOOL  CFFile::WriteEndOfLine()
	{
		_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));

		DWORD dwWritten;
		CString strCRLF = TEXT("\r\n");	// \r\n at the end of each line

		// Append CR-LF pair 
		if (!::WriteFile(m_hFile, strCRLF, strCRLF.GetLength(), &dwWritten, NULL))
		{
			//CXFileException ex((long)::GetLastError());
			//throw ex;
			return FALSE;
		}
		return TRUE;
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
			return FALSE;
		}

		m_strFileName = strNewName;
		return TRUE;
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

	BOOL CFPath::CreateDirTree(LPCTSTR szPath)
	{
		TCHAR szDirName[MAX_PATH] = { 0 };
		const TCHAR* p = szPath;
		TCHAR* q = szDirName;

		while(*p)
		{
			if (('\\' == *p) || ('/' == *p))
			{
				if (':' != *(p - 1))
				{
					if (TRUE == ATLPath::FileExists(szDirName))
					{
						if (FALSE == ATLPath::IsDirectory(szDirName))
						{
							return FALSE;
						}
					}
					else
					{
						if (FALSE == CreateDirectory(szDirName, NULL))
						{
							FTLTRACEEX(tlError, _T("CreateDirectory fail %s\n"), szDirName);
							return FALSE;
						}
					}
				}
			}
			*q++ = *p++;
			*q = '\0';
		}

		int nPathLen = _tcslen(szPath);
		if (nPathLen > 3)
		{
			if (('\\' != *(q - 1)) && ('/' != *(q - 1)))
			{
				if (TRUE == ATLPath::FileExists(szDirName))
				{
					if (FALSE == ATLPath::IsDirectory(szDirName))
					{
						return FALSE;
					}
				}
				else
				{
					if (FALSE == CreateDirectory(szDirName, NULL))
					{
						return FALSE;
					}
				}
			}
		}

		return TRUE;
	}

    BOOL CFPath::GetRelativePath(LPCTSTR pszFullPath, LPCTSTR pszParentPath, LPTSTR pszRelateivePath, UINT cchMax)
    {
        BOOL bRet = FALSE;

        LPTSTR pszRelative = StrStrI(pszFullPath, pszParentPath);
        if (pszRelative)
        {
            pszRelative += lstrlen(pszParentPath);
            while (pszRelative && (*pszRelative == _T('\\')))
            {
                pszRelative++;
            }
            if (pszRelative)
            {
                lstrcpyn(pszRelateivePath, pszRelative, cchMax);
                bRet = TRUE;
            }
        }
        return bRet;
    }

    BOOL CFFileUTF8Encoding::WriteEncodingString(CFFile* pFile, const CAtlString& strValue, DWORD* pnBytesWritten)
    {
        BOOL bRet = FALSE;
        CFConversion conv;
        INT nLength = 0;
        LPCSTR pszUtf8 = conv.TCHAR_TO_UTF8(strValue, &nLength);
        API_VERIFY(pFile->Write(pszUtf8, (nLength) * sizeof(char), pnBytesWritten));
        return bRet;
    }
    BOOL CFFileUnicodeEncoding::WriteEncodingString(CFFile* pFile, const CAtlString& strValue, DWORD* pnBytesWritten)
    {
        BOOL bRet = FALSE;
        CFConversion conv;
        INT nLength = 0;
        LPCTSTR pszUtf16 = conv.TCHAR_TO_UTF16(strValue, &nLength);
        API_VERIFY(pFile->Write(pszUtf16, (nLength) * sizeof(WCHAR), pnBytesWritten));
        return bRet;
    }

    template <typename TEncoding>
    CFTextFile<TEncoding>::CFTextFile(TextFileEncoding fileEncoding)
        :m_fileEncoding(fileEncoding)
    {
    }

    template <typename TEncoding>
    BOOL CFTextFile<TEncoding>::WriteFileHeader()
    {
        BOOL bRet = FALSE;
        ULONGLONG nSize = GetLength();
        FTLASSERT(nSize == 0LL);
        DWORD nBytesWritten = 0;
        switch (m_fileEncoding)
        {
        case tfeUTF8:
            API_VERIFY(Write(TEXT_FILE_HEADER_UTF8, sizeof(TEXT_FILE_HEADER_UTF8), &nBytesWritten));
            FTLASSERT(nBytesWritten == sizeof(TEXT_FILE_HEADER_UTF8));
            break;
        case tfeUnicode:
            API_VERIFY(Write(TEXT_FILE_HEADER_UNICODE, sizeof(TEXT_FILE_HEADER_UNICODE), &nBytesWritten));
            FTLASSERT(nBytesWritten == sizeof(TEXT_FILE_HEADER_UNICODE));
            break;
        case tfeUnicodeBigEndian:
            API_VERIFY(Write(TEXT_FILE_HEADER_UNICODE_BIG_ENDIAN, sizeof(TEXT_FILE_HEADER_UNICODE_BIG_ENDIAN), &nBytesWritten));
            FTLASSERT(nBytesWritten == sizeof(TEXT_FILE_HEADER_UNICODE_BIG_ENDIAN));
            break;
        default:
            FTLASSERT(FALSE);
            break;
        }
        return bRet;
    }

    template <typename TEncoding>
    BOOL CFTextFile<TEncoding>::WriteString(const CAtlString&strValue, DWORD* pnBytesWritten)
    {
        return TEncoding::WriteEncodingString(this, strValue, pnBytesWritten);
    }


    //////////////////////////////////////////////////////////////////////////

    CFFileFinder::CFFileFinder()
    {
        m_pCallback = NULL;
    }

    VOID CFFileFinder::SetCallback(IFileFindCallback* pCallBack)
    {
        m_pCallback = pCallBack;
    }

    BOOL CFFileFinder::Find(LPCTSTR pszDirPath, 
        LPCTSTR pszFilter /* =_T("*.*") */, 
        BOOL bRecursive /* = TRUE */)
    {
        BOOL bRet = FALSE;
        FTLASSERT(m_pCallback);
        if (!m_pCallback)
        {
            SetLastError(ERROR_INVALID_PARAMETER);
            return FALSE;
        }
        m_strDirPath = pszDirPath;
        m_strFilter = pszFilter;

        m_FindDirs.push_back(m_strDirPath);
        
        FileFindResultHandle resultHandler = rhContinue;

        while (!m_FindDirs.empty() && rhContinue == resultHandler)
        {
            const CAtlString& strFindPath = m_FindDirs.front();
            CPath path(strFindPath);
            path.Append(m_strFilter);

            WIN32_FIND_DATA findData = { 0 };

            HANDLE hFind = NULL;
            API_VERIFY((hFind = FindFirstFile(path.m_strPath, &findData)) != INVALID_HANDLE_VALUE);
            if (bRet)
            {
                do 
                {
                    CPath pathFullFindResult(strFindPath);
                    pathFullFindResult.Append(findData.cFileName);

                    if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    {
                        if ((lstrcmpi(findData.cFileName, _T(".")) != 0) 
                            && (lstrcmpi(findData.cFileName, _T("..")) != 0))
                        {
                            resultHandler = m_pCallback->OnFindFile(pathFullFindResult.m_strPath, findData);
                            //normal dir 
                            if (bRecursive)
                            {
                                m_FindDirs.push_back(pathFullFindResult.m_strPath);
                            }
                        }
                    }
                    else
                    {
                        resultHandler = m_pCallback->OnFindFile(pathFullFindResult.m_strPath, findData);
                    }

                    API_VERIFY_EXCEPT1(FindNextFile(hFind, &findData), ERROR_NO_MORE_FILES);
                } while (bRet);
                API_VERIFY(FindClose(hFind));
            }
            m_FindDirs.pop_front();
        }

        return bRet;
    }

    CFDirectoryCopier::CFDirectoryCopier()
    {
        m_pCallback = NULL;
        m_nTotalSize = 0LL;
        m_nTotoalCopiedSize = 0LL;
        m_nCurCopyFileSize = 0LL;
        m_nFileCount = 0;
        m_nTotalCopiedFileCount = 0;
        m_nCopyFileIndex = 0;
        m_bFailIfExists = FALSE;
        m_bRecursive = TRUE;
        m_bCopyEmptyFolder = TRUE;
    }

    CFDirectoryCopier::~CFDirectoryCopier()
    {

    }
    BOOL CFDirectoryCopier::SetCallback(ICopyDirCallback* pCallback)
    {
        m_pCallback = pCallback;

        return TRUE;
    }
    BOOL CFDirectoryCopier::SetCopyEmptyFolder(BOOL bCopyEmptyFolder)
    {
        m_bCopyEmptyFolder = bCopyEmptyFolder;
        return TRUE;
    }
    BOOL CFDirectoryCopier::Start(LPCTSTR pszSourcePath, 
        LPCTSTR pszDestPath, 
        LPCTSTR pszFilter /* = _T("*.*") */, 
        BOOL bFailIfExists /* = FALSE */,
        BOOL bRecursive /* = TRUE */)
    {
        BOOL bRet = FALSE;
        m_strSrcDirPath = pszSourcePath;
        m_strDstDirPath = pszDestPath;
        m_strFilter = pszFilter;
        m_bFailIfExists = bFailIfExists;
        m_bRecursive = bRecursive;
    
        API_VERIFY(m_threadCopy.Start(_CopierThreadProc, this, TRUE));
        return bRet;
    }
    DWORD CFDirectoryCopier::_CopierThreadProc(LPVOID lpThreadParameter)
    {
        CFDirectoryCopier* pThis = static_cast<CFDirectoryCopier*>(lpThreadParameter);
        DWORD dwResult = pThis->_InnerCopierThreadProc();
        return dwResult;
    }
    DWORD CFDirectoryCopier::_InnerCopierThreadProc()
    {
        FTLThreadWaitType waitType = ftwtContinue;
        do 
        {
            waitType = _PrepareSourceFiles();
            if (waitType != ftwtContinue)
            {
                break;
            }
            waitType = _CopyFiles();
        } while (0);

        return 0;
    }
    FTLThreadWaitType CFDirectoryCopier::_PrepareSourceFiles()
    {
        BOOL bRet = FALSE;
        FTLThreadWaitType waitType = ftwtError;
        CFFileFinder finder;
        finder.SetCallback(this);
        API_VERIFY(finder.Find(m_strSrcDirPath, m_strFilter, m_bRecursive));
        if (bRet)
        {
            waitType = ftwtContinue;
        }
        return waitType;
    }

    FTLThreadWaitType CFDirectoryCopier::_CopyFiles()
    {
        BOOL bRet = FALSE;

        FTLThreadWaitType waitType = m_threadCopy.GetThreadWaitType(INFINITE);
        TCHAR szRelativePath[MAX_PATH] = {0};
        while (waitType == ftwtContinue && !m_sourceFiles.empty())
        {
            const SourceFileInfo& fileInfo = m_sourceFiles.front();
            API_VERIFY(CFPath::GetRelativePath(fileInfo.strFullPath, m_strSrcDirPath, szRelativePath, _countof(szRelativePath)));
            if (bRet)
            {
                m_nCopyFileIndex++;
                CPath pathTarget(m_strDstDirPath);
                pathTarget.Append(szRelativePath);
                if (fileInfo.isDirectory)
                {
                    API_VERIFY(CFPath::CreateDirTree(pathTarget.m_strPath));
                }
                else
                {
                    CAtlString strFileName = PathFindFileName(pathTarget.m_strPath);
                    pathTarget.RemoveFileSpec();
                    API_VERIFY(CFPath::CreateDirTree(pathTarget.m_strPath));
                    if (bRet)
                    {
                        m_strCurSrcFilePath = fileInfo.strFullPath;
                        pathTarget.Append(strFileName);
                        m_strCurDstFilePath = pathTarget.m_strPath;
                        API_VERIFY(CopyFile(m_strCurSrcFilePath, m_strCurDstFilePath, m_bFailIfExists));
                    }
                }
                if (bRet)
                {
                    m_nTotalCopiedFileCount++;
                    m_nTotoalCopiedSize += fileInfo.nFileSize;
                    m_nCurCopyFileSize = fileInfo.nFileSize;
                    _NotifyCallBack(ICopyDirCallback::cbtCopyFile);    
                }
                if (!bRet)
                {
                    _NotifyCallBack(ICopyDirCallback::cbtError, GetLastError());
                }
            }

            m_sourceFiles.pop_front();
        }
        return waitType;
    }

    BOOL CFDirectoryCopier::Stop()
    {
        BOOL bRet = FALSE;
        API_VERIFY(m_threadCopy.Stop());
        return bRet;
    }
    BOOL CFDirectoryCopier::WaitToEnd(DWORD dwMilliseconds /* = INFINITE */)
    {
        BOOL bRet = FALSE;
        API_VERIFY(m_threadCopy.Wait(dwMilliseconds));
        return bRet;
    }
    
    VOID CFDirectoryCopier::_NotifyCallBack(ICopyDirCallback::CallbackType type, DWORD dwError /* = 0 */)
    {
        if (m_pCallback)
        {
            switch (type)
            {
            case ICopyDirCallback::cbtBegin:
                m_pCallback->OnBegin(m_nTotalSize, m_nFileCount);
                break;
            case ICopyDirCallback::cbtCopyFile:
                m_pCallback->OnCopyFile(m_strCurSrcFilePath, m_strCurDstFilePath, 
                    m_nCopyFileIndex, m_nCurCopyFileSize, m_nTotoalCopiedSize);
                break;
            case ICopyDirCallback::cbtEnd:
                m_pCallback->OnEnd(TRUE, m_nTotoalCopiedSize, m_nTotalCopiedFileCount);
                break;
            case ICopyDirCallback::cbtError:
                m_pCallback->OnError(m_strCurSrcFilePath, m_strCurDstFilePath, dwError);
                break;
            }
        }
    }
    FileFindResultHandle CFDirectoryCopier::OnFindFile(LPCTSTR pszFilePath, const WIN32_FIND_DATA& findData)
    {
        LARGE_INTEGER fileSize;
        fileSize.HighPart = findData.nFileSizeHigh;
        fileSize.LowPart = findData.nFileSizeLow;

        BOOL isDirectory = ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
        if (!isDirectory || m_bCopyEmptyFolder)
        {
            SourceFileInfo fileInfo;
            fileInfo.strFullPath = pszFilePath;
            fileInfo.nFileSize = fileSize.QuadPart;
            fileInfo.isDirectory = isDirectory;

            m_sourceFiles.push_back(fileInfo);
            m_nTotalSize += fileSize.QuadPart;
            m_nFileCount ++;
        }
        return rhContinue;
        
    }

    //////////////////////////////////////////////////////////////////////////

    CFStructuredStorageFile::CFStructuredStorageFile()
    {
        m_pRootStg = NULL;
        m_pCurrentStg = NULL;
    }

    CFStructuredStorageFile::~CFStructuredStorageFile()
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
        //HRESULT hr = S_OK;
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