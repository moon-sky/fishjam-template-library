#ifndef FTL_USER_H
#define FTL_USER_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlUser.h requires ftlbase.h to be included first
#endif

#pragma TODO(从 FUUser 文件中同步)
//资料:
//http://blog.csdn.net/chenjinxian88/article/details/8933073
//http://blog.csdn.net/xbgprogrammer/article/details/16818973
//https://msdn.microsoft.com/zh-cn/library/2007.07.securitywatch.aspx
//http://pnig0s1992.blog.51cto.com/393390/908495/
//http://wenku.baidu.com/link?url=tiLVXTK_8BYakF985QIRX9GFNiWKiLtKuRBJne0bDNmw-3379rz8wAAAjFkgvCWw7dN_zlHZMvRJGRszH9D_T5N5cJBwauafSfUKm2F32g3
//http://mqc173.blog.163.com/blog/static/30899093200751110582422/

/*********************************************************************************************************************************
* 最佳实践
* 
* 工具
*   cacls|icacls(推荐) -- 系统自带的命令行ACL管理工具
*     使用方式参见: https://technet.microsoft.com/zh-cn/magazine/2007.07.securitywatch.aspx
*   
*
* TODO:
*   CreateWellKnownSid -- ?
*********************************************************************************************************************************/


//用户帐号控制(UAC) -- http://www.cnblogs.com/rainbowzc/archive/2012/02/28/2371526.html
/*********************************************************************************************************************************
* 安全控制
*   ACE(Access Control Entry) -- 
*   ACL(Access Control List) -- 由0或多个ACE的有序列表组成, 有两种类型的ACL:
*     DACL(Discretionary ACL) -- 任意访问控制列表, 包含了用户和组的列表，以及相应的权限，允许或拒绝。
*        每一个用户或组在任意访问控制列表中都有特殊的权限。如果一个对象没有DACL，那么就是说这个对象是任何人都可以拥有完全的访问权限.
*     SACL(System ACL) -- 系统访问控制列表，为审核服务的，包含了对象被访问的时间
*   Security Context -- 安全上下文，定义某个进程允许做什么的许可和权限的集合，通过登录会话确定，通过访问令牌维护。
*   SD(Security Descriptors) -- 安全描述符,保存权限的设置信息,对应 SECURITY_DESCRIPTOR 结构,主要由 SID + DACL + SACL 等组成.
*   Securable Object -- 安全对象,即拥有SD的Windows的对象(主要是各种Handle)
*   SID(Security Identifier) -- 安全标识符, 标识用户、组和计算机帐户的唯一的号码,内部安全管理通过SID进行(而非用户名或组名),
*     因此删除账户后再创建同名的账户,也无法继承之前的权限。
*     注意: Ghost 的机器会因为使用相同的SID而造成严重的安全问题 -- 新版本的Ghost企业版可在控制台下修改? 微软也提供了 Riprep ?
*     SID分为内置的系统账号 和 用户账号(由计算机名+时间 等数据随机生成)，如：S-1-5-21-310440588-250036847-580389505-500，详细的格式说明:
*     S(表明是SID)-1(版本号)-5(标志符的颁发机构)-21(子颁发机构)-310440588-250036847-580389505-(ID authority和验证值等信息)-500(标志域内的帐户和组?)
*     HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\ProfileList 可以参看SID和用户名(参见 ProfileImagePath )的对应关系? TODO: 是否有函数?
*   Security Principal -- Windows信任的安装安全主体
*   LUID -- locally unique identifier, 本地唯一标识符，在系统的每一次运行期间保证是唯一。因此其值通常没有实际意义。通常需要通过 LookupPrivilegeName 获取对应的权限名等信息
*
*   DEP -- 数据执行保护
*   NTLM -- NT LAN Manager(win2000早期版本中的一种验证身份方式), 基于一种"提问-答复"机制来进行客户端验证
*   MIC(Mandatory Integrity Control) -- 强制完整性控制。Vista中的所有安全性对象和进程都有一个完整性级别，
*     完整性级别(Integrity Level -- IL)低的进程不能修改(通常可以读取?)级别高的文件或注册表表项。
*     注意：Win2K/XP 下，安全子系统只把进程的访问令牌和资源的访问控制列表进行匹配比较，以确认该进程是否具有访问该资源的权限。
*     0.AppContainer -- Win8 引入,是个沙盒机制，低的进程不能读写(注意:已经不能读了)级别高的文件或注册表项。
*             所有Metro-sytle的应用 和启用了EPM的 IE Tab进程 都运行在其中,需要声明权限?
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
    enum IntegrityLevel{
        ilUnknown = 0,
        ilAppContainer, //Win8 + IE10 + EPM 或 Metro 时, 
        ilLowCompat,    //TODO: 网页要求 ActiveX control which is not EPM-compatible 时
        ilUntrusted,
        ilLow, 
        ilMedium,
        ilHigh,
        ilSystem,
        ilProtectedProcess
    };

	class CFUserUtil
	{
	public:
        FTLINLINE static IntegrityLevel GetIntegrityLevel(DWORD dwIntegrityLevel);
        FTLINLINE static LPCTSTR GetIntegrityLevelString(IntegrityLevel iLevel);
        FTLINLINE static LPCTSTR GetWellKnownSidTypeString(WELL_KNOWN_SID_TYPE sidType);
        FTLINLINE static LPCTSTR GetAclInfo(CFStringFormater& formater, PACL pAcl);
        FTLINLINE static LPCTSTR GetPrivilegeNameByLuid(CFStringFormater& formater, PLUID pLuid, LPCTSTR lpSystemName = NULL);
        FTLINLINE static LPCTSTR GetSidInfo(CFStringFormater& formater, PSID pSid, BOOL bGetSubAuthority);
        FTLINLINE static LPCTSTR GetSidAttributesString(CFStringFormater& formater, DWORD dwAttributes, LPCTSTR pszDivide /*= TEXT("|") */);
        FTLINLINE static LPCTSTR GetSidAndAttributesInfo(CFStringFormater& formater, PSID_AND_ATTRIBUTES pSidAndAttributes, BOOL bGetSubAuthority, LPCTSTR pszDivide);
        FTLINLINE static LPCTSTR GetPrivilegeAttributesString(CFStringFormater& formater, DWORD dwAttributes, LPCTSTR pszDivide /*= TEXT("|") */);
        FTLINLINE static LPCTSTR GetSecurityDescriptorinfo(CFStringFormater& formater, SECURITY_DESCRIPTOR* pSecurityDescriptor);

        //判断当前用户(当前进程的Owner)是否是本地 Adminstrators 组中的成员(注意：不是域的)
        FTLINLINE static BOOL IsProcessUserAdministrator();
        FTLINLINE static BOOL IsVistaUACEnabled();

        FTLINLINE static IntegrityLevel GetProcessIntegrityLevel(HANDLE hProcess);

        //TODO: 函数还不正确，需要重写： 创建一个任何人都可以使用的 SECURITY_ATTRIBUTES  -- ATL中已有函数？
        FTLINLINE static BOOL CreateEmptySecurityAttributes(SECURITY_ATTRIBUTES* pSecurityAttr);

        //导出指定Token(访问令牌)的权限信息
        FTLINLINE static BOOL DumpTokenInformation(HANDLE hToken);
    private:
        static LPCTSTR WINAPI GetTokenReservedInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenDwordInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenUserInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenGroupsInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenPrivilegesInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenOwnerInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);

        static LPCTSTR WINAPI GetTokenPrimaryGroupInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenDefaultDaclInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenSourceInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenTypeInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetSecurityImpersonationLevelInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenStatisticsInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenGroupsAndPrivilegesInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenOriginInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenElevationTypeInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenLinkedTokenInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenElevationInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenAccessInformationInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenMandatoryLabelInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenMandatoryPolicyInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetTokenAppcontainerInformationInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
        static LPCTSTR WINAPI GetClaimSecurityAttributesInformationInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
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
        //FTLINLINE void _InitLsaString(PLSA_UNICODE_STRING LsaString,LPWSTR String);
    private:
        //LSA_HANDLE  m_hPolicy;
    };
}
#endif //FTL_USER_H

#ifndef USE_EXPORT
#  include "ftlUser.hpp"
#endif 