using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

/************************************************************************************************************
 * 状态存储和管理方式：视图状态、查询字符串、自定义Cookie、Session、Application、Profile 等
 * 
 * 会话(Page.Session) -- 可以将特定于用户的会话信息临时保存在服务器的内存中，通常用于保存敏感信息，和在不同页面之间保持信息。
 *   ASP.NET使用120比特的 SessionID 标识符来唯一标识一个会话，客户端每次请求时都要包含该ID：
 *     1.[缺省方式]使用自动创建的名为 ASP.NET_SessionId 的Cookie,一般不用直接访问。
 *     2.使用修改的URL -- 可用于客户端不支持cookie的情况。
 *   限制：因为会占用服务器内存，若任意使用，会使得Web应用程序无法扩展以支持大量客户端访问 -- 更好的选择是缓存？
 *   注意：为了安全，需要避免重用过期会话的SessionID，但会迫使当前页面中所有视图状态和表单数据丢失? -- 设置 regenerateExpiredSessionId = true;
 *   
 *   HttpSessionState -- Session类
 *     IsCookieless -- 标识SessionID是使用cookie进行跟踪还是使用改写的URL进行跟踪。
 *     IsNewSession -- 表示会话是否是与当前请求一起创建的。
 *     Mode -- 表示如何存储会话状态信息，由web.config配置文件中的设置决定，如:
 *       InProc(缺省，最好的执行性能和最少的持久能力，不支持Web Farsm),
 *       StateServer(名为 "ASP.NET State Service"的服务进程进行状态管理,当Web进程被重启时，对状态不会造成影响，通过 stateConnectionString 字符串指定的Socket通信),
 *       SQLServer(使用 sqlConnectionString 指定的数据库保存会话信息，需要通过 aspnet_regsql.exe -ssadd 安装需要的存储过程和会话信息数据库)
 *       Custom(存储会话状态信息时将使用由 customProvider 属性设置的会话状态存储提供程序，如Oracle公司提供的保存在Oracle数据库中)
 *     Timeout -- 持续时间
 *     Abandon -- 立即取消当前会话，并释放当前会话所占用的内存，常用语退出页面。
 *     
 * 应用程序状态(HttpApplicationState) -- 存储Web应用程序的全局对象，可被任何客户端访问，如保存 数据库连接字符串 等，
 *   通过 Global.asax 管理?
 *   
 * 用户配置(Profile, 22章) -- 允许程序员从数据库中取回或存储用户特定的信息，在取回信息前，需要先对用户进行认证
 * 
 * 缓存(Cache, 24章) -- 在内存中保存信息和数据库中保存信息的一个折中。
 *   使用会话信息来创建一个临时的数据库记录，同时把唯一记录ID保存在会话状态中，
 **************************************************************************************************************/

public partial class StateManager : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {

    }
}
