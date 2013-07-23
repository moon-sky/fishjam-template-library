#ifndef F_DRIVER_STUDY_H
#define F_DRIVER_STUDY_H

//寒江独钓-Windows内核安全编程 -- P14X
//Windows驱动开发技术详解 -- P118(UNICODE_STRING)  (优先看调试技巧?)

//与用户模式通信(DeviceIoControl?)
//  驱动程序通过 HAL 调用硬件抽象层，然后才和硬件交互。
//  在驱动程序中，应尽量避免使用针对平台的汇编指令(如 IN, OUT)，而应该使用与平台无关的HAL函数或宏（如 READ_PORT_BUFFER_UCHAR ）
/******************************************************************************************************************
* 内核模块的常见用途：虚拟光驱、防毒软件的实时监控(文件Filter)、防火墙、
*   但使用驱动可能会导致软硬件不兼容(比如QQ登录时键盘驱动保护技术 被淘汰)
*
* 
* 隔离的应用程序 + 共享的内核空间
*   内核模块位于内核空间(被所有进程共享)，但代码的执行一定位于某个进程空间中(可通过 PsGetCurrentProcessId 获取当前进程的进程号 )
* 
* CPU一般会有多个特权层(Intel的386有4个特权层, Ring0~Ring3)
*   内核模式(Kernel mode) -- 运行在Ring0层，可执行任意代码(如直接访问物理内存、物理端口等)
*   用户模式(User mode) -- 运行在Ring3成，只能执行优先的代码
*
* 内核空间 -- 受到硬件保护，x66架构下 Ring 0 层的代码才可以访问。分为 paged pool 和 non-paged pool，驱动程序根据需要进行申请
* 用户空间 -- 普通应用程序，运行在 Ring 3 层，通过操作系统提供的一个入口(该入口中调用sysenter)来调用R0层的功能
*
* 单一内核(Linux) -- 系统的主要核心组件全部在内核实现，各部分耦合性大，不利于模块化设计。但速度快。
* 微内核(Windows) -- 系统的主要核心组件运行在独立的进程中，有自己私有的地址空间，使用服务原语通过进程间通信交互。
*
* 每个驱动都是以一个类似服务的形式存在，在系统注册表的 HKLM\SYSTEM\CurrentControlSet\Services 下和驱动名相同的子树
* 
******************************************************************************************************************/

/******************************************************************************************************************
* CDO -- 控制设备(Control Device Object)，修改整个驱动的内部配置，一个Driver只对应一个CDO？
* DCT -- Display Compatibility Kit
* DDK -- Driver Development Kit，WinNT、WinXP等平台的开发包
* DMA -- 
* DSF -- Device Simulation Framework
* DPC -- Delayed Procedure Call，延迟过程调用，完成一个IO请求和开始另一个中断驱动的传输
* FDE -- Full Disk Encryption,全磁盘加密
* FDO -- Function Device Object ，功能设备对象，一般就是自己驱动中通过 IoCreateDevice 创建的设备对象?
* FSD -- File System Driver, 文件系统驱动
* HAL -- 
* HCT -- Hardware Compatibility Test(硬件兼容性测试工具)
* IFS -- Installable File Systems 
* IRP -- I/O Request Package，输入输出请求包，驱动程序的运行，依靠IRP驱动。通过主功能号(Major)和次功能号标识一个IRP的功能。
* ISR -- 中断服务程序
* KMDF (Kernel Mode Driver Framework): A framework for developing kernel mode drivers. 
* MDL -- 内存描述符链
* PDO -- Physical Device Object，物理设备对象，通常是设备栈最下面的那个设备对象？
* UMDF (User Mode Driver Framework): A framework for developing user mode drivers. 
* VPB -- Volume parameter block
* WDF  -- Wi传输ndows Driver Foundation
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
*   System -- 系统进程，在系统启动时创建。
*     当驱动加载的时候，系统进程启动新的线程，调用对象管理器，创建一个驱动对象(DRIVER_OBJECT)，然后调用其 DriverEntry 
******************************************************************************************************************/

/******************************************************************************************************************
* Windows 的设计目标和实现方式(设计思想是将内核设计的尽可能小，并采用"客户端"-"服务器"的结构，各个组件或模块通过消息进行通信)
*   1.可移植性 -- 软件分层，只有最下面的HAL(硬件抽象层)和硬件紧密关联
*   2.兼容性 -- 一致的API接口，环境子系统(向各自的应用程序提供相应的API支持)支持兼容模式
*   3.健壮性和可靠性 -- 用户模式和内核模式的划分，对于任何涉及操作硬件的操作，应用程序都必须通过对内核模式中的系统调用来完成。
*   4.可扩展性 -- 内核从执行体组件中分离出来，只负责关于线程的调度工作，其他执行程序组件(内存管理、进程管理)作为独立于内核
*       的组件，按照模块化的方法设计，在需要改进的时候可以修正或增加执行程序组件。
*   5.性能 -- 异步IO，多进程/多线程并行，
*
* Windows被设计成为软件分层的体系结构:
*   应用程序
*   子系统 -- SubSystem: Win32, native, OS/2, POSIX, WOW(Windows On Windows), 
*             VDW((Virtual DOS Machine) 等
*   Native API																				用户模式
*   ----------------------------------------------------------------------------------------------------
*   系统服务函数																			内核模式
*   执行体组件(IO管理器,对象管理器,进程管理器,虚拟内存管理器,配置管理器,其他组件等) + 驱动程序
*   内核(主要负责进程、线程的调度，多CPU同步支持，提供中断处理等)
*   HAL -- 硬件抽象层         
*   硬件层 -- 
* 
* 环境子系统 -- 向各自的应用程序提供相应的API支持，比如 WinXp 上运行Dos等程序
* Native API 穿过了用户模式和内核模式的界面，达到了内核模式
* 系统服务函数通过IO管理器将消息传递给驱动程序
******************************************************************************************************************/

#endif //F_DRIVER_STUDY_H