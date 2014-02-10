package com.fishjam.storehelper;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.graphics.Point;

class PositionInfo{
	public int iFloor;
	public Point ptPos = new Point(0, 0);

	@Override
	public String toString() {
		return "" + iFloor +"," + ptPos.x + "," + ptPos.y;
	}
}

public class StoreInformation {
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
		mCurPositionInfo = new PositionInfo();
	}

	Activity mActivity;
	PositionInfo mCarPositionInfo;
	PositionInfo mCurPositionInfo;
	
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
		LoadPositionInfo(sharedPreferences, KEY_CUR_POSITION, mCurPositionInfo);
		return true;
	}
	
	public boolean Save(){
		SharedPreferences sharedPreferences = mActivity.getPreferences(Context.MODE_PRIVATE);
		Editor editor = sharedPreferences.edit();
		SavePositionInfo(editor, KEY_CAR_POSITION, mCarPositionInfo);
		SavePositionInfo(editor, KEY_CUR_POSITION, mCurPositionInfo);
		return editor.commit();
	}
	private void SavePositionInfo(Editor editor, String strKey, PositionInfo positionInfo) {
		editor.putString(strKey, positionInfo.toString());
	}
	
	
}
