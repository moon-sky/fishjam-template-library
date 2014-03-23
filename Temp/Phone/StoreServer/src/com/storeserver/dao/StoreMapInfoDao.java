package com.storeserver.dao;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import com.storeserver.dao.base.BaseDao;
import com.storeserver.domain.StoreMapInfo;
import com.storeserver.service.StoreInfoManager;

/*
CREATE TABLE [tbl_Map] (
[id] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[storeId] INTEGER  NULL,
[floor] INTEGER  UNIQUE NOT NULL,
[map] BLOB  NULL,
[path] BLOB  NULL
)
*/

public class StoreMapInfoDao extends BaseDao{
	String mStrStoreDBPath;
	List<StoreMapInfo> mStoreMaps; 
			
	public StoreMapInfoDao(String strStoreDb) {
		mStrStoreDBPath = strStoreDb;
	}

	public List<StoreMapInfo> getStoreMap() {
		if (mStoreMaps == null) {
			mStoreMaps = new ArrayList<StoreMapInfo>();
			
			try {
				Connection conn = mSqliteDBManager.GetConnection(mStrStoreDBPath);
				Statement stmt = conn.createStatement();
				ResultSet rs = stmt.executeQuery("select id, storeId, floor, map, path from tbl_Map");
				while (rs.next()) {
					StoreMapInfo mapInfo = new StoreMapInfo();
					
					mapInfo.setId(rs.getInt(1));
					mapInfo.setStoreId(rs.getInt(2));
					mapInfo.setFloor(rs.getInt(3));
					mapInfo.setMap(rs.getBytes(4));
					mapInfo.setPath(rs.getBytes(5));
					
					mStoreMaps.add(mapInfo);
				}
				rs.close();
				stmt.close();
				conn.close();
			} catch (SQLException e) {
				
				e.printStackTrace();
			}
		}
		
		return mStoreMaps;
	}

	public int insertFloorInfo(StoreInfoManager.FloorInfo floorInfo) {
		try {
			Connection conn = mSqliteDBManager.GetConnection(mStrStoreDBPath);
			PreparedStatement stmt = conn.prepareStatement("insert into tbl_Map(id, storeId, floor, map, path)"
					+ "values(null, ?, ?, ?, ?)");
			stmt.setInt(1, floorInfo.storeId);
			stmt.setInt(2,  floorInfo.floor);
			
			
			if (floorInfo.mapDataStream != null) {
				int nMapSize = floorInfo.mapDataStream.available();
				byte[] bytes = new byte[nMapSize];
				floorInfo.mapDataStream.read(bytes);
				stmt.setBytes(3, bytes);
				//stmt.setBlob(3, floorInfo.mapDataStream);
			}
			if (floorInfo.pathDataStream != null) {
				int nPathSize = floorInfo.pathDataStream.available();
				byte[] bytes = new byte[nPathSize];
				floorInfo.mapDataStream.read(bytes);
				stmt.setBytes(4, bytes);

				//stmt.setBlob(4, floorInfo.pathDataStream);
			}
			stmt.execute();
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		
		return 0;
	}

}
