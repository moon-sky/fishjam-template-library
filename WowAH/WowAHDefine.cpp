#include "StdAfx.h"
#include "WowAHDefine.h"
#include <ftlSystem.h>

WowItemInfo::WowItemInfo()
{
	m_nId = 0;
	m_nTypeId = 0;
	m_nDataItem = 0;
	m_ItemSellTimeInfo = istiInvalid;
	m_nLastRefreshTickCount = FTL::CFSystemUtil::GetTickCount64();
	m_nExpirationTickCount = 0;
	m_nQuantity = 0;
	m_nPriceBid = 0;
	m_nPriceBuyout = 0;

	m_strItemName = _T("");
	m_strUrl = _T("");
	m_strSeller = _T("");
}

LPCTSTR WowItemInfo::GetSellTimeInfoString(ItemSellTimeInfo timeInfo)
{
	switch (timeInfo)
	{
		HANDLE_CASE_RETURN_STRING_EX(istiVeryLong, TEXT("非常长"));
		HANDLE_CASE_RETURN_STRING_EX(istiLong, TEXT("长"));
		HANDLE_CASE_RETURN_STRING_EX(istiMiddle, TEXT("中"));
		HANDLE_CASE_RETURN_STRING_EX(istiShort, TEXT("短"));
	default:
		//FTLASSERT(FALSE);
		break;
	}
	return _T("未知");
}

void WowItemInfo::UpdateRefreshTime()
{
	m_nLastRefreshTickCount = FTL::CFSystemUtil::GetTickCount64();
}

void WowItemInfo::SetItemSellTimeInfo(ItemSellTimeInfo timeInfo)
{
	if (m_ItemSellTimeInfo != timeInfo)
	{
		if (istiInvalid != m_ItemSellTimeInfo )
		{
			//发生了日期类型的变更(如 verylong -> long)，分析可能的剩余时间
			//注意：如果一开始就是 istiShort， 则无法估算剩余时间

			//TODO: 计算上次更新的时间，计算出时间差，从而更精确的估计
			ULONGLONG nCurTickCount = FTL::CFSystemUtil::GetTickCount64();

			ULONGLONG nDiffTickCount = nCurTickCount - m_nLastRefreshTickCount;

			switch (timeInfo)
			{
			case istiLong:	//刚刚变到12小时
				m_nExpirationTickCount = nCurTickCount + 12 * 3600 * 1000 - nDiffTickCount;
				break;
			case istiMiddle://刚刚变到2小时
				m_nExpirationTickCount = nCurTickCount + 2 * 3600 * 1000 - nDiffTickCount;
				break;
			case istiShort: //两小时以内
				m_nExpirationTickCount = nCurTickCount + 2 * 3600 * 1000 - nDiffTickCount;
				break;
			default:
				FTLASSERT(FALSE);
				break;
			}
			FTLTRACE(TEXT("nCurTickCount=%lld, nDiffTickCount = %lld, m_nExpirationTickCount=%lld\n"), 
				nCurTickCount, nDiffTickCount, m_nExpirationTickCount);

		}
		else
		{
			//初始设置，简单赋值，不进行其他操作
		}
		m_ItemSellTimeInfo = timeInfo;
	}
	
}

VOID WowItemInfo::Dump()
{
	CString strExpirationTime;
	if (m_nExpirationTickCount > 0)
	{
		ULONGLONG nRemaineTicket = (m_nExpirationTickCount - FTL::CFSystemUtil::GetTickCount64()) / 1000;
		LONG nHour = (LONG)(nRemaineTicket / 3600);
		LONG nMinute = (LONG)((nRemaineTicket % 3600) / 60 );
		LONG nSecond = (LONG)(nRemaineTicket % 60);
		strExpirationTime.Format(TEXT("%d:%d:%d"), 
			nHour, nMinute, nSecond);
	}
	else
	{
		strExpirationTime = TEXT("Unknown");
	}
	FTLTRACE(
		TEXT("Item: Name=%s, strUrl=%s, Seller=%s, ")
		TEXT("id=%d, typeId=%d, SellTimeInfo=%s, Expiration Remain:%s, ")
		TEXT("DataItem=%d, PriceBid=%d, PriceBuyout=%d, Quantity=%d\n"),

		m_strItemName, m_strUrl, m_strSeller, 
		m_nId, m_nTypeId, GetSellTimeInfoString(m_ItemSellTimeInfo), strExpirationTime,
		m_nDataItem, m_nPriceBid, m_nPriceBuyout, m_nQuantity);
}