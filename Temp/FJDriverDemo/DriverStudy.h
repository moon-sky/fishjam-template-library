#ifndef DRIVER_STUDY_H
#define DRIVER_STUDY_H

/******************************************************************************************************************
* CDO -- 控制设备(Control Device Object)，修改整个驱动的内部配置，一个Driver只对应一个CDO？
* DCT -- Display Compatibility Kit
* DDK -- Driver Development Kit，WinNT、WinXP等平台的开发包
* DSF -- Device Simulation Framework
* FDE -- Full Disk Encryption,全磁盘加密
* FDO -- Function Device Object ，功能设备对象
* HCT -- Hardware Compatibility Test(硬件兼容性测试工具)
* IFS -- Installable File Systems 
* IRP -- I/O Request Package，输入输出请求包，驱动程序的运行，依靠IRP驱动。通过主功能号(Major)和次功能号标识一个IRP的功能。
* KMDF (Kernel Mode Driver Framework): A framework for developing kernel mode drivers. 
* PDO -- Physical Device Object，物理设备对象
* UMDF (User Mode Driver Framework): A framework for developing user mode drivers. 
* VPB -- Volume parameter block
* WDF  -- Windows Driver Foundation
* WDM -- Windows Driver Model
* WDK -- Windows Driver Kit，Vista后的开发包，兼容WinXP
* WDTF -- Windows Device Testing Framework
* 
* WDK = DDK (Driver Development Kit) 
*   + HCT Kit (Hardware Compatibility Test) 
*   + WDF (Windows Driver Foundation) 
*   + DTM (Driver Test Manager) 
*   + WDF (Driver Verification Tools)
*   + IFS Kit (Installable File Systems Kit) 
*   + Free ISO image download 
*   - Visual Studio 2005 out of the box integration
* 
* 
* 不同类型的驱动程序
*   文件系统驱动(File System Driver)
*   存储设备驱动(Storage Driver)
*   网络驱动(Network Driver)
******************************************************************************************************************/


#endif //DRIVER_STUDY_H