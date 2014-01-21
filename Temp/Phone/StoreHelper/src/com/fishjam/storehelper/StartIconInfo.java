package com.fishjam.storehelper;

import android.app.Activity;

public class StartIconInfo {
	public int 			icon;
	public String 	name;
	
	Activity mActivity;
	StartIconInfo(Activity activity)
	{
		mActivity = activity;
	}
	void onClick(){
		//Log.i(TAG,  name + " is Clicked");
	}
}