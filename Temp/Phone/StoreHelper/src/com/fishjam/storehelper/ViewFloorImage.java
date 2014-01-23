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
	void onClick() {
		super.onClick();
		
		Intent intent = new Intent(mActivity, PositionActivity.class);
		intent.putExtra(PositionActivity.KEY_CUR_POSITION, new int[] { 150, 200});
		int nFloor = 0;
		intent.putExtra(PositionActivity.KEY_FLOOR,  nFloor);
		mActivity.startActivity(intent);
	}

}
