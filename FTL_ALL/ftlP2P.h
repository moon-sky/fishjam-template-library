#ifndef FTL_P2P_H
#define FTL_P2P_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlP2P.h requires ftlbase.h to be included first
#endif

#include <p2p.h>
#include <pnrpns.h>

namespace FTL
{
	/*************************************************************************************************************************
	* TODO:
	*   1.eMule 中为什么不通过IP地址的关系来生成节点ID(而是随机)，这样在进行XOR的时候，IP地址比较接近的节点的逻辑距离也会比较短
	*     CPrefs::m_uClientID
	*   2.CreateSearchExpressionTree 到底是创建搜索表达式还是创建节点的二叉树结构？
	*   3.刚启动时需要找的第一个节点(是类似服务器的节点？)
	*************************************************************************************************************************/

	/*************************************************************************************************************************
	* 已读例子
	*   (未完)netds\peertopeer\graphchat\xp
	*   (未完)netds\peertopeer\graphchat\vista -- Vista/Win7版本的P2P聊天程序
	*************************************************************************************************************************/

	/*************************************************************************************************************************
	*
	* P2P(Peer-to-Peer) -- 对等互联/点对点技术，是一种技术而不是协议。每个节点提供的功能对等，采用分布式网络存储结构
	*   可让一台计算机与另一台计算机直接交换数据，而不需要通过服务器处理。
	*   优势：负载均衡，降低服务器压力；高性价比；可扩展性强，可海量存储和计算
	*   问题：知识产权保护；网络安全
	* 设计模式：
	*   单纯型P2P架构 -- 没有中央服务器，各个节点之间直接交换信息，但需要用户手工指定对方的IP:端口，无法自动搜索扩展
	*   混合型P2P架构(NetpSter) -- 有一个中央服务器，仅起到促成各节点协调和扩展的功能。每个节点都向中央索引服务器注册和反注册，
	*     各个节点从中央服务器获取相关信息，再与对应的节点直接连接交互。
	* 开发的P2P算法：如Gnutella，FreeNet,Napster,Groove,Magi 等
	* 难点：
	*   1.维持应用层的连接 -- 有意（关机）或无意（拨号或网络断开）使P2P群体组发生频繁变化
	*   2.安全性 -- 
	*   3.需要开发用于消息交换的专用协议；
	*   4.必须查找并连接隐藏在"网络地址转换"(NAT) 或防火墙背后的某应用程序的实例
	*************************************************************************************************************************/
	

	/*************************************************************************************************************************
	* eMule(开源的P2P)
	*   下载和编译(第三方库需要用户自己下载)，编译时注意编译选项一致
	*     1.Crypto++(http://www.eskimo.com/~weidai/cryptlib.html), using namespace CryptoPP，
	*       CxImage
	*       id3lib -- 读、写ID3v1和ID3v2标签
	*       pnglib(http://www.libpng.org/pub/png/libpng.html)
	*       miniupnpc -- 操作局域网中所有的UPNP设备
	*       ResizableLib(http://sourceforge.net/projects/resizablelib/  -- 缩放时自动调整UI的库
	*       zlib(http://www.gzip.org/zlib/ -- 数据压缩
	*       AtlServer -- 似乎需要？SendMail.cpp 中使用了其中的 atlsmtpconnection.h
	*   分析:
	*     1.通过文件hash来区分文件(可判断出文件名不同而内容相同的文件) -- 不同内容的文件有相同的hash值很困难
	*     2.采用分块机制实现高效并发传输和验证(CKnownFile::CreateFromFile + CAICHRecoveryHashSet ), 采用了两层粒度的分块方式,
	*       (TODO:有什么好处？)先把大文件分隔成若干 9500K 的块，把这些块组织成一棵树状结构；再把每一个这样的块又分解成若干个 180K 的块,仍然按照树状结构组织
	*     3.AICH(Advanced Intelligent Corruption Handling,高级智能损坏处理) -- 分块计算hash，在文件传输时，即使发生错误，也可以不必重传整个文件
	*     4.ThrottledControlSocket + UploadBandwidthThrottler -- 实现网络全局限速的功能，将所有的套接字保存成发送队列，进行管理，其RunInternal中的逻辑：
	*       计算每个socket本次配额(能发送多少字节)->计算本次循环应该睡眠多少时间(通过睡眠进行限速)->操作控制信息队列发送队列中的数据
	*     5.通信协议(被设计成易于扩充的格式)，且利用了zlib进行压缩和解压缩
	*       TCP -- Header_Struct(协议簇 + 包长度 + 命令字 + 各个命令的结构数据)
	*       UDP -- UDPPack
    *     
	* Kad技术(eMule中使用,主要的类是 CKademlia) -- 是一种DHT(Distributed Hash Table 分散式杂凑表)的协议，可以使节点之间互相保留一些其他节点的联系信息，
	*   并且利用这样一个"关系网"寻找到整个网络中的任何一个节点以及上面的资源，整个过程不需要任何中心服务器。每个客户端负责处理一小部分
	*   search和source finding的工作，分配工作的原理是基于客户端的唯一ID和search或source的hash之间的匹配来确定的，然后用户即可定位
	*   发布或搜索时需要通知或访问的用户ID及地址 -- 通过用户id异或值决定他们之间的逻辑距离，大大提高了路由查询速度
	*   Kad网络中，所有节点(CContact)都被当作一棵二叉树的叶子，并且节点的位置都由其ID值的最短前缀唯一的确定，自己是二叉树的根节点。
	*   对于一个有N 个节点的Kad 网络，最多只需要经过logN 步查询，就可以准确定位到目标节点。
	*   四种远程RPC 操作:
	*     PING  -- 探测一个节点，用以判断其是否仍然在线
	*     STORE -- 通知一个节点存储一个<key, value>对
	*     FIND_NODE -- 对应emule中查找节点的操作
	*     FIND_VALUE -- 对应emule中文件检索的操作
	*   距离计算(查找和发布时进行，根据距离来确定查找的递进性以及应该发布到哪些节点)
	*     CRoutingZone::GetClosestTo -> CRoutingBin::GetClosestTo -> CUInt128::Xor
	*************************************************************************************************************************/

	/*****************************************************************************************************
	* 微软提供了 P2P 的SDK(p2p.h) -- 构建在IPV6上，目前的使用有较大限制
	*   PNRP --  基于网格名等要素来解析物理地址,本身就是一个采用服务形式的P2P应用程序
	*     WinXP上安装：添加/删除 Windows 组件 -> 网络服务 -> 对等网络，然后在服务中启动 "对等网络" 服务 
	*     通过 netsh 向PNRP注册一个新的PNRP名：netsh -> p2p phrp peer -> add 0.justtest(P2P名)
	*   PNM(People Near Me) -- Peer的名字格式是 [authority].[classifier]
	*     在PNM中注册程序，用户登录PNM后，p2phost.exe会在PNM网格中查找其他节点，找到后可以邀请, 
	*   
	*   调用流程：
	*     1.PeerGraphStartup -- 初始化整个P2P环境
	*     2.PeerGraphCreate  -- 
	*   相关概念
	*     Group
	*     Graph
	*     Identity
	*     PNRP(Peer Name Resolution Protocol) -- 对等名称解析协议，有对应的 PNRPsvc 服务
	*       PeerPnrpGetCloudInfo -- 获取调用节点参与的云列表，如 "LinkLocal_ff00::%11/8" 等
	*     Collaboration -- 
	*
	* Vista + .NET Framework 3.0 的WCF中引入了PeerChannel,
	*****************************************************************************************************/

	//微软P2P相关类的简单封装

	class CFP2PUtil
	{
	public:
		static void FreeData(LPCVOID pvData);
	};

	FTLEXPORT class CFP2PEnvInit
	{
	public:
		CFP2PEnvInit(BOOL bInitGraph = TRUE, BOOL bInitPnrp = TRUE);
		~CFP2PEnvInit();
	protected:
		//BOOL m_bStartup;
		BOOL m_bInitGraph;
		BOOL m_bInitPnrp;
		PEER_VERSION_DATA	m_PeerVersion;
	};

	FTLEXPORT class CFPeerPnrp
	{
	public:
		//need call FreeData to free ppCloudInfo
	};

	FTLEXPORT class CFPeerGraph
	{
	public:
		FTLINLINE CFPeerGraph();
		FTLINLINE virtual ~CFPeerGraph();
		FTLINLINE HRESULT CreateOrOpen(PCWSTR pwzGraphId, PCWSTR pwzPeerId, PCWSTR pwzDatabaseName, PPEER_SECURITY_INTERFACE pSecurityInterface);

		FTLINLINE HRESULT Create(PPEER_GRAPH_PROPERTIES pGraphProperties, PCWSTR pwzDatabaseName, PPEER_SECURITY_INTERFACE pSecurityInterface);
		FTLINLINE HRESULT Open(PCWSTR pwzGraphId, PCWSTR pwzPeerId, PCWSTR pwzDatabaseName, PPEER_SECURITY_INTERFACE pSecurityInterface,
			ULONG cRecordTypeSyncPrecedence, const GUID *pRecordTypeSyncPrecedence, PHGRAPH phGraph);

		FTLINLINE HRESULT Close();
	protected:

	private:
		HGRAPH	m_hGrpah;
	};


	FTLEXPORT class CFPeerIdentity
	{
	public: 
		FTLINLINE CFPeerIdentity();
		FTLINLINE virtual ~CFPeerIdentity();

		FTLINLINE HRESULT Create(PCWSTR pwzClassifier, PCWSTR pwzFriendlyName, HCRYPTPROV hCryptProv);
		FTLINLINE HRESULT Delete();

		FTLINLINE PWSTR  GetIdentity();
		//FTLINLINE PCWSTR GetFriendlyName();
		//FTLINLINE HRESULT SetFriendlyName(PCWSTR pwzFriendlyName);
		//FTLINLINE HCRYPTPROV GetCryptKey();
		
	private:
		PWSTR	m_pwzIdentity;

	};
}

#endif //FTL_P2P_H

#ifndef USE_EXPORT
#  include "ftlP2P.hpp"
#endif