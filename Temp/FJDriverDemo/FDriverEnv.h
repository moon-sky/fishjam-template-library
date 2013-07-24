#ifndef F_DRIVER_ENV_H
#define F_DRIVER_ENV_H

/******************************************************************************************************************
\\Device\\xxxx
\\DosDevice\xxxx
******************************************************************************************************************/

/******************************************************************************************************************
* 环境搭建 (若使用C++开发，需要注意最好只使用基本的C++特性，而不要使用模版、纯虚等高级特性)
*   1.VS2008/2010 + WDK(GRMWDK_EN_7600_1.ISO) + 
*   2.包含路径: inc\ddk; inc\crt; inc\api -- ？TODO: 必须放在SDK路径的后面，不能用vsprops或设置在Project中？否则会出现 C2085 等编译错误
*   3.链接 ntoskrnl.lib 
*   4.调用约定必须是 __stdcall(/Gz) -- VC默认使用__cdecl，否则可能链接不过，_DriverEntry(错误) => _DriverEntry@8(正确)
*     DDK 环境编译时默认采用的是复合要求的标准调用约定。
*   5./entry:"DriverEntry"
*   6.注意：
*     a.不能使用编译器运行时函数(RunTime Func)，甚至 malloc 和 new 都不能使用，相应的应该使用 RtlXxx的内核态运行时函数
*     b.尽量避免全局变量的使用(不容易同步)，通过将变量保存在 设备扩展对象 中解决
*   命令行编译：Build Environments -> Windows XXX -> Checked/Free， 到对应目录下执行 build 命令
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
******************************************************************************************************************/


/******************************************************************************************************************
* 现在推荐使用WDK开发驱动，可以兼容WinNT ~ Win7 等各个系统
*
* VXD() -- Win9X平台的，虚拟设备驱动程序, Win2K后完全被淘汰
* 
* DDK(Driver Development Kit) -- 使用WDM(Windows Driver Model)驱动模型,集成了测试套件HCT？，适用于WinXP及之前的平台
*   NT式驱动程序 -- 不支持即插即用。使用 NTDDK.h 头文件。不分层？
*     类似于Windows服务程序，可以以服务的方式加载在系统中进行测试。
*     
* WDM(Windows Driver Model) -- Vista以前的驱动模型。支持即插即用(插入设备后系统自动创建PDO，并提示请求用户安装FDO),
*   使用 WDM.h 头文件。链接 wdm.lib，
*   主要是通过各个 MajorFunction 进行分发处理( IRP_MJ_CREATE 等定义在 Wdm.h 中)？
*   建立在NT式驱动程序模型基础上，基于分层的，完成一个设备的操作，至少需要两个驱动设备共同完成。
*   一个PDO(物理设备对象)，当设备插入设备时由总线驱动自动创建，
*   一个FDO(功能设备对象)，系统提示检测到新设备时提示安装驱动程序，该WDM驱动程序创建FDO并附加到PDO上，此时PDO的AttachedDevice会指向FDO.
*   其中还有可能存在过滤驱动。通过自定义设备扩展(如名为NextStackDevice)记录并找到下层的过滤驱动，通过 AttachedDevice 找到上层驱动
* 
* WDK(Windows Driver Kit) -- 使用WDF(Windows Driver Foundation)模型，简化WDM的开发，
*   适用于Vista及之后版本OS，包含了以前的DDK，也能编译Win2K等平台的驱动
*   以WDM为基础进行了建模和封装，降低了开发难度：基于对象（属性、方法、事件），封装了一些共同的默认行为（如即插即用和电源管理）
*   驱动分为内核模式(KMD框架)和用户模式(UMD框架)的两种驱动程序。    
* 
*
* 各种驱动架构的对比
*     项目        |         DDK         |             WDM              |        WDK          |
*   入口程序      |     DriverEntry     |         DriverEntry          |     DriverEntry     |
*    加载方式     |     主动加载设备    |      加载PDO后被动加载       | 
*     初始化      |     DriverEntry     | AddDevice中创建FDO,设置Flags |
*     卸载        |     DriverUnload    |     IRP_MN_REMOVE_DEVICE     |
*   派遣函数      |        无要求?      |         IRP_MJ_PNP           |
******************************************************************************************************************/


/******************************************************************************************************************
* setenv.bat 后会设置如下环境变量(比较了 Win7 ia64 Checked 和 WinXp x86 Free)：
*   BUILD_ALT_DIR -- 目录， 如 chk_win7_IA64, fre_wxp_x86
*   BUILD_DEFAULT_TARGETS -- 目标类型(注意前面有一个横线)，如 -ia64, -386
*   DDKBUILDENV 或 _BuildType -- 编译类型，如 chk, fre
*   DDK_TARGET_OS -- 目标OS，如 Win7, WinXP
*   _BUILDARCH -- 架构，如 IA64, x86
*   _NT_TARGET_VERSION -- 如 0x601, 0x501
*   另外：x86 的会生成 386=1 的环境变量； Ia64 的会生成 IA64=1；
*
* makefile -- 里面一般只有一行 "!INCLUDE $(NTMAKEENV)\makefile.def"
* dirs -- 通过 DIRS=XXX 表示需要编译的子目录，并依次进入子目录进行编译 
* sources -- 其内容关系到该模块要编译那些文件，及编译出来的 .sys 文件的名字等，其中可通过 $(XXX) 引用其他变量
*   C_DEFINES -- 指示 C 预编译定义的参数，相当于在 C 文件中用 #define 声明的定义， 如 -DUSB2
*   DDKROOT -- DDK 的根目录
*   INCLUDES -- 设定包含目录的路径
*   MSC_WARNING_LEVEL -- 编译警告级别，默认为 /W3
*   SOURCES -- 工程所有的源文件，注意只指定 C/C++ 文件
*   TARGETLIBS -- 目标代码所需要的库, 如 ntoskrnl.lib
*   TARGETNAME -- 目标驱动的名称, 如 FJDriverDemo
*   TARGETPATH -- 目标代码生成的路径, 如 obj
*   TARGETTYPE -- 目标代码生成的类型: DRIVER(驱动), PROGRAM(Win32程序)
*   
******************************************************************************************************************/


#endif //F_DRIVER_ENV_H