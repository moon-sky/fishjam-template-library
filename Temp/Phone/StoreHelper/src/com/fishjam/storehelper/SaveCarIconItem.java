package com.fishjam.storehelper;

import com.fishjam.storehelper.zxing.IntentIntegrator;
import com.fishjam.storehelper.zxing.IntentResult;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

public class SaveCarIconItem extends StartIconInfo{
	private static final String TAG = SaveCarIconItem.class.getSimpleName();
	
	SaveCarIconItem(Activity activity) {
		super(activity);
		
		this.icon = R.drawable.car_logo_lan;
		this.name = mActivity.getResources().getString(R.string.saveCar);
	}

	@Override
	void onExecute() {
		super.onExecute();
		
		IntentIntegrator intentInte = new IntentIntegrator(mActivity);
		intentInte.initiateScan();
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		 IntentResult scanResult = IntentIntegrator.parseActivityResult(requestCode, resultCode, data);
		 if (scanResult != null) {
			String strContents = scanResult.getContents();  //scanResult.getFormatName
			Log.i(TAG, "strContents=" + strContents);  
			if (strContents != null) {
				StoreInformation.Instance(mActivity).strScanResult = strContents;
				
				Intent intent = new Intent(mActivity, PositionActivity.class);
				mActivity.startActivity(intent);

				Toast.makeText(mActivity,  strContents, Toast.LENGTH_LONG).show();
			 }	
		}
	}

}
