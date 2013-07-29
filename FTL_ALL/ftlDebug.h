#ifndef FTL_DEBUG_H
#define FTL_DEBUG_H

#pragma once

namespace FTL
{
    //WinDBG用法详解.pdf -- P19,  30.5.7 远程调试
    /*********************************************************************************************************
    * TODO: 
    *  1.双击内核调试 和 本地内核调试
    *  2.alias -- 
    *  3.通过启动内核调试对话框设置内核调试连接设置
    *  4.命令中的注释： * 或 $$ -- 使用前应该在前一条命令后加上分号作为分隔
    *  5.poi 是什么？ "j (poi(MyVar)>5) ' -- J 命令判断变量MyVar 的值是否大于5，
	*  6.当调试用户态的转储文件时，可以使用.ecxr 命令将转储文件中保存的异常上下文设置为寄存器上下文。
	*  7.VC 编译器缺省将类型符号放在VCx0.PDB 文件中，WinDBG不能很好的处理，因此显示局部变量时，会显示很多no type information 错误。
	*    解决方法：将符号格式设置为C7 Compatable（Settings>C++ > General > Debug Info）
	*  8.设置环境变量_NT_SYMBOL_PATH 和_NT_ALT_SYMBOL_PATH
	*  9.WinDBG 缺省使用所谓的懒惰式符号加载策略，模块加载时不会加载符号，显示为 deferred(延迟加载)，
	*    通过 .reload 强制全部加载或 .load 手动加载 ？
    *********************************************************************************************************/

    /*********************************************************************************************************
    * OllyDbg -- 强大的代码分析能力，无符号和源码时也能很好的进行调试(所以常用于软件破解、病毒分析等逆向工程)
    * WinDbg  -- 反汇编分析能力较弱，GUI界面偏弱，无源码时调试比较困难；支持dump调试，分析功能强大
    *            通过 SOS.dll 支持.NET调试，进行高级调试比较方便
    * 
	* WinDbg(http://msdn.microsoft.com/en-us/windows/hardware/gg463009)
    *   支持脚本和插件，而且软件包本身提供了大量非常有用的插件，可编写调试扩展模块来定制和补充调试功能
    *   .hh 关键字 -- 打开帮助文档并定位到指定关键字
	*
    * 工作空间(Workspace) -- 描述和存储一个调试项目的属性、参数、以及调试器设置等信息，类似IDE中的项目文件。
    *   配置信息保存在 HKCU\Software\Microsoft\Windbg\Workspaces，一般包含四个子键，分别对应四类不同的工作空间。
    *     User(用户态)
    *     Kernel(内核态)
    *     Dump(转储文件)
    *     Explicit(用户另存为的显式工作空间)
    *
    * 主题(Themes) -- WinDbg程序目录中的Themes子目录中包含了四种经过定制的工作空间设置(主要是窗口布局)。可通过将.reg导入注册表后使用。
    * 别名(Alias) -- 可用于简写命令或编写命令程序等？
    *   分三类(可通过 .echo 查看)：
    *     固定名称别名( $u0 ~ $u9 ) -- 修改方式: r $.u<0~9>=<别名实体>， 如 r $.u9=nt!KiServiceTable
    *     WinDBG 自动定义的别名 -- 
    *       $ntnsym(NT 内核或者NT DLL的符号名), 
    *       $ntwsym(在64位系统上调试32位目标时的NT系统DLL符号名), 
    *       $ntsym(与当前调试目标的机器模式匹配的NT 模块名称)
    *     用户命名别名 -- 修改方式：as 别名名称 别名实体。 引用方式：${用户别名} 或 直接引用
    * 日志文件(LogFile) -- 可以把输入的命令和命令的执行结果记录一个文本文件(日志文件)中。
    * 上下文:
	*   会话(Session)上下文 -- 只在内核调试时才有意义, !session 扩展命令, -s n 设置, !sprocess -- 列出某个会话的所有进程
	*   进程(Process)上下文 -- 通过 .process 显示和切换 ?
	*   寄存器(Register)上下文 -- 寄存器中存放的是当前正在执行线程的寄存器值，通过 .thread 显示或者设置寄存器上下文所针对的线程
    *   局部(Local)上下文 -- 局部变量所基于的语境，涉及到堆栈
	*  
    * 有三类命令(在命令编辑框中输入 "." 或 "!" 然后按 Tab，可以顺序显示各种可用的命令 )：
    *   标准命令 -- 一两个字符或者符号，用来提供适用于各种调试目标的最基本调试功能
    *   元命令(Meta-Command，以点"."开始) -- 提供标准命令没有提供的调试功能
    *   扩展命令(Extension Command，以感叹号"!"开始) -- 用于扩展某一方面的调试功能，是实现在动态加载的扩展模块(DLL)文件中的，
    *     在如 w2kchk/w2kfre/winext/winxp 等各个子目录中，可通过其SDK可编写自己的扩展模块。
    *     当调试目标被激活时，WinDBG 会根据调试目标的类型和当前的工作空间，自动加载某些扩展模块。也可通过 .load 元命令来加载
    * 
    * 常用命令(在中断状态下，通过F9键可以设置/取消断点), 直接输入问号 "?" 可以显示出一个标准命令的列表和简单介绍
    *   ~ -- 显示和切换线程
    *     ~<thread_index> s -- 切换当前线程， 如 "~6 s" 切换到第6号线程
    *   | -- 显示进程
    *     |<process_index> s -- 切换当前系统中的当前进程
    *     ||<n> s -- 将当前系统切换到<n>号，在同时调试多个系统(如 本地、远程、dump等)时使用
    *   ? -- 显示表达式
    *   ?? -- 显示C++表达式
    *   $ -- 执行命令文件
    *   a -- 汇编
    *   al -- 列出目前定义的所有用户命名别名
    *   ad -- 删除指定或全部(ad *) 用户别名
    *   as 别名名称 别名实体 -- 定义或者修改用户命名别名，如 as SST nt!KiServiceTable
    *   b -- 断点(F9 即是在光标所在源码行设定断点？)。bu 设置的断点会保存在Workspace中?
    *     ba -- 硬件断点
    *     bc/bd/be -- 清除/ 禁用/启用  断点，如 bc 1 或 bc *
    *     bl -- 列出所有断点
	*     bu -- 对没有加载符号表的模块使用延迟设置断点，常用于调试动态加载模块的入口函数或者初始化代码。
	*        如 bu HelloWDM!DriveEntry
    *     bp [Options] [Address [Passes]] ["CommandString"] -- 在指定函数名或地址处设置断点，
	*        如 bp HelloWDM!DriveEntry 
	*           bp MSVCR80D!printf+3 2 "kv;da poi(ebp+8)" -- 对printf 偏移3的地址设置断点，第二次时才中断，并自动执行指定命令。
	*              da poi(ebp+8)用来显示printf 的第一个参数所指定的字符串。设置偏移3的原因：入口处的栈帧建立代码执行好后，ebp+8才能指向第一个参数
	*           bp my.cpp:122 100
    *   dds 地址 -- 可显示指定位置的函数名?
    *   d 观察内存数据, 可通过 ln 指定长度为n, 
    *     dd -- 按照 DWORD 的方式显示？ 类似的有 du(Unicode Char?)
    *     dg -- 显示段选择子
    *     dt Xxxx -- 显示变量内容？ 如 dt _EPROCESS
    *     dv -- 显示函数参数和局部变量的信息(Ctrl+Alt+V 切换到更详细的显示模式)，注意：优化过的代码显示可能不准确。
    *   e -- 编辑内存数据
    *   g -- 恢复运行
	*     gu -- 执行到本函数返回
    *   j <条件表达式> [Command1>] ; [Command2>] -- 判断一个条件，然后选择性的执行后面的命令，类似 if..else，可用单引号包含一组命令
    *     例： r ecx; j (ecx<2) 'r ecx';'r eax' 表示先显示寄存器ecx 的值，然后执行j命令判断ecx是否小于2并执行不同部分。
    *          等价于：r ecx; .if (ecx>2) {r ecx} .else {r eax}
    *   k -- 观察调用堆栈
    *     k/kb/kn -- 打印调用堆栈， k* 显示当前线程的堆栈; ~*kb 显示所有线程的堆栈
	*   ld 模块 -- 加载指定模块的调试符号(可加载的模块名是通过 lm 能看到的)
    *   lm -- 列出当前加载的符号表，可查看是否正确加载了需要调试的程序、驱动的符号表。若有 M 标记，表示符号文件和执行映像文件存在不匹配
	*     lm v -- 可以显示详细信息
	*     lm e -- 只显示有符号问题的模块
    *     lmm -- list module match
    *   ln -- 搜索距离指定地址最近的符号
    *   p -- step over，单步执行完函数调用
	*     pa -- 单步到指定地址，不进入子函数
	*     pc -- 单步执行到下一个函数调用(Call 指令)
    *   q -- 结束调试会话
    *   r -- 观察和修改寄存器，如 r ecx=2 表示将ecx设置为2
    *   s -- 搜索内存数据
    *   sx -- 设置调试事件处理方式
    *   t -- trace into，单步如遇到函数调用，进入被调用函数
	*     ta -- 追踪到指定地址，进入子函数
	*     tc -- 追踪执行到下一个函数调用(Call 指令)
    *   u XXXXXXXX -- 反汇编指定的地址(不需要 0x 前缀 ?)， 如 lkd> u KeAddSystemServiceTable l 40
    *   version -- 显示版本和已经加载的扩展模块等信息
	*   wt -- 跟踪执行并生成一份报告(★调用分析的利器★?)
    *   x -- 分析调试符号，如 x /a win32k!NtUser* 列出win32k模块中所有以 NtUser 开头的符号并按地址升序排序。
	*     /t -- 显示符号的数据类型。 /v -- 显示符号的符号类型和大小
    *   
    * 元命令 -- 以一个点(.)开始
    *   .abandon -- 放弃用户态调试目标进程
    *   .asm -- 反汇编选项
    *   .attach -- 附加到指定进程
    *   .chain -- 列出当前加载的所有扩展模块
    *   .childdbg -- 控制子进程的调试，如果同时调试几个进程，可以使用 "|" 命令来切换
	*   .context -- 设置或者显示用来翻译用户态地址的页目录基地址(Base of Page Directory)
    *   .create -- 创建新进程
    *   .dbgdbg -- 启动另一个调试器来调试当前调试器
    *   .detach -- 分离调试目标
    *   .dump -- 显示或者转储调试目标数据
    *      .dump /ma Dump文件名   -- 手动生成Dump文件
    *   .echo -- 显示 别名(alias) 等的信息
    *   .endpsrv -- 结束远程进程服务器
    *   .endsrv -- 结束引擎服务器
    *   .exepath -- 可执行文件
    *   .expr -- 控制表达式评估器
    *   .extpath -- 扩展命令模块路径
    *   .extmatch -- 匹配扩展命令
	*   .frame -- 观察当前的局部上下文
    *   .fromats -- 以不同格式显示数据
    *   .help -- 列出所有元命令和每个命令的简单说明
    *   .kill -- 杀掉进程
    *   .ld -- 手动加载符号表，如 .ld ????
    *   .load 模块完整路径 -- 加载扩展命令模块
    *   .loadby 模块名称 -- WinDBG 会自动到当前配置中定义的扩展模块所有路径中搜索匹配的模块并加载
    *   .logfile -- 显示调试器日志文件
    *     .logopen(打开)/.logappend(追加)/.logclose(关闭)
    *   .lsrcpath -- 远程 源码路径
    *   .opendump -- 打开转储文件
	*   .process [EPROCESS] -- 显示、切换进程上下文， 使用 !process 0 0 列出系统中的所有进程的基本信息
    *   .reload -- 重新加载符号表， /i 允许加载不严格匹配的符号文件
    *   .remote -- 启动 remote.exe 支持远程调试
    *   .restart -- 重新开始调试会话
    *   .send_file -- 向远程服务器发送文件
    *   .server -- 启动引擎服务器
    *   .servers -- 列出可用服务器
    *   .srcpath/.srcnoise/.srcfix -- 设置本地源码路径(如果是本机编译调试的话不用设置)
	*   .suspend_ui -- 暂时停止刷新信息窗口，防止界面更新太频繁
    *   .symopt -- 显示和修改符号选项。使用一个32位的DWORD 来记录符号选项，每位代表一个选项，+号设置,-号取消
    *   .sympath -- 可以增加、修改、或者显示符号搜索路径(本地磁盘目录 + 符号服务器，多个位置之间用分号分隔)
	*   .symfix -- 设置符号服务器
	*   .thread [ETHREAD] -- 
    *   .time -- 显示调试会话时间
    *   .tlist -- 显示任务列表(task list)
    *   .ttime -- 显示线程时间
    *   .unload/.unloadall -- 卸载扩展命令模块
    *   .writemem -- 将原始内存数据写到文件
    *   编写命令程序(.if、.else、.elsif、.foreach、.do、.while、.continue、.catch、.break、.continue、.leave、.printf、.block 等)  
    * 
    * 扩展命令 -- 以感叹号(!)开始的命令，语法为： ![扩展模块名.]<扩展命令名> [参数]
    *     TODO: !for_each_XXXX 命令循环执行
    *   acpikd.dll -- 用于ACPI 调试，追踪调用ASL 程序的过程，显示ACPI 对象
    *   ext.dll -- 适用于各种调试目标的常用扩展命令
    *   exts.dll -- 关于堆(!heap)、进程/ 线程结构(!teb/!peb)、安全信息(!token、!sid、!acl)、和应用程序验证(!avrf)等的扩展命令。
    *   fltkd.dll -- 用于调试文件系统的过滤驱动程序(FsFilter)
    *   kdexts.dll -- 包含了大量用于内核调试的扩展命令
    *   kext.dll -- 内核态调试时的常用扩展命令
    *   ks.dll -- 用于调试内核流(Kernel Stream)
    *   logexts.dll -- 用于监视和记录API 调用(Windows API Logging Extensions)
    *   minipkd.dll -- 用于调试AIC78xx 小端口(miniport)驱动程序。
    *   ndiskd.dll -- 用于调试网络有关驱动程序
    *   ntsdexts.dll -- 实现了!handle、!locks、!dp、!dreg(显示注册表)等命令
    *   rpcexts.dll -- 用于RPC 调试
    *   scsikd.dll -- 用于调试SCSI 有关的驱动程序
    *   sos.dll -- 用于调试托管代码和.Net 程序
    *   traceprt.dll -- 用于格式化 ETW 信息
    *   uext.dll -- 用户态调试时的常用扩展命令
    *   vdmexts.dll -- 调试运行在 VDM 中的 DOS 程序和 WOW 程序
    *   wdfkd.dll -- 调试使用WDF(Windows Driver Foundation)编写的驱动程序
    *   wmitrace.dll -- 显示 WMI 追踪有关的数据结构、缓冲区和日志文件
    *   wow64exts.dll -- 调试运行在64 位Windows 系统中的32 位程序
    *   WudfExt.dll -- 用于调试使用UMDF 编写的用户态驱动程序
    * 
    * 伪寄存器(使用方式 @伪寄存器名, 如 ln @$exentry 或 ? @$pagesize)
    *   $csp -- 帧指针。x86 中即ESP 寄存器，x64 是RSP，安腾为BSP。
    *   $dbgtime -- 当前时间
    *   $ea -- 调试目标所执行上一条指令的有效地址(effective address)
    *   $exentry -- 当前进程的入口地址
    *   $exp -- 表达式评估器所评估的上一条表达式
    *   $ip -- 指令指针寄存器。x86 中即EIP，x64 即eip
    *   $pagesize -- 调试目标所在系统的内存页字节数
    *   $peb -- 当前进程的进程环境块(PEB)的地址
    *   $ptrsize -- 调试目标所在系统的指针类型宽度
    *   $proc -- 当前进程的EPROCESS 结构的地址
    *   $ra -- 当前函数的返回地址(retrun address)
    *   $retreg -- 首要的函数返回值寄存器。x86 架构使用的是EAX，x64 是RAX，安腾是ret0
    *   $retreg64 -- 64 位格式的首要函数返回值寄存器，x86 中是edx:eax 寄存器对
    *   $teb -- 当前线程的线程环境块(TEB)的地址
    *   $thread -- 当前线程的ETHREAD 结构的地址
    *   $tid -- 当前线程的线程ID
    *   $tpid -- 拥有当前线程的进程的进程ID(PID)
    * 
    * 远程调试 -- 通过和远程工具、转发器配合，实现各种灵活的远程调试方式，以支持不同的网络环境
    *   1.DbgSvr/KdSvr + remote.exe ?
    *   2.服务端，使用名为advdbg的命名管道： windbg –server npipe:pipe=advdbg 或 执行 .server npipe:pipe=advdbg 命令
	*     客户端，连接该命名管道：windbg -remote npipe:server=FJPC,pipe=advdbg 或 "Connect to Remote Session" 并输入 npipe:Pipe=advdbg,Server=FJPC
	*
    * 命令行参数
    *   -k -- 启动内核(Kernel)调试， -kl 是本地内核调试
	*   -pe PID -- 连接一个被Detach或僵死的调试进程，和 -p 的区别在于可以连接"DebugPort不为空"的进程
    *   -z -- 指定转储文件
    *
    * 调试技巧
    *   1.在 Watch 窗口中输入 "@err,hr" ，可以监视 API 的是否错误 及错误原因 -- GetLastError
    *   2.使用分号（;）号作为分隔符，可以在同一行输入多条命令。
    *   3.使用 Ctrl+Break 来终止一个长时间未完成的命令
    *   4.使用Windbg显示系统句柄信息(显示Session1 中的 WinSta0): lkd>!object Sessions\1\Windows\WindowStations\WinSta0 
    *   5.调试内核目标 -- 
    *
    * WinDbg调试dmp文件(可以下载并加载不匹配的符号 -- VS2008不能下载)
	*   1.设置符号路径 SRV*E:\OSSymbols*http://msdl.microsoft.com/download/symbols
	*   2.打开dmp文件
	*   3.!sym noisy  -- 相当于.symopt+0x80000000，即开启所谓的“吵杂”式符号加载，显示符号加载的调试信息。
	*   4.!analyze -v
	*********************************************************************************************************/

    /*********************************************************************************************************
    * 在Release版本中设置断点: _asm int 3
	*   
	* 使用 TRACE/ATLTRACE 打印带有中日韩的文字时，可能会报" _CrtDbgReport: String too long or IO Error "的错误，而无法打出日志：
	*   原因: wprintf_s 不能正确输出中日韩的 UNICODE 文字(似乎VS2010后修复了这个Bug？)
	*   补充资料(尚未仔细研究)：http://www.cnblogs.com/hnrainll/archive/2011/05/07/2039700.html
	*   解决方法：
	*     1.使用FTLTRACE(内部使用 StringCchVPrintfEx + OutputDebugString )
	*     2.#include <locale.h>
	*       TCHAR* old_locale = _tsetlocale( setlocale(LC_ALL,NULL) );
	*       _tsetlocale( LC_ALL, _T("chs") );    或 "", "korean"
	*       TRACE(XXXX);
	*       _tsetlocale( LC_ALL, old_locale); 
	*       free(old_locale); 
    *********************************************************************************************************/

    //printf 的 Ctrl Code -- printf("\033[30m" "其他控制符" "\033[0m" , 参数列表)
    #define CTRL_COLOR_RESET        TEXT("\033[0m")

    #define CTRL_COLOR_BOLD         TEXT("\033[1m")
    #define CTRL_COLOR_GREY         TEXT("\033[5m")
    #define CTRL_COLOR_UNDERLINE    TEXT("\033[4m")
    #define CTRL_COLOR_BLINK        TEXT("\033[5m")
    #define CTRL_COLOR_BLACKBLOCK   TEXT("\033[7m")
    #define CTRL_COLOR_DELETELINE   TEXT("\033[9m")

    #define CTRL_COLOR_BLACK        TEXT("\033[30m")
    #define CTRL_COLOR_RED          TEXT("\033[31m")
    #define CTRL_COLOR_GREEN        TEXT("\033[32m")
    #define CTRL_COLOR_YELLOW       TEXT("\033[33m")
    #define CTRL_COLOR_BLUE         TEXT("\033[34m")
    #define CTRL_COLOR_MAGENTA      TEXT("\033[35m")
    #define CTRL_COLOR_CYAN         TEXT("\033[36m")


	FTLEXPORT class CFRectDumpInfo : public CFConvertInfoT<CFRectDumpInfo, const RECT&, 64>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFRectDumpInfo);
	public:
		FTLINLINE explicit CFRectDumpInfo(const RECT& rect);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	};

	FTLEXPORT class CFFileTimeDumpInfo : public CFConvertInfoT<CFFileTimeDumpInfo, const FILETIME&, 64>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFFileTimeDumpInfo);
	public:
		FTLINLINE explicit CFFileTimeDumpInfo(const FILETIME& fileTime);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	};

	//双显示器时: dwFlags = 01(MONITORINFOF_PRIMARY) 表主显
	//  rcMonitor 显示器的显示区域
	//  rcWork    排除任务栏的区域
	//程序在左边的主显(下方有任务栏)：cbSize= 40,dwFlags=0x1, rcMonitor=(0,0)-(1440,900), 1440x900, rcWork=(0,0)-(1440,869), 1440x869
	//程序在右边的次显：              cbSize= 40,dwFlags=0x0, rcMonitor=(1440,-124)-(2720,900), 1280x1024, rcWork=(1440,-124)-(2720,900), 1280x1024

	FTLEXPORT class CFMonitorInfoDumpInfo: public CFConvertInfoT<CFMonitorInfoDumpInfo, const MONITORINFO &>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFMonitorInfoDumpInfo);
	public:
		FTLINLINE explicit CFMonitorInfoDumpInfo(const MONITORINFO& monitorInfo);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	};
}

#endif //FTL_DEBUG_H

#ifndef USE_EXPORT
#  include "ftlDebug.hpp"
#endif
