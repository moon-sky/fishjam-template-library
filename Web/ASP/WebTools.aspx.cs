using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

/************************************************************************************************************
 * aspnet_compiler.exe -- 对Web应用程序进行编译
 * aspnet_regsql.exe -- 执行一些与数据库相关的任务，TODO：可以安装SQLServer，然后查看生成的具体信息，然后移植到其他数据库上？
 *   不带参数运行时会出现向导程序
 *   -S (local) -- 指定SQL Server实例的位置
 *   -E -- 通过Windows认证来连接到数据库服务器，并使用当前已登录的Windows账户
 *   -A xxx -- 指定想要安装哪些数据表，如 all(全部), m(成员关系), r(基于角色的安全), p(用户配置), w(数据库缓存支持) 等
 *   -d DBName -- 指定用户数据库的名称，默认名字为 aspnetdb
 *   -sqlexportonly -- 向数据库中添加或移除指定功能创建相应的SQL脚本，但并不实际创建相应的表。常用于部署Web应用程序时
 * aspnet_regiis.exe -- IIS
 *   -lv -- 列出当前计算机上已经安装的所有ASP.NET版本和位置
 *   
 * aspnet_isapi.dll -- 
************************************************************************************************************/

public partial class WebTools : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {

    }
}
