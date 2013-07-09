#ifndef FTL_WINDOW_HPP
#define FTL_WINDOW_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlwindow.h"
#endif

#include <prsht.h>
#include <CommCtrl.h>
#include <zmouse.h>
#include <ftlConversion.h>
#include <ftlNLS.h>

namespace FTL
{

#define GET_MESSAGE_INFO_ENTRY(msg, classMsgInfo ) \
	case (msg): { StringCchCopy(m_bufInfo, _countof(m_bufInfo), classMsgInfo().GetMsgInfo(msg, TEXT(#msg), m_wParam, m_lParam)); break; }

    class CFDefaultMsgInfo
    {
    public:
		virtual LPCTSTR GetMsgInfo(UINT /*uMsg*/, LPCTSTR pszMsgName, WPARAM /*wParam*/, LPARAM /*lParam*/)
		{
			return pszMsgName;
		}
	protected:
		CFStringFormater	m_strFormater;
    };

	class CFMoveMsgInfo : public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT uMsg, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			FTLASSERT(WM_MOVE == uMsg);
			UNREFERENCED_PARAMETER(uMsg);
			
			HRESULT hr = E_FAIL;
			int xPos = (int)(short) LOWORD(lParam);   // horizontal position 
			int yPos = (int)(short) HIWORD(lParam);   // vertical position
			COM_VERIFY(m_strFormater.Format(TEXT("%s{xPos=%d, yPos=%d }"), 
				pszMsgName, xPos, yPos));
			return m_strFormater;
		}
	};
	class CFWindowPosMsgInfo : public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT uMsg, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			FTLASSERT(WM_WINDOWPOSCHANGING == uMsg || WM_WINDOWPOSCHANGED == uMsg);
			UNREFERENCED_PARAMETER(uMsg);
	  
			HRESULT hr = E_FAIL;
			WINDOWPOS* pWindowPos = (WINDOWPOS*)lParam;
			FTLASSERT(pWindowPos);
			if (pWindowPos)
			{
				CFStringFormater formaterFlags;
				COM_VERIFY(m_strFormater.Format(TEXT("%s{hWnd=0x%x, hwndInsertAfter=0x%x,")
					TEXT("Pos=(%d,%d)-(%d,%d), %dx%d, flags=0x%x(%s) }"), 
					pszMsgName, pWindowPos->hwnd, pWindowPos->hwndInsertAfter,
					pWindowPos->x, pWindowPos->y, pWindowPos->x + pWindowPos->cx, pWindowPos->y + pWindowPos->cy,
					pWindowPos->cx, pWindowPos->cy,
 					pWindowPos->flags, CFWinUtil::GetWindowPosFlagsString(formaterFlags, pWindowPos->flags)));
			}
			return m_strFormater;
		}
	};

	class CFScrollMsgInfo : public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT uMsg, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
            FTLASSERT(WM_VSCROLL == uMsg || WM_HSCROLL == uMsg);
            UNREFERENCED_PARAMETER(uMsg);

            HRESULT hr = E_FAIL;
			UINT nSBCode = LOWORD(wParam);
			UINT nPos = HIWORD(wParam);
			//nSBCode 为 SB_THUMBPOSITION 或 SB_THUMBTRACK 时, nPos 才有意义，其他时候(如 SB_ENDSCROLL)其值无意义 
			COM_VERIFY(m_strFormater.Format(TEXT("%s{nSBCode=%s, nPos=%d, lParam(HWND)=0x%x }"), 
				pszMsgName, CFWinUtil::GetScrollBarCodeString(nSBCode), nPos, lParam));
			return m_strFormater;
		}
	};

	class CFCommandMsgInfo: public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT uMsg, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			FTLASSERT(WM_COMMAND == uMsg);
			UNREFERENCED_PARAMETER(uMsg);

			BOOL bRet = FALSE;
			WORD wNotifyCode = HIWORD(wParam);
			WORD wID = LOWORD(wParam);
			HWND hWndCtrl = HWND(lParam);

			TCHAR szCommandMsgInfo[256] = {0};
			if (NULL == hWndCtrl) 
			{
				//Menu or Accelerator
				m_strFormater.Format(TEXT("%s{ Menu or Accelerator, wID=%d(0x%x)"), pszMsgName, wID, wID);
			}
			else
			{
				//Control
				TCHAR szClassName[FTL_MAX_CLASS_NAME_LENGTH] = {0};
				API_VERIFY(0 != GetClassName(hWndCtrl, szClassName, _countof(szClassName)));

				CFWinUtil::GetCommandNotifyString(hWndCtrl, wNotifyCode, szCommandMsgInfo, _countof(szCommandMsgInfo));
				m_strFormater.Format(TEXT("%s{ Control(\"%s\") wID=%d(0x%x), NotifyCode=%s(%d, 0x%x), hWndCtrl=0x%x }"),
					pszMsgName, szClassName, wID, wID, szCommandMsgInfo, wNotifyCode, wNotifyCode, hWndCtrl);
			}
			return m_strFormater;
		}
	};

	class CFNotifyMsgInfo: public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT uMsg, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			FTLASSERT(WM_NOTIFY == uMsg);
			BOOL bRet = FALSE;

			int nIdCtrl = (int)wParam;
			LPNMHDR pNmHdr = (LPNMHDR)lParam;

			TCHAR szClassName[FTL_MAX_CLASS_NAME_LENGTH] = {0};
			API_VERIFY(0 != GetClassName(pNmHdr->hwndFrom, szClassName, _countof(szClassName)));

			TCHAR szNotifyCodeString[MAX_PATH] = {0};

			m_strFormater.Format(TEXT("%s{ Control(\"%s\"), idCtrl=%d(0x%x), idFrom=%d(0x%x), code=%s(%d, 0x%x), hwndFrom=0x%x }"),
				pszMsgName, szClassName, nIdCtrl, nIdCtrl, pNmHdr->idFrom, pNmHdr->idFrom,
				CFWinUtil::GetNotifyCodeString(pNmHdr->hwndFrom, pNmHdr->code, szNotifyCodeString, _countof(szNotifyCodeString), NULL),
				pNmHdr->code, pNmHdr->code, pNmHdr->hwndFrom);

			return m_strFormater;
		}
	};

#ifdef _RICHEDIT_
	//RichEdit 有不少使用 WM_USER 的消息
	class CFRichEditCtrlMsgInfo : public CFDefaultMsgInfo
	{
		virtual LPCTSTR GetMsgInfo(UINT uMsg, LPCTSTR /*pszMsgName*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
		{
			switch (uMsg)
			{
				//TODO:不全，而且没有使用来检测
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_CANPASTE);//决定控件是否识别某种指定的格式

				//通过反复使用EM_FORMATRANGE和EM_DISPLAYBAND消息，打印Rich Text控件内容的应用程序可以实现条带化操作
				//（条带化操作指的是将输出分割为较小部分用于打印目的的操作。）
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_DISPLAYBAND);//在输出设备文本格式化完成后，将输出发送至设备

				//选择内容指的是选中字符的范围，或者表示没有字符选中时的插入点位置
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_EXGETSEL);//确定当前选中内容
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_EXLIMITTEXT);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_EXLINEFROMCHAR);//判断给定字符属于哪一行
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_EXSETSEL);//设置当前选择区域
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_FINDTEXT);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_FORMATRANGE);//对于特殊设备而言，要格式化Rich Edit控件中部分内容
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETCHARFORMAT);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETEVENTMASK);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETOLEINTERFACE);//从控件获取一个IRichEditOle接口，该接口允许它控制OLE对象
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETPARAFORMAT);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETSELTEXT);//获取Rich Edit控件中的选中文本(拷贝的字符数组中)
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_HIDESELECTION);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_PASTESPECIAL);//粘贴指定的剪贴板格式
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_REQUESTRESIZE);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SELECTIONTYPE);//获取当前选中内容的相关信息
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETBKGNDCOLOR);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETCHARFORMAT);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETEVENTMASK);//设置EventMask(如 EN_PROTECTED)
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETOLECALLBACK);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETPARAFORMAT);//

				//指定一个用于文本格式化的目标设备, 该消息对于WYSIWYG（所见即所得）模式非常有用，
				//  在该模式下应用程序采用默认打印机字体规格而非屏幕字体规格来定位文本
				//  可用于控制 RichEdit 的自动换行功能(自动换行时 Line width 为 0, TODO:是否会出现其他问题?)：
				//    http://my.oschina.net/wangjijian/blog/9416?from=rss
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETTARGETDEVICE);

				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_STREAMIN);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_STREAMOUT);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETTEXTRANGE);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_FINDWORDBREAK);//查找断字符或者确定一个字符类和断字标志位
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETOPTIONS);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETOPTIONS);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_FINDTEXTEX);//查找字符串
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETWORDBREAKPROCEX);//
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETWORDBREAKPROCEX);//自定义"断字处理函数"

				// RichEdit 2.0 messages 
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETUNDOLIMIT);//设置Undo队列的最大动作数目
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_REDO);//重做Redo队列中的下一动作
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_CANREDO);//判断是否在Redo队列中有一些动作
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETUNDONAME);//获取Undo队列中的下一动作的类型名称
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETREDONAME);//获取Redo队列中的下一动作的类型名称
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_STOPGROUPTYPING);//终止当前Undo动作的连续键入动作的组合
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETTEXTMODE);//设置文本模式或者Undo级别
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETTEXTMODE);//获取文本模式或者Undo级别
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_AUTOURLDETECT);//是否开启/关闭自动URL检测


				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_SETLANGOPTIONS);//设置IME和远东语言支持选项
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETLANGOPTIONS);//获取IME和远东语言支持选项
				HANDLE_CASE_TO_STRING_FORMATER(m_strFormater, EM_GETIMECOMPMODE);//获取当前输入方式编辑(IME)模式
				break;
			}
			//m_strFormater.Format(TEXT("%s{nFlag=%d(0x%x), Pos=[%d,%d]}"), pszMsgName, nFlags, nFlags, xPos, yPos );
			return m_strFormater;
		}
	};
#endif //_RICHEDIT_

	class CFMouseMsgInfo : public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT /*uMsg*/, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			UINT nFlags = (UINT)wParam;
			WORD xPos = LOWORD(lParam);
			WORD yPos = HIWORD(lParam);
			
			m_strFormater.Format(TEXT("%s{nFlag=%d(0x%x), Pos=[%d,%d]}"), pszMsgName, nFlags, nFlags, xPos, yPos );
			return m_strFormater;
		}
	};

	//可以在 PreTranslateMessage 中判断 WM_KEYDOWN 消息来转换处理
	class CFKeyMsgInfo : public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT /*uMsg*/, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			TCHAR nChar = (TCHAR)wParam;

			UINT nRepCnt = LOWORD(lParam);
			UINT nFlags = HIWORD(lParam);
			
			m_strFormater.Format(TEXT("%s{nChar=0x%x(%s), nRepCnt=%d, nFlags=0x%x}"), pszMsgName, 
				nChar, CFWinUtil::GetVirtualKeyString(nChar) , nRepCnt, nFlags);
			return m_strFormater;
		}
	};

	class CFPowerBroadcastMsgInfo : public  CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT /*uMsg*/, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			UNREFERENCED_PARAMETER(lParam);

			TCHAR szInfo[40] = {0};
			UINT nEvent = (UINT)wParam;
			switch (nEvent)
			{
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMQUERYSUSPEND);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMQUERYSTANDBY);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMQUERYSUSPENDFAILED);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMQUERYSTANDBYFAILED);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMSUSPEND);

				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMSTANDBY);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMRESUMECRITICAL);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMRESUMESUSPEND);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMRESUMESTANDBY);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMBATTERYLOW);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMPOWERSTATUSCHANGE);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMOEMEVENT);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_APMRESUMEAUTOMATIC);

#if (_WIN32_WINNT >= 0x0502)
				//case PBT_POWERSETTINGCHANGE:
				{
					//TODO:POWERBROADCAST_SETTING 
					HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), PBT_POWERSETTINGCHANGE);
					break;
				}
#endif // (_WIN32_WINNT >= 0x0502)
			default:
				FTLASSERT(FALSE);
				StringCchPrintf(szInfo, _countof(szInfo), TEXT("Unknown-%d"), nEvent);
				break;
			}
			m_strFormater.Format(TEXT("%s{%s}"), pszMsgName, szInfo);
			return m_strFormater;
		}
	};

	class CFIMENotifyMsgInfo : public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT /*uMsg*/, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			//UINT nCommand = (UINT)wParam;
			CFStringFormater formater;
			
			m_strFormater.Format(TEXT("%s{NotifyInfo=%s}"),pszMsgName, CFIMEUtil::GetIMENotifyInfoString(formater, wParam, lParam));
			return m_strFormater;
		}
	};

	class CFIMECompositionMsgInfo : public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT /*uMsg*/, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			//DBCS
			LPCSTR pszLastChange = (LPCSTR)wParam;
			CFStringFormater strLastChange;
			if (pszLastChange)
			{
				 CFConversion cov;
				 strLastChange.Format(TEXT("%s"), cov.MBCS_TO_TCHAR(pszLastChange));
			}
			CFStringFormater changeTypeFormater;
			m_strFormater.Format(
				TEXT("%s{pszLastChange=%s, changeType=%s}"),
				pszMsgName, 
				strLastChange, CFIMEUtil::GetCompositionValueString(changeTypeFormater, (UINT)lParam, TEXT("|"))
			);
			return m_strFormater;
		}
	};

	class CFIMECharMsgInfo : public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT /*uMsg*/, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			TCHAR nChar = (TCHAR)wParam;
			TCHAR nPrintChar = nChar;
			if(!IsCharAlphaNumeric(nPrintChar))
			{
				nPrintChar = TEXT('.');
			}

			UINT nRepCnt = LOWORD(lParam);
			UINT nFlags = HIWORD(lParam);

			m_strFormater.Format(TEXT("%s{nChar=0x%x(%c), nRepCnt=%d, nFlags=0x%x}"),pszMsgName, nChar, nPrintChar, nRepCnt, nFlags);
			return m_strFormater;
		}
	};

#if(_WIN32_WINNT >= 0x0501)
	class CFWtsSessionChangeMsgInfo : public CFDefaultMsgInfo
	{
	public:
		virtual LPCTSTR GetMsgInfo(UINT /*uMsg*/, LPCTSTR pszMsgName, WPARAM wParam, LPARAM lParam)
		{
			UNREFERENCED_PARAMETER(lParam);

			TCHAR szInfo[40] = {0};
			INT nReason = (INT)wParam;
			switch (nReason)
			{
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), WTS_CONSOLE_CONNECT);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), WTS_CONSOLE_DISCONNECT);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), WTS_REMOTE_CONNECT);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), WTS_REMOTE_DISCONNECT);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), WTS_SESSION_LOGON);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), WTS_SESSION_LOGOFF);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), WTS_SESSION_LOCK);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), WTS_SESSION_UNLOCK);
				HANDLE_CASE_TO_STRING(szInfo, _countof(szInfo), WTS_SESSION_REMOTE_CONTROL);
			default:
				FTLASSERT(FALSE);
				StringCchPrintf(szInfo, _countof(szInfo), TEXT("Unknown-%d"), nReason);
				break;
			}
			m_strFormater.Format(TEXT("%s{%s}"), pszMsgName, szInfo);
			return m_strFormater;
		}
	};
#endif /* _WIN32_WINNT >= 0x0501 */

	///////////////////////////////////////////////////////////////////////////////////////////////////
	CFRegistedMessageInfo::CFRegistedMessageInfo()
	{
		m_bInited = FALSE;

		//ZeroMemory(&m_bInited, sizeof(CFRegistedMessageMgr) - offsetofclass())
	}
	BOOL CFRegistedMessageInfo::Init()
	{
		if (!m_bInited)
		{
			m_bInited = TRUE;

			RWM_ATL_CREATE_OBJECT	= RegisterWindowMessage(TEXT("ATL_CREATE_OBJECT"));
			RWM_ATLGETCONTROL		= RegisterWindowMessage(TEXT("WM_ATLGETCONTROL"));
			RWM_ATLGETHOST			= RegisterWindowMessage(TEXT("WM_ATLGETHOST"));
			RWM_COLOROKSTRING		= RegisterWindowMessage(COLOROKSTRING);
			RWM_COMMDLG_FIND		= RegisterWindowMessage(TEXT("COMMDLG_FIND"));
			RWM_FILEOKSTRING		= RegisterWindowMessage(FILEOKSTRING);
			RWM_FINDMSGSTRING		= RegisterWindowMessage(FINDMSGSTRING);
			RWM_LBSELCHSTRING		= RegisterWindowMessage(LBSELCHSTRING);
			RWM_MSH_MOUSEWHEEL		= RegisterWindowMessage(MSH_MOUSEWHEEL);
			RWM_MSH_WHEELSUPPORT	= RegisterWindowMessage(MSH_WHEELSUPPORT);
			RWM_MSH_SCROLL_LINES	= RegisterWindowMessage(MSH_SCROLL_LINES);
			RWM_HELPMSGSTRING		= RegisterWindowMessage(HELPMSGSTRING);
			RWM_HTML_GETOBJECT		= RegisterWindowMessage(TEXT("WM_HTML_GETOBJECT"));
			RWM_SETRGBSTRING		= RegisterWindowMessage(SETRGBSTRING);
			RWM_SHAREVISTRING		= RegisterWindowMessage(SHAREVISTRING);
			RWM_TASKBARBUTTONCREATED	= RegisterWindowMessage(TEXT("TaskbarButtonCreated"));
			RWM_TASKBARCREATED		= RegisterWindowMessage(TEXT("TaskbarCreated")); //系统托盘编程完全指南 -- http://www.vckbase.com/index.php/wv/310
		}
		return m_bInited;
	}

#define HANDLE_IF_TO_STRING(buf,len,v,c) \
	if(v == c) { StringCchCopy(buf,len,TEXT(#c)); }
	

	LPCTSTR CFRegistedMessageInfo::GetMessageInfo(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		//switch .. case 需要常量 -- 而 RWM_ 等是变量
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_ATL_CREATE_OBJECT);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_ATLGETCONTROL);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_ATLGETHOST);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_COLOROKSTRING);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_COMMDLG_FIND);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_FILEOKSTRING);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_FINDMSGSTRING);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_LBSELCHSTRING);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_MSH_MOUSEWHEEL);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_MSH_WHEELSUPPORT);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_MSH_SCROLL_LINES);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_HELPMSGSTRING);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_HTML_GETOBJECT);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_SETRGBSTRING);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_SHAREVISTRING);
		HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, RWM_TASKBARBUTTONCREATED);

		//HANDLE_IF_TO_STRING(m_bufInfo, _countof(m_bufInfo), msg, XXXXXXXX);

		if(NULL == m_bufInfo[0])
		{
			StringCchPrintf(m_bufInfo, _countof(m_bufInfo), 
				TEXT("Unknown RegisterWindowMessage %d(0x%08x), wParam=%d, lParam=%d"), msg, msg, wParam, lParam);
		}
		return m_bufInfo;
	}

	__declspec(selectany) CFRegistedMessageInfo   CFMessageInfo::s_RegistedMessageInfo;

    CFMessageInfo::CFMessageInfo(UINT msg, WPARAM wParam, LPARAM lParam) 
        : CFConvertInfoT<CFMessageInfo,UINT>(msg)
        , m_wParam(wParam)
        , m_lParam(lParam)
    {
    }

    LPCTSTR CFMessageInfo::ConvertInfo()
    {
		s_RegistedMessageInfo.Init();
        if (NULL == m_bufInfo[0])
        {
            switch(m_Info)
            {
                GET_MESSAGE_INFO_ENTRY(WM_CREATE, CFDefaultMsgInfo);
				//GET_MESSAGE_INFO_ENTRY(WM_CREATE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DESTROY, CFDefaultMsgInfo);  //关闭窗口时,如果是主窗口,必须PostQuitMessage,否则进程还在运行(只是没有窗体)
                GET_MESSAGE_INFO_ENTRY(WM_MOVE, CFMoveMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SIZE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_ACTIVATE, CFDefaultMsgInfo); //窗口被激活或失去激活状态, 可用于查看窗体是否最小化，及前一窗体
                GET_MESSAGE_INFO_ENTRY(WM_SETFOCUS, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_KILLFOCUS, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_ENABLE, CFDefaultMsgInfo); //将禁止或激活时
                GET_MESSAGE_INFO_ENTRY(WM_SETREDRAW, CFDefaultMsgInfo); //设置窗口是否能重画 
                GET_MESSAGE_INFO_ENTRY(WM_SETTEXT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_GETTEXT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_GETTEXTLENGTH, CFDefaultMsgInfo); //得到与一个窗口有关的文本的长度（不包含空字符）
                GET_MESSAGE_INFO_ENTRY(WM_PAINT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_CLOSE, CFDefaultMsgInfo);
#ifndef _WIN32_WCE
                GET_MESSAGE_INFO_ENTRY(WM_QUERYENDSESSION, CFDefaultMsgInfo); //当用户选择结束对话框或程序自己调用ExitWindows函数
                GET_MESSAGE_INFO_ENTRY(WM_QUERYOPEN, CFDefaultMsgInfo);     //当用户窗口恢复以前的大小位置时，把此消息发送给某个图标
                //当系统进程发出WM_QUERYENDSESSION消息后，此消息发送给应用程序，通知它对话是否结束,
                //一半来说，过滤了 WM_CLOSE 消息的程序都需要处理这个消息，否则可能导致系统无法关闭
                // if(wParam || (lParam & ENDSESSION_LOGOFF)) { OnClose(); } 
                GET_MESSAGE_INFO_ENTRY(WM_ENDSESSION, CFDefaultMsgInfo);
#endif
                GET_MESSAGE_INFO_ENTRY(WM_QUIT, CFDefaultMsgInfo);
				//当窗口背景必须被擦除时（例在窗口改变大小时),如果实现中擦除了背景，则返回TRUE；如返回FALSE，则 PAINTSTRUCT::fErase 会为TRUE
				//缺省实现是使用窗体类结构中的Brush擦除
                GET_MESSAGE_INFO_ENTRY(WM_ERASEBKGND, CFDefaultMsgInfo); 
                GET_MESSAGE_INFO_ENTRY(WM_SYSCOLORCHANGE, CFDefaultMsgInfo);    //当系统颜色改变时，发送此消息给所有顶级窗口
                GET_MESSAGE_INFO_ENTRY(WM_SHOWWINDOW, CFDefaultMsgInfo);

#ifndef WM_CTLCOLOR
#  define WM_CTLCOLOR                             0x0019
#endif
                //子窗口总在绘制客户区之前，将其发送给父窗口，委托父窗口为自己准备一个画刷，同时父窗口（如对话框）也给自己发送该消息(为什么？)
                GET_MESSAGE_INFO_ENTRY(WM_CTLCOLOR, CFDefaultMsgInfo);  

                GET_MESSAGE_INFO_ENTRY(WM_WININICHANGE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DEVMODECHANGE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_ACTIVATEAPP, CFDefaultMsgInfo);//属于另一App的窗体将激活时
                GET_MESSAGE_INFO_ENTRY(WM_FONTCHANGE, CFDefaultMsgInfo);    //当系统的字体资源库变化时发送此消息给所有顶级窗口
                GET_MESSAGE_INFO_ENTRY(WM_TIMECHANGE, CFDefaultMsgInfo);    //当系统的时间变化时发送此消息给所有顶级窗口
                GET_MESSAGE_INFO_ENTRY(WM_CANCELMODE, CFDefaultMsgInfo);    //取消系统模式时
                GET_MESSAGE_INFO_ENTRY(WM_SETCURSOR, CFDefaultMsgInfo);     //如果鼠标引起光标在某个窗口中移动且鼠标输入没有被捕获时，就发消息给某个窗口
                GET_MESSAGE_INFO_ENTRY(WM_MOUSEACTIVATE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_CHILDACTIVATE, CFDefaultMsgInfo); //送此消息给MDI子窗口当用户点击此窗口的标题栏，或当窗口被激活，移动，改变大小
                GET_MESSAGE_INFO_ENTRY(WM_QUEUESYNC, CFDefaultMsgInfo);     //此消息由基于计算机的"训练?"程序发送，通过 WH_JOURNALPALYBACK 的hook程序分离出用户输入消息
                GET_MESSAGE_INFO_ENTRY(WM_GETMINMAXINFO, CFDefaultMsgInfo); //处理该消息可以得到一个改变最大和最小的窗口缺省值的机会

                GET_MESSAGE_INFO_ENTRY(WM_PAINTICON, CFDefaultMsgInfo);     //发送给最小化窗口当它图标将要被重画
                GET_MESSAGE_INFO_ENTRY(WM_ICONERASEBKGND, CFDefaultMsgInfo);//此消息发送给某个最小化窗口，仅当它在画图标前它的背景必须被重画
                GET_MESSAGE_INFO_ENTRY(WM_NEXTDLGCTL, CFDefaultMsgInfo);    //切换到下一个控件( 模拟Tab 或 禁用一个Focus的控件时 需要手工发送这个?)
                GET_MESSAGE_INFO_ENTRY(WM_SPOOLERSTATUS, CFDefaultMsgInfo); //每当打印管理列队增加或减少一条作业时发出此消息
                GET_MESSAGE_INFO_ENTRY(WM_DRAWITEM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MEASUREITEM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DELETEITEM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_VKEYTOITEM, CFDefaultMsgInfo);    //由一个LBS_WANTKEYBOARDINPUT风格的发出给它的所有者来响应WM_KEYDOWN消息
                GET_MESSAGE_INFO_ENTRY(WM_CHARTOITEM, CFDefaultMsgInfo);    //由一个LBS_WANTKEYBOARDINPUT风格的列表框发送给他的所有者来响应WM_CHAR消息
                GET_MESSAGE_INFO_ENTRY(WM_SETFONT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_GETFONT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SETHOTKEY, CFDefaultMsgInfo);     //应用程序发送此消息让一个窗口与一个热键相关连
                GET_MESSAGE_INFO_ENTRY(WM_GETHOTKEY, CFDefaultMsgInfo);     //应用程序发送此消息来判断热键与某个窗口是否有关联
                GET_MESSAGE_INFO_ENTRY(WM_QUERYDRAGICON, CFDefaultMsgInfo); //送给最小化窗口，当此窗口将要被拖放而它的类中没有定义图标，应用程序能返回一个图标或光标的句柄，
                //当用户拖放图标时系统显示这个图标或光标
                GET_MESSAGE_INFO_ENTRY(WM_COMPAREITEM, CFDefaultMsgInfo);   //发送此消息来判定combobox或listbox新增加的项的相对位置
#if(WINVER >= 0x0500)
                GET_MESSAGE_INFO_ENTRY(WM_GETOBJECT, CFDefaultMsgInfo);
#endif
                GET_MESSAGE_INFO_ENTRY(WM_COMPACTING, CFDefaultMsgInfo);    //显示内存已经很少了
                GET_MESSAGE_INFO_ENTRY(WM_COMMNOTIFY, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_WINDOWPOSCHANGING, CFWindowPosMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_WINDOWPOSCHANGED, CFWindowPosMsgInfo);	//当窗体的位置、大小或位置变化以后
                GET_MESSAGE_INFO_ENTRY(WM_POWER, CFDefaultMsgInfo);         //当系统将要进入暂停状态时发送此消息
                GET_MESSAGE_INFO_ENTRY(WM_COPYDATA, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_CANCELJOURNAL, CFDefaultMsgInfo); //当某个用户取消程序日志激活状态，提交此消息给程序
#if(WINVER >= 0x0400)
				GET_MESSAGE_INFO_ENTRY(WM_NOTIFY, CFNotifyMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_INPUTLANGCHANGEREQUEST, CFDefaultMsgInfo);    //当用户选择某种输入语言，或输入语言的热键改变
                GET_MESSAGE_INFO_ENTRY(WM_INPUTLANGCHANGE, CFDefaultMsgInfo);   //当平台现场已经被改变后发送此消息给受影响的最顶级窗口
                GET_MESSAGE_INFO_ENTRY(WM_TCARD, CFDefaultMsgInfo);         //当程序已经初始化windows帮助例程时发送此消息给应用程序
                GET_MESSAGE_INFO_ENTRY(WM_HELP, CFDefaultMsgInfo);          //此消息显示用户按下了F1，如果某个菜单是激活的，就发送此消息个此窗口关联的菜单，
                //  否则就发送给有焦点的窗口，如果当前都没有焦点，就把此消息发送给当前激活的窗口
                GET_MESSAGE_INFO_ENTRY(WM_USERCHANGED, CFDefaultMsgInfo);   //当用户已经登入或退出后发送此消息给所有的窗口，当用户登入或退出时系统更新用户的具体
                //  设置信息，在用户更新设置时系统马上发送此消息
                GET_MESSAGE_INFO_ENTRY(WM_NOTIFYFORMAT, CFDefaultMsgInfo);  //通过此消息来判断控件是使用ANSI还是UNICODE结构
                GET_MESSAGE_INFO_ENTRY(WM_CONTEXTMENU, CFDefaultMsgInfo);   //当用户某个窗口中点击了一下右键就发送此消息给这个窗口
                GET_MESSAGE_INFO_ENTRY(WM_STYLECHANGING, CFDefaultMsgInfo); //当调用SETWINDOWLONG函数 将要改变 一个或多个 窗口的风格时发送此消息给那个窗口
                GET_MESSAGE_INFO_ENTRY(WM_STYLECHANGED, CFDefaultMsgInfo);  //当调用SETWINDOWLONG函数 改变 一个或多个窗口的风格后发送此消息给那个窗口
                GET_MESSAGE_INFO_ENTRY(WM_DISPLAYCHANGE, CFDefaultMsgInfo); //当显示器的分辨率改变后发送此消息给所有的窗口
                GET_MESSAGE_INFO_ENTRY(WM_GETICON, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SETICON, CFDefaultMsgInfo);
#endif
                GET_MESSAGE_INFO_ENTRY(WM_NCCREATE, CFDefaultMsgInfo);      //当某个窗口第一次被创建时，此消息在WM_CREATE消息发送前发送
                GET_MESSAGE_INFO_ENTRY(WM_NCDESTROY, CFDefaultMsgInfo);     //此消息通知某个窗口，非客户区正在销毁
                
                //需要计算窗口客户区的大小和位置时发送，通过处理该消息，可以在窗口大小或位置改变时控制客户区的内容     
                GET_MESSAGE_INFO_ENTRY(WM_NCCALCSIZE, CFDefaultMsgInfo);    
                GET_MESSAGE_INFO_ENTRY(WM_NCHITTEST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCPAINT, CFDefaultMsgInfo);       //程序发送此消息给某个窗口当它（窗口）的框架必须被绘制时
                GET_MESSAGE_INFO_ENTRY(WM_NCACTIVATE, CFDefaultMsgInfo);    //此消息发送给某个窗口 仅当它的非客户区需要被改变来显示是激活还是非激活状态
                GET_MESSAGE_INFO_ENTRY(WM_GETDLGCODE, CFDefaultMsgInfo);    //发送此消息给某个与对话框程序关联的控件，widdows控制方位键和TAB键使输入进入此控件
                //  通过响应WM_GETDLGCODE消息，应用程序可以把他当成一个特殊的输入控件并能处理它
#ifndef _WIN32_WCE
                GET_MESSAGE_INFO_ENTRY(WM_SYNCPAINT, CFDefaultMsgInfo);
#endif
                GET_MESSAGE_INFO_ENTRY(WM_NCMOUSEMOVE, CFDefaultMsgInfo);   //非客户区内移动时发送此消息给这个窗口
                GET_MESSAGE_INFO_ENTRY(WM_NCLBUTTONDOWN, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCLBUTTONUP, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCLBUTTONDBLCLK, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCRBUTTONDOWN, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCRBUTTONUP, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCRBUTTONDBLCLK, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCMBUTTONDOWN, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCMBUTTONUP, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCMBUTTONDBLCLK, CFDefaultMsgInfo);

#if(_WIN32_WINNT >= 0x0500)
                GET_MESSAGE_INFO_ENTRY(WM_NCXBUTTONDOWN, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCXBUTTONUP, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCXBUTTONDBLCLK, CFDefaultMsgInfo);
#endif

#if(_WIN32_WINNT >= 0x0501)
                GET_MESSAGE_INFO_ENTRY(WM_INPUT, CFDefaultMsgInfo);
#endif
                //GET_MESSAGE_INFO_ENTRY(WM_KEYFIRST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_KEYDOWN, CFKeyMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_KEYUP, CFKeyMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_CHAR, CFKeyMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DEADCHAR, CFKeyMsgInfo); //当用 translatemessage 函数翻译WM_KEYUP消息时发送此消息给拥有焦点的窗口
                GET_MESSAGE_INFO_ENTRY(WM_SYSKEYDOWN, CFKeyMsgInfo);    //当用户按住ALT键同时按下其它键时提交此消息给拥有焦点的窗口
                GET_MESSAGE_INFO_ENTRY(WM_SYSKEYUP, CFKeyMsgInfo);      //当用户释放一个键同时ALT 键还按着时提交此消息给拥有焦点的窗口
                GET_MESSAGE_INFO_ENTRY(WM_SYSCHAR, CFKeyMsgInfo);       //当WM_SYSKEYDOWN消息被TRANSLATEMESSAGE函数翻译后提交此消息给拥有焦点的窗
                GET_MESSAGE_INFO_ENTRY(WM_SYSDEADCHAR, CFKeyMsgInfo);   //
#if(_WIN32_WINNT >= 0x0501)
                GET_MESSAGE_INFO_ENTRY(WM_UNICHAR, CFDefaultMsgInfo);
                //GET_MESSAGE_INFO_ENTRY(WM_KEYLAST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(UNICODE_NOCHAR, CFDefaultMsgInfo);
#else
                GET_MESSAGE_INFO_ENTRY(WM_KEYLAST, CFDefaultMsgInfo);
#endif

#if(WINVER >= 0x0400)
#endif
                GET_MESSAGE_INFO_ENTRY(WM_INITDIALOG, CFDefaultMsgInfo); //在一个对话框程序被显示前发送此消息给它，通常用此消息初始化控件和执行其它任务
                GET_MESSAGE_INFO_ENTRY(WM_COMMAND, CFCommandMsgInfo);

                //可用于屏蔽屏幕保护和显示器节电模式(SC_SCREENSAVE/SC_MONITORPOWER),返回0
                GET_MESSAGE_INFO_ENTRY(WM_SYSCOMMAND, CFDefaultMsgInfo);    //当用户选择窗口菜单的一条命令或当用户选择最大化或最小化时那个窗口会收到此消息

                GET_MESSAGE_INFO_ENTRY(WM_TIMER, CFDefaultMsgInfo);     //发生了定时器事件
                GET_MESSAGE_INFO_ENTRY(WM_HSCROLL, CFScrollMsgInfo);   //水平滚动条产生一个滚动事件
                GET_MESSAGE_INFO_ENTRY(WM_VSCROLL, CFScrollMsgInfo);   //垂直滚动条产生一个滚动事件
                GET_MESSAGE_INFO_ENTRY(WM_INITMENU, CFDefaultMsgInfo);  //当一个菜单将要被激活时发送此消息，它发生在用户菜单条中的某项或按下某个菜单键，
                //  它允许程序在显示前更改菜单
                GET_MESSAGE_INFO_ENTRY(WM_INITMENUPOPUP, CFDefaultMsgInfo);

                GET_MESSAGE_INFO_ENTRY(WM_SYSTIMER, CFDefaultMsgInfo);  //UnDocument Message

                GET_MESSAGE_INFO_ENTRY(WM_MENUSELECT, CFDefaultMsgInfo); //当用户选择一条菜单项时发送此消息给菜单的所有者（一般是窗口）
                GET_MESSAGE_INFO_ENTRY(WM_MENUCHAR, CFDefaultMsgInfo);  //当菜单已被激活用户按下了某个键（不同于加速键），发送此消息给菜单的所有者
                GET_MESSAGE_INFO_ENTRY(WM_ENTERIDLE, CFDefaultMsgInfo); //当一个模态对话框或菜单进入空载状态时(处理完一条或几条先前的消息后没有消息它的列队中等待)发送此消息给它的所有者，
#if(WINVER >= 0x0500)
                GET_MESSAGE_INFO_ENTRY(WM_MENURBUTTONUP, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MENUDRAG, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MENUGETOBJECT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_UNINITMENUPOPUP, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MENUCOMMAND, CFDefaultMsgInfo);
#  if(_WIN32_WINNT >= 0x0500)
                GET_MESSAGE_INFO_ENTRY(WM_CHANGEUISTATE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_UPDATEUISTATE, CFDefaultMsgInfo); //TODO: 可能导致没有 WM_PAINT 的重绘? 参见 WTL.CBitmapButtonImpl.OnUpdateUiState 
                GET_MESSAGE_INFO_ENTRY(WM_QUERYUISTATE, CFDefaultMsgInfo);
#  endif //_WIN32_WINNT >= 0x0500

#endif //WINVER >= 0x0500

				//控件将要被绘制前发送此消息给它的父窗口 -- 可以设置控件的文本、背景(返回的画刷句柄)等颜色
                GET_MESSAGE_INFO_ENTRY(WM_CTLCOLORMSGBOX, CFDefaultMsgInfo);    //在windows绘制消息框前发送此消息给消息框的所有者窗口，通过响应这条消息，所有者窗口可以
                //  通过使用给定的相关显示设备的句柄来设置消息框的文本和背景颜色
                GET_MESSAGE_INFO_ENTRY(WM_CTLCOLOREDIT, CFDefaultMsgInfo);      //可以设置编辑框的文本和背景颜色
                GET_MESSAGE_INFO_ENTRY(WM_CTLCOLORLISTBOX, CFDefaultMsgInfo);   //可以设置列表框的文本和背景颜色
                GET_MESSAGE_INFO_ENTRY(WM_CTLCOLORBTN, CFDefaultMsgInfo);       //可以设置按纽的文本和背景颜色
                GET_MESSAGE_INFO_ENTRY(WM_CTLCOLORDLG, CFDefaultMsgInfo);       //可以设置对话框的文本背景颜色(return CreateSolidBrush(xxx);
                GET_MESSAGE_INFO_ENTRY(WM_CTLCOLORSCROLLBAR, CFDefaultMsgInfo); //可以设置滚动条的背景颜色
                GET_MESSAGE_INFO_ENTRY(WM_CTLCOLORSTATIC, CFDefaultMsgInfo);    //可以设置静态控件的文本和背景颜色

                GET_MESSAGE_INFO_ENTRY(MN_GETHMENU, CFDefaultMsgInfo);

                //GET_MESSAGE_INFO_ENTRY(WM_MOUSEFIRST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MOUSEMOVE, CFMouseMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_LBUTTONDOWN, CFMouseMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_LBUTTONUP, CFMouseMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_LBUTTONDBLCLK, CFMouseMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RBUTTONDOWN, CFMouseMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RBUTTONUP, CFMouseMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RBUTTONDBLCLK, CFMouseMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MBUTTONDOWN, CFMouseMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MBUTTONUP, CFMouseMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MBUTTONDBLCLK, CFMouseMsgInfo);

#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
                GET_MESSAGE_INFO_ENTRY(WM_MOUSEWHEEL, CFDefaultMsgInfo);        //当鼠标轮子转动时发送此消息个当前有焦点的控件
#endif

#if (_WIN32_WINNT >= 0x0500)                
                GET_MESSAGE_INFO_ENTRY(WM_XBUTTONDOWN, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_XBUTTONUP, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_XBUTTONDBLCLK, CFDefaultMsgInfo);
#endif

#if (_WIN32_WINNT >= 0x0600)
                GET_MESSAGE_INFO_ENTRY(WM_MOUSEHWHEEL, CFDefaultMsgInfo);
#endif
                //GET_MESSAGE_INFO_ENTRY(WM_MOUSELAST, CFDefaultMsgInfo);

                GET_MESSAGE_INFO_ENTRY(WM_PARENTNOTIFY, CFDefaultMsgInfo); //当MDI子窗口被创建或被销毁，或用户按了一下鼠标键而光标在子窗口上时发送此消息给它的父窗口
                GET_MESSAGE_INFO_ENTRY(WM_ENTERMENULOOP, CFDefaultMsgInfo); //发送此消息通知应用程序的主窗口 已经进入了菜单循环模式 -- ?
                GET_MESSAGE_INFO_ENTRY(WM_EXITMENULOOP, CFDefaultMsgInfo);  //发送此消息通知应用程序的主窗口 已经退出了菜单循环模式 -- ?

#if(WINVER >= 0x0400)
                GET_MESSAGE_INFO_ENTRY(WM_NEXTMENU, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SIZING, CFDefaultMsgInfo);        //当用户正在调整窗口大小时发送此消息给窗口；通过此消息应用程序可以监视和修改窗口大小和位置
                GET_MESSAGE_INFO_ENTRY(WM_CAPTURECHANGED, CFDefaultMsgInfo);//窗口失去捕获的鼠标时
                GET_MESSAGE_INFO_ENTRY(WM_MOVING, CFDefaultMsgInfo);        //当用户在移动窗口时发送此消息
#endif

#if(WINVER >= 0x0400)
                GET_MESSAGE_INFO_ENTRY(WM_POWERBROADCAST, CFPowerBroadcastMsgInfo);//送给应用程序来通知它有关电源管理事件
                GET_MESSAGE_INFO_ENTRY(WM_DEVICECHANGE, CFDefaultMsgInfo);  //当设备的硬件配置改变时发送此消息给应用程序或设备驱动程序
#endif

                GET_MESSAGE_INFO_ENTRY(WM_MDICREATE, CFDefaultMsgInfo);     //要求创建一个MDI 子窗口
                GET_MESSAGE_INFO_ENTRY(WM_MDIDESTROY, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MDIACTIVATE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MDIRESTORE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MDINEXT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MDIMAXIMIZE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MDITILE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MDICASCADE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MDIICONARRANGE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MDIGETACTIVE, CFDefaultMsgInfo);


                GET_MESSAGE_INFO_ENTRY(WM_MDISETMENU, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_ENTERSIZEMOVE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_EXITSIZEMOVE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DROPFILES, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_MDIREFRESHMENU, CFDefaultMsgInfo);

				GET_MESSAGE_INFO_ENTRY(WM_IME_STARTCOMPOSITION, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(WM_IME_ENDCOMPOSITION, CFDefaultMsgInfo);
				//通知程序录入字串的改变，通常在 if(lParam & GCS_RESULTSTR) 时调用 ImmGetCompositionString 检索指示串或数据
				GET_MESSAGE_INFO_ENTRY(WM_IME_COMPOSITION, CFIMECompositionMsgInfo);	
#if(WINVER >= 0x0400)
                GET_MESSAGE_INFO_ENTRY(WM_IME_SETCONTEXT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_IME_NOTIFY, CFIMENotifyMsgInfo);		//通知程序IME视窗状态的通用变化
                GET_MESSAGE_INFO_ENTRY(WM_IME_CONTROL, CFDefaultMsgInfo);		//指示IME窗体执行命令
                GET_MESSAGE_INFO_ENTRY(WM_IME_COMPOSITIONFULL, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_IME_SELECT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_IME_CHAR, CFIMECharMsgInfo);			//IME获取到转换后的结果
#endif

#if(WINVER >= 0x0500)
                GET_MESSAGE_INFO_ENTRY(WM_IME_REQUEST, CFDefaultMsgInfo);
#endif

#if(WINVER >= 0x0400)
                GET_MESSAGE_INFO_ENTRY(WM_IME_KEYDOWN, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_IME_KEYUP, CFDefaultMsgInfo);
#endif

#if((_WIN32_WINNT >= 0x0400) || (WINVER >= 0x0500))
                GET_MESSAGE_INFO_ENTRY(WM_MOUSEHOVER, CFDefaultMsgInfo); //

				//默认情况下，鼠标移开消息是不会发送的，需要通过 TrackMouseEvent( {TME_LEAVE|TME_CANCEL } 注册
                GET_MESSAGE_INFO_ENTRY(WM_MOUSELEAVE, CFDefaultMsgInfo); //鼠标移开时，需要用 ON_MESSAGE 响应
#endif

#if(WINVER >= 0x0500)
                GET_MESSAGE_INFO_ENTRY(WM_NCMOUSEHOVER, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_NCMOUSELEAVE, CFDefaultMsgInfo);
#endif


#if(_WIN32_WINNT >= 0x0501)
                GET_MESSAGE_INFO_ENTRY(WM_WTSSESSION_CHANGE, CFWtsSessionChangeMsgInfo); //Session改变(比如锁定、解锁、退出登录等)
                GET_MESSAGE_INFO_ENTRY(WM_TABLET_FIRST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_TABLET_LAST, CFDefaultMsgInfo);
#endif

                GET_MESSAGE_INFO_ENTRY(WM_CUT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_COPY, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_PASTE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_CLEAR, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_UNDO, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RENDERFORMAT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RENDERALLFORMATS, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DESTROYCLIPBOARD, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DRAWCLIPBOARD, CFDefaultMsgInfo); //当剪贴板的内容变化时发送此消息给剪贴板观察链的第一个窗口；它允许用剪贴板观察窗口来显示剪贴板的新内容
                GET_MESSAGE_INFO_ENTRY(WM_PAINTCLIPBOARD, CFDefaultMsgInfo);//当剪贴板包含CF_OWNERDIPLAY格式的数据并且剪贴板观察窗口的客户区需要重画
                GET_MESSAGE_INFO_ENTRY(WM_VSCROLLCLIPBOARD, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SIZECLIPBOARD, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_ASKCBFORMATNAME, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_CHANGECBCHAIN, CFDefaultMsgInfo); //当一个窗口从剪贴板观察链中移去时发送此消息给剪贴板观察链的第一个窗口
                GET_MESSAGE_INFO_ENTRY(WM_HSCROLLCLIPBOARD, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_QUERYNEWPALETTE, CFDefaultMsgInfo);   //此消息能使窗口在收到焦点时同时有机会实现他的逻辑调色板
                GET_MESSAGE_INFO_ENTRY(WM_PALETTEISCHANGING, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_PALETTECHANGED, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_HOTKEY, CFDefaultMsgInfo);        //当用户按下由REGISTERHOTKEY函数注册的热键时提交此消息

#if(WINVER >= 0x0400)
                GET_MESSAGE_INFO_ENTRY(WM_PRINT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_PRINTCLIENT, CFDefaultMsgInfo);
#endif

#if(_WIN32_WINNT >= 0x0500)
                GET_MESSAGE_INFO_ENTRY(WM_APPCOMMAND, CFDefaultMsgInfo);
#endif

#if(_WIN32_WINNT >= 0x0501)
                GET_MESSAGE_INFO_ENTRY(WM_THEMECHANGED, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_CLIPBOARDUPDATE, CFDefaultMsgInfo); // 0x031D          
#endif /* _WIN32_WINNT >= 0x0501 */

#if(_WIN32_WINNT >= 0x0600)
                GET_MESSAGE_INFO_ENTRY(WM_DWMCOMPOSITIONCHANGED, CFDefaultMsgInfo);          //0x031E
                GET_MESSAGE_INFO_ENTRY(WM_DWMNCRENDERINGCHANGED, CFDefaultMsgInfo);          //0x031F
                GET_MESSAGE_INFO_ENTRY(WM_DWMCOLORIZATIONCOLORCHANGED, CFDefaultMsgInfo);    //0x0320
                GET_MESSAGE_INFO_ENTRY(WM_DWMWINDOWMAXIMIZEDCHANGE, CFDefaultMsgInfo);       //0x0321
#endif /* _WIN32_WINNT >= 0x0600 */

#if(WINVER >= 0x0600)
                GET_MESSAGE_INFO_ENTRY(WM_GETTITLEBARINFOEX, CFDefaultMsgInfo);              //0x033F
#endif /* WINVER >= 0x0600 */

#if(WINVER >= 0x0400)
                GET_MESSAGE_INFO_ENTRY(WM_HANDHELDFIRST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_HANDHELDLAST, CFDefaultMsgInfo);
#ifdef __AFXPRIV_H__
                //#  ifdef USING_MFC
                GET_MESSAGE_INFO_ENTRY(WM_QUERYAFXWNDPROC, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SIZEPARENT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SETMESSAGESTRING, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_IDLEUPDATECMDUI, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_INITIALUPDATE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_COMMANDHELP, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_HELPHITTEST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_EXITHELPMODE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RECALCPARENT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SIZECHILD, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_KICKIDLE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_QUERYCENTERWND, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DISABLEMODAL, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_FLOATSTATUS, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_ACTIVATETOPLEVEL, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RESERVED_036F, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RESERVED_0370, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RESERVED_0371, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RESERVED_0372, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SOCKET_NOTIFY, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_SOCKET_DEAD, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_POPMESSAGESTRING, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_HELPPROMPTADDR, CFDefaultMsgInfo);
                //GET_MESSAGE_INFO_ENTRY(WM_OCC_LOADFROMSTREAM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_OCC_LOADFROMSTORAGE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_OCC_INITNEW, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_OCC_LOADFROMSTREAM_EX, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_OCC_LOADFROMSTORAGE_EX, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_QUEUE_SENTINEL, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RESERVED_037C, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RESERVED_037D, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_RESERVED_037E, CFDefaultMsgInfo);
                //将消息转发给其他窗体先进行处理 -- 典型用法：PreTranslateMessage 中 将鼠标、键盘消息给子窗体优先处理
                //BOOL bRet = FALSE; 
                //if(pMsg->hwnd == m_hWnd || IsChild(pMsg->hwnd))
                //  {	bRet = (BOOL)SendMessage(WM_FORWARDMSG, 0, (LPARAM)pMsg);} return bRet;
                GET_MESSAGE_INFO_ENTRY(WM_FORWARDMSG, CFDefaultMsgInfo);
#  else
                GET_MESSAGE_INFO_ENTRY(WM_AFXFIRST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_AFXLAST, CFDefaultMsgInfo);

#  endif //end __AFXPRIV_H__

#endif

                GET_MESSAGE_INFO_ENTRY(WM_PENWINFIRST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_PENWINLAST, CFDefaultMsgInfo);
#  ifdef _DDEHEADER_INCLUDED_
                GET_MESSAGE_INFO_ENTRY(WM_DDE_INITIATE, CFDefaultMsgInfo);  //一个DDE客户程序提交此消息开始一个与服务器程序的会话来响应那个指定的程序和主题名
                GET_MESSAGE_INFO_ENTRY(WM_DDE_TERMINATE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DDE_ADVISE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DDE_UNADVISE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DDE_ACK, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DDE_DATA, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DDE_REQUEST, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DDE_POKE, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(WM_DDE_EXECUTE, CFDefaultMsgInfo);
                //GET_MESSAGE_INFO_ENTRY(WM_DDE_LAST, CFDefaultMsgInfo);
#  endif
#if(WINVER >= 0x0400)
                GET_MESSAGE_INFO_ENTRY(WM_APP, CFDefaultMsgInfo);
#endif
                GET_MESSAGE_INFO_ENTRY(DM_GETDEFID, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(DM_SETDEFID, CFDefaultMsgInfo);
#if(WINVER >= 0x0400)
                GET_MESSAGE_INFO_ENTRY(DM_REPOSITION, CFDefaultMsgInfo);
#endif

				//Edit Control Messages
				GET_MESSAGE_INFO_ENTRY(EM_GETSEL, CFDefaultMsgInfo); //最多获取到32KB(16Bit)的选中文本(RichEdit中使用 EM_EXGETSEL 来突破这个限制)
				GET_MESSAGE_INFO_ENTRY(EM_SETSEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETRECT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SETRECT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SETRECTNP, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SCROLL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_LINESCROLL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SCROLLCARET, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETMODIFY, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SETMODIFY, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETLINECOUNT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_LINEINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SETHANDLE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETHANDLE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETTHUMB, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_LINELENGTH, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_REPLACESEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETLINE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_LIMITTEXT, CFDefaultMsgInfo); //EM_SETLIMITTEXT
				GET_MESSAGE_INFO_ENTRY(EM_CANUNDO, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_UNDO, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_FMTLINES, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_LINEFROMCHAR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SETTABSTOPS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SETPASSWORDCHAR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_EMPTYUNDOBUFFER, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETFIRSTVISIBLELINE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SETREADONLY, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SETWORDBREAKPROC, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETWORDBREAKPROC, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETPASSWORDCHAR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SETMARGINS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETMARGINS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETLIMITTEXT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_POSFROMCHAR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_CHARFROMPOS, CFDefaultMsgInfo);
#if(WINVER >= 0x0500)
				GET_MESSAGE_INFO_ENTRY(EM_SETIMESTATUS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETIMESTATUS, CFDefaultMsgInfo);
#endif /* WINVER >= 0x0500 */
				
#if (_WIN32_WINNT >= 0x0501)
				GET_MESSAGE_INFO_ENTRY(EM_SETCUEBANNER, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETCUEBANNER, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_SHOWBALLOONTIP, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_HIDEBALLOONTIP, CFDefaultMsgInfo);
#if _WIN32_WINNT >= 0x0600
				GET_MESSAGE_INFO_ENTRY(EM_SETHILITE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(EM_GETHILITE, CFDefaultMsgInfo);
#endif //_WIN32_WINNT >= 0x0600
#endif //_WIN32_WINNT >= 0x0501

				//Button Control Messages
				GET_MESSAGE_INFO_ENTRY(BM_GETCHECK, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BM_SETCHECK, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BM_GETSTATE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BM_SETSTATE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BM_SETSTYLE, CFDefaultMsgInfo);
#if(WINVER >= 0x0400)
				GET_MESSAGE_INFO_ENTRY(BM_CLICK, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BM_GETIMAGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BM_SETIMAGE, CFDefaultMsgInfo);
#endif /* WINVER >= 0x0400 */

#if(WINVER >= 0x0600)
				GET_MESSAGE_INFO_ENTRY(BM_SETDONTCLICK, CFDefaultMsgInfo);
#endif /* WINVER >= 0x0600 */
				
#if (_WIN32_WINNT >= 0x0501)
				//Button control messages
				GET_MESSAGE_INFO_ENTRY(BCM_GETIDEALSIZE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_SETIMAGELIST, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_GETIMAGELIST, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_SETTEXTMARGIN, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_GETTEXTMARGIN, CFDefaultMsgInfo);
#endif // _WIN32_WINNT >= 0x0501

#if _WIN32_WINNT >= 0x0600
				GET_MESSAGE_INFO_ENTRY(BCM_SETDROPDOWNSTATE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_SETSPLITINFO, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_GETSPLITINFO, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_SETNOTE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_GETNOTE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_GETNOTELENGTH, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(BCM_SETSHIELD, CFDefaultMsgInfo);
#endif // _WIN32_WINNT >= 0x0600

				//Static Control Mesages
				GET_MESSAGE_INFO_ENTRY(STM_SETICON, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(STM_GETICON, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(STM_SETIMAGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(STM_GETIMAGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(STM_MSGMAX, CFDefaultMsgInfo);

				//Listbox messages
				GET_MESSAGE_INFO_ENTRY(LB_ADDSTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_INSERTSTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_DELETESTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SELITEMRANGEEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_RESETCONTENT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETSEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETCURSEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETSEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETCURSEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETTEXT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETTEXTLEN, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETCOUNT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SELECTSTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_DIR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETTOPINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_FINDSTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETSELCOUNT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETSELITEMS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETTABSTOPS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETHORIZONTALEXTENT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETHORIZONTALEXTENT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETCOLUMNWIDTH, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_ADDFILE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETTOPINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETITEMRECT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETITEMDATA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETITEMDATA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SELITEMRANGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETANCHORINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETANCHORINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETCARETINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETCARETINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETITEMHEIGHT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETITEMHEIGHT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_FINDSTRINGEXACT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETLOCALE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_GETLOCALE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_SETCOUNT, CFDefaultMsgInfo);
#if(WINVER >= 0x0400)
				GET_MESSAGE_INFO_ENTRY(LB_INITSTORAGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LB_ITEMFROMPOINT, CFDefaultMsgInfo);
#endif /* WINVER >= 0x0400 */

#if(_WIN32_WCE >= 0x0400)
				GET_MESSAGE_INFO_ENTRY(LB_MULTIPLEADDSTRING, CFDefaultMsgInfo);
#endif //_WIN32_WCE >= 0x0400

#if(_WIN32_WINNT >= 0x0501)
				GET_MESSAGE_INFO_ENTRY(LB_GETLISTBOXINFO, CFDefaultMsgInfo);
#endif /* _WIN32_WINNT >= 0x0501 */

//#if(_WIN32_WINNT >= 0x0501)
				GET_MESSAGE_INFO_ENTRY(LB_MSGMAX, CFDefaultMsgInfo);
//#endif //_WIN32_WINNT

				//ListView messages
				GET_MESSAGE_INFO_ENTRY(LVM_GETBKCOLOR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETBKCOLOR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETIMAGELIST, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETIMAGELIST, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETITEMCOUNT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETITEMA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETITEMW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETITEMA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETITEMW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_INSERTITEMA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_INSERTITEMW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_DELETEITEM, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_DELETEALLITEMS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETCALLBACKMASK, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETCALLBACKMASK, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETNEXTITEM, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_FINDITEMA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_FINDITEMW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETITEMRECT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETITEMPOSITION, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETITEMPOSITION, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETSTRINGWIDTHA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETSTRINGWIDTHW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_HITTEST, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_ENSUREVISIBLE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SCROLL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_REDRAWITEMS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_ARRANGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_EDITLABELA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_EDITLABELW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETEDITCONTROL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETCOLUMNA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETCOLUMNW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETCOLUMNA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETCOLUMNW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_INSERTCOLUMNA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_INSERTCOLUMNW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_DELETECOLUMN, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETCOLUMNWIDTH, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETCOLUMNWIDTH, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETHEADER, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_CREATEDRAGIMAGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETVIEWRECT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETTEXTCOLOR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETTEXTCOLOR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETTEXTBKCOLOR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETTEXTBKCOLOR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETTOPINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETCOUNTPERPAGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETORIGIN, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_UPDATE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETITEMSTATE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETITEMSTATE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETITEMTEXTA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETITEMTEXTW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETITEMTEXTA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETITEMTEXTW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETITEMCOUNT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SORTITEMS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETITEMPOSITION32, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETSELECTEDCOUNT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETITEMSPACING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETISEARCHSTRINGA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETISEARCHSTRINGW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETICONSPACING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETEXTENDEDLISTVIEWSTYLE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETEXTENDEDLISTVIEWSTYLE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETSUBITEMRECT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SUBITEMHITTEST, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETCOLUMNORDERARRAY, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETCOLUMNORDERARRAY, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETHOTITEM, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETHOTITEM, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETHOTCURSOR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETHOTCURSOR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_APPROXIMATEVIEWRECT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETWORKAREAS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETWORKAREAS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETNUMBEROFWORKAREAS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETSELECTIONMARK, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETSELECTIONMARK, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETHOVERTIME, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETHOVERTIME, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SETTOOLTIPS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_GETTOOLTIPS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(LVM_SORTITEMSEX, CFDefaultMsgInfo);
				//GET_MESSAGE_INFO_ENTRY(XXXXXXXXXXXX, CFDefaultMsgInfo);

				//Combo Box messages
				GET_MESSAGE_INFO_ENTRY(CB_GETEDITSEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_LIMITTEXT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SETEDITSEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_ADDSTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_DELETESTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_DIR, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETCOUNT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETCURSEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETLBTEXT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETLBTEXTLEN, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_INSERTSTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_RESETCONTENT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_FINDSTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SELECTSTRING, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SETCURSEL, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SHOWDROPDOWN, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETITEMDATA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SETITEMDATA, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETDROPPEDCONTROLRECT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SETITEMHEIGHT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETITEMHEIGHT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SETEXTENDEDUI, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETEXTENDEDUI, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETDROPPEDSTATE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_FINDSTRINGEXACT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SETLOCALE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETLOCALE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETTOPINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SETTOPINDEX, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETHORIZONTALEXTENT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SETHORIZONTALEXTENT, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_GETDROPPEDWIDTH, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_SETDROPPEDWIDTH, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(CB_INITSTORAGE, CFDefaultMsgInfo);
#if(_WIN32_WCE >= 0x0400)
				GET_MESSAGE_INFO_ENTRY(CB_MULTIPLEADDSTRING, CFDefaultMsgInfo);
#endif //_WIN32_WCE >= 0x0400
#if(_WIN32_WINNT >= 0x0501)
				GET_MESSAGE_INFO_ENTRY(CB_GETCOMBOBOXINFO, CFDefaultMsgInfo);
#endif //_WIN32_WINNT >= 0x0501
				GET_MESSAGE_INFO_ENTRY(CB_MSGMAX, CFDefaultMsgInfo);

				//Scroll bar messages
				GET_MESSAGE_INFO_ENTRY(SBM_SETPOS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(SBM_GETPOS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(SBM_SETRANGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(SBM_SETRANGEREDRAW, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(SBM_GETRANGE, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(SBM_ENABLE_ARROWS, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(SBM_SETSCROLLINFO, CFDefaultMsgInfo);
				GET_MESSAGE_INFO_ENTRY(SBM_GETSCROLLINFO, CFDefaultMsgInfo);

#if(_WIN32_WINNT >= 0x0501)
				GET_MESSAGE_INFO_ENTRY(SBM_GETSCROLLBARINFO, CFDefaultMsgInfo);
#endif /* _WIN32_WINNT >= 0x0501 */

                //Reflected Window Message IDs
                GET_MESSAGE_INFO_ENTRY(OCM_COMMAND, CFDefaultMsgInfo);

#ifdef _WIN32
                GET_MESSAGE_INFO_ENTRY(OCM_CTLCOLORBTN, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_CTLCOLOREDIT, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_CTLCOLORDLG, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_CTLCOLORLISTBOX, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_CTLCOLORMSGBOX, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_CTLCOLORSCROLLBAR, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_CTLCOLORSTATIC, CFDefaultMsgInfo);
#else 
                GET_MESSAGE_INFO_ENTRY(OCM_CTLCOLOR, CFDefaultMsgInfo);
#endif //ifndef _WIN32

                GET_MESSAGE_INFO_ENTRY(OCM_DRAWITEM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_MEASUREITEM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_DELETEITEM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_VKEYTOITEM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_CHARTOITEM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_COMPAREITEM, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_HSCROLL, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_VSCROLL, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_PARENTNOTIFY, CFDefaultMsgInfo);
                GET_MESSAGE_INFO_ENTRY(OCM_NOTIFY, CFDefaultMsgInfo);
				//GET_MESSAGE_INFO_ENTRY(XXXXXXXXXXXXXXXXX, CFDefaultMsgInfo);
            default:
				if (m_Info >= WM_USER && m_Info <= 0x7FFF)
				{
					//private window classes
					UINT userMsg = m_Info - WM_USER;
					StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
						TEXT("0x%08x = (WM_USER+%d[0x%x])"),m_Info, userMsg, userMsg );
				}
				else if(m_Info >= WM_APP && m_Info <= 0xBFFF)
				{
					//Messages available for use by applications
					UINT appMsg = m_Info - WM_APP;
					StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
						TEXT("0x%08x = (WM_APP+%d[0x%x])"),m_Info, appMsg, appMsg );

				}
				else if (m_Info >= 0xC000 && m_Info <= 0xFFFF)
				{
					//通过 RegisterWindowMessage 注册的消息
					StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
						s_RegistedMessageInfo.GetMessageInfo(m_Info, m_wParam, m_lParam));
				}
				else
				{
					StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
						TEXT("Unknown Message %d(0x%08x)"),m_Info,m_Info);
				}
            }
        }
        return m_bufInfo;
    }//CFMessageInfo


    ///////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename T, typename TWindowAutoSizeTraits >
    CFWindowAutoSize<T,TWindowAutoSizeTraits>::CFWindowAutoSize()
    {
        m_sizeWindow.cx = 0;
        m_sizeWindow.cy = 0; 
        m_ptMinTrackSize.x = -1;
        m_ptMinTrackSize.y = -1;
        m_bGripper = FALSE;
    }

    template <typename T, typename TWindowAutoSizeTraits >
    BOOL CFWindowAutoSize<T,TWindowAutoSizeTraits>::InitAutoSizeInfo(BOOL bAddGripper /* = TRUE */, BOOL bUseMinTrackSize /* = TRUE */)
    {
        BOOL bRet = FALSE;
        T* pT = static_cast<T*>(this);
        HWND hWnd = TWindowAutoSizeTraits::GetWinHwndProxy(pT);
        FTLASSERT(::IsWindow(hWnd));
        DWORD dwStyle = (DWORD)::GetWindowLong(hWnd, GWL_STYLE);
#ifdef FTL_DEBUG
        // Debug only: Check if top level dialogs have a resizeable border.
        if(((dwStyle & WS_CHILD) == 0) && ((dwStyle & WS_THICKFRAME) == 0))
        {
            FTLTRACEEX(tlWarning,TEXT("top level dialog without the WS_THICKFRAME style - user cannot resize it\n"));
        }
#endif // _DEBUG

        {
            // Cleanup in case of multiple initialization
            // block: first check for the gripper control, destroy it if needed
            HWND HwndGripper = ::GetDlgItem(hWnd, TWindowAutoSizeTraits::GetStatusBarCtrlID());
            if( ::IsWindow(HwndGripper) && m_allResizeData.size() > 0 && (m_allResizeData[0].m_dwResizeFlags & _WINSZ_GRIPPER) != 0)
            {
                API_VERIFY(::DestroyWindow(HwndGripper));
            }
        }
        // clear out everything else
        m_allResizeData.clear();
        m_sizeWindow.cx = 0;
        m_sizeWindow.cy = 0;
        m_ptMinTrackSize.x = -1;
        m_ptMinTrackSize.y = -1;

        // Get initial dialog client size
        RECT rectDlg = { 0 };
        API_VERIFY(::GetClientRect(hWnd,&rectDlg));
        m_sizeWindow.cx = rectDlg.right;
        m_sizeWindow.cy = rectDlg.bottom;

       // Create gripper if requested
        m_bGripper = FALSE;
        if(bAddGripper)
        {
            // shouldn't exist already
            FTLASSERT(!::IsWindow(::GetDlgItem(hWnd, TWindowAutoSizeTraits::GetStatusBarCtrlID())));
            if(!::IsWindow(::GetDlgItem(hWnd, TWindowAutoSizeTraits::GetStatusBarCtrlID())))
            {
                HWND hWndGripper = ::CreateWindowEx(0,_T("SCROLLBAR"),NULL, 
                    WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | SBS_SIZEBOX | SBS_SIZEGRIP | SBS_SIZEBOXBOTTOMRIGHTALIGN,
                    rectDlg.left,
                    rectDlg.top,
                    rectDlg.right - rectDlg.left,
                    rectDlg.bottom - rectDlg.top,
                    hWnd,
                    (HMENU)(TWindowAutoSizeTraits::GetStatusBarCtrlID()), 
                    NULL,
                    NULL);
                FTLASSERT(::IsWindow(hWndGripper));
                if(::IsWindow(hWndGripper))
                {
                    m_bGripper = TRUE;
                    RECT rectCtl = { 0 };
                    API_VERIFY(::GetWindowRect(hWndGripper,&rectCtl));
                    API_VERIFY(::MapWindowPoints(NULL, hWnd, (LPPOINT)&rectCtl, 2) != 0);
                    _WindowResizeData data = 
                    {
                        TWindowAutoSizeTraits::GetStatusBarCtrlID(), 
                        WINSZ_MOVE_X | WINSZ_MOVE_Y | WINSZ_REPAINT | _WINSZ_GRIPPER, 
                        { 
                            rectCtl.left, 
                            rectCtl.top, 
                            rectCtl.right, 
                            rectCtl.bottom 
                        }
                    };
                    m_allResizeData.push_back(data);
                }
            }
        }

        // Get min track position if requested
        if(bUseMinTrackSize)
        {
            if((dwStyle & WS_CHILD) != 0)
            {
                RECT rect = { 0 };
                API_VERIFY(::GetClientRect(hWnd,&rect));
                m_ptMinTrackSize.x = rect.right - rect.left;
                m_ptMinTrackSize.y = rect.bottom - rect.top;
            }
            else
            {
                RECT rect = { 0 };
                API_VERIFY(::GetWindowRect(hWnd, &rect));
                m_ptMinTrackSize.x = rect.right - rect.left;
                m_ptMinTrackSize.y = rect.bottom - rect.top;
            }
        }

        // Walk the map and initialize data
        const _WindowResizeMap* pMap = pT->GetWindowResizeMap();
        FTLASSERT(pMap != NULL);
        int nGroupStart = -1;
        for(int nCount = 1; !(pMap->m_nCtlID == -1 && pMap->m_dwResizeFlags == 0); nCount++, pMap++)
        {
            if(pMap->m_nCtlID == -1)    //开始或结束一个组
            {
                switch(pMap->m_dwResizeFlags)
                {
                case _WINSZ_BEGIN_GROUP:
                    FTLASSERT(nGroupStart == -1);
                    nGroupStart = static_cast<int>(m_allResizeData.size());
                    break;
                case _WINSZ_END_GROUP:
                    {
                        FTLASSERT(nGroupStart != -1);
                        int nGroupCount = static_cast<int>(m_allResizeData.size()) - nGroupStart;
                        m_allResizeData[nGroupStart].SetGroupCount(nGroupCount);
                        nGroupStart = -1;
                    }
                    break;
                default:
                    FTLASSERT(FALSE && _T("Invalid WINDOWRESIZE Map Entry"));
                    break;
                }
            }
            else
            {
                // this ID conflicts with the default gripper one
                FTLASSERT(m_bGripper ? (pMap->m_nCtlID != TWindowAutoSizeTraits::GetStatusBarCtrlID()) : TRUE);
                HWND hWndCtrl = ::GetDlgItem(hWnd,pMap->m_nCtlID);
                FTLASSERT(::IsWindow(hWndCtrl));
                RECT rectCtl = { 0 };
                API_VERIFY(::GetWindowRect(hWndCtrl,&rectCtl));
                API_VERIFY(::MapWindowPoints(NULL, hWnd, (LPPOINT)&rectCtl, 2)!= 0);

                DWORD dwGroupFlag = (nGroupStart != -1 && static_cast<int>(m_allResizeData.size()) == nGroupStart) ? _WINSZ_BEGIN_GROUP : 0;
                _WindowResizeData data = 
                {
                    pMap->m_nCtlID, 
                    pMap->m_dwResizeFlags | dwGroupFlag, 
                    { 
                        rectCtl.left, 
                        rectCtl.top, 
                        rectCtl.right, 
                        rectCtl.bottom 
                    } 
                };
                m_allResizeData.push_back(data);
            }
        }
        FTLASSERT((nGroupStart == -1) && _T("No End Group Entry in the WINDOWRESIZE Map"));
        return bRet;
    }

    template <typename T, typename TWindowAutoSizeTraits >
    BOOL CFWindowAutoSize<T,TWindowAutoSizeTraits>::AutoResizeUpdateLayout(int cxWidth, int cyHeight)
    {
        BOOL bRet = FALSE;
        T* pT = static_cast<T*>(this);
        HWND hWnd = TWindowAutoSizeTraits::GetWinHwndProxy(pT);

        FTLASSERT(::IsWindow(hWnd));

        // Restrict minimum size if requested
        if((((DWORD)::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD) != 0) && m_ptMinTrackSize.x != -1 && m_ptMinTrackSize.y != -1)
        {
            if(cxWidth < m_ptMinTrackSize.x)
            {
                cxWidth = m_ptMinTrackSize.x;
            }
            if(cyHeight < m_ptMinTrackSize.y)
            {
                cyHeight = m_ptMinTrackSize.y;
            }
        }

        BOOL bVisible = ::IsWindowVisible(hWnd);
        if(bVisible)
        {
            ::SendMessage(hWnd, WM_SETREDRAW, (WPARAM)FALSE, 0);
            //pT->SetRedraw(FALSE);
        }

        for(size_t i = 0; i < m_allResizeData.size(); i++)
        {
            if((m_allResizeData[i].m_dwResizeFlags & _WINSZ_BEGIN_GROUP) != 0)   // start of a group
            {
                int nGroupCount = m_allResizeData[i].GetGroupCount();
                FTLASSERT(nGroupCount > 0 && i + nGroupCount - 1 < m_allResizeData.size());
                RECT rectGroup = m_allResizeData[i].m_rect;

                int j = 1;
                for(j = 1; j < nGroupCount; j++)
                {
                    rectGroup.left = min(rectGroup.left, m_allResizeData[i + j].m_rect.left);
                    rectGroup.top = min(rectGroup.top, m_allResizeData[i + j].m_rect.top);
                    rectGroup.right = max(rectGroup.right, m_allResizeData[i + j].m_rect.right);
                    rectGroup.bottom = max(rectGroup.bottom, m_allResizeData[i + j].m_rect.bottom);
                }

                for(j = 0; j < nGroupCount; j++)
                {
                    _WindowResizeData* pDataPrev = NULL;
                    if(j > 0)
                        pDataPrev = &(m_allResizeData[i + j - 1]);
                    pT->AutoPositionControl(cxWidth, cyHeight, rectGroup, m_allResizeData[i + j], true, pDataPrev);
                }

                i += nGroupCount - 1;   // increment to skip all group controls
            }
            else // one control entry
            {
                RECT rectGroup = { 0, 0, 0, 0 };
                pT->AutoPositionControl(cxWidth, cyHeight, rectGroup, m_allResizeData[i], false);
            }
        }

        if(bVisible)
        {
            ::SendMessage(hWnd, WM_SETREDRAW, (WPARAM)TRUE, 0);
            //pT->SetRedraw(TRUE);
        }
        ::RedrawWindow(hWnd,NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
        return bRet;
    }

    template <typename T, typename TWindowAutoSizeTraits >
    BOOL CFWindowAutoSize<T,TWindowAutoSizeTraits>::AutoPositionControl(int cxWidth, int cyHeight, 
        RECT& rectGroup, _WindowResizeData& data, 
        bool bGroup, _WindowResizeData* pDataPrev /* = NULL */)
    {
        //BOOL bRet = FALSE;
        T* pT = static_cast<T*>(this);
        HWND hWnd = TWindowAutoSizeTraits::GetWinHwndProxy(pT);
        FTLASSERT(::IsWindow(hWnd));

        HWND hWndCtrl = NULL;
        RECT rectCtl = { 0 };

        hWndCtrl = ::GetDlgItem(hWnd, data.m_nCtlID);
        FTLASSERT(::IsWindow(hWndCtrl));
        ::GetWindowRect(hWndCtrl, &rectCtl);
        ::MapWindowPoints(NULL, hWnd, (LPPOINT)&rectCtl, 2);

        if(bGroup)
        {
            if((data.m_dwResizeFlags & WINSZ_CENTER_X) != 0)
            {
                int cxRight = rectGroup.right + cxWidth - m_sizeWindow.cx;
                int cxCtl = data.m_rect.right - data.m_rect.left;
                rectCtl.left = rectGroup.left + (cxRight - rectGroup.left - cxCtl) / 2;
                rectCtl.right = rectCtl.left + cxCtl;
            }
            else if((data.m_dwResizeFlags & (WINSZ_SIZE_X | WINSZ_MOVE_X)) != 0)
            {
                rectCtl.left = rectGroup.left + ::MulDiv(data.m_rect.left - rectGroup.left, rectGroup.right - rectGroup.left + (cxWidth - m_sizeWindow.cx), rectGroup.right - rectGroup.left);

                if((data.m_dwResizeFlags & WINSZ_SIZE_X) != 0)
                {
                    rectCtl.right = rectGroup.left + ::MulDiv(data.m_rect.right - rectGroup.left, rectGroup.right - rectGroup.left + (cxWidth - m_sizeWindow.cx), rectGroup.right - rectGroup.left);

                    if(pDataPrev != NULL)
                    {
                        HWND hWndCtlPrev = ::GetDlgItem(hWnd,pDataPrev->m_nCtlID);
                        FTLASSERT(::IsWindow(hWndCtlPrev));
                        RECT rcPrev = { 0 };
                        ::GetWindowRect(hWndCtlPrev, &rcPrev);
                        ::MapWindowPoints(NULL, hWnd, (LPPOINT)&rcPrev, 2);
                        int dxAdjust = (rectCtl.left - rcPrev.right) - (data.m_rect.left - pDataPrev->m_rect.right);
                        rcPrev.right += dxAdjust;
                        ::SetWindowPos(hWndCtlPrev, NULL, rcPrev.left,rcPrev.top,rcPrev.right - rcPrev.left,
                            rcPrev.bottom-rcPrev.top,SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
                    }
                }
                else
                {
                    rectCtl.right = rectCtl.left + (data.m_rect.right - data.m_rect.left);
                }
            }

            if((data.m_dwResizeFlags & WINSZ_CENTER_Y) != 0)
            {
                int cyBottom = rectGroup.bottom + cyHeight - m_sizeWindow.cy;
                int cyCtl = data.m_rect.bottom - data.m_rect.top;
                rectCtl.top = rectGroup.top + (cyBottom - rectGroup.top - cyCtl) / 2;
                rectCtl.bottom = rectCtl.top + cyCtl;
            }
            else if((data.m_dwResizeFlags & (WINSZ_SIZE_Y | WINSZ_MOVE_Y)) != 0)
            {
                rectCtl.top = rectGroup.top + ::MulDiv(data.m_rect.top - rectGroup.top, rectGroup.bottom - rectGroup.top + (cyHeight - m_sizeWindow.cy), rectGroup.bottom - rectGroup.top);

                if((data.m_dwResizeFlags & WINSZ_SIZE_Y) != 0)
                {
                    rectCtl.bottom = rectGroup.top + ::MulDiv(data.m_rect.bottom - rectGroup.top, rectGroup.bottom - rectGroup.top + (cyHeight - m_sizeWindow.cy), rectGroup.bottom - rectGroup.top);

                    if(pDataPrev != NULL)
                    {
                        HWND hWndCtlPrev = ::GetDlgItem(hWnd, pDataPrev->m_nCtlID);
                        FTLASSERT(::IsWindow(hWndCtlPrev));
                        RECT rcPrev = { 0 };
                        ::GetWindowRect(hWndCtlPrev, &rcPrev);
                        ::MapWindowPoints(NULL, hWnd, (LPPOINT)&rcPrev, 2);
                        int dxAdjust = (rectCtl.top - rcPrev.bottom) - (data.m_rect.top - pDataPrev->m_rect.bottom);
                        rcPrev.bottom += dxAdjust;
                        ::SetWindowPos(hWndCtlPrev, NULL, rcPrev.left,rcPrev.top,rcPrev.right - rcPrev.left,
                            rcPrev.bottom-rcPrev.top,SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
                    }
                }
                else
                {
                    rectCtl.bottom = rectCtl.top + (data.m_rect.bottom - data.m_rect.top);
                }
            }
        }
        else // no group
        {
            if((data.m_dwResizeFlags & WINSZ_CENTER_X) != 0)
            {
                int cxCtl = data.m_rect.right - data.m_rect.left;
                rectCtl.left = (cxWidth - cxCtl) / 2;
                rectCtl.right = rectCtl.left + cxCtl;
            }
            else if((data.m_dwResizeFlags & (WINSZ_SIZE_X | WINSZ_MOVE_X)) != 0)
            {
                rectCtl.right = data.m_rect.right + (cxWidth - m_sizeWindow.cx);

                if((data.m_dwResizeFlags & WINSZ_MOVE_X) != 0)
                    rectCtl.left = rectCtl.right - (data.m_rect.right - data.m_rect.left);
            }

            if((data.m_dwResizeFlags & WINSZ_CENTER_Y) != 0)
            {
                int cyCtl = data.m_rect.bottom - data.m_rect.top;
                rectCtl.top = (cyHeight - cyCtl) / 2;
                rectCtl.bottom = rectCtl.top + cyCtl;
            }
            else if((data.m_dwResizeFlags & (WINSZ_SIZE_Y | WINSZ_MOVE_Y)) != 0)
            {
                rectCtl.bottom = data.m_rect.bottom + (cyHeight - m_sizeWindow.cy);

                if((data.m_dwResizeFlags & WINSZ_MOVE_Y) != 0)
                    rectCtl.top = rectCtl.bottom - (data.m_rect.bottom - data.m_rect.top);
            }
        }

        if((data.m_dwResizeFlags & WINSZ_REPAINT) != 0)
        {
            ::InvalidateRect(hWndCtrl, NULL, TRUE);
        }
        if((data.m_dwResizeFlags & (WINSZ_SIZE_X | WINSZ_SIZE_Y | WINSZ_MOVE_X | WINSZ_MOVE_Y | WINSZ_REPAINT | WINSZ_CENTER_X | WINSZ_CENTER_Y)) != 0)
        {
            ::SetWindowPos(hWndCtrl, NULL, rectCtl.left,rectCtl.top,rectCtl.right - rectCtl.left,
                rectCtl.bottom-rectCtl.top,SWP_NOZORDER | SWP_NOACTIVATE);

        }
        return TRUE;
    }

    BOOL CFWinUtil::SetWindowFullScreen(HWND hWnd,BOOL isFullScreen, BOOL &oldZoomedState)
    {
        BOOL bRet = FALSE;
        //获取标题栏 SM_CXFRAME, SM_CYFRAME, SM_CXScreen 的大小
        int cyCaption = ::GetSystemMetrics(SM_CYCAPTION);
        int cxFrame = ::GetSystemMetrics(SM_CXFRAME);
        int cyFrame = ::GetSystemMetrics(SM_CYFRAME);
        int cyMenu = ::GetSystemMetrics(SM_CYMENU);
        int cxScreen = ::GetSystemMetrics(SM_CXSCREEN);
        int cyScreen = ::GetSystemMetrics(SM_CYSCREEN);
        //int cxBorder = ::GetSystemMetrics(SM_CXBORDER);
        //int cyBorder = ::GetSystemMetrics(SM_CYBORDER);

        if (isFullScreen)
        {
            //oldZoomedState = ::IsZoomed(hWnd); //保存当前是否是最大化状态
            //if(oldZoomedState) //当前是最大化
            {
                BringWindowToTop(hWnd);
                bRet = ::SetWindowPos(hWnd,HWND_TOPMOST,-cxFrame,-(cyFrame + cyCaption + cyMenu),
                    cxScreen + 2 * cxFrame, cyScreen + 2 * cyFrame + cyCaption + cyMenu,
                    0);//SWP_NOOWNERZORDER
                //SetForegroundWindow

            }
            //else //当前是普通状态，进最大化
            //{
            //    bRet = ::ShowWindow(hWnd,SW_SHOWMAXIMIZED);
            //}
        }
        else //恢复原窗口大小
        {
            if(oldZoomedState)
            {
                //是否错了？？？
                bRet = SetWindowPos(hWnd,NULL,-cxFrame,-cyFrame,cxScreen + 2*cxFrame, cyScreen + 2*cyFrame, SWP_NOZORDER);
            }
            else
            {
                bRet = ShowWindow(hWnd,SW_RESTORE);
            }
        }
        return bRet;
    }

#if 0
    BOOL CFWinUtil::CenterWindow(HWND hWndCenter , BOOL bCurrMonitor)
    {
        FTLASSERT ( ::IsWindow ( m_hWnd ) ) ;
        // determine owner window to center against
        DWORD dwStyle = (DWORD)GetWindowLongPtr ( GWL_STYLE ) ;
        if ( NULL == hWndCenter )
        {
            if( dwStyle & WS_CHILD )
            {
                hWndCenter = ::GetParent ( m_hWnd ) ;
            }
            else
            {
                hWndCenter = ::GetWindow ( m_hWnd , GW_OWNER ) ;
            }
        }

        // Get coordinates of the window relative to its parent
        RECT rcDlg ;
        GetWindowRect ( &rcDlg ) ;
        RECT rcArea ;
        RECT rcCenter ;
        HWND hWndParent ;
        if ( !( dwStyle & WS_CHILD ) )
        {
            // Don't center against invisible or minimized windows
            if( NULL != hWndCenter )
            {
                DWORD dwStyle = ::GetWindowLongPtr ( hWndCenter ,
                    GWL_STYLE   ) ;
                if( !( dwStyle & WS_VISIBLE  ) ||
                    ( dwStyle & WS_MINIMIZE )   )
                {
                    hWndCenter = NULL ;
                }
            }

            if ( FALSE == bCurrMonitor )
            {
                // Center within screen coordinates
                ::SystemParametersInfo ( SPI_GETWORKAREA ,
                    NULL            ,
                    &rcArea         ,
                    NULL             ) ;
            }
            else
            {
                // Center based on the monitor containing the majority of
                // the window.
                HMONITOR hMon = MonitorFromWindow ( m_hWnd  ,
                    MONITOR_DEFAULTTONEAREST); //MONITOR_DEFAULTTOPRIMARY

                MONITORINFO stMI ;
                ZeroMemory ( &stMI , sizeof ( MONITORINFO ) ) ;
                stMI.cbSize = sizeof ( MONITORINFO ) ;

                GetMonitorInfo ( hMon , &stMI ) ;

                rcArea = stMI.rcMonitor ;
            }

            if ( NULL == hWndCenter )
            {
                rcCenter = rcArea;
            }
            else
            {
                ::GetWindowRect ( hWndCenter , &rcCenter ) ;
            }
        }
        else
        {
            // center within parent client coordinates
            hWndParent = GetParent ( ) ;

            ::GetClientRect ( hWndParent , &rcArea ) ;

            ::GetClientRect ( hWndCenter , &rcCenter ) ;
            ::MapWindowPoints ( hWndCenter        ,
                hWndParent        ,
                (POINT*)&rcCenter ,
                2                  ) ;
        }

        int DlgWidth = rcDlg.right - rcDlg.left ;
        int DlgHeight = rcDlg.bottom - rcDlg.top ;

        // Find dialog's upper left based on rcCenter
        int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2 ;
        int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2 ;

        // If the dialog is outside the screen, move it inside
        if ( xLeft < rcArea.left )
        {
            xLeft = rcArea.left ;
        }
        else if ( xLeft + DlgWidth > rcArea.right )
        {
            xLeft = rcArea.right - DlgWidth ;
        }

        if ( yTop < rcArea.top )
        {
            yTop = rcArea.top ;
        }
        else if ( yTop + DlgHeight > rcArea.bottom )
        {
            yTop = rcArea.bottom - DlgHeight ;
        }

        // Map screen coordinates to child coordinates
        return ( ::SetWindowPos ( m_hWnd ,
            NULL   ,
            xLeft  ,
            yTop   ,
            -1     ,
            -1     ,
            SWP_NOSIZE |
            SWP_NOZORDER |
            SWP_NOACTIVATE  ) ) ;

    }

#endif 

	LRESULT CFWinUtil::CalcNcHitTestPostion(LPPOINT pPtClient, LPCRECT prcClient, LPCRECT prcCaption, BOOL bZoomed)
	{
		 //检测时各个方向的阈值(XP:4, Win7:8)
		int nCxFrame = GetSystemMetrics(SM_CXFRAME);
		int nCyFrame = GetSystemMetrics(SM_CYFRAME);
		RECT rcSizeBox = {nCxFrame, nCyFrame, nCxFrame, nCyFrame}; 

		POINT pt = *pPtClient;
		RECT rcClient = *prcClient;

		FTLASSERT(pt.x < 0xFFFF);
		FTLASSERT(pt.y < 0xFFFF);
		//什么时候会出现这种情况？一边拖拽，一边按 Win+D 等键?
		if (pt.x >= 0xffff) { pt.x -= 0xffff; }
		if (pt.y >= 0xffff) { pt.y -= 0xffff; }

		if( !bZoomed ) {
			if( pt.y < rcClient.top + rcSizeBox.top ) {
				if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
				if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
				return HTTOP;
			}
			else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
				if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
				if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
				return HTBOTTOM;
			}
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
		}

		RECT rcCaption = *prcCaption;
		if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
#pragma TODO(需要排除 关闭、最小化、最大化、Option菜单等控件的位置)
				//如果这些地方是按钮的话，按钮会优先处理，因此不需要排除
				//但如果类似DUI，是自绘的话，则必须排除

				//CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
				//if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
				//	_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
				//	_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
					return HTCAPTION;
		}
		return HTCLIENT;
	}

	HWND CFWinUtil::GetProcessMainWindow(DWORD dwProcessId)
	{
		HWND hWnd = ::GetTopWindow(NULL);
		while (hWnd)
		{
			DWORD dwPid = 0;
			DWORD dwThreadId = GetWindowThreadProcessId(hWnd, &dwPid);
			UNREFERENCED_PARAMETER(dwThreadId);
			if (dwPid != 0 && dwPid == dwProcessId)
			{
				return hWnd;
			}
			hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT);
		}
		return NULL;
	}

    BOOL CFWinUtil::ActiveAndForegroundWindow(HWND hWnd)
    {
        //IPMSG 中也有一个 SetForceForegroundWindow 方法，大致相同

        BOOL bRet = TRUE;
        HWND   hForegdWnd   =   ::GetForegroundWindow();
        if (hForegdWnd == hWnd)
        {
            return TRUE;
        }

        DWORD   dwThisThreadID   =   ::GetWindowThreadProcessId(hWnd, NULL);
        DWORD   dwForeThreadID   =   ::GetWindowThreadProcessId(hForegdWnd,NULL);
        if (dwThisThreadID != dwForeThreadID)
        {
            API_VERIFY(::AttachThreadInput(dwThisThreadID,dwForeThreadID,TRUE));

            //备份以前的值，然后设置为0
            DWORD sp_time = 0;
            SystemParametersInfo( SPI_GETFOREGROUNDLOCKTIMEOUT,0,&sp_time,0);
            SystemParametersInfo( SPI_SETFOREGROUNDLOCKTIMEOUT,0,(LPVOID)NULL, SPIF_SENDCHANGE); //0);

			API_VERIFY(::BringWindowToTop( hWnd ));
            API_VERIFY(::SetForegroundWindow(hWnd));

            SystemParametersInfo( SPI_SETFOREGROUNDLOCKTIMEOUT,0,&sp_time, SPIF_SENDCHANGE); //0);
            API_VERIFY(::AttachThreadInput(dwThisThreadID,dwForeThreadID,FALSE));
            //ShowWindow(hWnd,...)?
        }
        else
        {
			API_VERIFY(::BringWindowToTop( hWnd ));
            API_VERIFY(::SetForegroundWindow(hWnd));
            //ShowWindow(hWnd,...)?
        }
        SwitchToThisWindow(hWnd, TRUE);

//#pragma TODO(how to active a minimize window without change it min/max status)
//        if (IsIconic(hWnd))
//        {
//            API_VERIFY(ShowWindow(hWnd,SW_RESTORE));
//        }
//        else
//        {
//            API_VERIFY(ShowWindow(hWnd,SW_SHOW));
//        }

		//是否需要这些代码
#if 0
		if ( TRUE ==  ::IsIconic ( hWnd ) )
		{
			::SendMessage ( hWnd, WM_SYSCOMMAND, SC_RESTORE, 0 );
		}
#endif 

        return bRet;
    }

    LPCDLGTEMPLATE CFWinUtil::LoadDialog(HMODULE hModuleRes, LPCTSTR szResource, HINSTANCE * phInstFoundIn)
    {
        UNREFERENCED_PARAMETER(phInstFoundIn);
        // Find the dialog resource.
        HRSRC hRSRC = FindResourceEx ( hModuleRes ,RT_DIALOG,szResource,MAKELANGID ( LANG_NEUTRAL,SUBLANG_NEUTRAL )) ;
        if ( NULL == hRSRC )
        {
            return ( NULL ) ;
        }
        // Now load it.
        HGLOBAL hResource = LoadResource ( hModuleRes , hRSRC ) ;
        FTLASSERT ( NULL != hResource ) ;
        if ( NULL == hResource )
        {
            return ( NULL ) ;
        }
        LPCDLGTEMPLATE lpDlgTemplate = (LPCDLGTEMPLATE)LockResource ( hResource ) ;
        return lpDlgTemplate;
    }

	LPCTSTR CFWinUtil::GetColorString(ColorType clrType, int nColorIndex)
	{
		switch (clrType)
		{
		case ctCtrlColor:
			{
				switch(nColorIndex)
				{
					HANDLE_CASE_RETURN_STRING(CTLCOLOR_MSGBOX);
					HANDLE_CASE_RETURN_STRING(CTLCOLOR_EDIT);
					HANDLE_CASE_RETURN_STRING(CTLCOLOR_LISTBOX);
					HANDLE_CASE_RETURN_STRING(CTLCOLOR_BTN);
					HANDLE_CASE_RETURN_STRING(CTLCOLOR_DLG);
					HANDLE_CASE_RETURN_STRING(CTLCOLOR_SCROLLBAR);
					HANDLE_CASE_RETURN_STRING(CTLCOLOR_STATIC);
					HANDLE_CASE_RETURN_STRING(CTLCOLOR_MAX);
				}
				break;
			}
		case ctSysColor:
			{
				switch(nColorIndex)
				{
					HANDLE_CASE_RETURN_STRING(COLOR_SCROLLBAR);
					HANDLE_CASE_RETURN_STRING(COLOR_BACKGROUND);
					HANDLE_CASE_RETURN_STRING(COLOR_ACTIVECAPTION);
					HANDLE_CASE_RETURN_STRING(COLOR_INACTIVECAPTION);
					HANDLE_CASE_RETURN_STRING(COLOR_MENU);
					HANDLE_CASE_RETURN_STRING(COLOR_WINDOW);
					HANDLE_CASE_RETURN_STRING(COLOR_WINDOWFRAME);
					HANDLE_CASE_RETURN_STRING(COLOR_MENUTEXT);
					HANDLE_CASE_RETURN_STRING(COLOR_WINDOWTEXT);
					HANDLE_CASE_RETURN_STRING(COLOR_CAPTIONTEXT);
					HANDLE_CASE_RETURN_STRING(COLOR_ACTIVEBORDER);
					HANDLE_CASE_RETURN_STRING(COLOR_INACTIVEBORDER);
					HANDLE_CASE_RETURN_STRING(COLOR_APPWORKSPACE);
					HANDLE_CASE_RETURN_STRING(COLOR_HIGHLIGHT);
					HANDLE_CASE_RETURN_STRING(COLOR_HIGHLIGHTTEXT);
					HANDLE_CASE_RETURN_STRING(COLOR_BTNFACE);
					HANDLE_CASE_RETURN_STRING(COLOR_BTNSHADOW);
					HANDLE_CASE_RETURN_STRING(COLOR_GRAYTEXT);
					HANDLE_CASE_RETURN_STRING(COLOR_BTNTEXT);
					HANDLE_CASE_RETURN_STRING(COLOR_INACTIVECAPTIONTEXT);
					HANDLE_CASE_RETURN_STRING(COLOR_BTNHIGHLIGHT);
#if(WINVER >= 0x0400)
					HANDLE_CASE_RETURN_STRING(COLOR_3DDKSHADOW);
					HANDLE_CASE_RETURN_STRING(COLOR_3DLIGHT);
					HANDLE_CASE_RETURN_STRING(COLOR_INFOTEXT);
					HANDLE_CASE_RETURN_STRING(COLOR_INFOBK);
#endif /* WINVER >= 0x0400 */

#if(WINVER >= 0x0500)
					HANDLE_CASE_RETURN_STRING(COLOR_HOTLIGHT);
					HANDLE_CASE_RETURN_STRING(COLOR_GRADIENTACTIVECAPTION);
					HANDLE_CASE_RETURN_STRING(COLOR_GRADIENTINACTIVECAPTION);
#endif /* WINVER >= 0x0501 */

#if(WINVER >= 0x0501)
					HANDLE_CASE_RETURN_STRING(COLOR_MENUHILIGHT);
					HANDLE_CASE_RETURN_STRING(COLOR_MENUBAR);
#endif /* WINVER >= 0x0500 */
				}
				break;
			}
		}
		FTLTRACEEX(FTL::tlWarning, TEXT("Unknown Color type(%d) and Index(%d)\n"), clrType, nColorIndex);
		return TEXT("Unknown");
	}

    LPCTSTR CFWinUtil::GetScrollBarCodeString(UINT nSBCode)
    {
        switch(nSBCode)
        {
            HANDLE_CASE_RETURN_STRING((SB_LINEUP|SB_LINELEFT));
            HANDLE_CASE_RETURN_STRING((SB_LINEDOWN|SB_LINERIGHT));
            HANDLE_CASE_RETURN_STRING((SB_PAGEUP|SB_PAGELEFT));
            HANDLE_CASE_RETURN_STRING((SB_PAGEDOWN|SB_PAGERIGHT));
            HANDLE_CASE_RETURN_STRING(SB_THUMBPOSITION);
            HANDLE_CASE_RETURN_STRING(SB_THUMBTRACK);
            HANDLE_CASE_RETURN_STRING((SB_TOP|SB_LEFT));
            HANDLE_CASE_RETURN_STRING((SB_BOTTOM|SB_RIGHT));
            HANDLE_CASE_RETURN_STRING(SB_ENDSCROLL);
        default:
            FTLTRACEEX(FTL::tlWarning, TEXT("Unknown ScrollBar Code, %d\n"), nSBCode);
            return TEXT("Unknown");
        }
    }

	LPCTSTR CFWinUtil::GetVirtualKeyString(int nVirtKey)
	{
		switch (nVirtKey)
		{
			HANDLE_CASE_RETURN_STRING(VK_LBUTTON);
			HANDLE_CASE_RETURN_STRING(VK_RBUTTON);
			HANDLE_CASE_RETURN_STRING(VK_CANCEL);
			HANDLE_CASE_RETURN_STRING(VK_MBUTTON);
#if(_WIN32_WINNT >= 0x0500)
			HANDLE_CASE_RETURN_STRING(VK_XBUTTON1);
			HANDLE_CASE_RETURN_STRING(VK_XBUTTON2);
#endif /* _WIN32_WINNT >= 0x0500 */
			
			// * 0x07 : unassigned
			HANDLE_CASE_RETURN_STRING(VK_BACK);
			HANDLE_CASE_RETURN_STRING(VK_TAB);
			HANDLE_CASE_RETURN_STRING(VK_CLEAR);
			HANDLE_CASE_RETURN_STRING(VK_RETURN);
			HANDLE_CASE_RETURN_STRING(VK_SHIFT);
			HANDLE_CASE_RETURN_STRING(VK_CONTROL);
			HANDLE_CASE_RETURN_STRING(VK_MENU);
			HANDLE_CASE_RETURN_STRING(VK_PAUSE);
			HANDLE_CASE_RETURN_STRING(VK_CAPITAL);
			HANDLE_CASE_RETURN_STRING((VK_KANA|VK_HANGEUL|VK_HANGUL)); //have some old name
			HANDLE_CASE_RETURN_STRING(VK_JUNJA);
			HANDLE_CASE_RETURN_STRING(VK_FINAL);
			HANDLE_CASE_RETURN_STRING((VK_HANJA | VK_KANJI));
			HANDLE_CASE_RETURN_STRING(VK_ESCAPE);
			HANDLE_CASE_RETURN_STRING(VK_CONVERT);
			HANDLE_CASE_RETURN_STRING(VK_NONCONVERT);
			HANDLE_CASE_RETURN_STRING(VK_ACCEPT);
			HANDLE_CASE_RETURN_STRING(VK_MODECHANGE);
			HANDLE_CASE_RETURN_STRING(VK_SPACE);
			HANDLE_CASE_RETURN_STRING(VK_PRIOR);
			HANDLE_CASE_RETURN_STRING(VK_NEXT);
			HANDLE_CASE_RETURN_STRING(VK_END);
			HANDLE_CASE_RETURN_STRING(VK_HOME);
			HANDLE_CASE_RETURN_STRING(VK_LEFT);
			HANDLE_CASE_RETURN_STRING(VK_UP);
			HANDLE_CASE_RETURN_STRING(VK_RIGHT);
			HANDLE_CASE_RETURN_STRING(VK_DOWN);
			HANDLE_CASE_RETURN_STRING(VK_SELECT);
			HANDLE_CASE_RETURN_STRING(VK_PRINT);
			HANDLE_CASE_RETURN_STRING(VK_EXECUTE);
			HANDLE_CASE_RETURN_STRING(VK_SNAPSHOT);
			HANDLE_CASE_RETURN_STRING(VK_INSERT);
			HANDLE_CASE_RETURN_STRING(VK_DELETE);
			HANDLE_CASE_RETURN_STRING(VK_HELP);

			//VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
			HANDLE_CASE_RETURN_STRING('0');
			HANDLE_CASE_RETURN_STRING('1');
			HANDLE_CASE_RETURN_STRING('2');
			HANDLE_CASE_RETURN_STRING('3');
			HANDLE_CASE_RETURN_STRING('4');
			HANDLE_CASE_RETURN_STRING('5');
			HANDLE_CASE_RETURN_STRING('6');
			HANDLE_CASE_RETURN_STRING('7');
			HANDLE_CASE_RETURN_STRING('8');
			HANDLE_CASE_RETURN_STRING('9');
			//VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
			HANDLE_CASE_RETURN_STRING('A');
			HANDLE_CASE_RETURN_STRING('B');
			HANDLE_CASE_RETURN_STRING('C');
			HANDLE_CASE_RETURN_STRING('D');
			HANDLE_CASE_RETURN_STRING('E');
			HANDLE_CASE_RETURN_STRING('F');
			HANDLE_CASE_RETURN_STRING('G');
			HANDLE_CASE_RETURN_STRING('H');
			HANDLE_CASE_RETURN_STRING('I');
			HANDLE_CASE_RETURN_STRING('J');
			HANDLE_CASE_RETURN_STRING('K');
			HANDLE_CASE_RETURN_STRING('L');
			HANDLE_CASE_RETURN_STRING('M');
			HANDLE_CASE_RETURN_STRING('N');
			HANDLE_CASE_RETURN_STRING('O');
			HANDLE_CASE_RETURN_STRING('P');
			HANDLE_CASE_RETURN_STRING('Q');
			HANDLE_CASE_RETURN_STRING('R');
			HANDLE_CASE_RETURN_STRING('S');
			HANDLE_CASE_RETURN_STRING('T');
			HANDLE_CASE_RETURN_STRING('U');
			HANDLE_CASE_RETURN_STRING('V');
			HANDLE_CASE_RETURN_STRING('W');
			HANDLE_CASE_RETURN_STRING('X');
			HANDLE_CASE_RETURN_STRING('Y');
			HANDLE_CASE_RETURN_STRING('Z');

			HANDLE_CASE_RETURN_STRING(VK_LWIN);
			HANDLE_CASE_RETURN_STRING(VK_RWIN);
			HANDLE_CASE_RETURN_STRING(VK_APPS);
			HANDLE_CASE_RETURN_STRING(VK_SLEEP);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD0);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD1);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD2);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD3);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD4);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD5);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD6);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD7);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD8);
			HANDLE_CASE_RETURN_STRING(VK_NUMPAD9);
			HANDLE_CASE_RETURN_STRING(VK_MULTIPLY);
			HANDLE_CASE_RETURN_STRING(VK_ADD);
			HANDLE_CASE_RETURN_STRING(VK_SEPARATOR);
			HANDLE_CASE_RETURN_STRING(VK_SUBTRACT);
			HANDLE_CASE_RETURN_STRING(VK_DECIMAL);
			HANDLE_CASE_RETURN_STRING(VK_DIVIDE);
			HANDLE_CASE_RETURN_STRING(VK_F1);
			HANDLE_CASE_RETURN_STRING(VK_F2);
			HANDLE_CASE_RETURN_STRING(VK_F3);
			HANDLE_CASE_RETURN_STRING(VK_F4);
			HANDLE_CASE_RETURN_STRING(VK_F5);
			HANDLE_CASE_RETURN_STRING(VK_F6);
			HANDLE_CASE_RETURN_STRING(VK_F7);
			HANDLE_CASE_RETURN_STRING(VK_F8);
			HANDLE_CASE_RETURN_STRING(VK_F9);
			HANDLE_CASE_RETURN_STRING(VK_F10);
			HANDLE_CASE_RETURN_STRING(VK_F11);
			HANDLE_CASE_RETURN_STRING(VK_F12);
			HANDLE_CASE_RETURN_STRING(VK_F13);
			HANDLE_CASE_RETURN_STRING(VK_F14);
			HANDLE_CASE_RETURN_STRING(VK_F15);
			HANDLE_CASE_RETURN_STRING(VK_F16);
			HANDLE_CASE_RETURN_STRING(VK_F17);
			HANDLE_CASE_RETURN_STRING(VK_F18);
			HANDLE_CASE_RETURN_STRING(VK_F19);
			HANDLE_CASE_RETURN_STRING(VK_F20);
			HANDLE_CASE_RETURN_STRING(VK_F21);
			HANDLE_CASE_RETURN_STRING(VK_F22);
			HANDLE_CASE_RETURN_STRING(VK_F23);
			HANDLE_CASE_RETURN_STRING(VK_F24);
			HANDLE_CASE_RETURN_STRING(VK_NUMLOCK);
			HANDLE_CASE_RETURN_STRING(VK_SCROLL);
			HANDLE_CASE_RETURN_STRING((VK_OEM_NEC_EQUAL | VK_OEM_FJ_JISHO));
			HANDLE_CASE_RETURN_STRING(VK_OEM_FJ_MASSHOU);
			HANDLE_CASE_RETURN_STRING(VK_OEM_FJ_TOUROKU);
			HANDLE_CASE_RETURN_STRING(VK_OEM_FJ_LOYA);
			HANDLE_CASE_RETURN_STRING(VK_OEM_FJ_ROYA);
			HANDLE_CASE_RETURN_STRING(VK_LSHIFT);
			HANDLE_CASE_RETURN_STRING(VK_RSHIFT);
			HANDLE_CASE_RETURN_STRING(VK_LCONTROL);
			HANDLE_CASE_RETURN_STRING(VK_RCONTROL);
			HANDLE_CASE_RETURN_STRING(VK_LMENU);
			HANDLE_CASE_RETURN_STRING(VK_RMENU);
#if(_WIN32_WINNT >= 0x0500)
			HANDLE_CASE_RETURN_STRING(VK_BROWSER_BACK);
			HANDLE_CASE_RETURN_STRING(VK_BROWSER_FORWARD);
			HANDLE_CASE_RETURN_STRING(VK_BROWSER_REFRESH);
			HANDLE_CASE_RETURN_STRING(VK_BROWSER_STOP);
			HANDLE_CASE_RETURN_STRING(VK_BROWSER_SEARCH);
			HANDLE_CASE_RETURN_STRING(VK_BROWSER_FAVORITES);
			HANDLE_CASE_RETURN_STRING(VK_BROWSER_HOME);
			HANDLE_CASE_RETURN_STRING(VK_VOLUME_MUTE);
			HANDLE_CASE_RETURN_STRING(VK_VOLUME_DOWN);
			HANDLE_CASE_RETURN_STRING(VK_VOLUME_UP);
			HANDLE_CASE_RETURN_STRING(VK_MEDIA_NEXT_TRACK);
			HANDLE_CASE_RETURN_STRING(VK_MEDIA_PREV_TRACK);
			HANDLE_CASE_RETURN_STRING(VK_MEDIA_STOP);
			HANDLE_CASE_RETURN_STRING(VK_MEDIA_PLAY_PAUSE);
			HANDLE_CASE_RETURN_STRING(VK_LAUNCH_MAIL);
			HANDLE_CASE_RETURN_STRING(VK_LAUNCH_MEDIA_SELECT);
			HANDLE_CASE_RETURN_STRING(VK_LAUNCH_APP1);
			HANDLE_CASE_RETURN_STRING(VK_LAUNCH_APP2);
#endif /* _WIN32_WINNT >= 0x0500 */
			HANDLE_CASE_RETURN_STRING(VK_OEM_1);		// ';:' for US
			HANDLE_CASE_RETURN_STRING(VK_OEM_PLUS);		// '+' any country
			HANDLE_CASE_RETURN_STRING(VK_OEM_COMMA);	// ',' any country
			HANDLE_CASE_RETURN_STRING(VK_OEM_MINUS);	// '-' any country
			HANDLE_CASE_RETURN_STRING(VK_OEM_PERIOD);	// '.' any country
			HANDLE_CASE_RETURN_STRING(VK_OEM_2);		// '/?' for US
			HANDLE_CASE_RETURN_STRING(VK_OEM_3);		// '`~' for US
			HANDLE_CASE_RETURN_STRING(VK_OEM_4);		//  '[{' for US
			HANDLE_CASE_RETURN_STRING(VK_OEM_5);		//  '\|' for US
			HANDLE_CASE_RETURN_STRING(VK_OEM_6);		//  ']}' for US
			HANDLE_CASE_RETURN_STRING(VK_OEM_7);		//  ''"' for US
			HANDLE_CASE_RETURN_STRING(VK_OEM_8);		//  'AX' key on Japanese AX kbd
			HANDLE_CASE_RETURN_STRING(VK_OEM_AX);		//  "<>" or "\|" on RT 102-key kbd.
			HANDLE_CASE_RETURN_STRING(VK_OEM_102);		//  Help key on ICO
			HANDLE_CASE_RETURN_STRING(VK_ICO_HELP);		//  00 key on ICO
			HANDLE_CASE_RETURN_STRING(VK_ICO_00);
#if(WINVER >= 0x0400)
			HANDLE_CASE_RETURN_STRING(VK_PROCESSKEY);
#endif /* WINVER >= 0x0400 */
			HANDLE_CASE_RETURN_STRING(VK_ICO_CLEAR);
#if(_WIN32_WINNT >= 0x0500)
			HANDLE_CASE_RETURN_STRING(VK_PACKET);
#endif /* _WIN32_WINNT >= 0x0500 */

			//Nokia/Ericsson definitions
			HANDLE_CASE_RETURN_STRING(VK_OEM_RESET);
			HANDLE_CASE_RETURN_STRING(VK_OEM_JUMP);
			HANDLE_CASE_RETURN_STRING(VK_OEM_PA1);
			HANDLE_CASE_RETURN_STRING(VK_OEM_PA2);
			HANDLE_CASE_RETURN_STRING(VK_OEM_PA3);
			HANDLE_CASE_RETURN_STRING(VK_OEM_WSCTRL);
			HANDLE_CASE_RETURN_STRING(VK_OEM_CUSEL);
			HANDLE_CASE_RETURN_STRING(VK_OEM_ATTN);
			HANDLE_CASE_RETURN_STRING(VK_OEM_FINISH);
			HANDLE_CASE_RETURN_STRING(VK_OEM_COPY);
			HANDLE_CASE_RETURN_STRING(VK_OEM_AUTO);
			HANDLE_CASE_RETURN_STRING(VK_OEM_ENLW);
			HANDLE_CASE_RETURN_STRING(VK_OEM_BACKTAB);

			HANDLE_CASE_RETURN_STRING(VK_ATTN);
			HANDLE_CASE_RETURN_STRING(VK_CRSEL);
			HANDLE_CASE_RETURN_STRING(VK_EXSEL);
			HANDLE_CASE_RETURN_STRING(VK_EREOF);
			HANDLE_CASE_RETURN_STRING(VK_PLAY);
			HANDLE_CASE_RETURN_STRING(VK_ZOOM);
			HANDLE_CASE_RETURN_STRING(VK_NONAME);
			HANDLE_CASE_RETURN_STRING(VK_PA1);
			HANDLE_CASE_RETURN_STRING(VK_OEM_CLEAR);
		default:
			FTLTRACEEX(FTL::tlWarning, TEXT("Unknown VirtualKey, 0x%x\n"), nVirtKey);
			return TEXT("Unknown");
		}
	}

    LPCTSTR CFWinUtil::GetNotifyCodeString(HWND hWnd, UINT nCode, LPTSTR pszCommandNotify, int nLength, 
		TranslateWndClassProc pTransProc/* = g_pTranslateWndClassProc*/)
    {
		UNREFERENCED_PARAMETER(hWnd);
		UNREFERENCED_PARAMETER(pszCommandNotify);
		UNREFERENCED_PARAMETER(nLength);
		UNREFERENCED_PARAMETER(pTransProc);

        switch(nCode)
        {
            // generic to all controls
            HANDLE_CASE_RETURN_STRING(NM_FIRST);
            HANDLE_CASE_RETURN_STRING(NM_OUTOFMEMORY);
            HANDLE_CASE_RETURN_STRING(NM_CLICK);
            HANDLE_CASE_RETURN_STRING(NM_DBLCLK);
            HANDLE_CASE_RETURN_STRING(NM_RETURN);
            HANDLE_CASE_RETURN_STRING(NM_RCLICK);
            HANDLE_CASE_RETURN_STRING(NM_RDBLCLK);
            HANDLE_CASE_RETURN_STRING(NM_SETFOCUS);
            HANDLE_CASE_RETURN_STRING(NM_KILLFOCUS);
            HANDLE_CASE_RETURN_STRING(NM_CUSTOMDRAW);
            HANDLE_CASE_RETURN_STRING(NM_HOVER);
            HANDLE_CASE_RETURN_STRING(NM_NCHITTEST);
            HANDLE_CASE_RETURN_STRING(NM_KEYDOWN);
            HANDLE_CASE_RETURN_STRING(NM_RELEASEDCAPTURE);
            HANDLE_CASE_RETURN_STRING(NM_SETCURSOR);
            HANDLE_CASE_RETURN_STRING(NM_CHAR);
#if (_WIN32_IE >= 0x0401)
            HANDLE_CASE_RETURN_STRING(NM_TOOLTIPSCREATED);
#endif //_WIN32_IE >= 0x0401
#if (_WIN32_IE >= 0x0500)
            HANDLE_CASE_RETURN_STRING(NM_LDOWN);
            HANDLE_CASE_RETURN_STRING(NM_RDOWN);
            HANDLE_CASE_RETURN_STRING(NM_THEMECHANGED);
#endif //_WIN32_IE >= 0x0500

#if (_WIN32_WINNT >= 0x0600)
            HANDLE_CASE_RETURN_STRING(NM_FONTCHANGED);
            //HANDLE_CASE_RETURN_STRING(NM_CUSTOMTEXT);
            HANDLE_CASE_RETURN_STRING((NM_TVSTATEIMAGECHANGING | NM_CUSTOMTEXT)); //Same ID
#endif
            HANDLE_CASE_RETURN_STRING(NM_LAST);

            // listview
            HANDLE_CASE_RETURN_STRING(LVN_ITEMCHANGING);
            HANDLE_CASE_RETURN_STRING(LVN_ITEMCHANGED);
            HANDLE_CASE_RETURN_STRING(LVN_INSERTITEM);
            HANDLE_CASE_RETURN_STRING(LVN_DELETEITEM);
            HANDLE_CASE_RETURN_STRING(LVN_DELETEALLITEMS);
            HANDLE_CASE_RETURN_STRING(LVN_BEGINLABELEDITA);
            HANDLE_CASE_RETURN_STRING(LVN_BEGINLABELEDITW);
            HANDLE_CASE_RETURN_STRING(LVN_ENDLABELEDITA);
            HANDLE_CASE_RETURN_STRING(LVN_ENDLABELEDITW);
            HANDLE_CASE_RETURN_STRING(LVN_COLUMNCLICK);
            HANDLE_CASE_RETURN_STRING(LVN_BEGINDRAG);
            HANDLE_CASE_RETURN_STRING(LVN_BEGINRDRAG);
#if (_WIN32_IE >= 0x0300)
            HANDLE_CASE_RETURN_STRING(LVN_ODCACHEHINT);
            HANDLE_CASE_RETURN_STRING(LVN_ODFINDITEMA);
            HANDLE_CASE_RETURN_STRING(LVN_ODFINDITEMW);
            HANDLE_CASE_RETURN_STRING(LVN_ITEMACTIVATE);
            HANDLE_CASE_RETURN_STRING(LVN_ODSTATECHANGED);
#endif      // _WIN32_IE >= 0x0300


#if (_WIN32_IE >= 0x0400)
            HANDLE_CASE_RETURN_STRING(LVN_HOTTRACK);
#endif
            HANDLE_CASE_RETURN_STRING(LVN_GETDISPINFOA);
            HANDLE_CASE_RETURN_STRING(LVN_GETDISPINFOW);
            HANDLE_CASE_RETURN_STRING(LVN_SETDISPINFOA);
            HANDLE_CASE_RETURN_STRING(LVN_SETDISPINFOW);
            HANDLE_CASE_RETURN_STRING(LVN_KEYDOWN);
            HANDLE_CASE_RETURN_STRING(LVN_MARQUEEBEGIN);
            HANDLE_CASE_RETURN_STRING(LVN_GETINFOTIPA);
            HANDLE_CASE_RETURN_STRING(LVN_GETINFOTIPW);
#if (_WIN32_IE >= 0x0500)
#  ifndef LVN_INCREMENTALSEARCHA    
#    define LVN_INCREMENTALSEARCHA  (LVN_FIRST-62)
#  endif 
            HANDLE_CASE_RETURN_STRING(LVN_INCREMENTALSEARCHA);

#  ifndef LVN_INCREMENTALSEARCHW
#    define LVN_INCREMENTALSEARCHW  (LVN_FIRST-63)
#  endif 
            HANDLE_CASE_RETURN_STRING(LVN_INCREMENTALSEARCHW);
#endif //_WIN32_IE >= 0x0500 

#if _WIN32_WINNT >= 0x0600
            HANDLE_CASE_RETURN_STRING(LVN_COLUMNDROPDOWN);
            HANDLE_CASE_RETURN_STRING(LVN_COLUMNOVERFLOWCLICK);
#endif // _WIN32_WINNT >= 0x0600
#if (_WIN32_WINNT >= 0x0501)
            HANDLE_CASE_RETURN_STRING(LVN_BEGINSCROLL);
            HANDLE_CASE_RETURN_STRING(LVN_ENDSCROLL);
#endif
#if _WIN32_WINNT >= 0x0600
            HANDLE_CASE_RETURN_STRING(LVN_LINKCLICK);
            HANDLE_CASE_RETURN_STRING(LVN_GETEMPTYMARKUP);
#endif //_WIN32_WINNT >= 0x0600

            HANDLE_CASE_RETURN_STRING(LVN_LAST);

            // Property sheet reserved      (0U-200U) -  (0U-299U) - see prsht.h

            HANDLE_CASE_RETURN_STRING(PSN_SETACTIVE);
            HANDLE_CASE_RETURN_STRING(PSN_KILLACTIVE);  //PSN_VALIDATE
            HANDLE_CASE_RETURN_STRING(PSN_APPLY);
            HANDLE_CASE_RETURN_STRING(PSN_RESET);       //PSN_CANCEL
            HANDLE_CASE_RETURN_STRING(PSN_HELP);
            HANDLE_CASE_RETURN_STRING(PSN_WIZBACK);
            HANDLE_CASE_RETURN_STRING(PSN_WIZNEXT);
            HANDLE_CASE_RETURN_STRING(PSN_WIZFINISH);
            HANDLE_CASE_RETURN_STRING(PSN_QUERYCANCEL);
#if (_WIN32_IE >= 0x0400)
            HANDLE_CASE_RETURN_STRING(PSN_GETOBJECT);
#endif // 0x0400
#if (_WIN32_IE >= 0x0500)
            HANDLE_CASE_RETURN_STRING(PSN_TRANSLATEACCELERATOR);
            HANDLE_CASE_RETURN_STRING(PSN_QUERYINITIALFOCUS);
#endif // 0x0500

            // header
            HANDLE_CASE_RETURN_STRING(HDN_ITEMCHANGINGA);
            HANDLE_CASE_RETURN_STRING(HDN_ITEMCHANGINGW);
            HANDLE_CASE_RETURN_STRING(HDN_ITEMCHANGEDA);
            HANDLE_CASE_RETURN_STRING(HDN_ITEMCHANGEDW);
            HANDLE_CASE_RETURN_STRING(HDN_ITEMCLICKA);
            HANDLE_CASE_RETURN_STRING(HDN_ITEMCLICKW);
            HANDLE_CASE_RETURN_STRING(HDN_ITEMDBLCLICKA);
            HANDLE_CASE_RETURN_STRING(HDN_ITEMDBLCLICKW);
            HANDLE_CASE_RETURN_STRING(HDN_DIVIDERDBLCLICKA);
            HANDLE_CASE_RETURN_STRING(HDN_DIVIDERDBLCLICKW);
            HANDLE_CASE_RETURN_STRING(HDN_BEGINTRACKA);
            HANDLE_CASE_RETURN_STRING(HDN_BEGINTRACKW);
            HANDLE_CASE_RETURN_STRING(HDN_ENDTRACKA);
            HANDLE_CASE_RETURN_STRING(HDN_ENDTRACKW);
            HANDLE_CASE_RETURN_STRING(HDN_TRACKA);
            HANDLE_CASE_RETURN_STRING(HDN_TRACKW);
#if (_WIN32_IE >= 0x0300)
            HANDLE_CASE_RETURN_STRING(HDN_GETDISPINFOA);
            HANDLE_CASE_RETURN_STRING(HDN_GETDISPINFOW);
            HANDLE_CASE_RETURN_STRING(HDN_BEGINDRAG);
            HANDLE_CASE_RETURN_STRING(HDN_ENDDRAG);
#endif //_WIN32_IE
#if (_WIN32_IE >= 0x0500)
            HANDLE_CASE_RETURN_STRING(HDN_FILTERCHANGE);
            HANDLE_CASE_RETURN_STRING(HDN_FILTERBTNCLICK);
#endif //0x0500
#if (_WIN32_IE >= 0x0600)
            HANDLE_CASE_RETURN_STRING(HDN_BEGINFILTEREDIT);
            HANDLE_CASE_RETURN_STRING(HDN_ENDFILTEREDIT);
#endif
#if _WIN32_WINNT >= 0x0600
            HANDLE_CASE_RETURN_STRING(HDN_ITEMSTATEICONCLICK);
            HANDLE_CASE_RETURN_STRING(HDN_ITEMKEYDOWN);
            HANDLE_CASE_RETURN_STRING(HDN_DROPDOWN);
            HANDLE_CASE_RETURN_STRING(HDN_OVERFLOWCLICK);
#endif

            // treeview
            HANDLE_CASE_RETURN_STRING(TVN_SELCHANGINGA);
            HANDLE_CASE_RETURN_STRING(TVN_SELCHANGINGW);
            HANDLE_CASE_RETURN_STRING(TVN_SELCHANGEDA);
            HANDLE_CASE_RETURN_STRING(TVN_SELCHANGEDW);
            HANDLE_CASE_RETURN_STRING(TVN_GETDISPINFOA);
            HANDLE_CASE_RETURN_STRING(TVN_GETDISPINFOW);
            HANDLE_CASE_RETURN_STRING(TVN_SETDISPINFOA);
            HANDLE_CASE_RETURN_STRING(TVN_SETDISPINFOW);
            HANDLE_CASE_RETURN_STRING(TVN_ITEMEXPANDINGA);
            HANDLE_CASE_RETURN_STRING(TVN_ITEMEXPANDINGW);
            HANDLE_CASE_RETURN_STRING(TVN_ITEMEXPANDEDA);
            HANDLE_CASE_RETURN_STRING(TVN_ITEMEXPANDEDW);
            HANDLE_CASE_RETURN_STRING(TVN_BEGINDRAGA);
            HANDLE_CASE_RETURN_STRING(TVN_BEGINDRAGW);
            HANDLE_CASE_RETURN_STRING(TVN_BEGINRDRAGA);
            HANDLE_CASE_RETURN_STRING(TVN_BEGINRDRAGW);
            HANDLE_CASE_RETURN_STRING(TVN_DELETEITEMA);
            HANDLE_CASE_RETURN_STRING(TVN_DELETEITEMW);
            HANDLE_CASE_RETURN_STRING(TVN_BEGINLABELEDITA);
            HANDLE_CASE_RETURN_STRING(TVN_BEGINLABELEDITW);
            HANDLE_CASE_RETURN_STRING(TVN_ENDLABELEDITA);
            HANDLE_CASE_RETURN_STRING(TVN_ENDLABELEDITW);
            HANDLE_CASE_RETURN_STRING(TVN_KEYDOWN);
#if (_WIN32_IE >= 0x0400)
            HANDLE_CASE_RETURN_STRING(TVN_GETINFOTIPA);
            HANDLE_CASE_RETURN_STRING(TVN_GETINFOTIPW);
            HANDLE_CASE_RETURN_STRING(TVN_SINGLEEXPAND);
#endif // 0x400
#if (_WIN32_IE >= 0x0600)
            HANDLE_CASE_RETURN_STRING(TVN_ITEMCHANGINGA);
            HANDLE_CASE_RETURN_STRING(TVN_ITEMCHANGINGW);
            HANDLE_CASE_RETURN_STRING(TVN_ITEMCHANGEDA);
            HANDLE_CASE_RETURN_STRING(TVN_ITEMCHANGEDW);
            HANDLE_CASE_RETURN_STRING(TVN_ASYNCDRAW);
#endif // 0x0600

            //tooltips
            HANDLE_CASE_RETURN_STRING(TTN_GETDISPINFOA);
            HANDLE_CASE_RETURN_STRING(TTN_GETDISPINFOW);
            HANDLE_CASE_RETURN_STRING(TTN_SHOW);
            HANDLE_CASE_RETURN_STRING(TTN_POP);
            HANDLE_CASE_RETURN_STRING(TTN_LINKCLICK);

            //tab control
            HANDLE_CASE_RETURN_STRING(TCN_KEYDOWN);
            HANDLE_CASE_RETURN_STRING(TCN_SELCHANGE);
            HANDLE_CASE_RETURN_STRING(TCN_SELCHANGING);
#if (_WIN32_IE >= 0x0400)
            HANDLE_CASE_RETURN_STRING(TCN_GETOBJECT);
#endif      // _WIN32_IE >= 0x0400
#if (_WIN32_IE >= 0x0500)
            HANDLE_CASE_RETURN_STRING(TCN_FOCUSCHANGE);
#endif      // _WIN32_IE >= 0x0500


            // Shell reserved               (0U-580U) -  (0U-589U)

            // common dialog (new)

            // toolbar
            HANDLE_CASE_RETURN_STRING(TBN_GETBUTTONINFOA);
            HANDLE_CASE_RETURN_STRING(TBN_BEGINDRAG);
            HANDLE_CASE_RETURN_STRING(TBN_ENDDRAG);
            HANDLE_CASE_RETURN_STRING(TBN_BEGINADJUST);
            HANDLE_CASE_RETURN_STRING(TBN_ENDADJUST);
            HANDLE_CASE_RETURN_STRING(TBN_RESET);
            HANDLE_CASE_RETURN_STRING(TBN_QUERYINSERT);
            HANDLE_CASE_RETURN_STRING(TBN_QUERYDELETE);
            HANDLE_CASE_RETURN_STRING(TBN_TOOLBARCHANGE);
            HANDLE_CASE_RETURN_STRING(TBN_CUSTHELP);
#if (_WIN32_IE >= 0x0300)
            HANDLE_CASE_RETURN_STRING(TBN_DROPDOWN);
#endif //0x0300
#if (_WIN32_IE >= 0x0400)
            HANDLE_CASE_RETURN_STRING(TBN_GETOBJECT);
#endif //0x0400

            HANDLE_CASE_RETURN_STRING(TBN_HOTITEMCHANGE);
            HANDLE_CASE_RETURN_STRING(TBN_DRAGOUT);
            HANDLE_CASE_RETURN_STRING(TBN_DELETINGBUTTON);
            HANDLE_CASE_RETURN_STRING(TBN_GETDISPINFOA);
            HANDLE_CASE_RETURN_STRING(TBN_GETDISPINFOW);
            HANDLE_CASE_RETURN_STRING(TBN_GETINFOTIPA);
            HANDLE_CASE_RETURN_STRING(TBN_GETINFOTIPW);
            HANDLE_CASE_RETURN_STRING(TBN_GETBUTTONINFOW);
#if (_WIN32_IE >= 0x0500)
            HANDLE_CASE_RETURN_STRING(TBN_RESTORE);
            //HANDLE_CASE_RETURN_STRING(TBN_SAVE);
            HANDLE_CASE_RETURN_STRING(TBN_INITCUSTOMIZE);
#endif // (_WIN32_IE >= 0x0500)

            // updown
            HANDLE_CASE_RETURN_STRING(UDN_DELTAPOS); //注意：UDN_DELTAPOS 和 TBN_SAVE 的值一样（TBN_XXX 的值超过了 TBN_LAST）

            // datetimepick
            HANDLE_CASE_RETURN_STRING(DTN_USERSTRINGA);
            HANDLE_CASE_RETURN_STRING(DTN_USERSTRINGW);
            HANDLE_CASE_RETURN_STRING(DTN_WMKEYDOWNA);
            HANDLE_CASE_RETURN_STRING(DTN_WMKEYDOWNW);
            HANDLE_CASE_RETURN_STRING(DTN_FORMATA);
            HANDLE_CASE_RETURN_STRING(DTN_FORMATW);
            HANDLE_CASE_RETURN_STRING(DTN_FORMATQUERYA);
            HANDLE_CASE_RETURN_STRING(DTN_FORMATQUERYW);

            // monthcal
            HANDLE_CASE_RETURN_STRING(MCN_SELCHANGE);
            HANDLE_CASE_RETURN_STRING(MCN_GETDAYSTATE);
            HANDLE_CASE_RETURN_STRING(MCN_SELECT);
            //HANDLE_CASE_RETURN_STRING(MCN_VIEWCHANGE);

            // datetimepick2
            HANDLE_CASE_RETURN_STRING(DTN_DATETIMECHANGE);
            HANDLE_CASE_RETURN_STRING(DTN_DROPDOWN);
            HANDLE_CASE_RETURN_STRING(DTN_CLOSEUP);

            // combo box ex
            HANDLE_CASE_RETURN_STRING(CBEN_GETDISPINFOA);
            HANDLE_CASE_RETURN_STRING(CBEN_INSERTITEM);
            HANDLE_CASE_RETURN_STRING(CBEN_DELETEITEM);
            HANDLE_CASE_RETURN_STRING(CBEN_BEGINEDIT);
            HANDLE_CASE_RETURN_STRING(CBEN_ENDEDITA);
            HANDLE_CASE_RETURN_STRING(CBEN_ENDEDITW);
            HANDLE_CASE_RETURN_STRING(CBEN_GETDISPINFOW);
            HANDLE_CASE_RETURN_STRING(CBEN_DRAGBEGINA);
            HANDLE_CASE_RETURN_STRING(CBEN_DRAGBEGINW);

            // rebar
            HANDLE_CASE_RETURN_STRING(RBN_HEIGHTCHANGE);
#if (_WIN32_IE >= 0x0400)
            HANDLE_CASE_RETURN_STRING(RBN_GETOBJECT);
            HANDLE_CASE_RETURN_STRING(RBN_LAYOUTCHANGED);
            HANDLE_CASE_RETURN_STRING(RBN_AUTOSIZE);
            HANDLE_CASE_RETURN_STRING(RBN_BEGINDRAG);
            HANDLE_CASE_RETURN_STRING(RBN_ENDDRAG);
            HANDLE_CASE_RETURN_STRING(RBN_DELETINGBAND);
            HANDLE_CASE_RETURN_STRING(RBN_DELETEDBAND);
            HANDLE_CASE_RETURN_STRING(RBN_CHILDSIZE);
#if (_WIN32_IE >= 0x0500)
            HANDLE_CASE_RETURN_STRING(RBN_CHEVRONPUSHED);
#endif      // _WIN32_IE >= 0x0500
#if (_WIN32_IE >= 0x0600)
            HANDLE_CASE_RETURN_STRING(RBN_SPLITTERDRAG);
#endif      // _WIN32_IE >= 0x0600
#if (_WIN32_IE >= 0x0500)
            HANDLE_CASE_RETURN_STRING(RBN_MINMAX);
#endif      // _WIN32_IE >= 0x0500
#if (_WIN32_WINNT >= 0x0501)
            HANDLE_CASE_RETURN_STRING(RBN_AUTOBREAK);
#endif //0x0501
#endif //0x0400


#if (_WIN32_IE >= 0x0400)
            // internet address
            HANDLE_CASE_RETURN_STRING(IPN_FIELDCHANGED);

            // status bar
            HANDLE_CASE_RETURN_STRING(SBN_SIMPLEMODECHANGE);

            // Pager Control
            HANDLE_CASE_RETURN_STRING(PGN_SCROLL);
            HANDLE_CASE_RETURN_STRING(PGN_CALCSIZE);
            HANDLE_CASE_RETURN_STRING(PGN_HOTITEMCHANGE);
#endif //_WIN32_IE >= 0x0400

#if (_WIN32_IE >= 0x0500)
            //WMN_FIRST
#endif //_WIN32_IE >= 0x0500

#if (_WIN32_WINNT >= 0x0501)
            //BCN_FIRST
#  ifndef NM_GETCUSTOMSPLITRECT
#    define NM_GETCUSTOMSPLITRECT       (BCN_FIRST + 0x0003)
#  endif
            HANDLE_CASE_RETURN_STRING(NM_GETCUSTOMSPLITRECT);
            HANDLE_CASE_RETURN_STRING(BCN_HOTITEMCHANGE);
#endif //_WIN32_WINNT >= 0x0501

#if (_WIN32_WINNT >= 0x600)
            HANDLE_CASE_RETURN_STRING(BCN_DROPDOWN);
#endif // _WIN32_WINNT >= 0x600


#if (_WIN32_WINNT >= 0x0600)
            // trackbar
            HANDLE_CASE_RETURN_STRING(TRBN_THUMBPOSCHANGING);
#endif //_WIN32_WINNT >= 0x0600
        }

        FTLTRACEEX(FTL::tlWarning, TEXT("Unknown Notify Code, %d\n"), nCode);
        return TEXT("Unknown");
    }

    LPCTSTR CFWinUtil::GetCommandNotifyString(HWND hWnd, UINT nCode, LPTSTR pszCommandNotify, int nLength, 
		TranslateWndClassProc pTransProc/* = g_pTranslateWndClassProc*/)
    {
		CHECK_POINTER_RETURN_VALUE_IF_FAIL(pszCommandNotify, NULL);
		BOOL bRet = FALSE;
		HRESULT hr = E_FAIL;

		pszCommandNotify[0] = 0;
		TCHAR szClassName[FTL_MAX_CLASS_NAME_LENGTH] = {0};
		API_VERIFY( 0 != GetClassName(hWnd, szClassName, _countof(szClassName)));
		if (pTransProc)
		{
			TCHAR szNewClassName[FTL_MAX_CLASS_NAME_LENGTH] = {0};
			API_VERIFY((*pTransProc)(szClassName, szNewClassName, _countof(szNewClassName)));
			if (bRet)
			{
				FTLTRACEEX(FTL::tlInfo, TEXT("Translate Window Class Name From %s to %s\n"), szClassName, szNewClassName);
				StringCchCopy(szClassName, _countof(szClassName), szNewClassName);
			}
		}
		if (0 == lstrcmpi(szClassName, TEXT("Button")))
		{
			switch(nCode)
			{
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, BN_CLICKED);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, BN_PAINT);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, BN_HILITE);	//BN_PUSHED
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, BN_UNHILITE);	//BN_UNPUSHED
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, BN_DISABLE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, BN_DOUBLECLICKED);	//BN_DBLCLK
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, BN_SETFOCUS);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, BN_KILLFOCUS);
			default:
				break;
			}
		}
		else if (0 == lstrcmpi(szClassName, TEXT("ComboBox")))
		{
			//Combo Box Notification Codes
			switch(nCode)
			{
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_ERRSPACE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_SELCHANGE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_DBLCLK);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_SETFOCUS);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_KILLFOCUS);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_EDITCHANGE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_EDITUPDATE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_DROPDOWN);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_CLOSEUP);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_SELENDOK);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, CBN_SELENDCANCEL);
			default:
				break;
			}
        }
		else if (0 == lstrcmpi(szClassName, TEXT("ListBox")))
		{
			//Combo Box Notification Codes
			switch(nCode)
			{
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, LBN_SELCHANGE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, LBN_DBLCLK);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, LBN_SELCANCEL);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, LBN_SETFOCUS);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, LBN_KILLFOCUS);
			default:
				break;
			}
		}
		else if (0 == lstrcmpi(szClassName, TEXT("Edit")))
		{
			switch(nCode)
			{
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_SETFOCUS);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_KILLFOCUS);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_CHANGE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_UPDATE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_ERRSPACE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_MAXTEXT);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_HSCROLL);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_VSCROLL);
#if(_WIN32_WINNT >= 0x0500)
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_ALIGN_LTR_EC);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_ALIGN_RTL_EC);
#endif /* _WIN32_WINNT >= 0x0500 */
			default:
				break;
			}
		}
		else if(0 == lstrcmpi(szClassName, TEXT("RichEdit20W")))
		{
#ifdef _RICHEDIT_
			switch (nCode)
			{
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_MSGFILTER);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_REQUESTRESIZE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_SELCHANGE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_DROPFILES);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_PROTECTED);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_CORRECTTEXT);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_STOPNOUNDO);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_IMECHANGE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_SAVECLIPBOARD);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_OLEOPFAILED);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_OBJECTPOSITIONS);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_LINK);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_DRAGDROPDONE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_PARAGRAPHEXPANDED);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_PAGECHANGE);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_LOWFIRTF);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_ALIGNLTR);
				HANDLE_CASE_TO_STRING(pszCommandNotify, nLength, EN_ALIGNRTL);
			default:
				//StringCchCopy(pszCommandNotify,nLength,TEXT("Unknown RichEdit Notify"));
				break;
			}
#endif	//_RICHEDIT_
		}

		if ( 0 == pszCommandNotify[0] )
		{
			FTLTRACEEX(FTL::tlWarning, TEXT("Warning -- Unknown Command Code %d For Class %s\n"), nCode, szClassName);
			COM_VERIFY(StringCchPrintf(pszCommandNotify, nLength, TEXT("Unknown Command Code %d For Class %s"), nCode, szClassName));
			//FTLASSERT(FALSE);
		}
		return pszCommandNotify;
    }

	LPCTSTR CFWinUtil::GetWindowDescriptionInfo(FTL::CFStringFormater& formater, HWND hWnd)
	{
		BOOL bRet = FALSE;
		HRESULT hr = E_FAIL;
		if (::IsWindow(hWnd))
		{
			TCHAR szClass[FTL_MAX_CLASS_NAME_LENGTH] = {0};
			API_VERIFY(0 != GetClassName(hWnd, szClass, _countof(szClass)));

			TCHAR szName[FTL_MAX_CLASS_NAME_LENGTH] = {0};
			API_VERIFY_EXCEPT1(0 != GetWindowText(hWnd, szName, _countof(szName)), ERROR_SUCCESS);

			RECT rcWindow = {0};
			API_VERIFY(GetWindowRect(hWnd, &rcWindow));

			COM_VERIFY(formater.Format(TEXT("0x%x(%d), Class=%s, Name=%s, WinPos=(%d,%d)-(%d,%d) %dx%d"),
				hWnd, hWnd, szClass, szName, 
				rcWindow.left, rcWindow.top, rcWindow.right, rcWindow.bottom,
				rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top));
		}
		else
		{
			COM_VERIFY(formater.Format(TEXT("0x%x(%d) NOT valid Window"), hWnd, hWnd));
		}

		return formater.GetString();
	}

    LPCTSTR CFWinUtil::GetWindowClassString(FTL::CFStringFormater& formater, HWND hWnd,LPCTSTR pszDivide/* = TEXT("|") */)
    {
        FTLASSERT(::IsWindow(hWnd));
        ULONG_PTR clsStyle = ::GetClassLongPtr(hWnd,GCL_STYLE);
        API_ASSERT(clsStyle != 0);
        ULONG_PTR oldClsStyle = clsStyle;

        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_VREDRAW, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_HREDRAW, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_DBLCLKS, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_OWNDC, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_CLASSDC, pszDivide);

        //* 如子窗口设置了 CS_PARENTDC 属性,它可在其父窗口的显示设备上下文上进行绘制 -- 如 Edit/Button
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_PARENTDC, pszDivide);

        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_NOCLOSE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_SAVEBITS, pszDivide);
		//在字节边界上定位窗口的用户区域的位置 -- 有什么用？
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_BYTEALIGNCLIENT, pszDivide);
		//在字节边界上定位窗口的位置 -- 有什么用？
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_BYTEALIGNWINDOW, pszDivide);
		//应用程序全局的窗体类--可以被Exe或Dll注册，对进程内所有模块都有效，通常在提供UI的DLL中注册
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_GLOBALCLASS, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_IME, pszDivide);
#if(_WIN32_WINNT >= 0x0501)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, clsStyle, CS_DROPSHADOW, pszDivide);
#endif /* _WIN32_WINNT >= 0x0501 */

        FTLASSERT( 0 == clsStyle);
        //HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, XXXXXXXXX, pszDivide);
        if (0 != clsStyle)
        {
            FTLTRACEEX(FTL::tlWarning, TEXT("%s:Check Class String For HWND(0x%x) Not Complete, total=0x%x, remain=0x%x\n"),
                __FILE__LINE__, hWnd, oldClsStyle, clsStyle);
        }

        return formater.GetString();
    }

    LPCTSTR CFWinUtil::GetWindowStyleString(FTL::CFStringFormater& formater, HWND hWnd,LPCTSTR pszDivide/* = TEXT("|") */)
    {
        BOOL bRet = FALSE;

        FTLASSERT(::IsWindow(hWnd));
        LONG_PTR    lStyle = ::GetWindowLongPtr(hWnd, GWL_STYLE);
        LONG_PTR    lOldStyle = lStyle;
        //HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_OVERLAPPED, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_POPUP, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_CHILDWINDOW, pszDivide); //WS_CHILD
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_MINIMIZE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_VISIBLE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_DISABLED, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_CLIPSIBLINGS, pszDivide); //兄弟子窗口互相裁剪(只用于WS_CHILD)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_CLIPCHILDREN, pszDivide); //父窗口中不绘制子窗口(裁剪视频播放窗体)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_MAXIMIZE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_CAPTION, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_BORDER, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_DLGFRAME, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_VSCROLL, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_HSCROLL, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_SYSMENU, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_THICKFRAME, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_GROUP, pszDivide);      //WS_MINIMIZEBOX
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_TABSTOP, pszDivide);    //WS_MAXIMIZEBOX

        //通用的类型都大于 0x00010000L， 各种标准控件特有的 Style 小于 0x00010000L

        TCHAR szClassName[256+1] = {0}; //The maximum length for lpszClassName is 256.
        API_VERIFY(0 != ::GetClassName(hWnd, szClassName, _countof(szClassName)));
        
        //BUTTON
        if (0 ==  lstrcmpi(szClassName, WC_BUTTON))
        {
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_LEFTTEXT, pszDivide);

			if (0 != (lStyle & BS_TYPEMASK))
			{
#if _WIN32_WINNT >= 0x0600
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_DEFCOMMANDLINK, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_COMMANDLINK, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_DEFSPLITBUTTON, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_SPLITBUTTON, pszDivide);
#endif //_WIN32_WINNT >= 0x0600
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_OWNERDRAW, pszDivide); //按钮的自绘
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_PUSHBOX, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_AUTORADIOBUTTON, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_USERBUTTON, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_GROUPBOX, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_AUTO3STATE, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_3STATE, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_RADIOBUTTON, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_AUTOCHECKBOX, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_CHECKBOX, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_DEFPUSHBUTTON, pszDivide);
			}
			else
			{
				//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_PUSHBUTTON, pszDivide);
				formater.AppendFormat(TEXT("%s%s"), TEXT("BS_PUSHBUTTON"), pszDivide);
			}

            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_FLAT, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_NOTIFY, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_MULTILINE, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_PUSHLIKE, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_VCENTER, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_BOTTOM, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_TOP, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_CENTER, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_RIGHT, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_LEFT, pszDivide);

			if ( 0 != (lStyle & (BS_ICON|BS_TEXT)))
			{
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_BITMAP, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_ICON, pszDivide);
			}
			else
			{
				//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_TEXT, pszDivide);
				formater.AppendFormat(TEXT("%s%s"), TEXT("BS_TEXT"), pszDivide);
			}
        }

		//Combo Box styles
		if (0 ==  lstrcmp(szClassName, WC_COMBOBOX))
		{
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_DROPDOWNLIST, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_DROPDOWN, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_SIMPLE, pszDivide);

			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_OWNERDRAWFIXED, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_OWNERDRAWVARIABLE, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_AUTOHSCROLL, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_OEMCONVERT, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_SORT, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_HASSTRINGS, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_NOINTEGRALHEIGHT, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_DISABLENOSCROLL, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_UPPERCASE, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, CBS_LOWERCASE, pszDivide);
		}

		if (0 ==  lstrcmp(szClassName, WC_EDIT)                     //Edit Control Styles
            || 0 == lstrcmp(szClassName, TEXT("RICHEDIT50W")))      //Rich Edit Control Styles
		{
			#define ES_ALIGNMASK	(ES_LEFT | ES_CENTER | ES_RIGHT)
			if ( 0 != (lStyle & ES_ALIGNMASK))
			{
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_RIGHT, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_CENTER, pszDivide);
			}
			else
			{
				//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_LEFT, pszDivide);
				formater.AppendFormat(TEXT("%s%s"), TEXT("ES_LEFT"), pszDivide);
			}

			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_MULTILINE, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_UPPERCASE | ES_NOOLEDRAGDROP, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_LOWERCASE, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_PASSWORD, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_AUTOVSCROLL, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_AUTOHSCROLL, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_NOHIDESEL, pszDivide);

			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_OEMCONVERT, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_READONLY, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_WANTRETURN, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_NUMBER | ES_DISABLENOSCROLL, pszDivide); //两个的值一样，为什么
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_SUNKEN, pszDivide); 
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_SAVESEL, pszDivide);

            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_NOIME, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_VERTICAL, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, ES_SELECTIONBAR, pszDivide);
		}

        //ListView
        if (0 ==  lstrcmp(szClassName, WC_LISTVIEW))
        {
			if (0 != (lStyle & LVS_TYPEMASK))
			{
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_LIST, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SMALLICON, pszDivide);
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_REPORT, pszDivide);
			}
			else
			{
				//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_ICON, pszDivide);
				formater.AppendFormat(TEXT("%s%s"), TEXT("LVS_ICON"), pszDivide);
			}

			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SINGLESEL, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SHOWSELALWAYS, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SORTASCENDING, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SORTDESCENDING, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SHAREIMAGELISTS, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_NOLABELWRAP, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_AUTOARRANGE, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_EDITLABELS, pszDivide);

			//序列表(虚拟列表)技术 -- 在显示的时候才获取具体的信息，可以大幅减少UI资源的消耗
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_OWNERDATA, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_NOSCROLL, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_TYPESTYLEMASK, pszDivide);

			if (0 != (lStyle & LVS_ALIGNMASK ))
			{
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_ALIGNLEFT, pszDivide);
			}
			else
			{
				//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_ALIGNTOP, pszDivide);
				formater.AppendFormat(TEXT("%s%s"), TEXT("LVS_ALIGNTOP"), pszDivide);
			}

			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_NOSORTHEADER, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_NOCOLUMNHEADER, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_OWNERDRAWFIXED, pszDivide);
        }

		//Scroll Bar Styles
		if (0 ==  lstrcmp(szClassName, WC_SCROLLBAR))
		{
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, SBS_SIZEGRIP, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, SBS_SIZEBOX, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, SBS_BOTTOMALIGN|SBS_RIGHTALIGN|SBS_SIZEBOXBOTTOMRIGHTALIGN, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, SBS_TOPALIGN|SBS_LEFTALIGN|SBS_SIZEBOXTOPLEFTALIGN, pszDivide);
			if (0 != (lStyle & SBS_VERT))
			{
				HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, SBS_VERT, pszDivide);
			}
			else
			{
				//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, SBS_HORZ, pszDivide);
				formater.AppendFormat(TEXT("%s%s"), TEXT("SBS_HORZ"), pszDivide);
			}
		}

		//SysHeader32 Box styles
		if (0 ==  lstrcmp(szClassName, WC_HEADER))
		{
			//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_HORZ, pszDivide);
			formater.AppendFormat(TEXT("%s%s"), TEXT("HDS_HORZ"), pszDivide);

			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_BUTTONS, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_HOTTRACK, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_HIDDEN, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_DRAGDROP, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_FULLDRAG, pszDivide);
#if (_WIN32_IE >= 0x0500)
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_FILTERBAR, pszDivide);
#endif //_WIN32_IE >= 0x0500

#if (_WIN32_WINNT >= 0x0501)
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_FLAT, pszDivide);
#endif //_WIN32_WINNT >= 0x0501
#if _WIN32_WINNT >= 0x0600
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_CHECKBOXES, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_NOSIZING, pszDivide);
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, HDS_OVERFLOW, pszDivide);
#endif
		}

        FTLASSERT( 0 == lStyle);
		//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, XXXXXXXXX, pszDivide);
        if (0 != lStyle)
        {
            FTLTRACEEX(FTL::tlWarning, TEXT("%s:Check Style String For \"%s\" Not Complete, total=0x%08x, remain=0x%08x\n"),
                __FILE__LINE__, szClassName, lOldStyle, lStyle);
        }
        return formater.GetString();
    }

    LPCTSTR CFWinUtil::GetWindowExStyleString(FTL::CFStringFormater& formater, HWND hWnd, LPCTSTR pszDivide /* = TEXT */)
    {
        FTLASSERT(::IsWindow(hWnd));
        LONG_PTR    lExStyle = ::GetWindowLongPtr(hWnd, GWL_EXSTYLE);
        LONG_PTR    lOldExStyle = lExStyle;

        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_DLGMODALFRAME, pszDivide);
		//当该窗体(Child Window)创建或销毁时不会给父窗体发送 WM_PARENTNOTIFY 
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_NOPARENTNOTIFY, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_TOPMOST, pszDivide);
		//可以接收 drag-and-drop 文件(会收到 WM_DROPFILES 消息)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_ACCEPTFILES, pszDivide);
		//透明的，不会掩盖其下方的窗体，鼠标消息会穿透此窗体(典型应用是阴影)，会在其下发的所有窗体都更新完毕后收到 WM_PAINT 消息
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_TRANSPARENT, pszDivide);	
#if(WINVER >= 0x0400)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_MDICHILD, pszDivide);

		//通常用于浮动工具条(floating toolbar) -- 小的标题栏，不出现在任务栏和 Alt+Tab 列表中
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_TOOLWINDOW, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_WINDOWEDGE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_CLIENTEDGE, pszDivide);	//有3D客户区外观，即有一个凹边(sunken edge)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_CONTEXTHELP, pszDivide);
		if (0 != (lExStyle & WS_EX_RIGHT) )
		{
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_RIGHT, pszDivide);
		}
		else
		{
			//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_LEFT, pszDivide);
			formater.AppendFormat(TEXT("%s%s"), TEXT("WS_EX_LEFT"), pszDivide);
		}
		if (0 != (lExStyle & WS_EX_RTLREADING))
		{
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_RTLREADING, pszDivide);
		}
		else
		{
			//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_LTRREADING, pszDivide);
			formater.AppendFormat(TEXT("%s%s"), TEXT("WS_EX_LTRREADING"), pszDivide);
		}
		if (0 != (lExStyle & WS_EX_LEFTSCROLLBAR))
		{
			HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_LEFTSCROLLBAR, pszDivide);
		}
		else
		{
			//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_RIGHTSCROLLBAR, pszDivide);
			formater.AppendFormat(TEXT("%s%s"), TEXT("WS_EX_RIGHTSCROLLBAR"), pszDivide);
		}
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_CONTROLPARENT, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_STATICEDGE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_APPWINDOW, pszDivide);	//当激活时，任务条上会出现 Top-Level窗体
#endif /* WINVER >= 0x0400 */

#if(_WIN32_WINNT >= 0x0500)
        //创建分层窗口
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_LAYERED, pszDivide);
#endif /* _WIN32_WINNT >= 0x0500 */

#if(WINVER >= 0x0500)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_NOINHERITLAYOUT, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_LAYOUTRTL, pszDivide);
#endif /* WINVER >= 0x0500 */

#if(_WIN32_WINNT >= 0x0501)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_COMPOSITED, pszDivide);
#endif /* _WIN32_WINNT >= 0x0501 */

#if(_WIN32_WINNT >= 0x0500)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_NOACTIVATE, pszDivide);
#endif /* _WIN32_WINNT >= 0x0500 */

        FTLASSERT( 0 == lExStyle);
        if (0 != lExStyle)
        {
            FTLTRACEEX(FTL::tlWarning, TEXT("%s: Check ExStyle String Not Complete, total=0x%08x, remain=0x%08x\n"),
                __FILE__LINE__, lOldExStyle, lExStyle);
        }
        return formater.GetString();
    }

	LPCTSTR CFWinUtil::GetWindowPosFlagsString(FTL::CFStringFormater& formater, UINT flags, LPCTSTR pszDivide /* = TEXT */)
	{
		UINT    nOldFlags = flags;

		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOSIZE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOMOVE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOZORDER, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOREDRAW, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOACTIVATE, pszDivide);
		
		//更改新的边框类型(比如 DUILib中去掉 WS_CAPTION 后)
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_FRAMECHANGED, pszDivide);

		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_SHOWWINDOW, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_HIDEWINDOW, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOCOPYBITS, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOOWNERZORDER, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOSENDCHANGING, pszDivide);


#if(WINVER >= 0x0400)
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_DEFERERASE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_ASYNCWINDOWPOS, pszDivide);
#endif /* WINVER >= 0x0400 */

#ifndef SWP_NOCLIENTSIZE 
#  define SWP_NOCLIENTSIZE  0x0800		//Undocumented flags
#endif 
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOCLIENTSIZE, pszDivide);

#ifndef SWP_NOCLIENTMOVE
#  define SWP_NOCLIENTMOVE	0x1000		//Undocumented flags
#endif 
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_NOCLIENTMOVE, pszDivide);

#ifndef SWP_STATECHANGED  // window state (e.g. minimized, normalized, maximized) is changing or has changed,
#  define SWP_STATECHANGED  0x8000		//Undocumented flags
#endif 
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_STATECHANGED, pszDivide);

#ifndef SWP_UNKNOWN_0X01000000
#  define SWP_UNKNOWN_0X01000000  0x01000000	//Undocumented flags
#endif 
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_UNKNOWN_0X01000000, pszDivide);

#ifndef SWP_UNKNOWN_0X10000000
#  define SWP_UNKNOWN_0X10000000  0x10000000	//Undocumented flags
#endif 
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_UNKNOWN_0X10000000, pszDivide);

#ifndef SWP_UNKNOWN_0X20000000
#  define SWP_UNKNOWN_0X20000000  0x20000000	//Undocumented flags
#endif 
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_UNKNOWN_0X20000000, pszDivide);

#ifndef SWP_UNKNOWN_0X40000000
#  define SWP_UNKNOWN_0X40000000  0x40000000	//Undocumented flags
#endif 
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, flags, SWP_UNKNOWN_0X40000000, pszDivide);

		FTLASSERT( 0 == flags);
		if (0 != flags)
		{
			FTLTRACEEX(FTL::tlWarning, TEXT("%s: WARNING, Check Set Window Pos String Not Complete, total=0x%08x, remain=0x%08x\n"),
				__FILE__LINE__, nOldFlags, flags);
		}
		return formater.GetString();
	}

	LPCTSTR CFWinUtil::GetOwnerDrawState(FTL::CFStringFormater& formater, UINT itemState, LPCTSTR pszDivide)
	{
		UINT    oldItemState = itemState;

		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_SELECTED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_GRAYED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_DISABLED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_CHECKED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_FOCUS, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_DEFAULT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_COMBOBOXEDIT, pszDivide);
#if(WINVER >= 0x0500)
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_HOTLIGHT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_INACTIVE, pszDivide);
#if(_WIN32_WINNT >= 0x0500)
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_NOACCEL, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemState, ODS_NOFOCUSRECT, pszDivide);
#endif /* _WIN32_WINNT >= 0x0500 */
#endif /* WINVER >= 0x0500 */

		FTLASSERT( 0 == itemState);
		if (0 != itemState)
		{
			FTLTRACEEX(FTL::tlWarning, TEXT("%s: GetOwnerDrawState Not Complete, total=0x%08x, remain=0x%08x\n"),
				__FILE__LINE__, oldItemState, itemState);
		}
		return formater.GetString();
	}


    LPCTSTR CFWinUtil::GetOwnerDrawAction(CFStringFormater& formater, UINT itemAction, LPCTSTR pszDivide)
    {
        //UINT    oldItemAction = itemAction;

        HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemAction, ODA_DRAWENTIRE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemAction, ODA_SELECT, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, itemAction, ODA_FOCUS, pszDivide);

        FTLASSERT(0 == itemAction);
        return formater.GetString();
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////

	LPCTSTR CFWinHookUtil::GetCBTCodeInfo(CFStringFormater& formater, int nCode, WPARAM wParam, LPARAM lParam)
	{
		//FTLTRACE(TEXT("Enter GetCBTCodeInfo,TickCount=%d, nCode=%d\n"),GetTickCount(), nCode);
		switch(nCode)
		{
			HANDLE_CASE_TO_STRING_FORMATER(formater, HCBT_MOVESIZE);
			HANDLE_CASE_TO_STRING_FORMATER(formater, HCBT_MINMAX);
			HANDLE_CASE_TO_STRING_FORMATER(formater, HCBT_QS);
			HANDLE_CASE_TO_STRING_FORMATER(formater, HCBT_CREATEWND);
			HANDLE_CASE_TO_STRING_FORMATER(formater, HCBT_DESTROYWND);
		case HCBT_ACTIVATE:
			{
				CFStringFormater formaterActivae;
				CFStringFormater formaterActivateStruct;
				formaterActivateStruct.Format(TEXT("%s"), TEXT(""));

				HWND hWndActive = (HWND)wParam;
				CFWinUtil::GetWindowDescriptionInfo(formaterActivae, hWndActive);

				CBTACTIVATESTRUCT * pCBTActivateStruct = (CBTACTIVATESTRUCT*)lParam;
				if (pCBTActivateStruct)
				{
					CFStringFormater formaterActivaeInStruct;
					formaterActivateStruct.Format(TEXT("fMouse=%d, hWndActive=%s"),
						pCBTActivateStruct->fMouse, 
						CFWinUtil::GetWindowDescriptionInfo(formaterActivaeInStruct, pCBTActivateStruct->hWndActive));
				}
				formater.Format(TEXT("HCBT_ACTIVATE -- Active=%s, Struct=%s"), formaterActivae.GetString(), formaterActivateStruct.GetString());
				break;
			}
			HANDLE_CASE_TO_STRING_FORMATER(formater,HCBT_CLICKSKIPPED);
			HANDLE_CASE_TO_STRING_FORMATER(formater,HCBT_KEYSKIPPED);
			HANDLE_CASE_TO_STRING_FORMATER(formater,HCBT_SYSCOMMAND);
		case HCBT_SETFOCUS:
			{
				HWND hWndGetFocus = (HWND)wParam;
				HWND hWndLostFocus = (HWND)lParam;
				CFStringFormater formaterGetFocus, formaterLostFocus;
				formater.Format(TEXT("HCBT_SETFOCUS -- GetFocus=%s, LostFocus=%s"), 
					CFWinUtil::GetWindowDescriptionInfo(formaterGetFocus, hWndGetFocus),
					CFWinUtil::GetWindowDescriptionInfo(formaterLostFocus, hWndLostFocus));
				break;
			}

		default:
			break;
		}
		return formater.GetString();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////

    __declspec(selectany) HHOOK CFMessageBoxHook::s_hHook = NULL;
    __declspec(selectany) HWND  CFMessageBoxHook::s_ProphWnd = NULL;
    __declspec(selectany) LPCTSTR CFMessageBoxHook::s_pszOKString = NULL;

    CFMessageBoxHook::CFMessageBoxHook(DWORD dwThreadId, LPCTSTR pszOKString)
    {
        BOOL bRet = FALSE;
        CFMessageBoxHook::s_pszOKString = pszOKString;
        s_hHook = ::SetWindowsHookEx(WH_CBT, (HOOKPROC)CFMessageBoxHook::CBTProc
            , NULL, dwThreadId);
        API_VERIFY(NULL != s_hHook);
    }

    CFMessageBoxHook::~CFMessageBoxHook(void)
    {
        if (CFMessageBoxHook::s_ProphWnd)
        {
            RemoveProp(CFMessageBoxHook::s_ProphWnd,PREV_WND_PROC_NAME);
            CFMessageBoxHook::s_ProphWnd = NULL;
        }
        if (CFMessageBoxHook::s_hHook)
        {
            ::UnhookWindowsHookEx(CFMessageBoxHook::s_hHook);
            CFMessageBoxHook::s_hHook = NULL;
        }
    }


    LRESULT CFMessageBoxHook::CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
    {
        if (NULL == CFMessageBoxHook::s_hHook)
            return 0;
        BOOL bRet = FALSE;

        if (nCode == HCBT_CREATEWND){ //HCBT_CREATEWND = 3
            HWND hWnd = (HWND)wParam;
            TCHAR className[MAX_PATH];
            ::GetClassName(hWnd, className, _countof(className));
            if (_tcscmp(className, _T("#32770")) == 0)
            {
                WNDPROC prevWndProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)CFMessageBoxHook::WndProc);
                API_VERIFY(::SetProp(hWnd, PREV_WND_PROC_NAME, (HANDLE)prevWndProc));
                if (bRet)
                {
                    CFMessageBoxHook::s_ProphWnd = hWnd;
                }
            }
        }
        return ::CallNextHookEx(s_hHook, nCode, wParam, lParam);
    }

    LRESULT CFMessageBoxHook::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        WNDPROC prevWndProc = (WNDPROC)::GetProp(hWnd, PREV_WND_PROC_NAME);
        FTLASSERT(prevWndProc != NULL);

        if (message == WM_INITDIALOG)
        {
            BOOL bRet = FALSE;
            API_VERIFY(SetDlgItemText(hWnd, IDOK, CFMessageBoxHook::s_pszOKString));
        }
        return ::CallWindowProc(prevWndProc, hWnd, message, wParam, lParam);
    }

#if 0
    template <typename ControlT , typename ConverterFun>
    CControlPropertyHandleT<ControlT, ConverterFun>::
        CControlPropertyHandleT(ControlT& control)//, ConverterFun& fun)
        :m_control(control)
        //,m_fun(fun)
    {
    }

    template <typename ControlT , typename ConverterFun>
    INT CControlPropertyHandleT<ControlT, ConverterFun>::AddProperty(INT value)
    {
        INT index = m_control.AddString(ConverterFun(value));
        m_control.SetItemData(value);
        return index;
    }

#endif


}//FTL


#endif //FTL_WINDOW_HPP