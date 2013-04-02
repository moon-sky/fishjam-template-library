using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

/************************************************************************************************************
 * System.Text.StringBuilder -- 处理密集的字符串操作，使用内部缓存来处理字符串
 * 
 * 页面跟踪(ASP.NET 3.5从入门到精通 P257~258,缺页) -- 可以跟踪页面的详细信息，可用于性能分析？
 *   控件树、会话状态和应用程序状态。
 *   
 * Web农场(Web Farm) -- Web 的分布式扩展?
 *   machine.config -- 需要设置 <machineKey>，使得在所有的Web服务器上保持一致，否则可能会造成多个Web服务器服务同一
 *   用户时，在各个服务器上SessionId不一致，而出现问题？
************************************************************************************************************/

public partial class WebPerformance : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
    }
}
