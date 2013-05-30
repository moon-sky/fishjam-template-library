#include "StdAfx.h"
#include "WowAHDefine.h"
#include <ftlSystem.h>

LPCTSTR WowItemInfo::GetItemStatusString(ItemStatus itemStatus)
{
	switch (itemStatus)
	{
		HANDLE_CASE_RETURN_STRING_EX(isInvalid, TEXT("Invalid"));

		HANDLE_CASE_RETURN_STRING_EX(isOtherSelling, TEXT("OtherSelling"));

		HANDLE_CASE_RETURN_STRING_EX(isMyBidsActive, TEXT("MyBidsActive"));
		HANDLE_CASE_RETURN_STRING_EX(isMyBidsWon, TEXT("MyBidsWon"));
		HANDLE_CASE_RETURN_STRING_EX(isMyBidsLost, TEXT("MyBidsLost"));

		HANDLE_CASE_RETURN_STRING_EX(isMyAuctionActive, TEXT("MyAuctionActive"));
		HANDLE_CASE_RETURN_STRING_EX(isMyAuctionSold, TEXT("MyAuctionSold"));
		HANDLE_CASE_RETURN_STRING_EX(isMyAuctionEnded, TEXT("MyAuctionEnded"));
	default:
		FTLASSERT(FALSE);
		break;
	}
	return TEXT("Unknown");
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

WowItemInfo::WowItemInfo()
{
	//m_ItemSellerType = istInvalid;
	m_itemStatus = isInvalid;

	m_nAuctionId = INVALID_LONG;
	m_nTypeId = INVALID_LONG;
	m_nDataItem = INVALID_LONG;
	m_ItemSellTimeInfo = istiInvalid;

	m_nLastRefreshTickCount = FTL::CFSystemUtil::GetTickCount64();
	m_nExpirationTickCount = 0;

	m_nQuantity = INVALID_LONG;
	m_nPriceBid = INVALID_LONG;
	m_nPriceBuyout = INVALID_LONG;
	m_nAuctionSoldPrice = INVALID_LONG;
	m_nRemainDayInMail = INVALID_LONG;
	m_nMailId = INVALID_LONG;

	m_strItemName = _T("");
	m_strUrl = _T("");
	m_strSeller = _T("");
	m_strPurchaser = _T("");
}


BOOL WowItemInfo::IsValidItem()
{
	BOOL bRet = FALSE;
	bRet = (
		(m_itemStatus != isInvalid)
		&& (!m_strItemName.IsEmpty()) 
		&& (m_nTypeId != INVALID_LONG)
		);
	FTLASSERT(bRet);
	if (bRet)
	{
	}
	return bRet;
}

void WowItemInfo::SetUrl(const CString& strUrl)
{
	FTLASSERT(m_strUrl.IsEmpty() || m_strUrl == strUrl);
	if (m_strUrl.IsEmpty())
	{
		m_strUrl = strUrl;
	}
}

void WowItemInfo::SetItemName(const CString& strItemName)
{
	FTLASSERT(m_strItemName.IsEmpty() || m_strItemName == strItemName);
	if (m_strItemName.IsEmpty())
	{
		m_strItemName = strItemName;
	}
}

//void WowItemInfo::UpdateRefreshTime()
//{
//	m_nLastRefreshTickCount = FTL::CFSystemUtil::GetTickCount64();
//}

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
			case istiShort: //少于30分钟
				m_nExpirationTickCount = nCurTickCount + 30 * 60 * 1000 - nDiffTickCount;
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

	//m_nLastRefreshTickCount = FTL::CFSystemUtil::GetTickCount64();
}

VOID WowItemInfo::Dump(LONG nIndex, LONG nCount)
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
		TEXT("Item[%d-%d]: Status=%s, Name=%s, strUrl=%s, Seller=%s, Purchaser=%s, ")
		TEXT("AuctionId=%d, typeId=%d, SellTimeInfo=%s, Expiration Remain:%s, ")
		TEXT("DataItem=%d, LastRefreshTickCount=%lld, PriceBid=%d, PriceBuyout=%d, nAuctionSoldPrice=%d, Quantity=%d, ")
		TEXT("RemainDayInMail=%d, MailId=%d\n"),

		nIndex, nCount, GetItemStatusString(m_itemStatus), 
		m_strItemName, m_strUrl, m_strSeller, m_strPurchaser,
		m_nAuctionId, m_nTypeId, GetSellTimeInfoString(m_ItemSellTimeInfo), strExpirationTime,
		m_nDataItem, m_nLastRefreshTickCount, m_nPriceBid, m_nPriceBuyout, m_nAuctionSoldPrice, m_nQuantity,
		m_nRemainDayInMail, m_nMailId);
}
