using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Collections;

/************************************************************************************************************
 * 注意：《ASP.NET.3.5从入门到精通(Csharp2008版)》 中 16~17 章介绍，但未仔细看各控件的细节
 * 
 * ADO.NET -- 数据访问组件，用于访问数据库。
 *   Web访问数据库时必须额外考虑：1.可伸缩性; 2. 状态管理
 *   SQL Server 使用 TDS(tabular data stream,表格格式数据流)协议进行通信
 *   
 * 数据提供程序(Data Provider),.NET自带四种数据提供程序
 *   SQL Server Provider
 *   OLE DB Provider
 *   Oracle Provider
 *   ODBC Provider
 *   
 * 连接管理类(每种数据提供程序使用特有的前缀来命名相应的类，如 SqlConnection, SqlDataAdapter 等)
 *   Connection -- 通过连接字符串指定连接的数据库信息(如地址、数据库名、用户名、密码等),通常连接信息可以写到 web.config 中
 *     <connectionStrings>
 *       <add name="Northwind" connectionString="Data Source=localhost\SQLEXPRESS;Initial Catalog=Northwind;Integrated Security=SSPI" />
 *     </connectionStrings>
 *   Command -- 用于执行SQL语句，如 ExecuteReader() 方法返回 DataReader 数据实例; ExecuteNonQuery 执行Insertd 等
 *   DataReader -- 读取数据,会保持活动的DB连接，仅支持"快速向前只读"方式来访问记录，获取到数据后，while( myReader.Read()){ lblXXX = myReader["colName"]; }
 *   DataAdapter -- 支持断开式数据访问(DataSet), 获取数据并填充到DataSet中, adapter.Fill(dataSetPubs, "表名");
 * 数据管理类
 *   DataSet(可以做成内存数据库?) -- 数据容器，可以存储多个表，并可以定义定义表与表之间的关系；可以将数据保存到XML等文件中(可做离线存储?)
 *     访问数据：foreach(DataRow row in dataSetPubs.Tables["表名"].Rows) { lblXxx.Text = row["列名"]; }
 *   DataTable
 *   DataRow/DataColumn -- 行/列, 遍历时可以通过 GetChildRows 或 GetParentRows 获取关联表的数据
 *   DataRelation -- 定义表之间的关系(如外键约束)，然后可以在关联的表之间导航?
 *     创建外键约束： dataSetPubs.Relations.Add(new DataRelation("唯一的键名", dataSetPubs.Tables["表名1"].Columns["列名1"], 
 *       dataSetPubs.Tables["表名2"].Columns["列名2"]));
 *                    
 *   Constraint -- 约束
 * 
 * 直接数据访问
 *   创建Connection和DataReader对象 -> 检索数据并显示在Web控件中 -> 关闭连接 -> 返回页面
 *   
 * 断开式数据访问技术(将检索的数据存储在DataSet中，并在关闭数据库连接后，仍然能访问和使用数据)
 *  
 * 
 * SQL Server中的命令行工具: sqlcmd.exe，常用语执行数据库任务，如安装中创建数据库的表
 *   -S 路径 : 指定数据库服务器的位置,如 localhost\SQLEXPRESS
 *   -i SQL文件: 指定需要运行的SQL脚本文件
 *   
 * 
************************************************************************************************************/

/************************************************************************************************************
 * 数据绑定 -- 告诉控件在哪里找到数据, 以及如何显示数据，然后由控件自己处理其余的各种操作细节。
 *   声明了数据绑定后，需要在 Page.Load 等事件中通过调用 DataBind() 方法来激活
 * 注意：1.数据绑定发生在Web页面处理过程的末尾，并仅仅在Web页面被呈现之前。
 *         即 Page.Load -> 控件的事件 -> Page.PreRender -> 数据绑定 -> 回传
 *       2.每一个数据控件的绑定都会单独向 SqlDataSource 数据源控件发起请求，会触发一次独立的数据库查询(性能问题)。
 *       3.如果在Where中只是通过ID来更新数据，则可能出现并发问题，为了解决该问题，可以：
 *         a.在表中加入时间戳(Timestamp)，用时间戳来检查并发性(推荐)
 *         b.在Where中检查原始的值(即匹配每一个字段) -- 有性能问题
 *         
 *       
 *   分为两种类型：
 *     单值绑定(数据绑定表达式) -- 缺点：逻辑代码被混插到用户界面的标记代码中
 *       <%# 属性或内建对象等 %>, 如 <%# Request.Browser.Browser %> 表示当前浏览器的名称。问题：Chrome居然错误的认为是"AppleMAC-Safari"
 *         <# Eval("绑定信息,如字段名") %> -- 使用在 GridView.TemplateField 中
 *       <%$ ConnectionStrings:[NameOfConnectionString] %> -- 设置数据源控件的连接字符串信息,IDE的属性中设置 Data->Expressions ?
 *     多值绑定(通过控件的 DataSource 属性指定) -- 可以使用字段中的所有值，自动创建一个完整的列表，大大简化编码工作。
 *       支持多值绑定的控件
 *         列表控件(ListBox/DropDownList/CheckBoxList等) -- 
 *         多功能Web控件 -- 可以一次显示多个字段，并具有高度灵活的显示布局
 *           GridView -- 全功能的表格控件，用于显示数据表中的多行和多列数据
 *             其自动分页(AllowPaging)功能采用的是后台查询全部数据，然后只显示需要显示的数据，显示完毕后将数据抛弃的方式 -- 可以使用缓存提高服务器性能。
 *           DetailsView -- 每次显示一条数据记录，每一个字段被显示为表格中的一行
 *           FormView -- 类似DetailsView每次仅显示一条记录，是基于模版的，允许以灵活的布局方式来显示各个字段，而不必使用基于表格的布局
 *           ListView -- 类似GridView允许显示多行记录，是基于模版的，可获得更加灵活的布局。
 *       使用步骤：
 *         1.创建并填充某种数据对象(如数组、ArrayList、Hashtable、DataTable、DataSet等) -- 支持IEnumerable接口的集合
 *         2.将数据对象连接到相应的控件(设置控件的 DataSource/DataMember 等属性);
 *         3.激活数据绑定(DataBind 方法)
 *       控件属性(似乎都是设置字符串，然后运行时通过反射查找指定的数据变量?)
 *         DataSource
 *         DataMemeber -- 可以选择 DataSet 中的Table等
 *         DataTextField -- 显示部分(如 <option> 中的 Text)
 *         DataValueField -- 内部的值部分(如 <option> 中的 value)
 * 
 * 数据源控件(实现了 IDataSource 接口的控件，如 SqlDataSource/AccessDataSource/ObjectDataSource 等 )
 *   事件 -- 如果数据发生了变化，将会触发对应的事件，如更新(Updating,Updated),插入(Inserting,Inserted),删除(Deleting,Deleted); 
 *           在获取到数据后，将数据插入数据绑定控件是，将触发 Selecting 和 Selected 事件。
 *   属性
 *     ProviderName -- 选择数据源(的工厂?), 如 "System.Data.SqlClient"
 *     SelectParameters 等 -- 支持参数化查询
************************************************************************************************************/

/************************************************************************************************************
 * System.Web.UI.DataBinder
 *   Eval -- 静态方法，自动获取绑定到当前行的数据项，使用反射(reflection)来查找匹配的字段，并获取字段的值。
 *     <%# Eval("BirthDate", "{0:MM/dd/yy}") %>
************************************************************************************************************/

public partial class DataBindingStudy : System.Web.UI.Page
{
    public string MyPropertyInfo
    {
        get;
        set;
    }
    protected void Page_Load(object sender, EventArgs e)
    {
        if (!IsPostBack)
        {

            //可以查询数据库，并赋予 MyPropertyInfo 变量
            MyPropertyInfo = "Can Get Datas From DB";

            Dictionary<int, string> days = new Dictionary<int, string>();
            days.Add(0, "Monday");
            days.Add(1, "Tuesday");
            days.Add(2, "Wednesday");
            days.Add(3, "Thursday");
            days.Add(4, "Friday");
            days.Add(5, "Saturday");
            days.Add(6, "Sunday");

            ListBox1.DataSource = days;

            //选择想要在列表中显示的字典对象的值(Value),运行时将通过反射来检查数据对象并查找名为"Value"的属性
            ListBox1.DataTextField = "Value";

            //在控件元素中，添加相应的value属性信息，然后可在代码中访问 -- 常用于数据表的ID属性
            ListBox1.DataValueField = "Key";

            DataBind();
        }
    }
}
