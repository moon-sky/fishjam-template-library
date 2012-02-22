#ifndef FTL_CONFIG_DETECT_H
#define FTL_CONFIG_DETECT_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlConfigDetect.h requires ftlbase.h to be included first
#endif

/************************************************************************
* 用于判断当前编译器的选项，通常放在 stdafx.h 文件的最下方
************************************************************************/


//#if defined(_MSC_VER) && (_MSC_VER >= 1020)

#  pragma message(__FILE__ "(" QQUOTE(__LINE__) ") : Begin IDE Config Detect.................................." )


//判断编译器类型
#if defined(__cplusplus)
#  pragma message( "  C++ Compiler" )
#else
#  pragma message( "  C Compiler" )
#endif

//目标平台(目前不完全)
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)         // Windows
#  pragma message( "  Target Platform is Windows" )
#elif defined(__APPLE__)    //Mac OS X
#  pragma message( "  Target Platform is Mac OS" )
#else
#  pragma message( "  Unknown Target Platform" )
#endif 

#if defined(_M_IX86)
#  pragma message( "  defined _M_IX86" )
#elif defined(_M_IA64)
#  pragma message( "  defined _M_IA64")
#elif defined(_M_AMD64)
#  pragma message( "  defined _M_AMD64" )
#else
#  pragma message( "Unknown CPU Type")
#endif

//判断是否是64位编译
#if defined (WIN32)
#  pragma message( "  WIN32 Version" )
#elif defined(_WIN64) && defined(WIN64)
#  pragma message( "  WIN64 Version" )
#else 
#  pragma message( "  Unknown WinXX Version" )
#endif


//检测编译器的版本
#if defined _MSC_VER  
#  pragma message( "  _MSC_VER = " QQUOTE(_MSC_VER) )
#  if _MSC_VER == 1200
#    pragma message( "     IDE is VC6")  //实际上由于ftlbase.h 使用了 strsafe.h 等文件和函数，因此是不支持的
#  elif _MSC_VER == 1310
#    pragma message( "     IDE is VS2003")
#  elif _MSC_VER == 1400
#    pragma message( "     IDE is VS2005")
#  elif _MSC_VER == 1500
#    pragma message( "     IDE is VS2008")
#  else
#    pragma message( "     IDE is Unknow Type")
#  endif
#endif


//判断是否是Unicode版本
//  UNICODE is used by Windows headers
//  _UNICODE is used by C-runtime/MFC headers
#if defined (UNICODE) || defined (_UNICODE)
#  pragma message( "  Use Unicode Character Set" )
#elif defined (_MBCS)
#  pragma message( "  Use Multi-Byte Character Set" )
#else
#  pragma message( "  Not Set Character Set" )
#endif //UNICODE


//判断是Debug版本还是Release版本
#if defined (DEBUG) || defined(_DEBUG)
#  pragma message( "  DEBUG Version, defined ( DEBUG or _DEBUG )")
#else
#  pragma message( "  Release Version, not define ( DEBUG )")
#endif //DEBUG

//判断是否支持多线程库
#if defined _MT
#  pragma message( "  Multi Thread Version" )
#else
#  pragma message( "  Single Thread Version" )
#endif //_MT


//判断是否支持CLR
#ifdef _MANAGED
#  pragma message( "  Managed C++ -- /CLR" )
#else
#  pragma message( "  not support Managed C++" )
#endif 

#if defined ( AFX_NO_CLR_COINIT_STA )
#  pragma message( "  defined AFX_NO_CLR_COINIT_STA" )
#else
#  pragma message( "  not defined AFX_NO_CLR_COINIT_STA" )
#endif


#if defined(_WIN32_WCE)
#  pragma message( "  CE Specific _WIN32_WCE = " QQUOTE(_WIN32_WCE))
#endif 

#if defined(_DLL)
#  pragma message( "  CRT Dll version(/MD[d]) -- _DLL" )
#else
#  pragma message( "  CRT Static Lib Version(/MT[d])" )
#endif 

#if defined(_USRDLL)
#  pragma message( "  defined _USRDLL" )
#else
#  pragma message( "  not defined _USRDLL" )
#endif


//判断要求最小的系统和IE版本
#ifdef WINVER
#  pragma message( "  WINVER = " QQUOTE(WINVER) )
#  if WINVER >= 0x0600
#    pragma message( "     Minimum system required: Windows Vista")
#  elif WINVER >= 0x0502
#    pragma message( "     Minimum system required: Windows Server 2003")
#  elif WINVER >= 0x0501
#    pragma message( "     Minimum system required: Windows XP | Windows Me")
#  elif WINVER >= 0x0500
#    pragma message( "     Minimum system required: Windows 2000")
#  elif WINVER >= 0x0410
#    pragma message( "     Minimum system required: Windows 98")
#  elif WINVER >= 0x0400
#    pragma message( "     Minimum system required: Windows 95 | Windows NT 4.0")
#  else
#    pragma message( "     Minimum system required: Not Known")
#  endif 
#endif //WINVER

#ifdef _WIN32_WINNT
#  pragma message( "  _WIN32_WINNT = " QQUOTE(_WIN32_WINNT) )
#endif //_WIN32_WINNT

#ifdef _WIN32_WINDOWS
#  pragma message( "  _WIN32_WINDOWS = " QQUOTE(_WIN32_WINDOWS) )
#endif //_WIN32_WINDOWS

#ifdef _WIN32_IE
#  pragma message( "  _WIN32_IE = " QQUOTE(_WIN32_IE) )
#  if _WIN32_IE >= 0x0700
#    pragma message( "     Minimum Internet Explorer required: 7.0")
#  elif _WIN32_IE >= 0x0603
#    pragma message( "     Minimum Internet Explorer required: 6.0 SP2")
#  elif _WIN32_IE >= 0x0601
#    pragma message( "     Minimum Internet Explorer required: 6.0 SP1")
#  elif _WIN32_IE >= 0x0600
#    pragma message( "     Minimum Internet Explorer required: 6.0")
#  elif _WIN32_IE >= 0x0560
#    pragma message( "     Minimum Internet Explorer required: 5.6")
#  elif _WIN32_IE >= 0x0501
#    pragma message( "     Minimum Internet Explorer required: 5.01, 5.5")
#  elif _WIN32_IE >= 0x0500
#    pragma message( "     Minimum Internet Explorer required: 5.0, 5.0a, 5.0b")
#  elif _WIN32_IE >= 0x0401
#    pragma message( "     Minimum Internet Explorer required: 4.01")
#  elif _WIN32_IE >= 0x0400
#    pragma message( "     Minimum Internet Explorer required: 4.0")
#  elif _WIN32_IE >= 0x0300
#    pragma message( "     Minimum Internet Explorer required: 3.0, 3.01, 3.02")
#  else
#    pragma message( "     Minimum Internet Explorer required: Not Known")
#  endif 
#endif //_WIN32_IE

//NTDDI_WINXP、NTDDI_LONGHORN、
#ifdef NTDDI_VERSION
#  pragma message( "  NTDDI_VERSION = " QQUOTE(NTDDI_VERSION) )
#  if defined(NTDDI_LONGHORN) && (NTDDI_VERSION >= NTDDI_LONGHORN)
#    pragma message( "     Minimum system required: Windows Vista")
#  elif defined(NTDDI_WS03SP1) && (NTDDI_VERSION >= NTDDI_WS03SP1)
#    pragma message( "     Minimum system required: Windows Server 2003 SP1")
#  elif defined(NTDDI_WS03) && NTDDI_VERSION >= NTDDI_WS03
#    pragma message( "     Minimum system required: Windows Server 2003")
#  elif defined (NTDDI_WINXPSP2) && NTDDI_VERSION >= NTDDI_WINXPSP2
#    pragma message( "     Minimum system required: Windows XP SP2")
#  elif defined (NTDDI_WINXPSP1) && NTDDI_VERSION >= NTDDI_WINXPSP1
#    pragma message( "     Minimum system required: Windows XP SP1")
#  elif defined (NTDDI_WINXP) && NTDDI_VERSION >= NTDDI_WINXP
#    pragma message( "     Minimum system required: Windows XP")
#  elif defined (NTDDI_WIN2KSP4) && NTDDI_VERSION >= NTDDI_WIN2KSP4
#    pragma message( "     Minimum system required: Windows 2000 SP4")
#  elif defined (NTDDI_WIN2KSP3) && NTDDI_VERSION >= NTDDI_WIN2KSP3
#    pragma message( "     Minimum system required: Windows 2000 SP3")
#  elif defined (NTDDI_WIN2KSP2) && NTDDI_VERSION >= NTDDI_WIN2KSP2
#    pragma message( "     Minimum system required: Windows 2000 SP2")
#  elif defined (NTDDI_WIN2KSP1) && NTDDI_VERSION >= NTDDI_WIN2KSP1
#    pragma message( "     Minimum system required: Windows 2000 SP1")
#  elif defined (NTDDI_WIN2K) && NTDDI_VERSION >= NTDDI_WIN2K
#    pragma message( "     Minimum system required: Windows 2000")
#  else
#    pragma message( "     Minimum system required: Not Known")
#  endif
#endif //NTDDI_VERSION

//判断使用的各种库：如MFC、ATL、WTL等
#ifdef __AFX_H__
#  pragma message( "  Use MFC, _MFC_VER = " QQUOTE(_MFC_VER) )

#  if defined (_AFXDLL)
#    pragma message( "    Use MFC in a Shared DLL")
#  else
#    pragma message( "    Use MFC in a Static Library")
#  endif

#  if defined (_NATIVE_WCHAR_T_DEFINED) //Treat wchar_t as a native(Built-in) type(MFC use this way)
#    pragma message( "    Treat wchar_t as Built-in Type")
#  else
#    pragma message( "    Treat wchar_t as unsigned short")
#  endif

#  if defined (_AFX_MINREBUILD)
#    pragma message( "    defined _AFX_MINREBUILD")
#  else
#    pragma message( "    not defined _AFX_MINREBUILD")
#  endif

#  if defined (_AFX_FULLTYPEINFO)
#    pragma message( "    defined _AFX_FULLTYPEINFO = " QQUOTE(_AFX_FULLTYPEINFO) )
#  endif

#  if defined( _AFX_NOFORCE_LIBS )
#    pragma message( "    defined _AFX_NOFORCE_LIBS")
#  else
#    pragma message( "    not defined _AFX_NOFORCE_LIBS")
#  endif 

#  if defined(_AFX_NO_DEBUG_CRT)
#    pragma message( "    defined _AFX_NO_DEBUG_CRT")
#  else 
#    pragma message( "    not defined _AFX_NO_DEBUG_CRT")
#  endif

#  if defined( _AFX_NOFORCE_MANIFEST )
#    pragma message( "    defined _AFX_NOFORCE_MANIFEST")
#  else
#    pragma message( "    not defined _AFX_NOFORCE_MANIFEST")
#  endif 

#  if defined(_AFX_PACKING)
#    pragma message( "    _AFX_PACKING = " QQUOTE(_AFX_PACKING))
#  endif
#endif


#ifdef __ATLBASE_H__
#  pragma message( "  Use ATL, _ATL_VER = " QQUOTE(_ATL_VER))
#  ifdef _ATL_NO_COMMODULE
#    pragma message( "    defined _ATL_NO_COMMODULE")
#  else
#    pragma message( "    not defined _ATL_NO_COMMODULE")
#  endif

#  ifdef _ATL_MIN_CRT
//   注意：这个选项已经不再支持了，最好不要使用(参见 atlbase.h )
#    pragma message( "    defined _ATL_MIN_CRT")
#    pragma TODO(Using no longer supported _ATL_MIN_CRT)
#  else
#    pragma message( "    not defined _ATL_MIN_CRT")
#  endif

   //mixing compilation units that are built as native code with those that are built /clr
#  ifdef _ATL_MIXED
#    pragma message( "    defined _ATL_MIXED")
#  else
#    pragma message( "    not defined _ATL_MIXED")
#  endif 

#  ifdef _ATL_DLL
//   链接 atl.lib, 运行时依赖 ATL.dll
#    pragma message( "    defined _ATL_DLL")
#  else
//   链接静态库: atls.lib 或 atlsd.lib
#    pragma message( "    not defined _ATL_DLL")
#  endif 

#  ifdef _ATL_DLL_IMPL
#    pragma message( "    defined _ATL_DLL_IMPL")
#  else
#    pragma message( "    not defined _ATL_DLL_IMPL")
#  endif 

#  ifdef _ATL_NO_DEFAULT_LIBS
#    pragma message( "    defined _ATL_NO_DEFAULT_LIBS")
#  else
#    pragma message( "    not defined _ATL_NO_DEFAULT_LIBS")
#  endif 



#endif //__ATLBASE_H__


#ifdef __ATLAPP_H__
#  pragma message( "  Use WTL, _WTL_VER = " QQUOTE(_WTL_VER))
#endif //__AFX_H__


#ifdef BOOST_MSVC
#  pragma message( "  Use Boost , BOOST_MSVC = " QQUOTE(BOOST_MSVC))
#  if defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)
#    pragma message( "    defined BOOST_NO_MEMBER_TEMPLATE_FRIENDS")
#  endif
#  ifdef BOOST_MULTI_INDEX_HPP
#    pragma message( "    Use Boost MultiIndex")
#    if defined(BOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
#      pragma message( "      defined BOOST_MULTI_INDEX_DISABLE_SERIALIZATION")
#    endif
#    if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)    //通常 DEBUG 时打开
#      pragma message( "      defined BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING")
#    endif
#    if defined(BOOST_MULTI_INDEX_ENABLE_SAFE_MODE)             //通常 DEBUG 时打开
#      pragma message( "      defined BOOST_MULTI_INDEX_ENABLE_SAFE_MODE")
#    endif
#    if defined(BOOST_NO_POINTER_TO_MEMBER_TEMPLATE_PARAMETERS) 
#      pragma message( "      defined BOOST_NO_POINTER_TO_MEMBER_TEMPLATE_PARAMETERS")
#    endif
#  endif //BOOST_MULTI_INDEX_HPP
#endif //BOOST_MSVC
#  pragma message(__FILE__ "(" QQUOTE(__LINE__) ") : End IDE Config Detect.................................." )

#endif //FTL_CONFIG_DETECT_H