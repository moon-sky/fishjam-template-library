#ifndef F_DRIVER_DEBUG_H
#define F_DRIVER_DEBUG_H

/******************************************************************************************************************
* SrvInstW.exe(http://www.onlinedown.net/soft/36059.htm)
*   将驱动作为服务的方式安装，然后可以通过 net start/stop 来启动/停止 驱动程序
*
* WinObj(http://technet.microsoft.com/en-us/sysinternals/bb896657.aspx)
*   可以显示所有的内核对象
*
* 通过DebugView查看DbgPrint打印的日志信息，需要选中 Capture Kernel 才能看到内核输出
*
* 虚拟机调试
*   VMWare
*     共享目录：Options -> Shared Foloders -> 增加共享目录, 该共享目录会出现在虚拟机中的“网络驱动器”里
*     用管道虚拟串口：Hardware -> Add -> Serial Port -> Output to named pipe -> 修改第三个选项为"The other end is an application"
*       调试机上启动windbg：windbg.exe -b -k com:port=\\.\pipe\com_1,baud=115200,pipe
*       刚连接上时，虚拟机里的Windows系统会被中断(貌似时机),需要在WinDbg的命令提示符后执行 "g" 命令允许其继续运行
*
*
******************************************************************************************************************/
/******************************************************************************************************************
* WinDbg(http://msdn.microsoft.com/en-us/windows/hardware/gg463009)
*   调试内核时必须是双机调试？通过串口相连？网络？
*   设置被调试机器为调试执行方式，WinDbg会把内核视为一个整体进行调试
*     WinXP：boot.ini 文件中拷贝并增加一个启动项 /debug /debugport=com1 /baudrate=115200
*     Vista：
*       bcdedit /enum OSLOADER <== 先枚举出本机上所有的"操作系统加载器"
*       bcdedit /copy {current} /d "Windows Debug Model"  <== 拷贝出一个调试模式的配置项
*       TODO: 设置刚拷贝出来的配置项为当前配置项？否则会更改到当前的正常配置(或者拷贝出一个 Original ?)
*       bcdedit /debug ON  <== 设置为调试
*       bcdedit /bootdebug ON 
*       bcdedit /dbgsetting  <== 查看当前的调试配置
*
*   重用命令(在中断状态下，通过F9键可以设置/取消断点)
*     g -- 中断的程序继续运行
******************************************************************************************************************/



#endif //F_DRIVER_DEBUG_H