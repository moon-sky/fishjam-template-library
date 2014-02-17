package com.fishjam.storehelper;

import com.fishjam.storehelper.FloorMapManager.MapData;
import com.fishjam.util.AStar;

import android.R.integer;
import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.graphics.Point;
import android.util.Log;

class PositionInfo{
	public int iFloor = -1;
	public Point ptPos = new Point(0, 0);

	@Override
	public String toString() {
		return "" + iFloor +"," + ptPos.x + "," + ptPos.y;
	}

	public void reset() {
		iFloor = -1;
		ptPos.set(0, 0);
	}
}

public class StoreInformation {
	private final static String TAG = StoreInformation.class.getSimpleName();
	
    static final String KEY_CAR_POSITION = "carPosition";
    static final String KEY_CUR_POSITION = "curPosition";
	
	public static StoreInformation Instance(Activity activity){
		if (sInstanceOfStoreInformation == null) {
			sInstanceOfStoreInformation = new StoreInformation(activity);
			sInstanceOfStoreInformation.Load();
		}
		return sInstanceOfStoreInformation;
	}
	public static StoreInformation Instance(){
		return sInstanceOfStoreInformation;
	}
	private static StoreInformation	sInstanceOfStoreInformation;
	private StoreInformation(Activity activity){
		mActivity = activity;
		mCarPositionInfo = new PositionInfo();
		mUserPositionInfo = new PositionInfo();
	}

	Activity mActivity;
	PositionInfo mCarPositionInfo;
	PositionInfo mUserPositionInfo;
	String strScanResult;
	
	void LoadPositionInfo(SharedPreferences sharedPreferences, String strKey, PositionInfo positionInfo){
		String carPosString = sharedPreferences.getString(strKey, "0,0,0");
		String[] carPosStrings = carPosString.split(",");
		if (carPosStrings.length == 3) {
			positionInfo.iFloor = Integer.parseInt(carPosStrings[0]);
			positionInfo.ptPos.x = Integer.parseInt(carPosStrings[1]);
			positionInfo.ptPos.y = Integer.parseInt(carPosStrings[2]);
		}
	}
	public boolean Load(){
		SharedPreferences sharedPreferences = mActivity.getPreferences(Context.MODE_PRIVATE);
		LoadPositionInfo(sharedPreferences, KEY_CAR_POSITION, mCarPositionInfo);
		//LoadPositionInfo(sharedPreferences, KEY_CUR_POSITION, mCurPositionInfo);
		return true;
	}
	
	public boolean Save(){
		SharedPreferences sharedPreferences = mActivity.getPreferences(Context.MODE_PRIVATE);
		Editor editor = sharedPreferences.edit();
		SavePositionInfo(editor, KEY_CAR_POSITION, mCarPositionInfo);
		//SavePositionInfo(editor, KEY_CUR_POSITION, mCurPositionInfo);
		return editor.commit();
	}
	private void SavePositionInfo(Editor editor, String strKey, PositionInfo positionInfo) {
		editor.putString(strKey, positionInfo.toString());
	}
	
	public AStar.PosInfo[] findWayToCar(int nShowFloor){
		final FloorMapManager mapManager = FloorMapManager.getInstance();
		AStar.PosInfo[] posInfos = null;
		
		if (nShowFloor == mUserPositionInfo.iFloor && mUserPositionInfo.iFloor == mCarPositionInfo.iFloor) {
			//同一层中
			AStar aStar = new AStar(mapManager.getMapByFloor(mUserPositionInfo.iFloor).mTileData, 
					mUserPositionInfo.ptPos.x, mUserPositionInfo.ptPos.y, 
					mCarPositionInfo.ptPos.x, mCarPositionInfo.ptPos.y, false);
			posInfos = aStar.getResult(true);
		}else if(nShowFloor == mUserPositionInfo.iFloor){
			//显示用户所在的层，从用户到电梯处
			MapData mapData = mapManager.getMapByFloor(mUserPositionInfo.iFloor);
			AStar aStar = new AStar(mapData.mTileData, 
					mUserPositionInfo.ptPos.x, mUserPositionInfo.ptPos.y, 
					mapData.mElevatorData[0].x, mapData.mElevatorData[0].y, false);
			posInfos = aStar.getResult(true);
		}else if(nShowFloor == mCarPositionInfo.iFloor && mUserPositionInfo.iFloor != -1){
			//在汽车所在的层， 从电梯到汽车处
			MapData mapData = mapManager.getMapByFloor(mCarPositionInfo.iFloor);
			AStar aStar = new AStar(mapData.mTileData, 
					mapData.mElevatorData[0].x, mapData.mElevatorData[0].y,
					mCarPositionInfo.ptPos.x, mCarPositionInfo.ptPos.y, false);
			posInfos = aStar.getResult(true);
		}
		
		
		return posInfos;
	}
	
}
