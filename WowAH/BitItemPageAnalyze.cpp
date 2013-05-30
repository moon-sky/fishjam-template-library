#include "StdAfx.h"
#include "BitItemPageAnalyze.h"
#include "ftlWebInterfaceDetect.h"

//当前竞标
#define TABLE_NAME_ACTIVE_BIDS		TEXT("bids-active")

//赢得拍卖
#define TABLE_NAME_WON_BIDS			TEXT("bids-won")

//竞标失败
#define TABLE_NAME_LOST_BIDS		TEXT("bids-lost")


namespace ActiveBids{
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

namespace WonBids
{
	enum BitItemIndex
	{
		biiTypeUrl = 1,
		

		biiMinCount,
	};
};

namespace LostBids
{
	enum BitItemIndex
	{
		biiTypeUrl = 1,
		biiName = 1,
		//biiSeller = 5,
		biiQuantity = 3,
		//biiTimeInfo = 8,

		biiPriceBidGold = 8,		//竞标金币
		biiPriceBidSilver = 9,		//竞标银币
		biiPriceBidCopper = 10,		//竞标铜币

		//biiPriceBuyoutGold = 17,	//一口价金币
		//biiPriceBuyoutSilver = 18,	//一口价银币
		//biiPriceBuyoutCopper = 19,	//一口价铜币

		biiMinCount,

	};
};


CBitItemPageAnalyze::CBitItemPageAnalyze(CWowItemManager* pWowItemManager)
:CItemPageAnalyzeBase(pWowItemManager)
{
}

CBitItemPageAnalyze::~CBitItemPageAnalyze(void)
{
}

HRESULT CBitItemPageAnalyze::ParseItemPage( CComPtr<IHTMLDocument3>& spHtmlDoc, const CString& strSearchItemName )
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(spHtmlDoc, E_POINTER);
	HRESULT hr = E_FAIL;

	LPCTSTR pszTableName[] = {
		//TABLE_NAME_ACTIVE_BIDS,
		//TABLE_NAME_WON_BIDS,
		TABLE_NAME_LOST_BIDS,
	};

	for (int i = 0; i < _countof(pszTableName); ++i)
	{
		CComPtr<IHTMLElement> spBidsElement;
		COM_VERIFY(spHtmlDoc->getElementById(CComBSTR(pszTableName[i]), &spBidsElement));
		FTLASSERT(spBidsElement);
		if (spBidsElement)
		{
			CComPtr<IHTMLElement> spElementBids;
			COM_VERIFY(CFWebBrowserUtil::FindChildElement(spBidsElement, femByTag, TEXT("table"), 
				0, rwBreadthFirst, &spElementBids));

			CComQIPtr<IHTMLTable>	spTableBids = spElementBids;
			FTLASSERT(spTableBids);
			if (spTableBids)
			{
				COM_VERIFY(_InnerParseTable(spTableBids, pszTableName[i], strSearchItemName));
			}
		}

	}

	return hr;
}

HRESULT CBitItemPageAnalyze::_InnerGetActiveBidsRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
															WowItemInfoPtr& spItemInfo, 
															const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	long nLength = 0;
	COM_VERIFY(spChildElements->get_length(&nLength));
	FTLASSERT(nLength > ActiveBids::biiMinCount);
	if (nLength > ActiveBids::biiMinCount)
	{
		CString strItemName = _GetCollectionItemValue(spChildElements, ActiveBids::biiName, ivtInnerText);
		if (strSearchItemName.IsEmpty() || strSearchItemName == strItemName)
		{
			//搜索全部或当前的项目名就是需要搜索的项目
			CString strUrl = _GetCollectionItemValue(spChildElements, ActiveBids::biiTypeUrl, ivtToString);
			spItemInfo->SetUrl(strUrl);
			int nPosSlash = strUrl.ReverseFind(_T('/'));
			if (nPosSlash > 0)
			{
				CString strTypeId = strUrl.Mid(nPosSlash + 1);
				spItemInfo->SetTypeId(StrToLong(strTypeId));
			}

			spItemInfo->SetItemName(strItemName);
			spItemInfo->SetSeller(_GetCollectionItemValue(spChildElements, ActiveBids::biiSeller, ivtInnerText));
			spItemInfo->SetQuantity(StrToLong(_GetCollectionItemValue(spChildElements, ActiveBids::biiQuantity, ivtInnerText)));

			CString strTimeInfo = _GetCollectionItemValue(spChildElements, ActiveBids::biiTimeInfo, ivtInnerText, TRUE);
			spItemInfo->SetItemSellTimeInfo(_ConvertTimeInfo(strTimeInfo));

			spItemInfo->SetPriceBid(_GetPriceInfo(spChildElements, ActiveBids::biiPriceBidGold, 
				ActiveBids::biiPriceBidSilver, ActiveBids::biiPriceBidCopper));
			spItemInfo->SetPriceBuyout(_GetPriceInfo(spChildElements, ActiveBids::biiPriceBuyoutGold, 
				ActiveBids::biiPriceBuyoutSilver, ActiveBids::biiPriceBuyoutCopper));

			//最后才更新时间 -- 保证 SetItemSellTimeInfo 时能获取到上次更新的时间信息
			spItemInfo->UpdateRefreshTime();
		}
	}
	else
	{
		hr = E_FAIL;
		FTLASSERT(FALSE && _T("Web Page Change"));
	}
	return hr;
}

HRESULT CBitItemPageAnalyze::_InnerGetWonBidsRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
															WowItemInfoPtr& spItemInfo, 
															const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	long nLength = 0;

	CFHTMLElementCollectionDumper dumper(spChildElements, CFOutputWindowInfoOutput::Instance(), 0);

	COM_VERIFY(spChildElements->get_length(&nLength));
	FTLASSERT(nLength > WonBids::biiMinCount);
	if (nLength > WonBids::biiMinCount)
	{
#if 0
		CString strItemName = _GetCollectionItemValue(spChildElements, WonBids::biiName, ivtInnerText, TRUE);
		if (strSearchItemName.IsEmpty() || strSearchItemName == strItemName)
		{
			//搜索全部或当前的项目名就是需要搜索的项目
			CString strUrl = _GetCollectionItemValue(spChildElements, WonBids::biiTypeUrl, ivtToString);
			spItemInfo->SetUrl(strUrl);
			int nPosSlash = strUrl.ReverseFind(_T('/'));
			if (nPosSlash > 0)
			{
				CString strTypeId = strUrl.Mid(nPosSlash + 1);
				spItemInfo->SetTypeId(StrToLong(strTypeId));
			}

			spItemInfo->SetItemName(strItemName);
			//spItemInfo->SetSeller(_GetCollectionItemValue(spChildElements, LostBids::biiSeller, ivtInnerText));
			spItemInfo->SetQuantity(StrToLong(_GetCollectionItemValue(spChildElements, WonBids::biiQuantity, ivtInnerText)));

			//CString strTimeInfo = _GetCollectionItemValue(spChildElements, LostBids::biiTimeInfo, ivtInnerText, TRUE);
			//spItemInfo->SetItemSellTimeInfo(_ConvertTimeInfo(strTimeInfo));

			spItemInfo->SetPriceBid(_GetPriceInfo(spChildElements, WonBids::biiPriceBidGold, 
				WonBids::biiPriceBidSilver, WonBids::biiPriceBidCopper));
			//spItemInfo->SetPriceBuyout(_GetPriceInfo(spChildElements, LostBids::biiPriceBuyoutGold, 
			//	LostBids::biiPriceBuyoutSilver, LostBids::biiPriceBuyoutCopper));

			//最后才更新时间 -- 保证 SetItemSellTimeInfo 时能获取到上次更新的时间信息
			//spItemInfo->UpdateRefreshTime();
		}
#endif
	}
	else
	{
		hr = E_FAIL;
		FTLASSERT(FALSE && _T("Web Page Change"));
	}

	return hr;
}

HRESULT CBitItemPageAnalyze::_InnerGetLostBidsRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
															WowItemInfoPtr& spItemInfo, 
															const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	long nLength = 0;
	COM_VERIFY(spChildElements->get_length(&nLength));
	FTLASSERT(nLength > LostBids::biiMinCount);
	if (nLength > LostBids::biiMinCount)
	{
		CString strItemName = _GetCollectionItemValue(spChildElements, LostBids::biiName, ivtInnerText, TRUE);
		if (strSearchItemName.IsEmpty() || strSearchItemName == strItemName)
		{
			//搜索全部或当前的项目名就是需要搜索的项目
			CString strUrl = _GetCollectionItemValue(spChildElements, LostBids::biiTypeUrl, ivtToString);
			spItemInfo->SetUrl(strUrl);
			int nPosSlash = strUrl.ReverseFind(_T('/'));
			if (nPosSlash > 0)
			{
				CString strTypeId = strUrl.Mid(nPosSlash + 1);
				spItemInfo->SetTypeId(StrToLong(strTypeId));
			}

			spItemInfo->SetItemName(strItemName);
			//spItemInfo->SetSeller(_GetCollectionItemValue(spChildElements, LostBids::biiSeller, ivtInnerText));
			spItemInfo->SetQuantity(StrToLong(_GetCollectionItemValue(spChildElements, LostBids::biiQuantity, ivtInnerText)));

			//CString strTimeInfo = _GetCollectionItemValue(spChildElements, LostBids::biiTimeInfo, ivtInnerText, TRUE);
			//spItemInfo->SetItemSellTimeInfo(_ConvertTimeInfo(strTimeInfo));

			spItemInfo->SetPriceBid(_GetPriceInfo(spChildElements, LostBids::biiPriceBidGold, 
				LostBids::biiPriceBidSilver, LostBids::biiPriceBidCopper));
			//spItemInfo->SetPriceBuyout(_GetPriceInfo(spChildElements, LostBids::biiPriceBuyoutGold, 
			//	LostBids::biiPriceBuyoutSilver, LostBids::biiPriceBuyoutCopper));

			//最后才更新时间 -- 保证 SetItemSellTimeInfo 时能获取到上次更新的时间信息
			spItemInfo->UpdateRefreshTime();
		}
	}
	else
	{
		hr = E_FAIL;
		FTLASSERT(FALSE && _T("Web Page Change"));
	}
	return hr;
}

HRESULT CBitItemPageAnalyze::_GetRowItemInfo(const CString& strTableName,
											 CComQIPtr<IHTMLElementCollection>& spChildElements, 
												WowItemInfoPtr& spItemInfo, 
												const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	long nLength = 0;
	COM_VERIFY(spChildElements->get_length(&nLength));
	if (strTableName.Compare(TABLE_NAME_ACTIVE_BIDS) == 0)
	{
		COM_VERIFY(_InnerGetActiveBidsRowItemInfo(spChildElements, spItemInfo, strSearchItemName));
	}
	else if(strTableName.Compare(TABLE_NAME_WON_BIDS) == 0)
	{
		COM_VERIFY(_InnerGetWonBidsRowItemInfo(spChildElements, spItemInfo, strSearchItemName));
	}
	else if(strTableName.Compare(TABLE_NAME_LOST_BIDS) == 0)
	{
		CFHTMLElementCollectionDumper collDumper(spChildElements, CFOutputWindowInfoOutput::Instance(), 0);
		COM_VERIFY(_InnerGetLostBidsRowItemInfo(spChildElements, spItemInfo, strSearchItemName));
	}



	return hr;
}
