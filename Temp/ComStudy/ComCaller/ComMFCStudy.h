#ifndef COM_MFC_STUDY_H
#define COM_MFC_STUDY_H

/*****************************************************************************************************
* COM的初始化: AfxOleInit / CoInitialize(EX)
*
* 用MFC实现LPDispatch自动化接口属性
*   1.产生一个支持自动化(Automation)的程序（例子程序中，使用的是DLL） 
*   2.添加一个自动化类 CAAA ，派生于CCmdTarget。选择Createable by type ID。注意，只有选择这项，才能被外部程序(VB)CreateObject或(VC)CreateDispatch等方法建立对象 
*   3.添加另一个自动化类CBBB，派生于CCmdTarget。选择Automation 
*   4.在CAAA中，添加的成员对象，CBBB m_bbb 
*   5.在IAAA中，添加名称为BBB，类型为LPDispatch的属性，使用Get/Set类型 
*   6.完成BBB属性的Get/Set代码 
*     LPDISPATCH CAAA::GetBBB()
*     {
*       return m_bbb.GetIDispatch(TRUE);
*     }
*     void CAAA::SetBBB(LPDISPATCH newValue)
*     {
*       SetNotSupported();
*     }
*   7.在IBBB中，添加方法，如 Add
*   8.修改CBBB的析构函数，从protected移动到public中 
*
* 使用自动化组件：
*   方法1：Add Class => From a type Library(如 C:\Program Files\Microsoft Office\Office\MSWord.olb )
*   方法2：#import 
*
* 连接点
*   MFC通过CCmdTarget类实现了IConnectionPointContainer和IEnumConnectionPoints接口
*      通过CConnectionPoint类实现了IConnectionPoint接口
*   需要调用 EnableConnections 函数 ， METHOD_PROLOGUE ？
*
* 使用的映射宏
*   1.BEGIN_INTERFACE_MAP -> INTERFACE_PART -> END_INTERFACE_MAP
*     接口映射
*   2.BEGIN_INTERFACE_PART -> STDMETHOD -> END_INTERFACE_PART => DECLARE_INTERFACE_MAP
*   3.BEGIN_CONNECTION_PART -> CONNECTION_IID -> END_CONNECTION_PART => DECLARE_CONNECTION_MAP
*   4.BEGIN_CONNECTION_MAP -> CONNECTION_PART -> END_CONNECTION_MAP
*     连接点映射
*   4.IMPLEMENT_OLECREATE -> DECLARE_OLECREATE
*
* 常用方法、类
*   COleDispatchDriver -- MFC 使用该类帮助处理 IDispatch 接口
*   CWnd 类中可以直接调用 CreateControl(CLSID_WebBrowser, NULL, WS_VISIBLE|WS_CHILD, xxxx) 
*     来创建ActiveX对象，然后通过 GetControlUnknown 获取对应接口
* 
******************************************************************************************************/

#endif //COM_MFC_STUDY_H