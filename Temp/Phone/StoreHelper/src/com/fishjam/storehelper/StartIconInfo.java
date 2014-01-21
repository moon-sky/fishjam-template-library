package com.fishjam.storehelper;

import android.content.Context;
import android.content.ContextWrapper;
import android.util.Log;

public class StartIconInfo {
	public int 			icon;
	public String 	name;
	
	Context mContext;
	StartIconInfo(Context context)
	{
		mContext = context;
	}
	void onClick(){
		//Log.i(TAG,  name + " is Clicked");
	}
}