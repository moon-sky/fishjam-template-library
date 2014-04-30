#ifndef FTL_DUI_H
#define FTL_DUI_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlDUI.h requires ftlbase.h to be included first
#endif

//Duilib入门教程 -- http://www.cnblogs.com/Alberl/tag/duilib%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B/

//TODO:
//  1.duilib里面并没有对Capture做处理，分发鼠标消息到对应的子控件上，可能是还没有完善的原因
//    需要小心是否支持拖拽
//  2.duilib对多线程的支持并不是很好? -- 
//  3.不支持动画
//  4.未实现 IAccessible 接口 


/*************************************************************************************************************************
* DuiLib，一个中文的开源DUI分支 :http://duilib.googlecode.com/svn/trunk， 遵循BSD协议，可以免费用于商业项目
*   完全兼容ActiveX控件(如IE、Flash等)，可以和MFC等界面库配合使用
*   
* 工具类(不依赖任何外部的库，所以内部实现了很多用于支撑项目的基础类)
*   CDelegate -- 事件代理，通过 OnXxx += MakeDelegate(this, &OnXxxHandle) 关联事件和处理函数
*   CEventSource -- 事件源,通过数组 保存 CDelegateBase 的实例，在需要时可以依次激发所有的事件代理，
*     如果任一处理函数返回false，则中断调用并返回false。否则激发全部事件后返回true
*   简单容器：CStdPtrArray / CStdValArray / CStdString / CStdStringPtrMap
*
* 控件相关类
*   CControlUI -- 所有控件的基类，控件树中所有的节点都是一个CControlUI
*     DoPostPaint -- 可供子类重载的绘制后的回调，
*     Event/DoEevnt -- 处理事件
*     GetInterface(避免强转) -- 根据传入的字符串，传出指向控件的指针，如果没有对应的控件类型，则返回NULL
*     SetAttribute -- 从XML中解析出控件的各个属性，传入字符串的属性名称和值进行设置
*   CContainerUI -- 容器基类，其内部用一个数组(m_items)来保存所有的CControlUI的对象，
*       所有的容器都支持滚动条，对键盘和鼠标滚动事件进行处理(CContainerUI::DoEvent)，对其内部所有元素调整位置，绘制时实现滚动效果。
*       TODO：绘制控制(不在显示区域的不绘制)，控件会判断绘制区域，如果自己不在显示区，则不绘制
*     FindControl -- 查找子控件
*     Remove/RemoveAt -- 移除子控件
*     SetAutoDestroy -- 控制是否自动销毁，如果是延迟销毁的话，会由 CPaintMangerUI 销毁?
*   
*   基本控件(Control目录下,父类是 CControlUI) -- 
*     CActiveXUI -- ActiveX对象，通常响应 DUI_MSGTYPE_SHOWACTIVEX 后，调用 CActiveXUI::GetControl(IID_IUnknown,xxxx) 获得对应的控件接口
*     CButtonUI(Button)
*     CLabelUI
*     CListUI/CListTextElementUI -- 
*     CRichEditUI
*     CScrollBarUI --
*     CTextUI -- 
*     CWebBrowserUI -- 内嵌 IWebBrowser2
*
*   Layout(IContainerUI -> CContainerUI) -- 布局控制
*     CChildLayoutUI
*     CDialogLayoutUI
*     CHorizontalLayoutUI/CVerticalLayoutUI/CTileLayoutUI -- 纵向排列，横向排列格子排列  
*     CTabLayoutUI -- Tab 排列
* 
*   CRenderClip -- 裁减区域控制
*   CRenderEngine -- 渲染引擎，很多可以通用的静态绘制代码都在这里。比如 九宫格拉伸图片，绘制区域、文字等
*   CPaintManagerUI -- 控件绘制管理，对应于一个Windows的原生窗口，主要负责 控件、资源的管理，转化并分发Windows原生窗口消息
*     Init -- 通过原生HWND进行初始化
*     FindControl -- 根据名字、位置等查找指定的控件
*     FindSubControlByName -- 从指定容器中根据名字找到子控件，未找到的话会返回NULL
*     MessageHandler -- 内部会对很多窗口消息进行处理，并将其分发到对应的控件上去
*     SetResourcePath/SetResourceZip -- 设置皮肤资源的路径(通常是调试版本)和Zip文件(通常是Release版本)
*
*   CWindowWnd -- 原生窗口控制(Create/Subclass/HandleMessage等),通过该类将原生窗口的消息分发给其派生类，最后传给整个控件体系。
*     HandleMessage -- 基本的消息分发
*     OnFinalMessage -- 通常new出来的窗体实例需要在这个方法中 delete this
*   WindowImplBase -- DUI为了简化使用而提供的一个封装类，提供了常用的一些功能，★通常用户的各个原生容器窗体就是从该类继承★，
*     CPaintManagerUI m_PaintManager;  -- 保护成员变量，对该窗体的绘制进行管理，
*     HandleMessage -- 进行一些基本的消息处理(采用了虚函数的方式实现消息映射，如子类对这些消息感兴趣，需要重载对应的消息处理函数)
*     HandleCustomMessage -- 子类可以重载处理感兴趣的自定义或基类未处理的消息
*     GetWindowClassName -- 子类必须重载实现的纯虚函数
* 
* 消息(事件)处理方式 -- ( Event + Notify )
*   消息映射(通过字符串来区分的，需要从 CNotifyPump 或其子类继承), 最简单的响应函数的类型是 void CMyXXX::OnClick( TNotifyUI &msg );
*     头文件中：  DUI_DECLARE_MESSAGE_MAP 声明消息映射相关的变量和方法
*     实现文件中：DUI_BEGIN_MESSAGE_MAP(xxx, CNotifyPump) -> DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick) -> DUI_END_MESSAGE_MAP
*   IDialogBuilderCallback -- 根据传入的Class名，返回指定类型控件的实例?
*   INotifyUI -- 最基本的消息处理接口，使用 TNotifyUI 结构传递消息， 可以将其看成和WM_NOTIFY差不多的功能?
*     TNotifyUI:
*       sType -- DUI_MSGTYPE_XXX, 如 DUI_MSGTYPE_CLICK
*       sVirtualWnd -- 通过 AddVirtualWnd 加入时的虚拟窗体名字
*       pSender -- 发送通知的控件，一般通过其 GetName 来判断
*   IMessageFilterUI -- PreMessageFilter(MessageHandler 纯虚方法)，消息预处理
*   ITranslateAccelerator -- 加速键转换接口，纯虚方法 TranslateAccelerator
*   CEventSource -- 事件代理 
*     OnNotify += MakeDelegate(this, &CFrameWindowWnd::OnHChanged);
*   CNotifyPump -- 消息映射的基类，消息映射采用了虚窗体的方式(比如 WindowImplBase 中内嵌非窗体的 Page页)，
*     可通过 AddVirtualWnd 加入，然后通过 TNotifyUI::sVirtualWnd 递归找到对应的虚窗体进行消息处理
*  注意：
*    CPaintManagerUI::MessageHandler -- 内部会对很多窗口消息进行处理，并将其分发到对应的控件上去
* 
* 资源和皮肤管理(Deubg时使用资源目录，Release时使用Zip包)，通过XML配置界面控件布局和风格，支持带Alpha通道的PNG图片
*   CDialogBuilder -- 通过配置文件自动创建皮肤
*     Create -- 解析皮肤包（文件夹和zip格式）中的XML文件，并创建各种控件，返回控件树中的根节点控件
*
* 其他支持工具
*   UIDesigner -- 所见即所得的开发工具，但似乎老是有问题
* 
* XML资源(没有 DTD/Schema 的验证文档?)，描述窗口中控件的布局和样式
*   Window -- 最外层的窗体控件，其中有 Font、Default(VScrollBar/HScrollBar) 等子节点定义属性
*   VerticalLayout/HorizontalLayout/TabLayout/TileLayout/ChildLayout -- 布局控件，其子控件按照指定的方式自动布局
*   Container -- 可包含子控件，也可以用于填充占位?(默认大小是自动扩充剩余的位置)
*   各种控件
*     Edit
*     Label -- 标签控件
*     List(CListUI) -- 由 ListHeader/ListContainerElement 组合而成
*       TODO:如何动态增加 Item项，能把Element写成可重用的方式不？
*       IListCallbackUI -- 可以实现类似虚列表的功能？但只能返回文字，不能绘制图片等?
*     Option -- 可以实现Sheet中的切换Tab页(参见RichListDemo)
*     Progress -- 进度条
*   常见属性:
*     normalimage/hotimage/selectedimage/bkimage -- 
*************************************************************************************************************************/

/*************************************************************************************************************************
* DUI -- DirectUI
*   DirectUI and Duser Architecture
*   Resource Management and Build System
*   UI Files: Markup, Styles, and Macros
*   Layout, Rendering, and Animations
*   Notifications: Events, Listeners, Handlers, and Behaviors
*   Common Controls, Custom Controls, and Compound Controls
*   Best Practices and Future Work
*
* DUser -- Streamlined low level window manager (extending Window's User architecture. ) Includes painting coordination, 
*	gadget tree management, input management, messaging, and simple animations.
* DirectUI -- System above DUser including declarative layout, stylesheets, keyboard navigation, MSAA support, 
*	common controls, and content rendering	.
* Gadget -- Can be thought of as light weight HWNDs.  Can host HWNDs, or other Gadgets.
* DUI -- The whole of DUser and DirectUI as a platform.  Commonly used to refer to UX Platform.
* Parser -- Parser that takes UI markup in, and creates DirectUI visual tree.
* Layout -- Instructions given to element to allow for child positioning.
* Resource Management -- System designed to allow for build time processing and packaging of resources, 
*	runtime resource lookup, and extensions to the Win32 resource model.
* RCXML -- Build time resource processing tool.
*
*************************************************************************************************************************/

namespace FTL
{
}
#endif //FTL_DUI_H

#ifndef USE_EXPORT
#  include "ftlDUI.hpp"
#endif 