#pragma once

#include "ItemPageAnalyzeBase.h"

//开始拍卖 -- CreateMyAuction.xht
class CCreateMyAuctionPageAnalyze : public CItemPageAnalyzeBase
{
	enum CreateMyAuctionItemIndex
	{
		//siiTypeUrl = 1,
		//siiName = 2,
		//siiSeller = 5,
		//siiQuantity = 7,
		//siiTimeInfo = 14,

		//siiPriceBidGold = 17,		//竞标金币
		//siiPriceBidSilver = 18,		//竞标银币
		//siiPriceBidCopper = 19,		//竞标铜币

		//siiPriceBuyoutGold = 21,		//一口价金币
		//siiPriceBuyoutSilver = 22,	//一口价银币
		//siiPriceBuyoutCopper = 23,	//一口价铜币

		siiMinCount,
	};
public:
	CCreateMyAuctionPageAnalyze(CWowItemManager* pWowItemManager);
	~CCreateMyAuctionPageAnalyze(void);

	//重载
	virtual HRESULT	ParseItemPage(CComPtr<IHTMLDocument3>& spHtmlDoc, const CString& strSearchItemName);

	//分析每一行的价格
	HRESULT _GetRowItemInfo(const CString& strTableName, CComQIPtr<IHTMLElementCollection>& spChildElements, WowItemInfoPtr& spItemInfo, const CString& strSearchItemName);

private:
	HRESULT _InnerGetInventory_0_RowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
		WowItemInfoPtr& spItemInfo, const CString& strSearchItemName);

};
