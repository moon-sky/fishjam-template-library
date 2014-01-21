package com.fishjam.storehelper;

import java.util.Random;

import com.zxing.activity.CaptureActivity;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class FindCarIconItem extends StartIconInfo{

	FindCarIconItem(Activity activity) {
		super(activity);
		
		this.icon = R.drawable.car_normal;
		this.name = mActivity.getResources().getString(R.string.findCar);
	}

	Random mRandom = new Random();
	@Override
	void onClick() {
		super.onClick();

		Intent openCameraIntent = new Intent(mActivity, CaptureActivity.class);
		mActivity.startActivityForResult(openCameraIntent, 0);
		/*
		Intent intent = new Intent(mContext, PositionActivity.class);
		intent.putExtra(PositionActivity.KEY_CUR_POSITION, new int[] { 150, 200});
		int nFloor = mRandom.nextInt(2);
		Log.i("FindCar -- ",  Integer.valueOf(nFloor).toString());
		intent.putExtra(PositionActivity.KEY_FLOOR,  nFloor);
		mContext.startActivity(intent);
		*/
	}

}
