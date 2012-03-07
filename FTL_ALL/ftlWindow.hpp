#ifndef FTL_WINDOW_HPP
#define FTL_WINDOW_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlwindow.h"
#endif

#include <prsht.h>

namespace FTL
{
    CFMessageInfo::CFMessageInfo(UINT msg) : CFConvertInfoT<CFMessageInfo,UINT>(msg)
    {
    }

    LPCTSTR CFMessageInfo::ConvertInfo()
    {
        if (NULL == m_bufInfo[0])
        {
            switch(m_Info)
            {
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CREATE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DESTROY);  //关闭窗口时,如果是主窗口,必须PostQuitMessage,否则进程还在运行(只是没有窗体)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MOVE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SIZE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ACTIVATE); //窗口被激活或失去激活状态, 可用于查看窗体是否最小化，及前一窗体
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SETFOCUS);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_KILLFOCUS);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ENABLE); //将禁止或激活时
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SETREDRAW); //设置窗口是否能重画 
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SETTEXT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_GETTEXT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_GETTEXTLENGTH); //得到与一个窗口有关的文本的长度（不包含空字符）
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PAINT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CLOSE);
#ifndef _WIN32_WCE
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUERYENDSESSION); //当用户选择结束对话框或程序自己调用ExitWindows函数
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUERYOPEN);     //当用户窗口恢复以前的大小位置时，把此消息发送给某个图标
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ENDSESSION);    //当系统进程发出WM_QUERYENDSESSION消息后，此消息发送给应用程序，通知它对话是否结束
#endif
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUIT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ERASEBKGND); //当窗口背景必须被擦除时（例在窗口改变大小时）
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SYSCOLORCHANGE);    //当系统颜色改变时，发送此消息给所有顶级窗口
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SHOWWINDOW);

#ifndef WM_CTLCOLOR
#  define WM_CTLCOLOR                             0x0019
#endif
                //子窗口总在绘制客户区之前，将其发送给父窗口，委托父窗口为自己准备一个画刷，同时父窗口（如对话框）也给自己发送该消息(为什么？)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CTLCOLOR);  

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_WININICHANGE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DEVMODECHANGE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ACTIVATEAPP);//属于另一App的窗体将激活时
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_FONTCHANGE);    //当系统的字体资源库变化时发送此消息给所有顶级窗口
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_TIMECHANGE);    //当系统的时间变化时发送此消息给所有顶级窗口
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CANCELMODE);    //取消系统模式时
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SETCURSOR);     //如果鼠标引起光标在某个窗口中移动且鼠标输入没有被捕获时，就发消息给某个窗口
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MOUSEACTIVATE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CHILDACTIVATE); //送此消息给MDI子窗口当用户点击此窗口的标题栏，或当窗口被激活，移动，改变大小
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUEUESYNC);     //此消息由基于计算机的"训练?"程序发送，通过 WH_JOURNALPALYBACK 的hook程序分离出用户输入消息
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_GETMINMAXINFO); //此消息发送给窗口当它将要改变大小或位置

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PAINTICON);     //发送给最小化窗口当它图标将要被重画
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ICONERASEBKGND);//此消息发送给某个最小化窗口，仅当它在画图标前它的背景必须被重画
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NEXTDLGCTL);    //此消息发送给某个最小化窗口，仅当它在画图标前它的背景必须被重画
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SPOOLERSTATUS); //每当打印管理列队增加或减少一条作业时发出此消息
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DRAWITEM);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MEASUREITEM);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DELETEITEM);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_VKEYTOITEM);    //由一个LBS_WANTKEYBOARDINPUT风格的发出给它的所有者来响应WM_KEYDOWN消息
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CHARTOITEM);    //由一个LBS_WANTKEYBOARDINPUT风格的列表框发送给他的所有者来响应WM_CHAR消息
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SETFONT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_GETFONT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SETHOTKEY);     //应用程序发送此消息让一个窗口与一个热键相关连
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_GETHOTKEY);     //应用程序发送此消息来判断热键与某个窗口是否有关联
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUERYDRAGICON); //送给最小化窗口，当此窗口将要被拖放而它的类中没有定义图标，应用程序能返回一个图标或光标的句柄，
                //当用户拖放图标时系统显示这个图标或光标
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_COMPAREITEM);   //发送此消息来判定combobox或listbox新增加的项的相对位置
#if(WINVER >= 0x0500)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_GETOBJECT);
#endif
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_COMPACTING);    //显示内存已经很少了
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_COMMNOTIFY);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_WINDOWPOSCHANGING);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_WINDOWPOSCHANGED);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_POWER);         //当系统将要进入暂停状态时发送此消息
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_COPYDATA);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CANCELJOURNAL); //当某个用户取消程序日志激活状态，提交此消息给程序
#if(WINVER >= 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NOTIFY);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_INPUTLANGCHANGEREQUEST);    //当用户选择某种输入语言，或输入语言的热键改变
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_INPUTLANGCHANGE);   //当平台现场已经被改变后发送此消息给受影响的最顶级窗口
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_TCARD);         //当程序已经初始化windows帮助例程时发送此消息给应用程序
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_HELP);          //此消息显示用户按下了F1，如果某个菜单是激活的，就发送此消息个此窗口关联的菜单，
                //  否则就发送给有焦点的窗口，如果当前都没有焦点，就把此消息发送给当前激活的窗口
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_USERCHANGED);   //当用户已经登入或退出后发送此消息给所有的窗口，当用户登入或退出时系统更新用户的具体
                //  设置信息，在用户更新设置时系统马上发送此消息
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NOTIFYFORMAT);  //通过此消息来判断控件是使用ANSI还是UNICODE结构
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CONTEXTMENU);   //当用户某个窗口中点击了一下右键就发送此消息给这个窗口
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_STYLECHANGING); //当调用SETWINDOWLONG函数 将要改变 一个或多个 窗口的风格时发送此消息给那个窗口
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_STYLECHANGED);  //当调用SETWINDOWLONG函数 改变 一个或多个窗口的风格后发送此消息给那个窗口
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DISPLAYCHANGE); //当显示器的分辨率改变后发送此消息给所有的窗口
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_GETICON);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SETICON);
#endif
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCCREATE);      //当某个窗口第一次被创建时，此消息在WM_CREATE消息发送前发送
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCDESTROY);     //此消息通知某个窗口，非客户区正在销毁
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCCALCSIZE);    //当某个窗口的（非？）客户区域必须被核算时发送此消息
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCHITTEST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCPAINT);       //程序发送此消息给某个窗口当它（窗口）的框架必须被绘制时
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCACTIVATE);    //此消息发送给某个窗口 仅当它的非客户区需要被改变来显示是激活还是非激活状态
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_GETDLGCODE);    //发送此消息给某个与对话框程序关联的控件，widdows控制方位键和TAB键使输入进入此控件
                //  通过响应WM_GETDLGCODE消息，应用程序可以把他当成一个特殊的输入控件并能处理它
#ifndef _WIN32_WCE
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SYNCPAINT);
#endif
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCMOUSEMOVE);   //非客户区内移动时发送此消息给这个窗口
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCLBUTTONDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCLBUTTONUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCLBUTTONDBLCLK);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCRBUTTONDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCRBUTTONUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCRBUTTONDBLCLK);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCMBUTTONDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCMBUTTONUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCMBUTTONDBLCLK);

#if(_WIN32_WINNT >= 0x0500)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCXBUTTONDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCXBUTTONUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCXBUTTONDBLCLK);
#endif

#if(_WIN32_WINNT >= 0x0501)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_INPUT);
#endif
                //HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_KEYFIRST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_KEYDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_KEYUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CHAR);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DEADCHAR); //当用 translatemessage 函数翻译WM_KEYUP消息时发送此消息给拥有焦点的窗口
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SYSKEYDOWN);    //当用户按住ALT键同时按下其它键时提交此消息给拥有焦点的窗口
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SYSKEYUP);      //当用户释放一个键同时ALT 键还按着时提交此消息给拥有焦点的窗口
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SYSCHAR);       //当WM_SYSKEYDOWN消息被TRANSLATEMESSAGE函数翻译后提交此消息给拥有焦点的窗
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SYSDEADCHAR);   //
#if(_WIN32_WINNT >= 0x0501)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_UNICHAR);
                //HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_KEYLAST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),UNICODE_NOCHAR);
#else
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_KEYLAST);
#endif

#if(WINVER >= 0x0400)
#endif
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_STARTCOMPOSITION);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_ENDCOMPOSITION);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_COMPOSITION);
                //HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_KEYLAST);

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_INITDIALOG); //在一个对话框程序被显示前发送此消息给它，通常用此消息初始化控件和执行其它任务
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_COMMAND);

                //可用于屏蔽屏幕保护和显示器节电模式(SC_SCREENSAVE/SC_MONITORPOWER),返回0
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SYSCOMMAND);    //当用户选择窗口菜单的一条命令或当用户选择最大化或最小化时那个窗口会收到此消息

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_TIMER);     //发生了定时器事件
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_HSCROLL);   //水平滚动条产生一个滚动事件
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_VSCROLL);   //垂直滚动条产生一个滚动事件
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_INITMENU);  //当一个菜单将要被激活时发送此消息，它发生在用户菜单条中的某项或按下某个菜单键，
                //  它允许程序在显示前更改菜单
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_INITMENUPOPUP);
#ifdef WM_SYSTIMER
#error Already define WM_SYSTIMER
#endif 

#ifndef WM_SYSTIMER
#define WM_SYSTIMER 0x0118
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SYSTIMER);  //UnDocument Message
#endif

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MENUSELECT); //当用户选择一条菜单项时发送此消息给菜单的所有者（一般是窗口）
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MENUCHAR);  //当菜单已被激活用户按下了某个键（不同于加速键），发送此消息给菜单的所有者
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ENTERIDLE); //当一个模态对话框或菜单进入空载状态时(处理完一条或几条先前的消息后没有消息它的列队中等待)发送此消息给它的所有者，
#if(WINVER >= 0x0500)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MENURBUTTONUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MENUDRAG);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MENUGETOBJECT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_UNINITMENUPOPUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MENUCOMMAND);
#  if(_WIN32_WINNT >= 0x0500)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CHANGEUISTATE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_UPDATEUISTATE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUERYUISTATE);
#  endif //_WIN32_WINNT >= 0x0500

#endif //WINVER >= 0x0500

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CTLCOLORMSGBOX);    //在windows绘制消息框前发送此消息给消息框的所有者窗口，通过响应这条消息，所有者窗口可以
                //  通过使用给定的相关显示设备的句柄来设置消息框的文本和背景颜色
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CTLCOLOREDIT);      //当一个编辑型控件将要被绘制时发送此消息给它的父窗口 -- 可以设置编辑框的文本和背景颜色
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CTLCOLORLISTBOX);   //当一个列表框控件将要被绘制前发送此消息给它的父窗口 -- 可以设置列表框的文本和背景颜色
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CTLCOLORBTN);       //当一个按钮  控件将要被绘制时发送此消息给它的父窗口 -- 可以设置按纽的文本和背景颜色
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CTLCOLORDLG);       //当一个对话框控件将要被绘制前发送此消息给它的父窗口 -- 可以设置对话框的文本背景颜色
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CTLCOLORSCROLLBAR); //当一个滚动条控件将要被绘制时发送此消息给它的父窗口 -- 可以设置滚动条的背景颜色
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CTLCOLORSTATIC);    //当一个静态控件将要被绘制时发送此消息给它的父窗口 -- 可以设置静态控件的文本和背景颜色
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),MN_GETHMENU);


                //HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MOUSEFIRST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MOUSEMOVE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_LBUTTONDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_LBUTTONUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_LBUTTONDBLCLK);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RBUTTONDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RBUTTONUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RBUTTONDBLCLK);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MBUTTONDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MBUTTONUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MBUTTONDBLCLK);

#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MOUSEWHEEL);        //当鼠标轮子转动时发送此消息个当前有焦点的控件
#endif

#if (_WIN32_WINNT >= 0x0500)                
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_XBUTTONDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_XBUTTONUP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_XBUTTONDBLCLK);
#endif

                //HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MOUSELAST);

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PARENTNOTIFY); //当MDI子窗口被创建或被销毁，或用户按了一下鼠标键而光标在子窗口上时发送此消息给它的父窗口
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ENTERMENULOOP); //发送此消息通知应用程序的主窗口 已经进入了菜单循环模式 -- ?
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_EXITMENULOOP);  //发送此消息通知应用程序的主窗口 已经退出了菜单循环模式 -- ?

#if(WINVER >= 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NEXTMENU);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SIZING);        //当用户正在调整窗口大小时发送此消息给窗口；通过此消息应用程序可以监视和修改窗口大小和位置
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CAPTURECHANGED);//窗口失去捕获的鼠标时
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MOVING);        //当用户在移动窗口时发送此消息
#endif

#if(WINVER >= 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_POWERBROADCAST);//送给应用程序来通知它有关电源管理事件
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DEVICECHANGE);  //当设备的硬件配置改变时发送此消息给应用程序或设备驱动程序
#endif

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDICREATE);     //要求创建一个MDI 子窗口
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDIDESTROY);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDIACTIVATE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDIRESTORE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDINEXT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDIMAXIMIZE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDITILE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDICASCADE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDIICONARRANGE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDIGETACTIVE);


                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDISETMENU);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ENTERSIZEMOVE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_EXITSIZEMOVE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DROPFILES);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MDIREFRESHMENU);

#if(WINVER >= 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_SETCONTEXT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_NOTIFY);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_CONTROL);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_COMPOSITIONFULL);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_SELECT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_CHAR);
#endif

#if(WINVER >= 0x0500)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_REQUEST);
#endif

#if(WINVER >= 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_KEYDOWN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IME_KEYUP);
#endif

#if((_WIN32_WINNT >= 0x0400) || (WINVER >= 0x0500))
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MOUSEHOVER); //
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_MOUSELEAVE); //鼠标移开时，需要用 ON_MESSAGE 响应
#endif

#if(WINVER >= 0x0500)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCMOUSEHOVER);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_NCMOUSELEAVE);
#endif


#if(_WIN32_WINNT >= 0x0501)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_WTSSESSION_CHANGE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_TABLET_FIRST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_TABLET_LAST);
#endif

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CUT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_COPY);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PASTE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CLEAR);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_UNDO);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RENDERFORMAT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RENDERALLFORMATS);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DESTROYCLIPBOARD);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DRAWCLIPBOARD); //当剪贴板的内容变化时发送此消息给剪贴板观察链的第一个窗口；它允许用剪贴板观察窗口来显示剪贴板的新内容
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PAINTCLIPBOARD);//当剪贴板包含CF_OWNERDIPLAY格式的数据并且剪贴板观察窗口的客户区需要重画
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_VSCROLLCLIPBOARD);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SIZECLIPBOARD);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ASKCBFORMATNAME);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_CHANGECBCHAIN); //当一个窗口从剪贴板观察链中移去时发送此消息给剪贴板观察链的第一个窗口
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_HSCROLLCLIPBOARD);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUERYNEWPALETTE);   //此消息能使窗口在收到焦点时同时有机会实现他的逻辑调色板
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PALETTEISCHANGING);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PALETTECHANGED);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_HOTKEY);        //当用户按下由REGISTERHOTKEY函数注册的热键时提交此消息

#if(WINVER >= 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PRINT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PRINTCLIENT);
#endif

#if(_WIN32_WINNT >= 0x0500)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_APPCOMMAND);
#endif

#if(_WIN32_WINNT >= 0x0501)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_THEMECHANGED);
#endif

#if(WINVER >= 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_HANDHELDFIRST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_HANDHELDLAST);
#ifdef __AFXPRIV_H__
                //#  ifdef USING_MFC
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUERYAFXWNDPROC);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SIZEPARENT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SETMESSAGESTRING);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_IDLEUPDATECMDUI);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_INITIALUPDATE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_COMMANDHELP);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_HELPHITTEST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_EXITHELPMODE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RECALCPARENT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SIZECHILD);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_KICKIDLE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUERYCENTERWND);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DISABLEMODAL);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_FLOATSTATUS);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_ACTIVATETOPLEVEL);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RESERVED_036F);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RESERVED_0370);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RESERVED_0371);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RESERVED_0372);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SOCKET_NOTIFY);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_SOCKET_DEAD);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_POPMESSAGESTRING);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_HELPPROMPTADDR);
                //HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_OCC_LOADFROMSTREAM);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_OCC_LOADFROMSTORAGE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_OCC_INITNEW);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_OCC_LOADFROMSTREAM_EX);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_OCC_LOADFROMSTORAGE_EX);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_QUEUE_SENTINEL);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RESERVED_037C);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RESERVED_037D);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_RESERVED_037E);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_FORWARDMSG);
#  else
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_AFXFIRST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_AFXLAST);

#  endif //end __AFXPRIV_H__

#endif

                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PENWINFIRST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_PENWINLAST);
#  ifdef _DDEHEADER_INCLUDED_
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_INITIATE);  //一个DDE客户程序提交此消息开始一个与服务器程序的会话来响应那个指定的程序和主题名
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_TERMINATE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_ADVISE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_UNADVISE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_ACK);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_DATA);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_REQUEST);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_POKE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_EXECUTE);
                //HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_DDE_LAST);
#  endif
#if(WINVER >= 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),WM_APP);
#endif
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DM_GETDEFID);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DM_SETDEFID);
#if(WINVER >= 0x0400)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),DM_REPOSITION);
#endif
            default:
                StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
                    TEXT("Unknown Message %d(0x%08x)"),m_Info,m_Info);
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
                    MONITOR_DEFAULTTONEAREST);

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

    BOOL CFWinUtil::ActiveAndForegroundWindow(HWND hWnd)
    {
        //IPMSG 中也有一个 SetForceForegroundWindow 方法，大致相同

        BOOL bRet = TRUE;
        HWND   hForegdWnd   =   ::GetForegroundWindow();
        if (hForegdWnd == hWnd)
        {
            return TRUE;
        }

        DWORD   dwCurID   =   ::GetCurrentThreadId();   
        DWORD   dwForeID   =   ::GetWindowThreadProcessId(hForegdWnd,NULL);
        if (dwCurID != dwForeID)
        {
            API_VERIFY(::AttachThreadInput(dwCurID,dwForeID,TRUE));

            //备份以前的值，然后设置为0
            DWORD sp_time = 0;
            SystemParametersInfo( SPI_GETFOREGROUNDLOCKTIMEOUT,0,&sp_time,0);
            SystemParametersInfo( SPI_SETFOREGROUNDLOCKTIMEOUT,0,(LPVOID)0,0);

            API_VERIFY(::SetForegroundWindow(hWnd));

            SystemParametersInfo( SPI_SETFOREGROUNDLOCKTIMEOUT,0,&sp_time,0);
            API_VERIFY(::AttachThreadInput(dwCurID,dwForeID,FALSE));
            //ShowWindow(hWnd,...)?
        }
        else
        {
#pragma TODO(how to active a minimize window without change it min/max status)
            API_VERIFY(::SetForegroundWindow(hWnd));
            //ShowWindow(hWnd,...)?
        }
        SwitchToThisWindow(hWnd, TRUE);

        //if (IsWindowVisible(hWnd))
        //{
        //}
        //else
        //{
        //}

        //if (IsIconic(hWnd))
        //{
        //    API_VERIFY(ShowWindow(hWnd,SW_RESTORE));
        //}
        //else
        //{
        //    API_VERIFY(ShowWindow(hWnd,SW_SHOW));
        //}

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

    LPCTSTR CFWinUtil::GetNotifyCodeString(UINT nCode)
    {
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
            HANDLE_CASE_RETURN_STRING(NM_TOOLTIPSCREATED);
#if (_WIN32_IE >= 0x0500)
            HANDLE_CASE_RETURN_STRING(NM_LDOWN);
            HANDLE_CASE_RETURN_STRING(NM_RDOWN);
            HANDLE_CASE_RETURN_STRING(NM_THEMECHANGED);
#endif

#if (_WIN32_IE >= 0x0600)
            HANDLE_CASE_RETURN_STRING(NM_FONTCHANGED);
            HANDLE_CASE_RETURN_STRING(NM_CUSTOMTEXT);
            HANDLE_CASE_RETURN_STRING(NM_TVSTATEIMAGECHANGING);
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
            HANDLE_CASE_RETURN_STRING(LVN_INCREMENTALSEARCHA);
            HANDLE_CASE_RETURN_STRING(LVN_INCREMENTALSEARCHW);
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
            HANDLE_CASE_RETURN_STRING(MCN_VIEWCHANGE);

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

    LPCTSTR CFWinUtil::GetWindowStyleString(FTL::CFStringFormater& formater, HWND hWnd,LPCTSTR pszDivide/* = TEXT("|") */)
    {
        BOOL bRet = FALSE;

        FTLASSERT(::IsWindow(hWnd));
        LONG_PTR    lStyle = ::GetWindowLongPtr(hWnd, GWL_STYLE);
        LONG_PTR    lOldStyle = lStyle;
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_OVERLAPPED, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_POPUP, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_CHILD, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_MINIMIZE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_VISIBLE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_DISABLED, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_CLIPSIBLINGS, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, WS_CLIPCHILDREN, pszDivide);
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

#if _WIN32_WINNT >= 0x0600
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_DEFCOMMANDLINK, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_COMMANDLINK, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_DEFSPLITBUTTON, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_SPLITBUTTON, pszDivide);
#endif //_WIN32_WINNT >= 0x0600
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_OWNERDRAW, pszDivide);
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
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_PUSHBUTTON, pszDivide);
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
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_BITMAP, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_ICON, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, BS_TEXT, pszDivide);
        }

        //ListView
        if (0 ==  lstrcmp(szClassName, WC_LISTVIEW))
        {
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_LIST, pszDivide);
            //HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_ICON, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_REPORT, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SMALLICON, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SINGLESEL, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SHOWSELALWAYS, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SORTASCENDING, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SORTDESCENDING, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_SHAREIMAGELISTS, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_NOLABELWRAP, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_AUTOARRANGE, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_EDITLABELS, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_OWNERDATA, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_NOSCROLL, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_TYPESTYLEMASK, pszDivide);
            //HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_ALIGNMASK, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_ALIGNTOP, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_ALIGNLEFT, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_NOSORTHEADER, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_NOCOLUMNHEADER, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, LVS_OWNERDRAWFIXED, pszDivide);
        }



        FTLASSERT( 0 == lStyle);
        if (0 != lStyle)
        {
            FTLTRACEEX(FTL::tlWarning, TEXT("%s:Check Style String Not Complete, total=0x%08x, remain=0x%08x\n"),
                __FILE__LINE__, lOldStyle, lStyle);
        }
        return formater.GetString();
    }

    LPCTSTR CFWinUtil::GetWindowExStyleString(FTL::CFStringFormater& formater, HWND hWnd, LPCTSTR pszDivide /* = TEXT */)
    {
        FTLASSERT(::IsWindow(hWnd));
        LONG_PTR    lExStyle = ::GetWindowLongPtr(hWnd, GWL_EXSTYLE);
        LONG_PTR    lOldExStyle = lExStyle;

        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_DLGMODALFRAME, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_NOPARENTNOTIFY, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_TOPMOST, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_ACCEPTFILES, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_TRANSPARENT, pszDivide);
#if(WINVER >= 0x0400)
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_MDICHILD, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_TOOLWINDOW, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_WINDOWEDGE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_CLIENTEDGE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_CONTEXTHELP, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_RIGHT, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_LEFT, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_RTLREADING, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_LTRREADING, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_LEFTSCROLLBAR, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_RIGHTSCROLLBAR, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_CONTROLPARENT, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_STATICEDGE, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, lExStyle, WS_EX_APPWINDOW, pszDivide);
#endif /* WINVER >= 0x0400 */

#if(_WIN32_WINNT >= 0x0500)
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