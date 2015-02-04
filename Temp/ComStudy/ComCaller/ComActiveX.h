#ifndef COM_ACTIVEX_H
#define COM_ACTIVEX_H

/***********************************************************************************************************
* MFC的支持： COleControl
*
* ActiveX -- 一种实现了一些标准接口(如IOleObject、IOleControl等)的COM组件
*   分为两种模式(UserMode): 设计时(designtime)、运行时(runtime)
* 
* Ambient -- 容器的环境属性，由容器的IDispatch接口提供,基于ATL的控件可以调用 GetAmbient* 方法访问容器的环境属性。
*   UserMode:TRUE表明运行模式，FALSE表明设计模式
*   DisplayName:容器指定的控件名称
*   LocalID:容器的本地标识符
*   MessageReflect：为TRUE时，秒民容器可以将消息反馈给控件
*   UIDead：告诉控件不接收用户的输入或对用户的输入不做任何响应
*   SupportsMnemonics：容器对在控件中使用记忆技术(mnemonics)提供支持
*
* 实地激活(in-place activation/in-place editing 实地编辑) -- 可以直接在包容器窗口内部进行编辑
*   实现的接口
*     容器对象实现：IOleInPlaceFrame、
*     文档窗口对象实现：IOleInPlaceUIWindow
*     站点对象实现：IOleClientSite、IAdviseSink、IOleInPlaceSite、IOleControlSite、IPropertyNotifySink(问题：这几个接口应该由谁实现？)
*     IOleInPlaceObject、IOleInPlaceActiveObject
*   实现要点：
*     界面处理 -- 菜单合并、工具条、快捷键
*     消息发送、焦点控制 -- 鼠标消息、键盘消息
*     窗口大小和位置调整
*
* 标准分发ID -- 标准属性、标准方法、标准事件；包容器的标准环境属性
* 
* 标准控件属性(stock property，类型、名字、DISPID_XXX 的值都是固定的) -- 提供通用的控件功能，如Font、BackColor 等
*   通常需要在控件的构造函数中进行初始化
* 属性的持续性 -- IPersistStream、IPersistStreamInit、IPersistPropertyBag（提供名字-值对的方式）
*
* 属性页--每一个属性页本身也是一个COM组件，并由容器应用程序单独实例化，系统提供了几个标准的属性页(字体Font、颜色Color、图形Picture)
*   创建属性页的ATL对象(可以利用WTL处理属性页对话框的消息和映射)，并通过 PROP_ENTRY 将属性和属性页的CLSID关联
*     如果是CLSID_NULL，表明没有缺省的属性页，通常需要自己创建属性页，并进行映射
*   系统预定义的属性都可以在 $(VSInstallDir)\Vc7\VCWizards\ClassWiz\ATL\Control\Templates\1033\control.h 文件中查到
*   在属性页处理中：
*     1.OnInitDialog 中获取控件的属性值，用于初始化；
*          if(m_nObjects >0){       //问题：m_nObjects 和 m_ppUnk 是什么？
*            CComQIPtr<IXXX,&IID_IXXX> pXXX(m_ppUnk[0]); 
*            pXXX->get_YYYY(...) 
*          }
*     2.在自定义的 编辑框 等UI控件的 EN_CHANGE 等事件处理函数 OnPropertyChange 中设置 Apply 按钮
*       SetDirty(TRUE);  //设置Dirty标志
*       m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY | PROPPAGESTATUS_VALIDATE);//通知属性浏览器，属性值已改变
*     3.在 Apply 事件处理中
*       CComQIPtr<IXXX,&IID_IXXX> pXXX(m_ppUnk[0]); //获取控件的指针
*       从属性页的UI上获取用户输入的信息后，调用 pXXX->put_YYYY 等方法将属性值传递到控件
*       m_bDirty = FALSE;   //恢复Dirty状态
*       如果发生错误(如值范围)，需要通过 GetErrorInfo(0,&pIErrorInfo) 获取错误信息接口，并获取详细错误进行显示。
*     4.属性变化通知 -- 使用可连接对象机制，其出接口为 IPropertyNotifySink 
*   原理（可能不正确？）：客户通过 OleCreatePropertyFrame 创建属性表单框架，其中包含了多对 IPropertyPageSite <-> IPropertyPage，
*         组件通过 ISpecifyPropertyPages 导出所有的属性页
*
* 事件 -- 使用连接点技术(如连接点提供Click事件，DISPID_CLICK，在WM_LBUTTONDOWN 消息中 Fire_Click)
* 
* ActiveX控件的发布
*   1.客户-服务器协调控制：<OBJECT CLASSID="..." codebase="http://webserver/Polygon.dll" ...>
*       <PARAM NAME="Sides" VALUE=5></OBJECT>
*       IE自动下载到“Downloaded Program Files”目录中并注册到客户机上
*   2.使用CAB进行压缩，提高传输效率并且可包含多个代码文件：<OBJECT ... codebase="http://webserver/Polygon.cab" ... ></OBJECT>
*     CAB文件的制作：cabarc.exe N polygon.cab atl.dll polygon.dll \ polygon.inf
*       支持数字签名
*
*
* CComControl -- ATL通过CComControl提供了许多ActiveX控件功能（如固有属性、属性持续性、窗口功能、消息映射等）
*   如果发生了属性变化（可能会更改外观），可以调用 FireViewChange 通知更新
* CStockPropImpl -- 提供了ATL的固有属性实现
* IOleObjectImpl -- 提供了控件的很多默认实现,如果有需要，只需要重写响应的函数即可（如限制控件大小的 SetExtent）
*
***********************************************************************************************************/

#endif //COM_ACTIVEX_H