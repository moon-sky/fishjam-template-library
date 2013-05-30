#pragma once

#include "ItemPageAnalyzeBase.h"

//分析"我的竞标" -- MyBid.xht
class CMyBitItemPageAnalyze : public CItemPageAnalyzeBase
{
public:
	CMyBitItemPageAnalyze(CWowItemManager* pWowItemManager);
	~CMyBitItemPageAnalyze(void);

	virtual HRESULT ParseItemPage( CComPtr<IHTMLDocument3>& spHtmlDoc, const CString& strSearchItemName );
protected:
	virtual HRESULT _GetRowItemInfo(const CString& strTableName, CComQIPtr<IHTMLElementCollection>& spChildElements, WowItemInfoPtr& spItemInfo, const CString& strSearchItemName );

private:
	HRESULT _InnerGetMyBidsActiveRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, WowItemInfoPtr& spItemInfo, const CString& strSearchItemName);
	HRESULT _InnerGetMyBidsWonRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, WowItemInfoPtr& spItemInfo, const CString& strSearchItemName);
	HRESULT _InnerGetMyBidsLostRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements,WowItemInfoPtr& spItemInfo, const CString& strSearchItemName);
};
