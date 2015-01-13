<!-- 
-->

<%@ page language="java" import="java.util.*" pageEncoding="UTF-8" contentType="text/html; charset=UTF-8"%>
<!-- 《%@taglib prefix="s" uri="/struts-tags"%》 -->

<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>My JSP 'JSPStudy.jsp' starting page</title>
    
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
	<table border="1">
		<thead>
			<tr>
				<th>简介</th>
				<th>详情</th>
			</tr>
		</thead>
		<tr>
			<th>描述</th>
			<th>JSP 的本质是Servlet，tomcat 会将JSP编译成 HttpJspBase 子类( %TOMCAT_HOME%\work 目录下 ),
			  主要有 _jspInit、_jspService、_jspDestroy 三个方法
			</th>
		</tr>
        <tr>
            <th>开发</th>
            <th>JSP 在 MyEclipse中可视化开发： Window -> Show View -> Palette</th>
        </tr>
		<tr>
			<td>Cell</td>
			<td>Cell</td>
		</tr>
	</table>
	<br/>
    <br/>
    
    JSP Study Index<br>
    <table title="JSP Study Index" border="1">
		<thead>
			<tr>
				<th>索引</th>
				<th>描述</th>
				<th>链接</th>
				<th>备注</th>
			</tr>
		</thead>
		<tr>
            <td>1</td>
            <td>Cell</td>
            <td>Cell</td>
            <td>Cell</td>
        </tr>
		<tr>
			<td>2</td>
			<td>基本语法</td>
			<td><a href="./WebRoot/WEB-INF/content/JspLanguage.jsp">JspLanguage.jsp</a></td>
			<td>Cell</td>
		</tr>
		<tr>
			<td>3</td>
			<td>访问数据库</td>
			<td><a href="./WebRoot/WEB-INF/content/AccessDB.jsp">AccessDB.jsp</a></td>
			<td>Cell</td>
		</tr>
		<tr>
			<td>Cell</td>
			<td>Cell</td>
			<td>Cell</td>
			<td>Cell</td>
		</tr>
	</table>
</body>
</html>
