#include "StdAfx.h"
#include "MyAuctionItemPageAnalyze.h"
#include "UserInfoManager.h"
#include <ftlWebInterfaceDetect.h>

#define TABLE_NAME_AUCTIONS_ACTIVE	_T("auctions-active")
#define TALBE_NAME_AUCTIONS_SOLD	_T("auctions-sold")
#define TABLE_NAME_AUCTIONS_ENDED	_T("auctions-ended")


CMyAuctionItemPageAnalyze::CMyAuctionItemPageAnalyze(CWowItemManager* pWowItemManager)
	:CItemPageAnalyzeBase(pWowItemManager)
{
}

CMyAuctionItemPageAnalyze::~CMyAuctionItemPageAnalyze(void)
{
}

HRESULT	CMyAuctionItemPageAnalyze::ParseItemPage(CComPtr<IHTMLDocument3>& spHtmlDoc, 
											const CString& strSearchItemName)
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(spHtmlDoc, E_POINTER);
	HRESULT hr = E_FAIL;

	LPCTSTR pszTableName[] = {
		TABLE_NAME_AUCTIONS_ACTIVE,
		TALBE_NAME_AUCTIONS_SOLD,
		TABLE_NAME_AUCTIONS_ENDED,
	};

	COM_VERIFY(_INnerParseAllTables(spHtmlDoc, pszTableName, _countof(pszTableName), strSearchItemName));

	return hr;
}

namespace AuctionsActive
{
	enum SoldItemIndex
	{
		siiTypeUrl = 2,
		siiName = 2,
		siiSeller = 5,
		siiQuantity = 8,
		siiTimeInfo = 9,
		
		siiCurrentBid = 11,		//当前竞价

		siiPriceBidGold = 15,		//竞标金币
		siiPriceBidSilver = 16,		//竞标银币
		siiPriceBidCopper = 17,		//竞标铜币

		siiPriceBuyoutGold = 19,		//一口价金币
		siiPriceBuyoutSilver = 20,	//一口价银币
		siiPriceBuyoutCopper = 21,	//一口价铜币

		siiMinCount,
	};
};

HRESULT CMyAuctionItemPageAnalyze::_InnerGetAuctionsActiveRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
															WowItemInfoPtr& spItemInfo, 
															const CString& strSearchItemName)
{
	//CFHTMLElementCollectionDumper dumper(spChildElements, CFOutputWindowInfoOutput::Instance(), 0);

	HRESULT hr = E_FAIL;
	long nLength = 0;
	COM_VERIFY(spChildElements->get_length(&nLength));
	FTLASSERT(nLength >= AuctionsActive::siiMinCount);
	if (nLength >= AuctionsActive::siiMinCount)
	{
		spItemInfo->SetItemStatus(isMyAuctionActive);

		CString strItemName = _GetCollectionItemValue(spChildElements, AuctionsActive::siiName, ivtInnerText);
		if (strSearchItemName.IsEmpty() || strSearchItemName == strItemName)
		{
			CString strUrl = _GetCollectionItemValue(spChildElements, AuctionsActive::siiTypeUrl, ivtToString);
			spItemInfo->SetUrl(strUrl);
			int nPosSlash = strUrl.ReverseFind(_T('/'));
			if (nPosSlash > 0)
			{
				CString strTypeId = strUrl.Mid(nPosSlash + 1);
				spItemInfo->SetTypeId(StrToLong(strTypeId));
			}

			spItemInfo->SetItemName(strItemName);
			spItemInfo->SetSeller(_GetCollectionItemValue(spChildElements, AuctionsActive::siiSeller, ivtInnerText));
			spItemInfo->SetQuantity(StrToLong(_GetCollectionItemValue(spChildElements, AuctionsActive::siiQuantity, ivtInnerText)));

			CString strTimeInfo = _GetCollectionItemValue(spChildElements, AuctionsActive::siiTimeInfo, ivtInnerText, TRUE);
			spItemInfo->SetItemSellTimeInfo(_ConvertTimeInfo(strTimeInfo));
			CString strCurBid = _GetCollectionItemValue(spChildElements, AuctionsActive::siiCurrentBid, ivtInnerText, TRUE);

			spItemInfo->SetPriceBid(_GetPriceInfo(spChildElements, AuctionsActive::siiPriceBidGold, 
				AuctionsActive::siiPriceBidSilver, AuctionsActive::siiPriceBidCopper));
			spItemInfo->SetPriceBuyout(_GetPriceInfo(spChildElements, AuctionsActive::siiPriceBuyoutGold, 
				AuctionsActive::siiPriceBuyoutSilver, AuctionsActive::siiPriceBuyoutCopper));

			//最后才更新时间 -- 保证 SetItemSellTimeInfo 时能获取到上次更新的时间信息
			//spItemInfo->UpdateRefreshTime();
		}
	}

	return hr;
}


namespace AuctionsSold
{
	enum SoldItemIndex
	{
		siiTypeUrl = 1,
		siiName = 1,
		siiQuantity = 3,
		siiPurchaser = 6,
		//siiSeller = 5,
		//siiTimeInfo = 9,

		//siiCurrentBid = 11,		//当前竞价

		siiAuctionSoldPriceGold = 8,		//售出金币
		siiAuctionSoldPriceSilver = 9,		//售出银币
		siiAuctionSoldPriceCopper = 10,		//售出铜币

		//siiPriceBuyoutGold = 19,		//一口价金币
		//siiPriceBuyoutSilver = 20,	//一口价银币
		//siiPriceBuyoutCopper = 21,	//一口价铜币

		siiMinCount,
	};
};

HRESULT CMyAuctionItemPageAnalyze::_InnerGetAuctionsSoldRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
																	  WowItemInfoPtr& spItemInfo, 
																	  const CString& strSearchItemName)
{
	//CFHTMLElementCollectionDumper dumper(spChildElements, CFOutputWindowInfoOutput::Instance(), 0);

	HRESULT hr = E_FAIL;
	long nLength = 0;
	COM_VERIFY(spChildElements->get_length(&nLength));
	FTLASSERT(nLength >= AuctionsSold::siiMinCount);
	if (nLength >= AuctionsSold::siiMinCount)
	{
		spItemInfo->SetItemStatus(isMyAuctionSold);
		spItemInfo->SetSeller(CUserInfoManager::Instance()->GetUserName());

		CString strItemName = _GetCollectionItemValue(spChildElements, AuctionsSold::siiName, ivtInnerText, TRUE);
		if (strSearchItemName.IsEmpty() || strSearchItemName == strItemName)
		{
			CString strUrl = _GetCollectionItemValue(spChildElements, AuctionsSold::siiTypeUrl, ivtToString);
			spItemInfo->SetUrl(strUrl);
			int nPosSlash = strUrl.ReverseFind(_T('/'));
			if (nPosSlash > 0)
			{
				CString strTypeId = strUrl.Mid(nPosSlash + 1);
				spItemInfo->SetTypeId(StrToLong(strTypeId));
			}
			spItemInfo->SetItemName(strItemName);

			spItemInfo->SetQuantity(StrToLong(_GetCollectionItemValue(spChildElements, AuctionsSold::siiQuantity, ivtInnerText)));
			spItemInfo->SetPurchaser(_GetCollectionItemValue(spChildElements, AuctionsSold::siiPurchaser, ivtInnerText));

			spItemInfo->SetAuctionSoldPrice(_GetPriceInfo(spChildElements, AuctionsSold::siiAuctionSoldPriceGold, 
				AuctionsSold::siiAuctionSoldPriceSilver, AuctionsSold::siiAuctionSoldPriceCopper));
		}
	}

	return hr;
}

namespace AuctionsEnded
{
	enum SoldItemIndex
	{
		siiTypeUrl = 1,
		siiName = 1,
		siiQuantity = 3,
		siiRemainDayInMail = 8,

		siiMinCount,
	};
};
HRESULT CMyAuctionItemPageAnalyze::_InnerGetAuctionsEndedRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
																	WowItemInfoPtr& spItemInfo, 
																	const CString& strSearchItemName)
{
	//CFHTMLElementCollectionDumper dumper(spChildElements, CFOutputWindowInfoOutput::Instance(), 0);
	HRESULT hr = E_FAIL;
	long nLength = 0;
	COM_VERIFY(spChildElements->get_length(&nLength));
	FTLASSERT(nLength >= AuctionsEnded::siiMinCount);
	if (nLength >= AuctionsEnded::siiMinCount)
	{
		spItemInfo->SetItemStatus(isMyAuctionEnded);
		spItemInfo->SetSeller(CUserInfoManager::Instance()->GetUserName());

		CString strItemName = _GetCollectionItemValue(spChildElements, AuctionsEnded::siiName, ivtInnerText, TRUE);
		if (strSearchItemName.IsEmpty() || strSearchItemName == strItemName)
		{
			CString strUrl = _GetCollectionItemValue(spChildElements, AuctionsEnded::siiTypeUrl, ivtToString);
			spItemInfo->SetUrl(strUrl);
			int nPosSlash = strUrl.ReverseFind(_T('/'));
			if (nPosSlash > 0)
			{
				CString strTypeId = strUrl.Mid(nPosSlash + 1);
				spItemInfo->SetTypeId(StrToLong(strTypeId));
			}
			spItemInfo->SetItemName(strItemName);

			spItemInfo->SetQuantity(StrToLong(_GetCollectionItemValue(spChildElements, AuctionsEnded::siiQuantity, ivtInnerText)));
			spItemInfo->SetRemainDayInMail(StrToLong(_GetCollectionItemValue(spChildElements, AuctionsEnded::siiRemainDayInMail, ivtInnerText, TRUE)));
		}
	}
	return hr;
}

HRESULT CMyAuctionItemPageAnalyze::_GetRowItemInfo(const CString& strTableName, CComQIPtr<IHTMLElementCollection>& spChildElements, 
												   WowItemInfoPtr& spItemInfo, 
												   const CString& strSearchItemName)
{

	HRESULT hr = E_FAIL;
	if (strTableName.Compare(TABLE_NAME_AUCTIONS_ACTIVE) == 0)
	{
		COM_VERIFY(_InnerGetAuctionsActiveRowItemInfo(spChildElements, spItemInfo, strSearchItemName));
	}
	else if(strTableName.Compare(TALBE_NAME_AUCTIONS_SOLD) == 0)
	{
		COM_VERIFY(_InnerGetAuctionsSoldRowItemInfo(spChildElements, spItemInfo, strSearchItemName));
	}
	else if(strTableName.Compare(TABLE_NAME_AUCTIONS_ENDED) == 0)
	{
		COM_VERIFY(_InnerGetAuctionsEndedRowItemInfo(spChildElements, spItemInfo, strSearchItemName));
	}
	else
	{
		FTLASSERT(FALSE);
	}
	return hr;
}
