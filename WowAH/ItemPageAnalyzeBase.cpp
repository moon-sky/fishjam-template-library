#include "StdAfx.h"
#include "ItemPageAnalyzeBase.h"
#include "WowItemManager.h"

#include <ftlWebInterfaceDetect.h>

CItemPageAnalyzeBase::CItemPageAnalyzeBase(CWowItemManager* pWowItemManager)
{
	m_pWowItemManager = pWowItemManager;
}

CItemPageAnalyzeBase::~CItemPageAnalyzeBase(void)
{
}

HRESULT CItemPageAnalyzeBase::_INnerParseAllTables(CComPtr<IHTMLDocument3>& spHtmlDoc, 
												   LPCTSTR* ppszTalbeNames, INT nTableCount, const CString& strSearchItemName)
{
	HRESULT hr = E_INVALIDARG;

	for (int nTableIndex = 0; nTableIndex < nTableCount; ++nTableIndex)
	{
		CComPtr<IHTMLElement> spBidsElement;
		COM_VERIFY(spHtmlDoc->getElementById(CComBSTR(ppszTalbeNames[nTableIndex]), &spBidsElement));
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
				COM_VERIFY(_InnerParseTable(spTableBids, ppszTalbeNames[nTableIndex], strSearchItemName));
			}
		}
	}
	return hr;
}

HRESULT CItemPageAnalyzeBase::_InnerParseTable(CComPtr<IHTMLTable>& spTable, const CString& strTableName, const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	
	//CFHTMLTableDumper	tableDumper(spTable, CFOutputWindowInfoOutput::Instance(), 0);
	CComPtr<IHTMLElementCollection>	spRows;
	COM_VERIFY(spTable->get_rows(&spRows));
	if (spRows)
	{
		long nRows = 0;
		COM_VERIFY(spRows->get_length(&nRows));

		//nRows = FTL_MIN(2, nRows);

		//跳过第一行 -- 是表头，然后分析后面的价格数据
		for (long nIndex = 1; nIndex < nRows; ++nIndex)
		{
			CComPtr<IDispatch>	spDispItem;
			CComVariant varIndex(nIndex);
			COM_VERIFY(spRows->item(varIndex, varIndex, &spDispItem));
			CComQIPtr<IHTMLElement> spPriceElementItem = spDispItem;
			if (spPriceElementItem)
			{
#if 0
				//启用这行代码，可以打印出所有的价格列表信息，方便在网页更改后分析
				CFHTMLElementDumper htmlElementDumper(spPriceElementItem, CFOutputWindowInfoOutput::Instance(), 
					0, nIndex);
#endif
				//每一行的价格
				COM_VERIFY(_InnerParseRowItem(strTableName, nIndex, spPriceElementItem, strSearchItemName));
			}
		}
	}
	return hr;
}

HRESULT CItemPageAnalyzeBase::_InnerParseRowItem(const CString& strTableName, long /*nIndex*/,
												   CComQIPtr<IHTMLElement>& spPriceElementItem, const CString& strSearchItemName)
{
	HRESULT hr = E_FAIL;
	//FTLTRACE(TEXT("ParsePrice:%d\n"), nIndex);

	WowItemInfoPtr spItemInfo(NULL);

	//COM_DETECT_SERVICE_PROVIDER_FROM_LIST(spPriceElementItem);

	if (spPriceElementItem)
	{
		//CFHTMLElementDumper	htmlElementDumper(spPriceElementItem, CFOutputWindowInfoOutput::Instance(), 0);

		CComBSTR bstrId;
		COM_VERIFY(spPriceElementItem->get_id(&bstrId));
		if (bstrId)
		{
			CString strAuctionIdInfo(OLE2CT(bstrId));
			spItemInfo = m_pWowItemManager->GetItemInAHInfo(strAuctionIdInfo);
		}

		if (spItemInfo)
		{
			CComPtr<IDispatch>	spDispItemAll;
			COM_VERIFY(spPriceElementItem->get_all(&spDispItemAll));
			if (spDispItemAll)
			{
				CComQIPtr<IHTMLElementCollection> spChildElements(spDispItemAll);
				_GetRowItemInfo(strTableName, spChildElements, spItemInfo, strSearchItemName);
				//COM_DETECT_INTERFACE_FROM_REGISTER(spDispItemAll);
			}
		}
	}
	return hr;	
}



CString CItemPageAnalyzeBase::_GetCollectionItemValue(CComQIPtr<IHTMLElementCollection>& spChildElements, 
													  INT index, ItemValueType valueType, BOOL bTrim /* = FALSE */)
{
	HRESULT hr = E_FAIL;

	CString strResult;
	CComVariant varIndex(index);
	CComPtr<IDispatch> spDispValue;

	COM_VERIFY(spChildElements->item(varIndex, varIndex, &spDispValue));
	CComQIPtr<IHTMLElement>	spElement(spDispValue);
	if (spElement)
	{
		CComBSTR bstrValue;

		switch (valueType)
		{
		case ivtInnerText:
			COM_VERIFY(spElement->get_innerText(&bstrValue));
			strResult = OLE2CT(bstrValue);
			break;
		case ivtToString:
			COM_VERIFY(spElement->toString(&bstrValue));
			strResult = OLE2CT(bstrValue);
			break;
		default:
			FTLASSERT(FALSE);
			break;
		}
	}
	if (bTrim)
	{
		strResult.Trim();
	}
	return strResult;
}

ItemSellTimeInfo CItemPageAnalyzeBase::_ConvertTimeInfo(const CString& strTimeInfo)
{
	ItemSellTimeInfo timeInfo = istiVeryLong;
	if (0 == strTimeInfo.Compare(TEXT("非常长")))
	{
		timeInfo = istiVeryLong;
	}
	else if(0 == strTimeInfo.Compare(TEXT("长")))
	{
		timeInfo = istiLong;
	}
	else if(0 == strTimeInfo.Compare(TEXT("中")))
	{
		timeInfo = istiMiddle;
	}
	else if(0 == strTimeInfo.Compare(TEXT("短")))
	{
		timeInfo = istiShort;
	}
	else
	{
		FTLASSERT(FALSE);
	}
	return timeInfo;
}

LONG CItemPageAnalyzeBase::_GetPriceInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, INT nGoldIndex, INT nSilverIndex, INT nCopperIndex)
{
	LONG nPrice = 0;
	LONG nGold = StrToLong(_GetCollectionItemValue(spChildElements, nGoldIndex, ivtInnerText));
	LONG nSilver = StrToLong(_GetCollectionItemValue(spChildElements, nSilverIndex, ivtInnerText));
	LONG nCopper = StrToLong(_GetCollectionItemValue(spChildElements, nCopperIndex, ivtInnerText));

	nPrice = nGold * 10000 + nSilver * 100 + nCopper;
	//FTLTRACE(TEXT("_GetPriceInfo for %d,%d,%d is %d\n"), nGoldIndex, nSilverIndex, nCopperIndex, nPrice);

	return nPrice;
}