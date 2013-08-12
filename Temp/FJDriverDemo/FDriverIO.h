#ifndef F_DRIVER_IO_H
#define F_DRIVER_IO_H

/******************************************************************************************************************
* 驱动创建的设备一般有三种读写方式,在 IoCreateDevice 后通过设置设备对象的 Flags 项指定：
*   1.DO_BUFFERED_IO(缓冲区)方式 -- OS 自动在 R3(各个进程独立)/R0(整个OS的各个进程使用统一的内存空间)之间拷贝内存数据，
*     优点：比较简单地解决了将用户地址传入驱动的问题; 缺点：需要在用户模式和内核模式之间复制数据，影响效率。适用于只有少量内存操作时。
*     ReadFile/WriteFile -- 使用 IRP.AssociatedIrp.SystemBuffer 
*   2.DO_DIRECT_IO(直接读写)方式 -- OS 将用户模式下的缓冲区锁住，并使用MDL将这段缓冲区在内核模式地址再映射一次，使得用户模式和内核模式的缓冲区指向同一区域的物理内存。
*     使用 IRP.MdlAddress 结构得到被说定的缓冲区内存, 然后通过 MmGetMdlVirtualAddress(应用程序中的虚拟地址) / MmGetSystemAddressForMdlSafe(内核模式下的映射地址) 等宏使用
*   3.其他方式(即不设置 DO_BUFFERED_IO 也不设置 DO_DIRECT_IO) -- 派遣函数直接读写应用程序提供的缓冲区地址 -- 。
*     使用 IRP.UserBuffer(用户模式地址) -- 很危险但最有效率。应用层的缓冲区地址直接放在其中，在内核控制中访问。只有驱动和应用运行在相同线程上下文时才能使用这种方式。
*     需要格外小心，因为ReadFile等可能把空指针地址或非法地址传递给驱动程序，使用前需要先使用 try + ProbeForWrite 探测这段内存是否可读写，否则可能会蓝屏？
* 
*   缓冲区：
*     IoStatus.Information -- 返回表示驱动实际操作了多少字节
* 
*   通用的读写数据方式(TODO:没有标志指明使用的具体缓冲区？):
*     if(irp->MdlAddress != NULL) buffer = (PBYTE)MmGetSystemAddressForMdlSafe(irp->MdlAddress, xxx);
*     else buffer = (PBYTE)irp->UserBuffer;
*     if(buffer == NULL) buffer = (PBYTE)irp->AssociatedIrp.SystemBuffer;
******************************************************************************************************************/
 
/******************************************************************************************************************
* OBJECT_ATTRIBUTES -- 驱动中访问文件、其他的驱动时需要通过这个类型的变量指定目标文件、驱动
*   
*   
* NtCreateFile -- 老版本，头文件中已经不再包含，但实际存在，和Zw-是简单的对应关系？
* ZwCreateFile -- 当前用于操作文件的函数
*
* IoCreateFile -- 比 Zw-更底层的IO操作
* IoCreateDevice -- 生成一个设备对象
* IoCallDriver -- 发送请求
******************************************************************************************************************/

#if 0
//打开其他驱动的代码(同步方式)
UNICODE_STRING otherDeviceName;
OBJECT_ATTRIBUTES objAttributes;
HANDLE hOtherDevice;
IO_STATUS_BLOCK status_block;
RtlInitUnicodeString(&otherDeviceName, L"\\Device\\OtherDeviceName");
InitializeObjectAttributes(&objAttributes, &otherDeviceName, OBJ_CASE_INSENSITIVE, NULL, NULL);
status = ZwCreateFile(&hOtherDevice, FILE_READ_ATTRIBUTES | SYNCHRONIZE, &objAttributes, &status_block, 
	NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN_IF, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
#endif 

/******************************************************************************************************************
* StartIO -- 可以保证各个并行的IRP串行化，注意运行在 DISPATCH_LEVEL 级别，函数声明时需要加上 #pragma LOCKEDCODE 修饰符
*   KDEVICE_QUEUE -- 操作系统提供的可实现IRP串行化的队列，其实例为 DEVICE_OBJECT::DeviceQueue,
*   设置 DRIVER_OBJECT::DriverStartIo 变量指定的回调函数，
*
* 自定义的StartIo(很灵活，但需要负责入队和出队操作) -- 系统定义的StartIo只能使用一个队列，会将所有的IRP(如读、写)混在一起进行串行处理
*   KeInitializeDeviceQueue
*   KeInsertDeviceQueue -- 插入队列，若返回FALSE表明IRP没有插入到队列，需要立即处理
*   KeRemoveDeviceQueue -- 从队列中删除元素
******************************************************************************************************************/

/******************************************************************************************************************
* 中断服务程序(ISR)，
*   驱动中使用ISR，需要先获得中断对象(INTERRUPT)
*   
*   IoConnectInterrupt -- 将中断对象和ISR联系起来，当中断信号来临时进入ISR处理
*   KeSynchronizeExecution -- 
*
* DPC(Delayed Procedure Call) -- 延迟过程调用，有一个队列，系统依次调用，处于 DISPATCH_LEVEL 级别
*   ISR具有很高的IRQL，会打断正常运行的线程，因此一般在ISR中处理紧急代码，而将不需要紧急处理的代码放在DPC例程中。
*   KeInitializeDpc -- 
*   IoRequestDpc -- 放入DPC例程的队列
******************************************************************************************************************/

#endif //F_DRIVER_IO_H