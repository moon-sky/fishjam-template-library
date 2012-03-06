#ifndef FTL_DTE_HELPER_H
#define FTL_DTE_HELPER_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlDTEHelper requires ftlbase.h to be included first
#endif


namespace FTL
{
    using namespace EnvDTE;
    
    FTLEXPORT class CFDTEUtil
    {
    public:
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
        virtual HRESULT OnOutput(LPCTSTR pszKey, long nCount) = 0;
        virtual HRESULT OnOutput(LPCTSTR pszKey, VARIANT* pValue) = 0;
        virtual HRESULT OnOutput(LPCTSTR pszKey, long nTotal, long nIndex, VARIANT* pValue) = 0;
    };

    class CFDTEInfoStringOutput : public IDTEInfoOutput
    {
    public:
        CFDTEInfoStringOutput();
        ~CFDTEInfoStringOutput();
        virtual HRESULT SetIndent(int nIndent);
        virtual HRESULT OutputInfoName(LPCTSTR pszInfoName);
        virtual HRESULT OnOutput(LPCTSTR pszKey, LPCTSTR pValue);
        virtual HRESULT OnOutput(LPCTSTR pszKey, BSTR* pValue);
        virtual HRESULT OnOutput(LPCTSTR pszKey, long nCount);
        virtual HRESULT OnOutput(LPCTSTR pszKey, VARIANT* pValue);
        virtual HRESULT OnOutput(LPCTSTR pszKey, long nTotal, long nIndex, VARIANT* pValue);
    protected:
        int     m_nIndent;
        TCHAR*  m_pszIndentSpace;
        virtual VOID    OutputIndentSpace();
    };

    template <typename T>
    class CFDTEInterfaceDumperBase
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFDTEInterfaceDumperBase);
    public:
        CFDTEInterfaceDumperBase(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :m_pObj(pObj)
            ,m_nIndent(nIndent)
        {
            T* pT = static_cast<T*>(this);
            pInfoOutput->SetIndent(nIndent);
            pT->GetObjInfo(pInfoOutput);
        }
        
    public:
        HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput)
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
        explicit CFDTEDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFDTEDumper>(pObj, pInfoOutput, nIndent){}
    public:
        //override
        HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFSolutionDumper : public CFDTEInterfaceDumperBase<CFSolutionDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFSolutionDumper);
    public:
        explicit CFSolutionDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFSolutionDumper>(pObj, pInfoOutput, nIndent){}
    public:
        HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFProjectDumper : public CFDTEInterfaceDumperBase<CFProjectDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFProjectDumper);
    public:
        explicit CFProjectDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFProjectDumper>(pObj, pInfoOutput, nIndent){}
    public:
        HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFProjectItemsDumper : public CFDTEInterfaceDumperBase<CFProjectItemsDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFProjectItemsDumper);
    public:
        explicit CFProjectItemsDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFProjectItemsDumper>(pObj, pInfoOutput, nIndent){}
    public:
        HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFProjectItemDumper : public CFDTEInterfaceDumperBase<CFProjectItemDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFProjectItemDumper);
    public:
        explicit CFProjectItemDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFProjectItemDumper>(pObj, pInfoOutput, nIndent){}
    public:
        HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };
   

    //Events
    class CFEventsDumper : public CFDTEInterfaceDumperBase<CFEventsDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFEventsDumper);
    public:
        explicit CFEventsDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFEventsDumper>(pObj, pInfoOutput, nIndent){}
    public:
        HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };


    //Window
    class CFWindowsDumper : public CFDTEInterfaceDumperBase<CFWindowsDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFWindowsDumper);
    public:
        explicit CFWindowsDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFWindowsDumper>(pObj, pInfoOutput, nIndent){}
    public:
        HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFWindowDumper : public CFDTEInterfaceDumperBase<CFWindowDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFWindowDumper);
    public:
        explicit CFWindowDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFWindowDumper>(pObj, pInfoOutput, nIndent){}
    public:
        HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };


    //Document
    class CFDocumentDumper : public CFDTEInterfaceDumperBase<CFDocumentDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFDocumentDumper);
    public:
        explicit CFDocumentDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFDocumentDumper>(pObj, pInfoOutput, nIndent){}
    public:
        HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFTextSelectionDumper : public CFDTEInterfaceDumperBase<CFTextSelectionDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFTextSelectionDumper);
    public:
        explicit CFTextSelectionDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFTextSelectionDumper>(pObj, pInfoOutput, nIndent){}
    public:
        HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    //Code
    class CFFileCodeModelDumper : public CFDTEInterfaceDumperBase<CFFileCodeModelDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFFileCodeModelDumper);
    public:
        explicit CFFileCodeModelDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFFileCodeModelDumper>(pObj, pInfoOutput, nIndent){}
    public:
        HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFCodeModelDumper : public CFDTEInterfaceDumperBase<CFCodeModelDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCodeModelDumper);
    public:
        explicit CFCodeModelDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFCodeModelDumper>(pObj, pInfoOutput, nIndent){}
    public:
        HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFCodeElementsDumper : public CFDTEInterfaceDumperBase<CFCodeElementsDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCodeElementsDumper);
    public:
        explicit CFCodeElementsDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFCodeElementsDumper>(pObj, pInfoOutput, nIndent){}
    public:
        HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };

    class CFCodeElementDumper : public CFDTEInterfaceDumperBase<CFCodeElementDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCodeElementDumper);
    public:
        explicit CFCodeElementDumper(IDispatch* pObj, IDTEInfoOutput* pInfoOutput, int nIndent)
            :CFDTEInterfaceDumperBase<CFCodeElementDumper>(pObj, pInfoOutput, nIndent){}
    public:
        HRESULT GetObjInfo(IDTEInfoOutput* pInfoOutput);
    };


    
    

}//namespace FTL

#endif //FTL_DTE_HELPER_H

#ifndef USE_EXPORT
#  include "ftlDTEHelper.hpp"
#endif