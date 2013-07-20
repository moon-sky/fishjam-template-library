#ifndef F_DRIVER_ENV_H
#define F_DRIVER_ENV_H

/******************************************************************************************************************
* 环境搭建
*   1.VS2008/2010 + WDK(GRMWDK_EN_7600_1.ISO) + 
*   2.包含路径: inc\ddk; inc\crt; inc\api -- ？TODO: 必须放在SDK路径的后面，不能用vsprops或设置在Project中？否则会出现 C2085 等编译错误
*   3.调用约定需要改成 __stdcall(/Gz) ?
* 
* TODO: VirtualKD 中包含了 VirtualDDK ?
* VisualDDK(推荐)  -- http://visualddk.sysprogs.org/, 开源。
*   开发调试驱动的VS插件，提供了驱动程序的向导，可通过TCPIP在远程机器上安装驱动，支持VirtualBox/VMWare，可在VS中直接调试驱动程序。
*   远程调试设置:
*     1.TODO？--需要设置网卡(物理、VMnet)为自动获取IP，允许VMnet8的连接共享，设置虚拟机网卡类型为NAT; 本地主机和虚拟机都要开启远程(协助+桌面)、关闭防火墙
*       检测设置：本地主机和虚拟机能互相ping通；网上邻居能互访；
*     2.安装VirtualDDK后，拷贝 Target 文件下的内容到虚拟机并运行vminstall.exe;
*       TODO: 如果是 VirtualBox?
*
* DDKWizard -- http://ddkwizard.assarbad.net/ ，已被 VirtualDDK 替代
* 
* DriverStudio -- VC6 + Win2K 时代开发驱动的利器，有很好的向导(TODO: 尝试安装并分析学习向导生成的代码)
*
* Visual Studio 中工程向导中： General->Makefile Project，并设置对应的 Build command line 等
*
******************************************************************************************************************/


/******************************************************************************************************************
* 现在推荐使用WDK开发驱动，可以兼容WinNT ~ Win7 等各个系统
*
* VXD() -- Win9X平台的，虚拟设备驱动程序, Win2K后完全被淘汰
* 
* DDK(Driver Development Kit) -- 使用WDM(Windows Driver Model)驱动模型,集成了测试套件HCT？，适用于WinXP及之前的平台
*   NT式驱动程序 -- 不支持即插即用。使用 NTDDK.h 头文件。不分层？
*
* WDM(Windows Driver Model) -- Vista以前的驱动模型。支持即插即用,使用 WDM.h 头文件。
*   建立在NT式驱动程序模型基础上，基于分层的，完成一个设备的操作，至少需要两个驱动设备共同完成。
*   一个PDO(物理设备对象，由总线驱动自动创建)，一个FDO(功能设备对象)并附加到PDO上。
*   其中还有可能存在过滤驱动。通过自定义设备扩展(如名为NextStackDevice)记录并找到下层的过滤驱动，通过 AttachedDevice 找到上层驱动
* 
* WDK(Windows Driver Kit) -- 使用WDF(Windows Driver Foundation)模型，简化WDM的开发，
*   适用于Vista及之后版本OS，包含了以前的DDK，也能编译Win2K等平台的驱动
*   以WDM为基础进行了建模和封装，降低了开发难度：基于对象（属性、方法、事件），封装了一些共同的默认行为（如即插即用和电源管理）
*   驱动分为内核模式(KMD框架)和用户模式(UMD框架)的两种驱动程序。    
* 
******************************************************************************************************************/


/******************************************************************************************************************
* SOURCES -- 其内容关系到该模块要编译那些文件，及编译出来的 .sys 文件的名字
******************************************************************************************************************/


#endif //F_DRIVER_ENV_H