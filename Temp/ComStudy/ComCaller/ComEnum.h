#ifndef COM_ENUM_H
#define COM_ENUM_H
#pragma once

/******************************************************************************************************
* 枚举器(如 IEnumString、IEnumCLSID、IEnumVariant 等)
*   COM 通过枚举器对象来为条目清单的插入和循环提供标准方法(Reset、Next、Skip、Clone)。后扩展为集合(Collection)。
*     Clone -- 创建另一个包含同样状态(如位置)的枚举对象作为当前对象的副本，可用于保存位置。
*   通常客户端调用组件提供的一个特殊方法，返回枚举器接口的指针
*   ATL提供的支持：
*     CComEnum -- 用 CComEnumImpl 实际实现的枚举器对象
*     CComIEnum -- 定一个枚举接口的抽象类
*     CComEnumImpl -- 实现IEnumXXX接口，假设内部数据存储于数组中（T* m_begin）
*     CComEnumOnSTL -- 用标准模版实现枚举器接口
*   创建并返回一个 IEnumString 接口（如接口的 GetEnum([out,retval] IEnumString **ppEnum);方法 )
*     typedef CComObject< CComEnum< IEnumString, &IID_IEnumString,LPOLESTR,Copy<LPOLESTR> > > enumString;
*     enumString* pEnum = NULL;   //或使用 new enumString;
*     enumString::CreateInstance(&pEnum);
*     pEnum->Init(m_vecStrings.begin(), m_vecStrings.end(),0,AtlFlagCopy);//从内部vector变量中初始化枚举器对象的数据
*       //其中 std::vector<LPOLESTR> m_vecStrings;
*       //指定了 AtlFlagCopy 参数，表明会进行赋值，因此，客户端需要释放获取的数据？？
*     pEnum->QueryInterface(IID_IEnumString,(void**)ppUnk); //ppUnk 返回给客户，如果失败，需要 delete pEnum
*
* 集合(Collectioin)，基于集合的对象应该实现一下标准的属性和方法（IDispatch），通常以1为基址
*   Add(IMySubDispatchType* pSubObj) -- 通常
*   Remove
*   Count -- 只读属性，返回集合条目的数量
*   Item -- 只读的默认属性(DISPID_VALUE)，返回集合中的指定条目，通常返回类型是 IDispatch* 的子类
*   NeuEnum -- 只读属性(DISPID_NEWENUM)，返回一个能使客户对集合进行迭代的IEnumXXX接口
*   集合可以使用 For Each XXX in colXXXs 进行枚举
*   注意：1.集合内部的数据不能由客户端直接创建（应有容器方法创建），因此在IDL中不将对应的interface放入coclass部分
*         2.集合组件内部可以通过 vector<CComVariant> 保存数据
******************************************************************************************************/

#endif //COM_ENUM_H