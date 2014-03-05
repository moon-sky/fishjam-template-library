<%@ page language="java" import="java.util.*,javax.naming.*,java.sql.*,javax.sql.*" pageEncoding="ISO-8859-1"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>My JSP 'AccessDB.jsp' starting page</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->

  </head>
  
  <body>
    <%
//初始化Context，使用InitialContext初始化Context
Context ctx=new InitialContext(); 
/*
通过JNDI查找数据源，该JNDI为java:comp/env/jdbc/dstest，分成两个部分
java:comp/env是Tomcat固定的，Tomcat提供的JNDI绑定都必须加该前缀
jdbc/dstest是定义数据源时的数据源名
*/
DataSource ds=(DataSource)ctx.lookup("java:comp/env/" + "jdbc/sqlitedemo");
//获取数据库连接
Connection conn=ds.getConnection();
//获取Statement
Statement stmt=conn.createStatement();
//执行查询，返回ResulteSet对象
ResultSet rs=stmt.executeQuery("select * from tlb_Students");
out.println("Before output tlb_Students<br/>");
while(rs.next())
{
	out.println(rs.getString(1) 
		+ "\t" + rs.getString(2) + "<br/>");
}
out.println("After output tlb_Students<br/>");
%>
  </body>
</html>
