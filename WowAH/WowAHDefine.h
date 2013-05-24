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
	istiVeryLong,		//非常长(超过24小时)
	istiLong,			//长(在2小时和12小时之间)
	istiShort,			//短(2小时以内) ？
};

//拍卖行中的信息
struct ItemInAHInfo
{
	ItemInAHInfo();

	LONG				m_nId;		//每一项的唯一标识
	LONG				m_nTypeId;	//类型，蝎刺 对应的 5466, 和 UserConfigItemInfo 关联
	LONG				m_nDataItem;	//
	ItemSellTimeInfo	m_ItemSellTimeInfo;
	DWORD				m_nLastCheckTime;		//上次检测的时间点
	DWORD				m_nRemainTime;			//计算出来的准确值
	LONG				m_nQuantity;			//数量
	LONG				m_nPriceBid;			//拍卖价：金币 * 10000 + 银币 * 100 + 铜币
	LONG				m_nPriceBuyout;			//一口价：金币 * 10000 + 银币 * 100 + 铜币

	CString				m_strItemName;  
	CString				m_strUrl;
	CString				m_strSeller;

	VOID				Dump();
	LPCTSTR				GetSellTimeInfoString(ItemSellTimeInfo timeInfo);
};

typedef CFSharePtr<ItemInAHInfo>	ItemInAHInfoPtr;

typedef std::map<LONG, ItemInAHInfoPtr>		ItemInAHInfoContainer;