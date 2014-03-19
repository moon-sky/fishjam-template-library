package com.fishjam.utility.net.servlet;

import java.io.File;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

import javax.servlet.ServletContext;

public class SqliteWebDBManager {
	private ServletContext mApplication;
	private String mDBDir;
	
	//SqliteDBManager dbManger = new SqliteDBManager(request.getServletContext(), "/db");
	public SqliteWebDBManager(ServletContext application, String dbPath){
		mApplication = application;
		mDBDir = mApplication.getRealPath(dbPath);

		System.out.println("dbDir = " + mDBDir);
		File file = new File(mDBDir);
		if (!file.exists()) {
			file.mkdirs();
		}
		
		try {
			Class.forName("org.sqlite.JDBC");
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
	}
	
	public Connection GetConnection(String dbFileName) throws SQLException
	{
		Connection conn = DriverManager
				.getConnection("jdbc:sqlite:" + mDBDir + "/" + dbFileName);
		return conn;
	}
}
