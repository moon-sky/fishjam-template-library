#ifndef __VIEW_FILE_DETAIL_H__ 
#define __VIEW_FILE_DETAIL_H__ 1 

// FileDetailView.h : interface of the CViewFileDetail class 
// 
///////////////////////////////////////////////////////////////////////////// 

#pragma once 


#define FILE_VIEW_STYLES     (LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS | LVS_AUTOARRANGE ) 
#define FILE_VIEW_EX_STYLES  (/*WS_EX_CLIENTEDGE|*/LVS_EX_INFOTIP) 

typedef CWinTraitsOR<FILE_VIEW_STYLES, FILE_VIEW_EX_STYLES, CControlWinTraits> CFileDetailViewWinTraits; 

class CViewFileDetail 
    : public CWindowImpl<CViewFileDetail, CListViewCtrl, CFileDetailViewWinTraits > 
    , public CMessageFilter 
    , public CIdleHandler 
{ 
    typedef CWindowImpl<CViewFileDetail, CListViewCtrl, CFileDetailViewWinTraits> baseClass; 
public: 
    DECLARE_WND_SUPERCLASS(_T("CViewFileDetail"), CListViewCtrl::GetWndClassName()) 

    CViewFileDetail(CUpdateUIBase* pUpdateUI = NULL); 
    ~CViewFileDetail(); 
    virtual BOOL PreTranslateMessage(MSG* pMsg); 
    virtual BOOL OnIdle(); 
    virtual void OnFinalMessage(HWND);

    BEGIN_MSG_MAP(CViewFileDetail) 
        MESSAGE_HANDLER(WM_CREATE, OnWmCreate) 
        MESSAGE_HANDLER(WM_CUSTOM_POSTCREATE, OnWmCustomPostCreate) 
        MESSAGE_HANDLER(WM_DESTROY, OnWmDestroy ) 

        REFLECTED_NOTIFY_CODE_HANDLER_EX(LVN_COLUMNCLICK, OnLvnColumnClick) 
        REFLECTED_NOTIFY_CODE_HANDLER_EX(LVN_MARQUEEBEGIN, OnLvnMarqueeBegin) 
        REFLECTED_NOTIFY_CODE_HANDLER_EX(LVN_KEYDOWN, OnLvnKeyDown) 
        REFLECTED_NOTIFY_CODE_HANDLER_EX(LVN_DELETEITEM, OnLvnDeleteItem) 
//         REFLECTED_NOTIFY_CODE_HANDLER_EX(LVN_BEGINDRAG, OnLvnBeginDrag) 
        REFLECTED_NOTIFY_CODE_HANDLER_EX(LVN_ITEMCHANGED, OnLvnItemChanged) 
        REFLECTED_NOTIFY_CODE_HANDLER_EX(LVN_ITEMACTIVATE, OnLvnItemActivate) 

        DEFAULT_REFLECTION_HANDLER() 
    END_MSG_MAP() 

    LRESULT OnWmCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
    LRESULT OnWmCustomPostCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
    LRESULT OnWmDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 

//    // Notify handlers
    LRESULT OnLvnColumnClick ( NMHDR* phdr ); 
    LRESULT OnLvnMarqueeBegin ( NMHDR* phdr ); 
    LRESULT OnLvnKeyDown ( NMHDR* phdr ); 
    LRESULT OnLvnDeleteItem ( NMHDR* phdr ); 
//     LRESULT OnLvnBeginDrag ( NMHDR* phdr ); 
    LRESULT OnLvnItemChanged ( NMHDR* phdr ); 
    LRESULT OnLvnItemActivate ( NMHDR* phdr ); 

    void _Init(); 
    void InitColumns(); 
    void InitImageLists(); 

    void Clear(); 
    void SetViewMode ( int nMode ); 

    CImageList m_imlSmall, m_imlLarge, m_imlTiles; 
    
    void SetFileInfo(const std::vector<STATSTG_EX> & vecFileInfo); 

    void _AddFiles(const std::vector<STATSTG_EX> & vecFiles); 
    void _AddFile(const STATSTG_EX & rFileProp); 

//     bool GetDraggedFileInfo ( std::vector<CDraggedFileInfo>& vec ); 

// 
     int m_nSortedCol;   // -1 if list hasn't been sorted yet 
     bool m_bSortAscending; 
// 
    static int CALLBACK SortCallback ( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort ); 
    int SortCallback ( const STATSTG_EX& info1, const STATSTG_EX& info2 ) const; 
// 
 private: 

public: 
}; 

#endif  // __VIEW_FILE_DETAIL_H__ 
