using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

/************************************************************************************************************
 * 表单安全性(form security) -- 保护网站的页面，强制用户通过一个登陆页面后才能访问（20章)
 * 
 * SQL注入攻击(SQL injection attack) -- 使用参数化的数据库命令来彻底避免发生SQL注入攻击的危险。
 *   SQL Server 包含了一个特殊的系统存储过程(具体是什么?),利用该存储过程可以执行服务器上的任何程序。
 *   语法(注意：不同的数据提供程序，参数化数据命令的语法不同): 
 *     SQLServer要求以 @ 开始命名，如 WHERE CustomeID = @CustomerID, 然后 cmd.Parameters.AddWithValue("@CustomerID", txtID.Text);
 *     OLEDB要求以 ? 来表示，然后通过位置替换，如 WHERE CustomeID = ? 
 * 
 * 文件上传时的安全问题
 *   1. 如果上传文件大于允许上传的上限，ASP.NET的服务器在接收到完整的请求后，才会拒绝上传，这会造成服务器的停顿(拒绝服务攻击)，
 *      允许上传的文件越大，越容易受到这种攻击
 **************************************************************************************************************/

public partial class WebSecurity : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        if (!this.IsPostBack)
        {
            Session["userInfo"] = "SomeUserInfo";
        }
        lblSession.Text = "SessionId:" + Session.SessionID + "<br />";
        lblSession.Text += "Number of Object:" + Session.Count.ToString() + "<br />";
        lblSession.Text += "IsCookieless:" + Session.IsCookieless.ToString() + "<br />";
        lblSession.Text += "Mode:" + Session.Mode.ToString() + "<br />";
        lblSession.Text += "IsNewSession:" + Session.IsNewSession.ToString() + "<br />";
        lblSession.Text += "Timeout(minutes):" + Session.Timeout.ToString() + "<br />";

    }
}
