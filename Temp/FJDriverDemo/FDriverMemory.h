#ifndef F_DRIVER_MEMORY_H
#define F_DRIVER_MEMORY_H

/******************************************************************************************************************
* Windows驱动开发中，所有程序的函数和变量都要指明被加载到 分页内存 中还是 非分页内存 中
*   当程序的中断请求级在 DISPATCH_LEVEL 和其上时，程序只能使用非分页内存，否则将导致蓝屏死机。
*
*   分页内存 -- 可以交换到文件中的虚拟内存页面
*   非分页内存 -- 永远不会被交换到文件中的虚拟内存页面，
*   INIT标志的内存 -- 一般对DriverEntry或其直接调用的辅助函数使用，表明该函数只在加载的时候需要载入内存，当驱动程序成功加载后，该函数可以从内存中卸载掉
*
* 物理内存地址(Physical Memory Address) -- 显卡等的显存会映射到实际的物理内存地址
* 虚拟内存地址(Virtual Memory Address) -- 
* 
* 驱动程序里的不同例程运行在不同的进程中：
*   1.DriverEntry 和 AddDevice 例程是运行在系统(System)进程中
*   2.各派遣函数会运行于应用程序的上下文中，如 IRP_MJ_READ、IRP_MJ_WRITE 等
*
* 驱动程序的栈空间很小，所以驱动程序不适合递归调用或局部变量是大型结构体，大型数据需要在堆中申请。
* 频繁申请和回收内存，会导致在内存上产生大量的内存“空洞”，从而导致最终无法申请内存，DDK提供了 Lookaside 结构来解决这个问题。
*
* 试探指定内存的可读写性 -- 不可读写时会引发异常，必须用结构化异常处理(__try...__except)
*   ProbeForRead/ProbeForWrite
******************************************************************************************************************/

//分页内存，当函数在分页内存中时，需要在函数的实现中加入 PAGED_CODE() 宏检验该函数是否运行的中断请求级，不满足的话会产生断言
#define PAGEDCODE   code_seg("PAGE")

//非分页内存
#define LOCKEDCODE  code_seg()

//指明此函数是加载到INIT内存区域中，即成功加载后，可以退出内存(DriverEntry)
#define INITCODE    code_seg("INIT")            

#define PAGEDDATA   data_seg("PAGE")
#define LOCKEDDATA  data_seg()
#define INITDATA    data_seg("INIT")


#endif //F_DRIVER_MEMORY_H