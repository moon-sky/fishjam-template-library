#pragma once

#include "WowAHDefine.h"
#include <ftlThread.h>

class CWowItemManager
{
public:
	//id -> WowItemInfoPtr
	typedef std::map<LONG, WowItemInfoPtr>		ItemInAHInfoContainer;

	CWowItemManager(void);
	~CWowItemManager(void);

	WowItemInfoPtr	GetItemInAnInfo(const CString& strIdInfo);
private:
	ItemInAHInfoContainer		m_allWowItems;
	FTL::CFCriticalSection		m_csLock;
};
