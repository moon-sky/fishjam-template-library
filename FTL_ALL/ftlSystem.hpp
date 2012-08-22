#ifndef FTL_SYSTEM_HPP
#define FTL_SYSTEM_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlsystem.h"
#endif

#include <ShlObj.h>

namespace FTL
{
    CFOSInfo::CFOSInfo()
    {
        BOOL bRet = FALSE;
		ZeroMemory(&m_OsInfo, sizeof(m_OsInfo));
        m_OsInfo.dwOSVersionInfoSize = sizeof(m_OsInfo);
        API_VERIFY(::GetVersionEx(&m_OsInfo));
        FTLTRACEEX(tlTrace,_T("GetVersionEx :{ dwPlatformId=%d, dwMajorVersion=%d, dwMinorVersion=%d,dwBuildNumber=%d,szCSDVersion=%s}\r\n"),
            m_OsInfo.dwPlatformId,m_OsInfo.dwMajorVersion,m_OsInfo.dwMinorVersion,m_OsInfo.dwBuildNumber,m_OsInfo.szCSDVersion);
    }

    CFOSInfo::OSType CFOSInfo::GetOSType() const
    {
        OSType osType = ostLowUnknown;
        DWORD dwOsVersion = m_OsInfo.dwPlatformId * 10000 + m_OsInfo.dwMajorVersion * 100 + m_OsInfo.dwMinorVersion;
        switch(dwOsVersion)
        {
        case 00000: //TODO
            FTLASSERT(FALSE);
            osType = ostLowUnknown;//ostWin32;
            break;
        case 10400:		
            osType = ostWin95;
            break;
        case 10410://OK
            {
                if (_tcscmp(m_OsInfo.szCSDVersion,_T(" A ")) == 0)
                {
                    osType = ostWin98_SE;
                }
                else
                {
                    osType = ostWin98;
                }
                break;
            }
        case 10490://OK
            osType = ostWinMe;
            break;
        case 20351:
            osType = ostWinNT3;
            break;
        case 20400:
            osType = ostWinNT4;
            break;
        case 20500://OK
            osType = ostWin2000;
            break;
        case 20501://OK
            osType = ostWinXP;
            break;
        case 20502:
            osType = ostWin2003;
            break;
        case 20600:
            osType = ostVista;
            break;
        case 20601:
            osType = ostWindows7;
            break;
        default:
            {
                _ASSERT(FALSE);
                if (dwOsVersion > 20601)
                {
                    osType = ostHighUnknown;
                }
                else
                {
                    osType = ostLowUnknown;
                }
            }
            break;
        }
        return osType;
    }

    // if NT without ServicePack, return TRUE but csCSDVersion is ""
    BOOL CFOSInfo::GetWinNTServicePackName(LPTSTR pszNTVersion,DWORD nSize)	const
    {
        HRESULT hr = E_FAIL;
        COM_VERIFY(StringCchCopy(pszNTVersion, nSize, m_OsInfo.szCSDVersion));
        return TRUE;
    }

    BOOL CFOSInfo::IsGreaterWinNT() const
    {
        BOOL bRet = (m_OsInfo.dwPlatformId >= VER_PLATFORM_WIN32_NT);
        return bRet;
    }

    DWORD CFOSInfo::GetNumberOfProcessors() const
    {
        SYSTEM_INFO sysinfo = {0};
        ::GetSystemInfo(&sysinfo);
        return sysinfo.dwNumberOfProcessors;
    }

    BOOL CFOSInfo::GetPhysicalBytes(DWORDLONG* pAvailablePhysicalBytes, DWORDLONG *pTotalPhysicalBytes) const
    {
        BOOL bRet = FALSE;
        HMODULE hKernel = LoadLibrary(_T("kernel32.dll"));
        API_VERIFY(NULL != hKernel);
        if(pAvailablePhysicalBytes)
        {
            *pAvailablePhysicalBytes = 0;
        }
        if (pTotalPhysicalBytes)
        {
            *pTotalPhysicalBytes = 0;
        }

        if (hKernel)
        {
            typedef BOOL (WINAPI *FuncGlobalMemoryStatusEx)(LPMEMORYSTATUSEX);
            FuncGlobalMemoryStatusEx pfn = (FuncGlobalMemoryStatusEx)GetProcAddress(hKernel, "GlobalMemoryStatusEx");
            API_VERIFY(NULL != pfn);
            if (pfn)
            {
                MEMORYSTATUSEX memoryStatus = {0};
                //memset(&memoryStatus, 0, sizeof(MEMORYSTATUSEX));
                memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
                API_VERIFY((*pfn)(&memoryStatus));
                if (bRet)
                {
                    if(pAvailablePhysicalBytes)
                    {
                        *pAvailablePhysicalBytes = memoryStatus.ullAvailPhys;
                    }
                    if (pTotalPhysicalBytes)
                    {
                        *pTotalPhysicalBytes = memoryStatus.ullTotalPhys;
                    }
                }
            }
            FreeLibrary(hKernel);
        }
        if (!bRet)
        {
            MEMORYSTATUS memoryStatus = {0};
            //memset(&memoryStatus, 0, sizeof(MEMORYSTATUS));
            memoryStatus.dwLength = sizeof(MEMORYSTATUS);
            GlobalMemoryStatus(&memoryStatus);
            if(pAvailablePhysicalBytes)
            {
                *pAvailablePhysicalBytes = memoryStatus.dwTotalPhys;
            }
            if (pTotalPhysicalBytes)
            {
                *pTotalPhysicalBytes = memoryStatus.dwTotalPhys;
            }
            bRet = TRUE;
        }
        return bRet;
    }

    BOOL CFOSInfo::GetVolumeVisibleName(LPCTSTR pszVolume, LPTSTR pszBuf, DWORD bufSize) const
    {
        HRESULT hr = E_FAIL;
        BOOL bRet = FALSE;

        IShellFolder* pDesktop = 0;
        COM_VERIFY(::SHGetDesktopFolder(&pDesktop));
        if (SUCCEEDED(hr))
        {
            WCHAR wPath[] = L"@:\\";
            LPITEMIDLIST pidl = NULL;
            ULONG eaten = 0;
            ULONG attributes = 0;

            USES_CONVERSION;

            wPath[0] = CT2W(pszVolume)[0];
            COM_VERIFY(pDesktop->ParseDisplayName(0, 0, wPath, &eaten, &pidl, &attributes));
            if (SUCCEEDED(hr))
            {
                STRRET strret = {0};
                if (SUCCEEDED(pDesktop->GetDisplayNameOf(pidl, SHGDN_INFOLDER, &strret)))
                {
                    LPTSTR str = 0;
                    if (SUCCEEDED(StrRetToStr(&strret, pidl, &str)))
                    {
                        COM_VERIFY(StringCchCopy(pszBuf,bufSize,str));
                        CoTaskMemFree(str);
                        bRet = TRUE;
                    }
                }
                IMalloc* pMalloc = 0;
                if (SUCCEEDED(SHGetMalloc(&pMalloc)))
                {
                    pMalloc->Free(pidl);
                    pMalloc->Release();
                }
            }
            pDesktop->Release();
        }
        if(!bRet)
        {
            SetLastError(HRESULT_CODE(hr));
        }
        return bRet;
    }

	SystemParamProperty::SystemParamProperty()
	{
		Init();
	}
	SystemParamProperty::~SystemParamProperty()
	{

	}
	BOOL SystemParamProperty::Init()
	{
		BOOL bRet = FALSE;
		API_VERIFY(::SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &m_nKeyboardSpeed, 0));

		return bRet;
	}

	LPCTSTR SystemParamProperty::GetPropertyString()
	{

	}

	BOOL CFSystemUtil::GetVersionFromString(VERSIONINFO& ver, LPCTSTR pszVersion)
	{
		if ( NULL  == pszVersion )
		{
			return FALSE;
		}

		const TCHAR szToken[] = _T(",.");
		const int  MAX_VERSION_SIZE	= 64 ;
		DWORD dwTemp[4] = { 0 };
		//memset(dwTemp, 0x00, sizeof(dwTemp));

		TCHAR *pszTok = NULL;
		TCHAR szVersion[MAX_VERSION_SIZE] = { 0 };
		StringCchCopy(szVersion, _countof(szVersion), pszVersion) ;
		pszTok = _tcstok(szVersion, szToken) ;
		if (pszTok == NULL)
		{
			return FALSE;
		}

		*dwTemp = DWORD(_ttoi(pszTok)) ;

		for (int nIndex = 1; nIndex < 4; nIndex++)
		{
			pszTok = _tcstok(NULL, szToken) ;
			if (pszTok == NULL)
			{
				return FALSE;
			}

			*(dwTemp + nIndex) = DWORD(_ttoi(pszTok)) ;
		}

		ver.dwVer0 = dwTemp[0];
		ver.dwVer1 = dwTemp[1];
		ver.dwVer2 = dwTemp[2];
		ver.dwVer3 = dwTemp[3];

		return TRUE;
	}

	BOOL CFSystemUtil::GetCurrentUserID( LPTSTR pszUserName, int iSize)
	{
		const TCHAR SUBKEY_LOGON_INFO[] = _T( "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Authentication\\LogonUI" );
		CRegKey RegUsers;
		if ( RegUsers.Open( HKEY_LOCAL_MACHINE, SUBKEY_LOGON_INFO, KEY_READ ) != ERROR_SUCCESS )
		{
			return FALSE;
		}

		ULONG ul = MAX_PATH;
		TCHAR szBuf[MAX_PATH] = { 0 };
		if ( RegUsers.QueryStringValue( _T("LastLoggedOnUser"), szBuf, &ul ) != ERROR_SUCCESS )
		{
			return FALSE;
		}

		HRESULT hr = E_FAIL;
		ZeroMemory( pszUserName, iSize );
		COM_VERIFY(StringCchCopy( pszUserName, iSize, ::PathFindFileName( szBuf )));

		return TRUE;

	}

	LPCTSTR CFSystemUtil::GetClipboardFormatString(UINT uFormat, LPTSTR lpszFormatName, int cchMaxCount)
	{
		CHECK_POINTER_RETURN_VALUE_IF_FAIL(lpszFormatName, NULL);
		lpszFormatName[0] = TEXT('\0');
		switch (uFormat)
		{
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_TEXT);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_BITMAP);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_METAFILEPICT);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_SYLK);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_DIF);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_TIFF);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_OEMTEXT);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_DIB);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_PALETTE);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_PENDATA);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_RIFF);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_WAVE);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_UNICODETEXT);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_ENHMETAFILE);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_HDROP);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_LOCALE);
#if(WINVER >= 0x0500)
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_DIBV5);
#endif /* WINVER >= 0x0500 */

			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_OWNERDISPLAY);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_DSPTEXT);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_DSPBITMAP);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_DSPMETAFILEPICT);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_DSPENHMETAFILE);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_PRIVATEFIRST);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_PRIVATELAST);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_GDIOBJFIRST);
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_GDIOBJLAST);
		default:
			{
				int nRet = GetClipboardFormatName(uFormat, lpszFormatName, cchMaxCount);
				if (0 == nRet)
				{
					API_ASSERT(FALSE);
					StringCchPrintf(lpszFormatName, cchMaxCount, TEXT("Unknown:%d"), uFormat);
				}
			}
			break;
		}
		return lpszFormatName;
	}

    BOOL CFSystemUtil::Is64BitWindows()
    {
        BOOL bIsWow64 = FALSE;
#if defined(_WIN64)
        //64位程序只能在64位系统上运行
        bIsWow64 = TRUE;
#elif defined(_WIN32)
        //32位程序可同时在 32/64 位系统上运行
        HMODULE hKernel32 = GetModuleHandle(TEXT("kernel32.dll"));
        FTLASSERT(hKernel32);

        typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
        LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(hKernel32, "IsWow64Process");
        if (fnIsWow64Process)
        {
            if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
            {
                bIsWow64 = FALSE;
            }
        }
#endif
        return bIsWow64;
    }

    BOOL CFSystemUtil::SuspendProcess(DWORD dwProcessID, BOOL fSuspend, DWORD skipedThreadId/* = ::GetCurrentThreadId()*/)
    {
        BOOL bRet = FALSE;
        //Get the list of threads in the system
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,dwProcessID);
        if(hSnapshot != INVALID_HANDLE_VALUE)
        {
            //Walk the list of threads
            THREADENTRY32    te = {sizeof(te)};
            //枚举进程内所有线程
            API_VERIFY(Thread32First(hSnapshot, &te));
            for(; bRet; bRet = Thread32Next(hSnapshot, &te))
            {
                //Is this thread in the desired process and not the skipped thread(UI)
                if(te.th32OwnerProcessID == dwProcessID && te.th32ThreadID != skipedThreadId)
                {
                    //线程ID是全局（Windows）变量，线程Handle是进程中的变量，不能由 ID -> Handle(安全考虑)，只有循环枚举

                    //Attemp to covert the thread ID into a handle.
                    //Note: Win98 not support
                    HANDLE hThread = ::OpenThread (THREAD_SUSPEND_RESUME,FALSE,te.th32ThreadID);
                    if(hThread != NULL)
                    {
                        if(fSuspend)
                        {
                            API_VERIFY(-1 != ::SuspendThread(hThread));
                        }
                        else
                        {
                            API_VERIFY(-1 != ::ResumeThread(hThread));
                        }
                        SAFE_CLOSE_HANDLE(hThread,NULL);
                    }

                }
            }
            SAFE_CLOSE_HANDLE(hSnapshot,NULL);
        }
        return bRet;
    }

#ifdef UNICODE
#define CLIPBOARDFMT    CF_UNICODETEXT
#else
#define CLIPBOARDFMT    CF_TEXT
#endif

    BOOL CFSystemUtil::CopyTextToClipboard( LPCTSTR szMem , HWND hWndOwner )
    {
        FTLASSERT ( FALSE == IsBadStringPtr ( szMem , MAX_PATH ) ) ;
        if ( TRUE == IsBadStringPtr ( szMem , MAX_PATH ) )
        {
            return ( FALSE ) ;
        }

        BOOL bRet = TRUE ;
        BOOL bOpenedCB = FALSE ;
        if ( TRUE == ::OpenClipboard ( hWndOwner ) )
        {
            bOpenedCB = TRUE ;
            if ( TRUE == ::EmptyClipboard ( ) )
            {
                // Do the goofy memory stuff.
                HGLOBAL hGlob = ::GlobalAlloc ( GMEM_MOVEABLE         ,
                    (_tcslen ( szMem ) + 1) *
                    sizeof ( TCHAR )       );
                if ( NULL != hGlob )
                {
                    TCHAR * szClipMem = (TCHAR*)GlobalLock ( hGlob ) ;
                    FTLASSERT ( NULL != szMem ) ;
                    StringCchCopy(szClipMem , _tcslen ( szMem ), szMem);
                    //_tcscpy ( szClipMem, szMem) ;

                    GlobalUnlock ( hGlob ) ;
                    if ( NULL == ::SetClipboardData ( CLIPBOARDFMT , hGlob ) )
                    {
                        FTLASSERT ( !"SetClipboardData failed!!" ) ;
                    }
                }
                else
                {
                    FTLASSERT ( !"Unable to GlobalAlloc memory!!" ) ;
                    bRet = FALSE ;
                }
            }
            else
            {
                FTLASSERT ( !"Unable to empty the clipboard!!" ) ;
                bRet = FALSE ;
            }
        }
        else
        {
            FTLASSERT ( !"Unable to open the clipboard!!" ) ;
            bRet = FALSE ;
        }
        if ( TRUE == bOpenedCB )
        {
            CloseClipboard ( ) ;
        }
        return ( bRet ) ;
    }

    BOOL CFSystemUtil::IsLittleSystem()
    {
        FTLASSERT(sizeof(int) == 4);
        BOOL bisLittleSystem = TRUE;

        int tmpNumber = 0x12345678;
        const unsigned char bigNumberBuf[sizeof(int)] = {0x12, 0x34, 0x56, 0x78};
        const unsigned char littleNumberBuf[sizeof(int)] = {0x78, 0x56, 0x34, 0x12};

        if (memcmp(littleNumberBuf, (unsigned char*)&tmpNumber, sizeof(int)) == 0)
        {
            bisLittleSystem = TRUE;
        }
        else
        {
            FTLASSERT(memcmp(bigNumberBuf, (unsigned char*)&tmpNumber, sizeof(int)) == 0);
            bisLittleSystem = FALSE;
        }
        return bisLittleSystem;
    }

    int CFSystemUtil::DosLineToUnixLine(const char *src, char *dest, int maxlen)
    {
        int		len = 0;
        maxlen--;	// 去掉末尾的\0

        //如果src还没有到最后而且长度合法
        while (*src != '\0' && len < maxlen)
            if ((dest[len] = *src++) != '\r')
                len++;

        //最后加上结束符\0
        dest[len] = 0;

        return	len;
    }

    //把Unix的结束符转换为Dos的结束符(将 "\n" 改成 "\r\n" )
    int CFSystemUtil::UnixLineToDosLine(const char *src, char *dest, int maxlen)
    {
        int		len = 0;
        char	*tmpbuf = NULL;

        //如果起始方和目的方的地址相同，则首先把src拷贝到一个临时缓冲区tmpbuf中
        if (src == dest)
		{
#pragma warning(push)
#pragma warning(disable : 4996)
            tmpbuf = _strdup(src);
#pragma warning(pop)
			src = tmpbuf;
		}
        maxlen--;	//去掉 \0所占用的位置

        while (*src != '\0' && len < maxlen)
        {
            if ((dest[len] = *src++) == '\n')
            {
                dest[len++] = '\r';
                if (len < maxlen)
                    dest[len] = '\n';
            }
            len++;
        }
        dest[len] = 0;
        if (tmpbuf)
            free(tmpbuf);

        return	len;
    }

    CFSystemMetricsProperty::CFSystemMetricsProperty()
    {
        m_dwOldGetProperty = 0;
        
        m_cxScreen = GetSystemMetrics(SM_CXSCREEN);				//主显示屏幕宽度(1280像素/1440像素)，等价于 GetDeviceCaps( hdcPrimaryMonitor, HORZRES).
        m_cyScreen = GetSystemMetrics(SM_CYSCREEN);				//主显示屏幕高度(1024像素/900像素)         
        m_cxVScroll = GetSystemMetrics(SM_CXVSCROLL);			//垂直滚动条的宽度(XP -- 16像素, Win7 -- 17像素)
        m_cyHScroll = GetSystemMetrics(SM_CYHSCROLL);			//水平滚动条的高度(XP -- 16像素, Win7 -- 17像素)
        m_cyCaption = GetSystemMetrics(SM_CYCAPTION);			//标题栏的高度(XP -- 19像素, Win7 -- 22 像素)
        m_cxBorder = GetSystemMetrics(SM_CXBORDER);				//窗体边框的宽度(1像素)
        m_cyBorder = GetSystemMetrics(SM_CYBORDER);				//窗体边框的高度(1像素)
        m_cxDlgFrame = GetSystemMetrics(SM_CXDLGFRAME);			//对话框边框的宽度(3像素)
        m_cyDlgFrame = GetSystemMetrics(SM_CYDLGFRAME);			//对话框边框的高度(3像素)
        m_cyVThumb = GetSystemMetrics(SM_CYVTHUMB);				//垂直滚动条中 thumb box 的高度(XP -- 16像素, Win7 -- 17像素)
        m_cxHThumb = GetSystemMetrics(SM_CXHTHUMB);				//水平滚动条中 thumb box 的宽度(XP -- 16像素, Win7 -- 17像素)
        m_cxIcon = GetSystemMetrics(SM_CXICON);					//Icon的缺省宽度(32像素)
        m_cyIcon = GetSystemMetrics(SM_CYICON);					//Icon的缺省高度(32像素)
        m_cxCursor = GetSystemMetrics(SM_CXCURSOR);				//Cursor的宽度(32像素)
        m_cyCursor = GetSystemMetrics(SM_CYCURSOR);				//Cursor的高度(32像素)
        m_cyMenu = GetSystemMetrics(SM_CYMENU);					//菜单的高度(XP -- 19像素, Win7 -- 20像素)
        m_cxFullScreen = GetSystemMetrics(SM_CXFULLSCREEN);		//主显示器上全屏时的客户区宽度(1280像素)
        m_cyFullScreen = GetSystemMetrics(SM_CYFULLSCREEN);		//主显示器上全屏时的客户区高度(975) -- 任务条在下方时
        m_cyKanjiWindow = GetSystemMetrics(SM_CYKANJIWINDOW);	//double byte character 版本的Windows上屏幕底端 Kanji 窗体的高度(0像素)
        m_MousePresent = GetSystemMetrics(SM_MOUSEPRESENT);		//非0(1)表示有鼠标,0表示没有鼠标
        m_cyVscroll = GetSystemMetrics(SM_CYVSCROLL);			//垂直滚动条中箭头位图的高度(XP -- 16像素, Win7 -- 17像素)
        m_cxHscroll = GetSystemMetrics(SM_CXHSCROLL);			//水平滚动条中箭头位图的宽度(XP -- 16像素, Win7 -- 17像素)
        m_Debug = GetSystemMetrics(SM_DEBUG);					//非0(1)表示安装的是调试版的 User.exe(0)
        m_SwapButton = GetSystemMetrics(SM_SWAPBUTTON);			//非0表示交换了鼠标的左右键
        m_Reserved1 = GetSystemMetrics(SM_RESERVED1);			//0
        m_Reserved2 = GetSystemMetrics(SM_RESERVED2);			//0
        m_Reserved3 = GetSystemMetrics(SM_RESERVED3);			//0
        m_Reserved4 = GetSystemMetrics(SM_RESERVED4);			//0
        m_cxMin = GetSystemMetrics(SM_CXMIN);					//窗体的最小宽度(XP -- 112像素, Win7 -- 132像素)
        m_cyMin = GetSystemMetrics(SM_CYMIN);					//窗体的最小高度(XP -- 27像素, Win7 -- 38像素)
        m_cxSize = GetSystemMetrics(SM_CXSIZE);					//窗体Caption或任务栏上按钮的宽度(XP -- 18像素, Win7 -- 35像素)
        m_cySize = GetSystemMetrics(SM_CYSIZE);					//窗体Caption或任务栏上按钮的高度(XP -- 18像素, Win7 -- 21像素)
        m_cxFrame = GetSystemMetrics(SM_CXFRAME);				//可改变大小的框体的边界的宽度(XP -- 4像素, Win7 -- 8像素) -- 右下角的拖动区的宽度?
        m_cyFrame = GetSystemMetrics(SM_CYFRAME);				//可改变大小的框体的边界的高度(XP -- 4像素, Win7 -- 8像素) -- 右下角的拖动区的高度?
        m_cxMinTrack = GetSystemMetrics(SM_CXMINTRACK);			//Windows能拖到的的最小宽度(XP -- 112像素, Win7 -- 132像素) -- 可通过 WM_GETMINMAXINFO 重设置
        m_cyMinTrack = GetSystemMetrics(SM_CYMINTRACK);			//Windows能拖到的的最小高度(XP -- 27像素, Win7 -- 38像素) -- 可通过 WM_GETMINMAXINFO 重设置
        m_cxDoubleClk = GetSystemMetrics(SM_CXDOUBLECLK);		//双击有效区域的宽度(4像素)
        m_cyDoubleClk = GetSystemMetrics(SM_CYDOUBLECLK);		//双击有效区域的高度(4像素)
        m_cxIconSpacing = GetSystemMetrics(SM_CXICONSPACING);	//large icon view时Grid Cell的宽度(75像素)
        m_cyIconSpacing = GetSystemMetrics(SM_CYICONSPACING);	//large icon view时Grid Cell的高度(75像素)
        m_MenuDropAlignment = GetSystemMetrics(SM_MENUDROPALIGNMENT);	//非0表示下拉菜单相对于Menu-Bar Item是右对齐， 0表示是左对齐 (0)
        m_PenWindows = GetSystemMetrics(SM_PENWINDOWS);			//非0表示安装了 Pen computing extensions(0)
        m_DBCSEnabled = GetSystemMetrics(SM_DBCSENABLED);		//非0表示User32.dll支持 DBCS(1), IMM支持(亚洲语言输入法)
        m_CMouseButtons = GetSystemMetrics(SM_CMOUSEBUTTONS);	//表示鼠标上按钮的个数(3)

#if(WINVER >= 0x0400)
        m_cxFixedFrame = GetSystemMetrics(SM_CXFIXEDFRAME);		//同 SM_CXDLGFRAME(3)
        m_cyFixedframe = GetSystemMetrics(SM_CYFIXEDFRAME);		//同 SM_CYDLGFRAME(3)
        m_cxSizeFrame = GetSystemMetrics(SM_CXSIZEFRAME);		//同 SM_CXFRAME(4)
        m_cySizeFrame = GetSystemMetrics(SM_CYSIZEFRAME);		//同 SM_CYFRAME(4)

        m_Secure = GetSystemMetrics(SM_SECURE);					//This system metric should be ignored(始终返回0)
        m_cxEdge = GetSystemMetrics(SM_CXEDGE);					//3D边框的宽度(2像素)
        m_cyEdge = GetSystemMetrics(SM_CYEDGE);					//3D边框的高度(2像素)
        m_cxMinSpacing = GetSystemMetrics(SM_CXMINSPACING);		//最小化窗体在表格上的宽度(160像素) -- 多文档最小化还是任务栏上最小化?
        m_cyMinSpacing = GetSystemMetrics(SM_CYMINSPACING);		//最小化窗体在表格上的高度(XP--24像素, Win7--27像素) -- 多文档最小化还是任务栏上最小化?
        m_cxSMIcon = GetSystemMetrics(SM_CXSMICON);				//小Icon的建议宽度(16像素)
        m_cySMIcon = GetSystemMetrics(SM_CYSMICON);				//小Icon的建议高度(16像素)
        m_cySMCaption = GetSystemMetrics(SM_CYSMCAPTION);		//小标题栏(Caption)的高度(XP--16像素,Win7--20像素)
        m_cxSMSize = GetSystemMetrics(SM_CXSMSIZE);				//小标题栏上的按钮宽度(XP--12像素,Win7--19像素)
        m_cySMSize = GetSystemMetrics(SM_CYSMSIZE);				//小标题栏上的按钮高度(XP--15像素,Win7--19像素)
        m_cxMenuSize = GetSystemMetrics(SM_CXMENUSIZE);			//MenuBar上按钮(如多文档窗体中子窗体的关闭按钮)的宽度(18像素)
        m_cyMenuSize = GetSystemMetrics(SM_CYMENUSIZE);			//MenuBar上按钮(如多文档窗体中子窗体的关闭按钮)的高(18像素)
        m_Arrange = GetSystemMetrics(SM_ARRANGE);				//指示系统如何配列最小化窗体( 8 -- ARW_HIDE )
        m_cxMinimized = GetSystemMetrics(SM_CXMINIMIZED);		//最小化窗体的宽度(160像素)
        m_cyMinimized = GetSystemMetrics(SM_CYMINIMIZED);		//最小化窗体的高度(XP--24像素,Win7--27像素)
        m_cxMaxTrack = GetSystemMetrics(SM_CXMAXTRACK);			//有标题、可改变大小边框的窗体的缺省最大宽度,通常是窗体能达到的最大值
																//  (1292像素 = SM_CXFULLSCREEN + (SM_CXEDGE + SM_CXFRAME) * 2 )，如有多个屏幕，则会更大，如 2736
																//            = 1280            + (     2    +      4    ) * 2 )，
        m_cyMaxTrack = GetSystemMetrics(SM_CYMAXTRACK);			//有标题、可改变大小边框的窗体的缺省最大高度,通常是窗体能达到的最大值(1036 = 1024 + (2+4)*2 )
        m_cxMaximized = GetSystemMetrics(SM_CXMAXIMIZED);		//主显示器上，最大化的顶层窗体的宽度(1288 = 1280 + 4 * 2)
        m_cyMaximized = GetSystemMetrics(SM_CYMAXIMIZED);		//主显示器上，最大化的顶层窗体的高度(1002 = )
        m_Network = GetSystemMetrics(SM_NETWORK);				//代表网络是否存在的数据位( 0x03 )
        m_CleanBoot = GetSystemMetrics(SM_CLEANBOOT);			//代表系统如何启动(0 -- 正常启动)
        m_cxDrag = GetSystemMetrics(SM_CXDRAG);					//开始拖拽时的最小宽度(4像素)
        m_cyDrag = GetSystemMetrics(SM_CYDRAG);					//开始拖拽时的最小高度(4像素)
#endif /* WINVER >= 0x0400 */

        m_ShowSounds = GetSystemMetrics(SM_SHOWSOUNDS);//           70
#if(WINVER >= 0x0400)
        m_cxMenuCheck = GetSystemMetrics(SM_CXMENUCHECK);		//缺省的菜单选中位图的宽度(13像素) -- Use instead of GetMenuCheckMarkDimensions()
        m_cyMenuCheck = GetSystemMetrics(SM_CYMENUCHECK);		//缺省的菜单选中位图的高度(13像素)
        m_SlowMachine = GetSystemMetrics(SM_SLOWMACHINE);		//非0表示系统有一个低速的处理器(0)
        m_MideaStenabled = GetSystemMetrics(SM_MIDEASTENABLED);	//非0表示是 Hebrew and Arabic languages 
#endif /* WINVER >= 0x0400 */

#if (WINVER >= 0x0500) || (_WIN32_WINNT >= 0x0400)
        m_MouseWheelPresent = GetSystemMetrics(SM_MOUSEWHEELPRESENT);	//非0表示有鼠标滚轮(1)
#endif

#if(WINVER >= 0x0500)
        m_XVirtualScreen = GetSystemMetrics(SM_XVIRTUALSCREEN);		//虚拟屏幕的左边界(0)
        m_YVirtualScreen = GetSystemMetrics(SM_YVIRTUALSCREEN);		//虚拟屏幕的顶边界(0)
        m_cxVirtualScreen = GetSystemMetrics(SM_CXVIRTUALSCREEN);	//虚拟屏幕的宽度(1280像素，有多个屏幕时更大，如 2720 = 1280 + 1440)
        m_cyVirtualScreen = GetSystemMetrics(SM_CYVIRTUALSCREEN);	//虚拟屏幕的高度(1024像素，有多个屏幕且竖直排列时时可能更大)
        m_CMonitors = GetSystemMetrics(SM_CMONITORS);				//计算用于显示的显示器的个数(1，2个屏幕时2)
        m_SameDisplayFormat = GetSystemMetrics(SM_SAMEDISPLAYFORMAT);	//非0表示所有的显示器有相同的颜色格式(Win7--1)
#endif /* WINVER >= 0x0500 */

#if(_WIN32_WINNT >= 0x0500)
        m_IMMEnabled = GetSystemMetrics(SM_IMMENABLED);			//非0表示 Input Method Manager/Input Method Editor 有效(Win7--1)
#endif /* _WIN32_WINNT >= 0x0500 */
#if(_WIN32_WINNT >= 0x0501)
        m_cxFocusBorder = GetSystemMetrics(SM_CXFOCUSBORDER);	//DrawFocusRect 绘制Focus矩形时的左、右边界的宽度(1像素)
        m_cyFocusBorder = GetSystemMetrics(SM_CYFOCUSBORDER);	//DrawFocusRect 绘制Focus矩形时的上、下边界的高度(1像素)
#endif /* _WIN32_WINNT >= 0x0501 */

#if(_WIN32_WINNT >= 0x0501)
        m_TabletPc = GetSystemMetrics(SM_TABLETPC);				//(0)
        m_MediaCenter = GetSystemMetrics(SM_MEDIACENTER);		//(XP--0, Win7--1)
        m_Starter = GetSystemMetrics(SM_STARTER);				//(0)
        m_Serverr2 = GetSystemMetrics(SM_SERVERR2);				//(0)
#endif /* _WIN32_WINNT >= 0x0501 */

#if(_WIN32_WINNT >= 0x0600)
        m_MouseHorizontalWheelPresent = GetSystemMetrics(SM_MOUSEHORIZONTALWHEELPRESENT);	//(?)
        m_cxPaddedBorder = GetSystemMetrics(SM_CXPADDEDBORDER);	//(?)
#endif /* _WIN32_WINNT >= 0x0600 */

        m_CMetrics = GetSystemMetrics(SM_CMETRICS);				//(0)

#if(WINVER >= 0x0500)
        m_RemoteSession = GetSystemMetrics(SM_REMOTESESSION);	//是否是通过 Terminal Services(远程终端)环境运行
#  if(_WIN32_WINNT >= 0x0501)
        m_ShuttingDown = GetSystemMetrics(SM_SHUTTINGDOWN);		//当前Session是否在 Shutting Down过程中
#  endif /* _WIN32_WINNT >= 0x0501 */
#  if(WINVER >= 0x0501)
        m_RemoteControl = GetSystemMetrics(SM_REMOTECONTROL);	//和 SM_REMOTESESSION 的区别?
#  endif /* WINVER >= 0x0501 */
#  if(WINVER >= 0x0501)
        m_CaretBlinkingEnabled = GetSystemMetrics(SM_CARETBLINKINGENABLED);// 0x2002
#  endif /* WINVER >= 0x0501 */
#endif /* WINVER >= 0x0500 */

    }

    CFSystemMetricsProperty::~CFSystemMetricsProperty()
    {
        
    }

	LPCTSTR CFSystemMetricsProperty::GetPropertyString(DWORD dwPropertyGet /* = SYSTEM_METRICS_PROPERTY_GET_DEFAULT */)
	{
		UNREFERENCED_PARAMETER(dwPropertyGet);
		m_strFormater.Format(TEXT("%d"), 100);
		return m_strFormater.GetString();
	}

    template <typename T, typename F> // = FTL::ObjecteDeleter<T*> 
    BOOL CFPluginMgrT<T,F>::Init(LPCTSTR pszPluginPath, LPCSTR pszProcName, LPCTSTR pszExtName /*= TEXT("*.dll")*/)
    {
        CHECK_POINTER_ISSTRING_PTR_RETURN_VALUE_IF_FAIL(pszPluginPath, FALSE);
        //CHECK_POINTER_ISSTRING_PTR_RETURN_VALUE_IF_FAIL(pszProcName, FALSE);
        CHECK_POINTER_ISSTRING_PTR_RETURN_VALUE_IF_FAIL(pszExtName, FALSE);

        BOOL bRet = FALSE;
        CFStringFormater strFormater;
        strFormater.Format(TEXT("%s\\%s"), pszPluginPath, pszExtName);

        WIN32_FIND_DATA findData = {0};
        HANDLE hFind = INVALID_HANDLE_VALUE;
        API_VERIFY( INVALID_HANDLE_VALUE != (hFind = ::FindFirstFile(strFormater, &findData)));
        if (INVALID_HANDLE_VALUE != hFind )
        {
            do 
            {
                //TODO: isDot
                CFStringFormater strFullPath;
                strFullPath.Format(TEXT("%s\\%s"), pszPluginPath, findData.cFileName);
                LoadPlugin( strFullPath, pszProcName);
            } 
            while (::FindNextFile(hFind, &findData));

            API_VERIFY(::FindClose(hFind));
            hFind = INVALID_HANDLE_VALUE;
        }
        return bRet;
    }

    template <typename T, typename F > // = FTL::ObjecteDeleter<T*>
    void CFPluginMgrT<T,F>::UnInit()
    {
        for_each(m_plugins.begin(), m_plugins.end(), F() );
        for_each(m_modules.begin(), m_modules.end(), FreeLibrary );

        m_plugins.clear();
        m_modules.clear();
    }

    template <typename T, typename F> // = FTL::ObjecteDeleter<T*> 
    BOOL CFPluginMgrT<T,F>::LoadPlugin(LPCTSTR pszePlugPath, LPCSTR pszProcName)
    {
        BOOL bRet = FALSE;
        HMODULE hModule = NULL;
        T* pInstance = NULL;
        API_VERIFY(NULL != (hModule = ::LoadLibrary(pszePlugPath)));
        if (NULL != hModule)
        {
            FunGetInstance pFun = (FunGetInstance)::GetProcAddress(hModule, pszProcName);
            if (pFun)
            {
                pInstance = (*pFun)();
                if (pInstance)
                {
                    m_plugins.push_back(pInstance);
                    m_modules.push_back(hModule);
                    bRet = TRUE;
                }
            }
        }
        if (!pInstance)
        {
            ::FreeModule(hModule);
            bRet = FALSE;
        }
        FTLTRACE(TEXT("Load Plugin Path=%s, Result=%d\n"), pszePlugPath, bRet);
        return bRet;
    }

    template <typename T, typename F> // = FTL::ObjecteDeleter<T*> 
    CFPluginMgrT<T,F>::~CFPluginMgrT()
    {
        UnInit();
    }
}

#endif //FTL_SYSTEM_HPP