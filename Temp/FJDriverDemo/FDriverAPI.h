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
*
* 注意：
*   1.内核中有很多函数是未文档化或头文件中没有定义的(但在实际的文件中存在)，通常来说只要声明后就可以使用。
*     
******************************************************************************************************************/

/******************************************************************************************************************
* 应用程序一般通过 文件方式( 如 WriteFile / DeviceIoControl ) 与驱动交互 -- I/O管理器担当者用户模式代码和设备驱动程序之间的接口。
* 所有对设备的操作(比如端口的读写、键盘访问、磁盘文件的操作等)最终将转化为IRP请求,这些IRP请求会被传送到派遣函数处理？
* 
* 为了同时支持32/64的驱动，尽量避免使用IOCTL传递包含有指针的结构，如必须传递(如 HANDLE 等)需要特别处理：
*   1.通过 IoIs32bitProcess 判断上层调用者的程序类型，如是的话，可以通过 VOID* POINTER_32 转换(参见 PROTECT_WND_INFO_32 )
*   3.在64位程序中采用新的IOCTL命令
*
* 驱动程序中的设备名称只能在内核状态可见, 应用程序不能直接根据设备名打开设备,应用程序要识别设备有两种方法：
*   1.通过 符号链接(IoCreateSymbolicLink) 找到设备，如通过 "\??\C:" 找到 "/Device/HarddiskVolume1"
*     内核模式下，符号链接以 "\??\" 或者 "\DosDevices\" 开头，如 "\??\C:"
*     用户模式下，符号链接以 "\\.\" 开头， 如 "\\.\C:"
*   2.通过设备接口找到设备
*
* ZwOpenSymbolicLinkObject -- 打开内核模式的符号链接，如 L"\\??\\C:"
* ZwQuerySymbolicLinkObject -- 获取符号链接对应的设备信息
* 
* 驱动中调用其他驱动  
*   1.使用文件句柄方式(类似App调用驱动) -- ZwCreateFile(SYNCHRONIZE 同步) -> ZwReadFile 等
*   2.IoGetDeviceObjectPointer -- 获得设备指针
******************************************************************************************************************/

/******************************************************************************************************************
* 内核对象
* 
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
* 同步IRP -- 派遣函数中直接处理并调用 IoCompleteRequest 函数结束IRP请求( 其内部设置 IRP.UserEvent )
* 异步IRP -- 派遣函数中保存信息(如通过 IoStartPacket 放入StartIo队列)，调用 IoMarkIrpPending 函数告知操作系统该IRP处于挂起状态，并返回 STATUS_PENDING，
*            有助于提高效率，但通常需要进行同步处理(如 StartIO/中断服务例程等)，防止出现逻辑上的错误。
* 
* IO_STACK_LOCATION -- IO堆栈，对应设备堆栈中每层设备所做的操作，本层设备对应的值可通过 IoGetCurrentIrpStackLocation 获得
*   Parameters里面是很多结构体的union，需要根据具体的 功能号 选择对应的成员变量进行处理   
*
*   IoAllocateIrp -- 手工创建任意类型的IRP(其他手工创建方式的底层实现)，创建完毕后需要手动填充IRP的子域，退出时需要通过 IoFreeIrp 删除
*   IoBuildSynchronousFsdRequest/IoBuildAsynchronousFsdRequest -- 手工创建 同步/异步 IRP
*   IoBuildDeviceIoControlRequest -- 手工创建同步类型的 IRP_MJ_INTERNAL_DEVICE_CONTROL 或 IRP_MJ_DEVICE_CONTROL
*   IoCallDriver -- 给设备发送请求，通常是向设备堆栈的下层设备发送请求。
*   IoCancelIrp -- 取消指定的IRP请求，其内部会使用叫做 cancel 的自旋锁进行同步，需要在 IoSetCancelRoutine 设定的
*     取消回调函数中调用 IoReleaseCancelSpinLock(Irp->CancelIrql) 释放该自旋锁。
*     可通过DPC定时器设置超时并调用该函数取消IRP，防止无响应。
*   IoCompleteRequest(, IO_NO_INCREMENT) -- 结束IRP，如果调用(WriteFile)是同步，则结束Irp时会唤醒之前被阻塞的同步调用线程
*   IoCopyCurrentIrpStackLocationToNext -- 把当前IRP栈空间拷贝到下一个栈空间,需要和 IoSetCompletionRoutine 合用，来做键盘过滤(下发读请求并在读到结果后过滤)
*   IoGetCurrentIrpStackLocation -- 获得当前栈空间信息
*   IoMarkIrpPending -- 告知操作系统该IRP处于挂起状态
*   IoSetCancelRoutine -- 设置取消IRP请求的回调函数，当该IRP被取消时，操作系统会调用该取消函数
*   IoSetCompletionRoutine -- 设置完成回调函数
*   IoSkipCurrentIrpStackLocation -- 跳过当前栈空间(本质是将当前IO堆栈往上移动一层?)，常用于当前设备不需要对应I/O堆栈时
*   IoStartNextPacket -- 从队列中取出下一个IRP并调用StartIo
*   IoStartPacket -- 调用StartIo函数或将IRP放入等待队列
*   IoCreateSymbolicLink -- 在对象管理器中创建一个"符号链接"
*   IoDeleteSymbolicLink --
*   KeRemoveEntryDeviceQueue -- 
* 
* IRP类型
*
* 手动创建IRP
*   1.得到设备指针 -- IoGetDeviceObjectPointer 或 ObReferenceObjectByPointer 等
*   2.手工创建IRP  -- IoBuildSynchronousFsdRequest / IoAllocateIrp 等
*   2.构造IRP的I/O堆栈，每层IO堆栈对应一个设备对象, IoGetNextIrpStackLocation ?
*   3.通过 IoCallDriver 调用相应的驱动
******************************************************************************************************************/

/******************************************************************************************************************
* CONTAINING_RECORD -- 通过自定义数据结构中某一个变量的指针得到指向该数据结构的指针。
******************************************************************************************************************/

/******************************************************************************************************************
* DDK不鼓励程序员使用C语言的字符串，主要是容易出现缓冲区溢出。鼓励程序员使用 STRING/ANSI_STRING/UNICODE_STRING 等带长度的结构
*
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
* 容器(链表) -- 可通过 CONTAINING_RECORD 宏从 LIST_ENTRY 变量获得用户定义结构体变量的地址
*   单向链表(Next指向下一个) -- 
*   双向链表(Blink/Flink) -- 以 LIST_ENTRY 作为第一个元素，来定义每个元素的数据类型，
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
*
* 系统操作
*   MmGetSystemRoutineAddress -- 可以得到核心层的函数指针?
*   KeUserModeCallback -- Ring0调用Ring3的函数，其他的方式还有 APC 等
*
* 内核对象句柄和指针 -- 每个内核中的句柄都会和一个内核对象的指针联系起来，可互相转化
*   IoGetDeviceObjectPointer -- 通过设备名获得设备栈顶端设备和文件对象
*
* 辅助函数
*   PtrToUlong/PtrToLong -- 截断指针， 如果要将指针的值保存成 INT 等值，需要使用 INT_PTR 等
******************************************************************************************************************/

/******************************************************************************************************************
* Driver
*
* Device
*   IoAttachDevice/IoDetachDevice  -- 绑定/解除绑定设备到设备栈上，这样可以做过滤(Filter)
*   IoAttachDeviceToDeviceStack/IoAttachDeviceToDeviceStackSafe -- 可用于分层驱动挂载，如：WDM中IoCreateDevice创建FDO后，需要Attach到PDO上
*   IoCreateDevice/IoDeleteDevice
*   IoEnumerateDeviceObjectList -- 枚举一个驱动下的所有设备对象
*   IoGetAttachedDevice -- 获得一个设备所在的设备栈最顶端的那个设备
*   IoGetDeviceObjectPointer -- 根据设备名字获得设备对象的指针,需要使用 ObDereferenceObject 把文件对象解除引用防止泄露
*   IoSetDeviceInterfaceState
* 
* Other
*   ObDereferenceObject
*   ObReferenceObjectByName(头文件中没有声明?) -- 通过一个名字获得一个对象的指针，使用完毕后需要通过 ObDereferenceObject 将引用计数减1
*
* 时间相关的函数
*   时间参数：
*     小于0的时间值代表从当前开始的相对时间(100ns的倍数)
*     大于0的时间值代表从 1601-01-01 开始的绝对的时间(100ns的倍数)
*   定时器 -- 分为 IO定时器 和 DPC定时器
*     IoInitializeTimer/IoStartTimer/IoStopTimer -- IO定时器,系统每1s激发一次定时器例程(运行在 DISPATCH_LEVEL),
*       运行在任意线程，因此不能直接使用App中的内存地址
*     KeInitializeTimer/KeSetTimer/KeCancelTimer -- DPC定时器，通过DPC队列执行，可对任意间隔时间进行定时。
*       每一次 KeSetTimer 只会触发一次,若要周期性触发DPC例程，需要在处理函数中周期性调用 KeSetTimer.
* 
*   KeDelayExecutionThread -- 强制当前线程进入睡眠状态，经过指定的睡眠时间后，线程恢复运行。
*   KeStallExecutionProcessor -- 让CPU处于忙等状态，经过指定时间后，继续让线程运行(由于不会进入睡眠，因此不会发生线程的切换，延时比较精确)
*   KeQuerySystemTime -- 获取当前系统时间(从  1601-01-01 开始的格林尼治时间)
*   ExSystemTimeToLocalTime -- 将系统时间转换为当前时区对应的时间
*   RtlTimeToTimeFields -- 由系统时间得到具体的年月日等信息
*
* 电源(Power)相关?
*   PoStartNextPowerIrp -- 
*   PoCallDriver -- ??
******************************************************************************************************************/
/******************************************************************************************************************
* KeDelayExecutionThread(KernelMode, FALSE, &interval) -- 延时指定时间(原理? 其他线程会继续执行?)
* KeInitializeSpinLock / KeQueryInterruptTime
* KsGetCurrentThread --
* 
******************************************************************************************************************/

/******************************************************************************************************************
* WDF
*   WdfDriverCreate -- 
******************************************************************************************************************/

#ifdef __cplusplus
extern "C" 
{
#endif


//声明未文档化，且常用的一些API原型
NTSTATUS ZwQuerySystemInformation( 
								  IN ULONG SystemInformationClass, 
								  IN PVOID SystemInformation, 
								  IN ULONG SystemInformationLength, 
								  OUT PULONG ReturnLength);

NTKERNELAPI 
NTSTATUS ObReferenceObjectByName(
                                 IN PUNICODE_STRING ObjectName,                 //打开设备的设备名，如 L"\\??\\HelloDDK"
                                 IN ULONG Attributes,                           //一般设置为 OBJ_CASE_INSENSITIVE
                                 IN PACCESS_STATE PassedAccessState OPTIONAL,   //很少用，一般设为 NULL
                                 IN ACCESS_MASK DesiredAccess OPTIONAL,         //权限，一般为 FILE_ALL_ACCESS
                                 IN POBJECT_TYPE ObjectType,                    //指定对象类型，IoDeviceObjectType 表示需要获得设备对象的指针
                                 IN KPROCESSOR_MODE AccessMode,                 //如内核中访问该指针，设为 KernelMode
                                 IN OUT PVOID ParseContext OPTIONAL,            //一般设为 NULL
                                 OUT PVOID *Object                              //返回的内核对象指针
                                 );

extern POBJECT_TYPE IoDeviceObjectType;

//extern "C" NTSTATUS ZwDuplicateObject(
//						   IN HANDLE                 SourceProcessHandle,
//						   IN PHANDLE                 SourceHandle,
//						   IN HANDLE                 TargetProcessHandle,
//						   OUT PHANDLE               TargetHandle,
//						   IN ACCESS_MASK             DesiredAccess OPTIONAL,
//						   IN BOOLEAN                 InheritHandle,
//						   IN ULONG                   Options );


#ifdef __cplusplus
}
#endif


//ntifs.h 中

#endif //F_DRIVER_API_H