package com.fishjam.storehelper;

import android.app.Activity;
import android.content.Intent;

public class StartIconInfo {
	public int 			icon;
	public String 	name;
	
	Activity mActivity;
	StartIconInfo(Activity activity)
	{
		mActivity = activity;
	}
	void onExecute(){
		
	}
	public void onActivityResult(int requestCode, int resultCode, Intent data) {

	}
}