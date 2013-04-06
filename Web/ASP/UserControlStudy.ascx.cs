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
 * 创建用户指定的事件 -- 
 *   0.创建参数和事件签名的委托，如果系统已有的类型可以满足要求，则可以省略(需要放在 App_Code 目录下，不能和控件在同一个文件中 ?)
 *     public class LinkClickedEventArgs : EventArgs { ... }
 *     public delegate void MyLinkClickedEventHandler(object sender, LinkClickedEventArgs e);
 *   1.声明事件的签名 -- 标准的事件声明要求两个参数， 触发事件的控件的引用 和 与事件相关的 EventArgs 子类
 *     public event MyLinkClickedEventHandler MyLinkClicked; 
 *   2.触发事件
 *     if(MyLinkClicked != null) { MyLinkClicked(this, myLinkClickedEventArgs); }
 *     
 * Page.LoadControl() -- 可以在运行时从一个 .ascx 文件动态地创建一个用户控件，然后加到Web页面上某个容器控件中。
************************************************************************************************************/

public partial class UserControlStudy : System.Web.UI.UserControl
{
    protected void Page_Load(object sender, EventArgs e)
    {

    }
}
