using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
/************************************************************************************************************
 * HTML服务器端控件是Web控件与传统的HTML元素在程序设计上的一种折中，
 *
 * 在页面上添加HTML服务器端控件( HtmlControl及其子类， 如 HtmlInputButton 等) 
 *   1.在 .aspx 中手工添加 HTML 元素标记，然后为其添加 runat="server" 属性；
 *   2.从工具箱中的 HTML 选项卡中直接拖放 -- 似乎也要手工添加 runat="server" 属性 ?
 *   通常提供了两种服务器端事件：ServerClick 或 ServerChange事件，注意：无法通过IDE自动生成事件，
 *   属性：
 *     EnableViewState -- 如设置为 false 时，将禁用控件的自动状态管理功能。
 *     Style -- 用以访问控件的CSS样式属性, 如 Result.Style["color"] = "Red"; 
 *     TagName -- 指出控件所呈现的底层HTML元素的标记名称，如 img 或 div 等；
 *     Visible -- 如设置为 false，将关闭控件的呈现功能，在最终生成的HTML代码中将不包含该控件的呈现代码。
 *     数据绑定 -- ?
 *   HtmlContainer
 *     InnerHtml -- 表示位于控件的开始标记和关闭标记之间的HTML内容，且其中包含的特殊字符将不会被转换为对应的HTML实体字符。
 *     InnerText -- 表示位于控件的开始标记和关闭标记之间的纯文本内容，特殊字符将会被自动转换为对应的HTML实体字符，并在页面上显示为文本字符。
 *   HtmlInputControl -- 
 *     type -- 表示 <input>类型控件的类型，如 file, text 等;
 *
************************************************************************************************************/

public partial class HtmlControlStudy : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {

    }
}
