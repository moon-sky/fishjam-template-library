#include "StdAfx.h"
#include "WowAHDefine.h"

ItemInAHInfo::ItemInAHInfo()
{
	m_nId = 0;
	m_nTypeId = 0;
	m_nDataItem = 0;
	m_ItemSellTimeInfo = istiVeryLong;
	m_nLastCheckTime = 0;
	m_nRemainTime = 0;
	m_nQuantity = 0;
	m_nPriceBid = 0;
	m_nPriceBuyout = 0;

	m_strItemName = _T("");
	m_strUrl = _T("");
	m_strSeller = _T("");
}

LPCTSTR ItemInAHInfo::GetSellTimeInfoString(ItemSellTimeInfo timeInfo)
{
	switch (timeInfo)
	{
		HANDLE_CASE_RETURN_STRING_EX(istiVeryLong, TEXT("VeryLong"));
		HANDLE_CASE_RETURN_STRING_EX(istiLong, TEXT("Long"));
		HANDLE_CASE_RETURN_STRING_EX(istiShort, TEXT("Short"));
	default:
		FTLASSERT(FALSE);
	}
	return _T("");
}

VOID ItemInAHInfo::Dump()
{
	FTLTRACE(
		TEXT("Item: Name=%s, strUrl=%s, Seller=%s, ")
		TEXT("id=%d, typeId=%d, SellTimeInfo=%s, ")
		TEXT("DataItem=%d, PriceBid=%d, PriceBuyout=%d, Quantity=%d\n"),

		m_strItemName, m_strUrl, m_strSeller, 
		m_nId, m_nTypeId, GetSellTimeInfoString(m_ItemSellTimeInfo), 
		m_nDataItem, m_nPriceBid, m_nPriceBuyout, m_nQuantity);
}