#ifndef COM_COMMUNICATION_H
#define COM_COMMUNICATION_H
#pragma once

/******************************************************************************************************
* 注意：
*   1.(通常情况下：)Office(Word)等中有 IApplicationEvents2(列出了方法的interface) 和 ApplicationEvents2(客户端需要处理Evnets的dispinterface)，
*     ApplicationEvents2 客户端中 Invoke 时的 dispid 即是 IApplicationEvents2 中方法的顺序(从1开始计数)。
*     如 IApplicationEvents2 的第二个函数 Quit 对应的 dispid 为 2。详情参见 从 MSWORD.OLB import 生成的 msword.tlh 文件
*     ★问题★：从什么地方可以得到各个Events的 DISPID_XXXX -- 通过 OleView.exe 可以看到全部信息，但不好搜索，有没有更好的工具？
*   2.可以在 IDispEventImpl 中重载 Invoke 方法，打印出各种参数(如各种接口或参数)
*     注意：pdispparams->rgvarg 的顺序和函数签名中参数的顺序是反着的，因此需要从后向前输出信息
*     	FTL::CFIExplorerDispidInfo IExplorerDispId(dispidMember); FTLTRACE(TEXT("%s:\n"), IExplorerDispId.GetConvertedInfo()); //IE中的ID
*       for(UINT index = 0; index < pdispparams->cArgs ; index++) 
*       { 
*          FTL::CFVariantInfo varInfo(pdispparams->rgvarg[pdispparams->cArgs - index - 1]);
*          FTLTRACE(TEXT("\t %d, %s\n"), (index + 1), varInfo.GetConvertedInfo());
*       }
*       return __super::Invoke(xxxx);

*
* C语言中使用回调通知客户端，C++中通常使用 窗口消息、事件接口、硬编码回调方法名、thunk、 统一事件模型 等
*
* 组件主动与客户进行通信，回调客户提供的功能的机制
* 
* 回调接口（使用自定义接口，速度快，但限制大 -- 直接使用回调函数地址，只能用于本地 DLL 服务）
*   当COM组件中发生特定事件(如下载完成、定时到、用户操作等)时，需要通知调用者。
*   通过 “回调”函数的机制，将“回调接口--接收器接口ISink”设置到 COM 服务器中，发生事件时，COM调用该接口完成通知。
*   1.COM服务器“定义”将用于通知的回调接口 IMyMathCallBack，其中有通知结果的函数 Fire_Result，该接口将由客户端“实现”
*     在接口列表中需要使用 source 表明该接口是回调接口； [source,default] interface IMyMathCallBack;
*     由于 IMyMathCallBack 接口是由客户端实现的，因此不需要增加到 BEGIN_COM_MAP 中
*   2.COM服务器定义客户端要调用的方法 IMyMath::Sub，但不通过函数参数返回结果，而是通过 Fire_Result 通知结果；
*   3.客户端调用 IMyMath::Advise([in] IMyMathCallBack *pCallBack,[out] long *pdwCookie) 把客户端实现的接收器(sink)
*     接口指针(IMyMathCallBack *)传递到组件服务器中；其中定义的 dwCookie 用于区分多个 CallBack；当调用 Unadvise([in] long dwCookie)
*     时知道应该断开哪个回调；
*
* 连接点 -- 使用出接口(Outgoing Interface)，由客户的事件接收器(Sink)实现，用于向它的容器激发事件
*   使用系统预定义接口，速度慢，但可以跨进程、网络，是一种伪异步(pseudo-asynchronous)机制,COM+才支持真正的异步
*   1.一个 COM 组件，允许有多个连接点对象(IConnectionPoint)，即可以有多个发生“事件”的源头；
*   2.管理这些连接点的接口叫“连接点容器”(IConnectionPointContainer)，有2个函数（FindConnectionPoint、EnumConnectionPoints）；
*   3.每一个连接点，可以被多个客户端的接收器(Sink)连接 -- 使用 cookie 进行区别；
*   可连接对象(IConnectionPointContainer) 1 -> 1 连接点枚举器(IEnumConnectionPoints) 1 -> n 连接点(IConnectionPoint)
*   向导步骤：
*     1.ATL 对象向导中选择“支持连接点”,需要“双重”接口，会定义出 dispinterface _IXXXEvents 的事件接口
*     2.idl中增加接口函数(IMyTimer::SetTimer)；
*     3.idl中增加连接点事件接口(_IMyTimerEvents)的通知函数 Fire_Timer -- 注意：只需要输入 Timer 即可，生成后需要编译一下；
*     4.选择组件对象(CMyTimer)，右键 -> 添加 -> 添加连接点，将会自动生成事件代理类程序代码(CProxy_IMyTimerEvents::Fire_Timer)；
*     5.在适当的位置调用IDE帮我们生成的代理函数代码，发出事件，如 Fire_Timer(v)；
*     6.在接口映射(BEGIN_COM_MAP)中加入 IConnectionPointContainer 的映射 -- 向导自动生成？
*     7.向导会在连接点映射中添加映射 CONNECTION_POINT_ENTRY(__uuidof(_IMyTimerEvents))；
*     8.由于连接点的事件通过 Invoke 通知客户端（Fire_Timer），因此客户端接收器要从 IDispatch 派生，并在 Invoke 中
*       处理连接点函数穿过来的信息。Invoke要处理的序号就是 Fire_XXX 对应的id，如 Timer？
*   客户端使用：
*     1.COM组件实现了 连接点容器(IConnectionPointContainer) 和 连接点(IConnectionPoint -- 但该接口通常不直接导出) 接口，可以：
*       a.获取 IConnectionPointContainer 接口后，通过 FindConnectionPoint(__uuidof(_IMyTimerEvents),&m_spCPTimer)获得连接点接口；
*     2.调用 m_spCPTimer->Advise( &m_TimerSink, &m_dwCPCookie) 建立事件接收器(m_TimerSink)与连接点的连接。
*     2.使用MFC 的 DECLARE_EVENTSINK_MAP() / BEGIN_EVENTSINK_MAP(...) -> On_EVENT(...) -> END_EVENTSINK_MAP() 来进行映射
*       如 ON_EVENT(CTestAtlDlg, IDC_ATLCOMP1,     1    ,  OnAtlEvent, VTS_I4  VTS_BSTR)，
*         参数分别是：处理的类,  控件ID,      Invoke 的 ID, 处理函数，参数类型列表(上面代表 long state,char * buf)
*     3.客户端需要实现 IDispatch 接口，并在Invoke中处理回调（如果用ATL实现，可以 CComObject<CMyMathEvents> ）
*   ATL连接点客户端
*     1.从 IDispEventSimpleImpl 或 IDispEventImpl 继承
*     2.通过 BEGIN_SINK_MAP -> SINK_ENTRY(针对源对象的唯一ID, dispid, func)/SINK_ENTRY_EX(xxx) -> END_SINK_MAP 宏声明事件接收映射
*       事件接收函数(类的成员函数)签名格式和事件接口中定义的方法签名一致， 
*       如 HRESULT _IMyTimerEvents::Timer([in] VARIANT varDate) 对应的处理函数应该是 STDMETHOD(OnTimer)(VARIANT varDate);
*     3.对需要处理的事件接口通过调用 DispEventAdvise/DispEventUnadvise 函数通知和取消通知连接点
*       本质是先强制Cast后，通过 QI 到 IConnectionPointContainer，然后 FindConnectionPoint 指定Event接口后 Advise/Unadvise
*       注意：
*         a.DispSink的例子程序的使用方式为(IDispEventImpl<ID, Cxxx, DIID_IXXXEvents> 子类中)
*           CoCreateInstance(CLSID_XXX,xxx, &m_pIServ) <== 创建COM服务器的实例
*           AtlGetObjectSourceInterface(m_pIServ, &m_libid, &m_iid, &m_wMajorVerNum, &m_wMinorVerNum) <== 获得默认类型信息，并存起来
*           DispEventAdvise(m_pIServ, &m_iid) <== 建立连接点
*         b.不能直接用 AtlAdvise/AtlUnadvise的理由：
*           IDispEventImpl和IDispEventSimpleImpl这两个类并没有直接继承IDispatch，无法通过QueryInterface来直接获得对应事件回调接口，
*           通过DispEventAdvise内部强制Cast到IUnknown（IDispEventSimpleImpl虽然不是从IUnknown继承，但定义了IUnknown和IDispatch的所有方法，VTable的布局一致）

******************************************************************************************************/


/******************************************************************************************************
* 统一事件模型 ( 组件将有关异步“事件”的信息传递回客户端, 对于本机C++(native)、COM 和托管类以一致的方式实现)
*   该模型支持单线程和多线程的使用，并禁止对数据进行同步多线程访问，可以从事件源或接收器类派生子类，并支持此派生类中的扩展事件源/接收。
*   event_source -- 事件源,定义事件和包含事件的对象，使用关键字 __event 定义事件OnXXX(确保只声明方法而不要实现它)，__raise 激发事件
*   event_receiver -- 事件接收器，是接收事件的对象，并在其中实现“事件处理程序”
*   __hook/__unhook -- "挂钩/解除挂钩"事件，通常在 事件接收器的公用方法中实现
*   
*   委托 -- 可保持对方法的引用的类，该类具有签名并且只保持对与其签名相匹配的方法的引用(不同于其他类的地方)，
*  
*   注意：
*   1.似乎不用 event_source/event_receiver 编译指令也可以正确运行？
*   2.event_receiver 如果是一个类，则不能定义和使用成员变量（不会调用该类的构造函数来创建类实例）
*   3.(不懂？)统一事件模型支持动态挂钩，即在循环中挂钩/解除挂钩事件，或在接收参数形式的源方法和本地方法的函数中使用条件逻辑挂钩/解除挂钩事件
*   4.后__hook 的会先被调用，退出时需要 __unhook，否则会有内存泄漏
*   5.__hook(&CMyEventSource::OnValueChanged, pSource, &CMyEventReceiver::OnValueChangedHandler, this);
*     本质是生成如下代码(?): pSource->__AddEventHandle_CMyEventSource_OnValueChanged(XXX)
*  
******************************************************************************************************/

#endif //COM_COMMUNICATION_H