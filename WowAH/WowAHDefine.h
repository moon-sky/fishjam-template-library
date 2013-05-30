#pragma once

#include <ftlBase.h>
#include <ftlSharePtr.h>
using namespace FTL;

#include <map>
#include <functional>

//用户配置的想要买卖的项目的信息
struct UserConfigItemInfo
{
	LONG		nTypeId;				//5466  -- /wow/zh/item/5466
	CString		strName;				//产品名 -- 蝎刺
	CString     strUrl;					//http://content.battlenet.com.cn/wow/icons/36/spell_nature_nullifypoison.jpg
	INT			nBuyPriceBid;			//竞价
	INT			nBuyPriceBuyout;		//一口价
	//INT			nSellPriceBid;
	//INT			nSellPriceBuyout;
};

//typedef CFSharePtr<UserConfigItemInfo>	UserConfigItemInfoPtr;


enum ItemSellTimeInfo
{
	istiInvalid,		//无效值，用于初始化

	//具体值需要确认
	istiVeryLong,		//非常长(超过24小时)
	istiLong,			//长(在2小时和12小时之间)
	istiMiddle,			//中(2小时以内)
	istiShort,			//短(少于30分钟)
};

//拍卖行中的信息
class WowItemInfo
{
public:
	WowItemInfo();
	LPCTSTR				GetSellTimeInfoString(ItemSellTimeInfo timeInfo);
	VOID				Dump();

	void UpdateRefreshTime();

	LONG GetId() const { return m_nId; }
	void SetId(LONG val) { m_nId = val; }
	LONG GetTypeId() const { return m_nTypeId; }
	void SetTypeId(LONG val) { m_nTypeId = val; }
	LONG GetDataItem() const { return m_nDataItem; }
	void SetDataItem(LONG val) { m_nDataItem = val; }
	ItemSellTimeInfo GetItemSellTimeInfo() const { return m_ItemSellTimeInfo; }
	void SetItemSellTimeInfo(ItemSellTimeInfo timeInfo);
	LONG GetQuantity() const { return m_nQuantity; }
	void SetQuantity(LONG val) { m_nQuantity = val; }
	LONG GetPriceBid() const { return m_nPriceBid; }
	void SetPriceBid(LONG val) { m_nPriceBid = val; }
	LONG GetPriceBuyout() const { return m_nPriceBuyout; }
	void SetPriceBuyout(LONG val) { m_nPriceBuyout = val; }
	const CString& GetUrl() const { return m_strUrl; }
	void SetUrl(const CString& val) { m_strUrl = val; }
	const CString& GetSeller() const { return m_strSeller; }
	void SetSeller(const CString& val) { m_strSeller = val; }
	const CString& GetItemName() const { return m_strItemName; }
	void SetItemName(const CString& val) { m_strItemName = val; }
	
	
private:

	LONG				m_nId;		//每一项的唯一标识
	LONG				m_nTypeId;	//类型，蝎刺 对应的 5466, 和 UserConfigItemInfo 关联
	LONG				m_nDataItem;	//
	ItemSellTimeInfo	m_ItemSellTimeInfo;
	LONG				m_nQuantity;			//数量
	LONG				m_nPriceBid;			//拍卖价：金币 * 10000 + 银币 * 100 + 铜币
	LONG				m_nPriceBuyout;			//一口价：金币 * 10000 + 银币 * 100 + 铜币，如果值为0表示没有一口价
	CString				m_strItemName;  
	CString				m_strUrl;
	CString				m_strSeller;

	ULONGLONG			m_nLastRefreshTickCount;//上次刷新的时间点
	ULONGLONG			m_nExpirationTickCount;	//计算出来的到期时间点
};

typedef CFSharePtr<WowItemInfo>	WowItemInfoPtr;


//前向声明
class CWowItemManager;
class CBitItemPageAnalyze;
