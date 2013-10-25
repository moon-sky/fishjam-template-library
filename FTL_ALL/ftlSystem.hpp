///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlSystem.hpp
/// @brief  Fishjam Template Library System Implemention File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FTL_SYSTEM_HPP
#define FTL_SYSTEM_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlsystem.h"
#endif
#include "ftlConversion.h"

#include <ShlObj.h>
#include <atlbase.h>
#ifdef _DEBUG
#  include <winternl.h>
#endif

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
		case 20602:
			osType = ostWindows8;
			break;
        default:
            {
                _ASSERT(FALSE);
                if (dwOsVersion > 20602)
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
		DWORD dwProcessCount1 = 0; 
		DWORD dwProcessCount2 = 0; 

		//第一种方法 -- 只能得到物理CPU?
        SYSTEM_INFO sysinfo = {0};
        ::GetSystemInfo(&sysinfo);
		dwProcessCount1 = sysinfo.dwNumberOfProcessors;
		
		//第二种方法 -- 可以得到超线程 ?
		BOOL bRet = FALSE;
		DWORD_PTR dwProcessAffinityMask = 0, dwSystemAffinityMask = 0;
		API_VERIFY(GetProcessAffinityMask(GetCurrentProcess(), &dwProcessAffinityMask, &dwSystemAffinityMask));
		if (bRet)
		{
			for (; dwProcessAffinityMask; dwProcessAffinityMask >>= 1)
			{
				if (dwProcessAffinityMask & 0x1) 
				{
					dwProcessCount2++;
				}
			}
		}

		API_ASSERT(dwProcessCount1 == dwProcessCount2);
        return dwProcessCount2;
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

    LONG CFRegUtil::GetRegValueExportString(HKEY hKey, LPCTSTR pszValueName, CAtlString& strResult, DWORD* pRegType)
    {
        LONG lRet = ERROR_SUCCESS;
        BOOL bRet= FALSE;
        DWORD dwRegType = REG_NONE;
        DWORD nBytesCount = 0;
        REG_VERIFY(::RegQueryValueEx(hKey, pszValueName, NULL, &dwRegType, NULL, &nBytesCount));
        if (ERROR_SUCCESS == lRet)
        {
            CFMemAllocator<BYTE> memValue(nBytesCount);
            REG_VERIFY(::RegQueryValueEx(hKey, pszValueName, NULL, &dwRegType, memValue, &nBytesCount));
            if (ERROR_SUCCESS == lRet)
            {
                if (pRegType)
                {
                    *pRegType = dwRegType;
                }
                strResult.Empty();
                CAtlString strBinary;
                LONG nStrBinaryCount = 0;

                switch (dwRegType)
                {
                case REG_SZ:
                    {
                        CAtlString strTemp;
                        strTemp.Format(TEXT("%s"), (LPCTSTR)memValue.GetMemory());
                        strTemp.Replace(TEXT("\\"), TEXT("\\\\"));  // \ => \\  
                        strTemp.Replace(TEXT("\""), TEXT("\\\""));  // " => \" 
                        strResult.Format(TEXT("\"%s\""), strTemp);
                    }
                    break;
                case REG_EXPAND_SZ:
                case REG_BINARY:
                case REG_MULTI_SZ:
                case REG_QWORD_LITTLE_ENDIAN:  //REG_QWORD
                    {
                        API_VERIFY(CFConvUtil::HexFromBinary(memValue.GetMemory(), nBytesCount, NULL, &nStrBinaryCount, _T(',')));
                        CFMemAllocator<TCHAR> bufString(nStrBinaryCount);
                        API_VERIFY(CFConvUtil::HexFromBinary(memValue.GetMemory(), nBytesCount, bufString.GetMemory(), &nStrBinaryCount, _T(',')));
                        
                        if (REG_BINARY == dwRegType)
                        {
                            strResult = _T("hex:");
                        }
                        else
                        {
                            strResult.Format(TEXT("hex(%x):"), dwRegType);
                        }
                        strResult.Append(bufString.GetMemory());

                        break;
                    }
                case REG_DWORD_LITTLE_ENDIAN: //REG_DWORD:
                    {
                        strResult.Format(TEXT("dword:%08x"), *((DWORD*)memValue.GetMemory()));
                        break;
                    }
                case REG_DWORD_BIG_ENDIAN:
                case REG_LINK:
                case REG_RESOURCE_LIST:
                case REG_FULL_RESOURCE_DESCRIPTOR:
                case REG_RESOURCE_REQUIREMENTS_LIST:
                default:
                    FTLTRACEEX(tlWarning, TEXT("%s Unsupport RegType\n"), __FILE__LINE__);
                    FTLASSERT(FALSE);
                    lRet = ERROR_UNSUPPORTED_TYPE;
                    break;
                }
            }
        }
        return lRet;
    }
    

	///////////////////////////////////////// SystemParamProperty ///////////////////////////////////////////////
	SystemParamProperty::SystemParamProperty()
	{
		//Init();
		GetParametersInfo();
	}
	SystemParamProperty::~SystemParamProperty()
	{

	}
	BOOL SystemParamProperty::GetParametersInfo()
	{
		BOOL bRet = FALSE;

		API_VERIFY(::SystemParametersInfo(SPI_GETBEEP, 0, &m_bBeep, 0));			//警告蜂鸣器是否是打开
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSE, 0, &m_MouseInfo[0], 0));	//鼠标的2个阈值和加速特性
		API_VERIFY(::SystemParametersInfo(SPI_GETBORDER, 0, &m_nBorder, 0));		//窗口边界放大宽度的边界放大因子
		API_VERIFY(::SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &m_dwKeyboardSpeed, 0));	//键盘重复击键速度设置情况,0(约30次/秒)~31(约25次/秒)
		API_VERIFY(::SystemParametersInfo(SPI_LANGDRIVER, 0, &m_dwUnknown, 0));		//未实现
		API_VERIFY(::SystemParametersInfo(SPI_ICONHORIZONTALSPACING, 0, &m_nIconHorizontalSpacing, 0));	//在LargeIconView的时候Icon的宽度,设置必须大于SM_CXICON
		API_VERIFY(::SystemParametersInfo(SPI_ICONVERTICALSPACING, 0, &m_nIconVerticalSpacing, 0));		//在LargeIconView的时候Icon的高度,设置必须大于SM_CXICON
		API_VERIFY(::SystemParametersInfo(SPI_GETSCREENSAVETIMEOUT, 0, &m_nScreenSaveTimeout, 0));		//屏保超时的时间，单位为秒
#if 0
		API_VERIFY(::SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETGRIDGRANULARITY, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETICONTITLEWRAP, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMENUDROPALIGNMENT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETICONTITLELOGFONT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFASTTASKSWITCH, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMINIMIZEDMETRICS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETICONMETRICS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETWORKAREA, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETHIGHCONTRAST, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETKEYBOARDPREF, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSCREENREADER, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETANIMATION, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFONTSMOOTHING, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETLOWPOWERTIMEOUT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETPOWEROFFTIMEOUT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETLOWPOWERACTIVE, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETPOWEROFFACTIVE, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETDEFAULTINPUTLANG, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETWINDOWSEXTENSION, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSETRAILS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFILTERKEYS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETTOGGLEKEYS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSEKEYS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSHOWSOUNDS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSTICKYKEYS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETACCESSTIMEOUT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSERIALKEYS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSOUNDSENTRY, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSNAPTODEFBUTTON, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSEHOVERWIDTH, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSEHOVERHEIGHT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSEHOVERTIME, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMENUSHOWDELAY, 0, &XXXXX, 0));
#if (_WIN32_WINNT >= 0x0600)
		API_VERIFY(::SystemParametersInfo(SPI_GETWHEELSCROLLCHARS, 0, &XXXXX, 0));
#endif //_WIN32_WINNT >= 0x0600

		API_VERIFY(::SystemParametersInfo(SPI_GETSHOWIMEUI, 0, &XXXXX, 0));
#if(WINVER >= 0x0500)
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSESPEED, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETDESKWALLPAPER, 0, &XXXXX, 0));
#endif /* WINVER >= 0x0500 */

#if(WINVER >= 0x0600)
		API_VERIFY(::SystemParametersInfo(SPI_GETAUDIODESCRIPTION, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSCREENSAVESECURE, 0, &XXXXX, 0));
#endif /* WINVER >= 0x0600 */

#if(_WIN32_WINNT >= 0x0601)
		API_VERIFY(::SystemParametersInfo(SPI_GETHUNGAPPTIMEOUT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETWAITTOKILLTIMEOUT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETWAITTOKILLSERVICETIMEOUT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSEDOCKTHRESHOLD, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETPENDOCKTHRESHOLD, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETWINARRANGING, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSEDRAGOUTTHRESHOLD, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETPENDRAGOUTTHRESHOLD, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSESIDEMOVETHRESHOLD, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETPENSIDEMOVETHRESHOLD, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETDRAGFROMMAXIMIZE, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSNAPSIZING, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETDOCKMOVING, 0, &XXXXX, 0));
#endif /* _WIN32_WINNT >= 0x0601 */

#if(WINVER >= 0x0500)
		API_VERIFY(::SystemParametersInfo(SPI_GETACTIVEWINDOWTRACKING, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMENUANIMATION, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETCOMBOBOXANIMATION, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETLISTBOXSMOOTHSCROLLING, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETGRADIENTCAPTIONS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETKEYBOARDCUES, 0, &m_bKeyboardCues, 0)); //快捷键字符下是否一直显示下划线，如为FALSE表需要按下"Alt"后才显示
		API_VERIFY(::SystemParametersInfo(SPI_GETMENUUNDERLINES, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETACTIVEWNDTRKZORDER, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETHOTTRACKING, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMENUFADE, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSELECTIONFADE, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETTOOLTIPANIMATION, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETTOOLTIPFADE, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETCURSORSHADOW, 0, &XXXXX, 0));
#if(_WIN32_WINNT >= 0x0501)
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSESONAR, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSECLICKLOCK, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSEVANISH, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFLATMENU, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETDROPSHADOW, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETBLOCKSENDINPUTRESETS, 0, &XXXXX, 0));
#endif /* _WIN32_WINNT >= 0x0501 */
		API_VERIFY(::SystemParametersInfo(SPI_GETUIEFFECTS, 0, &XXXXX, 0));
#if(_WIN32_WINNT >= 0x0600)
		API_VERIFY(::SystemParametersInfo(SPI_GETDISABLEOVERLAPPEDCONTENT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETCLIENTAREAANIMATION, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETCLEARTYPE, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETSPEECHRECOGNITION, 0, &XXXXX, 0));
#endif /* _WIN32_WINNT >= 0x0600 */
		API_VERIFY(::SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETACTIVEWNDTRKTIMEOUT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFOREGROUNDFLASHCOUNT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETCARETWIDTH, 0, &XXXXX, 0));
#if(_WIN32_WINNT >= 0x0501)
		API_VERIFY(::SystemParametersInfo(SPI_GETMOUSECLICKLOCKTIME, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFONTSMOOTHINGTYPE, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFONTSMOOTHINGCONTRAST, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFOCUSBORDERWIDTH, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFOCUSBORDERHEIGHT, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETFONTSMOOTHINGORIENTATION, 0, &XXXXX, 0));
#endif /* _WIN32_WINNT >= 0x0501 */

#if(_WIN32_WINNT >= 0x0600)
		API_VERIFY(::SystemParametersInfo(SPI_GETMINIMUMHITRADIUS, 0, &XXXXX, 0));
		API_VERIFY(::SystemParametersInfo(SPI_GETMESSAGEDURATION, 0, &XXXXX, 0));
#endif /* _WIN32_WINNT >= 0x0600 */

#endif /* WINVER >= 0x0500 */
		//API_VERIFY(::SystemParametersInfo(SPI_XXX, 0, &XXXXX, 0));

		//API_VERIFY(::SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &m_nKeyboardSpeed, 0));
		//API_VERIFY(::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea ,NULL));
#endif 

		return bRet;
	}
	///////////////////////////////////////// SystemParamProperty ///////////////////////////////////////////////

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
        ATL::CRegKey RegUsers;
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
			HANDLE_CASE_TO_STRING(lpszFormatName, cchMaxCount, CF_HDROP); //DragQueryFile获取
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

    BOOL CFSystemUtil::EnableProcessPrivilege(HANDLE hProcess, LPCTSTR lpPrivilegeName /* = SE_DEBUG_NAME */,BOOL bEnabled /*= TRUE*/)
    {
        BOOL bRet = FALSE;
        TOKEN_PRIVILEGES TokenPrivileges = { 0 };
        API_VERIFY(LookupPrivilegeValue (NULL, lpPrivilegeName, &TokenPrivileges.Privileges[0].Luid));
        if (bRet)
        {
            HANDLE hToken = NULL;
            API_VERIFY(OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken));
            if (bRet)
            {
                TokenPrivileges.PrivilegeCount = 1;
                TokenPrivileges.Privileges[0].Attributes = bEnabled ? SE_PRIVILEGE_ENABLED : 0; //SE_PRIVILEGE_REMOVED
                API_VERIFY(AdjustTokenPrivileges (hToken,FALSE,&TokenPrivileges,
                    sizeof (TokenPrivileges),NULL,NULL));
                bRet = (GetLastError () == ERROR_SUCCESS);
                CloseHandle(hToken);
            }
        }
        return bRet;
    }


	BOOL CFSystemUtil::IsSpecialProcessName(LPCTSTR pszProcessName, HMODULE hModule /* = NULL */)
	{
		BOOL bRet = FALSE;
		TCHAR szModuleFilePath[MAX_PATH] = {0};
		API_VERIFY(::GetModuleFileName(hModule, szModuleFilePath, _countof(szModuleFilePath)) > 0);

		//如果相等，则是指定名字的进程
		if(lstrcmpi(pszProcessName, PathFindFileName(szModuleFilePath)) == 0)
		{
			bRet = TRUE;
		}
		else
		{
			bRet = FALSE;
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
        BOOL bRet = TRUE ;
		API_VERIFY(::OpenClipboard ( hWndOwner ))
        if ( bRet )
        {
            API_VERIFY(::EmptyClipboard( ));
			if(bRet)
            {
				const int nLen = _tcslen (szMem);

				//TODO:GlobalAlloc 分配的内存不释放?还是说通过下次的 EmptyClipboard 释放?
                HGLOBAL hGlob = ::GlobalAlloc ( GHND, ( nLen + 1) * sizeof ( TCHAR ));
                if ( NULL != hGlob )
                {
                    TCHAR * szClipMem = (TCHAR*)GlobalLock ( hGlob ) ;
                    FTLASSERT ( NULL != szMem ) ;
                    StringCchCopy(szClipMem , nLen, szMem);
                    GlobalUnlock ( hGlob ) ;
                    API_VERIFY(NULL != ::SetClipboardData (CLIPBOARDFMT, hGlob));

					//TODO:可以同时支持 CF_UNICODETEXT、CF_TEXT
                }
                else
                {
                    bRet = FALSE ;
                }
            }
			CloseClipboard();
        }
        return ( bRet ) ;
    }

	ULONGLONG CFSystemUtil::GetTickCount64()
	{
		typedef ULONGLONG (WINAPI* GetTickCount64Proc)(void);
		typedef ULONG (WINAPI* NtQuerySystemInformationProc)(int /*SYSTEM_INFORMATION_CLASS SystemInformationClass*/,
			PVOID /*SystemInformation*/, 
			ULONG /*SystemInformationLength*/, 
			PULONG /*ReturnLength*/);

		GetTickCount64Proc pVistaGetTickCount64 = (GetTickCount64Proc)
			GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "GetTickCount64");
		if (pVistaGetTickCount64)
		{
			return pVistaGetTickCount64();
		}
		else
		{
			typedef struct _SYSTEM_TIME_OF_DAY_INFORMATION
			{
				LARGE_INTEGER BootTime;
				LARGE_INTEGER CurrentTime;
				LARGE_INTEGER TimeZoneBias;
				ULONG CurrentTimeZoneId;
			} SYSTEM_TIME_OF_DAY_INFORMATION, *PSYSTEM_TIME_OF_DAY_INFORMATION;

			NtQuerySystemInformationProc pNtQuerySystemInformation = (NtQuerySystemInformationProc)
				GetProcAddress(GetModuleHandle(_T("ntdll.dll")), ("NtQuerySystemInformation"));

			SYSTEM_TIME_OF_DAY_INFORMATION  st ={0};
			ULONG                           oSize = 0;
			if((NULL == pNtQuerySystemInformation)
				|| 0 != (pNtQuerySystemInformation(3, &st, sizeof(st), &oSize))
				||(oSize!= sizeof(st)))
			{
				return GetTickCount();
			}
			return (st.CurrentTime.QuadPart - st.BootTime.QuadPart)/10000;
		}
	}


	DWORD CFSystemUtil::GetPidFromProcessName(LPCTSTR pszProcesName)
	{
		DWORD dwPidResult = 0;

		BOOL bRet = FALSE;
		HANDLE hSnapProcHandle = NULL;
		API_VERIFY((hSnapProcHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) != INVALID_HANDLE_VALUE);
		if (bRet)
		{
			PROCESSENTRY32 ProcEntry = { 0 };
			ProcEntry.dwSize = sizeof(PROCESSENTRY32);
			API_VERIFY(Process32First(hSnapProcHandle, &ProcEntry));
			while (bRet)
			{
				LPCTSTR pszFileName = PathFindFileName(ProcEntry.szExeFile);
				if (pszFileName)
				{
					if(lstrcmpi(pszProcesName, pszFileName) == 0)
					{
						dwPidResult = ProcEntry.th32ProcessID;
						break;
					}
				}
				API_VERIFY(Process32Next(hSnapProcHandle, &ProcEntry));
			}
			CloseHandle(hSnapProcHandle);
		}
		//OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, False, PID);

		return dwPidResult;
	}

    typedef struct  
    {  
        DWORD       ExitStatus;                     //PVOID Reserved1
        DWORD       PebBaseAddress;                 //PPEB PebBaseAddress;
        DWORD       AffinityMask;                   //PVOID Reserved2[0]
        DWORD       BasePriority;                   //PVOID Reserved2[1];
        ULONG_PTR   UniqueProcessId;                //ULONG_PTR UniqueProcessId;
        ULONG       InheritedFromUniqueProcessId;   //Reserved3
    }   PROCESS_BASIC_INFORMATION_FOR_PPID;  

#ifdef _DEBUG
    //C_ASSERT(sizeof(PROCESS_BASIC_INFORMATION_FOR_PPID) == sizeof(PROCESS_BASIC_INFORMATION));
#endif 

    typedef LONG (WINAPI *NtQueryInformationProcessProc)(HANDLE, UINT, PVOID, ULONG, PULONG);  
    DWORD CFSystemUtil::GetParentProcessId(DWORD dwPID, BOOL bCheckParentExist /* = TRUE*/)
    {
        DWORD   dwParentPID = (DWORD)-1;  
        BOOL    bRet = FALSE;
        LONG    nStatus = 0;
        HANDLE  hProcess = NULL;
        PROCESS_BASIC_INFORMATION_FOR_PPID pbi = {0};  

        HMODULE hModuleNtDll = GetModuleHandle(TEXT("ntdll"));
        NtQueryInformationProcessProc pNtQueryInformationProcess = 
            (NtQueryInformationProcessProc)GetProcAddress( hModuleNtDll, "NtQueryInformationProcess");  
        if (pNtQueryInformationProcess)
        {
            // Get process handle  
            hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPID);  
            if (hProcess)
            {
                nStatus = (pNtQueryInformationProcess)(hProcess,  
                    0, //ProcessBasicInformation,  
                    (PVOID)&pbi,  
                    sizeof(PROCESS_BASIC_INFORMATION_FOR_PPID),  
                    NULL  
                    );
                FTLASSERT(0 == nStatus);    //STATUS_SUCCESS
                if (0 == nStatus)
                {
                    if (bCheckParentExist)
                    {
                        HANDLE hParentProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pbi.InheritedFromUniqueProcessId); 
                        if (hParentProcess)
                        {
                            dwParentPID = pbi.InheritedFromUniqueProcessId;
                            CloseHandle(hParentProcess);
                        }
                    }
                    else
                    {
                        dwParentPID = pbi.InheritedFromUniqueProcessId;
                    }
                    
                }
                else
                {
                    FTLTRACEEX(FTL::tlError, TEXT("%s, CFSystemUtil::GetParentProcessId, Error Code=0x%x\n"),
                        __FILE__LINE__,  nStatus);
                }
                CloseHandle (hProcess);  
            }
        }
        return dwParentPID;  
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

#if defined(_M_IX86)
	// IsInsideVPC's exception filter  
	DWORD __forceinline IsInsideVPC_exceptionFilter(LPEXCEPTION_POINTERS ep)  
	{  
    	PCONTEXT ctx = ep->ContextRecord;  

    	ctx->Ebx = (DWORD)(-1); // Not running VPC  
    	ctx->Eip += 4; // skip past the "call VPC" opcodes  
    	return (DWORD)EXCEPTION_CONTINUE_EXECUTION; // we can safely resume execution since we skipped faulty instruction  
	}  

	BOOL CFSystemUtil::IsInsideVPC()
	{
	    bool rc = false;  

	    __try  
	    {  
	        _asm push ebx  
	        _asm mov  ebx, 0 // Flag  
	        _asm mov  eax, 1 // VPC function number  

	        // call VPC   
	        _asm __emit 0Fh  
	        _asm __emit 3Fh  
	        _asm __emit 07h  
	        _asm __emit 0Bh  

	        _asm test ebx, ebx  
	        _asm setz [rc]  
	        _asm pop ebx  
	    }  
	    // The except block shouldn't get triggered if VPC is running!!  
	    __except(IsInsideVPC_exceptionFilter(GetExceptionInformation()))  
	    {  
	    }  

	    return rc;  
	}

	FTLINLINE bool _IsInsideVMWare()
	{
		bool r;
		_asm
		{
			push   edx
			push   ecx
			push   ebx

			mov    eax, 'VMXh'
			mov    ebx, 0 // any value but MAGIC VALUE
			mov    ecx, 10 // get VMWare version
			mov    edx, 'VX' // port number
			in     eax, dx // read port
			// on return EAX returns the VERSION
			cmp    ebx, 'VMXh' // is it a reply from VMWare?
			setz   [r] // set return value

			pop    ebx
			pop    ecx
			pop    edx
		}
		return r;
	}

	BOOL CFSystemUtil::IsInsideVMWare()
	{
		BOOL bRet = FALSE;
		__try
		{
			bRet = !!_IsInsideVMWare();
		}
		__except(1) // 1 = EXCEPTION_EXECUTE_HANDLER
		{
			
		}
		return bRet;
	}
#endif 

    BOOL CFSystemUtil::IsRunningOnRemoteDesktop()
    {
        BOOL bRet = FALSE;
        BOOL bRunningOnRemoteDesktop = FALSE;
        DWORD dwSessionId = 0;
        API_VERIFY(ProcessIdToSessionId(GetCurrentProcessId(), &dwSessionId));
        if (bRet)
        {
            if (WTSGetActiveConsoleSessionId() != dwSessionId)
            {
                bRunningOnRemoteDesktop = TRUE;
            }
        }
        return bRunningOnRemoteDesktop;
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
        HANDLE hFind = ::FindFirstFile(strFormater, &findData);
        API_VERIFY_EXCEPT1( (INVALID_HANDLE_VALUE != (hFind)), ERROR_PATH_NOT_FOUND);
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