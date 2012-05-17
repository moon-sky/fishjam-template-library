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
*     完整性级别(IL)低的进程不能修改级别高的文件或注册表表项。
*     1.非信任级别 -- 被设置给匿名空连接会话
*     2.Low(低) -- 保护模式下的IE浏览器, 以 Untrusted 的系统权限运行程序，只能存取低存取级别的路径位置，如 "Temporary Internet Files\Low"。
*       注意：未经用户同意就下载的行为运行在低级别，
*             用户手动的下载或故意的执行激活一个内容 运行在中等级别，
*             被一个提升权限的用户所同意的内容(例如安装一个ActiveX控件)则运行在高完整性级别
*     3.Medium(中等) -- 默认情况下，用户级别的代码，如Windows Explorer和任务管理器, 使用"User"权限，能读写用户的文件和注册表项
* 　　4.High(高) -- 真正的 administrator用户 或提升权限后的程序
* 　　5.System(系统) -- 内核级别的Windows文件
*     6.保护进程级别 -- 级别最高，只有在系统需要的时候才会被使用
*   应用文件和注册虚拟化 -- 对指定位置进行读写的时候会被重定向到每个用户的虚拟化的区域，如
*     C:\Users\%USER_NAME%\AppData\Local\Microsoft\Windows\Temporary Internet Files\Virtualized\XXXX -- XXX部分即是应该的 C:\ProgramData\MyProgramData 等
*     以下注册表进行控制？：HKLM\SYSTEM\CurrentControlSet\services\luafv
*   SFP -- 系统文件保护, Win2K 以前的文件保护机制
*   UAC(User Account Control) -- 用户帐户控制
*   User Interface Privilege Isolation -- 用户界面特权隔离
*   WFP -- Windows文件保护, Win2K时引入，只保护文件
*   WRP -- Windows资源保护，对关键文件、文件夹和注册表值进行保护
*
* IE 7 的保护模式(Protected Mode)
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
