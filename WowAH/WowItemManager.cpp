#include "StdAfx.h"
#include "WowItemManager.h"

CWowItemManager::CWowItemManager(void)
{
}

CWowItemManager::~CWowItemManager(void)
{
	m_allWowItems.clear();
}

WowItemInfoPtr	CWowItemManager::GetItemInAnInfo(const CString& strIdInfo)
{
	WowItemInfoPtr spItemInfo(NULL);
	FTL::CFAutoLock<CFLockObject>	locker(&m_csLock);

	int nPos = strIdInfo.Find(TEXT("-"));
	FTLASSERT(nPos == 7);
	if (nPos > 0)
	{
		CString strId = strIdInfo.Mid(nPos + 1);
		LONG nId = StrToLong(strId);
		FTLASSERT(nId > 0);
		if (nId > 0)
		{
			ItemInAHInfoContainer::iterator iter = m_allWowItems.find(nId);
			if (iter != m_allWowItems.end())
			{
				spItemInfo = WowItemInfoPtr(iter->second);
			}
			else
			{
				spItemInfo = WowItemInfoPtr(new WowItemInfo());
				spItemInfo->SetId(nId);
				m_allWowItems.insert(ItemInAHInfoContainer::value_type(nId, spItemInfo));
			}
		}
	}
	return spItemInfo;
}
