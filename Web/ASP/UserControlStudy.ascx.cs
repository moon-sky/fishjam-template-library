using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
/************************************************************************************************************
 * UserControl -- 用户控件
 * 使用方式：
 *   1.在要包含该用户控件的Web页面指令下，添加一条注册指定，使用一个唯一的控件前缀来标识嵌在页面中的一个用户控件类(系统默认的是 asp )
 *     <%@ Register TagPrefix="fjc" TagName="UserControlStudy" Src="UserControlStudy.ascx" %>
 *   2.在Web页面中插入用户控件的标记代码，从而将用户控件加添加到Web页面之中
 *       <fjc:UserControlStudy id="myControl1" runat="server" />
 *   GUI方式使用：直接把 .ascx 文件拖到Web窗体的设计区域即可。
 *   
 * Page.LoadControl() -- 可以在运行时从一个 .ascx 文件动态地创建一个用户控件，然后加到Web页面上某个容器控件中。
************************************************************************************************************/

public partial class UserControlStudy : System.Web.UI.UserControl
{
    protected void Page_Load(object sender, EventArgs e)
    {

    }
}
