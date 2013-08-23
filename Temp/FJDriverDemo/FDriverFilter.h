#ifndef F_DRIVER_FILTER_H
#define F_DRIVER_FILTER_H

/******************************************************************************************************************
* 若干个设备(可以属于不同的驱动)依次绑定形成一个设备栈，总是最顶端的设备先接收到请求(IRP)。
*   上层过滤层驱动(FiDO)
*   功能驱动程序(FDO)
*   下层过滤层驱动(FiDO)
*   总线驱动程序(PDO)
* 
* 过滤(Filter) -- 在设备栈中可以优先进行处理(可实现文件过滤、实时防毒等功能)
*   1.生成一个虚拟的设备对象
*     IoCreateDevice -- 
*    
*   2.并Attach到真实的设备上
*     UNICODE_STRING com_device_name = RTL_CONSTANT_STRING(L"\\Device\\Serial0");
*     IoAttachDevice(myFilterDevice, &com_device_name, &attached_device);	//绑定具有名字的设备
*     绑定完毕必须清除 DO_DEVICE_INITIALIZING 标志，并设置需要的标志
*     
*   3.根据主功能号对IRP进行过滤，得到数据并处理
*     for (i < IRP_MJ_MAXIMUM_FUNCTION; i++) { driver->MajorFunction[i] = myDispatch; } 
*   n.IoDetachDevice + IoDeleteDevice
*     
*
******************************************************************************************************************/

/******************************************************************************************************************
* 键盘(类驱动:\\Driver\\Kbdclass, USB键盘:\\Driver\\Kbdhid, PS/2键盘: \\Driver\\i8042prt ): 
*   中断号为 0x93, 端口号为 0x60
*   如果按下的扫描码为 X，则同一个键弹起的扫描码为 X+0x80
*   键盘总是处于"有一个读请求没有完成"的状态(即处理完用户输入后马上会请求读取下一个输入，如没有输入则等待)
*   KEYBOARD_INPUT_DATA 中 MakeCode 就是扫描码， if( 扫描码 & 0x80) == 0 ){ 按键按下 }
*
* 一般来说，黑客不会去插入一个能被安全软件轻易监控到的过滤设备，但是依然能通过修改一个已经存在的驱动对象的分发函数
*   的指针来实现过滤所有请求的目的。
******************************************************************************************************************/

//不关心的分发函数实现方式, 需要先通过自定义的 PC2P_DEV_EXT 设备扩展结构体保存下一层级的设备对象
#if 0
NTSTATUS myDispatchGeneral(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	IoSkipCurrentIrpStackLocation(Irp);
	return IoCallDriver(((PC2P_DEV_EXT)DeviceObject->DeviceExtension)->LowerDeviceObject, Irp);
}
#endif //if 0

#endif //F_DRIVER_FILTER_H