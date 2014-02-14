package com.fishjam.storehelper;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import com.fishjam.storehelper.zxing.IntentIntegrator;
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
		//super.onExecute();

		
		IntentIntegrator intentInte = new IntentIntegrator(mActivity);
		intentInte.initiateScan();
		
		/*
		Intent openCameraIntent = new Intent(mActivity, CaptureActivity.class);
		mActivity.startActivityForResult(openCameraIntent, 0);
		*/
		
		/*
		Intent intent = new Intent(mContext, PositionActivity.class);
		intent.putExtra(PositionActivity.KEY_CUR_POSITION, new int[] { 150, 200});
		int nFloor = mRandom.nextInt(2);
		Log.i("FindCar -- ",  Integer.valueOf(nFloor).toString());
		intent.putExtra(PositionActivity.KEY_FLOOR,  nFloor);
		mContext.startActivity(intent);
		*/
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		//super.onActivityResult(requestCode, resultCode, data);
		
		if (resultCode == Activity.RESULT_OK) {
			Bundle bundle = data.getExtras();
			String scanResult = bundle.getString("result");
			
			PositionInfo curPositionInfo = StoreInformation.Instance(mActivity).mCurPositionInfo;
			curPositionInfo.iFloor = 0;
			curPositionInfo.ptPos.set(450,  400);
			
			Intent intent = new Intent(mActivity, PositionActivity.class);
			mActivity.startActivity(intent);

			Log.i(TAG, scanResult);
			//Toast.makeText(mActivity,  scanResult, Toast.LENGTH_LONG).show();
		}
	}

}
