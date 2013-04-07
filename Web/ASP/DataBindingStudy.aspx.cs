using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

/************************************************************************************************************
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
 *   Connection -- 通过连接字符串指定连接的数据库信息(如地址、数据库名、用户名、密码等)
 *   Command -- 用于执行SQL语句，如 ExecuteReader() 方法返回 DataReader 数据实例; ExecuteNonQuery 执行Insertd 等
 *   DataReader -- 读取数据,会保持活动的DB连接，仅支持"快速向前只读"方式来访问记录，获取到数据后，while( myReader.Read()){ lblXXX = myReader["colName"]; }
 *   DataAdapter -- 支持断开式数据访问(DataSet), 获取数据并填充到DataSet中, adapter.Fill(dataSetPubs, "表名");
 *     访问数据：foreach(DataRow row in dataSetPubs.Tables["表名"].Rows) { lblXxx.Text = row["列名"]; }
 * 数据管理类
 *   DataSet -- 数据容器，可以存储多个表，并可以定义定义表与表之间的关系；可以将数据保存到XML等文件中
 *   DataTable
 *   DataRow/DataColumn -- 行/列
 *   DataRelation -- 定义表之间的关系(如外键约束)，然后可以在关联的表之间导航?
 *   Constraint -- 约束
 * 
 * 直接数据访问
 *   创建Connection和DataReader对象 -> 检索数据并显示在Web控件中 -> 关闭连接 -> 返回页面
 *   
 * 断开式数据访问技术(将检索的数据存储在DataSet中，并在关闭数据库连接后，仍然能访问和使用数据)
 *   
 * SQL Server中的命令行工具: sqlcmd.exe，常用语执行数据库任务，如安装中创建数据库的表
 *   -S 路径 : 指定数据库服务器的位置,如 localhost\SQLEXPRESS
 *   -i SQL文件: 指定需要运行的SQL脚本文件
 *   
 * 
************************************************************************************************************/

public partial class DataBindingStudy : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {

    }
}
