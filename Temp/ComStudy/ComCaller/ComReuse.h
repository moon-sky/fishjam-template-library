#ifndef COM_REUSE_H
#define COM_REUSE_H
#pragma once

/***********************************************************************************************************
* C++  -- 源代码重用（语言限定、名字可能冲突、拷贝重用代码而导致工程膨胀）
* DLLs -- 二进制级共享代码,只能被C或理解C调用规范的语言使用(是平面的)
* COM  -- 二进制标准。 通过接口进行立体化。实现重用而不是代码重用。
* 
* 组件重用(复用) -- 在 FinalConstruct 中创建内部组件，在FinalRelease中释放.
*   1.聚合(Aggregation) -- 内部组件的接口直接向外公开(可以直接QueryInterface到内部组件的接口)，外部组件内部
*       不需要或不使用内部对象的功能，内外组件的生存期都是通过外部组件的 IUnknown 完成。
*       ATL目前只支持进程内组件的聚合。
*     向导中通过选项支持(属性页中 Aggregation)：
*       Yes  -- 由 CComCoClass 默认支持，不增加特别的宏(DECLARE_AGGREGATABLE宏)
*       No   -- 增加 DECLARE_NOT_AGGREGATABLE 宏
*       Only -- 增加 DECLARE_ONLY_AGGREGATABLE 宏
*     集合有两种方式：
*       a.有选择地集合(Selective Aggregation) -- 只在QI指定接口时，交由内部组件处理(推荐)
*           if(IID_XXX == riid) {return m_pInnerUnk->QueryInterface(...);}
*       b.盲目集合(Blind Aggregation) -- 在QI所有未知的接口时，都交由内部组件处理
*           if(IID_YYY == riid){...} else {return m_pInnerUnk->QueryInterface(...);}
*     创建内部组件时
*       a.传入外部组件的IUnknown接口（通过定义 DECLARE_GET_CONTROLLING_UNKNOWN 宏后，
*         调用 GetControllingUnknown 方法获得）;
*       b.创建内部组件，必须请求其IUnknow接口，并保存为外部组件的成员变量(IUnknown * m_pInnerUnk)
*       c.接口映射中加入以下的映射，使得客户端通过外部组件进行QI时，能获得内部组件的接口：
*         COM_INTERFACE_ENTRY_AGGREGATE -- 有选择地集合指定接口
*         COM_INTERFACE_ENTRY_AGGREGATE_BLIND -- 盲目集合其他任何接口(只能有一个内部组件进行盲目集合？)
*         COM_INTERFACE_ENTRY_AUTOAGGREGATE(_BLIND) -- 在请求接口时再创建内部组件(即不用在FinalContruct中创建，释放呢？)
*   2.包容(Containment) -- 类中内嵌另一类的指针作为成员变量，内部组件的接口以间接的方式展示。外部组件是内部组件
*       的客户端程序(COM的机制默认支持)。
*       通过外部组件的方法(可以和内部组件的方法同名，也可以更改成自己的方法)调用内部组件的实现来提供功能。
*     创建内部组件时
*       a.在继承列表中加入内部组件的接口 -- 直接使用内部接口方法的方式，也可以自定义其他接口；
*       b.IDL中加入内部组件的声明；
*       c.接口列表；
*       d.方法实现时，通过内部接口的指针调用其实现。
*
* CComAggObject 从 CComObjectRootEx 继承了两次，提供了IUnknown的两个实现
*  1.直接继承 -- IUnknown实现把调用转发到外部控制对象，包括生命周期和实体身份服务
*  2.通过包容用户从CComContainedObject派生的类的实例间接继承 -- IUnknown被外部控制对象用于私有用途，
*    用来真正维护内部的生命周期和接口查询
*
*
* 问题：
*   客户端调用使用了聚合功能的组件，应该包含哪个的头文件？内外组件的都需要？
***********************************************************************************************************/

#if 0
利用 CComAggObjecct 和它的两个IUnknown实现，自定义的CMyComClass类可以在不修改源码的情况下既支持独立的服务器也支持聚合激活
CMyComClass::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
    *ppv = NULL;
    if (pUnkOuter)
    {
        CComAggObject<CMyComClass>* pObj = new CComAggObject<CMyComClass>(pUnkOuter);
    }
    else
    {
        CComObject<CMyComClass>* pObj = new CComObject<CMyComClass>;
    }
}
#endif

#endif //COM_REUSE_H