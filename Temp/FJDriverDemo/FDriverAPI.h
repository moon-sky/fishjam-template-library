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
* 内核对象
*   DRIVER_OBJECT(驱动对象) -- 需要填写一组回调函数来让Windows调用，插件模式，每个驱动程序只有一个驱动对象
*     快速IO分发函数 -- FAST_IO_DISPATCH
*     MajorFunction -- 普通分发函数(DRIVER_DISPATCH),对应各种  IRP_MJ_XXX 
*     DriverStart/DriverSize -- 该驱动对象所代表的内核模块在内核空间中的开始地址和大小
*     DeviceObject -- 设备链(DEVICE_OBJECT::NextDevice 指向同一个驱动中的下一个设备)
*     DriverUnload -- 卸载函数的指针，若不为空，表示可以动态卸载
*     DriverExtension -- 设备扩展，其中有 AddDevice 等回调函数指针
* 
*   DEVICE_OBJECT(设备对象) -- 保存设备特征和状态信息，系统上的每一个虚拟、逻辑、物理的设备都有一个设备对象，可以接受请求(IRP)。
*     DeviceExtension -- 设备扩展,可包含任何自定义信息，在IoCreateDevice时指定。根据不同驱动程序的需要，负责补充定义设备的相关信息。
*   FILE_OBJECT(文件对象)
******************************************************************************************************************/

/******************************************************************************************************************
* 应用程序一般通过 DeviceIoControl 与驱动交互
* 所有对设备的操作最终将转化为IRP请求,这些IRP请求会被传送到派遣函数处理？
*
* IRP (主功能号 + 次功能号 PIO_STACK_LOCATION::MinorFunction),各种主功能的分发函数都按功能号做索引
*     保存在 DriveObject::MajorFunction 数组中(总个数为 IRP_MJ_MAXIMUM_FUNCTION - 0x1b)，操作系统遇到IRP时，就调用对应的函数处理
*     TODO: pDriverObject->MajorFunction[IRP_MJ_WRITE] = USE_WRITE_FUNCTION；   // ?
*   IRP是从非分页内存池中分配的可变大小的结构，关联一个 IO_STACK_LOCATION 结构数组
*
*   IoCopyCurrentIrpStackLocationToNext -- 把当前IRP栈空间拷贝到下一个栈空间,需要和 IoSetCompletionRoutine 合用，来做键盘过滤(下发读请求并在读到结果后过滤)
*   IoGetCurrentIrpStackLocation -- 获得当前栈空间信息
*   IoMarkIrpPending -- 
*   IoSetCompletionRoutine -- 设置完成回调函数
*   IoSkipCurrentIrpStackLocation -- 跳过当前栈空间
* 
*   IoCompleteRequest(, IO_NO_INCREMENT); ??
*   IoCreateSymbolicLink -- 在对象管理器中创建一个"符号链接"(如 Z: => \Device\NetworkRedirector )
*   IoDeleteSymbolicLink --
*
*   缓冲区：
*     MDLAddress -- 通过 MDL 把应用层的地址空间映射到内核空间
*     UserBuffer -- 最追求效率的解决方案，应用层的缓冲区地址直接放在其中，在内核控制中访问。只能用于当前进程和发送请求进程一致的情况？
*     AssociatedIrp.SystemBuffer -- 用于比较简单且不追求效率情况下的解决方案(把R3中内存空间中的缓冲数据拷贝到R0)
*   通用的读写数据方式(TODO:没有标志指明使用的具体缓冲区？):
*     if(irp->MdlAddress != NULL) buffer = (PBYTE)MmGetSystemAddressForMdlSafe(irp->MdlAddress);
*     else buffer = (PBYTE)irp->UserBuffer;
*     if(buffer == NULL) buffer = (PBYTE)irp->AssociatedIrp.SystemBuffer;
* 
******************************************************************************************************************/

/******************************************************************************************************************
*   
******************************************************************************************************************/

/******************************************************************************************************************
* <ntstrsafe.h> -- 字符串操作， 如要兼容Win2K，需要先定义 NTSTRSAFE_LIB 宏
*   RtlStringCchPrintfW
*   RtlInitUnicodeString -- 初始化 UNICODE_STRING 结构体，该结构体不需要使用NULL作为字符串的结束
******************************************************************************************************************/

/******************************************************************************************************************
* Driver
*   IoCallDriver -- 给设备发送请求?
*
* Device
*   IoAttachDevice/IoDetachDevice  -- 绑定/解除绑定设备到设备栈上，这样可以做过滤(Filter)
*   IoAttachDeviceToDeviceStack
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