using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

/************************************************************************************************************
* 缺省时不允许远程访问 .config 文件，若直接访问 .config 的地址会报错
*   
* 配置文件 采用多层架构的方式 ： 
*   machine.config(整个计算机的配置) + 根web.config + Web目录中的 web.config [ + 子目录中可选的 web.config]
* 
*
 ************************************************************************************************************/

/************************************************************************************************************
* machine.config
*   <machineKey validationKey 和 decryptionKey> -- 设置视图状态进行检测时使用的密钥，当在WebFarms中使用时需要
*     将所有Web服务器上的该字段设为一致，否则可能会造成多个Web服务器服务同一用户时，在各个服务器上SessionId不一致，而出现问题
* 
************************************************************************************************************/

/************************************************************************************************************
* web.config 
* 代码中通过 System.Web.Configuration.WebConfigurationManager 类访问, 如 string result = WebConfigurationManager.AppSettings.Get("FromAddress");
*   web.config -- 配置文件,如出错信息如何显示给用户，如何禁止非法用户的安全设置等
*    <appSetting> -- 自定义的应用程序设置，如 创建一个特殊的调试开关变量辅助调试 
*      <add key="xxxx" value="yyyy"/>
*    <connectionStrings> -- 定义和保存用于访问数据库的连接字符串,然后: WebConfigurationManager.ConnectionStrings["Pubs"].ConnectionString;
*      <add name="Pubs" connectionString="xxx"; Integrated Security=SSPI"/>
*    <system.web> -- 保存各种ASP.NET的配置设置
*      <authentication>
*      <authorization>
*      <compilation debug="true"> -- 设置调试(Debug)模式
*      
* <add type="System.Workflow.Runtime.Hosting.ASPNetThreadingService, System.Workflow.Runtime, Version=xxx"/> -- 更改WWF的工作流线程模型 ？
* 
* GUI的网站管理工具(WAT -- Website Administration Tool) : Web Site -> ASP.NET Configuration
* 
* <customErrors 
*      mode="RemoteOnly -- 对于远程用户进现实通用的出错信息(本地用户显示更详细的代码行等), 默认设置。
*          调试阶段可设置为 "On"，即对所有用户都显示详细错误信息，通常用于调试阶段，发布阶段应该设置为 RemoteOnly 或 Off
*      defaultRedirect="MyErrorHandle.aspx"  -- 设置自定义的错误信息。
*  >
*    <error statusCode="404" redirect="404.aspx"/>  -- 针对特定的HTTP错误定义一个用户定义的出错信息页面。
*  </customErrors>
* 
* <sessionState    -- 会话状态配置
*   cookieless     -- HttpCookieMode 类型的枚举之一，
*                     [UseCookies] -- 使用cookie，如客户端不支持cookie，则每次请求时都会生成新的SessionId(会话信息丢失)
*                     AutoDetect(推荐?为何不是缺省?) -- 自动检测，如客户端支持cookie，则使用cookie，否则使用改写的URL
*   timeout        -- 以分钟为单位的超时
*   mode           -- 有 "InProc" 等
* >
* 
* <trace> -- 启用用户级别的跟踪信息, 然后可通过Web访问虚拟的 trace.axd 文件查看跟踪信息
* 
* <urlMappings> -- URL映射，将一个请求的URL映射到另一个不同的URL上。注意：1.不区分大小写;2.查询字符串参数将被忽略。
 *  <add url="~/mySrc.aspx" mappedUrl="~/myTarget.aspx?category=targetParam" />
 *</urlMappings>
***********************************************************************************************************/

public partial class ConfigStudy : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        
    }
}
