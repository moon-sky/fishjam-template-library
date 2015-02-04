#ifndef COM_PERSIST_H
#define COM_PERSIST_H
#pragma once

/******************************************************************************************************
* IPersistXXXX -- 对象的持续性(持久化)，是一个非常常用，并且功能强大的接口家族。
*   调用者（容器）提供存储介质（如文件、内存、注册表、流等），
*     需要保存的时候，调用者通过 IPersistXXX::Save() 接口函数让组件去自己存储属性信息，而调用者根本不用关心存储格式和存储内容；
*     需要还原的时候，调用者打开存储介质，通过 IPersistXXX::Load() 接口函数让组件自己去读取属性信息并完成初始化的设置。
*   COM对象只要从 IPersistStreamInitImpl、IPersistStorageImpl、IPersistPropertyBagImpl 三个接口实现中任意一个继承，
*     并且增加名为 m_bRequiresSave 的成员变量，再把接口添加到 COM_MAP 中，COM对象就支持持久化 -- 自动对对象属性表(PROP_MAP)中的数据进行持久化
*   微软定义的持续性接口(只要在自己写的组件中根据需要实现其中一个或几个持续性接口即可)：
*     IPersist -- 所有持续性接口的根，只有一个函数 GetClassID 用于返回组件的 CLSID 号(如 GetObjectCLSID )，
*       以便调用者能保存这个号为将来 CoCreateInstance() 启动组件用。
*       +- IPersistStream -- 从流(IStream)中读写组件属性信息
*         +- IMoniker
*       +- IPersistStreamInit(最常用) -- 派生自 IPersistStream，并再增加了一个函数 InitNew() 用来完成一个默认的组件属性初始化。
*       +- IPersistMemory -- 使用内存块
*       +- IPersistStorage -- 使用存储 IStorage(一个 IStorage 中可以有多个 IStream)
*       +- IPersistFile -- 使用文件
*       +- IPersistPropertyBag -- 使用属性包（名称-值 对）的文本方式保存信息，如 <param name="属性名称" value="值"> ，
*         在 IE 浏览器中，HTML 嵌入 ActiveX 控件通常使用这个方法。可以保存COM组件中的属性值。
*         需要在调用者(容器)中实现 IPropertyBag 接口。
*       +- IPersistPropertyBag2 -- 扩展了 IPersistPropertyBag 接口。提供了更丰富一些的属性管理用函数。
*       +- IPersistHistory -- 运行于 IE 上，想在用户浏览 WEB 页面时存储和读取状态的持续性接口。
*     IPersistMoniker -- 用于命名(moniker)存储和读取状态的持续性接口。
*
*******************************************************************************************************/
#endif //COM_PERSIST_H