<%@taglib uri="/struts-tags" prefix="s"%><%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>My JSP 'MapInfo.jsp' starting page</title>
    
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
    This is Map Info JSP page
	<s:debug></s:debug>
	.
	<s:form>
	<table cellpadding="1" cellspacing="1" width="100%">
		<tr>
	<td colspan="3" >商场地图列表</td> </tr>
				<tr>
					<td>选中</td>
					<td>索引</td>
					<td>楼层</td>
					<td>地图</td>
					<td>定位数据</td>
				</tr>
			<s:iterator id="storemap" value="storeMaps" status="st">
			<tr>
				<td><input type="radio" name="mapId"  value=<s:property value="id"/> ></input></td>
				<td><s:property value="id"/></td>
				<td><s:property value="floor"/></td>
				<td><s:property value="map"/></td>
				<td><s:property value="path"/></td>
			</tr>
			</s:iterator>
		</table>
	</s:form>
	<br>
  </body>
</html>
