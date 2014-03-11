package com.fishjam.utility.db;

/*******************************************************************************
 * JDBC -- Java Database Connectivity 
 * JDBC通过驱动管理器(Driver Manager)动态维护所有数据库查询所需的所有驱动程序对象，
 *   使用Class.forName("类名全称")强迫装载驱动，并会自动注册到管理器中
 * 产生数据库URL("jdbc:子协议:数据库标识符")--如 jdbc:odbc:mydatabase
 *   或 jdbc:rmi://192.168.0.1:1099/jdbc:cloudscape:db -- 远程RMI使用DB
 *   DriverManager.getConnection("").createStatement().executeQuery("");//记住close
*******************************************************************************/

/*******************************************************************************
 * 打开顺序
 * 关闭顺序:  ResultSet -> PreparedStatement -> Connection
 * 
 * 常见DB相关的类   
 *   Connection
 *     createStatement() -- 创建执行环境，用于 SQL 中不带 "?" 参数的情况
 *     prepareStatement(sql) -- 准备select等语句的执行环境，用于SQL中带 "?" 参数的情况
 *   DriverManager
 *     getConnection()
 *   PreparedStatement
 *     executeUpdate() -- 执行更新相关的语句
 *     executeQuery() -- 执行 select 相关的语句，返回 ResultSet
 *     setObject -- 替换SQL语句中的"?" 参数
 *   ResultSet -- query 后返回的结果集
 *     getMetaData() -- 获取
 *     next() -- 到下一条记录，如还有则返回true，之后可通过 getString 等方法获取数据
 *     updateString -- 更新字段值
 *     updateRow -- 更新一行
 *   ResultSetMetaData -- ?
 *     getColumnCount() -- 获取列数目
*******************************************************************************/

public class DataBaseStudy
{
}
