#ifndef F_DRIVER_INF_STUDY_H
#define F_DRIVER_INF_STUDY_H
/******************************************************************************************************************
* INF -- Device INFormation File, 描述硬件参数和文件等数据信息的文件
* 
* 规则
*   1.由许多用方括号括起来的节组成，节与节之间没有先后顺序的区别。
*   2.每一个节又是由许多的条目组成的,每一个条目都是由形如 signature="$CHICAGO$" 的键值对，如有多值，用逗号","分开
*   3.分号(";")后面的内容为注释
*   
* 结构
*   [Defaultinstall]
*   [DestinationDirs] -- 目的的文件所在的目的路径
*   [Manufacturer] -- 描述INF文件可以识别的所有硬件设备,其中包含有设备的生产厂家,以便设备的正确安装
*   [SourceDisksNames.x86] -- 指明安装文件所在的介质, 可以再细分为 .x86, .amd64 等
*   [SourceDisksFiles.amd64] -- 驱动程序文件列表及被安装的位置，和 SourceDisksNames 相结合(通过其定义的数字)才能知道具体的位置
*   [String] -- 定义了会频繁在INF中使用的字符串变量
*   [Version] -- 描述支持的设备类型和适用的操作系统
*     signature => 使用的操作系统: "$CHICAGO$"(Win98后的系统), "$Windows NT$"(WinK/Xp/2003等)
*     CLASS => 设备的类型: Display(显示设备), Media(多媒体设备), Net(网络设备), Modem(调制解调器), Printer(打印), 
*              Image(图像捕获，如摄像头)
*     ClassGuid =>
*
* WDM相关的注册表
*   HKLM\SYSTEM\CurrentControlSet\Enum -- 驱动实例信息？
*   HKLM\SYSTEM\CurrentControlSet\Control\Class -- 类子健，每个设备都会从属于一个设备类(可是系统预定的，也可是自定义的)
*   HKLM\SYSTEM\CurrentControlSet\services -- 服务子健，兼容以前的NT式驱动程序
*
* 目录代码(用于 DestinationDirs 等地方, 具体参见 Using Dirids )
*   10 -- %SystemRoot%, 如 C:\Windows
*   11 -- %SystemRoot%\system32 , 如 C:\Windows\System32
*   12 -- %SystemRoot%\system32\drivers, 驱动目录
*   17 -- INF file directory
*   30 -- C盘根目录
*   31 -- Windows系统所在盘的根目录
* 
* Pnp式的驱动程序(WDM 等)需要借助INF文件，从控制面板的硬件向导安装(或通过 EzDriverInstaller 安装？) 
*   描述了WDM 驱动程序的操作硬件设备的信息和驱动程序的一些信息
*
******************************************************************************************************************/




#endif //F_DRIVER_INF_STUDY_H