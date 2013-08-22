#ifndef F_DRIVER_HOOK_API_H
#define F_DRIVER_HOOK_API_H
#pragma once

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif

//[原创开源]在WIN7 X64上Hook Shadow SSDT  --  http://www.m5home.com/bbs/thread-6963-1-1.html
//实现 Win64 上的内核级Inline Hook引擎 --  http://www.jybase.net/biancheng/20120429881.html

/******************************************************************************************************************
调用顺序
  KiFastCallEntry  -> SSDT[ServiceIndex] -> 
    ServiceIndex大于 0x1000 则用第二张表(Shadow SSDT)


系统中的函数
  nt!KeAddSystemServiceTable -- 系统的导出函数
系统中的变量, 
  nt!KeServiceDescriptorTable			-- fffff800`02ac5840
  nt!KeServiceDescriptorTableShadow	-- fffff800`02ac5880
  nt!KiServiceTable					-- fffff800`0288fb00, 即 nt!KeServiceDescriptorTable->ServiceTableBase(SSDT)
  win32k!W32pServiceTable				-- fffff960`001c1c00, 即 nt!KeServiceDescriptorTableShadow->ServiceTableBase[1](Shadow SSDT)

1: kd> dp nt!KeServiceDescriptorTableShadow
fffff800`02ac5880  fffff800`0288fb00 00000000`00000000
fffff800`02ac5890  00000000`00000191 fffff800`0289078c
fffff800`02ac58a0  fffff960`001c1c00 00000000`00000000
fffff800`02ac58b0  00000000`0000033b fffff960`001c391c

1: kd> ?? pServiceTable   -- (即通过类型显示 nt!KeServiceDescriptorTableShadow::ntoskrnl )
struct _SYSTEM_SERVICE_TABLE * 0xfffff800`02ac5880
+0x000 ServiceTableBase : 0xfffff800`0288fb00  -> 0x02f6f000`04106900 Void
+0x008 CounterTable     : (null) 
+0x010 NumberOfServices : 0x191
+0x018 ArgumentsTable   : 0xfffff800`0289078c  ""

1: kd> ?? &pServiceTable[1] -- (即通过类型显示 nt!KeServiceDescriptorTableShadow::win32k )
struct _SYSTEM_SERVICE_TABLE * 0xfffff800`02ac58a0
+0x000 ServiceTableBase : 0xfffff960`001c1c00  -> 0xfff0a301`fff39800 Void
+0x008 CounterTable     : (null) 
+0x010 NumberOfServices : 0x33b
+0x018 ArgumentsTable   : 0xfffff960`001c391c  ""

0: kd> dv
pServiceTable = 0xfffff800`02ac5880
nIndex = 0n8
newAddress = 0xfffff880`02ab6980
status = 0n0
W32pServiceTable = 0xfffff960`001c1c00
i = 0n0
qwTemp = 0
pWin32k = 0xfffff800`02ac58a0
dwTemp = 0n0

Shadow SSDT 中的地址在Win64下是相对地址(3.5 个字节，可表示的范围有限)，因此不能用交换地址的方式直接替换
TODO：最后一位有可能不是0，具体的用处是什么？

0: kd> dd fffff960`001c1c00	-- W32pServiceTable
fffff960`001c1c00  fff39800 fff0a301 000206c0 00101d00		1~4
fffff960`001c1c10  00095ac0 00022700 fff99a00 ffddfec3		5~8
fffff960`001c1c20  0003ac47 00fbb500 ffed1d80 ffe4f980		9~12
fffff960`001c1c30  000c5380 000af0c0 000e8700 fffeb5c0
fffff960`001c1c40  ffb1d500 0004e780 ffa561c0 000b9000
fffff960`001c1c50  000b1fc0 000fbd40 00037b00 000b3900
fffff960`001c1c60  ffb212c0 000b5380 000a0f00 ffb2a580
fffff960`001c1c70  ffa2a543 ffa83800 0012aac0 00093c00

0xfffff960`001c1c00 + 依次的数字>;4
Address of 1 is fffff960`001b2630 -- win32k!NtUserPeekMessage    	-- 001c1c00 + Ffff0a30   -- 1
Address of 2 is fffff960`001c3c6c -- win32k!NtUserCallOneParam 		-- 001c1c00 + 0000206c   -- 2
Address of 3 is fffff960`001d1dd0 -- win32k!NtUserGetKeyState		-- 001c1c00 + 000101d0   -- 3
Address of 4 is fffff960`001cb1ac -- win32k!NtUserInvalidateRect	-- 001c1c00 + 000095ac   -- 4
Address of 5 is fffff960`001c3e70 -- win32k!NtUserCallNoParam		-- 001c1c00 + 00002270	 -- 5
Address of 6 is fffff960`001bb5a0 -- win32k!NtUserGetMessage		-- 001c1c00 + Ffff99a0	 -- 6
Address of 7 is fffff960`0019fbec -- win32k!NtUserMessageCall		-- 001c1c00 + Fffddfec	 -- 7
Address of 8 is fffff960`001c56c4 -- win32k!NtGdiBitBlt			-- 001c1c00 + 00003ac4	 -- 8
Address of 9 is fffff960`002bd750 -- win32k!NtGdiGetCharSet		-- 001c1c00 + 000fbb50	 -- 9
******************************************************************************************************************/

//#include "WindowsTypes.h"

//注意：2个driver先后hook ssdt并且如果先后unload通常是灾难之源 -- 如果发现地址不在原来的位置(win32k.sys?)的话，不进行hook？

// nt!_KTHREAD -- X64下有这个全局变量(什么东西？线程? )，里面可以找到ServiceTable ？


//Tommy 的专栏 -- http://blog.csdn.net/whf727/article/details/4986575
//KeServiceDescriptorTable 结构及修改内存保护写的方法 --   http://www.3600safe.com/?post=94

//bf84d8de  win32k!FastWindowFromDC 
//bf8c30bc  win32k!_WindowFromDC 

//到SSDT Shadow表中的函数定义和参数说明 -- WRK(2003的源码)
//  WRK(Windows Research Kernel) -- 微软为高校操作系统课程提供的可修改和跟踪的操作系统教学平台。
//  给出了Windows这个成功的商业操作系统的内核大部分代码，可以对其进行修改、编译，并且可以用这个内核启动Windows操作系统。
//  检查导入表符号的小工具 -- 静态分析PE文件的导入表，挨个对导入表中的符号，去导入文件的导出表中去搜索(参考WRK)

//WinDbg 中循环查找SSDT的函数对应表 -- 需要进入Hook部分，并 load 相关的pdb文件才能查找
// 32 Bit(WinXP)
//   SSDT -- .for (r eax=0, edx=5; @eax <= @edx; reax=eax+1){? eax; ln (dwo(nt!KiServiceTable + 4 * eax)) }
//   Shadow SSDT -- .for (r eax=0, edx=5; @eax <= @edx; reax=eax+1){? eax; ln (dwo(win32k!W32pServiceTable + 4 * eax)) }
// 64 Bit(Win7)
//   SSDT -- 
//   Shadow SSDT -- .for (r eax=0, edx=5; @eax <= @edx; reax=eax+1){? eax; r ebx=dwo(win32k!W32pServiceTable+4*eax); .if(ebx & 0x80000000) { r ebx=((ebx>>>4)+0xF0000000); r rcx=0x100000000; } .else { r ebx=ebx>>>4; r rcx=0x0; }; ln win32k!W32pServiceTable+ebx-rcx }

//问题：给出来的表示错误的
//  x64里用windbg查看SSDT/Shadow SSDT -- http://hi.baidu.com/ithurricane/item/4cabc91964d1460de75c3634
//     查看SSDT函数的方法(Win7 x64测试 OK) -- ln (dwo(nt!KiServiceTable + 4*index)>>4) + nt!KiServiceTable
//     查看Shadow SSDT的方法(Win7 x64测试NG -- 3~5有问题 ) -- ln win32k!W32pServiceTable+((poi(win32k!W32pServiceTable+4*index)&0x00000000`ffffffff)>>4)-10000000
//       .for (r eax=0, edx=5; @eax <= @edx; reax=eax+1){? eax; ln (dwo(win32k!W32pServiceTable + 4*eax)>>4) + win32k!W32pServiceTable }


//  获取函数地址的公式是：dwo(nt!KiServiceTable+n)+nt!KiServiceTable（n=0,1,2…）。
//  http://bbs.dbgtech.net/forum.php?mod=viewthread&tid=360


//Win7 64位对于未有认证签名的驱动程序进行了限制安装 -- 启动时F8后可以选择“禁用驱动程序签名强制”或通过bcdedit更改
// on x86,we use Hook shadowSSDT; on x64,we use inline hook in user-mode;

//TODO: Depends 查看 ntkrnlpa.exe + win32k.sys 并比较索引值
//SoftICE for Windows NT/2000的ntcall命令可以将这些System Service显示出 -- http://dev.21tx.com/2005/03/14/12553.html
//读取硬盘上的ntoskrnl.exe文件，根据export table定位原始的SSDT，再用物理内存与虚拟内存映射的办法转换，得到系统中SSDT的位置

/******************************************************************************************************************
* 在Windows操作系统中，消息钩子（通过SetWindowsHookEx设置）只会当前的桌面上的窗口有效，
* 所以可以建立一个安全桌面，用于运行需要严密保护的进程，这样非本桌面上运行的程序无法通过消息钩子的方式来
* 获取需要保护进程窗口的信息，达到了保护目标进程窗口的目的(360保险箱和金山密保 的 安全桌面功能 )
******************************************************************************************************************/

/******************************************************************************************************************
* PatchGuard -- 64位系统中的内核补丁保护程序, 有应用或驱动尝试修改核心组件，就会产生 CRITICAL_STRUCTURE_CORRUPTION(0x109)蓝屏错误
*   保护下列组件
*     系统模块 (ntoskrnl.exe、ndis.sys、hal.dll)
*     系统服务分派表 (SSDT, System Service Dispatch Table -- KeServiceDescriptorTable)
*     全局描述符表 (GDT -- Global Descriptor Table)
*     中断描述符表 (IDT -- Interrupt Descriptor Table)
*     Processor MSRs (syscall)
*     使用不是由内核分配的内核堆栈(Using kernel stacks that are not allocated by the kernel)
*     修补内核的任何部分	 -- Patching any part of the kernel (detected only on AMD64-based systems) 
*     TODO：不检查 inline hook 和 Shadow SSDT ?
* 
*  禁用PatchGuard -- 进行双机调试且在被调试机启动时就挂载上内核调试器，PatchGuard就不会被启用。
*
*   PatchGuard可能是所有操作系统中第一个验证内核镜像、内存及 MSR并检查系统是否受到损害的组件。
*     为防止第三方内核模块禁用内核补丁保护，其初始化和操作模式已高度模糊化并随机化，试图分析 PatchGuard将是徒劳无功的。
*   PatchGuard 原理 -- 
*     1.初始化(使用了很多的错误引导迷惑对此好奇的人士)
*     	if (KiDivide6432(KiTestDividend, 0xCB5FA3) != 0x5EE0B7E5){ KeBugCheck(UNSUPPORTED_PROCESSOR); }
*     2.实际运行时，会产生除法溢出错误异常，由 KiDivideErrorFault 继续处理， 启动PatchGuard的初始化过程。
*     3.选定随机化的池标签及数位大小并分配内存，然后使用随机密钥对 PatchGuard 进行加密并复制到该内存
******************************************************************************************************************/

//  http://www.microsoft.com/whdc/driver/kernel/64bitPatching.mspx
//  http://uninformed.org/index.cgi?v=8&a=5
//  http://www.mcafee.com/cn/resources/reports/rp-defeating-patchguard.pdf
//  DEP(基于硬件的数据执行保护) -- 

/******************************************************************************************************************
* 驱动级的 HOOK API( 通过更改 SSDP 中的函数地址来 Hook) -- 
*   注意先看该作者的其他下载。http://download.csdn.net/detail/jyw1111/5275135
*   缺点：可能被反病毒软件作为 rootkit 提醒
*
* 软中断：
*   Win2K -- int 2eh
*   WinXp -- sysenter
*
* Native API：
*   ntdll.dll -- Native API 的接口，通过软中断方式将调用传到内核来调用系统服务，但本身属于Ring 3。
*     函数名一般是对应Win32API前加 Nt 两个字母。
*     1.把对应的功能编号存入eax寄存器,
*     2.使用 SYSENTER 指令(早期使用触发 Int 2e 中断的方式)引发"自陷"
*     3.KiSystemService 内核处理程序运行
*     4.在SDT里找到功能编号的对应函数位置，并调用
*       SDT在系统中一共有4个()：
*       0.SSDT -- 位于 ntoskrnl.exe 或 ntkrnlpa.exe 中，记录了系统关键功能的位置,涉及文件操作,注册表操作,进程操作等。TODO：为什么有两个？区别？
*       1.Shadow SSDT -- 位于 win32k.sys 中， 记录了和用户与图形界面有关的功能。
*       2.3 -- 预留的SDT，即有人工创造SDT的可能，不过基本上没有意义
*     5.Object Hook -- 更底层的接口，不过已经很难控制了
*
* ntkrnlpa.exe 
* ntkrnlmp.exe (SSDT?) -- Kernel32.Dll 的内核实现
* ntoskrnl.exe -- Windows执行体组件
* win32k.sys (Shadow SSDT?) -- User32.dll + Gdi32.DLL 的内核实现
*   将GUI的实现放入内核模式，会增大系统不稳定的几率。但会大大提高图形处理的运行效率。
*
* KeUserModeCallBack -- 从R0调用位于Ring3的函数(系统所有的消息钩子回调都是利用该函数完成的)
* 
* http://bbs.pediy.com/showthread.php?t=149861&highlight=NtGdiBitBlt
* NtGdiBitBlt / NtGdiAlphaBlend / NtGdiCancelDC / NtGdiColorCorrectPalette / NtGdiConsoleTextOut / NtGdiCreateColorSpace /NtGdiCreateColorTransform
* NtGdiGetDCPoint / NtGdiGetDCObject
*
 





NtGdiTransparentBlt
NtGdiCreateDIBSection
NtGdiAlphaBlend
NtGdiGetPixel
NtUserSendInput
NtOpenSection
NtGdiBitBlt，NtGdiMaskBlt，NtGdiPlgBlt，NtGdiStretchBlt。NtUserBuildHwndList，NtUserFindWindowEx
******************************************************************************************************************/

/******************************************************************************************************************
*      System Services Descriptor Table
* SSDT(System Service Descriptor Table) -- 系统服务描述表，是各个系统服务号的 索引 => 对应系统服务函数的地址。
*   把Ring3的Win32 API和Ring0的内核API联系起来。
*   通过修改此表的函数地址可以对常用windows函数及API进行hook，从而实现对一些关心的系统动作进行过滤、监控的目的.
*   TODO: 怎么得到一个函数的SSDT索引号(dumpbin 或 depends ?)
*         http://www.cnblogs.com/nlsoft/archive/2013/04/02/2994628.html
* Shadow SSDT(Shadow System Services Descriptor Table) -- 管图形、用户相关的函数(gdi32.dll,user32.dll)
*   注意：
*   1.win32k.sys不是常在内存的，如果不是GUI线程，Shadow SSDT 地址无效 -- 在GUI线程中DeviceIoControl即可;
*   2.通常需要附加的进程是 csrss.exe ? GetCsrPid()
*   3.使用MDL映射一块不分页内存，设置成可以写入，常驻在物理内存(参见 RegmonMapServiceTable)
*     TODO:通过 #pragma LOCKEDCODE 声明变量即可？
*   4.GDI的很多函数会操作 _pGdiSharedHandleTable 表?
*
* KeServiceDescriptorTable -- 系统预定义的保存SSDT信息的变量，可通过extern后访问，如 WinDbg 中: dd KeServiceDescriptorTable
* KeServiceDescriptorTableShadow -- 为了做 Shadow SSDT Hook 而定义的，系统中不存在，可以通过 KeServiceDescriptorTable 来定位。
* 

* 对SSDT进行Hook的过程
*   1.关闭CR0写保护(改变CR0寄存器的WP位)；
*   2.1.Hook Shadow SSDT 之前，必须将进程上下文切换到CSRSS进程
*   2.2.用新的函数地址替换原来SSDT中的函数地址
*   3.恢复CR0写保护
* 
* Shadow SSDT 中函数名定位：
*   1.可以解析pdb得到，但比较麻烦(已有 获取shadow地址和函数名称的工具 )
*     SymInitialize -> SymSetSearchPath -> SymLoadModule -> SymGetSymFromName
*   2.同一个版本的系统调用号一样，经过测试后可以直接写死
*     
* Hook 检查(避免和别人的Hook冲突或安全检查)
*   1.查看 OldFunc 的函数地址是否在win32k的模块里
* 
* 窗口保护 NtUserFindWindowEx、NtUserGetForegroundWindow、NtUserBuildHwndList、NtUserQueryWindow、
*          NtUserWindowFromPoint、NtUserSetParent、NtUserPostMessage、NtUserMessageCall、
*          NtUserSetWindowLong、NtUserShowWindow、NtUserDestroyWindow、NtUserCallHwndParamLock
* 模拟按键 NtUserSendInput
* 获取键盘按键状态 NtUserGetAsyncKeyState
* 打开安全桌面 NtUserOpenDesktop
* 将虚假按键还原成真实的按键 NtUserTranslateMessage
* 截屏保护 NtGdiStretchBlt
******************************************************************************************************************/

//http://drmemory.googlecode.com/svn/trunk/drmemory/syscall_usercallx.h
/******************************************************************************************************************
*             Ring3(kernel32)                    |   Native API(ntdll!)     |       Ring0(nt!)          |
* ------------------------------------------- SSDT (WinXP 284) -----------------------------------------|
* OpenProcess                                    | NtOpenProcess            | NtOpenProcess             |
* NtDuplicateObject                              |                          |                           |
* CreateThread/CreateRemoteThread                | NtCreateThread           | CreateThread              |
* OpenThread                                     |                          | NtOpenThread              |
* WriteProcessMemory                             | NtWriteVirtualMemory     | NtWriteVirtualMemory      |

* ------------------------------------- Shadow SSDT (WinXP 667)-----------------------------------------|
* BitBlt                                         |                          | NtGdiBitBlt               |
* CreateDC										 |                          | NtGdiOpenDCW              |
* DeleteDC                                       |                          | NtGdiDeleteObjectApp      |
* DestroyWindow                                  |                          | NtUserDestroyWindow       |
* EnableWindow                                   |                          | NtUserCallHwndParamLock   |
* EnumWindows                                    |                          | NtUserBuildHwndList       |
* FindWindow                                     |                          | NtUserFindWindowEx        |
* GetForegroundWindow                            |                          | NtUserGetForegroundWindow |
* GetWindowThreadProcessId                       |                          | NtUserQueryWindow         |
* PostMessage                                    |                          | NtUserPostMessage         |
* SendMessage                                    |                          | NtUserMessageCall         |
* SetParent                                      |                          | NtUserSetParent           |
* SetWindowLong                                  |                          | NtUserSetWindowLong       |
* ShowWindow                                     |                          | NtUserShowWindow          |
* StretchBlt                                     |                          | NtGdiStretchBlt           | 
* WindowFromDC                                   |                          | NtUserCallOneParam        |
* WindowFromPoint                                |                          | NtUserWindowFromPoint     |
*                                                |                          | NtUserCallTwoParam        |
******************************************************************************************************************/
//#define TWOPARAM_ROUTINE_SETDCBRUSHCOLOR    0xfffd0046
//#define NtUserSetDCBrushColor(hbr, crColor) (COLORREF)NtUserCallTwoParam((DWORD)(hbr), (DWORD)crColor, TWOPARAM_ROUTINE_SETDCBRUSHCOLOR)

#endif //F_DRIVER_HOOK_API_H

