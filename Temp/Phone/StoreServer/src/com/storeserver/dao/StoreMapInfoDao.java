package com.storeserver.dao;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import com.storeserver.dao.base.BaseDao;
import com.storeserver.domain.PositionInfo;
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

			Connection conn = null;
			Statement stmt = null;
			ResultSet rs = null;
			try {
				conn = mSqliteDBManager.GetConnection(mStrStoreDBPath);
				stmt = conn.createStatement();
				rs = stmt.executeQuery("select id, storeId, floor, map, path from tbl_Map order by floor");
				mStoreMaps = new ArrayList<StoreMapInfo>();
				while (rs.next()) {
					StoreMapInfo mapInfo = new StoreMapInfo();
					
					mapInfo.setId(rs.getInt(1));
					mapInfo.setStoreId(rs.getInt(2));
					mapInfo.setFloor(rs.getInt(3));
					mapInfo.setMap(rs.getBytes(4));
					mapInfo.setPath(rs.getBytes(5));
					
					mStoreMaps.add(mapInfo);
				}
			} catch (SQLException e) {
				e.printStackTrace();
			}
			finally{
				try {
					if (rs != null) {
						rs.close();
					}
					if (stmt != null) {
						stmt.close();
					}
					if (conn != null) {
						conn.close();
					}
				} catch (Exception ex) {
					ex.printStackTrace();
				}
			}
		}
		
		return mStoreMaps;
	}

	public int insertFloorInfo(StoreInfoManager.FloorInfo floorInfo) {
		Connection conn = null;
		PreparedStatement stmt = null;
		try {
			conn = mSqliteDBManager.GetConnection(mStrStoreDBPath);
			if (floorInfo.id != -1) {
				stmt = conn.prepareStatement("update tbl_Map(id, storeId, floor, map, path)"
						+ "values(null, ?, ?, ?, ?)");  
			}else{
				stmt = conn.prepareStatement("insert into tbl_Map(id, storeId, floor, map, path)"
						+ "values(null, ?, ?, ?, ?)");  
			}
			//stmt = conn.prepareStatement("replace into tbl_Map(id, storeId, floor, map, path)"  
			//stmt = conn.prepareStatement("insert into tbl_Map(id, storeId, floor, map, path)"
					
			stmt.setInt(1, floorInfo.storeId);
			stmt.setInt(2,  floorInfo.floor);
			
			if (floorInfo.mapDataStream != null) {
				int nMapSize = floorInfo.mapDataStream.available();
				byte[] bytes = new byte[nMapSize];
				floorInfo.mapDataStream.read(bytes);
				stmt.setBytes(3, bytes);
				//InputStream in = new FileInputStream(file);
	            //stmt.setBinaryStream(3, in,(int) file.length());
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
			mStoreMaps = null;
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		finally{
			try {
				if(stmt != null){
					stmt.close();
				}
				if (conn != null) {
					conn.close();
				}
			} catch (Exception ex) {
				ex.printStackTrace();
			}
		}
		
		return 0;
	}

	public StoreMapInfo getStoreMapInfo(int mapId) {
		getStoreMap();
		for (StoreMapInfo mapInfo : mStoreMaps) {
			if (mapInfo.getId() == mapId) {
				return mapInfo;
			}
		}
		return null;
	}

	public PositionInfo getPositionInfo(String strPosition) {
		Connection conn = null;
		PreparedStatement stmt = null;
		ResultSet rs = null;
		PositionInfo posInfo = null;
		try {
			int nPosId = Integer.parseInt(strPosition);
			conn = mSqliteDBManager.GetConnection(mStrStoreDBPath);
			stmt = conn.prepareStatement("select id, mapId, PosX, PosY,Desc from tbl_Position where id=?");
			stmt.setInt(1, nPosId);
			rs = stmt.executeQuery();
			if(rs.next()) {
					posInfo = new PositionInfo();
					posInfo.setId(rs.getInt(1));
					//posInfo.setStoreMapInfo(mapInfo);
					posInfo.setMapId(rs.getInt(2));
					posInfo.setX(rs.getInt(3));
					posInfo.setY(rs.getInt(4));
					posInfo.setDesc(rs.getString(5));
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		finally{
			try {
				if (rs != null) {
					rs.close();
				}
				if (stmt != null) {
					stmt.close();
				}
				if (conn != null) {
					conn.close();
				}
			} catch (Exception ex) {
				ex.printStackTrace();
			}
		}
		if (posInfo != null) {
			posInfo.setStoreMapInfo(getStoreMapInfo(posInfo.getMapId()));
			return posInfo;
		}
		return null;
	}

}
