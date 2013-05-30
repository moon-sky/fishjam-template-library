#include "StdAfx.h"
#include "MyBitItemPageAnalyze.h"
#include <ftlWebInterfaceDetect.h>

//当前竞标
#define TABLE_NAME_BIDS_ACTIVE		TEXT("bids-active")

//赢得拍卖
#define TABLE_NAME_BIDS_WON			TEXT("bids-won")

//竞标失败
#define TABLE_NAME_BIDS_LOST		TEXT("bids-lost")


CMyBitItemPageAnalyze::CMyBitItemPageAnalyze(CWowItemManager* pWowItemManager)
:CItemPageAnalyzeBase(pWowItemManager)
{
}

CMyBitItemPageAnalyze::~CMyBitItemPageAnalyze(void)
{
}

HRESULT CMyBitItemPageAnalyze::ParseItemPage( CComPtr<IHTMLDocument3>& spHtmlDoc, const CString& strSearchItemName )
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(spHtmlDoc, E_POINTER);
	HRESULT hr = E_FAIL;

	LPCTSTR pszTableName[] = {
		TABLE_NAME_BIDS_ACTIVE,
		TABLE_NAME_BIDS_WON,
		TABLE_NAME_BIDS_LOST,
	};
	COM_VERIFY(_INnerParseAllTables(spHtmlDoc, pszTableName, _countof(pszTableName), strSearchItemName));

	return hr;
}

namespace BidsActive{
	enum BitItemIndex
	{
		biiTypeUrl = 1,
		biiName = 2,
		biiSeller = 5,
		biiQuantity = 6,
		biiTimeInfo = 8,

		biiPriceBidGold = 13,		//竞标金币
		biiPriceBidSilver = 14,		//竞标银币
		biiPriceBidCopper = 15,		//竞标铜币

		biiPriceBuyoutGold = 17,	//一口价金币
		biiPriceBuyoutSilver = 18,	//一口价银币
		biiPriceBuyoutCopper = 19,	//一口价铜币

		biiMinCount,
	};
};
HRESULT CMyBitItemPageAnalyze::_InnerGetMyBidsActiveRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
															WowItemInfoPtr& spItemInfo, 
															const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	long nLength = 0;
	COM_VERIFY(spChildElements->get_length(&nLength));
	FTLASSERT(nLength >= BidsActive::biiMinCount);
	if (nLength >= BidsActive::biiMinCount)
	{
		spItemInfo->SetItemStatus(isMyBidsActive);
		CString strItemName = _GetCollectionItemValue(spChildElements, BidsActive::biiName, ivtInnerText);
		if (strSearchItemName.IsEmpty() || strSearchItemName == strItemName)
		{
			//搜索全部或当前的项目名就是需要搜索的项目
			CString strUrl = _GetCollectionItemValue(spChildElements, BidsActive::biiTypeUrl, ivtToString);
			spItemInfo->SetUrl(strUrl);
			int nPosSlash = strUrl.ReverseFind(_T('/'));
			if (nPosSlash > 0)
			{
				CString strTypeId = strUrl.Mid(nPosSlash + 1);
				spItemInfo->SetTypeId(StrToLong(strTypeId));
			}

			spItemInfo->SetItemName(strItemName);
			spItemInfo->SetSeller(_GetCollectionItemValue(spChildElements, BidsActive::biiSeller, ivtInnerText));
			spItemInfo->SetQuantity(StrToLong(_GetCollectionItemValue(spChildElements, BidsActive::biiQuantity, ivtInnerText)));

			CString strTimeInfo = _GetCollectionItemValue(spChildElements, BidsActive::biiTimeInfo, ivtInnerText, TRUE);
			spItemInfo->SetItemSellTimeInfo(_ConvertTimeInfo(strTimeInfo));

			spItemInfo->SetPriceBid(_GetPriceInfo(spChildElements, BidsActive::biiPriceBidGold, 
				BidsActive::biiPriceBidSilver, BidsActive::biiPriceBidCopper));
			spItemInfo->SetPriceBuyout(_GetPriceInfo(spChildElements, BidsActive::biiPriceBuyoutGold, 
				BidsActive::biiPriceBuyoutSilver, BidsActive::biiPriceBuyoutCopper));

			//最后才更新时间 -- 保证 SetItemSellTimeInfo 时能获取到上次更新的时间信息
			//spItemInfo->UpdateRefreshTime();
		}
	}
	else
	{
		hr = E_FAIL;
		FTLASSERT(FALSE && _T("Web Page Change"));
	}
	return hr;
}

namespace BidsWon
{
	enum BitItemIndex
	{
		biiTypeUrl = 1,
		biiName = 1,
		biiQuantity = 3,
		biiRemainDayInMail = 5,

		//拍卖里售出的价格?
		biiAuctionSoldGold = 8,
		biiAuctionSoldSilver = 9,
		biiAuctionSoldCopper = 10,

		biiMinCount,
	};
};
HRESULT CMyBitItemPageAnalyze::_InnerGetMyBidsWonRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
															WowItemInfoPtr& spItemInfo, 
															const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	long nLength = 0;

	//CFHTMLElementCollectionDumper dumper(spChildElements, CFOutputWindowInfoOutput::Instance(), 0);

	COM_VERIFY(spChildElements->get_length(&nLength));
	FTLASSERT(nLength >= BidsWon::biiMinCount);
	if (nLength >= BidsWon::biiMinCount)
	{
		spItemInfo->SetItemStatus(isMyBidsWon);

		CString strItemName = _GetCollectionItemValue(spChildElements, BidsWon::biiName, ivtInnerText, TRUE);
		if (strSearchItemName.IsEmpty() || strSearchItemName == strItemName)
		{
			//搜索全部或当前的项目名就是需要搜索的项目
			CString strUrl = _GetCollectionItemValue(spChildElements, BidsWon::biiTypeUrl, ivtToString);
			spItemInfo->SetUrl(strUrl);
			int nPosSlash = strUrl.ReverseFind(_T('/'));
			if (nPosSlash > 0)
			{
				CString strTypeId = strUrl.Mid(nPosSlash + 1);
				spItemInfo->SetTypeId(StrToLong(strTypeId));
			}

			spItemInfo->SetItemName(strItemName);
			//spItemInfo->SetSeller(_GetCollectionItemValue(spChildElements, BidsWon::biiSeller, ivtInnerText));
			spItemInfo->SetQuantity(StrToLong(_GetCollectionItemValue(spChildElements, BidsWon::biiQuantity, ivtInnerText)));

			//CString strTimeInfo = _GetCollectionItemValue(spChildElements, BidsWon::biiTimeInfo, ivtInnerText, TRUE);
			//spItemInfo->SetItemSellTimeInfo(_ConvertTimeInfo(strTimeInfo));
			spItemInfo->SetRemainDayInMail(StrToLong(_GetCollectionItemValue(spChildElements, BidsWon::biiRemainDayInMail, ivtInnerText)));

			//TODO: SetPriceBid -> SetAuctionSold
			spItemInfo->SetPriceBid(_GetPriceInfo(spChildElements, BidsWon::biiAuctionSoldGold, 
				BidsWon::biiAuctionSoldSilver, BidsWon::biiAuctionSoldCopper));
			//spItemInfo->SetPriceBuyout(_GetPriceInfo(spChildElements, BidsWon::biiPriceBuyoutGold, 
			//	BidsWon::biiPriceBuyoutSilver, BidsWon::biiPriceBuyoutCopper));

			//最后才更新时间 -- 保证 SetItemSellTimeInfo 时能获取到上次更新的时间信息
			//spItemInfo->UpdateRefreshTime();
		}
	}
	else
	{
		hr = E_FAIL;
		FTLASSERT(FALSE && _T("Web Page Change"));
	}

	return hr;
}

namespace BidsLost
{
	enum BitItemIndex
	{
		biiTypeUrl = 1,
		biiName = 1,
		biiQuantity = 3,

		biiPriceBidGold = 8,		//竞标金币
		biiPriceBidSilver = 9,		//竞标银币
		biiPriceBidCopper = 10,		//竞标铜币

		biiMinCount,

	};
};
HRESULT CMyBitItemPageAnalyze::_InnerGetMyBidsLostRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
															WowItemInfoPtr& spItemInfo, 
															const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	long nLength = 0;
	COM_VERIFY(spChildElements->get_length(&nLength));
	FTLASSERT(nLength >= BidsLost::biiMinCount);
	if (nLength >= BidsLost::biiMinCount)
	{
		spItemInfo->SetItemStatus(isMyBidsLost);
		CString strItemName = _GetCollectionItemValue(spChildElements, BidsLost::biiName, ivtInnerText, TRUE);
		if (strSearchItemName.IsEmpty() || strSearchItemName == strItemName)
		{
			//搜索全部或当前的项目名就是需要搜索的项目
			CString strUrl = _GetCollectionItemValue(spChildElements, BidsLost::biiTypeUrl, ivtToString);
			spItemInfo->SetUrl(strUrl);
			int nPosSlash = strUrl.ReverseFind(_T('/'));
			if (nPosSlash > 0)
			{
				CString strTypeId = strUrl.Mid(nPosSlash + 1);
				spItemInfo->SetTypeId(StrToLong(strTypeId));
			}

			spItemInfo->SetItemName(strItemName);
			//spItemInfo->SetSeller(_GetCollectionItemValue(spChildElements, BidsLost::biiSeller, ivtInnerText));
			spItemInfo->SetQuantity(StrToLong(_GetCollectionItemValue(spChildElements, BidsLost::biiQuantity, ivtInnerText)));

			//CString strTimeInfo = _GetCollectionItemValue(spChildElements, BidsLost::biiTimeInfo, ivtInnerText, TRUE);
			//spItemInfo->SetItemSellTimeInfo(_ConvertTimeInfo(strTimeInfo));

			spItemInfo->SetPriceBid(_GetPriceInfo(spChildElements, BidsLost::biiPriceBidGold, 
				BidsLost::biiPriceBidSilver, BidsLost::biiPriceBidCopper));
			//spItemInfo->SetPriceBuyout(_GetPriceInfo(spChildElements, BidsLost::biiPriceBuyoutGold, 
			//	BidsLost::biiPriceBuyoutSilver, BidsLost::biiPriceBuyoutCopper));
		}
	}
	else
	{
		hr = E_FAIL;
		FTLASSERT(FALSE && _T("Web Page Change"));
	}
	return hr;
}

HRESULT CMyBitItemPageAnalyze::_GetRowItemInfo(const CString& strTableName,
											 CComQIPtr<IHTMLElementCollection>& spChildElements, 
												WowItemInfoPtr& spItemInfo, 
												const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	if (strTableName.Compare(TABLE_NAME_BIDS_ACTIVE) == 0)
	{
		COM_VERIFY(_InnerGetMyBidsActiveRowItemInfo(spChildElements, spItemInfo, strSearchItemName));
	}
	else if(strTableName.Compare(TABLE_NAME_BIDS_WON) == 0)
	{
		COM_VERIFY(_InnerGetMyBidsWonRowItemInfo(spChildElements, spItemInfo, strSearchItemName));
	}
	else if(strTableName.Compare(TABLE_NAME_BIDS_LOST) == 0)
	{
		COM_VERIFY(_InnerGetMyBidsLostRowItemInfo(spChildElements, spItemInfo, strSearchItemName));
	}

	return hr;
}
