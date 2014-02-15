package com.fishjam.storehelper;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.fishjam.storehelper.zxing.IntentIntegrator;
import com.fishjam.storehelper.zxing.IntentResult;
import com.fishjam.util.CrashHandler;

public class FindCarIconItem extends StartIconInfo{
	private static final String TAG = FindCarIconItem.class.getSimpleName();
	
	FindCarIconItem(Activity activity) {
		super(activity);
		
		this.icon = R.drawable.car_normal;
		this.name = mActivity.getResources().getString(R.string.findCar);
	}

	@Override
	void onExecute() {
		IntentIntegrator intentInte = new IntentIntegrator(mActivity);
		intentInte.initiateScan();
		/*
		Intent openCameraIntent = new Intent(mActivity, CaptureActivity.class);
		mActivity.startActivityForResult(openCameraIntent, 0);
		*/
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		 IntentResult scanResult = IntentIntegrator.parseActivityResult(requestCode, resultCode, data);
		 if (scanResult != null) {
 			String strContents = scanResult.getContents();  //scanResult.getFormatName
 			if (strContents != null) {
 	 			Log.i(TAG, "scanResult=" + strContents);  
 				 
 	 			StoreInformation.Instance(mActivity).strScanResult = strContents;
 	 			 
 	 			//PositionInfo curPositionInfo = StoreInformation.Instance(mActivity).mCurPositionInfo;
 				//curPositionInfo.iFloor = 0;
 				//curPositionInfo.ptPos.set(450,  400);
 				
 				Intent intent = new Intent(mActivity, PositionActivity.class);
 				mActivity.startActivity(intent);
 				Toast.makeText(mActivity,  strContents, Toast.LENGTH_LONG).show();				
			}
		}
		/*/

		if (resultCode == Activity.RESULT_OK) {
			Bundle bundle = data.getExtras();
			String scanResult = bundle.getString("result");
			Log.i(TAG, "scanResult=" + scanResult);
			
			PositionInfo curPositionInfo = StoreInformation.Instance(mActivity).mCurPositionInfo;
			curPositionInfo.iFloor = 0;
			curPositionInfo.ptPos.set(450,  400);
			
			Intent intent = new Intent(mActivity, PositionActivity.class);
			mActivity.startActivity(intent);

			Toast.makeText(mActivity,  scanResult, Toast.LENGTH_LONG).show();
		}
		//*/
	}
}
