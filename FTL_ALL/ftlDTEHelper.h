#ifndef FTL_DTE_HELPER_H
#define FTL_DTE_HELPER_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlDTEHelper requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlDTEHelper requires ftlCom.h to be included first
#endif

////一般可以在头文件中增加如下的定义
//#define USE_DTE	1
//#define USE_DTE80	1
//#define USE_MSVSC	1
//#define USE_VCCML	1

//在Dump一些信息(比如Commands)时，由于数量太大，太浪费时间，因此设置默认的最大个数
#ifndef MAX_DUMP_CHILD_ITEM_COUNT
#  define MAX_DUMP_CHILD_ITEM_COUNT	5
#endif 

//DTE
//"libid:80CC9F66-E7D8-4DDD-85B6-D9E6CD0E93E2" version("8.0")
#ifndef USE_DTE
#  define	USE_DTE		1
#endif
#if USE_DTE
//VxDTE( VSIP SDK ) or EnvDTE( import DTE )
#  ifndef DTE_NS
#    define DTE_NS EnvDTE
#  endif 
#endif //USE_DTE

//Microsoft Development Environment 8.0
//"libid:1A31287A-4D7D-413E-8E32-3B374931BD89" version("8.0")
#ifndef USE_DTE80
#  define USE_DTE80		0
#endif
#if USE_DTE80
//Null( VSIP SDK,  ) or EnvDTE80( import DTE80 )
#  ifndef DTE80_NS
#    define DTE80_NS	EnvDTE80
#  endif
#endif //USE_DTE80

//Microsoft_VisualStudio_CommandBars
//"libid:1CBA492E-7263-47BB-87FE-639000619B15" version("8.0")
#ifndef USE_MSVSC
#  define USE_MSVSC		0
#endif
#if USE_MSVSC
//Null( VSIP SDK, msotl.h ) or Microsoft_VisualStudio_CommandBars( import Microsoft_VisualStudio_CommandBars )
#  ifndef MSVSC_NS
#    define MSVSC_NS		Microsoft_VisualStudio_CommandBars
#  endif
#endif //USE_MSVSC

//Microsoft Development Environment VC++ Code Model 9.0 Type Library
//#import "libid:B5D4541F-A1F1-4CE0-B2E7-5DA402367104" version("9.0") lcid("0") raw_interfaces_only named_guids
#ifndef USE_VCCML
#  define USE_VCCML		0
#endif
#if USE_VCCML
#  ifndef VCCML_NS
#    define VCCML_NS	VCCodeModelLibrary
#  endif //VCCML_NS
#endif //USE_VCCML
/********************************************************************************************
* DTE -- DTE提供了 ObjectExtenders、IVsProfferCommands、IVsIntelliMouseHandler 等 至少 60 多个Service
********************************************************************************************/

namespace FTL
{
    //using namespace EnvDTE;

    FTLEXPORT class CFDTEUtil
    {
    public:
#if USE_MSVSC
		FTLINLINE static LPCTSTR GetMsoControlTypeString(MSVSC_NS::MsoControlType controlType);
        FTLINLINE static LPCTSTR GetMsoBarTypeString(MSVSC_NS::MsoBarType barType);
#endif //USE_MSVSC

		FTLINLINE static LPCTSTR GetWindowTypeString(DTE_NS::vsWindowType nWindowType);
		FTLINLINE static LPCTSTR GetElementKindString(DTE_NS::vsCMElement nElementKind);
        FTLINLINE static LPCTSTR GetFunctionKindString(FTL::CFStringFormater& strFormater, int nFunctionKind); //vsCMFunction
		FTLINLINE static LPCTSTR GetCMFunctionKindString(FTL::CFStringFormater& strFormater, int nFunctionKind);
        FTLINLINE static LPCTSTR GetDTEGuidStringInfo(const CComBSTR& bstring);

		FTLINLINE static HRESULT OpenDocumentAndGotoLine(DTE_NS::_DTE* pDTE, LPCTSTR pszFileName,int line, 
			CComPtr<DTE_NS::Document>& spFileDocument);
    };

    class CFDTEDumper : public CFInterfaceDumperBase<CFDTEDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFDTEDumper);
    public:
        FTLINLINE explicit CFDTEDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFDTEDumper>(pObj, pInfoOutput, nIndent){}
    public:
        //override
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };

	class CFObjectExtendersDumper : public CFInterfaceDumperBase<CFObjectExtendersDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFObjectExtendersDumper);
	public:
		FTLINLINE explicit CFObjectExtendersDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFObjectExtendersDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFAddinsDumper : public CFInterfaceDumperBase<CFAddinsDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFAddinsDumper);
	public:
		FTLINLINE explicit CFAddinsDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFAddinsDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFAddinDumper : public CFInterfaceDumperBase<CFAddinDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFAddinDumper);
	public:
		FTLINLINE explicit CFAddinDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFAddinDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};


    //Solution  info (project/projectitem)
    class CFSolutionDumper : public CFInterfaceDumperBase<CFSolutionDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFSolutionDumper);
    public:
        FTLINLINE explicit CFSolutionDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFSolutionDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };

    class CFProjectDumper : public CFInterfaceDumperBase<CFProjectDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFProjectDumper);
    public:
        FTLINLINE explicit CFProjectDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFProjectDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };

    class CFProjectItemsDumper : public CFInterfaceDumperBase<CFProjectItemsDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFProjectItemsDumper);
    public:
        FTLINLINE explicit CFProjectItemsDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFProjectItemsDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };

    class CFProjectItemDumper : public CFInterfaceDumperBase<CFProjectItemDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFProjectItemDumper);
    public:
        FTLINLINE explicit CFProjectItemDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFProjectItemDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };
   

    //Events
    class CFEventsDumper : public CFInterfaceDumperBase<CFEventsDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFEventsDumper);
    public:
        FTLINLINE explicit CFEventsDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFEventsDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };


    //Window
    class CFWindowsDumper : public CFInterfaceDumperBase<CFWindowsDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFWindowsDumper);
    public:
        FTLINLINE explicit CFWindowsDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFWindowsDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };

    class CFWindowDumper : public CFInterfaceDumperBase<CFWindowDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFWindowDumper);
    public:
        FTLINLINE explicit CFWindowDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFWindowDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };

    //Documents and Document
	class CFDocumentsDumper : public CFInterfaceDumperBase<CFDocumentsDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFDocumentsDumper);
	public:
		FTLINLINE explicit CFDocumentsDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFDocumentsDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

    class CFDocumentDumper : public CFInterfaceDumperBase<CFDocumentDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFDocumentDumper);
    public:
        FTLINLINE explicit CFDocumentDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFDocumentDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };

    class CFTextDocumentDumper : public CFInterfaceDumperBase<CFTextDocumentDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFTextDocumentDumper);
    public:
        FTLINLINE explicit CFTextDocumentDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFTextDocumentDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };
 
    class CFTextSelectionDumper : public CFInterfaceDumperBase<CFTextSelectionDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFTextSelectionDumper);
    public:
        FTLINLINE explicit CFTextSelectionDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFTextSelectionDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };

	class CFTextPointDumper : public CFInterfaceDumperBase<CFTextPointDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFTextPointDumper);
	public:
		FTLINLINE explicit CFTextPointDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFTextPointDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

    //Code
    class CFFileCodeModelDumper : public CFInterfaceDumperBase<CFFileCodeModelDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFFileCodeModelDumper);
    public:
        FTLINLINE explicit CFFileCodeModelDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFFileCodeModelDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };

    class CFCodeModelDumper : public CFInterfaceDumperBase<CFCodeModelDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCodeModelDumper);
    public:
        FTLINLINE explicit CFCodeModelDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFCodeModelDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };

    class CFCodeElementsDumper : public CFInterfaceDumperBase<CFCodeElementsDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCodeElementsDumper);
    public:
        FTLINLINE explicit CFCodeElementsDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFCodeElementsDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };

    class CFCodeElementDumper : public CFInterfaceDumperBase<CFCodeElementDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCodeElementDumper);
    public:
        FTLINLINE explicit CFCodeElementDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFCodeElementDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };


    //CommandBars
    class CFCommandBarsDumper : public CFInterfaceDumperBase<CFCommandBarsDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCommandBarsDumper);
    public:
        FTLINLINE explicit CFCommandBarsDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFCommandBarsDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };

    class CFCommandBarDumper : public CFInterfaceDumperBase<CFCommandBarDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCommandBarDumper);
    public:
        FTLINLINE explicit CFCommandBarDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFCommandBarDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };

    class CFCommandBarControlsDumper : public CFInterfaceDumperBase<CFCommandBarControlsDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCommandBarControlsDumper);
    public:
        FTLINLINE explicit CFCommandBarControlsDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFCommandBarControlsDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };

    class CFCommandBarControlDumper : public CFInterfaceDumperBase<CFCommandBarControlDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCommandBarControlDumper);
    public:
        FTLINLINE explicit CFCommandBarControlDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFCommandBarControlDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };


    //Commands
    class CFCommandsDumper : public CFInterfaceDumperBase<CFCommandsDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCommandsDumper);
    public:
        FTLINLINE explicit CFCommandsDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFCommandsDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };
    
    class CFCommandDumper : public CFInterfaceDumperBase<CFCommandDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCommandDumper);
    public:
        FTLINLINE explicit CFCommandDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CFCommandDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };

#if USE_VCCML
	class CFVCFileCodeModelDumper : public CFInterfaceDumperBase<CFVCFileCodeModelDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVCFileCodeModelDumper);
	public:
		FTLINLINE explicit CFVCFileCodeModelDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVCFileCodeModelDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVCCodeModelDumper : public CFInterfaceDumperBase<CFVCCodeModelDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVCCodeModelDumper);
	public:
		FTLINLINE explicit CFVCCodeModelDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVCCodeModelDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};
	
	class CFVCCodeElementDumper : public CFInterfaceDumperBase<CFVCCodeElementDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVCCodeElementDumper);
	public:
		FTLINLINE explicit CFVCCodeElementDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVCCodeElementDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVCCodeFunctionDumper : public CFInterfaceDumperBase<CFVCCodeFunctionDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVCCodeFunctionDumper);
	public:
		FTLINLINE explicit CFVCCodeFunctionDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVCCodeFunctionDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};
#endif	//USE_VCCML
	

}//namespace FTL

#endif //FTL_DTE_HELPER_H

#ifndef USE_EXPORT
#  include "ftlDTEHelper.hpp"
#endif