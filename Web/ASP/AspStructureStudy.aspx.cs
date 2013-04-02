using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

/************************************************************************************************************
 * .asps -- Web页面文件
 * .ascx -- 用户控件
 * .asmx -- Web Services文件，
 * 
 * Global.asax(向导中的 Global Application Class) -- 全局应用程序文件，可定义全局变量、全局事件等，对应的类是 HttpApplication。
 * 处理器事件，可以编写日志代码。会话状态、认证等，都使用应用程序事件以插入ASP.NET的处理管线(pipeline)中。
 *   Application_Start -- Web应用程序启动时触发一次，通常用于创建或缓存一些初始化信息。
 *   Application_End -- Web应用程序停止时触发，通常清理资源
 *   Application_BeginRequest/Application_EndRequest -- 每当Web应用程序接收到一个请求时都会触发，分别在页面代码 前/后 触发
 *   Application_Error
 *   Session_Start -- 新用户请求且一个会话开始时,
 *   Session_End -- 会话超时或使用程序代码来结束了一个会话时，注意：只在使用进程内(InProc)Session模式时才触发，在 StateServer/SQLServer 模式时不触发
 *
************************************************************************************************************/

public partial class AspStructureStudy : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {

    }
}
