// FileDetailView.cpp : implementation of the CViewFileDetail class 
// 
///////////////////////////////////////////////////////////////////////////// 

#include "stdafx.h" 
#include "resource.h" 

#include <io.h> 

#include "viewfiledetail.h" 
// #include "DragDropSource.h" 
// #include "MainFrm.h" 

struct __declspec(uuid("DE5BF786-477A-11d2-839D-00C04FD918D0")) IDragSourceHelper; 

CViewFileDetail::CViewFileDetail(CUpdateUIBase* pUpdateUI) 
: m_nSortedCol(-1) 
, m_bSortAscending(true) 
{ 
} 

CViewFileDetail::~CViewFileDetail() 
{ 
} 

void CViewFileDetail::SetFileInfo(const std::vector<STATSTG_EX> & vecFileInfo) 
{ 
    this->DeleteAllItems(); 
    _AddFiles(vecFileInfo); 
} 

BOOL CViewFileDetail::PreTranslateMessage(MSG* pMsg) 
{ 
    if ((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) && 
        (pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST)) 
    { 
        return FALSE; 
    } 

    if( WM_COMMAND == pMsg->message ) 
    { 
        if (0 == ::SendMessage(this->m_hWnd, WM_COMMAND, pMsg->wParam, pMsg->lParam)) 
        { 
            return TRUE; 
        } 
    } 
    return FALSE; 
} 

BOOL CViewFileDetail::OnIdle() 
{ 
    return FALSE; 
} 

void CViewFileDetail::OnFinalMessage(HWND ) 
{ 
    // Unregister object for idle updates 
    CMessageLoop* pLoop = _Module.GetMessageLoop(); 
    ATLASSERT(pLoop != NULL); 
    pLoop->RemoveIdleHandler(this); 
} 


LRESULT CViewFileDetail::OnWmDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
{ 
    uMsg, wParam, lParam, bHandled; 
    bHandled = FALSE; 


    if ( m_imlSmall ) m_imlSmall.Destroy(); 

    bHandled = FALSE; 

    return 0; 
} 


/////////////////////////////////////////////////////////////////////////////
// Notify handlers

LRESULT CViewFileDetail::OnLvnColumnClick ( NMHDR* phdr ) 
{
    CWaitCursor w;
    int nCol = ((NMLISTVIEW*) phdr)->iSubItem;

    // If the user clicked the column that is already sorted, reverse the sort
    // direction. Otherwise, go back to ascending order.
    if ( nCol == m_nSortedCol )
        m_bSortAscending = !m_bSortAscending;
    else
        m_bSortAscending = true;

    // Store the column being sorted, and do the sort
    m_nSortedCol = nCol;

    SortItems ( SortCallback, (LPARAM)(DWORD_PTR) this );

    return 0;   // retval ignored
}

LRESULT CViewFileDetail::OnLvnMarqueeBegin ( NMHDR* phdr ) 
{ 
    // Don't allow the marquee if the list is empty. 
    return (GetItemCount() == 0); 
} 

LRESULT CViewFileDetail::OnLvnKeyDown ( NMHDR* phdr ) 
{ 
    NMLVKEYDOWN* pnmlv = (NMLVKEYDOWN*) phdr; 

    // Select all items if the user presses ^A 
    if ( 'A' == pnmlv->wVKey && (GetKeyState(VK_CONTROL) & 0x8000) ) 
        SetItemState ( -1, LVIS_SELECTED, LVIS_SELECTED ); 

    return 0;   // retval ignored 
} 

LRESULT CViewFileDetail::OnLvnDeleteItem ( NMHDR* phdr ) 
{ 
    NMLISTVIEW* pnmlv = (NMLISTVIEW*) phdr; 

    if ( 0 != pnmlv->lParam ) { 
        delete (STATSTG_EX*) pnmlv->lParam; 
    } 

    return 0;   // retval ignored 
} 

// LRESULT CViewFileDetail::OnLvnBeginDrag ( NMHDR* phdr ) 
// { 
//     NMLISTVIEW* pnmlv = (NMLISTVIEW*) phdr; 
//     std::vector<CDraggedFileInfo> vecFileInfo; 
//     CComObjectStack2<CDragDropSource> spDropSrc; 
//     DWORD dwEffect = DROPEFFECT_COPY; 
//     HRESULT hr; 
//     CComPtr<IDragSourceHelper> spDSH; 
// 
//     ATLTRACE(_T("Begin Dragging operation!\n")); 
// 
//     // Get a list of the files being dragged 
//     if ( false == GetDraggedFileInfo ( vecFileInfo ) ) 
//     { 
//         ATLTRACE(_T("Error: Couldn't get list of files dragged (or only partial files were dragged)\n")); 
//         return 0;   // do nothing 
//     } 
// 
//     ATLASSERT(m_pParentPtr); 
//     CMainFrame * pMainFrame = m_pParentPtr->m_pMainFrame; 
//     ATLASSERT(pMainFrame); 
// 
//     // Init the drag/drop data object. 
//     if ( !spDropSrc.Init (pMainFrame, vecFileInfo ) ) 
//     { 
//         ATLTRACE(_T("Error: Couldn't init drop source object\n")); 
//         return 0;   // do nothing 
//     } 
// 
//     // On 2K+, init a drag source helper object that will do the fancy drag 
//     // image when the user drags into Explorer (or another target that supports 
//     // the drag/drop helper). 
//     hr = spDSH.CoCreateInstance ( CLSID_DragDropHelper ); 
// 
//     if ( SUCCEEDED(hr) ) 
//     { 
//         CComQIPtr<IDataObject> spDataObj; 
// 
//         if ( spDataObj = spDropSrc.GetUnknown() ) 
//             spDSH->InitializeFromWindow ( m_hWnd, &pnmlv->ptAction, spDataObj ); 
//     } 
// 
//     // Start the drag/drop! 
//     hr = spDropSrc.DoDragDrop ( DROPEFFECT_COPY|DROPEFFECT_MOVE, &dwEffect ); 
// 
//     if ( FAILED(hr) ) { 
// #ifdef  _DEBUG 
//         TCHAR szTemp[MAX_PATH * 2] = { 0 }; 
//         wnsprintf(szTemp, _countof(szTemp), 
//             _T("Error: DoDragDrop() failed, error: 0x%08X == %s\n"), hr, _E(hr)); 
//         OutputDebugString(szTemp); 
// #endif  // _DEBUG 
//     } 
//     else 
//     { 
//         // if(dwEffect & DROPEFFECT_MOVE) 
//         if(spDropSrc.m_dwLastEffect  & DROPEFFECT_MOVE) 
//         { 
//             IFileClient * pFileClinet = pMainFrame->m_pDllFileClient; 
//             std::vector<CDraggedFileInfo>::const_iterator it; 
//             for (it=vecFileInfo.begin(); it!=vecFileInfo.end(); it++) 
//             { 
//                 pFileClinet->QueryDeleteFile(it->dwUserID, it->nFileId.QuadPart); 
//                 ::WaitForSingleObject(pMainFrame->m_hEventDeleteFile, INFINITE); 
//             } 
// 
//             { 
//                 CUserAccountView & userView = pMainFrame->m_wndUserAccount; 
//                 NMTREEVIEW stNMTV = { 0 }; 
//                 stNMTV.hdr.idFrom = IDC_ORGANIZE; 
//                 stNMTV.hdr.hwndFrom = userView.m_wndOrganize; 
//                 stNMTV.hdr.code = TVN_SELCHANGED; 
//                 stNMTV.action = TVC_UNKNOWN; 
// 
//                 CTreeItem tiSelected = userView.m_wndOrganize.GetSelectedItem(); 
//                 stNMTV.itemNew.hItem = tiSelected; 
// 
//                 userView.SendMessage(WM_NOTIFY, IDC_ORGANIZE, (LPARAM)&stNMTV); 
//             } 
//         } 
//     } 
// 
//     return S_OK; 
// } 

LRESULT CViewFileDetail::OnLvnItemChanged ( NMHDR* phdr ) 
{ 
    phdr; 
    int nCount = GetSelectedCount(); 
    UNREFERENCED_PARAMETER(nCount); 

    return S_OK; 
} 


LRESULT CViewFileDetail::OnLvnItemActivate ( NMHDR* lpnmh ) 
{ 
    USES_CONVERSION; 

    LV_ITEM lvItem; 
    ZeroMemory(&lvItem, sizeof(lvItem)); 
    lvItem.mask = LVIF_PARAM; 

    LPNMLISTVIEW lpnmlv = (LPNMLISTVIEW) lpnmh; 
    lvItem.iItem = lpnmlv->iItem; 

    if (GetItem(&lvItem)) 
    { 
        STATSTG_EX * pFileInfo = (STATSTG_EX *) lvItem.lParam; 
        
        ATLASSERT(pFileInfo); 
        if (pFileInfo) 
        { 
            ::PostMessage(GetParent(), WM_CUSTOM_ACTIVE_ITEM, (WPARAM)pFileInfo->htiHost, 0); 
        } 
    } 
    return S_OK; 
} 


LRESULT CViewFileDetail::OnWmCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
{ 
    uMsg, wParam, lParam, bHandled; 
    bHandled = FALSE; 
    CREATESTRUCT * pCreateStruct = (CREATESTRUCT *) lParam; 
    _Init(); 

    SetViewMode ( LV_VIEW_DETAILS ); 

    // register object for idle updates 
    CMessageLoop* pLoop = _Module.GetMessageLoop(); 
    ATLASSERT(pLoop != NULL); 
    pLoop->AddIdleHandler(this); 

    PostMessage(WM_CUSTOM_POSTCREATE); 

    return 0; 
} 

LRESULT CViewFileDetail::OnWmCustomPostCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) 
{ 
    bHandled = FALSE; 
    return S_OK; 
} 

void CViewFileDetail::_Init() 
{ 
    InitColumns(); 
    InitImageLists(); 
    SetExtendedListViewStyle ( LVS_EX_HEADERDRAGDROP, LVS_EX_HEADERDRAGDROP ); 
} 

void CViewFileDetail::InitColumns() 
{ 
    TCHAR szTmp[MAX_PATH] = { 0 }; 

    struct tagCOLUM_DATA { 
        UINT    uiStrID; 
        int     nWidth; 
    } COLUM_DATA[] = { 
        {IDS_ITEM_NAME,   256}, 
        {IDS_ITEM_TYPE,   128}, 
        {IDS_ITEM_SIZE,   64}, 
        {IDS_ITEM_MTIME,   110}, 
        {IDS_ITEM_CTIME,  110}, 
        {IDS_ITEM_ATIME,   110}, 
        {IDS_ITEM_GRF_MODE,      110}, 
        {IDS_ITEM_GRF_LOCKS,      110}, 
        {IDS_ITEM_CLSID,      250}, 
        {IDS_ITEM_GRF_STATE,      110}, 
        {IDS_ITEM_RESERVED,      110}, 
    } ; 

    for (int i=0; i<_countof(COLUM_DATA); i++) 
    { 
        AtlLoadString(COLUM_DATA[i].uiStrID, szTmp, _countof(szTmp) ); 
        InsertColumn ( i, szTmp, LVCFMT_LEFT, COLUM_DATA[i].nWidth, i ); 
    } 
} 

void CViewFileDetail::InitImageLists() 
{ 
    m_imlSmall.Create(IDB_TYPE, 16, 1, RGB(255, 0, 255)); 

    //SetImageList ( m_imlLarge, LVSIL_NORMAL ); 
    SetImageList ( m_imlSmall, LVSIL_SMALL ); 
} 

void CViewFileDetail::Clear() 
{ 
    DeleteAllItems(); 

    if ( -1 != m_nSortedCol ) 
    { 
        m_nSortedCol = -1; 
        m_bSortAscending = true; 
    } 
} 

void CViewFileDetail::SetViewMode ( int nMode ) 
{ 
    ATLASSERT(nMode >= LV_VIEW_ICON && nMode <= LV_VIEW_TILE); 

    DWORD dwViewStyle; 

    ATLASSERT(LV_VIEW_TILE != nMode); 

    switch ( nMode ) 
    { 
    case LV_VIEW_ICON:      dwViewStyle = LVS_ICON; break; 
    case LV_VIEW_SMALLICON: dwViewStyle = LVS_SMALLICON; break; 
    case LV_VIEW_LIST:      dwViewStyle = LVS_LIST; break; 
    case LV_VIEW_DETAILS:   dwViewStyle = LVS_REPORT; break; 
    default: ATLASSERT(FALSE); break; 
    } 

    ModifyStyle ( LVS_TYPEMASK, dwViewStyle ); 
} 


void CViewFileDetail::_AddFiles ( const std::vector<STATSTG_EX> & vecFiles ) 
{ 
    TCHAR szSwap[MAX_PATH] = { 0 }; 

    std::vector<STATSTG_EX>::const_iterator it; 

    for (it=vecFiles.begin(); it!=vecFiles.end(); it++) 
    { 
        const STATSTG_EX & rFileInfo = *it; 
        _AddFile( rFileInfo ); 
    } 
} 

BOOL FormatFileTimeToString(const FILETIME & mtime, LPTSTR szOut, UINT cchMax) 
{ 
    FILETIME ftLocal = {0}; 
    SYSTEMTIME st = {0}; 
    TCHAR szDate[64] = { 0 }, szTime[64] = { 0 }; 

    FileTimeToLocalFileTime ( &mtime, &ftLocal ); 
    FileTimeToSystemTime ( &ftLocal, &st ); 

    GetDateFormat ( LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, NULL, szDate, _countof(szDate) ); 
    GetTimeFormat ( LOCALE_USER_DEFAULT, TIME_NOSECONDS, &st, NULL, szTime, _countof(szTime) ); 

    wnsprintf(szOut, cchMax, _T("%s %s"), szDate, szTime); 
    
    return TRUE; 
} 


void CViewFileDetail::_AddFile (const STATSTG_EX & rFileProp) 
{ 
    USES_CONVERSION; 
    int nIdx; 
    TCHAR szSize[64] = { 0 }; 
    FILETIME ft = {0}, ftLocal = {0}; 
    SYSTEMTIME st = {0}; 
    TCHAR szFilename[MAX_PATH] = { 0 }; 
    TCHAR szUserName[MAX_PATH] = { 0 }; 

    // 1. name 
    lstrcpyn(szFilename, W2CT(rFileProp.szwName), _countof(szFilename)); 

    // 2. type 
    int nImage = 0; 

    TCHAR szType[MAX_PATH] = { 0 }; 
    switch(rFileProp.type) 
    { 
    case STGTY_STORAGE: 
        lstrcpyn(szType, _T("STGTY_STORAGE"), _countof(szType)); 
        nImage = 0; 
    	break; 
    case STGTY_STREAM:
        lstrcpyn(szType, _T("STGTY_STREAM"), _countof(szType)); 
        nImage = 2; 
    	break;
    case STGTY_LOCKBYTES:
        lstrcpyn(szType, _T("STGTY_LOCKBYTES"), _countof(szType)); 
        nImage = 1; 
        break;
    case STGTY_PROPERTY: 
        lstrcpyn(szType, _T("STGTY_PROPERTY"), _countof(szType)); 
        nImage = 3; 
        break; 
    default: 
        break; 
    } 

    // 3. Get the file size with the size in KB or MB or whatever suffix. 
    StrFormatByteSize64 ( rFileProp.cbSize.QuadPart, szSize, _countof(szSize) ); 

    // 4. mtime 
    TCHAR szDateTime[MAX_PATH] = { 0 }; 
    FormatFileTimeToString(rFileProp.mtime, szDateTime, _countof(szDateTime)); 

    // 5. ctime
    TCHAR ctime[MAX_PATH] = { 0 }; 
    FormatFileTimeToString(rFileProp.ctime, ctime, _countof(ctime)); 

    // 6. atime
    TCHAR atime[MAX_PATH] = { 0 }; 
    FormatFileTimeToString(rFileProp.atime, atime, _countof(atime)); 

    // 7. grfMode 
    TCHAR grfMode[MAX_PATH/10] = { 0 }; 
    wnsprintf(grfMode, _countof(grfMode), _T("0x%08x"), rFileProp.grfMode); 

    // 8. grfLocksSupported 
    TCHAR grfLocksSupported[MAX_PATH/10] = { 0 }; 
    wnsprintf(grfLocksSupported, _countof(grfLocksSupported), _T("0x%08x"), rFileProp.grfLocksSupported); 

    // 9. clsid 
    OLECHAR clsid[MAX_PATH] = { 0 }; 
    StringFromGUID2(rFileProp.clsid, clsid, _countof(clsid)); 

    // 10. grfStateBits 
    TCHAR grfStateBits[MAX_PATH/10] = { 0 }; 
    wnsprintf(grfStateBits, _countof(grfStateBits), _T("0x%08x"), rFileProp.grfStateBits); 

    // 11. reserved 
    TCHAR reserved[MAX_PATH/10] = { 0 }; 
    wnsprintf(reserved, _countof(reserved), _T("0x%08x"), rFileProp.reserved); 

    // Add a new list item for this file. 
    nIdx = InsertItem ( GetItemCount(), szFilename, nImage ); 
    SetItemText ( nIdx, 1, szType ); 
    if (STGTY_STREAM == rFileProp.type) { SetItemText ( nIdx, 2, szSize ); } 
    SetItemText ( nIdx, 3, szDateTime ); 
    SetItemText ( nIdx, 4, ctime ); 
    SetItemText ( nIdx, 5, atime ); 
    SetItemText ( nIdx, 6, grfMode ); 
    SetItemText ( nIdx, 7, grfLocksSupported ); 
    SetItemText ( nIdx, 8, OLE2CT(clsid) ); 
    SetItemText ( nIdx, 9, grfStateBits ); 
    SetItemText ( nIdx, 10, reserved ); 

    STATSTG_EX * pInfo = new STATSTG_EX(rFileProp); 

    SetItemData ( nIdx, (DWORD_PTR) pInfo ); 
} 

// bool CViewFileDetail::GetDraggedFileInfo ( std::vector<CDraggedFileInfo>& vec ) 
// { 
//     vec.clear(); 
// 
//     ATLASSERT(m_pParentPtr); 
//     CMainFrame * pMainFrame = m_pParentPtr->m_pMainFrame; 
//     ATLASSERT(pMainFrame); 
// 
//     int nIdx = -1; 
//     TCHAR szSwap[MAX_PATH] = { 0 }; 
// 
//     while ( (nIdx = GetNextItem ( nIdx, LVNI_SELECTED )) != -1 ) 
//     { 
//         STATSTG_EX* pInfo = (STATSTG_EX*) GetItemData ( nIdx ); 
//         ATLASSERT(NULL != pInfo); 
// 
//         ULARGE_INTEGER tmp; 
//         tmp.QuadPart = pInfo->BackupFileId; 
// 
//         vec.push_back ( CDraggedFileInfo ( CHAR2TCHAR(pInfo->FileName, szSwap, _countof(szSwap)), 
//             nIdx, tmp, pMainFrame->m_szTempDir, pInfo->UserId) ); 
// 
// #ifdef  _DEBUG 
//         char szDebugOut[MAX_PATH*2] = { 0 }; 
//         wsprintfA(szDebugOut, "Adding file \"%s\" to Dragging\n", PathFindFileNameA(pInfo->FileName)); 
//         OutputDebugStringA(szDebugOut); 
// #endif  // _DEBUG 
//     } 
// 
//     return ! vec.empty(); 
// } 


int CALLBACK CViewFileDetail::SortCallback (LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort ) 
{ 
    ATLASSERT(NULL != lParam1); 
    ATLASSERT(NULL != lParam2); 
    ATLASSERT(NULL != lParamSort); 

    STATSTG_EX& info1 = *(STATSTG_EX*) lParam1; 
    STATSTG_EX& info2 = *(STATSTG_EX*) lParam2; 
    CViewFileDetail* pThis = (CViewFileDetail*) lParamSort; 

    return pThis->SortCallback ( info1, info2 ); 
} 

int CViewFileDetail::SortCallback (const STATSTG_EX& info1, 
                                   const STATSTG_EX& info2 ) const 
{ 
    USES_CONVERSION; 
    int nRet = 0; 
    LPCTSTR sz1, sz2; 

    ATLASSERT(-1 != m_nSortedCol); 

    TCHAR szOrg1[MAX_PATH] = { 0 }; 
    lstrcpyn(szOrg1, W2CT(info1.szwName), _countof(szOrg1)); 

    TCHAR szOrg2[MAX_PATH] = { 0 }; 
    lstrcpyn(szOrg2, W2CT(info2.szwName), _countof(szOrg2)); 

    // Default to comparing the file names, unless something in the switch 
    // changes these pointers. 
    sz1 = szOrg1; 
    sz2 = szOrg2; 

    switch ( m_nSortedCol ) 
    { 
    case 0:     // file name 
        sz1 = szOrg1; 
        sz2 = szOrg2; 
        break; 

    case 1:     // type description 
        if ( info1.type < info2.type ) 
            nRet = -1; 
        else if ( info1.type > info2.type ) 
            nRet = 1; 
        break; 

    case 2:     // size 
        if ( info1.cbSize.QuadPart < info2.cbSize.QuadPart ) 
            nRet = -1; 
        else if ( info1.cbSize.QuadPart > info2.cbSize.QuadPart ) 
            nRet = 1; 
        break; 

    case 3:     // modified date/time 
        nRet = (int) CompareFileTime ( &info1.mtime, &info2.mtime ); 
        break; 
        
    case 4:     // 
        nRet = (int) CompareFileTime ( &info1.ctime, &info2.ctime ); 
        break; 

    case 5:     // 
        nRet = (int) CompareFileTime ( &info1.atime, &info2.atime ); 
        break; 

    case 6: 
        if (info1.grfMode > info2.grfMode) 
            nRet = 1; 
        else if(info1.grfMode < info2.grfMode) 
            nRet = -1; 
        else 
            nRet = 0; 
        break; 
    default: 
        ATLASSERT(FALSE); 
    } 

    // If the primary comparison in the switch returned equality (nRet is still 0), 
    // then compare sz1/sz2 so equal items will still be sorted by file names. 
    if ( 0 == nRet ) 
        nRet = lstrcmpi ( sz1, sz2 ); 

    return m_bSortAscending ? nRet : -nRet; 
} 
