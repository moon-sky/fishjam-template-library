#ifndef FTL_DTE_HELPER_H
#define FTL_DTE_HELPER_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlDTEHelper requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlDTEHelper requires ftlCom.h to be included first
#endif


namespace FTL
{
    //using namespace EnvDTE;

    FTLEXPORT class CFDTEUtil
    {
    public:
        FTLINLINE static LPCTSTR GetMsoControlTypeString(MsoControlType controlType);
        FTLINLINE static LPCTSTR GetMsoBarTypeString(MsoBarType barType);
		FTLINLINE static LPCTSTR GetWindowTypeString(EnvDTE::vsWindowType nWindowType);
        FTLINLINE static LPCTSTR GetElementKindString(EnvDTE::vsCMElement nElementKind);
        FTLINLINE static LPCTSTR GetFunctionKindString(FTL::CFStringFormater& strFormater, int nFunctionKind); //vsCMFunction
        FTLINLINE static LPCTSTR GetDTEGuidStringInfo(const CComBSTR& bstring);
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


    //Document
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

    

}//namespace FTL

#endif //FTL_DTE_HELPER_H

#ifndef USE_EXPORT
#  include "ftlDTEHelper.hpp"
#endif