#ifndef DRIVER_FILTER_H
#define DRIVER_FILTER_H

/******************************************************************************************************************
* 过滤(Filter) -- 在设备栈中可以优先进行处理(可实现文件过滤、实时防毒等功能)
*   1.生成一个虚拟的设备对象
*     IoCreateDevice -- 
*   2.并Attach到真实的设备上
*     UNICODE_STRING com_device_name = RTL_CONSTANT_STRING(L"\\Device\\Serial0");
*     IoAttachDevice(myFilterDevice, &com_device_name, &attached_device);	//绑定具有名字的设备
*     //IoAttachDeviceToDeviceStackSafe -- 根据设备对象的指针绑定
* 
*
******************************************************************************************************************/


#endif //DRIVER_FILTER_H