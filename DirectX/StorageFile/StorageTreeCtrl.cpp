// StorageTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "StorageFile.h"
#include "StorageTreeCtrl.h"


// CStorageTreeCtrl

IMPLEMENT_DYNAMIC(CStorageTreeCtrl, CTreeCtrl)
CStorageTreeCtrl::CStorageTreeCtrl()
{
    m_pRootStg = NULL;
}

CStorageTreeCtrl::~CStorageTreeCtrl()
{
    CloseStorage();
}


BEGIN_MESSAGE_MAP(CStorageTreeCtrl, CTreeCtrl)
END_MESSAGE_MAP()


BOOL CStorageTreeCtrl::OpenStorage(IStorage* pStg)
{
    BOOL bRet = FALSE;
    HRESULT hr = E_FAIL;

    ASSERT(NULL == m_pRootStg);
    if (m_pRootStg)
    {
        API_VERIFY(CloseStorage());
    }

    m_pRootStg = pStg;
    if (m_pRootStg)
    {
        CComPtr<IEnumSTATSTG> spEstg = NULL;
        COM_VERIFY(m_pRootStg->EnumElements(0, NULL, 0, &spEstg));
        
        STATSTG stgstruct;
        ULONG fetched;
        USES_CONVERSION;

        while (spEstg->Next(1, &stgstruct, &fetched) == S_OK)
        {
            HTREEITEM newItem = InsertItem(OLE2T(stgstruct.pwcsName),0,0);
            //if (stgstruct.type == STGTY_STORAGE)
            //    title += " <storage>";
            //if (stgstruct.type == STGTY_STREAM)
            //    title += " <stream>";
        }
    }
    return bRet;
}

BOOL CStorageTreeCtrl::CloseStorage()
{
    BOOL bRet = FALSE;
    SAFE_RELEASE(m_pRootStg);
    return bRet;
}

// CStorageTreeCtrl message handlers

