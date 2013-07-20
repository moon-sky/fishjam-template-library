#ifndef F_DRIVER_MEMORY_H
#define F_DRIVER_MEMORY_H

/******************************************************************************************************************
* Windows驱动开发中，所有程序的函数和变量都要指明被加载到 分页内存 中还是 非分页内存 中
*   分页内存
*   非分页内存
*   INIT标志的内存 -- DriverEntry函数使用，表明该函数只在加载的时候需要载入内存，当驱动程序成功加载后，该函数可以从内存中卸载掉
*
******************************************************************************************************************/

#define PAGEDCODE   code_seg("PAGE")
#define LOCKEDCODE  code_seg()

//指明此函数是加载到INIT内存区域中，即成功加载后，可以退出内存(DriverEntry)
#define INITCODE    code_seg("INIT")            

#define PAGEDDATA   data_seg("PAGE")
#define LOCKEDDATA  data_seg()
#define INITDATA    data_seg("INIT")


#endif //F_DRIVER_MEMORY_H