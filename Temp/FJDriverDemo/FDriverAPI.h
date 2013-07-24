#ifndef F_DRIVER_API_H
#define F_DRIVER_API_H

//可以用与 KeDelayExecutionThread 的宏
#define DELAY_ONE_MICROSECOND	(-10)
#define DELAY_ONE_MILLISECOND	(DELAY_ONE_MICROSECOND * 1000)
#define DELAY_ONE_SECOND		(DELAY_ONE_MILLISECOND * 1000)

/******************************************************************************************************************
* DriverEntry -- 驱动程序的入口函数(extern "C")，由内核中的IO管理器负责调用，主要工作是初始化 DriverObject, 指定Dispatch方法
*   IN PDRIVER_OBJECT DriverObject    -- IO管理器传递进来的驱动对象
*   IN PUNICODE_STRING  RegistryPath  -- 驱动程序在注册表中的路径
******************************************************************************************************************/

/******************************************************************************************************************
* 应用程序一般通过 文件方式( 如 WriteFile / DeviceIoControl ) 与驱动交互 -- I/O管理器担当者用户模式代码和设备驱动程序之间的接口。
* 所有对设备的操作(比如端口的读写、键盘访问、磁盘文件的操作等)最终将转化为IRP请求,这些IRP请求会被传送到派遣函数处理？
*
* 驱动程序中的设备名称只能在内核状态可见, 应用程序不能直接根据设备名打开设备,应用程序要识别设备有两种方法：
*   1.通过 符号链接(IoCreateSymbolicLink) 找到设备，如通过 "\??\C:" 找到 "/Device/HarddiskVolume1"
*     内核模式下，符号链接以 "\??\" 或者 "\DosDevices\" 开头，如 "\??\C:"
*     用户模式下，符号链接以 "\\.\" 开头， 如 "\\.\C:"
*   2.通过设备接口找到设备
*
*   
******************************************************************************************************************/

/******************************************************************************************************************
* 内核对象
*   DRIVER_OBJECT(驱动对象) -- 需要填写一组回调函数来让Windows调用，插件模式，每个驱动程序只有一个驱动对象
*     FastIoDispatch -- 快速IO分发函数(文件驱动中使用)
*     MajorFunction -- 普通分发函数(DRIVER_DISPATCH)的指针数组,对应各种  IRP_MJ_XXX 
*     DriverStart/DriverSize -- 该驱动对象所代表的内核模块在内核空间中的开始地址和大小
*     DriverStartIo -- 记录StartIO函数的地址，用于串行化操作
*     DeviceObject -- 本驱动中的设备链(DEVICE_OBJECT::NextDevice 指向同一个驱动中的下一个设备)
*     DriverName -- 驱动程序的名字，一般为 "\Driver\驱动程序名称"
*     DriverUnload -- 卸载函数的指针，若不为空，表示可以动态卸载
*     DriverExtension -- 驱动扩展
*       AddDevice -- WDM类型的驱动中，由Pnp管理器调用，负责创建设备对象，其PhysicalDeviceObject为Pnp管理器
*                    传入的底层物理设备对象。
*
*   DEVICE_OBJECT(设备对象) -- 保存设备特征和状态信息，系统上的每一个虚拟、逻辑、物理的设备都有一个设备对象，可以接受请求(IRP)。
*     AttachedDevice -- 有更高一层的驱动附加到这个驱动时，所指向的那个高层的驱动(如 PDO 指向 FDO)
*     CurrentIrp -- 使用StartIO例程时，指向当前IRP结构
*     DeviceExtension -- 设备扩展,可包含任何自定义信息，在IoCreateDevice时指定，由IO管理器创建，保存在非分页内存中。
*       根据不同驱动程序的需要，负责补充定义设备的相关信息。
*     DeviceType -- 设备的类型，如 FILE_DEVICE_UNKNOWN(常在制作虚拟设备时使用) 等
*     DriverObject -- 指向驱动程序中的驱动对象，同属于一个驱动程序的驱动对象指向的是同一驱动对象(过滤驱动中的如何？)
*     NextDevice -- 指向下一个设备对象，形成设备链
*     StackSize -- 多层驱动的层数( 驱动之间会形成类似堆栈的结构,IRP会依次从最高层传递到最底层 )
*   FILE_OBJECT(文件对象)
******************************************************************************************************************/

/******************************************************************************************************************
* 驱动程序的主要功能是在 Dispatch Function(分发函数/派遣函数) 中处理 I/O 请求。
* IRP(I/O Request Packages) -- 主功能号 + 次功能号 PIO_STACK_LOCATION::MinorFunction,
*     各种主功能的分发函数都按功能号做索引，保存在 DriveObject::MajorFunction 数组中(总个数为 IRP_MJ_MAXIMUM_FUNCTION - 0x1b)，
*     操作系统遇到IRP时，就调用对应的函数处理，若不设置，默认是关联 _IopInvalidDeviceRequest 函数(但通过 extern 声明后无法链接 ?)
*     TODO: pDriverObject->MajorFunction[IRP_MJ_WRITE] = USE_WRITE_FUNCTION；   // ?
*   IRP是从非分页内存池中分配的可变大小的结构，关联一个 IO_STACK_LOCATION 结构数组
* 
* IO_STACK_LOCATION -- IO堆栈，对应设备堆栈中每层设备所做的操作，本层设备对应的值可通过 IoGetCurrentIrpStackLocation 获得
* 
*   IoCopyCurrentIrpStackLocationToNext -- 把当前IRP栈空间拷贝到下一个栈空间,需要和 IoSetCompletionRoutine 合用，来做键盘过滤(下发读请求并在读到结果后过滤)
*   IoGetCurrentIrpStackLocation -- 获得当前栈空间信息
*   IoMarkIrpPending -- 
*   IoSetCompletionRoutine -- 设置完成回调函数
*   IoSkipCurrentIrpStackLocation -- 跳过当前栈空间
* 
*   IoCompleteRequest(, IO_NO_INCREMENT) -- 结束IRP，如果调用(WriteFile)是同步，则结束Irp时会唤醒之前被阻塞的同步调用线程
*   IoCreateSymbolicLink -- 在对象管理器中创建一个"符号链接"
*   IoDeleteSymbolicLink --
*
* 驱动创建的设备一般有三种读写方式：1.缓冲区方式(DO_BUFFERED_IO); 2.直接方式; 3.其他方式(?)
*   缓冲区：
*     MDLAddress -- 通过 MDL 把应用层的地址空间映射到内核空间
*     UserBuffer -- 最追求效率的解决方案，应用层的缓冲区地址直接放在其中，在内核控制中访问。只能用于当前进程和发送请求进程一致的情况？
*     AssociatedIrp.SystemBuffer -- 用于比较简单且不追求效率情况下的解决方案(把R3中内存空间中的缓冲数据拷贝到R0)
*   通用的读写数据方式(TODO:没有标志指明使用的具体缓冲区？):
*     if(irp->MdlAddress != NULL) buffer = (PBYTE)MmGetSystemAddressForMdlSafe(irp->MdlAddress);
*     else buffer = (PBYTE)irp->UserBuffer;
*     if(buffer == NULL) buffer = (PBYTE)irp->AssociatedIrp.SystemBuffer;
* 
* IRP类型
******************************************************************************************************************/

/******************************************************************************************************************
* CONTAINING_RECORD -- 通过自定义数据结构中某一个变量的指针得到指向该数据结构的指针。
******************************************************************************************************************/

/******************************************************************************************************************
* DDK不鼓励程序员使用C语言的字符串，主要是容易出现缓冲区溢出。鼓励程序员使用 STRING/ANSI_STRING/UNICODE_STRING 等带长度的结构
* ASCII字符串(ANSI_STRING) -- 
* 宽字符串(UNICODE_STRING) = RTL_CONSTANT_STRING(L"xxx")
*   字符串操作时，Rtl-函数(如 RtlInitUnicodeString、)
*
* <ntstrsafe.h> -- 字符串操作， 如要兼容Win2K，需要先定义 NTSTRSAFE_LIB 宏？
*   RtlInitAnsiString -- 初始化 ANSI_STRING 结构体，这种方法 Buffer 指针会指向初始化的字符串(会跟随其变，但不用清理内存)
*   RtlInitUnicodeString -- 初始化 UNICODE_STRING 结构体，注意：该结构体不要求使用NULL作为字符串的结束(使用通过NULL判断结束的字符串函数会出现问题)
*   RtlStringCchPrintfW
*   RtlFreeAnsiString/RtlFreeUnicodeString -- 销毁用 ExAllocatePool 手工分配内存的字符串(★注意★：不是用RtlInit初始化的字符串)
*   RtlUnicodeStringToInteger/RtlIntegerToUnicodeString -- 字符串 和 整数互相转换
*
* 内核态运行时函数(RtlXxx)
*   RtlCopyMemory -- 不可重叠的内存间复制
*   RtlMoveMemory -- 可重叠的内存间复制(还是移动？以前的内存会被更改吗？)
*   RtlCompareMemory/RtlEqualMemory -- 内存比较
*   
* 堆内存管理
*   ExAllocatePool/ExFreePool -- 指定分配 分页内存/非分页内存, 内存大小最好是 4/8 的倍数
*   ExAllocatePoolWithTag/ExFreePoolWithTag -- 额外分配4字节的标签，调试时可找出是否有标有这个标签的内存没有被释放
*   ExAllocatePoolWithQuota	-- 按配额分配
*   ExAllocatePoolWithQuotaTag
*
* Lookaside 避免内存空洞(适用于：每次申请固定大小的内存；申请和释放的操作十分频繁)
*   初识时向系统申请一块较大的内存，以后每次申请时，从中划分，并能智能地避免产生内存空洞，而且效率高。
*   Win2K后的系统Lookaside分为 Paged 和 NPaged 两类函数，Vista 后的系统通过一套函数(ExInitializeLookasideListEx)统一实现
*   ExInitializeNPagedLookasideList/ExInitializePagedLookasideList -- 初始化 Lookaside
*   ExDeleteNPagedLookasideList/ExDeletePagedLookasideList -- 释放Lookaside
*   ExAllocateFromNPagedLookasideList/ExAllocateFromPagedLookasideList -- 从Lookaside分配内存
*   ExFreeToNPagedLookasideList/ExFreeToPagedLookasideList -- 回收内存
*
*   
*   
* 容器(链表)
*   单向链表(Next指向下一个) -- 
*   双向链表(Blink/Flink) -- 以 LIST_ENTRY 作为第一个元素，来定义每个元素的数据类型，可通过 CONTAINING_RECORD 宏获得变量地址
*     InitializeListHead -- 初始化链表头，Flink和Blink都指向自己，表示为空链
*     IsListEmpty -- 判断是否是空链
*     InsertHeadList/InsertTailList -- 从头部、尾部插入元素
*     RemoveEntryList -- 直接移除元素
*     RemoveHeadList/RemoveTailList -- 从头部、尾部删除元素
*
* 文件操作( Zw 系列函数)，文件名必须是符号链接("\??\C:\Somefile.txt")或设备名("\Device\HarddiskVoluem1\Somefile.txt")
*   ZwCreateFile/ZwOpenFile  -> ZwClose
*   ZwSetInformationFile
*
* 注册表操作
*   通过Rtl运行时函数进行了封装，简化了操作
*     RtlCreateRegistryKey/RtlWriteRegistryValue 等
*   ZwCreateKey/ZwOpenKey
*   ZwSetValueKey/ZwQueryValueKey
*   ZwQueryKey(枚举子项)/ZwEnumerateKey(枚举子健)
******************************************************************************************************************/

/******************************************************************************************************************
* Driver
*   IoCallDriver -- 给设备发送请求?
*
* Device
*   IoAttachDevice/IoDetachDevice  -- 绑定/解除绑定设备到设备栈上，这样可以做过滤(Filter)
*   IoAttachDeviceToDeviceStack -- WDM中IoCreateDevice创建FDO后，需要Attach到PDO上
*   IoCreateDevice/IoDeleteDevice
*   IoEnumerateDeviceObjectList -- 枚举一个驱动下的所有设备对象
*   IoGetAttachedDevice -- 获得一个设备所在的设备栈最顶端的那个设备
*   IoGetDeviceObjectPointer -- 根据设备名字获得设备对象的指针,需要使用 ObDereferenceObject 把文件对象解除引用防止泄露
*   IoSetDeviceInterfaceState
* 
* Other
*   ObDereferenceObject
*   ObReferenceObjectByName(头文件中没有声明?) -- 通过一个名字获得一个对象的指针
*
* 电源(Power)相关?
*   PoStartNextPowerIrp -- 
*   PoCallDriver -- ??
******************************************************************************************************************/

/******************************************************************************************************************
* KeDelayExecutionThread(KernelMode, FALSE, &interval) -- 延时指定时间(原理? 其他线程会继续执行?)
* KsGetCurrentThread --
*
******************************************************************************************************************/

/******************************************************************************************************************
* WDF
*   WdfDriverCreate -- 
******************************************************************************************************************/



#endif //F_DRIVER_API_H