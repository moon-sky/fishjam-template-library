#ifndef FTL_DTE_HELPER_H
#define FTL_DTE_HELPER_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlDTEHelper requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlDTEHelper requires ftlCom.h to be included first
#endif

//#define USE_MSO
//#define USE_MSC		//Microsoft_VisualStudio_CommandBars

#ifndef DTE_NS		// VxDTE( VSIP SDK ) or EnvDTE( import DTE )
#  error Please define DTE_NS first
#endif 

//#ifndef VCCML_NS	// VCCodeModelLibrary
////Microsoft Development Environment VC++ Code Model 9.0 Type Library
////#import "libid:B5D4541F-A1F1-4CE0-B2E7-5DA402367104" version("9.0") lcid("0") raw_interfaces_only named_guids
//#  error Please define VCCML_NS first
//#endif 


/********************************************************************************************
* DTE -- DTE提供了 ObjectExtenders、IVsProfferCommands、IVsIntelliMouseHandler 等 至少 60 多个Service
********************************************************************************************/

namespace FTL
{
    //using namespace EnvDTE;

    FTLEXPORT class CFDTEUtil
    {
    public:

#ifdef USE_MSO
        FTLINLINE static LPCTSTR GetMsoControlTypeString(MsoControlType controlType);
        FTLINLINE static LPCTSTR GetMsoBarTypeString(MsoBarType barType);
#endif 
		FTLINLINE static LPCTSTR GetWindowTypeString(DTE_NS::vsWindowType nWindowType);
		FTLINLINE static LPCTSTR GetElementKindString(DTE_NS::vsCMElement nElementKind);
        FTLINLINE static LPCTSTR GetFunctionKindString(FTL::CFStringFormater& strFormater, int nFunctionKind); //vsCMFunction
#ifdef VCCML_NS
		FTLINLINE static LPCTSTR GetCMFunctionKindString(FTL::CFStringFormater& strFormater, int nFunctionKind);
#endif 
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


#ifdef USE_MSC //Microsoft_VisualStudio_CommandBars
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
#endif

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

	///////////////  Microsoft Development Environment VC++ Code Model 9.0 Type Library /////////////////////
    /////////////////////////////////         VCCodeModelLibrary            /////////////////////////////////
	/////////////////////////////////  B5D4541F-A1F1-4CE0-B2E7-5DA402367104 /////////////////////////////////
#ifdef VCCML_NS
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
#endif
	

}//namespace FTL

#endif //FTL_DTE_HELPER_H

#ifndef USE_EXPORT
#  include "ftlDTEHelper.hpp"
#endif