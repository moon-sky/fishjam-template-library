#pragma once

#include "WowAHDefine.h"

class CItemPageAnalyzeBase
{
	enum ItemIndex
	{
		iiTypeUrl = 1,
		iiName = 2,
		iiSeller = 5,
		iiQuantity = 7,
		iiTimeInfo = 14,

		iiPriceBidGold = 17,		//竞标金币
		iiPriceBidSilver = 18,		//竞标银币
		iiPriceBidCopper = 19,		//竞标铜币
		
		iiPriceBuyoutGold = 21,		//一口价金币
		iiPriceBuyoutSilver = 22,	//一口价银币
		iiPriceBuyoutCopper = 23,	//一口价铜币

		iiMinCount,
	};
	
	enum ItemValueType
	{
		ivtInnerText,	
		ivtToString,

	};
public:
	CItemPageAnalyzeBase(void);
	~CItemPageAnalyzeBase(void);

	virtual HRESULT	ParseItemPage(CComPtr<IHTMLDocument3>& spHtmlDoc);
	HRESULT _InnerParseTable(CComPtr<IHTMLTable>& spTable);

	//分析每一行的价格
	HRESULT _InnerParsePriceRowItem(long nIndex, CComQIPtr<IHTMLElement>& spPriceElementItem);
	
	ItemInAHInfoPtr	_GetItemInAnInfo(const CString& strIdInfo);
	CString _GetCollectionItemValue(CComQIPtr<IHTMLElementCollection>& spChildElements, INT index, ItemValueType valueType);
	HRESULT _GetPriceRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, ItemInAHInfoPtr& spItemInfo);

	ItemSellTimeInfo _ConvertTimeInfo(const CString& strTimeInfo);
	LONG _GetPriceInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, INT nGoldIndex, INT nSilverIndex, INT nCopperIndex);
protected:
	ItemInAHInfoContainer	m_Items;
};

