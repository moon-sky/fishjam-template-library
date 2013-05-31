///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlComDetect.h
/// @brief  Fishjam Template Library ComDetect Header File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
/// @defgroup ftlComDetect ftl Com Detect function and class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FTL_COM_DETECT_H
#define FTL_COM_DETECT_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlComDetect.h requires ftlbase.h to be included first
#endif

#include <atlcomcli.h>
#include <OAIdl.h>

//TODO -- HKLM下的Software\Classes\Interface 下也有接口，似乎不一样？
/**********************************************************************************************************
* 有两种模式查找一个组件支持的接口及数目（QI）
*   1.注册表的 HKEY_CLASSES_ROOT\Interface 目录下有本机上注册的所有接口(包括名字) -- CoDetectInterfaceFromRegister
*     优点：可以随着注册的接口数进行扩展，而且能找到自定义接口；
*     缺点：不能找到本机上没有注册的（如：不能从ActiveX控件中找到 IForegroundTransfer 接口）
*           不能从找到的接口中获取进一步的信息；
*           速度比较慢；
*   2.从自定义的接口列表中查找(CoDetectInterfaceFromList) -- 不能找到接口列表中没有写入的；
*     优点：能自定义需要查找的接口，而且速度快；
*           能从接口中获得进一步的信息 -- 通过 DumpInterfaceInfo 方法
*     缺点：只能从列表中查找，范围较小。而且对不同的编译器需要定义不同的编译条件，并包含不同的头文件
* 
* 自动从 IDL 编译后的 .h 文件中提取出接口的宏(在VS2003下制作) -- 两个文件之间切换，查找并拷贝
* 注意：最好只打开两个文件；禁用  Visual Assist；先激活IDL的头文件，并将光标放在最前面
Option Strict Off
Option Explicit Off
Imports EnvDTE
Imports System.Diagnostics

Public Module InsertInterfaceDetectEntry
Sub GetInterfaceName()
DTE.ExecuteCommand("Edit.Find")
DTE.Find.FindWhat = "typedef interface "
DTE.Find.Target = vsFindTarget.vsFindTargetCurrentDocument
DTE.Find.MatchCase = True
DTE.Find.MatchWholeWord = False
DTE.Find.Backwards = False
DTE.Find.MatchInHiddenText = False
DTE.Find.PatternSyntax = vsFindPatternSyntax.vsFindPatternSyntaxLiteral
DTE.Find.Action = vsFindAction.vsFindActionFind
DTE.Find.Execute()
DTE.Windows.Item(Constants.vsWindowKindFindReplace).Close()
DTE.ActiveDocument.Selection.CharRight()
DTE.ActiveDocument.Selection.WordRight(True)
DTE.ActiveDocument.Selection.CharLeft(True)
DTE.ActiveDocument.Selection.Copy()
DTE.ExecuteCommand("Window.NextDocumentWindow")
DTE.ActiveDocument.Selection.Text = "DETECT_INTERFACE_ENTRY("
DTE.ActiveDocument.Selection.Paste()
DTE.ActiveDocument.Selection.Text = ")"
DTE.ActiveDocument.Selection.NewLine()
DTE.ExecuteCommand("Window.NextDocumentWindow")
End Sub
End Module
**********************************************************************************************************/


#include <atlconv.h>

namespace FTL
{
    FTLINLINE HRESULT GetInterfaceNameByIID(REFIID rclsid, BSTR * pszName);
}

#ifndef INCLUDE_DETECT_ATLIFACE
# define INCLUDE_DETECT_ATLIFACE    1
#endif

#ifndef INCLUDE_DETECT_ACTIVSCP
# define INCLUDE_DETECT_ACTIVSCP    1
#endif

#ifndef INCLUDE_DETECT_AUDIOPOLICY
# define INCLUDE_DETECT_AUDIOPOLICY    0
#endif

#ifndef INCLUDE_DETECT_COMCAT
# define INCLUDE_DETECT_COMCAT     1
#endif

#ifndef INCLUDE_DETECT_CONTROL
# define INCLUDE_DETECT_CONTROL     1
#endif

#ifndef INCLUDE_DETECT_DDRAW
# define INCLUDE_DETECT_DDRAW    0
#endif //INCLUDE_DETECT_DDRAW

#ifndef INCLUDE_DETECT_DIRECT3D
#  define INCLUDE_DETECT_DIRECT3D 0
#endif //INCLUDE_DETECT_DIRECT3D

#ifndef INCLUDE_DETECT_DISPEX
# define INCLUDE_DETECT_DISPEX      1
#endif //INCLUDE_DETECT_DISPEX

#ifndef INCLUDE_DETECT_DOCOBJ
# define INCLUDE_DETECT_DOCOBJ      1
#endif //INCLUDE_DETECT_DOCOBJ

#ifndef INCLUDE_DETECT_DTE
#   define INCLUDE_DETECT_DTE       0
#endif 

#ifndef INCLUDE_DETECT_EXDISP
# define INCLUDE_DETECT_EXDISP      1
#endif

#ifndef INCLUDE_DETECT_KSPROXY
# define INCLUDE_DETECT_KSPROXY     0
#endif

//Intel Media SDK
#ifndef INCLUDE_DETECT_INTEL_MEDIA_SDK
# define INCLUDE_DETECT_INTEL_MEDIA_SDK     0
#endif

//MainConcept SDK
#ifndef INCLUDE_DETECT_MAINCONCEPT
# define INCLUDE_DETECT_MAINCONCEPT     0
#endif

#ifndef INCLUDE_DETECT_MEDIAOBJ
# define INCLUDE_DETECT_MEDIAOBJ    0
#endif

//Media Foundation
#ifndef INCLUDE_DETECT_MEDIA_FOUNDATION
# define INCLUDE_DETECT_MEDIA_FOUNDATION    0
#endif

#ifndef INCLUDE_DETECT_MSHTMLC
# define INCLUDE_DETECT_MSHTMLC     1
#endif  

#ifndef INCLUDE_DETECT_MSHTMHST
# define INCLUDE_DETECT_MSHTMHST	1
#endif 

#ifndef INCLUDE_DETECT_MSXML
# define INCLUDE_DETECT_MSXML       1
#endif

//Net Firewall
#ifndef INCLUDE_DETECT_NETFW
# define INCLUDE_DETECT_NETFW       1
#endif

#ifndef INCLUDE_DETECT_OAIDL
# define INCLUDE_DETECT_OAIDL       1
#endif 

#ifndef INCLUDE_DETECT_OBJIDL
# define INCLUDE_DETECT_OBJIDL      1
#endif

#ifndef INCLUDE_DETECT_OBJSAFE
# define INCLUDE_DETECT_OBJSAFE     1
#endif

#ifndef INCLUDE_DETECT_OCIDL
# define INCLUDE_DETECT_OCIDL       1
#endif 

#ifndef INCLUDE_DETECT_OLEIDL
# define INCLUDE_DETECT_OLEIDL      1
#endif

#ifndef INCLUDE_DETECT_PROPSYS
# define INCLUDE_DETECT_PROPSYS      1
#endif

#ifndef INCLUDE_DETECT_QEDIT
# define INCLUDE_DETECT_QEDIT       0
#endif

#ifndef INCLUDE_DETECT_RICHOLE
# define INCLUDE_DETECT_RICHOLE       0
#endif

#ifndef INCLUDE_DETECT_SERVPROV
# define INCLUDE_DETECT_SERVPROV    1
#endif 

#ifndef INCLUDE_DETECT_SHLOBJ
# define INCLUDE_DETECT_SHLOBJ      0
#endif

#ifndef INCLUDE_DETECT_STRMIF
# define INCLUDE_DETECT_STRMIF      0
#endif

#ifndef INCLUDE_DETECT_URLMON
# define INCLUDE_DETECT_URLMON      1
#endif

#ifndef INCLUDE_DETECT_VIDEOACC
# define INCLUDE_DETECT_VIDEOACC	0
#endif 

#ifndef INCLUDE_DETECT_VFW
#    define INCLUDE_DETECT_VFW     1
#endif 

#ifndef INCLUDE_DETECT_VSIP
#    define INCLUDE_DETECT_VSIP     0
#endif 

#ifndef INCLUDE_DETECT_WM_CODEC_DSP
#    define INCLUDE_DETECT_WM_CODEC_DSP     1
#endif 


#ifndef INCLUDE_DETECT_WMF
//Windows Media Format(asf,wma,wmv)
# define INCLUDE_DETECT_WMF         0
#endif

#if INCLUDE_DETECT_WMF
#   define INCLUDE_DETECT_DSHOWASF  1
#   define INCLUDE_DETECT_WMSDKIDL  1
# else
#   define INCLUDE_DETECT_DSHOWASF  0
#   define INCLUDE_DETECT_WMSDKIDL  0
#endif

#if INCLUDE_DETECT_ATLIFACE
#  include <atliface.h>
#endif

#if INCLUDE_DETECT_ACTIVSCP
#  include <ActivScp.h>
#endif

#if INCLUDE_DETECT_AUDIOPOLICY
#  include <audiopolicy.h>
#endif 

#if INCLUDE_DETECT_COMCAT
#  include <ComCat.h>
#endif 

#if INCLUDE_DETECT_CONTROL
#  include <control.h>
#endif 

#if INCLUDE_DETECT_DDRAW
#  include <ddraw.h>
#endif

#if INCLUDE_DETECT_DIRECT3D
#  include <d3d9types.h>
#  include <dxgi.h>
#endif

#if INCLUDE_DETECT_DISPEX
#  include <DispEx.h>
#endif

#if INCLUDE_DETECT_DOCOBJ
#  include <DocObj.h>
#endif

#if INCLUDE_DETECT_DSHOWASF
#  include <dshowasf.h>
#endif

#if INCLUDE_DETECT_EXDISP
#  include <exdisp.h>
#endif

#if INCLUDE_DETECT_KSPROXY
#  include <ks.h>
#  include <ksproxy.h>
#endif 

#if INCLUDE_DETECT_INTEL_MEDIA_SDK
#  include <mfxvideo.h>
#  include <mfxvideo++.h>
#  include <d3d_allocator.h>
#  include <mfx_filter_externals.h>
#  include <memory_allocator.h>
//#  include <samples/sample_common/include/sample_utils.h>
//#  include <samples/sample_dshow_plugins/common/include/codec_presets.h>
//#  include <samples/sample_dshow_plugins/video_enc_filter/include/mfx_video_enc_proppage.h>
#endif 

#if INCLUDE_DETECT_MAINCONCEPT
#  include <ModuleConfig.h>
#endif 

#if INCLUDE_DETECT_MEDIAOBJ
#  include <mediaobj.h>
#endif 

#if INCLUDE_DETECT_MEDIA_FOUNDATION
#  include <dxva2api.h>
#  include <evr.h>
#  include <mfidl.h>
#  include <mfobjects.h>
#  include <mftransform.h>
#  include <ftlMediaFoundation.h>
#endif

#if INCLUDE_DETECT_MSHTMLC
#  include <Mshtmlc.h>
#endif

#if INCLUDE_DETECT_MSHTMHST
#  include <MsHtmHst.h>
#endif

#if INCLUDE_DETECT_MSXML
#  include <msxml.h>
#endif 

#if INCLUDE_DETECT_NETFW
#  include <netfw.h>
#endif 

#if INCLUDE_DETECT_OAIDL
#  include <Oaidl.h>
#endif

#if INCLUDE_DETECT_OBJIDL
#  include <ObjIdl.h>
#endif

#if INCLUDE_DETECT_OBJSAFE
#  include <ObjSafe.h>
#endif

#if INCLUDE_DETECT_OCIDL
#  include <OCIdl.h>
#endif 

#if INCLUDE_DETECT_OLEIDL
#  include <OleIdl.h>
#endif 

#if INCLUDE_DETECT_PROPSYS
#  include <propsys.h>
#endif 

#if INCLUDE_DETECT_QEDIT
#  include <qedit.h>
#endif 

#if INCLUDE_DETECT_RICHOLE
#  include <RichOle.h>
#endif 

#if INCLUDE_DETECT_SERVPROV
#  include <ServProv.h>
#endif 

#if INCLUDE_DETECT_SHLOBJ
#  include <ShlObj.h>
#endif 

#if INCLUDE_DETECT_STRMIF
#  include <strmif.h>
#  include <dmodshow.h>
#  include <amvideo.h>
#  include <qnetwork.h>
#endif 

#if INCLUDE_DETECT_URLMON
#  include <UrlMon.h>
#endif 

#if INCLUDE_DETECT_VIDEOACC
#  include <videoacc.h>
#endif 

#if INCLUDE_DETECT_VFW
#  include <vfw.h>
#  pragma comment(lib, "Vfw32.lib")
//#  pragma comment(lib, "msvfw32.Lib")
#endif 

#if INCLUDE_DETECT_VSIP
#  include <containedlanguage.h>
#  include <textmgr.h>
#  include <vsshell.h>
#  include <vssplash.h>
#endif 

#if INCLUDE_DETECT_WM_CODEC_DSP
#  include <wmcodecdsp.h>
#endif 

#if INCLUDE_DETECT_WMSDKIDL
#  include <wmsdkidl.h>
#endif 

#ifdef FTL_DEBUG
//从自定义列表中检测接口指针支持(能进行QI)的接口
# define COM_DETECT_INTERFACE_FROM_LIST(pUnk) \
    {\
        FTLTRACEEX(FTL::tlTrace, TEXT("%s(%d) : Begin Detect Interface %s( 0x%p ) From List\n"),TEXT(__FILE__),__LINE__,TEXT(#pUnk),pUnk);\
        DWORD dwIntCount = FTL::CFComDetect::CoDetectInterfaceFromList(pUnk,GUID_NULL,FTL::CFComDetect::cdtInterface);\
        FTLTRACEEX(FTL::tlInfo,TEXT("%s's Interfaces Count are at least %d\n\n"),TEXT(#pUnk),dwIntCount);\
    }

//从自定义列表中检测 IMoniker 接口指针能 BindToObject 的接口
# define COM_DETECT_MONIKER_BIND_TO_OBJECT_FROM_LIST(pMoniker)\
    {\
        FTLTRACEEX(FTL::tlTrace,TEXT("%s(%d) : Begin Detect Moniker Can Bind to Object %s( 0x%p ) From List\n"),\
        TEXT(__FILE__),__LINE__,TEXT(#pMoniker),pMoniker);\
        DWORD dwIntCount = FTL::CFComDetect::CoDetectInterfaceFromList(pMoniker,GUID_NULL,FTL::CFComDetect::cdtMonikerBind);\
        FTLTRACEEX(FTL::tlTrace,TEXT("%s Can Bind to at least %d Interfaces\n\n"),TEXT(#pMoniker),dwIntCount);\
    }

//从列表中检查 IServiceProvider::QueryService 能Query 到的接口
//  其中的 guidService 也使用 riid，能检测出大部分的接口，但有部分不能检查出，
//  比如：EnvDTE 中 的 QueryService(SID_SVsTextManager, IID_IVsTextManager, xxxx) -- SID_SVsTextManager 和 IID_IVsTextManager 不一样
# define COM_DETECT_SERVICE_PROVIDER_FROM_LIST(pServiceProvider) \
    {\
        FTLTRACEEX(FTL::tlTrace, TEXT("%s(%d) : Begin Detect Service Provider %s( 0x%p ) From List\n"),\
            TEXT(__FILE__),__LINE__,TEXT(#pServiceProvider),pServiceProvider);\
        DWORD dwServiceCount = FTL::CFComDetect::CoDetectInterfaceFromList(pServiceProvider,GUID_NULL,FTL::CFComDetect::cdtService);\
        FTLTRACEEX(FTL::tlInfo,TEXT("%s's Services Count are at least %d\n\n"),TEXT(#pServiceProvider),dwServiceCount);\
    }

//从自定义列表中检测希望检测的RIID是什么接口（如用于 DirectShow 中的 NonDelegatingQueryInterface）
//使用同前面相同的自定义列表
# define COM_DETECT_RIID_FROM_LIST(riid)\
    {\
        USES_CONVERSION;\
        LPOLESTR  lpszRIID = NULL;\
        StringFromIID(riid,&lpszRIID);\
        FTLTRACEEX(FTL::tlTrace,TEXT("%s(%d) : Begin Detect RIID %s(%s) From List\n"),TEXT(__FILE__),__LINE__,TEXT(#riid),OLE2T(lpszRIID));\
        DWORD dwIntCount = FTL::CFComDetect::CoDetectInterfaceFromList(NULL,riid,FTL::CFComDetect::cdtIID);\
        if(0 == dwIntCount)\
        {\
            FTLTRACEEX(FTL::tlWarning, TEXT("Can't Detect RIID %s(%s).\n"),TEXT(#riid),OLE2T(lpszRIID));\
        }\
        CoTaskMemFree(lpszRIID);\
    }


//从注册表中检测接口指针支持(能进行QI)的接口
# define COM_DETECT_INTERFACE_FROM_REGISTER(pUnknown) \
    {\
        FTLTRACEEX(FTL::tlTrace,TEXT("%s(%d) : Begin Detect Interface %s( 0x%p ) From Register\n"),TEXT(__FILE__),__LINE__,TEXT(#pUnknown),pUnknown);\
        DWORD dwIntCount = FTL::CFComDetect::CoDetectInterfaceFromRegister(pUnknown, GUID_NULL,FTL::CFComDetect::cdtInterface);\
        FTLTRACEEX(FTL::tlTrace,TEXT("%s's Interfaces Count are at least %d\n\n"),TEXT(#pUnknown),dwIntCount);\
    }

# define COM_DETECT_MONIKER_BIND_TO_OBJECT_FROM_REGISTER(pMoniker) \
	{\
		FTLTRACEEX(FTL::tlTrace,TEXT("%s(%d) : Begin Detect Moniker Can Bind to Object %s( 0x%p ) From Register\n"),TEXT(__FILE__),__LINE__,TEXT(#pMoniker),pMoniker);\
		DWORD dwIntCount = FTL::CFComDetect::CoDetectInterfaceFromRegister(pMoniker, GUID_NULL,FTL::CFComDetect::cdtMonikerBind);\
		FTLTRACEEX(FTL::tlTrace,TEXT("%s's Can Bind to at least %d\n\n"),TEXT(#pMoniker),dwIntCount);\
	}

//IVsWindowFrame::QueryViewInterface
# define COM_DETECT_VIEW_INTERFACE_FROM_REGISTER(pUnknown) \
	{\
		FTLTRACEEX(FTL::tlTrace,TEXT("%s(%d) : Begin Detect View Interface %s( 0x%p ) From Register\n"),TEXT(__FILE__),__LINE__,TEXT(#pUnknown),pUnknown);\
		DWORD dwIntCount = FTL::CFComDetect::CoDetectInterfaceFromRegister(pUnknown, GUID_NULL,FTL::CFComDetect::cdtViewInterface);\
		FTLTRACEEX(FTL::tlTrace,TEXT("%s's Interfaces Count are at least %d\n\n"),TEXT(#pUnknown),dwIntCount);\
	}

# define COM_DETECT_SERVICE_PROVIDER_FROM_REGISTER(pUnknown) \
	{\
		FTLTRACEEX(FTL::tlTrace, TEXT("%s(%d) : Begin Detect Service Provider %s( 0x%p ) From Register\n"),\
		TEXT(__FILE__),__LINE__,TEXT(#pUnknown),pUnknown);\
		DWORD dwServiceCount = FTL::CFComDetect::CoDetectInterfaceFromRegister(pUnknown,GUID_NULL,FTL::CFComDetect::cdtService);\
		FTLTRACEEX(FTL::tlInfo,TEXT("%s's Services Count are at least %d\n\n"),TEXT(#pUnknown),dwServiceCount);\
	}

#else //FTL_DEBUG
# define COM_DETECT_INTERFACE_FROM_LIST(pUnk)        (void)pUnk;
# define COM_DETECT_RIID_FROM_LIST(riid)             (void)riid;
# define COM_DETECT_INTERFACE_FROM_REGISTER(pUnk)    (void)pUnk;
#endif  //NONE FTL_DEBUG

	#define DETECT_INTERFACE_ENTRY_IID(IntType, riid) \
		DETECT_INTERFACE_ENTRY_EX_IID(IntType,riid,CFDummyDump)

	#define DETECT_INTERFACE_ENTRY(IntType) \
		DETECT_INTERFACE_ENTRY_EX_IID(IntType,__uuidof(IntType),CFDummyDump)

	#define DETECT_INTERFACE_ENTRY_EX(IntType,classDumpInfo) \
		DETECT_INTERFACE_ENTRY_EX_IID(IntType,__uuidof(IntType),classDumpInfo)


//新的方式 -- 效率高(每个接口对应的宏只是数组中的一项)，但不能使用接口的实例
//  怎么在数组中存储 接口 的类型，或者尝试在接口的数组中保存接口的实例指针 ?
typedef HRESULT (*DumpInterfaceInfoProc)(IUnknown* pUnknown);

struct CFInterfaceEntryExIID
{
	//DWORD_PTR m_vtbl;   // filled in with USE_INTERFACE_PART
	//IUnknown*  m_pUnk;
	LPCTSTR		pszInterfaceName;
	GUID	  id;
	DumpInterfaceInfoProc	m_pDumpInfoProc;
	//CFInterfaceEntryExIID() { m_pUnk = 0; }
};


#define BEGIN_DETECT_INTERFACE() \
		HRESULT hr = E_FAIL;\
		DWORD dwInterfaceCount = 0;\
		DWORD dwTotalCheckCount = 0;\
		IMoniker* pMoniker = NULL;\
		IServiceProvider* pSvrProvider = NULL;\
		if(CFComDetect::cdtMonikerBind == detectType)\
		{\
			COM_VERIFY((pUnknown)->QueryInterface(IID_IMoniker,(void**)(&pMoniker)));\
			if(!pMoniker) {return DWORD(-1);}\
		}\
		else if(CFComDetect::cdtService == detectType)\
		{\
			COM_VERIFY((pUnknown)->QueryInterface(IID_IServiceProvider,(void**)(&pSvrProvider)));\
			if(!pSvrProvider) {return DWORD(-1);}\
		}\
		static CFInterfaceEntryExIID allInterfaceEntries[] = \
		{\

#define DETECT_INTERFACE_ENTRY_EX_IID(IntType,riid,classDumpInfo) \
			{	TEXT(#IntType), riid, classDumpInfo::DumpInterfaceInfo },
	
#define END_DETECT_INTERFACE() \
			{	NULL, GUID_NULL, CFDummyDump::DumpInterfaceInfo }\
		};\
		CFInterfaceEntryExIID* pEntry = &allInterfaceEntries[0];\
		while(pEntry && pEntry->id != GUID_NULL)\
		{\
			dwTotalCheckCount++;\
			if(FTL::CFComDetect::cdtInterface == detectType)\
			{\
				IUnknown* pQueryUnknown = NULL;\
				hr = (pUnknown)->QueryInterface(pEntry->id,(void**)(&pQueryUnknown));\
				if(SUCCEEDED(hr) && pQueryUnknown)\
				{\
					dwInterfaceCount++;\
					if(pCallback){ (*pCallback)(pCallbackParm, pQueryUnknown, dwInterfaceCount, pEntry->id, pEntry->pszInterfaceName); } \
					pEntry->m_pDumpInfoProc(pQueryUnknown);\
					pQueryUnknown->Release();\
					pQueryUnknown = NULL;\
				}\
				else if(E_NOINTERFACE != hr)\
				{\
					FTLTRACEEX(tlWarning,TEXT("Warning: Detect %s ,return 0x%p\n"),pEntry->pszInterfaceName,hr);\
				}\
			}\
			else if(FTL::CFComDetect::cdtIID == detectType)\
			{\
				if(pEntry->id == checkRIID)\
				{\
					dwInterfaceCount++;\
					FTLTRACEEX(FTL::tlTrace,TEXT("\tRiid is %s\n"),pEntry->pszInterfaceName);\
				}\
			}\
			else if(FTL::CFComDetect::cdtMonikerBind == detectType)\
			{\
				IUnknown* pBindUnknown = NULL;\
				hr = (pMoniker)->BindToObject(NULL,NULL,pEntry->id,(void**)(&pBindUnknown));\
				if(SUCCEEDED(hr) && pBindUnknown != NULL)\
				{\
					dwInterfaceCount++;\
					FTLTRACEEX(FTL::tlTrace,TEXT("\t%d: %s\n"),dwInterfaceCount,pEntry->pszInterfaceName);\
					pBindUnknown->Release();\
					pBindUnknown = NULL;\
				}\
			}\
			else if(FTL::CFComDetect::cdtService == detectType)\
			{\
				IUnknown* pServiceUnknown = NULL;\
				hr = (pSvrProvider)->QueryService(pEntry->id, pEntry->id,(void**)(&pServiceUnknown));\
				if(SUCCEEDED(hr) && pServiceUnknown != NULL)\
				{\
					dwInterfaceCount++;\
					FTLTRACEEX(FTL::tlTrace,TEXT("\t%d: %s\n"),dwInterfaceCount,pEntry->pszInterfaceName);\
					pServiceUnknown->Release();\
					pServiceUnknown = NULL;\
				}\
				else if(E_NOINTERFACE != hr && E_FAIL != hr)\
				{\
					FTLTRACEEX(tlWarning,TEXT("Warning: Detect Service %s ,return 0x%p\n"),pEntry->pszInterfaceName,hr);\
				}\
			}\
			else\
			{\
				FTLTRACEEX(tlError,TEXT("\tUnknown Operation \n"));\
			}\
			pEntry++;\
		}\
		if(CFComDetect::cdtInterface == detectType || CFComDetect::cdtService == detectType)\
		{\
			FTLTRACEEX(FTL::tlTrace,TEXT("\tTotal Check %d Interfaces\n"),dwTotalCheckCount);\
		}\
		SAFE_RELEASE(pMoniker);\
		SAFE_RELEASE(pSvrProvider);\
		return dwInterfaceCount;

	
namespace FTL
{
	typedef HRESULT (*DetectInterfaceCallBackProc)(DWORD_PTR pParam, IUnknown* pUnknwon, 
		DWORD dwInterfaceCount, REFIID checkedRIIF, LPCTSTR pszInterfaceName);

	class CFComDetect
    {
    public:
        typedef enum ComDetectType
        {
            cdtInterface,       //QueryInterface
			cdtViewInterface,	//IVsWindowFrame::QueryViewInterface
            cdtIID,             //
            cdtMonikerBind,     //
            cdtService,         //QueryService(利用 SID_XXX 一般定义为 IID_XXX 的机制判断)
        }ComDetectType;

		FTLINLINE static DWORD CoDetectInterfaceFromRegister(IUnknown* pUnknown, REFIID checkRIID, 
			ComDetectType detectType);
        FTLINLINE static DWORD CoDetectInterfaceFromList(IUnknown* pUnknown, REFIID checkRIID, 
			ComDetectType detectType, DetectInterfaceCallBackProc pCallback = _DefaultDetectInterfaceCallBackProc, DWORD_PTR pCallbackParm = NULL);
	private:
		FTLINLINE static HRESULT _innerCoDtectInterfaceFromRegister(IUnknown* pUnknown, REFGUID guidInfo);
		FTLINLINE static HRESULT _innerCoDtectServiceFromRegister(IServiceProvider* pServiceProvider, REFGUID guidInfo);
#if INCLUDE_DETECT_VSIP
		FTLINLINE static HRESULT _innerCoDtectViewInterfaceFromRegister(IVsWindowFrame* pVsWindowFrame, REFGUID guidInfo);
#endif 

		FTLINLINE static HRESULT _DefaultDetectInterfaceCallBackProc(DWORD_PTR pParam, IUnknown* pUnknwon, 
			DWORD dwInterfaceCount, REFIID checkedRIIF, LPCTSTR pszInterfaceName);
    }; //class CFComDetect
}//namespace FTL

#endif //FTL_COM_DETECT_H

#ifndef USE_EXPORT
#  include "ftlComDetect.hpp"
#endif