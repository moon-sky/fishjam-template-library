package com.storeserver.dao.base;

import javax.servlet.ServletContext;

import com.fishjam.utility.net.servlet.SqliteWebDBManager;

public class BaseDao {
	protected 	SqliteWebDBManager mSqliteDBManager;
	
	public void InitSqliteDB(ServletContext servletContext) {
		if (mSqliteDBManager == null) {
			mSqliteDBManager = new SqliteWebDBManager(servletContext, "/db");
		}

	}
	
}
