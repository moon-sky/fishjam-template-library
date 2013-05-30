#pragma once

#include "ItemPageAnalyzeBase.h"

//我的拍卖 -- MyAuction.xht
class CMyAuctionItemPageAnalyze : public CItemPageAnalyzeBase
{
public:
	CMyAuctionItemPageAnalyze(CWowItemManager* pWowItemManager);
	~CMyAuctionItemPageAnalyze(void);

	//重载
	virtual HRESULT	ParseItemPage(CComPtr<IHTMLDocument3>& spHtmlDoc, const CString& strSearchItemName);

	//分析每一行的价格
	HRESULT _GetRowItemInfo(const CString& strTableName, CComQIPtr<IHTMLElementCollection>& spChildElements, WowItemInfoPtr& spItemInfo, const CString& strSearchItemName);

private:

	HRESULT _InnerGetAuctionsActiveRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
		WowItemInfoPtr& spItemInfo, const CString& strSearchItemName);
	HRESULT _InnerGetAuctionsSoldRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
		WowItemInfoPtr& spItemInfo, const CString& strSearchItemName);
	HRESULT _InnerGetAuctionsEndedRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, 
		WowItemInfoPtr& spItemInfo, const CString& strSearchItemName);

};
