#ifndef F_DRIVER_API_H
#define F_DRIVER_API_H

//可以用与 KeDelayExecutionThread 的宏
#define DELAY_ONE_MICROSECOND	(-10)
#define DELAY_ONE_MILLISECOND	(DELAY_ONE_MICROSECOND * 1000)
#define DELAY_ONE_SECOND		(DELAY_ONE_MILLISECOND * 1000)

/******************************************************************************************************************
* 内核对象
*   DRIVER_OBJECT(驱动对象) -- 需要填写一组回调函数来让Windows调用，插件模式，每个驱动程序只有一个驱动对象
*     快速IO分发函数 -- FAST_IO_DISPATCH
*     普通分发函数   -- DRIVER_DISPATCH,  MajorFunction?
*     DriverStart/DriverSize -- 该驱动对象所代表的内核模块在内核空间中的开始地址和大小
*     DeviceObject -- 设备链(其中的 NextDevice 指向同一个驱动中的下一个设备)
*     DriverUnload -- 卸载函数的指针，若不为空，表示可以动态卸载
*     
*   DEVICE_OBJECT(设备对象) -- 可以接受请求(IRP)
*     DeviceExtension -- 设备扩展,可包含任何自定义信息，在IoCreateDevice时指定
*   FILE_OBJECT(文件对象)
******************************************************************************************************************/

/******************************************************************************************************************
* IRP (主功能号 + 次功能号 PIO_STACK_LOCATION::MinorFunction)
*   各种主功能的分发函数都按功能号做索引保存在 DriveObject::MajorFunction 数组中
*
*   IoCopyCurrentIrpStackLocationToNext -- 把当前IRP栈空间拷贝到下一个栈空间,需要和 IoSetCompletionRoutine 合用，来做键盘过滤(下发读请求并在读到结果后过滤)
*   IoGetCurrentIrpStackLocation -- 获得当前栈空间
*   IoMarkIrpPending -- 
*   IoSetCompletionRoutine -- 设置完成回调函数
*   IoSkipCurrentIrpStackLocation -- 跳过当前栈空间
*   IoCompleteRequest(, IO_NO_INCREMENT); ??
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
*   RtlInitUnicodeString
******************************************************************************************************************/

/******************************************************************************************************************
* Driver
*   IoCallDriver -- 给设备发送请求?
*
* Device
*   IoAttachDevice/IoDetachDevice  -- 绑定/解除绑定设备到设备栈上，这样可以做过滤(Filter)
*   IoCreateDevice/IoDeleteDevice
*   IoEnumerateDeviceObjectList -- 枚举一个驱动下的所有设备对象
*   IoGetAttachedDevice -- 获得一个设备所在的设备栈最顶端的那个设备
*   IoGetDeviceObjectPointer -- 根据设备名字获得设备对象的指针,需要使用 ObDereferenceObject 把文件对象解除引用防止泄露
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



#endif //F_DRIVER_API_H