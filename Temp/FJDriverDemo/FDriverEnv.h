#ifndef F_DRIVER_ENV_H
#define F_DRIVER_ENV_H

/******************************************************************************************************************
\\Device\\xxxx
\\DosDevice\xxxx

头文件包含：
  Vista之前的WDK，用于驱动程序开发的主要头文件为 Wdm.h、Ntddk.h 和 Ntifs.h，里面有狠多重复声明(C2371)；
  Vista之后的WDK，按层次结构来组织 Wdm.h、Ntddk.h 和 Ntifs.h 并且不包含重复信息， 每一个都会先包含前一个文件。
  因此：实际开发时，只需要包含 Ntifs.h 即可(能使用所有的函数，而且不会出现 C2371 错误)
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
* 签名：%WDKPATH%\bin\selfsign
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
* VirtualDDK -- 
*   安装好以后，在VS的Tools 菜单下有 "VirtualDDK Settings"，可以设置调试符号等
* 
* DriverStudio -- VC6 + Win2K 时代开发驱动的利器，有很好的向导(TODO: 尝试安装并分析学习向导生成的代码)
*
* Visual Studio 中工程向导中： General->Makefile Project，并设置对应的 Build command line 等
*
* 32位平台下，驱动程序默认采用__stdcall调用约定，该约定编译生成的函数名均带有后缀@xxx，
*   而汇编代码采用__cdecl调用约定，用汇编器编译生成的函数名并不带后缀，从而会链接失败
*   解决方法：强制指定调用外部的汇编函数为 __cdecl 调用约定，如 extern int __cdecl asm_rng_available();
* 64位平台下，只有一个本机调用约定和一个__cdecl约定，DDK编译器生成的函数名和汇编器生成的函数名是一致的，函数名不会带有后缀
******************************************************************************************************************/


/******************************************************************************************************************
* 现在推荐使用WDK开发驱动，可以兼容WinNT ~ Win7 等各个系统
*
* VXD() -- Win9X平台的，虚拟设备驱动程序, Win2K后完全被淘汰
* 
* KMD(Kernel Mode Driver) -- 也叫NT式驱动
*
* DDK(Driver Development Kit) -- 使用WDM(Windows Driver Model)驱动模型，在DriverEntry中创建设备,集成了测试套件HCT？，适用于WinXP及之前的平台
*   NT式驱动程序 -- 不支持即插即用。使用 NTDDK.h 头文件。不分层？
*   安装测试：创建服务，以服务的方式加载在系统中进行测试
*     
* WDM(Windows Driver Model) -- Vista以前的驱动模型。支持即插即用(插入设备后系统自动创建PDO，并提示请求用户安装FDO),在AddDevice中创建设备
*   使用 WDM.h 头文件。链接 wdm.lib，
*   主要是通过各个 MajorFunction 进行分发处理( IRP_MJ_CREATE 等定义在 Wdm.h 中)？
*   建立在NT式驱动程序模型基础上，基于分层的，完成一个设备的操作，至少需要两个驱动设备共同完成。
*   一个PDO(物理设备对象)，当设备插入设备时由总线驱动自动创建，
*   一个FDO(功能设备对象)，系统提示检测到新设备时提示安装驱动程序，该WDM驱动程序创建FDO并附加到PDO上，此时PDO的AttachedDevice会指向FDO.
*   其中还有可能存在过滤驱动。通过自定义设备扩展(如名为NextStackDevice)记录并找到下层的过滤驱动，通过 AttachedDevice 找到上层驱动
*   安装测试：控制面板 -> 增加硬件向导 -> 选择INF文件，然后通过设备管理器 禁用/启用 设备
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
* setenv.bat 后会设置如下环境变量(cmd 中通过 set 查看，注意，有一些 SDK_LIB_PATH 等环境变量没有列举出来)：
*          项目				|  WinXp x86 Checked   |      Win7 x64 Free   |    Win7 ia64 Checked    |
*  BUILD_ALT_DIR(目录)		|     chk_wxp_x86      |     fre_win7_AMD64   |       chk_win7_IA64     |
*  BUILD_DEFAULT_TARGETS	|         -386         |         -amd64       |          -ia64          |
*       DDK_LIB_DEST        |   %WDKPATH%\lib\wxp  |   %WDKPATH%\lib\win7 |     %WDKPATH%\lib\win7  |
*       SDK_LIB_PATH        |                      |                      |                         |
*  DDKBUILDENV/_BuildType	|          chk         |           fre        |           chk           |
*       DDK_TARGET_OS       |         WinXP        |          Win7        |           Win7          |
*       _BUILDARCH          |          x86         |        AMD64         |           IA64          |
*    _NT_TARGET_VERSION     |         0x501        |          0x601       |          0x601          |
*       特殊环境变量        |         386=1        |         AMD64=1      |          IA64=1         |
*                           |    _IA32bit=true     |      _AMD64bit=true  |                         |
*------------------------------------------  代码中可用的宏  ----------------------------------------
*          DBG              |           1          |           0          |            1            |
*       平台编译宏          |        _M_IX86       |         _M_AMD64     |          _M_IA64        |
*----------------------------------------------------------------------------------------------------
* 
*
* 注意：以下文件中，以 井号(#) 开始的是注释
*   makefile -- 里面一般只有一行 "!INCLUDE $(NTMAKEENV)\makefile.def"
*   dirs -- 通过 DIRS=XXX 表示需要编译的子目录，并依次进入子目录进行编译 
*   sources -- 其内容关系到该模块要编译那些文件，及编译出来的 .sys 文件的名字等，其中可通过 $(XXX) 引用其他变量
*     C_DEFINES -- 指示 C 预编译定义的参数，相当于在 C 文件中用 #define 声明的定义， 如 -DUSB2
*     DDKROOT -- DDK 的根目录
*     INCLUDES -- 设定包含目录的路径,如 INCLUDES=$(INCLUDES) $(SDK_INC_PATH)
*     MSC_WARNING_LEVEL -- 编译警告级别，默认为 /W3
*     SOURCES -- 工程所有的源文件，注意只指定 C/C++ 文件
*     TARGETLIBS -- 目标代码所需要的库,如 TARGETLIBS=$(TARGETLIBS) $(SDK_LIB_PATH)\Win32k.lib
*     TARGETNAME -- 目标驱动的名称, 如 FJDriverDemo
*     TARGETPATH -- 目标代码生成的路径, 如 obj
*     TARGETTYPE -- 目标代码生成的类型: DRIVER(驱动), PROGRAM(Win32程序)
*   
******************************************************************************************************************/


#endif //F_DRIVER_ENV_H