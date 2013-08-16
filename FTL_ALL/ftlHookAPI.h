#ifndef FTL_HOOK_API_H
#define FTL_HOOK_API_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlHookAPI.h requires ftlbase.h to be included first
#endif

#  define DETOURS_VERIFY(x) \
	nRet = (x);\
	if(nRet != NO_ERROR)\
	{\
		REPORT_ERROR_INFO(FTL::CFAPIErrorInfo, nRet, x);\
	}
//http://bbs.csdn.net/topics/300138796 -- atlstdthunk.h 里面有 jmp rax 的代码

//卡巴斯基 在64BitWin7上的Hook
//3		NtUserGetKeyState				0xFFFFF96000131DCA	0xFFFFF96000131DD0	-	
//5		NtUserCallNoParam				0xFFFFF96000123E6A	0xFFFFF96000123E70	-	
//7		NtUserMessageCall				0xFFFFF960000FFBE6	0xFFFFF960000FFBEC	-	
//8		NtGdiBitBlt						0xFFFFF960001256BE	0xFFFFF960001256C4	-	
//15	NtUserPostMessage				0xFFFFF96000120756	0xFFFFF9600012075C	-	
//28	NtUserBuildHwndList				0xFFFFF960000C464E	0xFFFFF960000C4654	-	
//49	NtGdiStretchBlt					0xFFFFF9600024813A	0xFFFFF96000248140	-	
//68	NtUserGetAsyncKeyState			0xFFFFF9600012A5E6	0xFFFFF9600012A5EC	-	
//95	NtUserPostThreadMessage			0xFFFFF960000D428E	0xFFFFF960000D4294	-	
//105	NtGdiMaskBlt					0xFFFFF96000247222	0xFFFFF96000247228	-	
//110	NtUserFindWindowEx				0xFFFFF9600012FD12	0xFFFFF9600012FD18	-	
//119	NtUserSetParent					0xFFFFF9600012BF4E	0xFFFFF9600012BF54	-	
//120	NtUserGetKeyboardState			0xFFFFF96000131C4E	0xFFFFF96000131C54	-	
//130	NtUserSendInput					0xFFFFF9600012F83A	0xFFFFF9600012F840	-	
//140	NtUserSetWindowsHookEx			0xFFFFF960000CE096	0xFFFFF960000CE09C	-	
//236	NtUserAttachThreadInput			0xFFFFF96000129582	0xFFFFF96000129588	-	
//260	NtUserSetWinEventHook			0xFFFFF96000128152	0xFFFFF96000128158	-	
//586	NtGdiPlgBlt						0xFFFFF9600022F95E	0xFFFFF9600022F964	-	
//755	NtUserRegisterHotKey			0xFFFFF9600012B72A	0xFFFFF9600012B730	-	
//756	NtUserRegisterRawInputDevices	0xFFFFF9600013687A	0xFFFFF96000136880	-	
//811	NtUserUnregisterHotKey			0xFFFFF9600012CBE6	0xFFFFF9600012CBEC	-	


/******************************************************************************************************
* 有些情况下，必须打破进程的界限，访问另一个进程的地址空间，这些情况包括：
* 1.当你想要为另一个进程创建的窗口建立子类时 -- SetWindowLongPtr(hwnd,GWLP_WNDPROC,..) 改变窗口的内存块中的窗口过程地址。
* 2.当你需要调试帮助时（例如，当你需要确定另一个进程正在使用哪个DLL时）。
* 3.当你想要挂接其他进程时。
*   但需要将子类过程的代码放入目的进程的地址空间－－将DLL插入目的进程的地址空间。
*
* 注意：
*   1.如果注入的DLL生成了任何新的线程，请确保在程序退出时，退出线程。
*   2.远程注入后，需要通过进程间通信的方式交互 -- 如 FileMapping、WM_COPYDATA、Clipboard、#pragma data_seg 等
*     #pragma data_seg (".shared")
*       UINT    WM_HOOKEX = 0;
*       HHOOK   g_hHook = 0;
*       XXXX    g_MyDATA = yyy; //初始化
*     #pragma data_seg ()
*     #pragma comment(linker,"/SECTION:.shared,RWS")  //设置共享节
*   3.通常都在DLL中的 DllMain 中响应 DLL_PROCESS_ATTACH 来处理 
*       可能需要调用 DisableThreadLibraryCalls 来禁止本DLL 接收线程通知消息；
*   4.通过调用 RegisterWindowMessage 注册消息并进行共享，则 Exe 和 注入的DLL可以交互。
*       DLL 的 DllMain 中：if(WM_HOOKEX == 0) WM_HOOKEX = ::RegisterWindowMessage(TEXT("WM_HOOKEX_RK"));
*       Exe 中：
*
* 常用方法
*   a. 使用注册表来插入DLL
*      HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows 的 AppInit_DLLs ，包含一个DLL文件名或者一组DLL文件名
*      (用空格或逗号隔开)。列出的第一个DLL文件名可以包含一个路径 －－ 最好将你的DLL放入 Windows 的系统目录中，这样就不必设定路径。
*      系统启动时，有 User32.dll 依次调用DLL中的 DllMain(DLL_PROCESS_ATTACH)
*      不足：只能在系统重启后，加载到所有使用User32.dll 的 GUI 程序中；不支持Win98
*
*   b.使用 Windows 挂钩来插入DLL。有针对单个线程的Hook和针对整个系统的Hook(具体是哪个钩子无所谓，但一般好像是 WH_GETMESSAGE/WH_CALLWNDPROC ?)
*       系统Hook： HHOOK hHook = SetWindowsHookEx(WH_GETMESSAGE,GetMsgProc,hInstDll,0);
*       线程Hook： g_hHook = SetWindowsHookEx( WH_CALLWNDPROC,HookProc,hInstDll, GetWindowThreadProcessId(hWndParam,NULL));
*     当系统插入或者映射包含挂钩过滤器函数的DLL时，整个DLL均被映射，而不只是挂钩过滤器函数被映射。
*     这意味着DLL中包含的任何一个函数或所有函数现在都存在，并且可以从目的进程的环境下运行的线程中调用。
*     Sample：跨进程获取桌面 ListView 中各个项目的位置。
*
*   c.使用远程线程来插入DLL -- 具有更大的灵活性(DLL 中执行 DLL_PROCESS_ATTACH 通知的 DllMain 函数 )
*     CreateRemoteThread/NtCreateThreadEx(其参数 pfnStartAddr指明线程函数的内存地址,其纤程函数的代码不能位于自己进程的地址空间中)，
*     需要该线程调用 LoadLibrary 函数来加载我们的DLL。创建一个新线程，并且使线程函数的地址成为 LoadLibraryA 或LoadLibraryW函数的地址。
*     TODO：？使用 VirtualQueryEx 函数把存放指令的页面的权限更改为可读可写可执行，再改写其内容，从而修改正在运行的程序 
*     1).使用 VirtualAllocEx 函数，分配远程进程的地址空间中的内存。
*     2).使用 WriteProcessMemory 函数，将DLL的路径名拷贝到第一个步骤中已经分配的内存中。
*     3).使用 GetProcAddress 函数，获取 LoadLibraryA 或 LoadLibraryW 函数的实地址（在Kernel32.dll中）
*     4).使用 CreateRemoteThread 函数，在远程进程中创建一个线程，它调用正确的 LoadLibrary 函数，为它传递第一个步骤中分配的内存的地址。
*        这时,DLL已经被插入远程进程的地址空间中，同时DLL的 DllMain 函数接收到一个 DLL_PROCESS_ATTACH 通知，并且能够执行需要的代码。
*        当 DllMain 函数返回时，远程线程从它对 LoadLibrary 的调用返回,使远程线程终止运行。
*        现在远程进程拥有第一个步骤中分配的内存块，而DLL则仍然保留在它的地址空间中。
*     5).调用GetExitCodeThread获取远程线程的退出码 -- 即LoadLibrary函数的返回值，表明映射的DLL的基址，可以保留并用于以后释放清除操作：
*     6).使用 VirtualFreeEx 函数，释放第一个步骤中分配的内存。
*     7).使用 GetProcAddress 函数，获得 FreeLibrary 函数的实地址。
*     8).使用 CreateRemoteThread 函数，在远程进程中创建一个线程，它调用 FreeLibrary 函数，传递远程DLL的HINSTANCE。
*
*   d.使用特洛伊DLL来插入DLL
*     取代你知道进程将要加载的DLL(自己的DLL取名为需要替换的DLL，将需要替换的DLL换成别的名字，使用 "函数转发器" 输出原始DLL的符号)
*     --不具备版本升级能力
*     或改变需要插入DLL的输入节
*   
*   e.将DLL作为调试程序来插入
*     调试程序能够对被调试的进程执行特殊的操作。当被调试进程加载时，在被调试进程的地址空间已经作好准备，
*     但是被调试进程的主线程尚未执行任何代码之前，系统将自动将这个情况通知调试程序。这时，调试程序可以强制
*     将某些代码插入被调试进程的地址空间中(比如使用 WriteProcessMemory 函数来插入)，然后使被调试进程的主线程执行该代码。
*     要求对被调试线程的 CONTEXT 结构进行操作，意味着必须编写特定CPU的代码
*   
*   f. 用 CreateProcess 插入代码 -- 需要自己的进程生成了想插入代码的新进程。
*     1).使你的进程生成暂停运行的子进程;
*     2).从.exe 模块的头文件中检索主线程的起始内存地址。
*     3).将机器指令保存在该内存地址中。
*     4).将某些硬编码的机器指令强制放入该地址中。这些指令应该调用 LoadLibrary 函数来加载DLL。
*     5).继续运行子进程的主线程，使该代码得以执行。
*     6).将原始指令重新放入起始地址。
*     7).让进程继续从起始地址开始执行，就像没有发生任何事情一样
*   
*   g. 使用 WriteProcessMemory 在远程进程中写入执行代码，并使用 CreateRemoteThread(Vista前) 或 NtCreateThreadEx(Vista及其后) 执行。
*     1).使用 VirtualAllocEx 在远程进程中分配 插入代码 和 参数(INJDATA)的内存；
*     2).使用 WriteProcessMemory 写入"插入代码"和“参数”的拷贝；
*     3).使用 CreateRemoteThread 开始执行 插入代码；
*     4).使用 WaitForSingleObject 等待线程结束；
*     5).使用 ReadProcessMemory 或 GetExitCodeThread 获取远程线程的执行结果；
*     6).示范相关资源（VirtualFreeEx、CloseHandle）。
*     优点：
*       可以只有一个EXE(不需要DLL文件)；
*     缺点：
*       非常复杂并且容易出错。
*       远程方法只能执行 kernel32.dll 和 user32.dll 中的方法；
*       不能使用 /GZ 编译选项（Debug中的缺省值） -- 会在函数开头增加调试用的代码，在远程进程中不存在。
*       线程函数必须是 static 的(被直接调用)，或者禁止 incremental linking(通过 JMP 调用，远程进程中地址不同)
*       线程函数中的本地变量必须在一个 Page 宽度内（4K）--ESP不再直接变化，而调用函数(远程进程中地址不同)进行调整 
*       switch 中不能使用过多（超过3个）的case，最好换成 if..else if -- 超过3个时会在编译时创建使用绝对地址的跳转地址表，
*         通过偏移量跳转（提高性能），但跳转表在远程进程中不存在。
*         
*     注意：使用 VirtualAllocEx 时必须指定 PAGE_EXECUTE_READWRITE 属性 -- 能在写入代码的地方执行
*
*
******************************************************************************************************/

/******************************************************************************************************
* Hook API
*   实现方式
*     1.改引入表式(import/export) -- 易于实现，OpenProcess + WriteProcessMemory 改写函数地址
*     2.陷阱式(inline) -- 可以用来hook内部函数，而且调用者很难绕过hook
*         通常需要完备的反汇编器和解释器
*   
* 已有的库
*   1.EasyHook：http://easyhook.codeplex.com/
*               https://code.google.com/p/easyhook-continuing-detours
*       使用的例子:http://blog.csdn.net/baggiowangyu/article/details/7675098
*     支持驱动中Hook，支持X64注入，且完全免费，License为 LGPL, 作者通过 IDA Pro 5.0反汇编 Ildasm.exe 生成目录DriverShared\ASM下主要核心汇编代码(Trampoline_ASM_x86/x64)
*     提供了两种模式的注入管理：托管代码 和 非托管代码(例子为 UnmanagedHook 和 TestDriver)，通过Inline Hook 方式实现
*     示例:
*       TRACED_HOOK_HANDLE hHookCreateFileW = { NULL };  
*       ULONG HookCreateFileW_ACLEntries[1] = {0}; 
*       LhInstallHook(OriginalCreateFileW, MyCreateFileW, NULL, &hHookCreateFileW);
*       LhSetExclusiveACL(HookCreateFileW_ACLEntries, 1, hHookCreateFileW);  
*       调用函数
*       LhUninstallAllHooks();
*       LhUninstallHook(&hHookCreateFileW);  
*       delete hHookCreateFileW;  
*       LhWaitForPendingRemovals(); //等待内存释放
*     源码分析:
*       LhInstallHook -> LhRelocateEntryPoint -> 
* 
*	  Detours源码分析(参见 Detours.chm )：
*       概念:
*         Source Function -- 用户调用的函数
*         Target Function -- 被调用的函数
*         Detour Function -- 自己定义的替代函数
*         Trampoline -- 从Detour Function 跳转回Target的一个函数，实质是 Target的一部分加了个跳转指令，
*       流程:
*         Hook前: Source -> 调用Target -> 返回Source
*         Hook后: Source(Target中直接Jmp到Detour) -> Detour [ -> Trampoline -> Target -> 返回Detour ] -> 返回Source
*       相关结构和类:
*         _DETOUR_TRAMPOLINE -- 平台相关(即 Trampoline 函数体？)，保存了多种信息：
*            Target 被截掉的代码(rbRestore？)、长度(cbCode)、Target处理后首条指令的地址(pbRemain)、Detour函数的地址(pbDetour)
*         DetourOperation -- 链表结构，记录了Target首条指令的地址(ppbPointer?)、
*       实现函数
*         DetourCodeFromPointer -- 内部调用 detour_skip_jmp得到函数的真正位置。调用动态链接库时通常通过 "JMP 地址" 这种方式来进行，
*         因此通过该函数找到代码的真正位置。
*         
*   2.Detours：http://research.microsoft.com/en-us/projects/detours
*     微软的开源研究库，免费版本不能用于商业，商业版本需要 10K USD。支持x64和IA64等64位平台
*     编译：nmake，TODO: Debug/Release？ 编译时可能需要更改 samples\common.mak 文件，去掉其中的 /nologo 
*     缺陷：x86无法hook小于5字节的函数，因为没有完整的反汇编器，无法处理后面的代码 jmp 到函数前五个字节范围内(刚好被替代) 的代码逻辑
*     原理：在汇编层改变目标API出口和入口的一些汇编指令
*       几部分：
*         Target函数：要拦截的函数，通常为Windows的API，被Hook后其最开始部分的汇编被更改为：jmp DetourFunction
*         Trampoline函数(蹦床,原始被替代的代码 + JMP到Target)：Target函数的部分复制品。
*           因为Detours将会改写Target函数，所以先把Target函数的前5个字节复制保存好，
*           一方面仍然保存Target函数的过程调用语义，另一方面便于以后的恢复。然后是一个无条件转移
*         Detour函数：用来替代Target函数的截获函数
*       源码分析：
*         1.
*     Detours同样提供了DLL注入的方式，它可编辑任何DLL或EXE导入表的功能，达到向存在的二进制代码中添加任意数据节表的目的
*     有效负荷(Payloads) -- 可以对Win32二进制文件添加、卸载数据节表(.detours Section)以及编辑DLL导入表
*     实现注意：
*       1.需要将DLL复制到目标进程目录 或 System32(SysWOW64 -- 64位上的32位程序时)
*       2.通过 #pragma data_seg("MyShare") + #pragma comment(linker,"/SECTION:MyShare,RWS") 将特定变量加入共享数据段
*       3.DLL_PROCESS_ATTACH + DLL_THREAD_ATTACH 时进行Hook？ 
*         DLL_PROCESS_DETACH + DLL_THREAD_DETACH 时进行UnHook？
*       4.是否需要该文件？ detoured.dll 是一个标志，帮助微软的开发人员和工具判断某个进程是否已经被detours拦截
*       5.32 <==> 64 互相插入时，会生成辅助进程(HelperProcess)？
*       6.可以静态或动态地创建：区别？ComicViewer中不能拦截视频的BitBlt？
*       A1.一定要枚举线程并调用DetourUpdateThread函数。否则可能出现很低几率的崩溃问题，这种问题很难被检查出来。
*       A2.如果拦截函数在DLL中，那么绝大多数情况下不能在Unhook之后卸载这个DLL，或者卸载存在造成崩溃的危险。
*     使用方式：
*       1.普通：DetourTransactionBegin => DetourUpdateThread => DetourAttach/DetourDetach => DetourTransactionCommit
*       2.类成员函数 -- 定义CXxxHook类(注意：尚未测试 -- http://wenku.baidu.com/view/7b441cb665ce05087632133b.html)
*         a.定义static的函数指针： static int (CXxxHook::* TrueFun)(xxxx) = &CTrueXxx::Fun;
*         b.定义 FilterFun 的成员函数，其中可以通过 (this->*TrueFun)(xxx) 的方式调用原来的实现
*         c.DetourAttach(&(PVOID&)CXxxHook::TrueFun, (PVOID)(&(PVOID&)CXxxHook::FilterFun));
*     主要的API
*       DetourCreateProcessWithDllEx -- 在启动指定进程的时候注入DLL(该DLL最少要有一个导出函数)
*       DetourIsHelperProcess -- 判断当前是否在辅助进程里,通过在DllMain中，该函数返回TRUE时，直接 return TRUE;
*       DetourRestoreAfterWith -- 恢复初始状态(在 DllMain::PROCESS_ATTACH 时调用？)
*       DetourUpdateThread -- 
*       DetourFindFunction -- 动态找到函数指针的真实地址给 TrueXxx 赋值?
*     工具:
*       SetDll.exe -- 可以在Exe文件中增加对指定DLL的引用(加入.detours节，会更改可执行文件),
*         注意DLL需要有序号为"1" 的导出函数(Makefile 中有: /export:DetourFinishHelperProcess,@1,NONAME )
*         TODO:def 文件中如何设置？
*       WithDll.exe -- 在注入指定DLL的情况下创建并运行指定进程(纯内存操作，可执行文件不受影响)
*     TODO:
*       是否需要调用 DisableThreadLibraryCalls ?
*     移植到 x64 的 Kernel
*       1.VirtualProtect让内存 READ_WRITE_EXECUTE -> 清除cr0 或 使用 native API 完成VirtualProtect的功能( NtProtectVirtualMemory )
*       2.x64下不存在 jmp 64_address这种指令

*   3.Deviare(不支持C/C++?  http://www.nektra.com/products/deviare-api-hook-windows/)
*   4.MinHook -- 日本人写的，比较简单，只有Hook功能。
*     其使用了 Relay 函数来支持最小5字节的函数。VirtualAlloc 在跳转函数附近分配空间，先使用32位的JMP跳转到其位置，然后再进行64位的跳转。
* 
* TODO(FUAPIHook.h 中已有) -- http://wenku.baidu.com/view/5ae307f04693daef5ef73d48.html
*   Win64的：http://bbs3.driverdevelop.com/read.php?tid=99096
* http://blog.csdn.net/zzw315/article/details/4102488
******************************************************************************************************/

/******************************************************************************************************
* 应用级的全局 Hook API：SetWindowsHookEx + Detours -- TODO: 单独的 ftlHookAPI(DUI 中 HookAPI 源码，比较)
*   缺点（？）：
*     1.需要管理员权限
*     2.不稳定，依赖于其他WindowsHook的实现(Hooks 是一个合作机制，没有任何保证，其他的实现不调用 CallNextHookEx 就不能进行处理)？
*     3.可能被反病毒软件提醒？
*   注意：对应的有 驱动级的全局 Hook API(参见 FDriverHookAPI.h )
*   钩子机制 -- 优先处理特定消息、事件的代码段，每一类Hook都是一个链表(钩子链，Hook Chains)，
*     每一项都是一个应用程序定义的钩子处理函数，每一个处理函数都有责任通过调用 CallNextHookEx 保证之前的过滤函数被调用，
*     最近安装的钩子放在链的开始，优先获得控制权。
*     
*   SetWindowsHookEx
*     系统钩子() -- 必须在DLL中，系统自动将该DLL注入受影响的进程地址空间
*     线程钩子 -- 
*    
*     IAT(对于加壳的软件无效) ? 
*     JMP 方式 -- 可以对付加壳软件   
*  
*   远程注入：
*     1.Ring3: CreateRemoteThread + LoadLibrary
******************************************************************************************************/

/******************************************************************************************************
* 常见反汇编代码(默认 x86, 前面有三个星号的是 x64 上的)
*   0x26 -- jmp es:
*   0x2e -- jmp cs:
*   0x33c0 -- xor     eax,eax
*   0x36 -- jmp ss:
*   0x488d -- lea xxxx
*** 0x48b8 XXXXXXXX`XXXXXXXX -- mov rax, Xxxxxx; 将指定的64位数据放入 rax 
*   0x50 -- push eax / rax
*   0x58 -- pop eax / rax
*   0x64 -- jmp fs:
*   0x65 -- jmp gs:
*   0x74 XX -- je imm8
*   0x90 -- nop	, 一般用于填充代码区域
*   0xB8 XXXXXXXX -- mov eax, xxxx , 将指定的32位数据放入 eax 
*   0xc2 -- ret +imm8
*   0xc3 -- ret
*   0xcc -- int 3, 软件断点，一般用于代码空余部分的填充
*   0xe0 -- jmp eax
*   0xe3 -- jmp ds 还是 jcxz imm8 ?
*   0xe8 XXXXXXXX -- call imm16/imm32
*   0xe9 XXXXXXXX -- jmp imm16/imm32 (jmp offset, offset大于0向前挑,offset小于0向后跳) ; x64 下对应 ff15  [xxxxxxxx], xxxxxxxx 是32位的，[xxxxxxxx]指向一个64位地址
*   0xeb -- jmp imm8
*   0xff25 -- jmp [+imm32] -- 根据指定内存中保存的 Address Table 进行跳转? JMP QWORD NEAR，
*     跳转目标是指定8字节内存中的地址(64位系统上可以直接跳转到全局目的地)。可以指定8字节地址为0,即根据当前指令后偏移位置为0的地址进行跳转
*   0xffe0 -- jmp rax -- 根据 rax 寄存器的值进行跳转，即 jmp imm64 ?
* 
*
* 64位系统下，如要跳转到64位的绝对地址，需要通过寄存器来实现, 如(EasyHook的实现方式)
*   48 B8 8877665544332211  MOV RAX,1122334455667788H 
*   ffe0                    jmp rax
*
* 当前指令地址： x86下是 EIP， x64下是 RIP
* JMP有好几种形式(无条件的相对位置跳转, 新的地址 = EIP/RIP + OFFSET + sizeof(JMP OFFSET) )
*   因此，代码中需要计算的 偏移 = 目的地址 - (From + sizeof(JMP OFFSET) )
*   0xe9 -- 32位上覆盖整个地址空间
*
******************************************************************************************************/

namespace FTL
{
}
#endif //FTL_HOOK_API_H

#ifndef USE_EXPORT
#  include "ftlHookAPI.hpp"
#endif 