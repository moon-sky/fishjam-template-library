using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.Profile;

/************************************************************************************************************
 * 注意：
 *   1.为Web应用程序提供安全保障的大部分工作，不是编写代码，而是在适当的时机实施安全策略。
 *   2.ASP.NET代码并不是运行在IUSER账户之下，因为其并不具有足够的权限来运行代码，包括为了关联编译过程，允许创建和删除临时文件的权限。
 *     运行ASP.NET 代码的账户是在 machine.config 文件或 应用程序池标识(application pool identity)来设置的。
 * 
 * 常见安全措施
 *   1.表单安全性(form security) -- 保护网站的页面，强制用户通过一个登陆页面后才能访问
 *   2.强密码保护后台数据库，并在存储信息前对敏感信息进行加密
 *   3.使用参数化的数据命令来防止SQL注入攻击
 *   4.对于Forms验证，可以使用安全票据(Ticket)
 * 
 * ASP.NET的安全机制
 *   认证：确定用户身份并强制用户证明自己就是所宣称身份的过程。通常是用户登录(用户名和密码)。
 *   授权：用户经过认证之后，授权将决定该用户是否具有执行某一特定操作的足够权限(如查看某一页面)
 *   一个Web应用程序中所有目录必须使用相同的认证系统，但每一个目录可以有自己的授权规则
 * 
 * 两种用户认证方式：
 *   表单认证 -- 适合于公共网站，使用用户数据库进行认证。
 *     1.在 Web.config 文件中(或使用WAT)将认证模式设置为表单认证, 
 *       <authentication mode="Forms"><forms loginUrl="~/Login.aspx" timeout="10" xxxx/></authentication>
 *     2.限制匿名用户仅能访问Web应用程序中的特定页面或目录(通常需要将要求认真后才能访问的文件放在一个单独的文件夹中，并配置单独的web.config)
 *       <authorization><deny users="?" /><allow users="*" /></authorization>  -- 禁止匿名用户访问，并允许所有其他的用户访问
 *       可通过 <location> 控制对特定文件的访问权限 -- 但推荐使用目录的方式
 *     3.创建一个登录页面，验证用户信息，如果通过则可以继续访问
 *       if( MemberShip.ValidateUser(name,pwd)) {  FormsAuthentication.RedirectFromLoginPage(Login1.UserName, Login1.RememberMeSet); } 
 *       FormsAuthentication 类
 *         HashPasswordForStoringInConfigFile -- 使用指定算法来加密字符串，然后可保存在DB中
 *         SignOut -- 注销当前用户
 *       Page.User 对象 -- 可以获取用户的各种信息,
 *     4.注销命令
 *   Windows认证 -- IIS强制用户作为一个Windows用户来登录，适合于公司内部网络的内联网应用程序
 * 
 * ASP.NET 为表单认证框架添加的"成员关系(membership)"功能层，需要SQL Server的支持(aspnet_regsql.exe 创建 aspnetdb)
 *   管理用户信息 -- 在DB中自动创建和维护用户的信息
 *   安全控件 -- Toolbox中的Login选项卡，提供了大量利用 Membership/MembershipUser 等类的控件
 *   基于角色(User.IsInRow)的安全性 -- 定义了相应权限的用户组，通过相应的工具，可以自动创建具有角色信息的数据库
 *   TODO：代码中可以通过 MemberShip 等类访问相关的功能，是否能访问非 SQLServer 的库?
 * 
 * 身份模拟(必须采用Windows认证?) -- 允许在某一个特定用户标识之下执行某些代码
 *   配置方式
 *   编码方式
 *     WindowsIdentity id = User.Identity as WindowsIdentity;
 *     WindowsImpersonationContext impersonateContext = id.Impersonate();
 *     //可在模拟的身份Id之下执行任务(如 读取某个文件等)
 *     impersonateContext.Undo();  //恢复原来的身份ID
 * 
 * 用户配置文件(Profile) -- 在DB中自动保持特定于用户的信息，优点是简单透明
 *   缺点
 *     1.必须使用预定义的数据库结构，用户的配置文件被存储为一条单独的记录，并通过用户名来唯一地标识。
 *     2.频繁读写配置信息或数据量比较大时，会出现性能问题
 *   使用
 *     需要在web.config 中通过 <profile><properties><add xxx> 定义用户配置文件中可以存储的内容
 *     
 * SQL注入攻击(SQL injection attack) -- 使用参数化的数据库命令来彻底避免发生SQL注入攻击的危险。
 *   SQL Server 包含了一个特殊的系统存储过程(具体是什么?),利用该存储过程可以执行服务器上的任何程序。
 *   语法(注意：不同的数据提供程序，参数化数据命令的语法不同): 
 *     SQLServer要求以 @ 开始命名，如 WHERE CustomeID = @CustomerID, 然后 cmd.Parameters.AddWithValue("@CustomerID", txtID.Text);
 *     OLEDB要求以 ? 来表示，然后通过位置替换，如 WHERE CustomeID = ? 
 * 
 * Hash Collision DoS(Hash碰撞的拒绝式服务攻击) -- 受攻击的服务器会变慢，几乎所有的语言都会受影响。
 *   利用各语言中Hash算法的“非随机性”制造出N多不一样的value，但是key一样数据，然后让Hash表成为一张单向链表，
 *   从而导致整个网站或是程序的运行性能以级数下降。
 *   ASP.NET 中可以通过 MS11-100 补丁修复
 * 
 * 
 * 文件上传时的安全问题
 *   1. 如果上传文件大于允许上传的上限，ASP.NET的服务器在接收到完整的请求后，才会拒绝上传，这会造成服务器的停顿(拒绝服务攻击)，
 *      允许上传的文件越大，越容易受到这种攻击
 **************************************************************************************************************/


/**************************************************************************************************************
 * SSL 建立安全会话(该安全会话会使用一个随机生成的密钥)的步骤
 *   1.客户端请求一个SSL连接;
 *   2.Web服务器对数字证书进行签名(sign),并发送给客户端
 *   3.客户端对该数字证书进行核实验证(是否有其所信任的证书授权中心发布，是否与服务器匹配，是否过期或被撤销等)
 *   4.客户端通知所支持的密钥长度
 *   5.服务器选择一个客户端和服务器都能支持的最强密钥长度，然后将密钥长度通知客户端
 *   6.客户端生成一个会话密钥(随机的字节数组),并使用服务器的公钥来加密该会话密钥，并传送给服务器
 *   7.服务器将使用其私钥机密该会话密钥
 *   这样，客户端和服务器都具有了相同的随机会话密钥，在随后的会话期间，客户端和服务器可使用该会话密钥来加密通信的数据。
 **************************************************************************************************************/

/**************************************************************************************************************
 * 同源策略(Same Origin Policy) --如果两个页面的协议、域名和端口完全相同，则是同源的。
 *   同源策略是为了防止从一个地址加载的文档或脚本读写另一个地址加载的文档的属性。
 * 
 * 跨域请求(cross-site) , 通常在 Ajax 中才会涉及到，主要是安全限制
 *   一个站点中的资源去访问另外一个不同域名站点上的资源，默认情况下，脚本访问文档属性等数据采用的是同源策略
 *   
 * 跨域请求的标准 -- Cross-Origin Resource Sharing(跨域资源共享), http://www.w3.org/TR/access-control/
 *   通过添加 HTTP 头的方法来判断哪些资源允许 Web 浏览器访问该域名下的信息。
 *   服务器aspx的代码中 : Response.AddHeader("Access-Control-Allow-Origin", "* 或指定允许访问的域名"); 如果设为*，可能有安全问题 
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


        if (User.Identity.IsAuthenticated)
        {
            Profile.UserName = "Profile_Fishjam";
            Profile.Save();

            GridView1.DataSource = ProfileManager.GetAllProfiles(ProfileAuthenticationOption.All);
            GridView1.DataBind();
        }
    }
}
