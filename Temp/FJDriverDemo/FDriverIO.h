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
*
* 分为 Buffered IO 和 Direct IO
*
* NtCreateFile -- 老版本，头文件中已经不再包含，但实际存在，和Zw-是简单的对应关系？
* ZwCreateFile -- 当前用于操作文件的函数
*
* IoCreateFile -- 比 Zw-更底层的IO操作
* IoCreateDevice -- 生成一个设备对象
* IoCallDriver -- 发送请求
******************************************************************************************************************/

#endif //F_DRIVER_IO_H