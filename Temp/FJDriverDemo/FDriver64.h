#ifndef F_DRIVER_64_H
#define F_DRIVER_64_H

/******************************************************************************************************************
* 64位平台的驱动开发
* 
* 汇编
*   64位编译器不再支持内嵌式的汇编代码(__asm) -- 可以将汇编代码提取出函数写在单独的汇编文件中，编译成obj后通过 TARGETLIBS 链接
*   MS的编译器是 ml64.exe；其他的有 GoASM(GoASM编译器 + GoLINK链接器 + GoRC资源编译器)等
*    
*   Windows X64汇编入门 -- http://bbs.pediy.com/showthread.php?t=43967
*     X32中原有的寄存器在X64中均为扩展为64位，寄存器从小到大依次为 AL/AH(8) -> AX(16) -> EAX(32) -> RAX(64)
*     x64中前四个参数分析通过四个寄存器传递：RCX、RDX、R8、R9，有更多的参数时才通过椎栈(RDI指向的内存)传递 -- 可以减少溢出的可能性
*     调用者负责椎栈空间的分配与回收，？ 即调用前需要 sub rsp, （参数个数+0/1返回值)*8， 调用结束后需要 add rsp,(参数个数+0/1返回值)*8 
*     在Windows驱动程序调用汇编文件导出的函数 -- http://blog.sina.com.cn/s/blog_4b88b4cc0100bc6w.html
*     TODO:测试未通过。可以在source文件中加入对.asm文件引用， 如 AMD64_SOURCES=AMD64\test3.asm， 其他的有 I386_SOURCES/IA64_SOURCES
*     64位的汇编地址需要用"`"  符号(数字1左边的)连接？ 如 0xFFFFFFFF`FFB5C4F8
*
* SSDT表处理
*   1.64位系统不导出 KeServiceDescriptorTable, 会遇到 LNK2001: unresolved external symbol 的错误，
*     实际上在内存中是存在的，比如可以通过 dq KeServiceDescriptorTable 和 ln KeServiceDescriptorTable 查看(实际地址会变)
*     0: kd> dp KeServiceDescriptorTable
*        fffff800`02ab1840  fffff800`0287bb00 00000000`00000000     <== 第一个 KeServiceDescriptorTable
*        fffff800`02ab1850  00000000`00000191 fffff800`0287c78c     <== SSDT的个数是0x191个
*        fffff800`02ab1860  00000000`00000000 00000000`00000000
*        fffff800`02ab1870  00000000`00000000 00000000`00000000
*        fffff800`02ab1880  fffff800`0287bb00 00000000`00000000		<== 第二个 KeServiceDescriptorTable -- 实际是 KeServiceDescriptorTableShadow ?
*        fffff800`02ab1890  00000000`00000191 fffff800`0287c78c		<== SSDT的个数是0x191个
*        fffff800`02ab18a0  fffff960`00131c00 00000000`00000000		<== Shadow SSDT
*        fffff800`02ab18b0  00000000`0000033b fffff960`0013391c		<== Shadow SSDT个数是0x33b个
*     0: kd> ln KeServiceDescriptorTable -- 有两个？ fffff800`02ab1840 和 fffff800`02ab1880
*        (fffff800`02ab1840)   nt!KeServiceDescriptorTable   |  (fffff800`02ab1880)   nt!KeServiceDescriptorTableShadow
*     0: kd> ln KeServiceDescriptorTableShadow  -- 显示 Shadow SSDT ? -- 为什么其地址和第二个 KeServiceDescriptorTable 一样？
*        (fffff800`02ab1880)   nt!KeServiceDescriptorTableShadow   |  (fffff800`02ab18c0)   nt!KeUserCallbackDispatcher
*     目前只能通过搜索导出函数(如 KeAddSystemServiceTable)的特征码搜索找出 KeServiceDescriptorTable 的地址
*
*   2.引入的 PatchGuard 技术，当Hook SSDT 函数时会 BSOD
*   
* 在64位系统，内核函数开头地址的低四位一般是0，形如:xxxxxxxx`xxxxxxx0(搜索时可以利用？)
*
* 代码中检查64位系统
*   1.IA32_EFER.LMA
*   2.if(sizeof(ULONG_PTR)==4)  32位 
*   
******************************************************************************************************************/


/*
[notes]x64的工程里编译ASM文件(R0和R3)   -- http://hi.baidu.com/ithurricane
做一个记录而已。。。
x64里由于无法内联汇编语句，所以必须专门写asm文件来编译汇编语言，
先总结一下R0里面如何编译：
1. 下载MASM64编译器（Tesla.Angela修改版），下载地址：http://good.gd/1399004.htm
然后写一个编译的bat文件（假设装在C盘下面，项目在D:\Devlopment\test，asm文件叫x64.asm）
@Set ProgName=64bit
@Color 0E
@Title %ProgName%
@call C:\Masm64\Env.Cmd
@set Directory="D:\Devlopment\test"
@Set SrcName=x64
@cd %Directory%
@del "%Directory%\%SrcName%.obj"
@C:\Masm64\BIN\x64\ml64 /c "%Directory%\%SrcName%.asm"
@pause   
2.修改sources文件
添加 AMD64_SOURCES=x64.asm和TARGETLIBS = x64.obj这两行
3.最后修改一下mybuild.bat文件
ddkbuild.cmd -WLHX64 chk . -cZ -WDF，用64位来编译
下面是R3的工程编译ASM文件，我本来想在设定里面搞定的，无奈一直无法成功，只好直接修改工程文件
1. VS2008的工程文件：
修改**.PowerTool64.vcproj
添加一个Filter节点，内容如下
<Filter Name="ASM" >
<FileRelativePath=".\ASM\x64.asm">
<FileConfiguration Name="Release|x64">
<Tool Name="VCCustomBuildTool"
CommandLine="cd "$(IntDir)" ml64.exe /D_WIN32 /c /Cx "$(InputPath)""

*/

#endif //F_DRIVER_64_H