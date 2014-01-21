package com.fishjam.storehelper;

import java.util.Random;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.hardware.Camera;

import com.zxing.activity.CaptureActivity;

public class CameraParamIconItem extends StartIconInfo{
	CameraParamIconItem(Activity activity) {
		super(activity);
		
		this.icon = R.drawable.camera;
		this.name = mActivity.getResources().getString(R.string.cameraParam);
	}
	
	@Override
	void onClick() {
		// TODO Auto-generated method stub
		super.onClick();
		
		Camera camera = Camera.open();
		if (camera != null ) {
			Camera.Parameters parameters  = camera.getParameters();

		    Camera.Size size = parameters.getPreviewSize();  
		    
		    StringBuilder sBuilder = new StringBuilder();
		    sBuilder.append("size = " + size.width + "x" + size.height);
		    
		    showParam(sBuilder.toString());
		}
	}

	private void showParam(String string) {
		AlertDialog alertDialog = new AlertDialog.Builder(mActivity).create();
		alertDialog.setMessage(string);
		alertDialog.show();
	}
}

