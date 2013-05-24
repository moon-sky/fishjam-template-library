#include "StdAfx.h"
#include "ItemPageAnalyzeBase.h"
#include <ftlWebInterfaceDetect.h>


CItemPageAnalyzeBase::CItemPageAnalyzeBase(void)
{
}

CItemPageAnalyzeBase::~CItemPageAnalyzeBase(void)
{
	m_Items.clear();
}

HRESULT	CItemPageAnalyzeBase::ParseItemPage(CComPtr<IHTMLDocument3>& spHtmlDoc)
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
				COM_VERIFY(_InnerParseTable(spTable));
			}
		}
	}

	return hr;
}

HRESULT CItemPageAnalyzeBase::_InnerParseTable(CComPtr<IHTMLTable>& spTable)
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
				COM_VERIFY(_InnerParsePriceRowItem(nIndex, spPriceElementItem));
			}
		}
	}
	return hr;
}

ItemInAHInfoPtr	CItemPageAnalyzeBase::_GetItemInAnInfo(const CString& strIdInfo)
{
	ItemInAHInfoPtr spItemInfo(NULL);

	int nPos = strIdInfo.Find(TEXT("-"));
	FTLASSERT(nPos == 7);
	if (nPos > 0)
	{
		CString strId = strIdInfo.Mid(nPos + 1);
		LONG nId = StrToLong(strId);
		FTLASSERT(nId > 0);
		if (nId > 0)
		{
			ItemInAHInfoContainer::iterator iter = m_Items.find(nId);
			if (iter != m_Items.end())
			{
				spItemInfo = ItemInAHInfoPtr(iter->second);
			}
			else
			{
				spItemInfo = ItemInAHInfoPtr(new ItemInAHInfo());
				spItemInfo->m_nId = nId;
				m_Items.insert(ItemInAHInfoContainer::value_type(nId, spItemInfo));
			}
		}
	}
	return spItemInfo;
}

HRESULT CItemPageAnalyzeBase::_InnerParsePriceRowItem(long nIndex, CComQIPtr<IHTMLElement>& spPriceElementItem)
{
	HRESULT hr = E_FAIL;
	//FTLTRACE(TEXT("ParsePrice:%d\n"), nIndex);

	ItemInAHInfoPtr spItemInfo(NULL);

	//COM_DETECT_SERVICE_PROVIDER_FROM_LIST(spPriceElementItem);

	if (spPriceElementItem)
	{
		//CFHTMLElementDumper	htmlElementDumper(spPriceElementItem, CFOutputWindowInfoOutput::Instance(), 0);

		CComBSTR bstrId;
		COM_VERIFY(spPriceElementItem->get_id(&bstrId));
		if (bstrId)
		{
			CString strIdInfo(OLE2CT(bstrId));
			spItemInfo = _GetItemInAnInfo(strIdInfo);
		}

		if (spItemInfo)
		{
			CComPtr<IDispatch>	spDispItemAll;
			COM_VERIFY(spPriceElementItem->get_all(&spDispItemAll));
			if (spDispItemAll)
			{
				CComQIPtr<IHTMLElementCollection> spChildElements(spDispItemAll);
				_GetPriceRowItemInfo(spChildElements, spItemInfo);
				spItemInfo->Dump();

				//COM_DETECT_INTERFACE_FROM_REGISTER(spDispItemAll);
			}
		}

	}

	
	return hr;	
}

HRESULT CItemPageAnalyzeBase::_GetPriceRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, ItemInAHInfoPtr& spItemInfo)
{
	HRESULT hr = E_FAIL;
	long nLength = 0;
	COM_VERIFY(spChildElements->get_length(&nLength));
	FTLASSERT(nLength >= iiMinCount);

	if (nLength > iiMinCount)
	{
		spItemInfo->m_strUrl = _GetCollectionItemValue(spChildElements, iiTypeUrl, ivtToString);
		int nPosSlash = spItemInfo->m_strUrl.ReverseFind(_T('/'));
		if (nPosSlash > 0)
		{
			CString strTypeId = spItemInfo->m_strUrl.Mid(nPosSlash + 1);
			spItemInfo->m_nTypeId = StrToLong(strTypeId);
		}

		spItemInfo->m_strItemName = _GetCollectionItemValue(spChildElements, iiName, ivtInnerText);
		spItemInfo->m_strSeller = _GetCollectionItemValue(spChildElements, iiSeller, ivtInnerText);
		spItemInfo->m_nQuantity = StrToLong(_GetCollectionItemValue(spChildElements, iiQuantity, ivtInnerText));
		
		CString strTimeInfo = _GetCollectionItemValue(spChildElements, iiTimeInfo, ivtInnerText);
		spItemInfo->m_ItemSellTimeInfo = _ConvertTimeInfo(strTimeInfo);

		spItemInfo->m_nPriceBid = _GetPriceInfo(spChildElements, iiPriceBidGold, iiPriceBidSilver, iiPriceBidCopper);
		spItemInfo->m_nPriceBuyout = _GetPriceInfo(spChildElements, iiPriceBuyoutGold, iiPriceBuyoutSilver, iiPriceBuyoutCopper);
		
	}
	else
	{
		hr = E_FAIL;
	}

	return hr;
}

CString CItemPageAnalyzeBase::_GetCollectionItemValue(CComQIPtr<IHTMLElementCollection>& spChildElements, 
													  INT index, ItemValueType valueType)
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
	
	return strResult;
}

ItemSellTimeInfo CItemPageAnalyzeBase::_ConvertTimeInfo(const CString& strTimeInfo)
{
	ItemSellTimeInfo timeInfo = istiVeryLong;
	if (strTimeInfo.Compare(TEXT("非常长")))
	{
		timeInfo = istiVeryLong;
	}
	else if(strTimeInfo.Compare(TEXT("长")))
	{
		timeInfo = istiLong;
	}
	else
	{
		FTLASSERT(FALSE); //短?
		timeInfo = istiShort;
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