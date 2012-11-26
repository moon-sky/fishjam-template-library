#ifndef __ATL_CUSTOM_H__ 
#define __ATL_CUSTOM_H__ 1 

#ifndef __ATLAPP_H__ 
#error "atlcustom.h" requires "atlapp.h" to be included first 
#endif 

#ifndef __ATLDLGS_H__ 
#error "atlcustom.h" requires "atldlgs.h" to be included first 
#endif 

#ifndef __ATLCOM_H__ 
#error "atlcustom.h" requires "atlcom.h" to be included first 
#endif 

#ifndef __ATLCTRLS_H__ 
#error "atlcustom.h" requires "atlctrls.h" to be included first 
#endif 

//#ifndef _WTL_USE_CSTRING 
// #error "atlcustom.h" requires "atlmisc.h" to be included first 
//#endif 

#ifndef _countof 
#define _countof( x ) (sizeof(x)/sizeof((x)[0])) 
#endif  // _countof 

#ifndef ARR_SIZE 
#define ARR_SIZE    _countof 
#endif // !defined(ARR_SIZE) 

#if _ATL_VER < 0x0700 
#undef BEGIN_MSG_MAP 
#define BEGIN_MSG_MAP(x) BEGIN_MSG_MAP_EX(x) 
#endif 


#define WTL_WND_SAMPLE_IMPL( wnd_type )     \
class wnd_type##Wnd : public CWindowImpl< wnd_type##Wnd, wnd_type>  \
{   \
public: \
    DECLARE_WND_SUPERCLASS( _T(#wnd_type) _T("Wnd"), GetWndClassName() )    \
    BEGIN_MSG_MAP( wnd_type##Wnd )  \
    END_MSG_MAP()   \
}; 


WTL_WND_SAMPLE_IMPL(CButton); 
WTL_WND_SAMPLE_IMPL(CListBox); 
WTL_WND_SAMPLE_IMPL(CEdit); 
WTL_WND_SAMPLE_IMPL(CListViewCtrl); 
WTL_WND_SAMPLE_IMPL(CTreeViewCtrl); 
WTL_WND_SAMPLE_IMPL(CTreeViewCtrlEx); 



#define TVS_COMBINE     TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT //|TVS_CHECKBOXES|WS_TABSTOP 

typedef CWinTraitsOR<TVS_COMBINE, 0, CControlWinTraits> CTreeViewWinTraits; 


class CTreeViewCtrlExWindow 
    : public CWindowImpl< CTreeViewCtrlExWindow, CTreeViewCtrlEx, CTreeViewWinTraits> 
{ 
public: 
    DECLARE_WND_SUPERCLASS( _T("TreeViewCtrlExWindow"), CTreeViewCtrlEx::GetWndClassName() ) 
    BEGIN_MSG_MAP( CTreeViewCtrlExWindow ) 
        DEFAULT_REFLECTION_HANDLER() 
    END_MSG_MAP() 
}; 




// Version of CComObjectStack that doesn't freak out and 
// assert when IUnknown methods are called. 
template <class Base> 
class CComObjectStack2 : public CComObjectStack<Base> 
{ 
public: 
    CComObjectStack2() : CComObjectStack<Base>() { } 

    STDMETHOD_(ULONG, AddRef)() { return 1; } 
    STDMETHOD_(ULONG, Release)() { return 1; } 

    STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject) 
    { return _InternalQueryInterface(iid, ppvObject); } 
}; 


class CModifiedFileDialog : public CFileDialogImpl < CModifiedFileDialog > 
{ 
public: 
    // Construction 
    CModifiedFileDialog ( BOOL bOpenFileDialog, 
        _U_STRINGorID szDefExt = 0U, 
        _U_STRINGorID szFileName = 0U, 
        DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING, 
        _U_STRINGorID szFilter = 0U, 
        HWND hwndParent = NULL ) 
        : CFileDialogImpl<CModifiedFileDialog>(bOpenFileDialog, NULL, NULL, 0, NULL, hwndParent) 
    { 
        ZeroMemory(m_sDefExt, sizeof(m_sDefExt)); 
        ZeroMemory(m_sFileName, sizeof(m_sFileName)); 
        ZeroMemory(m_sFilter, sizeof(m_sFilter)); 

        if (szDefExt.m_lpstr) { 
            if(IS_INTRESOURCE( (UINT) szDefExt.m_lpstr )) 
            { 
                AtlLoadString( (UINT) szDefExt.m_lpstr, m_sDefExt, _countof(m_sDefExt)); 
            } 
            else 
            { 
                lstrcpyn(m_sDefExt, szDefExt.m_lpstr, _countof(m_sDefExt)); 
            } 
        } 

        if (szFileName.m_lpstr) { 
            if(IS_INTRESOURCE( (UINT) szFileName.m_lpstr )) 
            { 
                AtlLoadString( (UINT) szFileName.m_lpstr, m_sFileName, _countof(m_sFileName)); 
            } 
            else 
            { 
                lstrcpyn(m_sFileName, szFileName.m_lpstr, _countof(m_sFileName)); 
            } 
        } 

        if (szFilter.m_lpstr) { 
            if(IS_INTRESOURCE( (UINT) szFilter.m_lpstr )) 
            { 
                AtlLoadString( (UINT) szFilter.m_lpstr, m_sFilter, _countof(m_sFilter)); 
            } 
            else 
            { 
                lstrcpyn(m_sFilter, szFilter.m_lpstr, _countof(m_sFilter)); 
            } 
        } 

        m_ofn.lpstrDefExt = m_sDefExt; 
        m_ofn.Flags |= dwFlags; 
        m_ofn.lpstrFilter = PrepFilterString ( m_sFilter ); 

        // setup initial file name 
        if ( lstrlen(m_sFileName) ) 
            lstrcpyn ( m_szFileName, m_sFileName, _MAX_PATH ); 
    } 

    // Maps
    BEGIN_MSG_MAP( CModifiedFileDialog ) 
        CHAIN_MSG_MAP( CFileDialogImpl< CModifiedFileDialog > ) 
    END_MSG_MAP() 

    // Overrides
    void OnInitDone ( LPOFNOTIFY lpon ) 
    { 
        GetFileDialogWindow().CenterWindow ( lpon->lpOFN->hwndOwner ); 
    } 

protected: 
    TCHAR m_sDefExt[MAX_PATH*2], m_sFileName[MAX_PATH], m_sFilter[MAX_PATH*4]; 

    LPCTSTR PrepFilterString ( LPTSTR sFilter ) 
    { 
        LPTSTR psz = sFilter; 
        while ( '\0' != *psz ) 
        { 
            if ( '|' == *psz ) 
                *psz++ = '\0'; 
            else 
                psz = ::CharNext ( psz ); 
        } 
        return sFilter; 
    } 
}; 


////////////////////////////////////////////////////////////////////// 
// CModifiedFolderDialog 

class CModifiedFolderDialog : public CFolderDialogImpl<CModifiedFolderDialog> 
{ 
public: 
    // Construction 
    CModifiedFolderDialog ( HWND hWndParent = NULL, _U_STRINGorID szTitle = 0U, UINT uFlags = BIF_RETURNONLYFSDIRS ) 
        : CFolderDialogImpl<CModifiedFolderDialog>(hWndParent, NULL, uFlags) 
    { 
        if (szTitle.m_lpstr) { 
            if(IS_INTRESOURCE( (UINT) szTitle.m_lpstr )) 
            { 
                AtlLoadString( (UINT) szTitle.m_lpstr, m_szTitle, _countof(m_szTitle)); 
            } 
            else 
            { 
                lstrcpyn(m_szTitle, szTitle.m_lpstr, _countof(m_szTitle)); 
            } 
        } 

        m_bi.lpszTitle = m_szTitle; 
    } 

    virtual ~CModifiedFolderDialog() 
    { 
    }

    // Overrides 
    void OnInitialized() 
    { 
        if(lstrlen(m_szInitialDir)) 
        { 
            SetSelection ( m_szInitialDir ); 
        } 
    } 

    void SetInitialDir(LPCTSTR lpszInitDir) 
    { 
        if(lpszInitDir) { 
            lstrcpyn(m_szInitialDir, lpszInitDir, _countof(m_szInitialDir)); 
        } 
    } 

protected: 
    TCHAR m_szTitle[MAX_PATH]; 
    TCHAR m_szInitialDir[MAX_PATH]; 
}; 


////////////////// 
// Function to get language code from LANGID 
// 
inline BOOL GetLanguageCode(LANGID langid, TCHAR szLangCode[], UINT cchMax) 
{ 
    ZeroMemory(szLangCode, cchMax*sizeof(TCHAR)); 
    int nPrimaryLang = PRIMARYLANGID(langid); 
    int nSubLang = SUBLANGID(langid); 
    // locale ID 
    LCID lcid = MAKELCID(MAKELANGID(nPrimaryLang, nSubLang), SORT_DEFAULT); 
    ::GetLocaleInfo(lcid, LOCALE_SABBREVLANGNAME, szLangCode, cchMax); 
    return _tcslen(szLangCode) ? TRUE : FALSE; 
} 


#endif  // __ATL_CUSTOM_H__ 
