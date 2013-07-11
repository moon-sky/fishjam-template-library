#ifndef DRIVER_STUDY_H
#define DRIVER_STUDY_H

//寒江独钓-Windows内核安全编程 -- P
/******************************************************************************************************************
* 内核模块的常见用途：虚拟光驱、防毒软件的实时监控(文件Filter)、防火墙、
*   但使用驱动可能会导致软硬件不兼容(比如QQ登录时键盘驱动保护技术 被淘汰)
*
* 
* 隔离的应用程序 + 共享的内核空间
*   内核模块位于内核空间(被所有进程共享)，但代码的执行一定位于某个进程空间中(可通过 PsGetCurrentProcessId 获取当前进程的进程号 )
* 
* 内核模式(Kernel mode) -- 
* 用户模式(User mode) -- 
*
* 内核空间 -- 受到硬件保护，x66架构下 Ring 0 层的代码才可以访问
* 用户空间 -- 普通应用程序，运行在 Ring 3 层，通过操作系统提供的一个入口(该入口中调用sysenter)来调用R0层的功能
*
******************************************************************************************************************/

/******************************************************************************************************************

******************************************************************************************************************/



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
* MDL -- 内存描述符链
* PDO -- Physical Device Object，物理设备对象，通常是设备栈最下面的那个设备对象？
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
*   
*   类驱动 -- 统管一类设备的驱动程序，如 不管是URB键盘还是PS/2键盘均经过它。
******************************************************************************************************************/

/******************************************************************************************************************
* 系统进程分析
*   csrss.exe -- 
*     win32!RawInputThread 通过 GUID_CLASS_KEYBOARD 获得键盘设备栈中PDO的符号连接名
*        (应用程序不能直接根据设备名打开设备，一般都通过符号链接名来打开)
******************************************************************************************************************/

#endif //DRIVER_STUDY_H