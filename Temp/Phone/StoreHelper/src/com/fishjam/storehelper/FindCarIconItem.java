package com.fishjam.storehelper;

import java.util.Random;

import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class FindCarIconItem extends StartIconInfo{

	FindCarIconItem(Context context) {
		super(context);
		
		this.icon = R.drawable.car_normal;
		this.name = "FindCar";
	}

	Random mRandom = new Random();
	@Override
	void onClick() {
		super.onClick();
		
		Intent intent = new Intent(mContext, PositionActivity.class);
		intent.putExtra(PositionActivity.KEY_CUR_POSITION, new int[] { 150, 200});
		int nFloor = mRandom.nextInt(2);
		Log.i("FindCar -- ",  Integer.valueOf(nFloor).toString());
		intent.putExtra(PositionActivity.KEY_FLOOR,  nFloor);
		mContext.startActivity(intent);
	}

}
