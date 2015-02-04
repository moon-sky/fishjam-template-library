#ifndef COM_AUTOMATION_H
#define COM_AUTOMATION_H
#pragma once

/***************************************************************************************************
* 自动化接口的建议
*   1.不要在一个单独的实现中支持多个双接口
*   2.不要把可重用的、多态的COM接口设计成双接口(即普通接口不要是双接口)
*   3.如果想支持脚本客户，建议定义一个单独的双接口来暴露类的所有功能
*   即：COM对象最好不要继承自多个IDispatch接口(理由：脚本客户仅能获得缺省双接口的方法和属性)
*  
* dispid映射到虚函数表的索引： 3(IUnknown) + 4(IDispatch) + dispid(对应的函数) - 偏移基数(通常是第一个dispid的值)
* 
* 自动化(Automation) -- 通过 IDispatch 接口提供后绑定(Late Binding)，实现动态调用，是唯一能用于脚本调用的机制。
*   允许向外公开整个应用程序的功能(通过类型库 tlb 提供)，其中包括了实现IDispatch接口所需的全部信息（如 DispID、调用参数等）
*   就是实现了 IDispatch 接口的组件，用于脚本等“后绑定”，通过四个函数提供功能，能实现调用自动化组件中所有的函数。
*     GetTypeInfoCount  -- 组件中提供几个类型库(IDispatch 接口的个数)
*     GetTypeInfo   -- 取得想要的类型库(指定 IDispatch 接口对应的 ITypeInfo,  )
*     GetIDsOfNames -- 根据函数名字，取得函数序号(DISPID)，为调用 Invoke 做准备
*     Invoke -- 根据函数序号，执行函数，注意 DISPPARAMS 数组的参数按从右向左的顺序存放。如：Object.Method(arg1, arg2, arg3)，
*       pDispParams参数中：cArgs为3(有3个参数)，rgvarg[0]对应arg3，...，rgvarg[2]对应arg1
*       可以使用命名参数(cNamedArgs表示命名参数的个数)，调用时将不受次序的约束。
*       函数调用过程中若出现异常，信息会保留在pExcepInfo中。可以由Invoke函数直接填充pExcepInfo结构，
*         也可以通过回调函数(pfnDeferredFillIn)由客户控制填充
*     RemoteInvoke -- 远程版函数(有这个吗？)
*   执行效率比较低(根据名字找序号，再通过序号调用)，但使用方便，可以使用 CComDispatchDriver  智能指针调用
*
* IDispatch -- 给 VTable 格式的接口添加了名为DispInterface(派发接口)的间接层，通过查询(GetIDsOfNames)
*   获得函数名对应的分发ID(DispID)，再通过派发表调用(Invoke)。（一个组件只能有一个默认的IDispatch接口，
*   如果需要实现多个，需要使用 COM_INTERFACE_ENTRY2(IDispatch,IMyMath) 宏的方式说明由 IMyMath 提供默认的 IDispatch 接口）
* ITypeInfo -- 简单来说是一个代表TypeLibrary中一个类型的COM对象（如接口），可以获取到各种信息，
*   Invoke是把DispID使用TypeLibrary中的接口定义，换算成虚函数的Index，然后通过虚函数表(Vtable)来直接调用接口的函数
* 
* 通过给自动化对象添加 LPDispatch 类型的属性（具有自动化类型的属性），就可以通过属性获取其他的自动化对象。
*   可以连续调用，如： obj.Document.Write <== Document 是obj的一个 LPDispatch 属性，指向类似 IDocumnet 的接口
*   IDL中IAAA的声明：[propget, id(1), helpstring("property BBB")] HRESULT BBB([out, retval] LPDISPATCH *pVal);
*   STDMETHODIMP CAAA::get_BBB(LPDISPATCH *pVal) //将IBBB作为IAAA接口的 LPDispatch 类型属性输出
*   {
*      QueryInterface(IID_IBBB,(LPVOID *)pVal);  //IBBB 是第二个IDispatch接口(其中定义类似Add一类的方法)
*      return S_OK;
*    }
*
* 自动化兼容的数据类型（不需要特殊的 Proxy/Stub，通过标准的通用调度器(OleAut32.dll)进行Marshaling）
*   BSTR、CURRENCY、DATE、IUnknown*、IDispatch*、SafeArray、VARIANT、VARIANT_BOOL、CY(货币类型)
*   其他标准数据类型(如 short、long、float、double 等) -- 能使用的类型就是 VARIANT 结构体支持的那些？
*
* VARIANT <== 可以表示（存储）任意类型的数据。用域(vt)表示数据类型，真正的数据则存贮在 union 空间中。
*   CComVariant(ATL中)、COleVariant(MFC中)、_variant_t(CRT?)
*   VariantInit、VariantClear、VariantCopy、VariantChangeType(Ex)-数据类型转换、
*   专用转换函数：Var<type>From<type>( 如 VarR4FromI2 )
* 
* SafeArray(SAFEARRAY) <== CComSafeArray，安全数组，安全数组的边界不一定从0或1开始
*   可以有多维，通过引用计数进行内存管理，不能直接使用其数据成员，应该使用API
*   SafeArrayCreate/SafeArrayDestroy/SafeArrayGetUBound/SafeArrayGetLBound
*   
* 分发ID(DISPID) -- 自动化接口通过分发ID管理方法和属性，0和负数有特殊含义
*   保留的分发ID及其含义
*     DISPID_VALUE	        0	接口的缺省成员，如果在客户程序中不指定属性或方法，则用此缺省成员。
*     DISPID_UNKNOWN	    -1	在GetIDsOfNames函数中返回此值表明相应的成员或参数名没有找到分发ID。
*     DISPID_PROPERTYPUT	-3	在属性设置函数中指示一个接收值的参数。
*     DISPID_NEWENUM	    -4	集合对象的_NewEnum方法。
*     DISPID_EVALUATE	    -5	对象的Evaluate方法，在控制器的脚本语言中可以用方括号[]来表示。
*     DISPID_CONSTRUCTOR	-6	表示具有与构造函数相同功能的方法。
*     DISPID_DESTRUCTOR 	-7	表示具有与析构函数相同功能的方法。
*
*   
* 相关的API(P238)
*   CreateStdDispatch -- 
*   CreateDispTypeInfo --
*   LoadTypeLib(Ex) -- 通过类型库文件加载类型库
*   LoadRegTypeLib -- 通过注册表加载类型库
***************************************************************************************************/

//P238,
//P248 -- 通过 ITypeInfo 获得信息
//OLE文档接口的意义 P317
//属性页接口的说明 P324

#endif //COM_AUTOMATION_H