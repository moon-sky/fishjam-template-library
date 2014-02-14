package com.fishjam.storehelper;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

public class SaveCarIconItem extends StartIconInfo{

	SaveCarIconItem(Activity activity) {
		super(activity);
		
		this.icon = R.drawable.car_logo_lan;
		this.name = mActivity.getResources().getString(R.string.saveCar);
	}

	@Override
	void onExecute() {
		super.onExecute();
		
		PositionInfo carPositionInfo = StoreInformation.Instance(mActivity).mCarPositionInfo;
		carPositionInfo.iFloor = 0;
		carPositionInfo.ptPos.set(150,  200);
		
		Intent intent = new Intent(mActivity, PositionActivity.class);
		mActivity.startActivity(intent);
		
		//IntentIntegrator intentInte = new IntentIntegrator(mActivity);
		//intentInte.initiateScan();
		
		//Toast.makeText(mActivity, "save car", Toast.LENGTH_LONG).show();		
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (resultCode == Activity.RESULT_OK) {
			Bundle bundle = data.getExtras();
			String scanResult = bundle.getString("result");
			
			PositionInfo carPositionInfo = StoreInformation.Instance(mActivity).mCarPositionInfo;
			carPositionInfo.iFloor = 0;
			carPositionInfo.ptPos.set(150,  200);
			
			Intent intent = new Intent(mActivity, PositionActivity.class);
			mActivity.startActivity(intent);

			//Toast.makeText(mActivity,  scanResult, Toast.LENGTH_LONG).show();
		}
	}

}
