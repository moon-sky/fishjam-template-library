using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

/************************************************************************************************************
 * 表单安全性(form security) -- 保护网站的页面，强制用户通过一个登陆页面后才能访问（20章)
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
