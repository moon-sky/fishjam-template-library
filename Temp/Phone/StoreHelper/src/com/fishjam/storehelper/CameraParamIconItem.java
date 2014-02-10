package com.fishjam.storehelper;

import java.util.Random;
import java.util.regex.Pattern;

import android.R.integer;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.Intent;
import android.graphics.Point;
import android.hardware.Camera;
import android.util.Log;
import android.view.Display;
import android.view.WindowManager;

import com.zxing.activity.CaptureActivity;

public class CameraParamIconItem extends StartIconInfo{
	private final static String TAG = CameraParamIconItem.class.getSimpleName();
	private static final Pattern COMMA_PATTERN = Pattern.compile(",");

	CameraParamIconItem(Activity activity) {
		super(activity);
		
		this.icon = R.drawable.more;
		this.name = mActivity.getResources().getString(R.string.cameraParam);
	}
	
	@Override
	void onExecute() {
		// TODO Auto-generated method stub
		super.onExecute();
		
		Camera camera = Camera.open();
		if (camera != null ) {
		    StringBuilder sBuilder = new StringBuilder();
		    Log.w(TAG, "Before get Camera Info");
		    
			Camera.Parameters parameters  = camera.getParameters();
		    Camera.Size size = parameters.getPreviewSize();  
		    sBuilder.append("size = " + size.width + "x" + size.height + "\n");

		    int previewFormat = parameters.getPreviewFormat();
		    String previewFormatString = parameters.get("preview-format");
		    sBuilder.append("Default preview format: " + previewFormat + '/' + previewFormatString + "\n");
		    WindowManager manager = (WindowManager) mActivity.getSystemService(Context.WINDOW_SERVICE);
		    Display display = manager.getDefaultDisplay();
		    Point screenResolution = new Point(display.getWidth(), display.getHeight());
		    sBuilder.append("Screen resolution: " + screenResolution+ "\n");
		    Point cameraResolution = getCameraResolution(parameters, screenResolution);
		    sBuilder.append("Camera resolution: " + cameraResolution+ "\n");
		    
			WindowManager winMgr = mActivity.getWindowManager();
			int window_width = winMgr.getDefaultDisplay().getWidth();
			int window_height = winMgr.getDefaultDisplay().getHeight();
			sBuilder.append("Window: " + window_width + "x" + window_height);
			
		    camera.release();
		    
		    showParam(sBuilder.toString());
		}
	}

	private static Point getCameraResolution(Camera.Parameters parameters, Point screenResolution) {

	    String previewSizeValueString = parameters.get("preview-size-values");
	    // saw this on Xperia
	    if (previewSizeValueString == null) {
	      previewSizeValueString = parameters.get("preview-size-value");
	    }

	    Point cameraResolution = null;

	    if (previewSizeValueString != null) {
	      Log.d(TAG, "preview-size-values parameter: " + previewSizeValueString);
	      cameraResolution = findBestPreviewSizeValue(previewSizeValueString, screenResolution);
	    }

	    if (cameraResolution == null) {
	      // Ensure that the camera resolution is a multiple of 8, as the screen may not be.
	      cameraResolution = new Point(
	          (screenResolution.x >> 3) << 3,
	          (screenResolution.y >> 3) << 3);
	    }

	    return cameraResolution;
	  }
	
	 private static Point findBestPreviewSizeValue(CharSequence previewSizeValueString, Point screenResolution) {
		 Log.w(TAG, "findBestPreviewSizeValue for " + previewSizeValueString);
		    int bestX = 0;
		    int bestY = 0;
		    int diff = Integer.MAX_VALUE;
		    for (String previewSize : COMMA_PATTERN.split(previewSizeValueString)) {

		      previewSize = previewSize.trim();
		      int dimPosition = previewSize.indexOf('x');
		      if (dimPosition < 0) {
		        Log.w(TAG, "Bad preview-size: " + previewSize);
		        continue;
		      }

		      int newX;
		      int newY;
		      try {
		        newX = Integer.parseInt(previewSize.substring(0, dimPosition));
		        newY = Integer.parseInt(previewSize.substring(dimPosition + 1));
		      } catch (NumberFormatException nfe) {
		        Log.w(TAG, "Bad preview-size: " + previewSize);
		        continue;
		      }
		      
		      if(newX == screenResolution.x && newY == screenResolution.y)
		      {
		    	  bestX = newX;
		    	  bestY = newY;
		    	  break;
		      }
		      else if(newX == screenResolution.y && newY == screenResolution.x)
		      {
		    	  bestX = newY;
		    	  bestY = newX;
		    	  break;
		      }
		      //.zxing 的源代码是横屏的,本作者的程序是坚屏的
		      //zxing 作者的意图很明显,因为相机的预览像素不等于屏幕的显示像素,作者想找到最合适的.在预览方向和屏幕方向一致的情况下,差值最小的就是最优的.
		      //int newDiff = Math.abs(newX - screenResolution.x) + Math.abs(newY - screenResolution.y);
		      int newDiff = Math.abs(newY - screenResolution.x) + Math.abs(newX - screenResolution.y);
		      //boolean isScaleRight = ((double)newX / (double)screenResolution.y == (double)newY / (double)screenResolution.x);
		      if (newDiff == 0) {
		        bestX = newX;
		        bestY = newY;
		        break;
		      } else if (newDiff < diff) {  //else if (isScaleRight && newDiff < diff) {
		        bestX = newX;
		        bestY = newY;
		        diff = newDiff;
		      }
		    }

		    if (bestX > 0 && bestY > 0) {
		      return new Point(bestX, bestY);
		    }
		    return null;
		  }

		  private static int findBestMotZoomValue(CharSequence stringValues, int tenDesiredZoom) {
		    int tenBestValue = 0;
		    for (String stringValue : COMMA_PATTERN.split(stringValues)) {
		      stringValue = stringValue.trim();
		      double value;
		      try {
		        value = Double.parseDouble(stringValue);
		      } catch (NumberFormatException nfe) {
		        return tenDesiredZoom;
		      }
		      int tenValue = (int) (10.0 * value);
		      if (Math.abs(tenDesiredZoom - value) < Math.abs(tenDesiredZoom - tenBestValue)) {
		        tenBestValue = tenValue;
		      }
		    }
		    return tenBestValue;
		  }
		  
	private void showParam(String string) {
		AlertDialog alertDialog = new AlertDialog.Builder(mActivity).create();
		alertDialog.setMessage(string);
		alertDialog.show();
	}
}

