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
	<s:form action="/mapInfo">
    <div>
		<table cellpadding="1" cellspacing="1" width="100%">
		<tr>
	<td colspan="3" >商场列表</td> </tr>
				<tr>
					<td>选中</td>
					<td>索引</td>
					<td>名字</td>
					<td>地址</td>
					<td>数据库名</td>
				</tr>
			<s:iterator id="store" value="stores" status="st">
			<tr>
				<td><input type="radio" name="stroreDbName"  value=<s:property value="dbName"/> ></input></td>
				<td><s:property value="id"/></td>
				<td><s:property value="name"/></td>
				<td><s:property value="position"/></td>
				<td><s:property value="dbName"/></td>
			</tr>
			</s:iterator>
		</table>
	</div>
		<input type="submit" value="编辑">
    </s:form>
</body>
</html>
