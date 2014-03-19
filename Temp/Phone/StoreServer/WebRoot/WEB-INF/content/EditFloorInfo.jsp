<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@taglib uri="/struts-tags" prefix="s"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<base href="<%=basePath%>">

<title>My JSP 'EditFloorInfo.jsp' starting page</title>

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
	This is Edit Floor Info JSP page.
	<br>
	<s:form action="/addStoreMap" method="post" enctype="multipart/form-data">
		<table>
			<tr>
				<td>id：<s:textfield id="storeId" name="storeId"></s:textfield></td>
			</tr>
			<tr>
				<td>floor：<s:textfield id="floor" name="floor"></s:textfield></td>
			</tr>
			<tr>
				<td>Map 文件<input type="file"  id="map" name="map" ></input></td>
			</tr>
			<tr>
				<td>路径 文件<input type="file"  id="path" name="path" ></input></td>
			</tr>
			<tr>
				<td><s:submit value="提交"></s:submit></td>
			</tr>
		</table>
	</s:form>
</body>
</html>
