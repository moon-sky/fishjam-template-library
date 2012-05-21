#ifndef FTL_IE_HELPER_H
#define FTL_IE_HELPER_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlIEHelper.h requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlIEHelper.h requires ftlCom.h to be included first
#endif


/********************************************************************************************
* 安全控制
*   Security Principal -- Windows信任的安装安全主体
* 
*   DEP -- 数据执行保护
*   MIC(Mandatory Integrity Control) -- 强制完整性控制。Vista中的所有安全性对象和进程都有一个完整性级别，
*     完整性级别(Integrity Level -- IL)低的进程不能修改(可以读取?)级别高的文件或注册表表项。
*     注意：Win2K/XP 下，安全子系统只把进程的访问令牌和资源的访问控制列表进行匹配比较，以确认该进程是否具有访问该资源的权限。
*     1.非信任级别 -- 被设置给匿名空连接会话
*     2.Low(低) -- 保护模式下的IE浏览器, 以 Untrusted 的系统权限运行程序，只能存取低存取级别的路径位置，如 "Temporary Internet Files\Low"。
*       注意：未经用户同意就下载的行为运行在低级别，
*             用户手动的下载或故意的执行激活一个内容 运行在中等级别，
*             被一个提升权限的用户所同意的内容(例如安装一个ActiveX控件)则运行在高完整性级别
*     3.Medium(中等) -- 默认情况下，用户级别的代码，如Windows Explorer和任务管理器, 使用"User"权限，能读写用户的文件和注册表项
* 　　4.High(高) -- 真正的 administrator用户 或提升权限后的程序
* 　　5.System(系统) -- 内核级别的Windows文件
*     6.保护进程级别 -- 级别最高，只有在系统需要的时候才会被使用
*     
*   应用文件和注册虚拟化(虚拟重定向) -- 对指定位置进行读写的时候会被重定向到每个用户的虚拟化的区域，
*     在以下位置建立了一个和"用户配置文件夹"完全一致的且完整性级别为"Low"的目录层次:
*     %UserProfile%\AppData\Local\Microsoft\Windows\Temporary Internet Files\Virtualized\XXXX -- XXX部分即是应该的 C:\ProgramData\MyProgramData 等
*
*     以下注册表进行控制？：HKLM\SYSTEM\CurrentControlSet\services\luafv
*   SFP -- 系统文件保护, Win2K 以前的文件保护机制
*   UAC(User Account Control) -- 用户帐户控制， 让管理员帐户自动获得一个标准用户的访问令牌，以减少Windows Vista系统的受攻击面
*   UIPI(User Interface Privilege Isolation) -- 用户界面特权隔离，完整性级别低的进程，不能向完整性级别高的进程发送Window消息。
*   WFP -- Windows文件保护, Win2K时引入，只保护文件
*   WRP -- Windows资源保护，对关键文件、文件夹和注册表值进行保护
*
* IE 7 的保护模式(Protected Mode) -- 也叫IE低权利(Low Rights)，通过 Process Explorer 查看进程的"安全"属性，可看到有
*   "Mandatory Label\Low Mandatory Level"的SID(相应的标志位是"Integrity")，这表明IE进程的完整性级别是"Low"。
*   "Virtualized" 为 Yes
*   保护模式下访问的文件目录(完整性级别都是"Low")：
*     1.IE临时文件: %UserProfile%\AppData\Local\Microsoft\Windows\Temporary Internet Files\Low
*     2.系统临时目录: %UserProfile%\AppData\Local\Temp\Low
*     3.Cookies: %UserProfile%\AppData\Roaming\Microsoft\Windows\Cookies\Low
*     4.历史: %UserProfile%\AppData\Local\Microsoft\Windows\History\Low
*     5.收藏夹: 收藏夹默认允许低级别的IE进程访问，因此不需要独立的目录？
*   解决兼容性问题：
*     1.通过建立NTFS软链接(Soft Link)的方法来解决IE保护模式的兼容性问题：linkd 源目录 低权限的目的目录
*       实质：给NTFS文件增加一个重解析点(REPARSE)的属性
*       Linkd 可执行程序在 Windows Server 2003 Resource Kit Tools工具包 中
*     2.SHGetSpecialFolderPath(NULL, szSavePath, CSIDL_LOCAL_APPDATA , TRUE); //取得 %UserProfile%\AppData\Local
*       CString strFilePath(szSavePath); 
*       strFilePath += TEXT("\\Temp\\Low\\nCapture\\IEToobar\\");  -- 之后的访问都在具有 Low 完整性等级的目录下
*   
* 代理进程(特权分离思想) -- 把传统的IE进程一分为三，分别负责不同安全等级的任务.
*   IE进程本身 -- 完成最常用的、无需特权的网页浏览任务
*   IeUser.exe(用户级代理进程) -- 完成中等特权的任务(例如保存图片等)
*   IeInstal.exe(管理级代理进程) -- 完成高特权的任务(例如安装加载项)。
* 
* IE中的多进程控制
*   HKLM或HKCR \SOFTWARE\Microsoft\Internet Explorer\MAIN\TabProcGrowth  -- REG_SZ, 为 0 表示， 为 Medium 表示
*
* 权限检查工具：AccessChk -- 可知道指定账号对特定目录的权限和完整性级别等
*   AccessChk -i 目录 -- 查看目录下文件的完整性级别
*   AccessChk -d -q %AppData% 
********************************************************************************************/

namespace FTL
{
    class CFIEUtils
    {
    public:
		FTLINLINE static BOOL IsProtectedModeProcess();
    };

}//namespace FTL

#ifndef USE_EXPORT
#  include "ftlIEHelper.hpp"
#endif

#endif //FTL_IE_HELPER_H
