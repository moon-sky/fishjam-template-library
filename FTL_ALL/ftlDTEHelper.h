#ifndef FTL_DTE_HELPER_H
#define FTL_DTE_HELPER_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlDTEHelper requires ftlbase.h to be included first
#endif


namespace FTL
{
    //using namespace EnvDTE;

    FTLEXPORT class CFDTEUtil
    {
    public:
        FTLINLINE static LPCTSTR GetMsoControlTypeString(MsoControlType controlType);
        FTLINLINE static LPCTSTR GetMsoBarTypeString(MsoBarType barType);
        FTLINLINE static LPCTSTR GetWindowTypeString(vsWindowType nWindowType);
        FTLINLINE static LPCTSTR GetElementKindString(vsCMElement nElementKind);
        FTLINLINE static LPCTSTR GetFunctionKindString(FTL::CFStringFormater& strFormater, int nFunctionKind); //vsCMFunction
        FTLINLINE static LPCTSTR GetDTEGuidStringInfo(const CComBSTR& bstring);
    };

    class IDTEInfoOutput
    {
    public:
        virtual HRESULT SetIndent(int nIndent) = 0;
        virtual HRESULT OutputInfoName(LPCTSTR pszInfoName) = 0;
        virtual HRESULT OnOutput(LPCTSTR pszKey, LPCTSTR pValue) = 0;
        virtual HRESULT OnOutput(LPCTSTR pszKey, BSTR* pValue) = 0;
        virtual HRESULT OnOutput(LPCTSTR pszKey, long nValue) = 0;
        virtual HRESULT OnOutput(LPCTSTR pszKey, VARIANT* pValue) = 0;
        virtual HRESULT OnOutput(LPCTSTR pszKey, long nTotal, long nIndex, VARIANT* pValue) = 0;
    };

    class CFDTEInfoStringOutput : public IDTEInfoOutput
    {
    public:
        FTLINLINE CFDTEInfoStringOutput();
        FTLINLINE ~CFDTEInfoStringOutput();
        FTLINLINE virtual HRESULT SetIndent(int nIndent);
        FTLINLINE virtual HRESULT OutputInfoName(LPCTSTR pszInfoName);
        FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, LPCTSTR pValue);
        FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, BSTR* pValue);
        FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, long nValue);
        FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, VARIANT* pValue);
        FTLINLINE virtual HRESULT OnOutput(LPCTSTR pszKey, long nTotal, long nIndex, VARIANT* pValue);
    protected:
        int     m_nIndent;
        TCHAR*  m_pszIndentSpace;
        FTLINLINE virtual VOID    OutputIndentSpace();
    };

    template <typename T>
    class CFDTEInterfaceDumperBase
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFDTEInterfaceDumperBase);
    public:
        FTLINLINE explicit CFDTEInterfaceDumperBase(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :m_pObj(pObj)
            ,m_nIndent(nIndent)
        {
            T* pT = static_cast<T*>(this);
            pInfoOutput->SetIndent(nIndent);
            pT->GetObjInfo(pInfoOutput);
        }
        
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput)
        {
            FTLASSERT(FALSE);
            return S_FALSE;
        }
    protected:
        CComPtr<IDispatch>  m_pObj;
        int                 m_nIndent;
    };

    class CFDTEDumper : public CFDTEInterfaceDumperBase<CFDTEDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFDTEDumper);
    public:
        FTLINLINE explicit CFDTEDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFDTEDumper>(pObj, pInfoOutput, nIndent){}
    public:
        //override
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    //Solution  info (project/projectitem)
    class CFSolutionDumper : public CFDTEInterfaceDumperBase<CFSolutionDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFSolutionDumper);
    public:
        FTLINLINE explicit CFSolutionDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFSolutionDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFProjectDumper : public CFDTEInterfaceDumperBase<CFProjectDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFProjectDumper);
    public:
        FTLINLINE explicit CFProjectDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFProjectDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFProjectItemsDumper : public CFDTEInterfaceDumperBase<CFProjectItemsDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFProjectItemsDumper);
    public:
        FTLINLINE explicit CFProjectItemsDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFProjectItemsDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFProjectItemDumper : public CFDTEInterfaceDumperBase<CFProjectItemDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFProjectItemDumper);
    public:
        FTLINLINE explicit CFProjectItemDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFProjectItemDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };
   

    //Events
    class CFEventsDumper : public CFDTEInterfaceDumperBase<CFEventsDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFEventsDumper);
    public:
        FTLINLINE explicit CFEventsDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFEventsDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };


    //Window
    class CFWindowsDumper : public CFDTEInterfaceDumperBase<CFWindowsDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFWindowsDumper);
    public:
        FTLINLINE explicit CFWindowsDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFWindowsDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFWindowDumper : public CFDTEInterfaceDumperBase<CFWindowDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFWindowDumper);
    public:
        FTLINLINE explicit CFWindowDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFWindowDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };


    //Document
    class CFDocumentDumper : public CFDTEInterfaceDumperBase<CFDocumentDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFDocumentDumper);
    public:
        FTLINLINE explicit CFDocumentDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFDocumentDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFTextSelectionDumper : public CFDTEInterfaceDumperBase<CFTextSelectionDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFTextSelectionDumper);
    public:
        FTLINLINE explicit CFTextSelectionDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFTextSelectionDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    //Code
    class CFFileCodeModelDumper : public CFDTEInterfaceDumperBase<CFFileCodeModelDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFFileCodeModelDumper);
    public:
        FTLINLINE explicit CFFileCodeModelDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFFileCodeModelDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFCodeModelDumper : public CFDTEInterfaceDumperBase<CFCodeModelDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCodeModelDumper);
    public:
        FTLINLINE explicit CFCodeModelDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFCodeModelDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFCodeElementsDumper : public CFDTEInterfaceDumperBase<CFCodeElementsDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCodeElementsDumper);
    public:
        FTLINLINE explicit CFCodeElementsDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFCodeElementsDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFCodeElementDumper : public CFDTEInterfaceDumperBase<CFCodeElementDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCodeElementDumper);
    public:
        FTLINLINE explicit CFCodeElementDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFCodeElementDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };


    //CommandBars
    class CFCommandBarsDumper : public CFDTEInterfaceDumperBase<CFCommandBarsDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCommandBarsDumper);
    public:
        FTLINLINE explicit CFCommandBarsDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFCommandBarsDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFCommandBarDumper : public CFDTEInterfaceDumperBase<CFCommandBarDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCommandBarDumper);
    public:
        FTLINLINE explicit CFCommandBarDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFCommandBarDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFCommandBarControlsDumper : public CFDTEInterfaceDumperBase<CFCommandBarControlsDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCommandBarControlsDumper);
    public:
        FTLINLINE explicit CFCommandBarControlsDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFCommandBarControlsDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFCommandBarControlDumper : public CFDTEInterfaceDumperBase<CFCommandBarControlDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCommandBarControlDumper);
    public:
        FTLINLINE explicit CFCommandBarControlDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFCommandBarControlDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };


    //Commands
    class CFCommandsDumper : public CFDTEInterfaceDumperBase<CFCommandsDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCommandsDumper);
    public:
        FTLINLINE explicit CFCommandsDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFCommandsDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };
    
    class CFCommandDumper : public CFDTEInterfaceDumperBase<CFCommandDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCommandDumper);
    public:
        FTLINLINE explicit CFCommandDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFCommandDumper>(pObj, pInfoOutput, nIndent){}
    public:
        FTLINLINE HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    

}//namespace FTL

#endif //FTL_DTE_HELPER_H

#ifndef USE_EXPORT
#  include "ftlDTEHelper.hpp"
#endif