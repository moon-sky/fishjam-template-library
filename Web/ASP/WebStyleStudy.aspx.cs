using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

/************************************************************************************************************
 * MasterPage + Theme -- 使得一个站点在标题、菜单和链接的布局上保持一致
 *   Theme -- 将一组预定义的格式化属性定义为一个主题，然后再将主题应用到各个页面，以确保Web应用程序中的各个控件具有协调一致的外观样式。
 *     1.创建一个定义主题的文件夹(App_Themes),里面每一个主题放在一个独立的文件夹中，并创建实现主题后台定义的 .skin 皮肤文件。
 *       .skin 本质是进行了简单转换后的控件标记列表，在其中写入控件的各种外观属性，可以通过 SkinID 属性创建命名皮肤然后在控件中指定。
 *     2.[可选]可以设置控件的 EnableTheming 属性为false，使得主题跳过该控件。
 *     3.将主题应用于某个Web页面或整个网站中
 *       Web页面 -- Page 指令中将 Theme(推荐) 或 StyleSheetTheme 属性设置为指定主题的文件夹名称；或修改Document对象的对应属性
 *       整个网站 -- <system.web> 的 <pages> 元素 中设置 Theme 或 StyleSheetTheme
  *    4.运行时，当设置Theme属性时，皮肤中的属性有优先级； StyleSheetTheme属性时，控件中的属性有优先级。
 *   MaterPage(.master文件) -- 定义标准化的网页模板，可以使用该模版来规范网站中各个页面的布局(如 标题、页脚、菜单栏等)，然后可以只关注内容页的创建。
 *     类似普通Web页面，其中包含各种Web控件和逻辑代码，扩展名为 .master，设置统一布局的内容，内容页将放置在 ContentPlaceHolder 中，
 *       如果在 ContentPlaceHolder 中放置控件，将会作为缺省内容，如果子页面中有内容，将会替换。否则(如果没有Content项)会显示Master中定义的这些缺省内容。
 *     创建新的WebPage时，可以选择该MaterPage，并可在Page指令中设置各个页面对应的 Title 属性，并在各个 <asp:Content> 中放入特定的控件
 *     高级用法：通过在母版页中加入 <runat="server"> 的 HTML 表格页面，可以确保内容页面按照需要的方式进行对其
 *     
 * Add New Item -> StyleSheet, 可以创建CSS文件。
 *   Format -> New Style, 可以创建 CSS。
 *   打开Web页面时，View -> CSS Properties 编辑CSS属性。
 * 
 * 控件中可以设置 CssClass、等属性
************************************************************************************************************/

public partial class WebStyleStudy : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {

    }
}
