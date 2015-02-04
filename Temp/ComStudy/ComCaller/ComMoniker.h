#ifndef COM_MONIKER_H
#define COM_MONIKER_H
#pragma once

/******************************************************************************************************
* 通过 MkParseDisplayName 方法可以从字符串得到对应的 IMoniker 对象(参见amcap例子)
* Moniker(标记) -- 名字对象(也是COM对象)为组件对象提供了符号化的表示方法，表示COM对象的名字，
*   可以通过名字进行智能启动，客户通过名字对象建立与com对象的连接，名字对象是客户与对象之间的桥梁
*   显示名分隔符：“!”、“\”、“/”、“:”或“[”
*     Excel 中的 Sheet!表格 方式就是 复合名字对象 的例子？
*   绑定：
*     对象的状态：激活状态或者运行状态、被动状态
*     绑定：使对象从被动态自动进入运行态 —— 激活、连接，所以也被称为“永久智能对象”
*     通常是 客户 -> IClassFactory -> COM 对象
*     使用Moniker：客户 -> IMoniker -> COM 对象
*       IMoniker::BindToObject绑定过程：
*         1.检查ROT表
*         2.分解,pmkRight : pmkLeft，最右边部分分离
*         3.调用,pmkRight->BindToObject(...,pmkLeft,...)
*         4.执行,pmkRight->BindToObject，如果pmkLeft为简单名字对象，则可终止循环；
*           否则,pmkRight往往要调用pmkLeft->BindToObject，从而形成自右向左的循环绑定过程
*           如:File!Item1!Item2 的绑定和构造过程
*              客户程序调用 pComp->BindToObject ( 复合名字对象被拆分成pmkFileItem1和pmlItem2两部分)
*                             => pmkItem2->BindToObject 
*                               => pmkFileItem1->BindToObject ( pmkFileItem1被拆分成pmkFile和pmlItem1两部分 )
*                                 => pmkItem1->BindToObject
*                                   => pmkFile->BindToObject(创建文件对象并返回其IOleItemContainer接口指针)
*                                     => 
*  使用方法：
*    1.客户创建名字对象 -- CreateFileMoniker 、CreateItemMoniker
*    2.绑定到名字对象所指的对象 -- IMoniker::BindToObject / BindMoniker
*  复合名字对象 -- CreateGenericComposite
*    按从左到右的顺序组合，满足结合律，如“文档内部的电子表格”
*  ROT(Running Object Table)表 -- COM使用ROT表管理当前系统中正在运行的、已经被注册的名字对象，
*    客户调用GetRunningObjectTable函数访问ROT表，典型的是DirectShow中注册到 Graph Edit 中
*    IRunningObjectTable
*  绑定环境对象(IBindCtx) -- CreateBindCtx，也是COM实现的系统对象
*    管理已被绑定的对象 -- RegisterObjectBound、RevokeObjectBound、ReleaseBoundObjects
*    管理绑定参数：(文件访问模式、超时设置等) -- SetBindOptions、GetBindOptions
*    管理绑定过程中的对象参数 -- RegisterObjectParam、GetObjectParam、EnumObjectParam、RevokeObjectParam
*   
* IOleItemContainer -- ?
*
* COM名字对象分类
*   COM提供的系统名字对象 -- 
*     文件名字对象(CreateFileMoniker)
*     复合名字对象(CreateGenericComposite)
*     单项名字对象(CreateItemMoniker)
*     反-名字对象(CreateAntiMoniker)
*     指针名字对象(CreatePointerMoniker)
*     类名字对象(CreateClassMoniker)
*   URL名字对象
*     异步名字对象 -- 实现了IAsyncMoniker接口，其他接口：IEnumFORMATETC、IBindStatusCallback、
*   自定义名字对象(很少使用) -- 根据需要实现IMoniker接口的成员函数，同时提供一条创建自定义名字对象的途径
*
* 名字对象的应用与发展
*   是OLE链接对象的重要技术保障
*   COM+又扩充了新的名字对象，比如queue:、new:
*   作为客户与COM对象之间连接的一种强有力的手段
*   VBScript中访问对象的主要机制
*
* MFC对名字对象的支持(VC带的例子：OClient和Scribble)
*   COleLinkingDoc
*
* 统一数据传输（UDT）
*   数据传输机制
*     IDataObject -- 表示信息实体的数据对象
*     传输协议 -- 一个应用程序如何得到另一个应用程序所提供的数据对象即IDataObject接口指针，最基本的传输协议为剪贴板、拖-放
*   通过剪贴板传输数据
*   拖-放数据传输协议
*     源：实现 IDataObject/IDropSource 接口，调用 DoDragDrop 函数，该函数内部是一个循环，它不断检查鼠标和键盘的状态，
*         以及询问“放”目标窗口和“源”窗口，并作出反应
*     目标： 实现 IDropTarget 接口，调用 RegisterDragDrop 函数把对象与窗口联系起来
******************************************************************************************************/
#endif //COM_MONIKER_H
