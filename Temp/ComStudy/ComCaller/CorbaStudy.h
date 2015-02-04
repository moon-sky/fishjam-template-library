#ifndef COM_CORBA_STUDY_H
#define COM_CORBA_STUDY_H
#pragma once

/******************************************************************************************************
* CORBA(Common Object Request Broker Architecture)，由OMG(Object Management Group)提出的规范，其实现为ORB
* 具有跨平台(利用IOR--Interoperable Object reference -- 实现)、跨语言(C++语言、Java语言、Smalltalk)的特性。
* 有多个ORB的实现：如Orbix、Visibroker、ORBit、ORBUS 等
* 客户 -> Stub(Corba库) -> 对象/ORB -> Object Adapter
*
* C++映射开发过程：
*   编写IDL(支持多继承、支持结构化数据类型) -> IDL生成C++ -> 
*     利用生成的stub类编写客户
*     利用生成的impl类编写服务器对象
*   
******************************************************************************************************/

#endif //COM_CORBA_STUDY_H