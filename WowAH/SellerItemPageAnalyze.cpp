#include "StdAfx.h"
#include "SellerItemPageAnalyze.h"

CSellerItemPageAnalyze::CSellerItemPageAnalyze(CWowItemManager* pWowItemManager)
	:CItemPageAnalyzeBase(pWowItemManager)
{
}

CSellerItemPageAnalyze::~CSellerItemPageAnalyze(void)
{
}

HRESULT	CSellerItemPageAnalyze::ParseItemPage(CComPtr<IHTMLDocument3>& spHtmlDoc, 
											const CString& strSearchItemName)
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(spHtmlDoc, E_POINTER);
	HRESULT hr = E_FAIL;


	//获取所有的table
	CComPtr<IHTMLElementCollection> spTables;
	COM_VERIFY(spHtmlDoc->getElementsByTagName(CComBSTR(TEXT("table")), &spTables));
	if (spTables)
	{
		long nLength = 0;
		COM_VERIFY(spTables->get_length(&nLength));
		for (long nIndex = 0; nIndex < nLength; ++nIndex)
		{
			CComPtr<IDispatch>	spDispTable;
			CComVariant varIndex(nIndex);
			COM_VERIFY(spTables->item(varIndex, varIndex, &spDispTable));
			CComQIPtr<IHTMLTable> spTable = spDispTable;
			if (spTable)
			{
				COM_VERIFY(_InnerParseTable(spTable, TEXT("TODO:"), strSearchItemName));
			}
		}
	}

	return hr;
}

HRESULT CSellerItemPageAnalyze::_GetRowItemInfo(const CString& strTableName, CComQIPtr<IHTMLElementCollection>& spChildElements, 
												   WowItemInfoPtr& spItemInfo, 
												   const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	long nLength = 0;
	COM_VERIFY(spChildElements->get_length(&nLength));
	FTLASSERT(nLength >= siiMinCount);

	if (nLength > siiMinCount)
	{
		CString strItemName = _GetCollectionItemValue(spChildElements, siiName, ivtInnerText);
		if (strSearchItemName.IsEmpty() || strSearchItemName == strItemName)
		{
			//搜索全部或当前的项目名就是需要搜索的项目
			CString strUrl = _GetCollectionItemValue(spChildElements, siiTypeUrl, ivtToString);
			spItemInfo->SetUrl(strUrl);
			int nPosSlash = strUrl.ReverseFind(_T('/'));
			if (nPosSlash > 0)
			{
				CString strTypeId = strUrl.Mid(nPosSlash + 1);
				spItemInfo->SetTypeId(StrToLong(strTypeId));
			}

			spItemInfo->SetItemName(strItemName);
			spItemInfo->SetSeller(_GetCollectionItemValue(spChildElements, siiSeller, ivtInnerText));
			spItemInfo->SetQuantity(StrToLong(_GetCollectionItemValue(spChildElements, siiQuantity, ivtInnerText)));

			CString strTimeInfo = _GetCollectionItemValue(spChildElements, siiTimeInfo, ivtInnerText);
			spItemInfo->SetItemSellTimeInfo(_ConvertTimeInfo(strTimeInfo));

			spItemInfo->SetPriceBid(_GetPriceInfo(spChildElements, siiPriceBidGold, siiPriceBidSilver, siiPriceBidCopper));
			spItemInfo->SetPriceBuyout(_GetPriceInfo(spChildElements, siiPriceBuyoutGold, siiPriceBuyoutSilver, siiPriceBuyoutCopper));

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
