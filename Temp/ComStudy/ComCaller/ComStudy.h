#ifndef COM_STUDY_H
#define COM_STUDY_H
#pragma once

/******************************************************************************************************
* 在组件对象(如CMyMath)的 Class View 视图中，通过右键菜单能自动加入 需要实现的接口和连接点
* 
* COM 常见错误
*   1.error LNK2001: unresolved external symbol _CLSID_DirectDraw
*     有两个不同版本的 DEFINE_GUID 宏，一个只是声明外部引用的符号名称，另一个进行实际的符号定义
*     如果使用预编译头的话：#include <InitGuid.h> 来定义 DEFINE_GUID 宏
*     如果不使用预编译头的话：在包含 <objbase.h> 前定义 DEFINE_GUID 宏
*     但需要确保只Initguid.h只包含一次，否则可能出现"redefinition; multiple initialization"的错误
*   2.ATL本身对多个IDispatch实现支持不太好，无法很容易的将多个IDispatch实现合并成一个，
*     如继承自多个IDispatchImpl时必须使用 COM_INTERFACE_ENTRY2(IDispatch, XXXX), 
*     是否有其他错误或需要注意的地方?
*
* ATL中的宏(TODO)
*   DECLSPEC_UUID -- class DECLSPEC_UUID("74740E11-DDD5-4B62-A739-32A56DD60933") FtlLogAddinLib;
*   DECLARE_LIBID(LIBID_XXXX) -- CXXXModule 中定义整个COM组件的 ATL::CAtlModule::m_libid 变量
*   DECLARE_REGISTRY_APPID_RESOURCEID -- CXXXModule 中
*   DECLARE_PROTECT_FINAL_CONSTRUCT -- 有什么用? TODO:用于在 FinalConstruct 中聚合对象的引用计数? 一般是必加的?
*   DECLARE_CLASSFACTORY_SINGLETON(COM类) -- 声明COM服务为单实例，所有的客户端会连接到同一个实例
*   OBJECT_ENTRY_AUTO(CLSID_XXX, 实现类) -- 在类定义体外部声明，可自动实现相关的类工厂等
*     旧的映射方式为 BEGIN_OBJECT_MAP =>OBJECT_ENTRY=> END_OBJECT_MAP
*
* ATL的基类(没有看到标准文档，自己分析出来的，是否正确?)
*   _ATL_MODULE
*     CAtlModule
*       CAtlModuleT
*         CAtlDllModuleT -- DLL 类型的COM组件基类，提供了DllMain，实现了 DllCanUnloadNow、DllRegisterServer 等函数
*         CAtlExeModuleT -- EXE 类型的COM组件基类，提供了WinMain，
******************************************************************************************************/


/******************************************************************************************************
* COM(Component Object Model) -- 组件对象模型
*   COM服务器(TypeLib=DLL/Exe) 1 > n COM组件/对象(coclass,CLSID_XXX) 1 > n 接口(IID_YYY)
*   COM接口实际上就是一个指向 VTable 结构的指针
* COM++ -- 更容易创建分布式组件软件
* DCOM -- 分布式COM，需要DCOM服务器？
* ProgID -- Programmatic identifier，和版本无关的程序标识符，版本相关的ProgID由版本无关的ProgID加后缀(如 ".1" 组成)
*
* COM 组件的注册：
*   Exe -- RegServer、UnregServer、Automation
*   DLL -- Regsvr32.exe [/u] XXX.dll
* 文件实现中包含<initguid.h>头文件，就使得 DEFINE_GUID 变为定义和初始化GUID常量
* 
* 类工厂(IClassFacory)--以语言无关的方式来实例化一个组件，其唯一功能就是简化其他COM对象的创建方法。
*   CoGetClassObject->IClassFactory::CreateInstance
* 
* COM的规则
*   1.必须实现Iunknown -- 接口必须直接或间接地从IUnknown继承
*   2.COM对象同一性 -- 比较两个COM指针是否为同一个对象，必须 QI 出 IUnknown 的指针进行比较。
*     使用 AreComObjectsEqual 工具函数比较
*   3.对象完整性 -- 对于可处理的接口设置，必须有反身性(QI(A)->A)，对称性(QI(QI(A)->B)->A)和过渡性。
*   4.在改变一个已有接口时必须实现一个新的接口(客户在编译时已同原有接口的VTable绑定在一起了)
* 
* 接口定义的考虑
*   1.接口函数的功能必须简单、明确
*   2.要充分考虑Marshalling对性能的影响，在交互次数、数据传送量等做出抉择
*   3.要充分考虑到内存的管理问题
*   4.要充分考虑COM的应用环境，在IUnknown和IDispatch之间做出选择
*
* 引用计数管理的原则
*   调用AddRef规则
*     1.当把一个非空指针写到局部变量中时。
*     2.当被调用方把一个非空接口指针些到方法或函数的[out]或[in,out]参数中时。
*     3.当被调用方返回一个非空接口指针作为函数的实际结果时。
*     4.当把一个非空接口指针作为对象的一个数据成员时。
*   调用Release的规则
*     1.在改写一个非空局部变量或者数据成员前。
*     2.在离开非空局部变量的作用域之前。
*     3.在被调用方要改写方法或函数的[in,out]参数时，并且参数的初始值非空。
*     4.在改写一个对象的非空数据成员前。
*     5.在离开一个对象的析构函数前，并且此时还有一个非空的接口指针作为数据成员。
*   不调用AddRef和Release的规则
*     1.当调用方把一个非空接口指针通过[in]参数传给一个函数或方法时。
*     2.同一个对象的两个接口，接口一的作用域包含接口二的作用域时，对于第二个接口不须调用。
*
* COM 组件是运行在分布式环境中的 -- INPROC_SERVER、LOCAL_SERVER、REMOTE_SERVER
* 
* 由于牵扯到内存分配和释放等问题，COM 需要明确标注参数方向（如 [in,out]等） -- 需要使用COM的方式(CoTaskMemAlloc/CoTaskMemFree)
*   [in] -- 表示客户机程序(调用者)负责分配合释放这些参数所需要的内存；
*   [out] -- 表示服务器(被调用者)负责分配这些参数所需的内存，客户机程序(调用者)负责释放
*   [in,out] -- 表示客户机程序(调用者)分配并释放内存，但如果需要的话，组件可以选择重新分配内存。
*   [out,retval]表示参数方向是输出，同时可以作为函数运算结果的返回值。[retval]只能有一个，并且要和[out]组合后在最后一个位置。
*     VBS，VC(Import)等语言的包装下作为函数的返回值，如果发生了错误，通过异常来进行处理
*
* Visual Studio的工程向导中（ATL）
*   Allow merging of proxy/stub code -- 把“代理/存根”代码合并到组件程序中(节约并少注册一个DLL)，否则需要单独编译，单独注册代理存根程序。
*     选中后VC7中不会生成单独的 XXXCP 工程。为了真正的合并代理/存根代码，需要定义 _MERGE_PROXYSTUB 宏，并编译 dlldata.c 等文件
*   Support MTS(Microsoft Transaction Server) -- 支持事务服务器，也就是支持 COM+ 功能。编译时会自动调用 mtsrereg.exe ?
*   Threading Model -- 组件支持的线程模型(决定进程内服务器自注册设置中的ThreadingModel值)
*     Single    -- CComSingleThreadModel,没有 ThreadingModel 属性
*     Apartment -- CComSingleThreadModel,ThreadingModel = s 'Apartment'
*     Both      -- CComMultiThreadModel ,ThreadingModel = s 'Both' 
*     Free      -- CComMultiThreadModel ,ThreadingModel = s 'Free'
*     Neutral   -- CComMultiThreadModel ,ThreadingModel = s 'Neutral'。中性，只能用于Win2K及以后。 
*   Interface   -- 接口类型
*     Dual      -- 双接口，在一个接口中，同时支持自定义接口和 IDispatch 接口，即自定义接口 从 IDispatch 继承，
*                  客户端调用时能任意选择使用 前绑定 或 后绑定 -- 既满足脚本语言的使用方便性，又满足编译型语言的使用高效性
*                  并在属性声明中有 "dual",接口中有 id 序号,CMyMath 从 IDispatchImpl<IMyMath...> 继承，
*                  BEGIN_COM_MAP 映射中有 COM_INTERFACE_ENTRY(IDispatch)。
*       如果不是明确非要支持脚本的调用，则最好不要使用双接口：
*         1.如果所有函数都放在一个双接口中，那么层次、结构、分类不清，
*         2.如果使用多个双接口，则会产生其它问题
*         3.双接口、IDispatch接口只支持自动化兼容的参数类型，使用受到限制，某些情况下很不方便 -- 或“必须使用VARIVANT”？
*         4.双接口属于用户接口，一旦发布就不能再改变。
*     Custom    -- 自定义接口, IMyMath 接口 从 IUnknown 继承，CMyMath 从 IMyMath 继承
*   Aggregation -- 对象是否能够作为受控的内部对象参与聚合，但不影响类的对象是否能作为外部控制对象使用聚合
*   Free Threaded Marshaler(FTM) -- 创建一个自由线程列集器对象，能有效地在同一个进程的不同线程之间列集接口指针(但其描述据说太过夸大)
* 
* ISupportErrorInfo -- 继承列表中增加 ISupportErrorInfo，并增加 CXXX::InterfaceSupportsErrorInfo 方法的实现
* Connection Points -- 继承列表中增加 IConnectionPointContainerImpl<XXX>，增加 CProxy_IXXXEvents 类，
*                      IDL中增加 dispinterface _IXXXEvents接口声明 和 [default, source] dispinterface _IXXXEvents;
* IObjectWithSite -- IE对象支持
* 
* Visual Studio的工程向导中（MFC） -- 支持自动化
*   1.创建 MFC DLL，需要支持 Automation;
*   2.New Class -> 基类要选择 CCmdTarget -> Automation 中选择 "Createable by type ID"(创建可以实例化的COM组件对象)
* 
* 已经发布的接口不能再更改（保证软件升级的“鲁棒”性），只能提供新的接口，并在接口中包含旧接口
*   (只需要包含声明，实现会自动使用旧的)，如 IMath(旧用户) -> IMath2(新用户)
* 如果要在一个组件中添加第二个接口，必须手工加入 IDL 的声明和其他信息，不能再使用向导（生成的是一个DLL中的第二个组件？）
*   1.IDL 中的接口声明：interface IMyMath2 ...
*   2.IDL 中 library -> coclass 中的接口列表
*   3.MyMath.h 头文件中的继承列表
*   4.BEGIN_COM_MAP 处的接口映射入口表 -- 才能被 QueryInterface 找到
*   
* 若增加多个接口时，应该是“Custom”接口（即不从IDispatch继承的Dual接口），否则可能会造成两个 IDispatch 的继承混乱？
* 
* 编译
*   1.选择“最小依赖”（静态链接到ATL），目标文件尺寸会稍大，但运行时不需要 ATL.DLL 文件
*   2.如果在 ATL 组件程序中调用了 CRT 的运行时刻库函数，比如开平方 sqrt() ，那么编译的时候可能会报错
*     “error LNK2001: unresolved external symbol _main” -- 需要删除预定义宏“_ATL_MIN_CRT”（VC6）或
*      选择“在 ATL 中最小使用 CRT”（VC7）
*   3.IDL(接口定义描述语言) -- 组件的接口描述，在任何语言环境中都能够认识，IDL 经过编译生成二进制的等价类型库
*     文件 TLB 提供给其它弱类型语言(脚本、VB等)来使用
*   注意：
*     a.编译后，类型库以 TLB 文件形式单独存在，同时也保存在目标文件的资源中。因此，在 #import 引入类型库的时候，
*       既可以指定 TLB 文件，也可以指定目标文件；
*     b.IDL 编译后，提供了 C 语言形式的接口文件（XXX.h） 和 CLSID 的定义文件( XXX_i.c )，需要加入客户端工程；
*     c.IDL 编译后生成代理/存根源程序，有：dlldata.c、xxx_p.c、xxxps.def、xxxps.mak，可以用 NMAKE.EXE 
*       再次编译来产生真正的代理/存根DLL目标文件
*    
* 调用组件程序的方法（前两种效率最高）：
*   1.#include IDL 编译后生成的 XXX.h 和 XXX_i.c 文件，直接使用接口(接口变化后将造成问题) -- 前绑定
*   2.#import，(使用LoadTypeLib API)导入类型库(tbl)或可执行程序(dll 和 exe)，由VC产生包装类(tli 和 tih文件)，
*     控制属性(必须在一行内定义完，必须通过 "\" 进行换行，多个控制属性之间用空格或逗号分开):
*       exclude("OLECMDID", "OLECMDF") -- 排除指定的部分? 如 #import <shdocvw.dll> 时
*       named_guids -- 定义IID和CLSID的名字，否则要用__uuidof(...)来引用
*       no_namespace -- 不生成名称空间(默认是idl文件中library定义的名字)
*       no_implementation --
*       no_smart_pointers -- 
*       raw_interfaces_only -- 仅使用原始接口(返回HRESULT，且看不到XXXEvents的方法)，缺省时import会自动生成符合automation的接口(通过异常指示错误)，
*         如 Foo([out, retval] long * pVal) 函数
*           缺省时生成智能指针 IXxxPtr(以前的方法名字为 raw_XXX)，调用方式为 long val = obj->Foo();
*             需要用 try...catch(const _com_error &e) 处理错误,其中封装了hr和所有关联的IErrorInfo对象。
*           使用该属性后，调用方式为 hr = obj->Foo(&val);
*         如果不带这个参数，则从生成的 tlh/tli 文件中能看到 Invoke 的方法名和dispid
*       raw_native_types
*       rename("旧名","新名") -- 对指定对象进行重命名，常用于解决 "error C2872 ambiguous symbol" 的名称冲突错误
*   3.使用 IDispatch 接口直接调用（或用 CComDispatchDriver 等智能指针调用）
*   3.加载类型库包装类(组件提供了 IDispatch 接口) -- 添加类 -> MFC -> 类型库中的MFC类
*   4.加载ActiveX包装类
*   5.在脚本等解释语言中，使用自动化接口(IDispatch) -- COM组件必须实现了IDispatch接口
* 
* COM 的调用有三种绑定方式（效率由高到低）
*   前绑定 -- Custom接口(编译时通过VTable确定函数地址)；
*   ID绑定 -- 通过类型库(import/class->from TypeLib)，在编译时确定DispID，并通过DispID进行Invoke，
*     如可能的话，直接通过VTable。
*   后绑定 -- IDispatch，运行时通过 GetIDsOfNames 获得命令的DispID，再通过 ID 进行 Invoke
*
* 智能指针(模板包装类) -- T* operator->() const throw(){ return m_p;}  //使用类似语法重载"->"运算符返回接口的指针
*   CComPtr -- 最基本的智能指针
*     Release -- 采用将指针成员变量赋值到一个临时变量中，并把成员变量重置为NULL后，才使用临时变量释放接口指针，
*       这个方法避免了只能指针保存的接口可能被释放两次的问题（两个智能指针互相有引用，Release中释放时造成对方析构，
*       反向调用本指针的析构，析构中会检测成员变量是否为NULL，如非NULL则可能重复释放）
*     operator->() -- 返回 _NoAddRefReleaseOnCComPtr 模板类，禁止(编译错误)直接使用指针成员变量的 AddRef/Release
*     SetSite -- 把内部指针引用的对象与一个site对象(参数punkParent)联系起来，只能指针必须指向一个实现了 IObjectWithSite 接口的对象
*     Advise(Sink) -- 把一个连接点接收器对象与智能接口指针引用的对象(事件源对象)联系起来
*   CComQIPtr -- CComPtr 的扩展，但能自动调用QueryInterface函数,唯一的缺点就是不能定义 IUnknown * 指针
*     CComQIPtr < IMyMath, &IID_IMyMath > spMyMath;
*   CComDispatchDriver -- 是IDispatch接口的智能指针(typedef CComQIPtr<IDispatch>)，可获取和设置对象的属性
*     CComDispatchDriver pDisp = XXX; pDisp->GetProperty(DISPID_COUNT, &var);
*     GetIDOfName -- 可以根据属性名字的字符串获得属性的DISPID
*     GetProperty/PutProperty -- 获取或设置属性的值
*     GetPropertyByName/PutPropertyByName -- 根据属性名字来获取或设置属性值
*     InvokeN -- 提供了多个重载的方法，方便对Invoke的调用
*   _com_ptr_t -- 使用 _COM_SMARTPTR_TYPEDEF 宏定义的智能指针(IXxxPtr)，使用异常表明错误(_com_error)
*
* 生存期管理
*   服务器生存期：锁计数和组件计数
*   组件生存期：接口引用计数
* 内存管理
*   IMalloc
*   CoGetMalloc -- MEMCTX_TASK(作业分配器)、MEMCTX_SHARED(共享分配器 )
* 
* 组件类别(CATID) -- 表明组件的功能类型(通常可以用于制作插件，或枚举系统能支持的组件-如插入VC中插入ActiveX控件，
*   但在自定义程序时，更好的方法是放在Plugin目录下，并且实现特定接口的DLL？)，可以用 OLEView 查看。
* 组件分类管理器(Component Categories Manager -- CCM)是一个简单的COM组件
*  （CLSID_StdComponentCategoriesMgr），实现了组件管理的接口：
*   ICatRegister
*     RegisterCategories <== 创建并注册自定义的类别，通常在DllRegisterServer函数中进行
*     UnRegisterCategories <== 注销自定义的类别，在 DllUnRegisterServer 函数中进行
*     RegisterClassImplCategories <== 注册 CLSID 为指定的类别
*     RegisterClassReqCategories  <== ?
*   ICatInformation 
*     EnumCategories
*   注册自定义类别
*     CATEGORYINFO catinfo;
*     catinfo.catid = CATID_MyComPlugin;
*     catinfo.lcid = 0x0409;  // locale=english
*     USES_CONVERSION;        // uses A2W
*     wcscpy(catinfo.szDescription, A2W("My Com Plugin."));
*     pcr->RegisterCategories(1, &catinfo);
*
* 窗体消息支持
*   如果COM组件要响应消息(如CVzCsManager通过消息处理)，可以从 CWindowImpl 继承，声明DECLARE_WND_CLASS宏(注册窗体类？)，
*   使用 BEGIN_MSG_MAP 进行消息映射。在 FinalConstruct 中调用Create创建窗口，在FinalRelease中调用DestroyWindow销毁窗口。
*    
*
* ATL的内部工作方式以及它所使用的技术
*   1.没有任何成员的类对象大小为 1(占位)；有成员后，大小为成员(数据、函数)的大小之和。
*     普通函数不会增加大小，但有虚函数时，增加一个虚函数表指针VTPR(大小为4，在内存结构中的第一个位置)
*     虚函数表中保存类中所有虚函数的地址，最后一个入口为NULL。
*     多重继承时，派生类会拥有所有基类的虚函数表指针，即sizeof(obj) = 4 * 有虚函数的基类个数 ？？
*     ATL 使用宏 offsetofclass(base, derived) 返回在派生类对象模型中基类虚函数表指针的偏移量 -- 调用指定基类中的虚函数
*
* ATL 建立复合控件(向导中：Controls->Composite Control) -- 一个控件中有多个子控件(如可以同时包含列表控件和进度条控件)
*   然后可以在IDE的"ToolBox->General"中选择"Add/Remove Items" 来插入编译后的COM组件，并放在Dialog中使用。
*   处理控件事件：右键 -> Add Event Handler...
*
* 映射：
*   1.COM组件对象映射(建立COM对象和C++实现类的映射，是旧的方式，新的方式已经采用OBJECT_ENTRY_AUTO)
*       BEGIN_OBJECT_MAP(ObjectMap) -> OBJECT_ENTRY -> END_OBJECT_MAP
*       OBJECT_ENTRY(CLSID_AtlComp, CAtlComp) <== 将 CLSID_AtlComp 映射到 CAtlCom 类，用于类工厂，
*       在 CoCreateInstace 时知道应该new哪个
*       实现：生成一个静态全局 _ATL_OBJMAP_ENTRY 型数组 ObjectMap[]，并在 _Module.Init(ObjectMap...) 中初始化
*   2.接口映射：BEGIN_COM_MAP -> COM_INTERFACE_ENTRY -> END_COM_MAP
*       http://www.cnblogs.com/huqingyu/articles/46505.aspx
*       COM_INTERFACE_ENTRY(IMyMath)  <== 可以 QueryInterface 到 IMyMath 接口
*       COM_INTERFACE_ENTRY2(查询接口, 指定的继承分支)  -- 查询菱形继承的父接口时，通过指定的继承分支返回，
*         COM继承树中如果有多个分支实现了同一个接口(如IDispatch)，需要知道查询这个接口时把哪个分支返回给它
*         宏中会设置 pEntries->dw 的值为 (DWORD)((x*)(x2*)((_ComMapClass*)8))-8，然后通过 
*         IUnknown* pUnk = (IUnknown*)((int)pThis+pEntries->dw) 找到对应分支的虚函数表指针
*           <== 菱形继承时，多个虚函数表指针和内存块顶端之间的偏移量
*       COM_INTERFACE_ENTRY_IID(想要的IID, 实际会返回的接口类型) -- 一般用于有父子关系的接口? 还是用于聚合?
*       COM_INTERFACE_ENTRY_TEAR_OFF(IID_IMyTearOff, CMyInnerTearOff) -- 把很少用到的IID_IMyTearOff接口
*         放在一个单独的组件(CMyInnerTearOff)中实现，仅当查询到这个接口时，才创建这个组件，
*         并且当它的引用计数减为0时就会被释放掉。注意：内部的 CMyInnerTearOff 需要从 CComTearOffObjectBase 继承
*       COM_INTERFACE_ENTRY_CACHED_TEAR_OFF -- 当查询分割对象中其他接口时，不会再新建新的对象
*       COM_INTERFACE_ENTRY_AGGREGATE -- 用于聚集对象
*       COM_INTERFACE_ENTRY_NOINTERFACE -- 查询时会返回E_NOINTERFACE(不想让别人查到这个接口)
*   3.属性映射(也支持持久化)：BEGIN_PROP_MAP -> PROP_ENTRY/PROP_DATA_ENTRY -> END_PROP_MAP
*       PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4) <== 
*       PROP_ENTRY("ForeColor", DISPID_FORECOLOR, CLSID_StockColorPage)     <== 映射系统的ForeColor属性
*       PROP_PAGE(CLSID_XXXX) <== 指定一个ATL固有的属性页，容器构造控件的属性单时可以包含上述的属性页
*   4.连接点映射：BEGIN_CONNECTION_POINT_MAP -> CONNECTION_POINT_ENTRY -> END_CONNECTION_POINT_MAP
*       CONNECTION_POINT_ENTRY(__uuidof(_IMyTimerEvents))  <== 当前连接点容器中有 GUID 为 __uuidof(_IMyTimerEvents) 的连接点
*   5.消息映射(WTL使用 _EX 版本可以进行消息crack)：BEGIN_MSG_MAP -> CHAIN_MSG_MAP/MESSAGE_HANDLER -> END_MSG_MAP
*       MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)  <== 用 OnInitDialog 函数处理 WM_INITDIALOG 消息
*       CHAIN_MSG_MAP/CHAIN_MSG_MAP_MEMBER <== 未处理的消息可以交给父类或成员变量等进行处理；
*       DEFAULT_REFLECTION_HANDLER <== 进行反射?
*   6.事件处理映射，提供DISPID和函数实现的对应关系，用于连接点客户端实现，将dispid对应到处理函数：
*       BEGIN_SINK_MAP ( MyClassName )
*         SINK_ENTRY_INFO(标识ID, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete, &DocumentComplete2_Info)
*         SINK_ENTRY_EX ( 标识ID, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete )
*         SINK_ENTRY(标识ID, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
*       END_SINK_MAP()
*       说明：
*         a.SINK_ENTRY_INFO -- 需要根据对应函数类型_ATL_FUNC_INFO类型的变量，如 
*           __declspec(selectany) _ATL_FUNC_INFO CBrowserView::DocumentComplete2_Info = { CC_STDCALL, VT_EMPTY, 2, { VT_DISPATCH, VT_BYREF | VT_VARIANT } };
*         b.如果通过 GetFuncInfoFromId 提供调用方式(默认通过 AtlGetFuncInfoFromId 实现?)，示例如下：
*           virtual HRESULT GetFuncInfoFromId ( const IID&, DISPID, LCID, _ATL_FUNC_INFO& ) { check(iid); switch(dispid){info.xxx=???;}}
*         b.OnDocumentComplete 函数的签名（签名可以通过MSDN或 OleView 查看对应的 tlb 等文件）
*           void __stdcall OnDocumentComplete ( IDispatch* pDispatch, VARIANT* pvUrl); 

*   7.实现组件类别注册(如IE工具栏的类型)：BEGIN_CATEGORY_MAP -> IMPLEMENTED_CATEGORY -> END_CATEGORY_MAP
*       本质是在组件的注册信息中增加 "Implemented Categories" 子项和对应的 CLSID
*       IMPLEMENTED_CATEGORY(CATID_SafeForScripting)  <== 使用脚本时，添加脚本安全组件类型，可以去掉“不安全交互”的提示
*       IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
*       IMPLEMENTED_CATEGORY(CATID_InfoBand) <== IE中的垂直浏览栏?
*       IMPLEMENTED_CATEGORY(CATID_CommBand) <== IE中的水平浏览栏
*       IMPLEMENTED_CATEGORY(CATID_DeskBand) <== 桌面工具栏
*       IMPLEMENTED_CATEGORY(CATID_SimpleFrameControl) <== 
*       IMPLEMENTED_CATEGORY(CATID_PropertyNotifyControl) <== 
*       IMPLEMENTED_CATEGORY(CATID_VBDataBound) <== 
*       IMPLEMENTED_CATEGORY(CATID_WindowlessObject) <== 
*       IMPLEMENTED_CATEGORY(CATID_PersistsToMoniker) <== 
*       IMPLEMENTED_CATEGORY(CATID_PersistsToStreamInit) <== 
*       IMPLEMENTED_CATEGORY(CATID_PersisitsToStream) <== 
*       IMPLEMENTED_CATEGORY(CATID_PersistsToStorage) <== 
*       IMPLEMENTED_CATEGORY(CATID_PersistsToMemory) <== 
*       IMPLEMENTED_CATEGORY(CATID_PersistsToFile) <== 
*       IMPLEMENTED_CATEGORY(CATID_PersistsToPropertyBag) <== 
*       IMPLEMENTED_CATEGORY(CATID_RequiresDataPathHost) <== 
*       IMPLEMENTED_CATEGORY(CATID_AppContainerCompatible) <== IE工具条中支持EPM, //{59FB2056-D625-48D0-A944-1A85B5AB2640}
*   8.服务注册？：BEGIN_SERVICE_MAP -> SERVICE_ENTRY -> END_SERVICE_MAP
*
*   A.MFC中 BEGIN_EVENTSINK_MAP -> ON_EVENT -> END_EVENTSINK_MAP
*       ON_EVENT(CTestAtlDlg, IDC_ATLCOMP1,     1    ,  OnAtlEvent, VTS_I4  VTS_BSTR)，
*       参数分别是：处理的类,  控件ID,      Invoke 的 ID, 处理函数，参数类型列表，上面代表 OnAtlEvent(LONG state,BSTR info)
*
* 名字服务 -- ?
*
* 常用接口及辅助实现(ATL通过大量的 IxxxImpl 接口实现模板类 提供接口的基本实现和扩展)：
*   IConnectionPointContainerImpl -- 连接点容器的基本实现，支持查找和枚举连接点
*   IDispatchImpl< disp子类接口, &IID_对应的接口, &LIBID_包含接口的组件对象, 主版本号, 次版本号 >
*     -- 提供自动化/双向接口的实现支持(IDispatch接口实现)
*   IDispEventSimpleImpl -- 事件处理的基本实现，但需要自己手工(重载 GetFuncInfoFromId 或 使用 SINK_ENTRY_INFO)提供函数的调用原型(SINK_INFO)
*     <== IDispEventImpl<标识ID, 子类名, &__uuidof(需要处理的事件接口), &__uuidof(XXX::library), 主版本号, 次版本号>
*           ATL 中连接点客户端的实现(在 Invoke中处理 DISPID_XXX )，可以在定义类时指定对应的类型库(LibId和版本)，
*           自动加载对应的Lib获得函数的调用原型，可以直接使用 SINK_ENTRY_EX/SINK_ENTRY，
*           版本可用 AtlGetObjectSourceInterface 获得(有没有工具?)，然后对应构造
*   
*   IObjectWithSiteImpl -- Site是位于包容器与被包容对象两个概念之间的一个对象,通过它，包容器能对被包容
*       对象的细节进行控制，而被包容对象则能执行自己的功能，IObjectWithSite就是连通二者的Site对象接口。
*   IOleWindow          -- 
*   IPropertyPageImpl   -- 属性页，其中有 m_ppUnk[i] 的接口数组
*     
* API 函数
*   CoRegisterClassObject -- 进程外服务器启动后，注册类工厂(说明该类工厂可使用)
*     CoRegisterClassObject(CLSID_CHello, pCF, CLSCTX_LOCAL_SERVER,REGCLS_MULTIPLEUSE, &dwRegistration);
*   DllGetClassObject   -- 进程内服务器导出类工厂
*   CoCreateInstanceEx  -- 可以在远程机器上创建COM组件，并且可以一次查询多个接口
*
* QueryInterface的调用顺序
*   1.CComObject::QueryInterface
*   2.YourClass::_InternalQueryInterface
*   3.CComObjectRootBase::InternalQueryInterface
*   4.AtlInternalQueryInterface 
*
* COM类
*   CComClassFactory    -- 提供了对基本的类工厂的支持，有单实例扩展 CComClassFactorySingleton 
*   CComCoClass         -- 提供错误支持(通过Error方法发送出错信息，简化了发送出错信息的过程)，并能获取对象的CLSID和描述信息
*                          需要提供 ISupportErrorInfo 支持
*   CComControl         -- ActiveX控件的基类
*   CComCreator(2)      -- 提供CreateInstance方法，通过函数指针创建实例。 每个creator类有一个静态CreateInstance函数。
*   CComModule          -- 封装了组件的宿主(DLL 或 EXE)支持，现在是 CATLComModule，类似 CWinApp
*   CComObject          -- 定义后的COM组件还是纯虚类(IUnknown)，需要将 CMyMath 作为模版 CComObject 的父类，CComObject会从中获得CComObjectRootEx的功能
*                          提供 IUnknown 的接口，有 CComObject(非聚合对象)、CComAggObject(聚合对象)、
*                          CComPolyObject(？？)、CComObjectCached(用于类工厂)等几种不同的类型。
*                          如果需要手工创建 COM 组件，需要使用类似如下的方法：
*                          CComObject<CMyMath> *pMyMathObj;  //如果使用智能指针，引用计数会出现问题，为什么？
*                          CComObject<CMyMath>::CreateInstance(&pMyMathObj);  //得到的是对象的指针pMyMathObj，而不是接口指针
*                          pMyMathObj->AddRef(); //必须调用一次增加引用计数，否则以后会变成 -1，无法删除
*                          使用完毕后，必须通过 pMyMathObj->Release(); 进行释放
*   CComObjectRootBase  -- 定义了FinalConstruct 和 FinalRelease 方法用于初始化和清理
*   CComObjectRootEx    -- 模版基类，提供恰好足够线程安全(just-safe-thread-enough)的对象生命周期管理、对象锁定功能
*                          和IUnknown的部分实现，其他部分通过 BEGIN_COM_MAP 实现
*   ObjectLock          -- 由 CComObjectLockT 进行 typedef 的自动锁: ObjectLock lock(this);
******************************************************************************************************/

#if 0

//例：处理 WORD 中的事件(MSDN Sample中的 ATLEventHandling)
// #import doesn't generate a LIBID, so we have to do it manually
namespace Word
{
	struct __declspec(uuid("00020905-0000-0000-C000-000000000046"))
		/* library */ Library;
};

class CMyWordEventHandle : xxxxx
	, public IDispEventImpl</*nID*/ 1, CNotSoSimple, &__uuidof(Word::ApplicationEvents2), 
	  &__uuidof(Word::Library), /*wMajor*/ 8, /*wMinor*/ 1>

#endif

#endif //COM_STUDY_H