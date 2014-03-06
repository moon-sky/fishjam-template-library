package com.fishjam.utility.db;

/*******************************************************************************
 * JDBC -- Java Database Connectivity 
 * JDBC通过驱动管理器(Driver Manager)动态维护所有数据库查询所需的所有驱动程序对象，
 *   使用Class.forName("类名全称")强迫装载驱动，并会自动注册到管理器中
 * 产生数据库URL("jdbc:子协议:数据库标识符")--如 jdbc:odbc:mydatabase
 *   或 jdbc:rmi://192.168.0.1:1099/jdbc:cloudscape:db -- 远程RMI使用DB
 *   DriverManager.getConnection("").createStatement().executeQuery("");//记住close
 *   
*******************************************************************************/

public class DataBaseStudy
{
}
