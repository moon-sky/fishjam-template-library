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

	WowItemInfoPtr	GetItemInAHInfo(const CString& strAuctionIdInfo);

	//目前的实现不管类型 -- 以后可能会扩展成只清除特定类型
	VOID			ClearItems(ItemStatus	itemStatus);

	//拍卖结束或失败?
	VOID			RemoveItemInfo(LONG nAuctionId);
	VOID			DumpAllItemInfo();
private:
	ItemInAHInfoContainer		m_allWowItems;
	FTL::CFCriticalSection		m_csLock;
};
