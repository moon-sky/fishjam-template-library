#pragma once

#include "WowAHDefine.h"

class CItemPageAnalyzeBase
{
public:
	CItemPageAnalyzeBase(CWowItemManager* pWowItemManager);
	~CItemPageAnalyzeBase(void);

	virtual HRESULT	ParseItemPage(CComPtr<IHTMLDocument3>& spHtmlDoc, const CString& strSearchItemName) = 0;
protected:
	virtual HRESULT _GetRowItemInfo(const CString& strTableName, CComQIPtr<IHTMLElementCollection>& spChildElements, WowItemInfoPtr& spItemInfo, const CString& strSearchItemName) = 0;

protected:
	enum ItemValueType
	{
		ivtInnerText,	
		ivtToString,
	};

	//子类可以重用的方法
	HRESULT _InnerParseTable(CComPtr<IHTMLTable>& spTable, const CString& strTableName, const CString& strSearchItemName);
	HRESULT _InnerParseRowItem(const CString& strTableName, long nIndex, CComQIPtr<IHTMLElement>& spPriceElementItem, const CString& strSearchItemName);

	CString _GetCollectionItemValue(CComQIPtr<IHTMLElementCollection>& spChildElements, INT index, ItemValueType valueType, BOOL bTrim = FALSE);

	ItemSellTimeInfo _ConvertTimeInfo(const CString& strTimeInfo);
	LONG _GetPriceInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, INT nGoldIndex, INT nSilverIndex, INT nCopperIndex);

	CWowItemManager* m_pWowItemManager;
};

