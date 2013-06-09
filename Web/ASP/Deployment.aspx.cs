using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
/************************************************************************************************************
 * 注意：
 *   1.正式部署前应确保用于部署Web应用程序中的debug模式已经被禁用。
 *   2.VS中测试时使用的是当前账户，通常拥有比正式运行时更高的权限，需要小心权限问题，不不能读取注册表，向特定目录写权限。
 * 
 * IIS(Internet Information Services,因特网信息服务) 
 *   Web服务器中中注册了 .aspx 文件将由 aspnet_isapi.dll 组件(一个ISAPI扩展)处理。
 *   当把Web应用部署到Web服务器上时，该Web应用程序对外暴露为一个虚拟目录的形式，网络访问时，服务器将在对应的物理目录中查找文件。
 *   
 * 1.安装IIS
 *   版本 -- 桌面版OS有连接数量限制(如 XP只支持10个并发请求)
 *   XP Pro -- IIS 5.1
 *   Server2003 -- IIS6
 *   Vista/Win7 -- IIS7，安装：程序和功能->打开和关闭Windows功能->Internet信息服务
 *   Server2008 -- IIS7?，安装：Administrative Tools -> Server Manager -> Add Roles -> Web Server role
 * 2.安装 .NET 3.5 Rumetime 
 * 3.注册ASP.NET文件映射(当先安Runtime，后安IIS时): aspnet_regiis.exe -i [可选的 -s W3SVC/1/ROOT/ASPStudy ]
 *   (检查所有的虚拟目录，并向IIS注册ASP.NET的文件类型)
 *   注意：有多个同名文件，64位Win7上的目录是 C:\Windows\Microsoft.NET\Framework64\v4.0.30319\aspnet_regiis.exe
 * 4.管理工具：
 *     Win7: 管理工具 -> Internet 信息服务(IIS)管理器
 *       网站(右键) -> 添加网站 -> 指定网站名称和物理路径 -> 
************************************************************************************************************/

public partial class Deployment : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {

    }
}
