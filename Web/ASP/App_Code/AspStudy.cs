using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
//ASP.NET2.0入门经典 -- P27(缺少第二章，且文档不全)  //http://www.wroxunited.net/, 用户名/密码：Lou/lou@123
//ASP.NET.3.5从入门到精通(Csharp2008版) -- P371(独立用户控件)

//ASP.NET + WWF 开发采用 MVC模式(模型-视图-控制器)，由 工作流模型告诉ASP.NET控制器（自定义的WorkflowController)下一步怎么切换。

//私有程序集 和 共享程序集(存储在 GAC,Global Assembly Cache, 全局程序集缓存中)

//Decimal.TryParse 代替 Decimal.Parse 方法
//Visual Studio 会把一个默认的隐藏 .sln 文件保存在 %USERPROFILE%\My Documents\Visual Studio 2008\Projects\工程名 下
//默认采用无项目文件(.csproj)的方式，但可以采用Web项目文件(.csproj)的方式 -- 通过 "ASP.NET Web Application"向导模版生成，
//  通常只用于 MSBuild 创建自动化部署 时
//在一个元素(如 head,form 中) 加上 runat="server" 属性，且有唯一的ID属性，则可以在代码中操作该元素的信息。
//视图状态(隐藏字段 __VIEWSTATE ) -- 以加密的格式来保存页面中每一个控件的状态信息，从而可以在代码中操作控件的属性，并且在浏览器与Web服务器的每一次请求和响应中，自动保持其属性状态。

//常见的正则表达式
//  E-Mail地址： \S+@\S+\.S+ <== 检查是否存在一个@字符和一个点号字符，并且其他字符仅允许为非空白字符
//  高级密码：   [a-zA-Z]\w*\d+\w*  <== 有很多其他的写法
//  
//  
/******************************************************************************************************************
  * ASP -- Active Server Pages，基于脚本的程序设计语言
 * ASP.NET -- 完全面向对象的程序设计模型
 * 每一个ASP.NET应用程序都在一个独立的"应用程序域(application domain)"中执行，是内存中相互隔离的内存区域，使得当一个Web应用程序出现致命错误时，不会影响到当前在同一台计算机上运行的其他应用程序。
 *   两次编译：实现了软件的一致性和针对硬件优化的双重目标
 *   1.代码编译成 MISL(Microsoft Intermediate Language) 的中间语言
 *   2.部署时,对页面进行更进一步的编译，将其编译成CLR(Common Language Runtime -- 公共语言运行时)，
 *     运行时，通过即时编译器将IL代码转换为与当前计算机平台相匹配的本地机器语言的代码 -- 会进行特别的硬件平台优化。
 * 
 * ASP.NET 2.0 中的开发工具： Visual Web Developer，很早以前且被淘汰的是 Front Page 和 Visual Interdev 等
 * 使用ASP.NET 2.0创建动态的、数据驱动的、复杂的Web站点
 * 
 * HTML规范
 *   过渡模式 --  <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
 *   严格模式 --  <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
 *   略微修正的1.1 -- <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
 *     XHTML1.1与XHTML1.0绝大部分是完全相同的，但是 1.1 精简了少量的细节，并移除了少量的遗留特性，且不提供过渡模式 -- 完全是XHTML的
 *  注意：
 *     如果一个页面在IE浏览器中的外观与在Firefox浏览器中大不相同(文本大小、空白变化)，故障的原因可能是缺少了 DOCTYPE 声明，或使用了无效的 DOCTYPE 声明。
 *     如果Web页面中没有包含 DOCTYPE 声明，则IE浏览器将自动切换到传统的 兼容模式(quirks mode),可能会按一种不规范非标准的方式处理某些格式化的细节。
 *
 * 
 * XHTML -- HTML的一种改进，遵循XML规范，加强了HTML标记的规则和约束，不允许标记的缺失等，
 *          XHTML 不允许文本直接添加在<form>元素内，因此一般需要放置在其他容器(比如<div> 中)
 *          xhtmlConformance -- 配置选项，可以改变 ASP.NET 的工作方式，使之应用HTML标准而不是XHTML标准(但一般不用)
 * 
 * 常见控件(ASP.NET 2.0 书籍中) -- 服务器端控件，运行时会转换成 标准HTML + 脚本 发给客户端，保证客户端可显示
 *   MasterPage + Theme -- 使得一个站点在标题、菜单和链接的布局上保持一致
 *   SiteMap/Navigation -- 导航控件，可以根据站点地图和当前页面的名称显示菜单和其他导航信息
 *   
 *   Login/CreateUserWizard 等 -- 登录、安全和角色管理等，可以通过电子邮件给出口令提示
 *   数据源(Data Source) + 数据绑定(Data bound) -- 连接到数据源 和 获取信息并显示
 *     底层的 ADO.NET(ActiveX Data Objects) 和数据源(MSSQL, MySQL, XML 等)交互
 *   
 * Web服务(Web Services) -- 允许创建一种可供远程调用的代码，可为Web引用程序提供类似普通桌面应用程序的特性。
 *   SOAP(Simple Object Access Protocol) -- 简单对象访问协议,从Web服务请求数据的方式
 *   将被WCF(Windows Communication Foundation -- Windows通信基础)所取代
 *   
 * 性能和缓存
 *   缓存 、 碎片缓存、 数据无效缓存(data invalidation caching)
 *
 * Ajax -- 许多优秀的ASP.NET控件能职能地检测客户端浏览器的特性，如支持JavaScript，这这些控件会生成包含了JavaScript代码的页面，
 *   以向客户端浏览器返回更具交互性的页面，如支持拖放操作和自动补全功能等
 *   
 * LINQ -- 可以用与查询数据库类似的方式来搜索内存中的数据。
 *
 * 自定义控件
 *   ASP.NET引擎将为每一个Web控件调用一个名为Render() 的方法
 * 
 * 文件夹
 *   App_Data -- 存放应用程序将用到的数据文件。
 *  
 *   
 * 部署：一个简单的XCOPY命令将把站点从开发机部署到部署主机上。所有的注册操作和机器层的定制都将在第一个请求到达站点时自动发生
 *   IIS(Internet Information Server) -- Internet 信息服务
 *   在Windows中，ASP.NET 2.0和Internet Information Server相互配合从硬盘上读取页面并将其发送给请求者。。IIS的特定用户是ASPNET？
 *   开发时，可以使用轻量级的Web服务器Cassini测试
 *   虚拟目录
 *   
 * 调试
 *   1.web.config 中设置 <compilation debug="true">， 则会把页面相关的错误报告发送到浏览器(只在开发时使用)
 *   2.默认情况下，IE浏览器中的脚本调试器是禁用的 -- IE的Tools -> Internet Options -> Advanced -> Browsing -> Disable Script Debugging(禁用脚本调试)
 *   
 * Master Page -- 允许选择一个母版页(用于标准化页面布局的独立文件)文件作为创建Web页面的模版
 * 
 * 错误处理 -- VS中 Debug -> Exception -> Common Language Runtime Exceptions
 *   结构化异常处理( try...catch...finally ) -- 基于对象的异常，可以根据异常的类型来捕获异常，
 *   C#中的异常对象是 new 出来后 throw 的，利用GC进行垃圾回收。
 *   Exception 类
 *     InnerException -- 嵌套的异常
 *     StackTrace -- 调用堆栈
 *     TargetSite -- 反射对象，提供了引发当前异常的方法的相关信息，如方法名、参数类型，但不包含实参的值
 *     GetBaseException -- 可以通过异常的 InnerException 链，从嵌套异常中获取源异常
 *   ApplicationException -- 用户定义的异常基类，为了完善用户异常类，需要三个标准的构造函数，实现时直接 base(xxx) 即可
 *     默认构造，不带任何参数
 *     带有一个用户自定义的出错信息的构造函数
 *     带有一个用户自定义的出错星系和底层异常对象作为参数的构造函数
 *     
 * 日志系统 (VS 中 View -> Server Explorer 可以在 Event Logs 中查看
 *    System.Diagnostics.EventLog(Windows的事件日志) -- 
 *      注意：1.事件日志达到设定的最大容量时，旧的事件日志会自动丢弃
 *            2.运行ASP.NET的账户通常都不具有创建日志记录的权限(HKLM\SYSTEM\CurrentControlSet\services\eventlog\Application\XXX 设置权限)
 *            
******************************************************************************************************************/


/******************************************************************************************************************
 * .NET 3.0 中引入
 *   WPF(Windows Presentation Foundation) -- 创建精美Windows应用程序的新平台 
 *   WF(Windows Workflow Foundation) -- 使用流程图方式来建模应用程序逻辑的新平台。
 *   WCF(Windows Communication Foundation, Windows通信系统基础) -- 一种用来设计访问服务以供其他计算机调用的平台。
 *   
 * CLS(Common Language Specification) -- 通用语言规范，用于规范化.NET语言的兼容性(即遵循该约定的组件能被其他.NET语言使用)
 * CTS(Common Type System) -- 通用类型系统，定义了所有.NET语言通用的数据类型的规范，如字符串类型、数值类型和数组类型。
 * 
 *   
******************************************************************************************************************/

/******************************************************************************************************************
 * C# 中基于 XML 的注释语法(三个斜线 /// )，然后可用工具生成相关资料
 * 注意：
 *   C# 中 long 是 Int64， C++ 中是 Int32， C#中 char 是 16 比特位的Unicode编码的字符， byte 才是 8位
 *   decimal/Decimal -- 128位的固定小数点的数值，可以支持最大28位的有效数字
 *   数字的标识符： M(decimal), D(double), F(float), L(long), 如 decimal myDecimal = 14.5M;
 *   数组：
 *      string[] stringArray = new string[4];
 *      int [,] intArray = new int[2,4]; //创建一个包含8个(4x2)整型元素的数组(2行 4列)
 *      int [,] intArray = { {1, 2}, {3, 4}, {5, 6}, {7, 8} }; 
 *      int element = intArray[0, 1];
 *      GetLowerBound/GetUpperBound(n) -- 返回某一维度的 最小/最大索引值; Sort,Reverse
 *   集合类：ArrayList, SortedList, Map
 *   向下转型： checked{ myIntValue = (int)myLongValue; }  //使用 checked 包含转型代码，当出现溢出时会抛出异常 -- 效率很低
 *     最好的方式是“转型以前先检查”？
 *   Convert -- 有很多静态转换方法，来支持常见数据类型的转换。
 *   DateTime -- Now/UtcNow, Today(当前日期，且时间是 00:00:00), AddDays, IsLeapYear, ToString(可以指定格式字符串)
 *   TimeSpan -- date1.Subtract(date2) 等价于 date1-date2; TotalMinutes, FromHours
 *   循环：for, foreach, while, 
 *   调试：System.Diagnostics.Debug.Write(xxx);
 *   
 * ReadOnlyCollection
******************************************************************************************************************/

public class AspStudy
{
	public AspStudy()
	{
		//
		// TODO: Add constructor logic here
		//
        
	}
}
