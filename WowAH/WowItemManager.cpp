#include "StdAfx.h"
#include "WowItemManager.h"

CWowItemManager::CWowItemManager(void)
{
}

CWowItemManager::~CWowItemManager(void)
{
	m_allWowItems.clear();
}

VOID CWowItemManager::ClearItems(ItemStatus itemStatus)
{
	//UNREFERENCED_PARAMETER(itemStatus);
	CFAutoLock<CFLockObject>	locker(&m_csLock);
	m_allWowItems.clear();
}

WowItemInfoPtr	CWowItemManager::GetItemInAHInfo(const CString& strAuctionIdInfo)
{
	FTL::CFAutoLock<CFLockObject>	locker(&m_csLock);
	WowItemInfoPtr spItemInfo(NULL);

	int nPos = strAuctionIdInfo.Find(TEXT("-"));
	FTLASSERT(nPos == 7);
	if (nPos > 0)
	{
		CString strAuctionId = strAuctionIdInfo.Mid(nPos + 1);
		LONG nAuctionId = StrToLong(strAuctionId);
		FTLASSERT(nAuctionId > 0);
		if (nAuctionId > 0)
		{
			ItemInAHInfoContainer::iterator iter = m_allWowItems.find(nAuctionId);
			if (iter != m_allWowItems.end())
			{
				spItemInfo = WowItemInfoPtr(iter->second);
			}
			else
			{
				spItemInfo = WowItemInfoPtr(new WowItemInfo());
				spItemInfo->SetAuctionId(nAuctionId);
				m_allWowItems.insert(ItemInAHInfoContainer::value_type(nAuctionId, spItemInfo));
			}
		}
	}
	return spItemInfo;
}

VOID CWowItemManager::RemoveItemInfo(LONG nAuctionId)
{
	FTL::CFAutoLock<CFLockObject>	locker(&m_csLock);
	ItemInAHInfoContainer::iterator iter = m_allWowItems.find(nAuctionId);
	if (iter != m_allWowItems.end())
	{
		m_allWowItems.erase(iter);
	}
}

VOID CWowItemManager::DumpAllItemInfo()
{
	LONG nIndex = 1;
	FTL::CFAutoLock<CFLockObject>	locker(&m_csLock);
	LONG nCount = m_allWowItems.size();

	for (ItemInAHInfoContainer::iterator iter = m_allWowItems.begin();
		iter != m_allWowItems.end(); 
		++iter)
	{
		const WowItemInfoPtr& pWowItemInfo = iter->second;
		FTLASSERT(pWowItemInfo->IsValidItem());
		pWowItemInfo->Dump(nIndex++, nCount);
	}
}