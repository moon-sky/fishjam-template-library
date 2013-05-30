#pragma once

#include <ftlBase.h>
#include <ftlSharePtr.h>
using namespace FTL;

#include <map>
#include <functional>

#ifndef INVALID_LONG
#  define INVALID_LONG		(LONG)(-1)
#endif 

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

//enum ItemSellerType{
//	istInvalid = -1,
//	istOthers,
//	istMine,
//};

enum ItemStatus
{
	isInvalid = -1,

	isOtherSelling,			//其他人正在出售

	//竞标页
	isMyBidsActive,
	isMyBidsWon,
	isMyBidsLost,

	//拍卖页
	isMyAuctionActive,		//进行中的拍卖
	isMyAuctionSold,		//已售出
	isMyAuctionEnded,		//已结束的拍卖
};

//拍卖行中的信息
class WowItemInfo
{
public:
	WowItemInfo();
	static LPCTSTR GetSellTimeInfoString(ItemSellTimeInfo timeInfo);
	static LPCTSTR GetItemStatusString(ItemStatus itemStatus);

	BOOL				IsValidItem();
	VOID				Dump(LONG nIndex, LONG nCount);

	//void UpdateRefreshTime();

	ItemStatus GetItemStatus() const { return m_itemStatus; }
	void SetItemStatus(ItemStatus val) { m_itemStatus = val; }
	//ItemSellerType GetItemSellerType() const { return m_ItemSellerType; }
	//void SetItemSellerType(ItemSellerType val) { m_ItemSellerType = val; }
	LONG GetAuctionId() const { return m_nAuctionId; }
	void SetAuctionId(LONG val) { m_nAuctionId = val; }
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
	LONG GetAuctionSoldPrice() const { return m_nAuctionSoldPrice; }
	void SetAuctionSoldPrice(LONG val) { m_nAuctionSoldPrice = val; }
	LONG GetRemainDayInMail() const { return m_nRemainDayInMail; }
	void SetRemainDayInMail(LONG val) { m_nRemainDayInMail = val; }
	LONG GetMailId() const { return m_nMailId; }
	void SetMailId(LONG val) { m_nMailId = val; }

	const CString& GetItemName() const { return m_strItemName; }
	void SetItemName(const CString& strItemName);
	const CString& GetUrl() const { return m_strUrl; }
	void SetUrl(const CString& strUrl);
	const CString& GetSeller() const { return m_strSeller; }
	void SetSeller(const CString& val) { m_strSeller = val; }
	CString GetPurchaser() const { return m_strPurchaser; }
	void SetPurchaser(CString val) { m_strPurchaser = val; }

	
private:
	ItemStatus			m_itemStatus;
	//ItemSellerType		m_ItemSellerType;
	LONG				m_nAuctionId;		//每一个拍买项的唯一标识
	LONG				m_nTypeId;			//类型，蝎刺 对应的 5466, 和 UserConfigItemInfo 关联
	LONG				m_nDataItem;		//
	ItemSellTimeInfo	m_ItemSellTimeInfo;
	LONG				m_nQuantity;			//数量
	LONG				m_nPriceBid;			//拍卖价：金币 * 10000 + 银币 * 100 + 铜币
	LONG				m_nPriceBuyout;			//一口价：金币 * 10000 + 银币 * 100 + 铜币，如果值为0表示没有一口价
	LONG				m_nAuctionSoldPrice;	//拍卖售出的价格
	LONG				m_nRemainDayInMail;		//邮箱里保存的天数
	LONG				m_nMailId;				//邮箱ID
	CString				m_strItemName;  
	CString				m_strUrl;
	CString				m_strSeller;			//出售者
	CString				m_strPurchaser;			//购买者
	ULONGLONG			m_nLastRefreshTickCount;//上次刷新的时间点
	ULONGLONG			m_nExpirationTickCount;	//计算出来的到期时间点
};

typedef CFSharePtr<WowItemInfo>	WowItemInfoPtr;


//前向声明
class CWowItemManager;

