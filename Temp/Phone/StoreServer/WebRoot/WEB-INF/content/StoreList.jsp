<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";

%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>My JSP 'StoreMgr.jsp' starting page</title>
    
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
    Store Manager.
	<s:debug></s:debug>
	<br>
    <div>
		<table cellpadding="1"
	cellspacing="1" style="border-bottom-style: double; border-bottom-width: thin; border-top-style: dotted; border-left-color: #000000; border-top-color: #000000; border-left-style: inset; border-right-color: #000000; border-right-width: thin; border-right-style: dashed; border-left-width: thin; border-bottom-color: #000000; border-top-width: thin">
		<tr>
	<td colspan="3" >商场列表</td> </tr>
				<tr>
					<td>名字</td>
					<td>地址</td>
					<td>数据库名</td>
				</tr>
			<s:iterator id="store" value="stores" status="st">
			<tr>
				<td><s:property value="name"/></td>
				<td><s:property value="position"/></td>
				<td><s:property value="dbName"/></td>
			</tr>
			</s:iterator>
		</table>
	</div>
    
</body>
</html>
