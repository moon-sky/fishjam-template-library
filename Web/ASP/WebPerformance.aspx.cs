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
 * Web农场(Web Farms) -- 共同负责处理HTTP请求的一组Web服务器，专用于需要处理重负载的高性能网站。即Web 的分布式扩展？
************************************************************************************************************/

public partial class WebPerformance : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
    }
}
