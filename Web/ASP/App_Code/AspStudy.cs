using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
//ASP.NET2.0入门经典 -- P27(缺少第二章，且文档不全)  //http://www.wroxunited.net/, 用户名/密码：Lou/lou@123
//ASP.NET.3.5从入门到精通(Csharp2008版) -- P56(for循环)

/******************************************************************************************************************
  * ASP -- Active Server Pages，基于脚本的程序设计语言
 * ASP.NET -- 完全面向对象的程序设计模型
 *   两次编译：实现了软件的一致性和针对硬件优化的双重目标
 *   1.代码编译成 MISL(Microsoft Intermediate Language) 的中间语言
 *   2.部署时,对页面进行更进一步的编译，将其编译成CLR(Common Language Runtime -- 公共语言运行时)，
 *     运行时，通过即时编译器将IL代码转换为与当前计算机平台相匹配的本地机器语言的代码 -- 会进行特别的硬件平台优化。
 * 
 * ASP.NET 2.0 中的开发工具： Visual Web Developer，很早以前且被淘汰的是 Front Page 和 Visual Interdev 等
 * 使用ASP.NET 2.0创建动态的、数据驱动的、复杂的Web站点
 * 
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
 *    
 * 部署：一个简单的XCOPY命令将把站点从开发机部署到部署主机上。所有的注册操作和机器层的定制都将在第一个请求到达站点时自动发生
 *   IIS(Internet Information Server) -- Internet 信息服务
 *   在Windows中，ASP.NET 2.0和Internet Information Server相互配合从硬盘上读取页面并将其发送给请求者。。IIS的特定用户是ASPNET？
 *   开发时，可以使用轻量级的Web服务器Cassini测试
 *   
 * 调试
 *   1.web.config 中设置 <compilation debug="true">， 则会把页面相关的错误报告发送到浏览器(只在开发时使用)
******************************************************************************************************************/


/******************************************************************************************************************
 * .NET 3.0 中引入
 *   WPF(Windows Presentation Foundation) -- 创建精美Windows应用程序的新平台 
 *   WF(Windows Workflow Foundation) -- 使用流程图方式来建模应用程序逻辑的新平台。
 *   WCF(Windows Communication Foundation) -- 一种用来设计访问服务以供其他计算机调用的平台。
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
