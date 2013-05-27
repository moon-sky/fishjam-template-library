#pragma once

#include "ItemPageAnalyzeBase.h"

class CBitItemPageAnalyze : public CItemPageAnalyzeBase
{
public:
	CBitItemPageAnalyze(CWowItemManager* pWowItemManager);
	~CBitItemPageAnalyze(void);

	virtual HRESULT ParseItemPage( CComPtr<IHTMLDocument3>& spHtmlDoc, const CString& strSearchItemName );
protected:
	virtual HRESULT _GetRowItemInfo(const CString& strTableName, CComQIPtr<IHTMLElementCollection>& spChildElements, WowItemInfoPtr& spItemInfo, const CString& strSearchItemName );

private:
	HRESULT _InnerGetActiveBidsRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, WowItemInfoPtr& spItemInfo, const CString& strSearchItemName);
	HRESULT _InnerGetWonBidsRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements, WowItemInfoPtr& spItemInfo, const CString& strSearchItemName);
	HRESULT _InnerGetLostBidsRowItemInfo(CComQIPtr<IHTMLElementCollection>& spChildElements,WowItemInfoPtr& spItemInfo, const CString& strSearchItemName);
};
