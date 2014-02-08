package com.fishjam.storehelper;

import android.app.Activity;
import android.widget.Toast;

public class SaveCarIconItem extends StartIconInfo{

	SaveCarIconItem(Activity activity) {
		super(activity);
		
		this.icon = R.drawable.car_logo_lan;
		this.name = mActivity.getResources().getString(R.string.saveCar);
	}

	@Override
	void onClick() {
		super.onClick();
		
		Toast.makeText(mActivity, "save car", Toast.LENGTH_LONG).show();		
	}

}
