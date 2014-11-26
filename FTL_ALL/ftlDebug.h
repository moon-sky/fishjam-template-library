#ifndef FTL_DEBUG_H
#define FTL_DEBUG_H

#pragma once

namespace FTL
{
    //WinDBG用法详解.pdf -- P79,  30.14 遍历链表
    //Windows用户态程序高效排错(普通下载).pdf -- P37(SharePoint中文界面变英文) P43, 符号文件
    //  http://blog.csdn.net/eparg/

	//http://wenku.baidu.com/view/14fdd446a8956bec0975e30c.html -- P31, 挑选技术

    //TODO:Time Travel Debugging -- 微软最新的调试技术(记录代码执行流程，且所有的代码都是顺序执行 ?)

    /*********************************************************************************************************
    * BSOD(Blue Screen of Death) -- 蓝屏死机
    *
    * WinDbg调试dmp文件(可以下载并加载不匹配的符号 -- VS2008不能下载)
    * 一般来说都是Mini dump，只包含寄存器、堆栈、和部分内存
    *   1.设置符号路径 程序pdb路径;SRV*D:\OSSymbols*http://msdl.microsoft.com/download/symbols
    *   2.打开dmp文件
    *   3.!sym noisy  -- 相当于.symopt+0x80000000，即开启所谓的“吵杂”式符号加载，显示符号加载的调试信息(可检测调试符号加载时的问题)。
    *   4.!analyze -v -- 一般这个时候可以看到出问题的代码位置，但如果有异常处理(如Nelo)等，则kn看出来的地址不对,需执行下一步
    *       -v    -- 对信息进行详细输出
    *       -hang -- 对问题进行自动分析(似乎是直接调用 kn? 有Nelo时不准 )
    *      分析结果:
    *        FAULTING_IP -- 错误发生时指令指针(EIP)的值，通过pdb一般能定位到最后出错的代码位置(注意：不一定是原因)
    *        EXCEPTION_RECORD -- 异常记录(Exception Record), 如值是ffffffff表示没有一个异常包含
    *        DEFAULT_BUCKET_ID -- 详细给出故障所属的类别，典型情况有 NULL_POINTER_READ, INVALID_POINTER_WRITE 等
    *        FAULTING_THREAD -- 导致出错的线程，可以通过 ? 查看
    *        BUGCHECK_STR -- "错误检查"的文字描述，但这里不区分用户态和内核态，可能导致误解
    *   5. .ecxr 后 kn -- 在有异常处理函数的情况下查看错误时的真实调用堆栈
    * 
    * 通过MAP文件根据地址(如异常地址)定位代码位置
    *   1.编译时需要生成Map -- Linker: Generate Map File(Yes, /MAP); Map File Name(xxx); Map Exports(Yes, /MAPINFO:EXPORTS)
    *   2.将指定地址和 Rva+Base栏地址(即 偏移地址+基址) 进行比较，找到最后一个比指定地址小的函数,即为目标函数;
    *   3.用指定地址减去该函数的Rva+Base,得到代码相对于函数起始地址的偏移，
    *     TODO(VS2012不一样了), 然后在"Line number for"部分找相应行号。
    * 
    * 通过汇编(COD)文件定位 -- TODO:未确认
    *   1.编译时选中"Assembly with Machine Code",会生成与每一个cpp文件同名的.cod文件
    *
    * adplus.vbs -- 能够检测一个或多个进程，并且在这些进程发生崩溃时自动生成转储文件并通知用户
    *   手工生成dmp文件, 也有专门的工具
    *   adplus -Crash -p 进程ID -quiet -fullonfirst -o C:\dumps
    *      -fullonfirst：在first chance时捕捉完整的dump信息，也就是进程的所有完整信息
    *
    * 确认二进制文件的编译选项
    *   1.判断Exe是Unicode/ASCII -- dumpbin /headers exe文件后查看"entry point"是不是 "_wWinMainCRTStartup"
    *     TODO: Unicode 的 DLL 会是"__DllMainCRTStartup@12" ?
    *     另外：dumpbin /exports 后利用 undname 查看函数签名
    * 
    * gflags.exe -- WinDbg的全局配置工具
    *  定位内存泄露( http://support.microsoft.com/kb/268343 ):
    *    1.选中"Enable pool tagging" 和"Enable heap tagging"。
    *    2.cmd.exe 中输入 gflags.exe /i text.exe +ust  <== 在注册表中设置 text.exe的GlobalFlag信息,
    *        ust 表示 "Create user mode stack trace database"
    *      注册表路径: HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options
    *    3.VC中发现内存泄露的提示后(如 e:\tests\testsdlg.cpp(101) : {118} normal block at 0x003BBAD8, 100 bytes long.),
    *      针对内存泄露的地址执行： !heap -x 内存地址(如0x003BBAD8) 获取 Entry 地址，如 
    *      Entry     User      Heap      Segment       Size  PrevSize  Unused    Flags
    *      -----------------------------------------------------------------------------
    *      003bbab0  003bbab8  003b0000  003b0640        a0      17f0        18  busy extra fill 
    *    4.针对获取到的Entry地址，执行: !heap -p -a 地址(如 003bbab0 ), 会显示调用堆栈 ?
    * 
    *********************************************************************************************************/

    /*********************************************************************************************************
    * 手工回溯栈(缓冲区溢出等造成栈帧信息被破坏)--
    *   a.ln eip -- 得到当前函数名
    *   b.寻找当前函数的栈帧基准地址 -- 没有破坏的 EBP或者ESP, 或 !teb
    *   c.寻找当前函数的返回地址 -- EBP+4
    *********************************************************************************************************/

    /*********************************************************************************************************
    * TODO: 
    *  1.双机内核调试 和 本地内核调试
    *  2.alias -- 
    *  3.通过启动内核调试对话框设置内核调试连接设置
    *  4.命令中的注释： *(其后所有内容都是注释) 或 $$(其后内容都是注释，可使用分号结束) -- 使用前应该在前一条命令后加上分号作为分隔
    *  5.poi 将变量转换成指针 "j (poi(MyVar)>5) ' -- J 命令判断变量MyVar 的值是否大于5； 若 pSomeFun 为函数指针，则 ln poi(pSomeFun) 显示其地址的函数名
    *    da poi(ebp+8) -- 在 printf 函数中显示第一个参数所指定的字符串，此断点必须在建立栈的代码(mov ebp,esp)执行完毕后才行。
	*  6.当调试用户态的转储文件时，可以使用.ecxr 命令将转储文件中保存的异常上下文设置为寄存器上下文。
	*  7.VC 编译器缺省将类型符号放在VCx0.PDB 文件中，WinDBG不能很好的处理，因此显示局部变量时，会显示很多no type information 错误。
	*    解决方法：将符号格式设置为C7 Compatable（Settings>C++ > General > Debug Info）
	*  8.设置环境变量_NT_SYMBOL_PATH 和_NT_ALT_SYMBOL_PATH
	*  9.WinDBG 缺省使用所谓的懒惰式符号加载策略，模块加载时不会加载符号，显示为 deferred(延迟加载)，
	*    通过 .reload 强制全部加载或 .load 手动加载 ？
    *  10.加载不严格匹配的符号文件(通常没有完全匹配的符号文件时) -- 先通过 !sym noisy 开启吵杂模式确认，然后 .reload /i 模块名 加载
    *     加载完后通过 lm 查看，其前面会有 "M" 的标记
    *  11.系统自带的 Dr.Watson(???.exe) -- 系统自带的应用程序调试程序，崩溃时可生成dmp，-i 参数设置为默认
    *
    * 扩展命令
    *   sdbgext -- 可以根据窗体句柄查看窗体信息等， 如 !sdbgext.hwnd 00070836
    *********************************************************************************************************/

	/*********************************************************************************************************
	* API Monitor
	*   http://www.rohitab.com/apimonitor
	*   http://www.apimonitor.com/
	*********************************************************************************************************/

	/*********************************************************************************************************
	* Windows 的调试模型是事件驱动的。整个调试过程就是围绕调试事件的产生、发送、接收和处理为线索而展开的。
	* Windows 定义了9 类调试事件:
	*   EXCEPTION_DEBUG_EVENT(1) -- 异常事件,根据异常代码分为很多个子类:
	*     Win32 异常(非法访问、除零 -- 定义在 ntstatus.h 中)
	*     Visual C++异常(throw调用RaiseException -- 0xe06d7363, .msc)
	*     托管异常(0xe0636f6d, .com )
	*     其他异常(用户直接调用RaiseException和其它C++编译器抛出的异常等)
	*   CREATE_THREAD_DEBUG_EVENT(2) -- 
	*   CREATE_PROCESS_DEBUG_EVENT(3) -- 
	*   EXIT_THREAD_DEBUG_EVENT(4) -- 
	*   EXIT_PROCESS_DEBUG_EVENT(5) -- 
	*   LOAD_DLL_DEBUG_EVENT(6) -- 
	*   UNLOAD_DLL_DEBUG_EVENT(7) -- 
	*   OUTPUT_DEBUG_STRING_EVENT(8) -- 
	*   RIP_EVENT(9) --
	*********************************************************************************************************/

    /*********************************************************************************************************
    * WinDbg  -- 反汇编分析能力较弱，GUI界面偏弱，无源码时调试比较困难；支持dump调试，分析功能强大
    *            通过 SOS.dll 支持.NET调试，进行高级调试比较方便
    * http://download.microsoft.com/download/A/6/A/A6AC035D-DA3F-4F0C-ADA4-37C8E5D34E3D/setup/WinSDKDebuggingTools_amd64/dbg_amd64.msi
    * 
	* WinDbg(http://msdn.microsoft.com/en-us/windows/hardware/gg463009) -- 现在已经不再提供单独下载，在SDK、WDK等中包含。
    *   支持脚本和插件，而且软件包本身提供了大量非常有用的插件，可编写调试扩展模块来定制和补充调试功能
    *   .hh 关键字 -- 打开帮助文档并定位到指定关键字
    *   其他帮助文档：kernel_debugging_tutorial.doc(内核调试)、symhttp.doc(建立符号服务器)、srcsrv.doc(介绍建立源文件服务器)、
    *                 dml.doc(介绍DML的用途和编写方法)
	*   
    * 工作空间(Workspace) -- 描述和存储一个调试项目的属性、参数、以及调试器设置等信息，类似IDE中的项目文件。
    *   配置信息保存在 HKCU\Software\Microsoft\Windbg\Workspaces(也可以保存成文件)，一般包含四个子键，分别对应四类不同的工作空间。
    *     User(用户态)
    *     Kernel(内核态) -- 可以分为 Default/x86/AMD64/Itanium 等
    *     Dump(转储文件)
    *     Explicit(用户另存为的显式工作空间) -- 通常是可执行文件的名称或全路径
    *
    * 主题(Themes) -- WinDbg程序目录中的Themes子目录中包含了四种经过定制的工作空间设置(主要是窗口布局)。可通过将.reg导入注册表后使用。
    * 别名(Alias) -- 可用于简写命令或编写命令程序等？
    *   分三类(可通过 .echo 查看)：
    *     固定名称别名( $u0 ~ $u9 ) -- 修改方式: r $.u<0~9>=<别名实体>， 如 r $.u9=nt!KiServiceTable， 使用方式：dp $u9
    *     WinDBG 自动定义的别名 -- 
    *       $ntnsym(NT 内核或者NT DLL的符号名), 
    *       $ntwsym(在64位系统上调试32位目标时的NT系统DLL符号名), 
    *       $ntsym(与当前调试目标的机器模式匹配的NT 模块名称)
    *     用户命名别名(可通过 .aliascmds 查看) -- 修改方式：as 别名名称 别名实体。 如 as v version.
    *       引用方式：${用户别名} 或 直接引用
    * 日志文件(LogFile) -- 可以把输入的命令和命令的执行结果记录一个文本文件(日志文件)中。
    * 上下文:
	*   会话(Session)上下文 -- 只在内核调试时才有意义, !session 扩展命令, -s n 设置, !sprocess [n] -- 列出某个会话的所有进程
    *     改变会话后，隐含进程通常也会变化，以前缓存的虚拟内存数据不再有效。因此需要将缓存选项中设置强制将虚拟内存翻译为物理内存(.cache forcedecodeuser)。
	*   进程(Process)上下文 -- 通过 .process 显示和切换 ? 在内核调试时，通常只有一个进程的用户空间是可见的，要观察其他进程的用户空间，必须将进程上下文切换到对应进程。
	*   寄存器(Register)上下文 -- 寄存器中存放的是当前正在执行线程的寄存器值，通过 .thread 显示或者设置寄存器上下文所针对的线程
    *   局部(Local)上下文 -- 局部变量所基于的语境，涉及到堆栈
	*  
    * 有三类命令(在命令编辑框中输入 "." 或 "!" 然后按 Tab，可以顺序显示各种可用的命令 )：
    *   1.标准命令 -- 一两个字符或者符号，用来提供适用于各种调试目标的最基本调试功能
    *   2.元命令(Meta-Command，以点"."开始) -- 提供标准命令没有提供的调试功能
    *   3.扩展命令(Extension Command，以感叹号"!"开始) -- 用于扩展某一方面的调试功能，是实现在动态加载的扩展模块(DLL)文件中的，
    *     在如 w2kchk/w2kfre/winext/winxp 等各个子目录中，可通过其SDK可编写自己的扩展模块。
    *     当调试目标被激活时，WinDBG 会根据调试目标的类型和当前的工作空间，自动加载某些扩展模块。也可通过 .load 元命令来加载
    *   命令中的通配符：*()， ?()  
	* 
    * 常用命令(在中断状态下，通过F9键可以设置/取消断点), 直接输入问号 "?" 可以显示出一个标准命令的列表和简单介绍
    *   ~ -- 显示和切换线程
    *     ~<thread_index> s -- 切换当前线程， 如 "~6 s" 切换到第6号线程
    *     ~*e 命令 -- 针对所有线程执行命令，如 ~*kb 或 ~*e kn 显示所有线程的调用堆栈; ~1 命令 -- 针对1号线程执行命令
    *     ~ThreadIndex n/m -- 增加或减少指定线程的 Suspend 数(改变线程的挂起计数系统属性)
    *     ~ThreadIndex f/u -- 冻结(Freeze)或解冻(Unfreeze)线程(改变调试器的线程设置)
    *   | -- 显示进程
    *     |<process_index> s -- 切换当前系统中的当前进程
    *   || -- 显示当前的系统(如 本地、远程、dump等)，在同时调试多个系统时使用
    *     ||<n> s -- 将当前系统切换到<n>号
    *   ? -- 以MASM方式显示表达式的值(显示地址?)
    *   ?? -- 以C++方式显示表达式的值(显示内容?)，通过 ?? varName=value 的方式可以改变变量的值
    *     如 ??(MyClass*)@ecx 或 (MyClass*)@ESI -- 显示this指针的内容(通常在ecx中，但有时会优化到 esi 等中, TODO: 如何设置和判断)
	*   @@ -- 用"d"显示变量内容时使用C++赋值方式？
    *   $ -- 执行命令文件
    *   a -- 汇编 -- 可以将指定位置的数据进行汇编？
    *   al -- 列出目前定义的所有用户命名别名,等价于 .aliascmds ?
    *   ad -- 删除指定或全部(ad *) 用户别名
    *   as 别名名称 别名实体 -- 定义或者修改用户命名别名，如 as SST nt!KiServiceTable
    *   b -- 断点(F9 即是在光标所在源码行设定断点？)。bu 设置的断点会保存在Workspace中?
    *     ba Access Size [Address] -- 数据断点(也叫硬件断点)，可检视数据和IO访问等。通过CPU的调试寄存器设置(x86设置在DR0~DR7)，具有数量限制。
    *       如 ba w4 0xffb5c4f8+0x18+0x4
    *       Access: e(读取和执行指令时);r(读写数据时);w(写数据时);i(执行输入输出访问时)
    *       Size:指定访问的长度。e(1),其他种类的可以使 1/2/4/8。注意地址的内存对齐。
    *     bc/bd/be -- 清除/ 禁用/启用  断点，如 bc 1 或 bc *
    *     bl -- 列出所有断点
	*     bm -- 通过通配符设置一批断点，如 bm msvcr80d!print* 对于 msvcr80d 模块中的所有print 开头的函数设置断点。
	*           注意：要求目标模块的调试符号有类型信息，能够判断出一个符号的类型 -- 即要求私有的调试符号文件
	*     bu -- 对没有加载符号表的模块使用延迟设置断点，常用于调试动态加载模块的入口函数或者初始化代码。
	*        如 bu MyDriver!DriveEntry
    *     bp [Options] [Address [Passes]] ["CommandString"] -- 在指定函数名或地址处设置软件断点，
	*        如 bp HelloWDM!DriveEntry  
	*           bp MSVCR80D!printf+3 2 "kv;da poi(ebp+8)" -- 对printf 偏移3的地址设置断点，第二次时才中断，并自动执行指定命令。
	*              da poi(ebp+8)用来显示printf 的第一个参数所指定的字符串。设置偏移3的原因：入口处的栈帧建立代码执行好后，ebp+8才能指向第一个参数
    *           bp USER32!MessageBeep -- 对系统API MessageBeep 下断点， 注意：VC中对应的断点方式为 {,,USER32.DLL}_MessageBeep@4
	*           bp my.cpp:122 100
    *        TODO:(似乎是用 wt ?)如何在PostMessageW/SendMessageW 上设定条件断点，每次调用这两个函数时，打印消息的详细信息和callstack
    *             Windows用户态程序高效排错(普通下载).pdf 中 P29 所述
	*        条件断点: 命令行中通过 j(条件) 或 .if(条件) 来判断并处理。
	*          如：当命令行参数的个数大于1时中断
	*            1.[MASM表达式时]bp myProgress!wmain "j (poi(argc)>1) 'dd argc l1;du poi(poi(argv)+4)';'gc'"
	*            2.[MASM表达式时]bp myProgress!wmain ".if (poi(argc)>1) {dd argc l1;du poi(poi(argv)+4)} .else {gc}"
	*            3.[C++表达式时] bp myProgress!wmain "j (argc>1) '? argc;?? argv[1]';'gc'"
	*     /p EPROCESS -- 只能用在内核调试中, 只有当前进程是指定进程时才触发这个断点
	*     /t ETHREAD  -- 只能用在内核调试中, 只有当前线程是指定线程时才触发这个断点
    *   dds 地址 -- 可显示指定位置的函数名?
    *   d{a|b|c|d|D|f|p|q|S|u|w|W} 地址 [L长度] -- 观察指定地址的内存数据
    *     a(ASCII),b(Byte and ASCII),c(DWORD and ASCII),dd(DWORD),f(float),p(Pointer),q(Quad-word),S(UNICODE_STRING),u(Unicode),w(Word),W(Word and ASCII) 
    *     dg -- 显示段选择子
    *     dl -- 遍历链表，提供的地址必须是LIST_ENTRY/SINGLE_LIST_ENTRY的地址，命令中要指定链表元素的大小，而且只能显示下一个ENTRY的指针数值。
    *           相对来说 dt -l 或 !list 方式对用户更友好。
    *     dt Xxxx [Address] -- 按照类型(如 结构体？)显示变量内容，如 dt _EPROCESS 0xXXXX -- 使用_EPROCESS结构来显示指定内存地址的数据
    *       -l -- 遍历列表，如：dt nt!_EPROCESS -l ActiveProcessLinks.Flink -y Ima -yoi Uni poi(PsInitialSystemProcess) 表示遍历进程，类似 !process 0 0
    *       dt -r this -- 递归显示this的成员变量类型及值
    *     dv /i /t /v -- 显示函数参数和局部变量的信息(Ctrl+Alt+V 切换到更详细的显示模式)，注意：优化过的代码显示可能不准确。
    *       /i -- 显示变量范围(param,local,function 等); /t -- 显示变量类型(int,wchar_t* 等), /v -- 显示相对ebp等寄存器的地址
    *       缺少私有符号时dv命令无法工作，可通过查看内存、 EBP+-N 的方式查看
    *   e -- 编辑内存数据，根据格式有 e{b|d|D|f|p|q|w} 等
    *   g[=StartAddress] -- 恢复运行
    *     gc -- 从条件断点继续执行，通常用于设置断点时的 "CommandString" 中
	*     gh/gn -- 发生异常时继续并返回系统 已处理(通常用于动态改变值后继续)/未处理(通常程序会崩溃或蓝屏) 该异常
	*     gu -- 执行到本函数返回，等价于 ta @$ra
    *   j <条件表达式> [Command1>] ; [Command2>] -- 判断一个条件，然后选择性的执行后面的命令，类似 if..else，可用单引号包含一组命令
    *     例： r ecx; j (ecx<2) 'r ecx';'r eax' 表示先显示寄存器ecx 的值，然后执行j命令判断ecx是否小于2并执行不同部分。
    *          等价于：r ecx; .if (ecx>2) {r ecx} .else {r eax}
    *   k -- 显示调用堆栈
	*     kb -- 显示传入函数的头三个参数
	*     kp -- 显示每个函数的所有参数，需要私有符号。
	*     kn -- 显示堆栈层次数，可通过 .frame n 切换
    *     kv -- FPO（栈指针省略）信息和调用协议(如 cdecl、等)，需要私有符号。
    *     k* 显示当前线程的堆栈; ~*kb 显示所有线程的堆栈
	*   ld 模块 -- 加载指定模块的调试符号(可加载的模块名是通过 lm 能看到的)
    *   lm -- 列出当前加载的模块列表，可查看是否正确加载了需要调试的程序、驱动的符号表。
    *     标记符号的意义：
    *       C 或 T(具体区别？) -- 校验和缺失，不可访问，或者等于0
    *       # -- 表示符号文件和执行映像文件存在不匹配，比如时间戳、校验和等
    *       M -- 表示符号文件和执行映像文件存在不匹配，要么是时间戳或者校验和。但是仍然加载了这样的符号文件。
    *       PERF -- 执行文件包含性能优化代码，对地址进行简单加减运行可能产生错误结果。
	*     lm e -- 只显示有符号问题的模块
	*     lm -f -- 显示模块全路径，通常可以查看运行的模块是否是指定的模块(尤其是服务、驱动、COM组件等)
    *     lm l -- 只显示已经加载符号的模块
	*     lm v -- 可以显示详细信息，如路径、时间戳、版本等
    *     lm m -- 指定对模块名的过滤模式，比如 lm m k* 显示以模块名k 开头的模块
    *   ln 地址 -- 搜索距离指定地址最近的符号
    *   p[r][=StartAddress] -- step over，单步执行完函数调用, r 表示禁止断点时显示寄存器值,可通过等号(=)指定起始地址。
	*     pa StopAddress-- 单步执行到指定地址，如果中间有函数调用，那么不进入所调用的函数
	*     pc -- 单步执行到下一个函数调用(Call 指令, 汇编调试中使用 pc + t 跟踪执行流程时非常有用)
    *   q -- 结束调试会话
    *   r -- 观察和修改寄存器，如 r ecx=2 表示将ecx设置为2
    *   s -- 搜索内存，主要分为三种应用方式
    *     s -[[Flags]]sa|su Range -- 在指定的内存范围内搜索数据(ASCII字符|UNICODE字符),
    *       例：s-[l5]sa poi(nt!PsInitialSystemProcess) l200 -- 搜索nt!PsInitialSystemProcess 变量所指向地址开始的512个字节范围内任何长度不小于5的ASCII字符串
    *     s -[[Flags]]v Range Object -- 在指定内存地址范围内搜索与指定对象相同类型的对象(相同的类实例)
    *     s [-[[Flags]]Type] Range Pattern  -- 在指定范围内搜索某一内容模式
    *       例：s-u 0x400000 l2a000 "AdvDbg" -- 从0x40000开始长度为0x2a000的范围内搜索"AdvDbg"的Unicode字符串
    *     通用参数
    *       -s -- 将搜索结果保存起来
    *       -r -- 在保存的结果中搜索
    *   sx -- 显示或设置调试事件处理方式(GUI方式为Debug -> Events Filters)
    *    sxr 可以将所有事件处理选项恢复为缺省值
    *   t -- trace into，单步如遇到函数调用，进入被调用函数
	*     ta -- 追踪到指定地址，进入子函数
	*     tb -- 追踪执行到下一条分支指令
	*     tc -- 追踪执行到下一个函数调用(Call 指令)
    *   u XXXXXXXX -- 反汇编指定的地址(不需要 0x 前缀 ?)， 如 lkd> u KeAddSystemServiceTable l 40
    *     l 长度 -- 可以指定反汇编出来的语句长度，是16进制
    *   version -- 显示版本和已经加载的扩展模块等信息
	*   wt(Watch and Trace data) -- 查看并追踪某函数从入口到return的整个执行过程(★调用分析的利器★?)，
    *     使用方式: 设置函数入口的断点 -> g执行程序 -> 激活断点后wt自动追踪 -> 程序会继续执行,return 后打印日志信息
    *     执行情况表中：
	*       第一列表示指令数(函数入口的数字表示 从本函数入口进入到下一行所对应的函数的入口所执行的指令数；函数出口的数字表示本函数执行的指令数 )
	*       第二列表示本行所对应的函数调用其它函数时所执行的总指令数；
	*       第三列表示函数调用深度;
	*       第四列表示函数名称
    *   x -- (Examine Symbols)分析调试符号，支持 *,?,#,[0-9], 等正则表达式。注意：有些选项只有私有PDB中才起作用。
	*     /n -- 按名称升序。 /s <符号大小> -- 按符号(函数体、数据类型)大小设置过滤条件。 
    *     /t -- 显示符号的数据类型。 /v -- 显示符号的符号类型和大小
    *     如: x /a win32k!NtUser* 列出win32k模块中所有以 NtUser 开头的符号并按地址升序排序。
    *   z -- 循环执行一或多个命令，循环执行时会有 redo 的日志提示
    *   
    * 表达式评估器(通过 .expr 切换)
    *   MASM[缺省]--Microsoft Assembler expressions, 此时可使用 @@ 前导符来嵌入C++表达式。引用伪寄存器时可以 在$符号前加@符号也可不加。
    *   C++ -- 引用伪寄存器时一定要加 @符号，引用别名的典型方式是使用 ${}
    *     @@c++(表达式) -- 强制使用C++表达式评估器
    *
	* 操作符(Operator) -- 如 r eip = poi(@esp) -- 设置Eip(命令指针)为堆栈偏移为0x0指向的值
	*   poi -- 从指定地址获得指针大小的数据,类似的有 by(字节),wo(字),dwo(双字),qwo(四字) 等
	* 
    * 元命令 -- 以一个点(.)开始
    *   .abandon -- 放弃用户态调试目标进程
    *   .aliascmds -- 列出当前所有的用户命名别名
    *   .asm -- 反汇编选项
    *   .attach -- 附加到指定进程, 如果带参数 -k 表示是内核调试， -premote 可指定远程系统的位置和通信方式
    *   .call -- 从调试器中调用被调试程序中的函数，必须有被调用函数的私有符号。需要小心其可能的副作用
    *   .chain -- 列出当前加载的所有扩展模块
    *   .childdbg -- 控制子进程的调试，如果同时调试几个进程，可以使用 "|" 命令来切换
    *   .cls -- 清屏, 清除当前屏幕上的文字
	*   .context -- 设置或者显示用来翻译用户态地址的页目录基地址(Base of Page Directory)
    *   .create -- 创建新进程
    *   .dbgdbg -- 启动另一个调试器来调试当前调试器
    *   .detach -- 分离调试目标
    *   .dump -- 显示或者转储调试目标数据
    *      .dump /ma Dump文件名   -- 手动生成Dump文件
    *   .ecxr -- 当调试用户态的转储文件时将转储文件中保存的异常上下文设置为寄存器上下文。
    *   .echo -- 显示信息，如 别名(alias)、提示信息 等
    *   .effmach -- 显示或改变调试器使用的处理器模式(x86/amd64/ia64 等) -- 通常用于跨平台分析?
    *   .endpsrv -- 结束远程进程服务器
    *   .endsrv -- 结束引擎服务器
    *   .exepath -- 可执行文件
    *   .expr [/s] -- 显示或切换表达式评估器，分为 MASM, C++ 等
    *   .extpath -- 扩展命令模块路径
    *   .extmatch -- 匹配扩展命令
	*   .frame [n]-- 观察或切换当前的调用堆栈层次
    *   .formats -- 以不同格式显示数据(比如: .formats 0x302 -- 会将十六进制显示为二进制格式)
    *   .help -- 列出所有元命令和每个命令的简单说明
    *   .kill -- 杀掉进程
    *   .load 模块完整路径 -- 加载扩展命令模块
    *   .loadby 模块名称 -- WinDBG 会自动到当前配置中定义的扩展模块所有路径中搜索匹配的模块并加载
    *   .logfile -- 显示调试器日志文件
    *     .logopen(打开)/.logappend(追加)/.logclose(关闭)
    *   .lsrcpath -- 远程 源码路径
    *   .opendump -- 打开转储文件
	*   .process [EPROCESS] -- 显示、切换进程上下文，其 EPROCESS 可通过 !process 查看
    *   .reload [module]-- 重新加载符号表， /i 允许加载不严格匹配的符号文件， /f 强制更新包括lazy在内的
    *   .remote -- 启动 remote.exe 支持远程调试
    *   .restart -- 重新开始调试会话
    *   .send_file -- 向远程服务器发送文件
    *   .server -- 启动引擎服务器
    *   .servers -- 列出可用服务器
    *   .sleep -- 让调试器睡眠一段时间
	*   .srcfix -- 
	*   .srcnoisy [options]-- 设置搜索源码目录时的输出信息
	*   .srcpath [源码目录]-- 设置本地源码路径(通常用于非本地编译时，如果是本机编译调试的话不用设置)
	*   .suspend_ui -- 暂时停止刷新信息窗口，防止界面更新太频繁
    *   .symopt -- 显示和修改符号选项。使用一个32位的DWORD 来记录符号选项，每位代表一个选项，+号设置,-号取消，
    *      如 SYMOPT_DEFERRED_LOADS(延迟加载，默认为ON)；SYMOPT_DEBUG(显示符号加载过程，默认Off) 等
    *         SYMOPT_LOAD_ANYTHING(加载不完全匹配的符号，默认OFF；影响范围太大，容易出错，建议不要设为ON)
    *   .sympath -- 可以增加、修改、或者显示符号搜索路径(本地磁盘目录 + 符号服务器，多个位置之间用分号分隔)
	*   .symfix -- 设置符号服务器，设置完毕后需要使用 .reload 刷新
    *   .thread [ETHREAD] -- 将指定线程的上下文设置为新的线程上下文，具体的 ETHREAD 可在 .process <EPROCESS> 时看到
    *   .time -- 显示调试会话时间
    *   .tlist -- 显示任务列表(task list)
    *   .ttime -- 显示线程时间
    *   .unload/.unloadall -- 卸载扩展命令模块
    *   .wake -- 唤醒处于睡眠状态的调试器
    *   .writemem -- 将原始内存数据写到文件
    *   流程控制符号(Control Flow Token)，多个命令可以以分号分隔并包含在大括号{} 中
    *     .if、.else、.elsif、.foreach、.do、.while、.continue、.break、.continue、.printf
    *    .catch、.leave -- 捕捉异常和从异常块中退出
    *    .block -- 定义代码块
    * 
    * 扩展命令 -- 实现在DLL中扩展某一方面的调试功能，部分会自动加载，不许需要通过 .load/.loadby 手动加载。
    *     以感叹号(!)开始的命令，语法为： ![扩展模块名.]<扩展命令名> [参数]
    *     TODO: !for_each_XXXX 命令循环执行， 如 !for_each_frame 对每个栈帧执行一个操作， !for_each_local对每个局部变量
    *           打印出每个栈帧的每个局部变量 -- !for_each_frame !for_each_local dt @#Local
	*   [Dbghelp.dll] -- 缺省的
	*      !lmi 模块名 -- 显示模块详细信息,每次只能观察一个模块
	*      !process -- 查看指定(默认是当前进程)进程信息，包括 EPROCESS 块的信息
    *        !process 0 0 列出系统中的所有进程的基本信息， !process -1 0 查看当前进程
    *      !session -- 显示和切换会话上下文
	*      !sym -- 控制调试符号加载时的提示信息( .symopt 命令的扩展)
	*         noisy(开启嘈杂模式), quiet(关闭嘈杂模式)
    *   acpikd.dll -- 用于ACPI 调试，追踪调用ASL 程序的过程，显示ACPI 对象
    *   ext.dll -- 适用于各种调试目标的常用扩展命令
    *     !address [Address] -- 显示内存地址(区域)的特征信息，如 /Protect(PAGE_READWRITE)/State(PAGE_READWRITE) 等
    *     !list -- 遍历链表，显示链表的每个元素或者对其执行其它命令，在命令中可以使用 !list 命令所定义的伪寄存器$extret(代表每个链表节点的地址)
    *   exts.dll -- 关于堆(!heap)、进程/ 线程结构(!teb/!peb)、安全信息(!token、!sid、!acl)、和应用程序验证(!avrf)等的扩展命令。
    *   fltkd.dll -- 用于调试文件系统的过滤驱动程序(FsFilter)
    *   kdexts.dll -- 包含了大量用于内核调试的扩展命令
    *     !pte -- 显示指定地址所属的页表表项(PTE)和页目录表项(PDE)
    *   kext.dll -- 内核态调试时的常用扩展命令
    *     !d{b|c|d|p|q|u|w} -- 显示物理地址(只能在内核调试时使用)
    *   ks.dll -- 用于调试内核流(Kernel Stream)
    *   logexts.dll -- 用于监视和记录API 调用(Windows API Logging Extensions)
    *   minipkd.dll -- 用于调试AIC78xx 小端口(miniport)驱动程序。
    *   ndiskd.dll -- 用于调试网络有关驱动程序
    *   ntsdexts.dll -- 实现了 !dp、!dreg(显示注册表)等命令
    *     !cs -- 列出 CriticalSection 的详细信息
    *     !handle -- 检查句柄信息
    *     !locks -- 显示当前所有的锁,通常可以用来解决死锁问题
    *   rpcexts.dll -- 用于RPC 调试
    *   scsikd.dll -- 用于调试SCSI 有关的驱动程序
    *   sos.dll -- 用于调试托管代码和.Net 程序 -- .NET程序的内存问题很多情况下是缓存中没有清除机制造成的
    *     !eeheap -gc -- 查看托管堆上的内容(如GC拖管堆、堆上对象占用的空间)，如其中有"Large object heap"表示有大量的数据在堆上
    *     !dumpheap -min 85000 -stat -- 显示大对象(最小大小为85K ?)的统计信息(如果不带 -stat 参数，则显示详细列表)
    *     !gcroot 地址 -- 
    *   traceprt.dll -- 用于格式化 ETW 信息
    *   uext.dll -- 用户态调试时的常用扩展命令
    *     !vprot -- 显示一个内存地址的属性,显示结果类似于 !address
    *   vdmexts.dll -- 调试运行在 VDM 中的 DOS 程序和 WOW 程序
    *   wdfkd.dll -- 调试使用WDF(Windows Driver Foundation)编写的驱动程序
    *   wmitrace.dll -- 显示 WMI 追踪有关的数据结构、缓冲区和日志文件
    *   wow64exts.dll -- 调试运行在64 位Windows 系统中的32 位程序
    *   WudfExt.dll -- 用于调试使用UMDF 编写的用户态驱动程序
    * 
    * 伪寄存器(Pseudo-Register, 使用方式 @伪寄存器名, 如 ln @$exentry 或 ? @$pagesize )
    *   $callret -- 使用.call 命令调用的上一个函数的返回值，或者使用.fnret 命令设置的返回值
    *   $csp -- 帧指针。x86 中即ESP 寄存器，x64 是RSP，安腾为BSP。
    *   $dbgtime -- 当前时间
    *   $ea -- 调试目标所执行上一条指令的有效地址(effective address)
    *   $ea2 -- 调试目标所执行上一条指令的第二个有效地址
    *   $exentry -- 当前进程的入口地址
    *   $exp -- 表达式评估器所评估的上一条表达式
    *   $eventip -- 当前调试事件发生时的指令指针
    *   $frame -- 当前栈帧的序号
    *   $ip -- 指令指针寄存器。x86 中即 EIP，x64 即 RIP
    *   $pagesize -- 调试目标所在系统的内存页字节数
    *   $peb -- 当前进程的进程环境块(PEB)的地址, 其 BeingDebugged 字段表明当前进程是否在被调试
    *   $ptrsize -- 调试目标所在系统的指针类型宽度
    *   $proc -- 当前进程的 EPROCESS 结构的地址
    *   $ra -- 当前函数的返回地址(retrun address)
    *   $retreg -- 首要的函数返回值寄存器。x86 架构使用的是EAX，x64 是RAX，安腾是ret0
    *   $retreg64 -- 64 位格式的首要函数返回值寄存器，x86 中是edx:eax 寄存器对
    *   $scopeip -- 当前上下文(scope)的指令指针。
    *   $teb -- 当前线程的线程环境块(TEB)的地址
    *   $thread -- 当前线程的 _ETHREAD 结构的地址, 如 ?? @$thread
    *   $tid -- 当前线程的线程ID
    *   $tpid -- 拥有当前线程的进程的进程ID(PID)
    * 用户伪寄存器：$t0~$t19， 可以使用 r 命令来定义用户伪寄存器的实体。
    *
    * 远程调试 -- 通过和远程工具、转发器配合，实现各种灵活的远程调试方式，以支持不同的网络环境
    *   1.DbgSvr/KdSvr + remote.exe ?
    *   2.服务端，使用名为advdbg的命名管道： windbg -server npipe:pipe=advdbg 或 执行 .server npipe:pipe=advdbg 命令
	*     客户端，连接该命名管道：windbg -remote npipe:server=FJPC,pipe=advdbg 或 "Connect to Remote Session" 并输入 npipe:Pipe=advdbg,Server=FJPC
	*
    * 调试器命令程序(Debugger Command Program) -- 把一系列调试器命令放在一个文件中，然后以文件的形式通过命令提交给调试器来执行。
    *   可以使用流程控制符号控制流程，自动的伪寄存器、用户赋值的伪寄存器等作为变量
    *   提交执行文件的命令:
    *     $>< FileName -- 读取文件并将其中的内容浓缩成一个单一命令(浓缩时把换行符替换为分号)然后执行
    *     $$<> FileName -- 允许在文件名前有空格，并允许使用双引号包围文件名。
    *     $$>a< FileName arg1 arg2 -- 允许使用参数，然后命令程序中可以通过 ${$arg1} 的方式使用
    * 
    * 命令行参数(WinDbg Command-Line Options)
    *   -k -- 内核(Kernel)调试时的连接类型， -kl 是本地内核调试
	*   -pe PID -- 连接一个被Detach或僵死的调试进程，和 -p 的区别在于可以连接"DebugPort不为空"的进程
    *   -server -- 以服务方式启动，通常用于远程调试的服务端
    *   -z -- 指定转储文件
    *
    * 调试技巧
    *   1.在 Watch 窗口中输入 "@err,hr" ，可以监视 API 的是否错误 及错误原因 -- GetLastError
    *   2.使用分号（;）号作为分隔符，可以在同一行输入多条命令。
    *   3.使用 Ctrl+Break 或菜单项的 "Debug->Break" 来终止一个长时间未完成的命令
    *   4.使用Windbg显示系统句柄信息(显示Session1 中的 WinSta0): lkd>!object Sessions\1\Windows\WindowStations\WinSta0 
    *   5.调试内核目标 -- 
    *
    *
    * DML(Debugger Markup Language) -- 一种标记语言，用于标记WinDBG 或者扩展命令的信息输出
	*********************************************************************************************************/

	/*********************************************************************************************************
	* OllyDbg -- 强大的代码分析能力，无符号和源码时也能很好的进行调试(所以常用于软件破解、病毒分析等逆向工程)
    *            在窗口中输入汇编代码，可以直接显示出对应的二进制编码(方便进行汇编的二进制编码)
	* 
    *   OllyDbg 的数据窗口能够显示的所有数据格式：HEX、ASCII、UNICODE、 16/32位有/无符号/HEX整数、
    *     32/64/80位浮点数、地址、反汇编（MASM、IDEAL或是HLA）、PE文件头或线程数据块。
    *   OllyDbg 可以调试多线程程序。因此您可以在多个线程之间转换，挂起、恢复、终止线程或是改变线程优先级。
    *     并且线程窗口将会显示每个线程的错误（GetLastError）
    *   OllyDbg 的最大特点之一就是分析。它会分析函数过程、循环语句、选择语句、表［tables］、
    *     常量、代码中的字符串、欺骗性指令［tricky constructs］、API调用、函数中参数的数目，
    *     import表等等。
    *   OllyDbg 支持各种断点：一般断点、条件断点、记录断点（比如记录函数参数到记录窗口）、内存读写断点、
    *     硬件断点（只适用于ME/NT/2000）等。在Hit跟踪情况下，可以在模块的每条命令上都设置INT3断点。
    *   信息窗口：显示反汇编窗口中选中的第一个命令的参数及一些跳转目标地址、字串等。
    *   配置项：
    *   插件及 UDD 的目录：选项->界面 -> 目录；作用？？？
    *     UDD -- 保存调试的工作。如调试软件时设置的断点，添加的注释等。
    *     插件：超级字串参考＋，
    *   调试选项：如 异常
    * 
    *  函数参考 -- 
    *  函数中断 -- 反汇编窗口中右键，查找->当前模块中的名称(Ctrl+N)，在弹出的导入函数列表中直接输入想查找的函数名字可快速定位。
    *  内存断点 -- 数据窗口 -> 右击 ->转到->表达式->输入地址(直接输入16进制，不需要0x)，到达指定位置后，
    *    用鼠标拖动一个范围后设置断点,右键->断点->读取/写入
    *    注意:内存断点只在当前调试的进程中有效，如果重新载入程序的话内存断点会自动删除。且内存断点每一时刻只能有一个。
    *  消息断点 -- 在运行时，查看->窗口，通过标题找到指定的窗口(控件)，右键->在ClassProc上设置消息断点。  
    *  RUN 跟踪 -- 把被调试程序执行过的指令保存下来，查看被调试程序运行期间干了哪些事,会把地址、寄存器的内容、
    *    消息以及已知的操作数记录到 RUN 跟踪缓冲区中，一般会选择“总是跟踪步过系统DLL”--对系统DLL不感兴趣
    *    注意缓冲区大小，但空间不够时，会自动删除旧的跟踪记录。
    *    调试 -> 打开或清除 RUN 跟踪。但在程序领空时，在反汇编窗口中点击右键，RUN 跟踪->添加所有函数过程的入口。
    *      之后执行过得代码前面会用红色条标注。
    *    查看->Run跟踪->右键：统计模块，可以知道有哪些指令执行过。破解时通过设置，关心执行过一次的。即可找到相关的代码。
    *
    *   常用快捷键：
    *     F2：设置断点；
    *     F4：运行到光标所在位置；
    *     F7：单步进入；
    *     F8：单步跳过；
    *     F9：运行；
    *     CTR+F9：执行到返回（RET）。
    *     ALT+F9：执行到用户代码。可用于从系统领空快速返回到我们调试的程序领空 ？？？
    *
    * 示例：
	*   Ctrl + G(输入要跟随的表达式) , 输入OpenProcess
	*     kernel32!OpenProcess -> ntdll!NtOpenProcess == ntdll!ZwOPenProcess 
	*       -> KiFastSystemCall(EDX 指定SSDT的调用) -> sysenter 软中断切到内核 
	*         -> ntkrnlpa.ZwOpenProcess -> SSDT -> ntkrnlpa.NtOpenProcess
	* 
	* IDA -- 逆向
	*********************************************************************************************************/

    /*********************************************************************************************************
    * Reflector -- 反编译CLR工程的工具
    *   
    *********************************************************************************************************/

    /*********************************************************************************************************
    * 在Release版本中设置断点: _asm int 3 (或 ntdll!DbgBreakPoint ?), 会产生 0x80000003 的异常
    * Release 中的 this 指针一般会保存在 ecx 寄存器中，但有可能优化后会放在 ESI 等中（TODO: 怎么区分确认?）
    *  在命令窗口里通过: eval ecx 查看 ?
	*   
	* 使用 TRACE/ATLTRACE 打印带有中日韩的文字时，可能会报" _CrtDbgReport: String too long or IO Error "的错误，而无法打出日志：
	*   原因: wprintf_s 不能正确输出中日韩的 UNICODE 文字(似乎VS2010后修复了这个Bug？)
	*   补充资料(尚未仔细研究)：http://www.cnblogs.com/hnrainll/archive/2011/05/07/2039700.html
	*   解决方法：
	*     1.使用FTLTRACE(内部使用 StringCchVPrintfEx + OutputDebugString )
	*     2.#include <locale.h>
	*       TCHAR* old_locale = _tsetlocale( setlocale(LC_ALL,NULL) );
	*       _tsetlocale( LC_ALL, _T("chs") );    或 "", "korean", 
	*       TRACE(XXXX);
	*       _tsetlocale( LC_ALL, old_locale); 
	*       free(old_locale); 
    *
    *      locale_string = setlocale(LC_CTYPE, "Japanese_Japan"); if (locale_string == NULL) { "Japanese locale not enabled" }
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
