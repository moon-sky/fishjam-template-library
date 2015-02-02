#ifndef FTL_USER_H
#define FTL_USER_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlUser.h requires ftlbase.h to be included first
#endif

#pragma TODO(从 FUUser 文件中同步)
//用户帐号控制(UAC) -- http://www.cnblogs.com/rainbowzc/archive/2012/02/28/2371526.html
/*********************************************************************************************************************************
* 安全控制
*   ACE(Access Control Entry) -- 
*   Security Context -- 安全上下文，定义某个进程允许做什么的许可和权限的集合，通过登录会话确定，通过访问令牌维护。
*   SID(Security Identifier) -- LookupAccountName 等函数获取
*   Security Principal -- Windows信任的安装安全主体
*   LUID -- locally unique identifier
*
*   DEP -- 数据执行保护
*   NTLM -- NT LAN Manager(win2000早期版本中的一种验证身份方式), 基于一种"提问-答复"机制来进行客户端验证
*   MIC(Mandatory Integrity Control) -- 强制完整性控制。Vista中的所有安全性对象和进程都有一个完整性级别，
*     完整性级别(Integrity Level -- IL)低的进程不能修改(可以读取?)级别高的文件或注册表表项。
*     注意：Win2K/XP 下，安全子系统只把进程的访问令牌和资源的访问控制列表进行匹配比较，以确认该进程是否具有访问该资源的权限。
*     1.Untrusted(非信任级别) -- 被设置给匿名空连接会话, TODO: Chrome.exe 的部分进程显示为这个
*     2.Low(低) -- 保护模式下的IE浏览器, 以 Untrusted 的系统权限运行程序，只能存取低存取级别的路径位置，如 "Temporary Internet Files\Low"。
*       注意：未经用户同意就下载的行为运行在低级别，
*             用户手动的下载或故意的执行激活一个内容 运行在中等级别，
*             被一个提升权限的用户所同意的内容(例如安装一个ActiveX控件)则运行在高完整性级别
*     3.Medium(中等) -- 默认情况下，用户级别的代码，如Windows Explorer和任务管理器, 使用"User"权限，能读写用户的文件和注册表项
*     4.High(高) -- 真正的 administrator用户 或提升权限后的程序
*     5.System(系统) -- 内核级别的Windows文件
*     6.保护进程级别 -- 级别最高，只有在系统需要的时候才会被使用
*     
*   应用文件和注册虚拟化(虚拟重定向) -- 对指定位置进行读写的时候会被重定向到每个用户的虚拟化的区域，
*     在以下位置建立了一个和"用户配置文件夹"完全一致的且完整性级别为"Low"的目录层次:
*     %LocalAppData%\Microsoft\Windows\Temporary Internet Files\Virtualized\XXXX -- XXX部分即是应该的 C:\ProgramData\MyProgramData 等
*     HKCU/Software/Microsoft/InternetExplorer/InternetRegistry -- TODO:尚未确认
* 
*     以下注册表进行控制？：HKLM\SYSTEM\CurrentControlSet\services\luafv

*   LSA(Local Security Authority) -- 控制本地权限的API
*     权限管理API流程：
*       LsaOpenPolicy -> LookupAccountName/LsaLookupNames(get SID) -> LsaAddAccountRights/LsaRemoveAccountRights(增加或减少权限) -> LsaClose
*         LsaEnumerateAccountRights(枚举用户权限), LsaEnumerateAccountsWithUserRight(枚举有指定权限的用户)
* 
*   FUS(Fast User Switch) -- 快速用户切换
*   会话隔离(Session Isolation) -- Vista后所有系统服务运行在会话0以增强系统服务的安全性
*   Restricted Token -- 受限访问令牌
*   SACL(System Access Control List) -- 
*   SFP -- 系统文件保护, Win2K 以前的文件保护机制
*   UAC(User Account Control) -- 用户帐户控制， 管理员登录时会为该登录会话创建了两个不同的访问令牌，默认使用受限访问令牌(创建时指定了禁用SID并删除了某些权限)，
*     以减少Windows Vista系统的受攻击面；需要权限提升时，会使用非限制访问令牌所提供的安全上下文来运行应用程序
*     UAC Elevation -- 实际上一个软件在用不同的模块运行，因Virtualization映射到其他位置的Data文件，所以在操作磁盘文件以及注册表的时候，实际上是一个软件在用不同的模块运行。
*       CheckElevationEnabled -- Kernel32.dll 中的未公开函数，typedef DWORD (WINAPI* CheckElevationEnabledProc)(BOOL* pResult);
*   UIPI(User Interface Privilege Isolation) -- 用户界面特权隔离，完整性级别低(lower integrity)的进程，不能向完整性级别高的进程发送Window消息。
*     默认情况下，所有在 WM_USER 以上的消息都会被屏蔽，可以通过 ChangeWindowMessageFilter[EX](WM_xxx, MSGFLT_ADD) 允许从低完整性进程处接受消息
*   WFP -- Windows文件保护, Win2K时引入，只保护文件
*   WRP -- Windows资源保护，对关键文件、文件夹和注册表值进行保护
*
* 权限提升的方式(允许运行在受限用户（LUA）的应用程序激活一个提升权限的COM对象，即在Vista系统上会弹出UAC提示框让用户提升权限（UAC打开状态下）)
*   1.manifest 中的 requireAdministrator ?
*   2.COM Elevation Moniker -- 参见 ftlCom.h 
*
* 安全描述符
*   
* 函数
*   CreateRestrictedToken -- 根据现有的访问令牌的约束创建一个新的受限访问令牌
*   ConvertStringSecurityDescriptorToSecurityDescriptor -- 按安全描述符格式的字符串转换成有效的安全描述符结构
*********************************************************************************************************************************/


namespace FTL
{
	class CFUserUtil
	{
	public:
        //判断当前用户(当前进程的Owner)是否是本地 Adminstrators 组中的成员(注意：不是域的)
        FTLINLINE static IsProcessUserAdministrator();
	};

    //How To Manage User Privileges Programmatically in Windows NT
    //  support.microsoft.com/kb/132958

    //TODO: 未写完
    class CFUserPrivileges
    {
    public:
        FTLINLINE CFUserPrivileges();
        FTLINLINE ~CFUserPrivileges();
        FTLINLINE BOOL Open();
        FTLINLINE VOID Close();
        //设置权限：如 L"SeServiceLogonRight", 其内部可调用 LsaAddAccountRights/LsaRemoveAccountRights
        FTLINLINE BOOL SetPrivilegeOnAccount(LPWSTR PrivilegeName, BOOL bEnable ); 
    private:
        FTLINLINE void _InitLsaString(PLSA_UNICODE_STRING LsaString,LPWSTR String);
    private:
        LSA_HANDLE  m_hPolicy;
    };
}
#endif //FTL_USER_H

#ifndef USE_EXPORT
#  include "ftlUser.hpp"
#endif 