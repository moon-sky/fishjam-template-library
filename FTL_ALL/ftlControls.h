#ifndef FTL_CONTROLS_H
#define FTL_CONTROLS_H
#pragma once

#include "ftlDefine.h"

#ifndef __ATLCTRLS_H__
#  error ftlControls.h requires WTL( atlctrls.h )
#endif //__ATLCTRLS_H__
#include <atlbase.h>
#include <atlimage.h>

#include <ftlWindow.h>
#include <ftlcom.h>

#include <atlcrack.h>

//控件内部处理 LButtonUp 时，可以通过以下代码向父窗体发送点击消息
/*
//参见 WTL.CBitmapButtonImpl.OnLButtonUp
bool bHover = IsHoverMode();
if(!bHover)
{
   lRet = DefWindowProc(uMsg, wParam, lParam);
}
if(::GetCapture() == m_hWnd)
{
	if(bHover && m_fPressed == 1)
	{
		::SendMessage(GetParent(), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
    }
	::ReleaseCapture();
}
*/

/***********************************************************************************************************
* Rich Edit Control ( RichEdit.H, 在标准 EDIT 控件的基础上扩展 )， 
* 为了使用RichEdit，需要先加载对应的DLL，MFC下调用AfxInitRichEdit ，
*                                        WTL下调用(可能不对) LoadLibrary(CRichEditCtrl::GetLibraryName());
*   IID_ITextServices 
*     系统在textserv.h预定义的（E_NOINTERFACE）                -- {957C25FF-0149-25FF-7895-4901FF25AC8A}
*     自定义或通过GetProcAddress(IID_ITextServices)得到的  -- 8D33F740-CF58-11CE-A89D-00AA006CADC5
*     
+		IID_ITS         	{3B032DE9-017F-25FF-AC2A-7F01FF25B02A}	-- 会返回 E_NOINTERFACE
+		IID_ITextServices	{357825FF-017F-25FF-AC2A-7F01FF25B02A}	_GUID


*     CreateTextServices 
*       riched20.dll!CTxtEdit::Init 中会通过QI查询这个接口：{13E670F5-1A5A-11CF-ABEB-00AA00B65EA1}
*       msftedit.dll!CTxtEdit::Init                         {13E670F5-1A5A-11CF-ABEB-00AA00B65EA1}
*
* 有多个版本(可能不对，需要确认)
*   1.0 -- RichEd32.dll (Win95)
*   2.0 -- RICHED20.DLL (Win95/Win98), 不再支持一些在亚洲语言版本的Rich Edit 1.0中支持的消息
*          (如 EM_CONVPOSITION、EM_SETIMECOLOR、EM_SETPUNCTUATION 等)
*   3.0 -- 
*          Rich Edit 3.0支持Hex To Unicode IME，允许用户采用一种或两种热键方式在十六进制字符和Unicode字符间互换。
*          Word 使用 Alt+x 可将光标前的文字转换成 UNICODE 对应的值；
*   4.1 -- Msftedit.DLL (WinXP SP1)
*   6.1 -- Win7 
*
*
*   CHARFORMAT2(EM_SETCHARFORMAT) -- 字符格式，如 字体、尺寸、颜色以及如粗体、斜体和保护(父窗口接受到EN_PROTECTED的通知)等
*   PARAFORMAT2(EM_SETPARAFORMAT) -- 段落格式，如 对齐、跳格、缩进和编号
*   包含嵌入的COM对象
*   消息(EM_) -- 
*     EM_EXLIMITTEXT -- 一个Rich Edit控件不可能包含超过32K字节的文本，除非你使用EM_EXLIMITTEXT消息扩展了这个限制
*   通知（EN_) -- 注意需要用 EM_SETEVENTMASK 设置能发送的通知类型, 标准控件是父窗口处理，无窗口是 ITextHost::TxNotify
*     注意：有两种类型的通知， direct and delayed
*     EN_MSGFILTER -- 过滤所有的键盘和鼠标输入，父窗体可以防止键盘和鼠标消息被处理(在ITextHost中不发送,参见MSDN)
*     EN_SELCHANGE -- 父窗口可检测当前选中内容何时被改变
*     EN_REQUESTRESIZE(无底,bottomless) -- 在任何时候若其内容大小改变，它将向其父窗体发送EN_REQUESTRESIZE通知，
*        可调整控件的大小以便它总是与其内容大小匹配，可通过发送 EM_REQUESTRESIZE 消息强制发送该通知(在 WM_SIZE 时很有用)
*   断字处理函数 -- (用户可通过 EM_SETWORDBREAKPROC 自定义) 查找单词间分隔符以及判断何处可以换行
*   
*   剪贴板格式(注册了两种)
*     1.富文本格式(CF_RTF)
*     2.RichEdit 文本与对象(CF_RETEXTOBJ)
*   流(Stream) -- 向控件传入或者传出数据
*     EM_STREAMIN -- 将数据读入控件(数据传入)，控件将重复调用应用程序的回调函数，该函数每次传入数据的一部分到缓冲区中
*     EM_STREAMOUT -- 保存控件内容(数据传出)，控件将重复写入缓冲区然后调用应用程序的回调函数。
*     
*     
* ITextHost -- 无窗口
* 
* CToolTipCtrl -- 
***********************************************************************************************************/

//声明可以和 DDX_CONTROL 一起工作的 CWindowImpl 派生类
//  如：DDX_CONTROL_IMPL(CListViewCtrl)，这样就有了一个名为 CListViewCtrl_DDX 的类，
//      功能与 CListViewCtrl 一样，但是可以被 DDX_CONTROL 接受。

#define DDX_CONTROL_IMPL(x) \
    class x##_DDX : public CWindowImpl<x##_DDX, x> \
    { public: DECLARE_EMPTY_MSG_MAP() }; 


namespace FTL
{
	FTLEXPORT class CFControlUtil
	{
	public:
		FTLINLINE static LPCTSTR GetEditNotifyCodeString(CFStringFormater& formater, DWORD iNotify, void *pv);

		//tomeTrue 或 tomFalse
		FTLINLINE static LPCTSTR GetRichEditTomBoolString(long nValue);

		//获取Story的类型（MainText、Comments、Footnotes 等)
		FTLINLINE static LPCTSTR GetRichEditStoryTypeString(long nStoryType);
		FTLINLINE static LPCTSTR GetRichEditAnimationTypeString(long nAnimation);
		FTLINLINE static LPCTSTR GetRichEditUnderLineStyleString(long nUnderLine);
		FTLINLINE static LPCTSTR GetRichEditColorString(FTL::CFStringFormater& formater, long nColor);

		//获得 RichEditCtrl 中 CHARFORMAT 的信息 -- 也可用于 Effect（部分的 CFE_ 和 CFM_）
		FTLINLINE static LPCTSTR GetCharFormatEffectAndMaskString(FTL::CFStringFormater& formater, DWORD dwMasks, LPCTSTR pszDivide = TEXT("|"));

		//获得 RichEditCtrl 中 PARAFORMAT 的信息 -- 也用于Effect?
		FTLINLINE static LPCTSTR GetParaFormatEffectAndMaskString(FTL::CFStringFormater& formater, DWORD dwMasks, LPCTSTR pszDivide = TEXT("|"));

		//获得 RichEditCtrl 中的 PropertyBits 信息(TxGetPropertyBits)
		FTLINLINE static LPCTSTR GetRichEditPropertyBits(FTL::CFStringFormater& formater, DWORD dwBits, LPCTSTR pszDivide = TEXT("|"));
	};

	//RichEdit 中的 ITextRange
	class CFTextRangeDumper : public CFInterfaceDumperBase<CFTextRangeDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFTextRangeDumper);
	public:
		FTLINLINE explicit CFTextRangeDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFTextRangeDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFTextFontDumper : public CFInterfaceDumperBase<CFTextFontDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFTextFontDumper);
	public:
		FTLINLINE explicit CFTextFontDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFTextFontDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//显示文件名路径的窗口 -- 显示成带省略号的，并且有 Tooltip
	//未写完和测试
	class CFileNameWnd : public CWindowImpl<CFileNameWnd>
	{
	public:
		BEGIN_MSG_MAP(CFileNameWnd)
			MESSAGE_HANDLER(WM_CREATE, OnCreate)
		END_MSG_MAP()
	protected:
		LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			m_tooltip.Create(m_hWnd);
			//CToolInfo ti(0, m_hWnd, m_nToolTipID, &rcClient, NULL);
			//DrawText 的 DT_CALCRECT 可计算Rect的返回值，确认是否要 Tooltip::UpdateTipText 和 Active
			//后期可用 DT_PATH_ELLIPSIS 绘制，
			//父窗体处理鼠标消息，给 Tooltip 处理： if(m_tooltip.IsWindow()) { m_tooltip.RelayEvent(...); }
		}
		
	private:
		CToolTipCtrl	m_tooltip;
	};

	//自动出现垂直滚动条的 EDIT，需要先将属性设置为： ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN
	class CFEditEx : public CWindowImpl<CFEditEx, CEdit> //<ATL::CWindowImpl>
	{
		enum  {	UWM_CHECKTEXT = WM_USER + 100 };
	public:
		CFEditEx(void);
		~CFEditEx(void);
	public:
		BEGIN_MSG_MAP_EX(CFEditEx)
			//DUMP_WINDOWS_MSG(__FILE__LINE__, DEFAULT_DUMP_FILTER_MESSAGE | DUMP_FILTER_KEYDOWN, uMsg, wParam, lParam)
			MSG_WM_CHAR(OnChar)
			MESSAGE_HANDLER(UWM_CHECKTEXT, OnCheckText)
		END_MSG_MAP()
	private:
		FTLINLINE void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		FTLINLINE LRESULT OnCheckText(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

		FTLINLINE void ShowHorizScrollBar(BOOL bShow = TRUE);
		FTLINLINE void ShowVertScrollBar(BOOL bShow = TRUE);
	private:
		BOOL m_bShowHoriz;
		BOOL m_bShowVert;
	};

    struct FScrollSkinInfo
    {
        CImage* pBtnVUp;
        CImage* pBtnVDown;
    };

    class CFSkinComboBox : public CWindowImpl<CFSkinComboBox, CComboBox>
    {
    public:
        CFSkinComboBox()
        {
            m_hWndList = NULL;
            m_pSkinInfo = NULL;
        }
        virtual ~CFSkinComboBox()
        {

        }

        BEGIN_MSG_MAP_EX(CFSkinComboBox)
#ifdef FTL_DEBUG
            //DUMP_WINDOWS_MSG(__FILE__LINE__, DEFAULT_DUMP_FILTER_MESSAGE, uMsg, wParam, lParam)
#endif 
            REFLECTED_COMMAND_CODE_HANDLER_EX(CBN_DROPDOWN, OnCbnDropDown)
            REFLECTED_COMMAND_CODE_HANDLER_EX(CBN_CLOSEUP, OnCbnCloseup)
            MSG_WM_CTLCOLORLISTBOX(OnCtlColorListBox)
            //MSG_WM_CREATE(OnCreate)
        END_MSG_MAP()

        FTLINLINE LRESULT OnCbnDropDown(UINT uNotifyCode, int nID, CWindow wndCtl);
        FTLINLINE LRESULT OnCbnCloseup(UINT uNotifyCode, int nID, CWindow wndCtl);
        FTLINLINE HBRUSH OnCtlColorListBox(CDCHandle dc, CListBox listBox);
        FTLINLINE BOOL SkinScroll(const FScrollSkinInfo* pSkinInfo);
        FTLINLINE void UnSkinScroll();
    protected:
        HWND	m_hWndList;
        const FScrollSkinInfo*    m_pSkinInfo;
    };

    //template </*class T,*/ class TBase = WTL::CScrollBar, class TWinTraits = ATL::CControlWinTraits>
    class CFSkinScrollBar : public ATL::CWindowImpl<CFSkinScrollBar, WTL::CScrollBar>
    {
    public:
        BEGIN_MSG_MAP_EX(CFSkinScrollBar)
            MSG_WM_CREATE(OnCreate)
            //MSG_WM_ERASEBKGND(OnEraseBkgnd)
            //CHAIN_MSG_MAP(CZoomScrollWindowImpl<CScrollImageViewView>) // CScrollWindowImpl<CScrollImageViewView>)
        END_MSG_MAP()
    public:
        FTLINLINE CFSkinScrollBar();
        FTLINLINE ~CFSkinScrollBar();
        FTLINLINE int OnCreate(LPCREATESTRUCT lpCreateStruct);
    private:
        FScrollSkinInfo    m_SkinInfo;
    };

    class CFSkinScrollWindow : public ATL::CWindowImpl<CFSkinScrollWindow, ATL::CWindow>
    {
    public:
        FTLINLINE CFSkinScrollWindow();
        FTLINLINE virtual ~CFSkinScrollWindow();

        BEGIN_MSG_MAP_EX(CFSkinScrollWindow)
        END_MSG_MAP()
    public:
        FTLINLINE BOOL SkinScroll(HWND hWnd, const FScrollSkinInfo* pSkinInfo);
        FTLINLINE void UnSkinScroll();
    public:
        int                 m_nScrollWidth;
        CFSkinScrollBar     m_ScrollBarVert;
        CFSkinScrollBar     m_ScrollBarHorz;
        ATL::CWindow        m_wndMiddle;
        WNDPROC             m_funOldProc;
        FTLINLINE static LRESULT CALLBACK HookWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
    };

    extern "C" FTLINLINE CFSkinScrollWindow* SkinWndScroll(HWND hWnd, const FScrollSkinInfo* pScrollSkinInfo);
    extern "C" FTLINLINE BOOL UnSkinWndScroll(HWND  hWnd);
}

#endif //FTL_CONTROLS_H

#ifndef USE_EXPORT
#  include "ftlControls.hpp"
#endif