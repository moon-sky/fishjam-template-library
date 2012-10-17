#pragma once


// CStorageTreeCtrl

class CStorageTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CStorageTreeCtrl)

public:
	CStorageTreeCtrl();
	virtual ~CStorageTreeCtrl();
    BOOL     OpenStorage(IStorage* pStg);
    BOOL     CloseStorage();
protected:
	DECLARE_MESSAGE_MAP()
private:
    IStorage*   m_pRootStg;
};


