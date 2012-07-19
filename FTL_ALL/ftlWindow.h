#ifndef FTL_WINDOW_H
#define FTL_WINDOW_H
#pragma once


#ifndef FTL_BASE_H
#  error ftlwindow.h requires ftlbase.h to be included first
#endif
/******************************************************************************************************
* Monitor -- MonitorFromPoint ，系统有一个 multimon.h 文件
*   
* CHAIN_MSG_MAP(__super)
* 
* Vista/Win7
*   Aero向导(CPropertySheetImpl<>, CPropertyPageImpl<>)
*     由属性页变为经典样式的向导:  m_psh.dwFlags |= PSH_WIZARD97; Areo 向导 PSH_AEROWIZARD
*     消息 PSM_SHOWWIZBUTTONS -- 显示或隐藏向导中的标准按钮， 有 PropSheet_ShowWizButtons 辅助宏
*          PSM_ENABLEWIZBUTTONS -- 启用或禁用某个标准按钮，有 PropSheet_EnableWizButtons 辅助宏
*          PSM_SETBUTTONTEXT -- 修改按钮上的文字，有 PropSheet_SetButtonText  辅助宏
* 任务对话框(TaskDialog/TaskDialogIndirect) -- 
*   TASKDIALOGCONFIG::pfCallback 回调函数，用来响应任务对话框所触发的事件
*     通知顺序: TDN_DIALOG_CONSTRUCTED -> TDN_CREATED 
*   消息：TDM_SET_ELEMENT_TEXT -- 设置任务对话框上控件的文本
*         TDM_SET_BUTTON_ELEVATION_REQUIRED_STATE -- 在链接旁显示出UAC盾形图标
*   Flags
*     TDF_USE_COMMAND_LINKS -- 将自定义按钮显示为命令链接(不能控制标准按钮TDCBF_OK_BUTTON等)
*     TDF_SHOW_PROGRESS_BAR -- 显示进度条
*     用TDF_SHOW_MARQUEE_PROGRESS_BAR -- 显示走马灯样式(不停的从左到右)的进度条
*   进度条()
*     TDM_SET_PROGRESS_BAR_RANGE -- 指定进度条的指示范围的消息
*     TDM_SET_PROGRESS_BAR_POS -- 指定进度条在指示范围中的位置
*     TDM_SET_PROGRESS_BAR_STATE -- 改变进度条的状态
*     
* 
* UAC(User Account Control)
*   Button_SetElevationRequiredState --好像无效?
*
* 分层窗口(WS_EX_LAYERED) -- 允许控制窗体的透明度。分层窗体提供了两种截然不同的编程模型
*   SetLayeredWindowAttributes(简单) -- 允许设置一个RGB颜色(通常是窗体中不会出现的颜色)，然后所有以该颜色绘出的像素都将呈现为透明
*     SetLayeredWindowAttributes( 0, 150, LWA_ALPHA);  //设置透明度为150(窗体整体透明,子控件也透明)
*     SetLayeredWindowAttributes( RGB(240,240,240), 0, LWA_COLORKEY); //窗体整体透明,子控件不透明 
*       设置指定颜色的部分透明(Dialog背景颜色),如要设置其他颜色，需要在 OnCtlColor 或 WM_ERASEBKGND 中指定颜色
*     TODO: 组合使用 LWA_ALPHA 和 LWA_COLORKEY ?
*   UpdateLayeredWindow(困难) -- 提供一个与设备无关的位图，完整定义屏幕上窗体的整体样式，会将指定的位图完整地保留Alpha通道信息并拷贝到窗体上
*     ::UpdateLayeredWindow( m_hWnd, NULL, &ptDst, &WndSize, dcMem.m_hDC, &ptSrc, 0, &blendPixelFunction, ULW_ALPHA );
*     这种窗体不支持子控件，不支持OnPaint()，但可以通过PNG图片中的Alpha值来 完全控制屏幕上窗体的透明情况
*   通过两个窗体重合且分别使用 SetLayeredXXX(,LWA_COLORKEY) 和 UpdateXXX 的方法来提供异形窗体
* 
* DWM(Desktop Window Manager,窗口管理器) -- 负责组合桌面上的各个窗体, 允许开发者设置某个窗体在于其它窗体组合/重叠时的显示效果，
*   即能用来实现“半透明玻璃(Glass)”特效（允许控制窗体范围内部分区域的透明度)
*     窗体区域(Window Region) -- 指操作系统允许窗体在其中进行绘制的区域，除非切换回Basic主题，否则Vista已不再使用
*     桌面合成(Desktop Composition) -- DWM所提供的一个功能，可以实现诸如玻璃、3D窗口变换等视觉效果，
*       启用时，DWM默认将把窗体的非客户区域以玻璃效果呈现，而客户区域默认为不透明。
*       DwmIsCompositionEnabled -- 判断是否启用了合成效果
*       DwmEnableComposition -- 暂时禁用/启用桌面合成功能，不需要管理员权限？程序退出时自动恢复
*       DwmGetColorizationColor -- 检测到合成效果是半透明的还是不透明的，以及合成颜色
*       DwmEnableBlurBehindWindow -- 让客户区域完全或某部分实现玻璃效果
*       DwmExtendFrameIntoClientArea -- 可让框架(Window Frame)向客户区扩展
*       DwmSetWindowAttribute  -- 设置DWM窗体的属性,如 控制 Flip3D、最小化时的动画效果
*         MARGINS margins={-1}; -- 将框架扩展为整个客户区，即可将整个客户区域和非客户区域作为一个无缝的整体进行显示(如玻璃效果)
*     极光效果(aurora effect) -- 
*     Flip3D(Win+Tab) -- 
*     任务栏缩略图自动同步 -- DwmRegisterThumbnail、DwmUpdateThumbnailProperties
* 
* RGB --  0x00BBGGRR
* Gdiplus::ARGB -- 0xAARRGGBB  <== 注意：颜色顺序和RGB的相反
******************************************************************************************************/

/******************************************************************************************************
* CHAIN_MSG_MAP(__super)
* 
* Vista/Win7
*   Aero向导(CPropertySheetImpl<>, CPropertyPageImpl<>)
*     由属性页变为经典样式的向导:  m_psh.dwFlags |= PSH_WIZARD97; Areo 向导 PSH_AEROWIZARD
*     消息 PSM_SHOWWIZBUTTONS -- 显示或隐藏向导中的标准按钮， 有 PropSheet_ShowWizButtons 辅助宏
*          PSM_ENABLEWIZBUTTONS -- 启用或禁用某个标准按钮，有 PropSheet_EnableWizButtons 辅助宏
*          PSM_SETBUTTONTEXT -- 修改按钮上的文字，有 PropSheet_SetButtonText  辅助宏
* 任务对话框(TaskDialog/TaskDialogIndirect) -- 
*   TASKDIALOGCONFIG::pfCallback 回调函数，用来响应任务对话框所触发的事件
*     通知顺序: TDN_DIALOG_CONSTRUCTED -> TDN_CREATED 
*   消息：TDM_SET_ELEMENT_TEXT -- 设置任务对话框上控件的文本
*         TDM_SET_BUTTON_ELEVATION_REQUIRED_STATE -- 在链接旁显示出UAC盾形图标
*   Flags
*     TDF_USE_COMMAND_LINKS -- 将自定义按钮显示为命令链接(不能控制标准按钮TDCBF_OK_BUTTON等)
*     TDF_SHOW_PROGRESS_BAR -- 显示进度条
*     用TDF_SHOW_MARQUEE_PROGRESS_BAR -- 显示走马灯样式(不停的从左到右)的进度条
*   进度条()
*     TDM_SET_PROGRESS_BAR_RANGE -- 指定进度条的指示范围的消息
*     TDM_SET_PROGRESS_BAR_POS -- 指定进度条在指示范围中的位置
*     TDM_SET_PROGRESS_BAR_STATE -- 改变进度条的状态
*     
* 
* UAC(User Account Control)
*   Button_SetElevationRequiredState --好像无效?
*
* 分层窗口(WS_EX_LAYERED) -- 允许控制窗体的透明度。分层窗体提供了两种截然不同的编程模型
*   SetLayeredWindowAttributes(简单) -- 允许设置一个RGB颜色(通常是窗体中不会出现的颜色)，然后所有以该颜色绘出的像素都将呈现为透明
*     SetLayeredWindowAttributes( 0, 150, LWA_ALPHA);  //设置透明度为150(窗体整体透明,子控件也透明)
*     SetLayeredWindowAttributes( RGB(240,240,240), 0, LWA_COLORKEY); //窗体整体透明,子控件不透明 
*       设置指定颜色的部分透明(Dialog背景颜色),如要设置其他颜色，需要在 OnCtlColor 或 WM_ERASEBKGND 中指定颜色
*     TODO: 组合使用 LWA_ALPHA 和 LWA_COLORKEY ?
*   UpdateLayeredWindow(困难) -- 提供一个与设备无关的位图，完整定义屏幕上窗体的整体样式，会将指定的位图完整地保留Alpha通道信息并拷贝到窗体上
*     ::UpdateLayeredWindow( m_hWnd, NULL, &ptDst, &WndSize, dcMem.m_hDC, &ptSrc, 0, &blendPixelFunction, ULW_ALPHA );
*     这种窗体不支持子控件，不支持OnPaint()，但可以通过PNG图片中的Alpha值来 完全控制屏幕上窗体的透明情况
*   通过两个窗体重合且分别使用 SetLayeredXXX(,LWA_COLORKEY) 和 UpdateXXX 的方法来提供异形窗体
* 
* DWM(Desktop Window Manager,窗口管理器) -- 负责组合桌面上的各个窗体, 允许开发者设置某个窗体在于其它窗体组合/重叠时的显示效果，
*   即能用来实现“半透明玻璃(Glass)”特效（允许控制窗体范围内部分区域的透明度)
*     窗体区域(Window Region) -- 指操作系统允许窗体在其中进行绘制的区域，除非切换回Basic主题，否则Vista已不再使用
*     桌面合成(Desktop Composition) -- DWM所提供的一个功能，可以实现诸如玻璃、3D窗口变换等视觉效果，
*       启用时，DWM默认将把窗体的非客户区域以玻璃效果呈现，而客户区域默认为不透明。
*       DwmIsCompositionEnabled -- 判断是否启用了合成效果
*       DwmEnableComposition -- 暂时禁用/启用桌面合成功能，不需要管理员权限？程序退出时自动恢复
*       DwmGetColorizationColor -- 检测到合成效果是半透明的还是不透明的，以及合成颜色
*       DwmEnableBlurBehindWindow -- 让客户区域完全或某部分实现玻璃效果
*       DwmExtendFrameIntoClientArea -- 可让框架(Window Frame)向客户区扩展
*         MARGINS margins={-1}; -- 将框架扩展为整个客户区，即可将整个客户区域和非客户区域作为一个无缝的整体进行显示(如玻璃效果)
*     极光效果(aurora effect) -- 
*
* 
* RGB --  0x00BBGGRR
* Gdiplus::ARGB -- 0xAARRGGBB  <== 注意：颜色顺序和RGB的相反
******************************************************************************************************/

/******************************************************************************************************
* WinProc 返回值：
*   0 -- 用户已经处理好消息；如 WM_CLOSE 中返回0则不会关闭，返回 DefaultWindowProc 才关闭(WM_DESTROY)
*
* 创建窗口的顺序：PreCreateWindow -> PreSubclassWindow -> OnGetMinMaxInfo -> OnNcCreate -> OnNcCalcSize -> OnCreate
*   -> OnSize -> OnMove -> OnChildNotify
* 关闭窗口的顺序：OnClose -> OnDestory -> OnNcDestroy -> PostNcDestory
*
* 窗口实例有四个关系窗口的句柄:
*   1.本窗口的 Z-Order 最高子窗口句柄 <== GetTopWindow
*   2.本窗口的下一兄弟窗口句柄 <== GetNextWindow
*   3.本窗口的父窗口的句柄   <== GetParent
*   4.本窗口的所有者窗口句柄 <== 
*
* SendMessage 后执行体是UI线程(后台线程会等待，直到UI线程执行完毕)
*   如果收到Send消息时，UI线程还在消息处理体中,则★不会立即强制抢断执行★,需要等待执行体结束后，
*   再通过GetMessage★优先★调用执行。
*   WTL的例子程序分析:
*     Send时:_GetMessage -> _DispatchClientMessage -> _InternalCallWinProc
*     Post时:_DispatchMessage -> DispatchMessageWorker -> _InternalCallWinProc
* 
* DefaultWindowProc -- 默认消息处理函数
*
* 注意：改变窗体大小时，会连续发送 WM_SIZE + WM_PAINT 消息，擦除背景，容易造成闪烁
* 
* 自绘控件：OWNERDRAW，在创建时处理 WM_MEASUREITEM 消息，改变外观时处理 WM_DRAWITEM 消息
* 
* 创建全屏窗体( WS_POPUP属性, 0,0 ~ CxScreen, CyScreen )：
*   hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP | WS_VISIBLE, 0, 0, 
*     GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);
*
* 获取鼠标右键单击的位置，并显示弹出菜单(点击后返回)
*   TrackPopupMenu(hPopMenu, TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN, LOWORD(lParam), HIWORD(lParam) , 0, hwnd, NULL);
*
* 在 FormView 中启用界面更新机制：
*   1.包含 <afxpriv.h>；
*   2.映射处理消息 WM_IDLEUPDATECMDUI (对话框中需要处理 WM_KICKIDLE )
*   3.在实现中调用 UpdateDialogControls
*
* AdjustWindowRectEx -- 根据客户区的大小和窗口样式，计算并调整窗口的完整尺寸,然后MoveWindow进行调整即可
*   RECT rcClient = { 0,0,800,600 };
*   AdjustWindowRectEx( &rcClient, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL, GetWindowExStyle(hWnd));
* MapWindowPoints -- 把相对于一个窗口的坐标空间的一组点映射成相对于另一窗口的坐标空间的一组点
*
* 滚动窗体 -- 依靠更改MapMode、ViewportExt/WindowExt, ViewportOrg/WindowOrg 等实现
*   CScrollView 中有 m_totalDev, m_pageDev 等参数
*   CScrollView 的 SetScrollSize 是逻辑坐标
*
*
* ShowWindow 
*   SW_HIDE -- 隐藏，失活
*   SW_MINIMIZE -- 最小化，失活
*   SW_RESTORE -- 激活并恢复原显示
*   SW_SHOW -- 激活并显示
*   SW_SHOWMAXIMIZED -- 激活并最大化
*   SW_SHOWMINIMIZED -- 激活并图标化
*   SW_SHOWMINNOACTIVE -- 图标化但是不激活
*   SW_SHOWNA -- 显示并保持原激活状态
*   SW_SHOWNOACTIVATE -- 用最近的大小和位置显示并保持原激活状态
*   SW_SHOWNORMAL -- 激活并显示，必要时从最大最小化中恢复
*
* 反射消息
*  WM_COMMAND -- 消息的参数中既有发送消息的控件的 ID，又有控件的 HWND，还有通知代码
*  WM_NOTIFY  -- 消息除 WM_COMMAND 的参数之外还有一个指向 NMHDR 数据结构的指针
******************************************************************************************************/

/******************************************************************************************************
* RichEdit -- CWindowImple<CEditView, CRichEditCtrl>
*   BitBlt 到 HDC 中，称为图片
*   StreamOutRtf -- 到Buffer中持久化
*
* SliderCtrl -- 滑块控件,滑块以创建时所指定的增量移动
*   常见属性、方法:
*     Buddy -- 关联窗口
*     LineSize -- 对应键盘的方向键
*     PageSize -- 对应PageUp、PageDown键
*     Selection-- 选择范围，高亮显示
*     ChannelRect -- 在刻度之上，供Thumb移动的区域
*     ThumbRect -- “拇指”(通过鼠标拖动的那个小方块)的范围
*     Tic/TicFreq -- 获取或设置显示刻度的位置/间隔
*   风格:
*     TBS_AUTOTICKS -- 滑动条具有刻度(?)
*     TBS_NOTICKS -- 滑动条不具有刻度(?)
*     TBS_ENABLESELRANGE -- 允许选择范围
*   要处理滑动条变化的事件，需要父窗体处理 WM_HSCROLL/WM_VSCROLL 消息(因为和水平滚动条公用同一个OnXScroll函数,
*     所以参数中的指针变量被定义为CScrollBar*类型，需要转换为CSliderCtrl*)
*     TRBN_THUMBPOSCHANGING Notify 事件不可以
*     
******************************************************************************************************/

/******************************************************************************************************
* 资源文件： RC -> RES，定义方式通常都是  ID 类型 文件名或资源名    （注意：只能有一个STRINGTABLE，没有名字）
*   CURSOR -- 光标
*   DIALOGEX --
*   ICON --
*   MENU -- DISCARDABLE指示什么？尽量丢弃？
*   STRINGTABLE -- 每个RC文件中只能有一个字符串表，每行字符串不能超过255个字符
*   WAVE -- 声音，使用 PlaySound 播放，SND_PURGE(停止播放)
******************************************************************************************************/

/******************************************************************************************************
* Windows 2000分层窗口 -- SetLayeredWindowAttributes, 可以实现窗体透明特效
*   WS_EX_LAYERED 扩展窗口风格 -- 窗体将具备复合形状、动画、Alpha混合等方面的视觉特效
*   创建圆形窗体：在Windows 9x下的正确做法是通过 SetWindowRgn 函数指出需要的窗体形状，但是这种处理在频繁更改窗体形状
*     或是在屏幕上拖拽时仍有缺陷存在：前台窗体将要求位于其下的窗体重绘整个区域，这将生过多的消息和计算量。
*     而且使用 SetWindowRgn 只能实现窗体的全透明而无法实现半透明效果。
*   分层窗口真正实现了两个截然不同的概念：分层和重定向。
*     1. 设置 WS_EX_LAYERED 属性;
*     2. 通过 UpdateLayeredWindow 函数来更新分层窗口 -- 需要在位图中绘制出可视区域，并将其与关键色、
*        Alpha混合参数等一起提供给 UpdateLayeredWindow 函数,此时应用程序并不需要响应WM_PAINT或其他绘制消息.
*     或: 使用传统的Win32绘制机制 -- 调用 SetLayeredWindowAttributes 完成对关键色(COLORREF crKey)或阿尔法(BYTE bAlpha)混合参数值的设定,
*         之后系统将开始为分层窗口重定向所有的绘制并自动应用指定的特效
*   半透明窗体:(#define _WIN32_WINNT 0x0501)
*     SetWindowLong(m_hWnd,GWL_EXSTYLE,GetWindowLong(m_hWnd,GWL_EXSTYLE) | WS_EX_LAYERED );
*     //将对话框的窗体(颜色为COLOR_BTNFACE的地方)设为透明并且不再进行点击检测,其他半透明
*     SetLayeredWindowAttributes(GetSysColor(COLOR_BTNFACE),127,LWA_COLORKEY|LWA_ALPHA); 
*     //RedrawWindow(NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
*   异形窗体 -- 编辑出具有特定KeyColor的图片,使用 SetLayeredWindowAttributes 设置KeyColor
*   阴影效果 -- http://www.codeproject.com/Articles/16362/Bring-your-frame-window-a-shadow
* MoveWindow
*  WM_WINDOWPOSCHANGING => WM_WINDOWPOSCHANGED => WM_MOVE=> WM_SIZE => WM_NCCALCSIZE
* SetWindowPos -- 改变一个窗口的尺寸，位置和Z序
*   WM_SYNCPAINT(!SWP_DEFERERASE)
*   WM_NCCALCSIZE(SWP_FRAMECHANGED)
*   WM_WINDOWPOSCHANGING(!SWP_NOSENDCHANGING)
*   WM_WINDOWPOSCHANGED 
* BeginDeferWindowPos/DeferWindowPos/EndDeferWindowPos -- 一次性移动多个窗口
* SetForegroundWindow
*
* Z序
*   顶端(Topmost) => 顶层(普通) => 子窗口(后创建在上面)
*   BringWindowToTop -- 同类的顶部
******************************************************************************************************/

/******************************************************************************************************
* 模拟按键 Paste ( Ctrl + V )
*   keybd_event(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0), 0, 0);
*   keybd_event('V', MapVirtualKey('V', 0), 0, 0);
*   keybd_event('V', MapVirtualKey('V', 0), KEYEVENTF_KEYUP, 0);
*   keybd_event(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0), KEYEVENTF_KEYUP, 0);
*
* 鼠标位置检测(HitTest), 系统已经定义了 HTOBJECT、HTCLIENT 等宏
******************************************************************************************************/


namespace FTL
{
#define DUMP_FILTER_MOUSE_MOVE              ((DWORD)(0x0001L))
#define DUMP_FILTER_NCHITTEST               ((DWORD)(0x0002L))
#define DUMP_FILTER_SETCURSOR               ((DWORD)(0x0004L))
#define DUMP_FILTER_PAINT					((DWORD)(0x0008L))
#define DUMP_FILTER_IDLE					((DWORD)(0x0010L))

#define DUMP_FILTER_KEYDOWN					((DWORD)(0x1000L))
#define DUMP_FILTER_TIMER                   ((DWORD)(0x2000L))

#define DEFAULT_DUMP_FILTER_MESSAGE \
    DUMP_FILTER_MOUSE_MOVE\
    |DUMP_FILTER_NCHITTEST\
    |DUMP_FILTER_SETCURSOR\
	|DUMP_FILTER_PAINT\
	|DUMP_FILTER_IDLE

    //在Output中Dump出当前接受到的消息
#ifdef FTL_DEBUG
#  define DUMP_WINDOWS_MSG(pszName, filters, uMsg, wParam, lParam) \
    {\
        BOOL bFilterd = FALSE;\
        if( (filters) & DUMP_FILTER_MOUSE_MOVE)\
        {\
			bFilterd = (WM_MOUSEMOVE == uMsg || WM_NCMOUSEMOVE == uMsg) ? TRUE : bFilterd; \
        }\
        if( (filters) & DUMP_FILTER_NCHITTEST )\
        {\
            bFilterd = (WM_NCHITTEST == uMsg) ? TRUE : bFilterd;\
        }\
        if( (filters) & DUMP_FILTER_SETCURSOR )\
        {\
            bFilterd = (WM_SETCURSOR == uMsg) ? TRUE : bFilterd;\
        }\
		if( (filters) & DUMP_FILTER_PAINT )\
		{\
			bFilterd = (WM_PAINT == uMsg || WM_ERASEBKGND == uMsg) ? TRUE : bFilterd;\
		}\
		if( (filters) & DUMP_FILTER_IDLE )\
		{\
			bFilterd = (WM_ENTERIDLE == uMsg) ? TRUE : bFilterd;\
		}\
		if( (filters) & DUMP_FILTER_KEYDOWN)\
		{\
			bFilterd = (WM_KEYDOWN == uMsg) ? TRUE : bFilterd; \
		}\
        if( (filters) & DUMP_FILTER_TIMER )\
        {\
            bFilterd = (WM_TIMER == uMsg || WM_SYSTIMER == uMsg) ? TRUE : bFilterd;\
        }\
        if(!bFilterd)\
        {\
            FTLTRACE(TEXT("%s(%d) %s, wParam=0x%x, lParam=0x%x, Tick=%d\n"),\
            pszName,uMsg, FTL::CFMessageInfo(uMsg, wParam, lParam).GetConvertedInfo(),\
			wParam, lParam, GetTickCount() );\
        }\
    }
#else
#  define DUMP_WINDOWS_MSG(pszName, filters, uMsg, wParam, lParam) __noop
#endif 

	//通过 RegisterWindowMessage 注册的消息
	//非线程安全
	class CFRegistedMessageInfo
	{
	public:
		FTLINLINE BOOL Init();

		FTLINLINE explicit CFRegistedMessageInfo();
		FTLINLINE virtual LPCTSTR GetMessageInfo(UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		BOOL m_bInited;
		TCHAR m_bufInfo[128];

		UINT RWM_MSH_MOUSEWHEEL;
		UINT RWM_COLOROKSTRING;
		UINT RWM_FILEOKSTRING;
		UINT RWM_FINDMSGSTRING;
		UINT RWM_HELPMSGSTRING;
		UINT RWM_LBSELCHSTRING;
		UINT RWM_SETRGBSTRING;
		UINT RWM_SHAREVISTRING;
		UINT RWM_COMMDLG_FIND;
		UINT RWM_HTML_GETOBJECT;
	};

    //! 将消息( WM_XXX )转换为易读的格式，类似于 ",wm"
    FTLEXPORT class CFMessageInfo : public CFConvertInfoT<CFMessageInfo,UINT>
    {
    public:
        FTLINLINE explicit CFMessageInfo(UINT msg, WPARAM wParam, LPARAM lParam);
        FTLINLINE virtual LPCTSTR ConvertInfo();
    public:
        WPARAM m_wParam;
        LPARAM m_lParam;

	private:
		static CFRegistedMessageInfo	s_RegistedMessageInfo;
    };

    #define BEGIN_WINDOW_RESIZE_MAP(thisClass) \
        static const _WindowResizeMap* GetWindowResizeMap() \
        { \
            static const _WindowResizeMap theMap[] = \
            {

    #define END_WINDOW_RESIZE_MAP() \
                { (DWORD)(-1), 0 }, \
            }; \
            return theMap; \
        }

    #define WINDOW_RESIZE_CONTROL(id, flags) \
                { id, flags },

    //开始定义一个组，之后定义的Control的位置将以Group前的一个Control的位置为标准
    #define BEGIN_WINDOW_RESIZE_GROUP() \
                { -1, _WINSZ_BEGIN_GROUP },

    #define END_WINDOW_RESIZE_GROUP() \
                { -1, _WINSZ_END_GROUP },


    FTLEXPORT template <typename T>
    class CFMFCDlgAutoSizeTraits
    {
    public:
        static HWND GetWinHwndProxy(T* pThis)
        {
            return pThis->GetSafeHwnd();
        }
        static DWORD GetStatusBarCtrlID()
        {
            return AFX_IDW_STATUS_BAR;
        }
        //static LRESULT OnSizeProxy(T* pThis,UINT nType, int cx, int cy);
    };

    FTLEXPORT template <typename T>
    class CFWTLDlgAutoSizeTraits
    {
    public:
        static HWND GetWinHwndProxy(T* pThis)
        {
            return pThis->m_hWnd;
        }
        static DWORD GetStatusBarCtrlID()
        {
            return ATL_IDW_STATUS_BAR;
        }
        //static void OnSizeProxy(UINT nType, CSize size);
    };

    //! 从WTL的 CDialogResize 拷贝并改造的自动调整窗体控件位置、大小的类，支持MFC、WTL的Dialog
    //! TODO:支持普通的Windows，如 View；增加窗体位置保存、恢复等功能（扩展WTL）
    //! Bug:是否有 TWindowAutoSizeTraits 的必要？
    //! 使用方法：继承列表中增加 public FTL::CFWindowAutoSize<CMyDlg,FTL::CFWTLDlgAutoSizeTraits<CMyDlg> >
    //!   然后在 OnInitDialog 中调用 InitAutoSizeInfo，在 WM_SIZE 处理函数中调用 AutoResizeUpdateLayout，处理 WM_GETMINMAX...
    //! 窗体如果具有 WS_CLIPCHILDREN 属性，可以防止出现闪烁？但实际测试时发现Child时无法正确绘制
    FTLEXPORT template <typename T, typename TWindowAutoSizeTraits >
    class CFWindowAutoSize
    {
    public:
        // Data declarations and members
        enum
        {
            WINSZ_SIZE_X        = 0x00000001,   //自动横向扩展、伸缩，保证左、右边界不变
            WINSZ_SIZE_Y        = 0x00000002,   //自动纵向扩展、伸缩，保证上、下边界不变
            WINSZ_MOVE_X        = 0x00000004,   //自动横向移动，保证右边界不变
            WINSZ_MOVE_Y        = 0x00000008,   //自动纵向移动，保证下边界不变
            WINSZ_CENTER_X      = 0x00000010,   //自动横向移动，保证左、右边界不变
            WINSZ_CENTER_Y      = 0x00000020,   //自动纵向移动，保证上、下边界不变
            WINSZ_REPAINT       = 0x00000040,   //Resize后强制重绘，通常没有必要 

            // internal use only
            _WINSZ_BEGIN_GROUP  = 0x00001000,
            _WINSZ_END_GROUP    = 0x00002000,
            _WINSZ_GRIPPER      = 0x00004000    //内部使用，表明是拖拽的Gripper，只能有一个(内部设置)
        };
        struct _WindowResizeMap
        {
            DWORD   m_nCtlID;
            DWORD   m_dwResizeFlags;
        };
        struct _WindowResizeData
        {
            int     m_nCtlID;
            //! 0x   7   6   |    5   4   |    3   2   1   0    |
            //!      未用(0) |   Group数  |      WINSZ 标志     |
            DWORD   m_dwResizeFlags; 
            RECT    m_rect;

            int GetGroupCount() const
            {
                return (int)LOBYTE(HIWORD(m_dwResizeFlags));
            }

            void SetGroupCount(int nCount)
            {
                FTLASSERT(nCount > 0 && nCount < 256);
                FTLASSERT(( m_dwResizeFlags & 0x00FFFFFF ) == m_dwResizeFlags); //目前最高的一个字节没有内容
                DWORD dwCount = (DWORD)MAKELONG(0, MAKEWORD(nCount, 0));
                m_dwResizeFlags &= 0xFF00FFFF;
                m_dwResizeFlags |= dwCount;
            }

            bool operator ==(const _WindowResizeData& r) const
            { 
                return (m_nCtlID == r.m_nCtlID && m_dwResizeFlags == r.m_dwResizeFlags); 
            }
        };
        typedef std::vector<_WindowResizeData> WindowResizeDataArray;
        WindowResizeDataArray   m_allResizeData;
        SIZE    m_sizeWindow;
        POINT   m_ptMinTrackSize;
        BOOL    m_bGripper;
    public:
        FTLINLINE CFWindowAutoSize();
        FTLINLINE BOOL InitAutoSizeInfo(BOOL bAddGripper = TRUE, BOOL bUseMinTrackSize = TRUE);
        FTLINLINE BOOL AutoResizeUpdateLayout(int cxWidth, int cyHeight);
    protected:
        FTLINLINE BOOL AutoPositionControl(int cxWidth, int cyHeight, RECT& rectGroup, _WindowResizeData& data, bool bGroup, 
            _WindowResizeData* pDataPrev = NULL);
    };
    //管理显示器的信息(如双显)
    class CFMonitorManager
    {
    public:
    private:
        //static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
    };

	enum ColorType
	{
		ctCtrlColor,
		ctSysColor,
	};
	
	typedef BOOL (CALLBACK* TranslateWndClassProc)(LPCTSTR pszOriClassName, LPTSTR pszNewClass, UINT nLength);
	
    FTLEXPORT class CFWinUtil
    {
    public:
		//查找指定ProcessId的主窗口ID
		FTLINLINE static HWND GetProcessMainWindow(DWORD dwProcessId);

        //激活并将指定窗体放在最前方 -- 解决在状态栏闪烁但不到前台的Bug
        //常用于二重启动时将窗体显示在前台
        FTLINLINE static BOOL ActiveAndForegroundWindow(HWND hWnd);

        //全屏窗口：WS_POPUP|WS_VISIBLE 属性(不能是 WS_OVERLAPPEDWINDOW ), 0,0 ~ CxScreen, CyScreen
        FTLINLINE static BOOL SetWindowFullScreen(HWND hWnd,BOOL isFullScreen, BOOL &oldZoomedState);
        FTLINLINE static LPCDLGTEMPLATE LoadDialog(HMODULE hModuleRes, LPCTSTR szResource, HINSTANCE * phInstFoundIn);

		//获取 GetSysColor 时颜色索引对应的字符串
		FTLINLINE static LPCTSTR GetColorString(ColorType clrType, int nColorIndex);

        //WM_HSCROLL 或 WM_VSCROLL 的通知码
        FTLINLINE static LPCTSTR GetScrollBarCodeString(UINT  nSBCode);

        //获取 WM_NOTIFY 消息 Code 对应的字符串信息
        FTLINLINE static LPCTSTR GetNotifyCodeString(HWND hWnd, UINT nCode, LPTSTR pszCommandNotify, int nLength, TranslateWndClassProc pTransProc = NULL);

        //获取 WM_COMMAND 消息的 notifyCode
        FTLINLINE static LPCTSTR GetCommandNotifyString(HWND hWnd, UINT nCode, LPTSTR pszCommandNotify, int nLength, TranslateWndClassProc pTransProc = NULL);

		//获取窗体的类型、名字、位置、大小等最基本的信息
		FTLINLINE static LPCTSTR GetWindowDescriptionInfo(FTL::CFStringFormater& formater, HWND hWnd);
		//获取 Windows 窗体属性对应的字符串信息 
        FTLINLINE static LPCTSTR GetWindowClassString(FTL::CFStringFormater& formater, HWND hWnd, LPCTSTR pszDivide = TEXT("|"));
        FTLINLINE static LPCTSTR GetWindowStyleString(FTL::CFStringFormater& formater, HWND hWnd, LPCTSTR pszDivide = TEXT("|"));
        FTLINLINE static LPCTSTR GetWindowExStyleString(FTL::CFStringFormater& formater, HWND hWnd, LPCTSTR pszDivide = TEXT("|"));

		FTLINLINE static LPCTSTR GetOwnerDrawState(FTL::CFStringFormater& formater, UINT itemState, LPCTSTR pszDivide = TEXT("|"));
        FTLINLINE static LPCTSTR GetOwnerDrawAction(FTL::CFStringFormater& formater, UINT itemAction, LPCTSTR pszDivide = TEXT("|"));
#if 0
        //窗体居中放置(ATL有源码？)
        FTLINLINE static BOOL CenterWindow(HWND hWndCenter , HWND hWndParent, BOOL bCurrMonitor);
#endif
    };//CFWinUtil

	//SetWindowHook 时的一些辅助方法
	FTLEXPORT class CFWinHookUtil
	{
	public:
		FTLINLINE static HWND GetCBTCodeInfo(CFStringFormater& formater, int nCode, WPARAM wParam, LPARAM lParam);
	};

    //DI项目中使用来适配Notification的基类
    template <typename T>
    class CFUIAdapterForWin
    {
    public:
        //typedef T     parent_class;

        //virtual bool PostNotification(XfNotificationPtr pNotify) = 0;
        virtual void UpdateUI() = 0;

        //static bool PostNotificationProxy(XfNotificationPtr pNotify, void* data)
        //{
        //    T* pThis = static_cast<T*>(data);
        //    return pThis->PostNotification(pNotify);
        //}
        static void UpdateUIProxy(void* data)
        {
            T* pThis = static_cast<T*>(data);
            pThis->UpdateUI();
        }
    };

    //对MessageBox进行Hook，可以更改按钮的文本，目前没有更改按钮的大小
    //! 用法：CFMessageBoxHook(m_hWnd, TEXT("MyClose"));
    //        ::MessageBox(...);
    #define PREV_WND_PROC_NAME  TEXT("PrevWndProc")
    class CFMessageBoxHook
    {
    public:
         //更改从线程弹出的对话框上的OK按钮文字
        FTLINLINE CFMessageBoxHook(DWORD dwThreadId, LPCTSTR pszOKString);
        FTLINLINE ~CFMessageBoxHook(void);
    private:
        FTLINLINE static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam);
        FTLINLINE static LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    private:
        static HHOOK    s_hHook;
        static HWND     s_ProphWnd;
        static LPCTSTR  s_pszOKString;
    };

    class CFLockUpdate     
    { 
    public: 
        CFLockUpdate(HWND hwnd, BOOL bHighPrivilege = FALSE)
            :m_bHighPrivilege(bHighPrivilege)
            ,m_hwnd(hwnd) 
          { 
              if(m_hwnd==NULL)return; 
              if(m_bHighPrivilege) 
                  LockWindowUpdate(m_hwnd); 
              else 
                  SendMessage(m_hwnd,WM_SETREDRAW,FALSE,0); 
          } 
          ~CFLockUpdate() 
          { 
              if(m_hwnd==NULL)
              {
                  return; 
              }
              if(m_bHighPrivilege) 
                  LockWindowUpdate(NULL); 
              else 
                  SendMessage(m_hwnd,WM_SETREDRAW,TRUE,0); 
          } 
    private: 
        BOOL   m_bHighPrivilege; 
        HWND   m_hwnd; 
    };

#if 0
    template <typename ControlT , typename ConverterFun>
    class CControlPropertyHandleT
    {
    public:
        CControlPropertyHandleT(ControlT& control);//, ConverterFun& fun);
        FTLINLINE INT AddProperty(INT value);
    private:
        ControlT&       m_control;
        //ConverterFun&   m_fun;
    };
#endif

}//namespace FTL

#endif //FTL_WINDOW_H

#ifndef USE_EXPORT
#  include "ftlwindow.hpp"
#endif 