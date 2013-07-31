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
*     x64中前四个参数分析通过四个寄存器传递：RCX、RDX、R8、R9，有更多的参数时才通过椎栈传递；
*     调用者负责椎栈空间的分配与回收，？ 即调用前需要 sub rsp, （参数个数+0/1返回值)*8， 调用结束后需要 add rsp,(参数个数+0/1返回值)*8 
*     在Windows驱动程序调用汇编文件导出的函数 -- http://blog.sina.com.cn/s/blog_4b88b4cc0100bc6w.html
*     TODO:测试未通过。可以在source文件中加入对.asm文件引用， 如 AMD64_SOURCES=AMD64\test3.asm， 其他的有 I386_SOURCES/IA64_SOURCES
*     64位的汇编地址需要用"`"  符号(数字1左边的)连接？ 如 0xFFFFFFFF`FFB5C4F8
*
* SSDT表处理
*   1.64位系统不导出 KeServiceDescriptorTable, 会遇到 LNK2001: unresolved external symbol 的错误，
*     目前只能通过搜索导出函数(如 KeAddSystemServiceTable)的特征码搜索找出 KeServiceDescriptorTable 的地址
*   2.引入的 PatchGuard 技术，当Hook SSDT 函数时会 BSOD
*   
******************************************************************************************************************/


#endif //F_DRIVER_64_H