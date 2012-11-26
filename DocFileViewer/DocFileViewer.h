// DocFileViewer.h

#pragma once 

// 
// used by CChildFrame::OpenCompositeDocument function only 
// 
struct StgInfo { 
    IStorage *      pStg; 
    IEnumSTATSTG *  pEnum; 
    HTREEITEM       hParent; 
    
    StgInfo( IStorage* p1, IEnumSTATSTG* p2, HTREEITEM p3 ) 
        : pStg(p1) , pEnum(p2) , hParent(p3) 
    { 
    } 
}; 


// 
// used by class CChildFrame 
// 
struct TREE_ITEM_DATA 
{ 
    CComPtr<IUnknown> spStgOrStrm; 
    DWORD dwStgType; 
    
    TREE_ITEM_DATA(IUnknown *pUnkn=NULL, DWORD dwType=0) 
        : spStgOrStrm(pUnkn) , dwStgType(dwType) 
    { } 
} ; 
 

__declspec(selectany) STATSTG statstgDummy = { 0 }; 

// 
// used by class CViewFileDetail 
// 
struct STATSTG_EX : public STATSTG  
{ 
    wchar_t szwName[MAX_PATH]; 
    HTREEITEM htiHost; 

    STATSTG_EX(const STATSTG & stat = statstgDummy) 
    { 
        *this = stat; 
    } 

    STATSTG_EX & operator = (const STATSTG & rhs) 
    { 
        ZeroMemory(this, sizeof(STATSTG_EX)); 

        memcpy(this, &rhs, sizeof(STATSTG)); 
        
        if (rhs.pwcsName) { 
            lstrcpynW(szwName, rhs.pwcsName, _countof(szwName)); 
        } 

        htiHost = NULL; 

        return *this; 
    } 
}; 
