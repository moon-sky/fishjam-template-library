package com.fishjam.storehelper;

import android.app.Activity;
import android.content.Intent;


public class ViewFloorImage extends StartIconInfo{

	ViewFloorImage(Activity activity) {
		super(activity);
		
		this.icon = R.drawable.bank_atm;
		this.name = mActivity.getResources().getString(R.string.viewImage);
	}

	@Override
	void onExecute() {
		super.onExecute();
		
		PositionInfo carPositionInfo = StoreInformation.Instance(mActivity).mCarPositionInfo;
		carPositionInfo.iFloor = 0;
		carPositionInfo.ptPos.set(150,  200);
		
		PositionInfo curPositionInfo = StoreInformation.Instance(mActivity).mCurPositionInfo;
		curPositionInfo.iFloor = 0;
		curPositionInfo.ptPos.set(350,  400);
		
		Intent intent = new Intent(mActivity, PositionActivity.class);
		
		/*
		intent.putExtra(PositionActivity.KEY_CUR_POSITION, new int[] { 150, 200});
		int nFloor = 0;
		intent.putExtra(PositionActivity.KEY_FLOOR,  nFloor);
		*/
		mActivity.startActivity(intent);
		
		
	}

}
