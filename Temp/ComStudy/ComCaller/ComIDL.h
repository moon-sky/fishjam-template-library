#ifndef COM_IDL_H
#define COM_IDL_H
/************************************************************************************************************
* 对具有类型库的DLL等文件使用 tlbimp.exe 会产生一个 interop 库，如 quartz.dll 生成 Interop.QuartzTypeLib.dll
*
* IDL(Interface Definition Language) -- 接口定义描述语言，原用来描述 远程过程调用接口(Remote Procedure Call Interfaces)，
*   被MS扩展为 MIDL，能定义 结构、枚举等类型，最后生成 接口声明的 头文件(.h)、GUID定义(_i.c)、tlb、接口的Marshaler定义(_p.c)、
*   接口 Marshaler 的进程内服务代码(DLLDATA.c)。提供了对自定义接口的Marshaling支持，以及通过类型库进行自描述(Self Description)的功能。
* 
* RegTlib.exe -- TLB的注册(#import)
* 
*  关键字
*    appobject -- 
*    coclass(component object class) -- 组件对象类(注册在 HKCR\CLSID)，包含着一个或者多个接口的代码。
*      COM对象在内存中表现为 coclasss 的一个实例
*    cpp_quote -- 指导MIDL编译器将限定了的字符串转换成生成的头文件
*    default -- 指定了默认的组件接口 -- 即指定 IID_IUnknown 时返回的接口？
*    dispinterface -- 定义纯IDispatch的接口，一般用于连接点的事件， 不能和 object、dual 等同时使用，（？只能在 library 内部定义？）
*      idl直接编译生成的.h文件中Events接口看不到定义的方法和属性，通过OleView.exe可以查看(exe/dll/tlb)
*      不带 raw_interfaces_only 的import生成的 tlh 中能看到方法,tli中能看到对应的 dispid
*    dual -- 说明是双接口(即同时通过IDispatch 和 VTBL 导出属性和方法，一般用在从IDispatch继承的接口属性定义时)
*    helpcontext(2481) -- 
*    helpstring("A Class") --
*    import "xxx.idl" -- 用于将其他idl文件中的定义包含到当前文件中，一般在idl文件头使用
*    importlib("xxx.tlb") -- 导入tlb文件，一般是在 library 中使用
*    interface -- 定义接口(注册在 HKCR\Interface)，
*    library -- 定义类型库(注册在 HKCR\TypeLib)，其中包含了block中定义和引用的所有元素定义。
*    lcid(0x0409) -- 语言ID
*    local -- 告诉MIDL编译器,不要生成接口代理和存根
*    nonextensible -- 用于 IDispatch，表示不能再运行时增加成员进行扩展?
*    object --   表示所定义的接口是一个COM接口，关键字object是Microsoft对于IDL的一种扩展。
*      如果没有这个属性信息，所生成的列集代码将与COM函数库（运行时）提供的标准列集代码不兼容。
*    oleautomation -- 使用标准的通用调度器(Universal marshaler)，注意需要使用自动化兼容的数据类型
*    pointer_default(类型)--只在发生跨进程或跨机器通讯时起作用。客户和组件交互有指针的方法时，不能只传指针的值，还要把指针所指向的内容一起传到组件端。
*     设定了没有显示限定的嵌入式指针（embedded pointer）的默认指针属性信息。
*     嵌入式指针（embedded pointer）包括结构体（structrue）、联合体（union）、数组（array）的成员指针.
*      <== 客户调用组件方法的指针参数时，不能只传指针的值，
*      还要把指针所指向的内容一起传到组件端。如果组件修改了指针所指的内容，则还要把指针所指的内容传回客户端。
*      为了减少在进程边界间或网络（不同机器）间的传输量，指针归纳为三类：
*      1.ref(引用指针) -- 这类指针总是指向一个合法的已申请来的内存，不能为NULL值；无论在调用前后，指针都指向同一个地址；
*          组件服务器端返回的数据总能写到指针所指的已存在的内存；指针所指的内存不能同时被其它指针和变量引用着。
*          不能为空，不能重复
*      2.unique(唯一指针) -- 可以是NULL值；它可以在调用前后由NULL变为非NULL，这时系统为你申请内存；它可以在调用前后由
*          非NULL变为NULL，这时客户端必须负责释放原来的内存；指针值在调用前后可以被改变；指针指向的内存不能
*          同时被其它指针和变量引用着；如果指针值非空，则组件方返回的数据写入已经存在的内存。
*          可以为空，不能重复。代理不提供对重复指针探测的支持(若两个指针指向同一个数据，那个数据的两份相同的副本会被送往远程对象)。
*      3.ptr/pointer(完全指针) -- 与unique相同，但是指针指向的内存可以被其它指针和变量引用。
*          可以为空，可以重复，若两个指针指向同一个数据，只有一个副本会被送往远程对象
*      这三种类型一个比一个复杂，对于 ref，COM的 proxy/stub 在 marshaling 时处理最简单，而 pointer 最复杂。   
*    source -- 表明是引出接口(outcoming Interface)，由客户端实现(接收器对象Sink)。默认是引入接口(incoming interface)，由服务器实现
*    string -- 提示列集器,修饰的本参数是一个字符串，是以null终止的
*    typedef -- 如 typedef [unique] IUnknown *LPUNKNOWN;
*    uuid(12345678-1234-1234-1234-123456789ABC) -- 当前元素的GUID值
*    version(1.0) -- 定义版本（在哪里看？）
*  自定义结构
*    enum
*    struct
*    union
*  接口方法说明
*    iid_is -- 只适用于指针，存根需要准确地知道正在调度的接口指针的riid。
*    in -- 告诉MIDL需要将此参数值从客户传递给组件，存根代码不需要送回任何值
*    optional -- 自动化中表明是可选参数？
*    out -- 告诉MIDL参数仅被用来从组件向客户传回有关的数据
*    propget -- 获取数据的属性，属性名必须有和函数名一样
*    retval --
*    size_is -- 显示地告诉列集器，指针实际指向一些字节的块。列集器根据这个块的大小分配内存。
*      常用于动态数组，说明数组的大小；如果是多维的话，可以变化的只能是第一维，其他维的大小必须是固定的。也可以用于输出参数
*      HRESULT GetPrim([out] long * plCount, [out, size_is(,* plCount)] long** ppLData);
*    switch_type -- 
*    switch_is -- 
*
* 常见错误：
*   1.error MIDL2311 : statements outside library block are illegal in mktyplib compatability mode 
*     -- 工程属性中将IDL文件的MIDL/mktyplib203选项去掉
*   
************************************************************************************************************/

#endif //COM_IDL_H