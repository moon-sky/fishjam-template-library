using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

/************************************************************************************************************
 * 度量单位Unit -- 结构类型，将一个数值设置为某种类型的长度度量单位，如 像素(px),百分比(%)等，代码中可使用其静态方法转换。
 * 颜色：Color.FromArgb(255, 0, 0, 255); Color.xxx; ColorTranslator.FromHtml("Blue"); #RGB
 * 字体：FontInfo -- 
 *   可以对Font.Names 属性按优先级顺序指定一组字体列表，客户端计算机上依次尝试是否有该字体并使用。所有的浏览器都支持 Times,Arial,Courier
 *   
 * Web控件 -- 相对于HTML服务器端控件，提供了更加复杂和丰富的对象模型，可以根据不同浏览器自动调整输出(如是否支持JavaScript)
 * 
 * 布局方式
 *   GridLayout(网格布局) -- VS2008中网格布局(GridLayout)已经不再提供 -- 如想强制使用，则源码中通过 style="POSITION: absolute; left:100px; top:50px" 的方式使用
 *   FlowLayout(流布局)
 *
************************************************************************************************************/

/************************************************************************************************************
 * 
 * 常见控件
 *   WebControl -- Web控件的基类
 *     AutoPostBack(自动回传)属性 -- 当控件检测到特定的用户行为时(如CheckBox切换),立即给服务器回传页面，立即触发事件。
 *       问题：可能会造成频繁的网络交互，最好还是通过 Ajax 实现。
 *     AccessKey -- 快捷键，如 Y 表示 Atl+Y;
 *     Controls -- 提供了一个当前控件所包含的所有子控件的集合，
 *     TabIndex -- Tab键索引，页面加载时，TabIndex为0的Web控件将获得输入焦点。
 *     数据绑定 -- 
 *   
 *   列表控件(如 ListBox,DropDownList, CheckBoxList 等)
 *     SelectedIndex -- 0基址的被选项索引，
 *     SelectedItem? -- 选中项(ListItem)的集合，可用于多选时的判断(检查 Selected 属性), 还是需要遍历 Items ?
 *   BulletedList --  对应 <ul>(无序列表) 或 <or>(有序列表), 通过 BulletStyle 区分
 *   Table(TableRow + TableCell) 
 *   
 *   Label
 *     AssociatedControlId -- 快捷键时的关联控件。
 * 
 * Page
 *   IsPostBack -- 为false表示是第一次请求当前页面，为true表示该页面是响应应用控件事件而提交的回传请求，此时页面通常在试图状态中保存了页面的相关信息。一般用在 Page.Load 中
 *   EnableViewState -- 是否保持其视图状态，为false时页面上所包含的所有控件都不能维护状态信息。
 *   Application -- 保存应用程序级别信息的集合，保存当前Web网站中被所有用户共享的信息。
 *   Session -- 保存单个用户会话信息的集合，可在不同的多个页面之间共享，如 购物车信息。
 *   Cache -- 保存在创建时比较耗时的对象的集合，以便在其他用户访问页面时快速地重用这些对象，适当地使用该技术可以有效提高Web页面的执行性能。
 *   Request -- 对当前页面的HttpRequest对象的引用，包含了当前Web请求的相关信息，可获取Cookies, 浏览器信息等，使用查询字符串将在页面间传递信息等。
 *   Response -- 对当前页面的HttpResponse对象的引用，表示将发回给客户端浏览器的响应(如 Cookies 对象等)，也可以对用户重定向(Redirect)。
 *   Server -- 对当前页面的HttpServerUtility 对象的引用，如 字符串编码, Transfer 等
 *     Transfer -- 不向客户端浏览器回传的重定向，但该方法只允许同一Web中从一个ASP.NET页面跳转到另一个ASP.NET页面，客户端不知道实际已经跳转到另一个页面(可用于地址隐藏?)
 *     HtmlEncode -- 如 ctrl.innerHtml = Server.HtmlEncode("Enter a word <here>"); -- 常用于从数据库中获取到文本或字符字段的值时。
 *     HtmlDecode -- 将已编码的字符串转换为原来的形式。
 *     UrlEncode/UrlDecode -- 将字符串转换为可用于URL的有效编码形式，常用于对附加在URL之后的字符串(如中文)进行编码 
 *   Trace -- 写跟踪信息
 *   User -- 对用户进行认证(authenticate)的信息 -- 20章
 *   
 * Page的事件流程?：Init -> Load -> PreRender -> Unload
 * 
 * Cookie -- 通过Request读取，通过Response设置，类型是 HttpCookie，使用时注意判断是否为null(防止用户禁用cookie)
 * 
/************************************************************************************************************/

public partial class WebControlsStudy : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        
        
    }
}
