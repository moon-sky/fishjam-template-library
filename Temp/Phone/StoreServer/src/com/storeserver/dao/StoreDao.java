package com.storeserver.dao;

import java.io.UnsupportedEncodingException;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import com.storeserver.dao.base.BaseDao;
import com.storeserver.domain.Store;

public class StoreDao extends BaseDao {


	ArrayList<Store> mStores;
	
	public List<Store> getStoreList() {
		if (mStores == null) {
			mStores = new ArrayList<Store>();
			
			try {
				Connection conn =  mSqliteDBManager.GetConnection("stores.db");
				Statement stmt = conn.createStatement();
				ResultSet rs = stmt.executeQuery("select id,name,position,dbname from tbl_Stores");
				while (rs.next()) {
					Store store = new Store();
					
					store.setId(rs.getInt(1));
					
					byte[] val = rs.getBytes(2); 
					store.setName(new String(val,"GBK"));
					
					val = rs.getBytes(3);
					store.setPosition(new String(val, "GBK"));
					
					val = rs.getBytes(4);
					store.setDbName(new String(val, "GBK"));
					mStores.add(store);
				}
				rs.close();
				stmt.close();
				conn.close();
			} catch (SQLException e) {
				e.printStackTrace();
			} catch (UnsupportedEncodingException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		return mStores;
	}

	public Store getStoreByStoreId(int storeId) {
		getStoreList();
		if (mStores != null) {
			for (Store store : mStores) {
				if(store.getId() == storeId){
					return store;
				}
			}
		}
		return null;
	}

}
