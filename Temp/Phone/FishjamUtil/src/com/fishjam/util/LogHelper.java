package com.fishjam.util;

import java.util.Locale;

import android.graphics.Matrix;
import android.graphics.Point;
import android.graphics.PointF;
import android.graphics.Rect;
import android.graphics.RectF;
import android.text.BoringLayout.Metrics;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.KeyEvent;
import android.view.MotionEvent;

public final class LogHelper {
	
	public static String FormatPoint(Point point){
		StringBuilder sBuilder = new StringBuilder();
		sBuilder.append(point.x + "," + point.y);
		return sBuilder.toString();
	}
	
	public static String FormatPoint(PointF point){
		StringBuilder sBuilder = new StringBuilder();
		sBuilder.append(point.x + "," + point.y);
		return sBuilder.toString();
	}
	
	public static String FormatRect(Rect rect){
		StringBuilder sBuilder = new StringBuilder();
		sBuilder.append("(" + rect.left + "," + rect.top + ")-(" + rect.right + "," + rect.bottom
				+ "), " + rect.width() + "x" + rect.height());
		return sBuilder.toString();
	}
	
	public static String FormatRect(RectF rect){
		StringBuilder sBuilder = new StringBuilder();
		sBuilder.append("(" + rect.left + "," + rect.top + ")-(" + rect.right + "," + rect.bottom
				+ "), " + rect.width() + "x" + rect.height());
		return sBuilder.toString();
	}
	
	public static String FormatMatrix(Matrix matrix) {
		StringBuilder sBuilder = new StringBuilder();
		if (matrix != null) {
			float[] matrixValues = new float[9];
			matrix.getValues(matrixValues);
			for (int i = 0; i < 3; ++i) {
				for (int j = 0; j < 3; ++j) {
					sBuilder.append(matrixValues[3 * i + j] + ",");
				}
				if (i < 2) {
					sBuilder.append("    ");
				}
			}
		}
		return sBuilder.toString();
	}
	
	public static String FormatDisplay(Display display){
		//StringBuilder sBuilder = new StringBuilder();
		DisplayMetrics metrics = new DisplayMetrics();
		display.getMetrics(metrics);
		String string = String.format("Display {Id=%d, WxH=[%dx%d], Ori=%d, Rot=%d,Pixel=%d, [%s] }",  
				//1, 2, 3, "fishjam"
				display.getDisplayId(),  display.getWidth(), display.getHeight(),
				display.getOrientation(), display.getRotation(), display.getPixelFormat(),
				FormatMetrics(metrics)
				);
		return string;
	}
	
	private static String FormatMetrics(DisplayMetrics metrics) {
		String string = String.format("Metrics{ density=%f, scaledDensity=%f, densityDpi=%d," +
				"WxH(Pixels)=[%dx%d], Dpi=(%f,%f) }", 
				metrics.density, metrics.scaledDensity, metrics.densityDpi, 
				metrics.widthPixels, metrics.heightPixels, 
				metrics.xdpi, metrics.ydpi
				);
		return string;
	}

	public static String FormatMotionEvent(MotionEvent event){
		StringBuilder sBuilder = new StringBuilder();
		int nAction = event.getAction() & MotionEvent.ACTION_MASK;
		switch(nAction) {
		case MotionEvent.ACTION_DOWN:
			sBuilder.append("ACTION_DOWN: " + event.getX() + "," + event.getY() + ",PointerCount=" + event.getPointerCount());
			break;
		case MotionEvent.ACTION_UP:
			sBuilder.append("ACTION_UP: " + event.getX() + "," + event.getY() + ",PointerCount=" + event.getPointerCount());
			break;
		case MotionEvent.ACTION_MOVE:
			sBuilder.append("ACTION_MOVE: " + event.getX() + "," + event.getY() + ",PointerCount=" + event.getPointerCount());
			break;
		case MotionEvent.ACTION_CANCEL:
			sBuilder.append("ACTION_CANCEL");
			break;
		case MotionEvent.ACTION_OUTSIDE:
			sBuilder.append("ACTION_OUTSIDE");
			break;
		case MotionEvent.ACTION_POINTER_DOWN:
			sBuilder.append("ACTION_POINTER_DOWN: " + event.getX(1) + "," + event.getY(1) + ",PointerCount=" + event.getPointerCount());
			break;
		case MotionEvent.ACTION_POINTER_UP:
			sBuilder.append("ACTION_POINTER_UP: " + event.getX(1) + "," + event.getY(1) + ",PointerCount=" + event.getPointerCount());
			break;
//		case MotionEvent.ACTION_HOVER_MOVE:
//			sBuilder.append("ACTION_HOVER_MOVE");
//			break;
//		case MotionEvent.ACTION_SCROLL:
//			sBuilder.append("ACTION_SCROLL");
//			break;
//		case MotionEvent.ACTION_HOVER_ENTER:
//			sBuilder.append("ACTION_HOVER_ENTER");
//			break;
//		case MotionEvent.ACTION_HOVER_EXIT:
//			sBuilder.append("ACTION_HOVER_EXIT");
//			break;
		default:
			sBuilder.append("Unknown=" + nAction);
			break;
		}
		return sBuilder.toString();
	}
	public static String FormatKey(int key){
		String strResult; 
		switch (key) {
		case KeyEvent.KEYCODE_SOFT_LEFT: 	strResult = "SoftLeft"; break;
		case KeyEvent.KEYCODE_SOFT_RIGHT: 	strResult = "SoftRight";break;
		case KeyEvent.KEYCODE_HOME:			strResult = "Home";		break;
		case KeyEvent.KEYCODE_BACK:			strResult = "Back";		break;
		case KeyEvent.KEYCODE_CALL:			strResult = "Call";		break;
		case KeyEvent.KEYCODE_ENDCALL:		strResult = "EndCall";	break;
		case KeyEvent.KEYCODE_DPAD_UP:		strResult = "DPadUp";	break;
		case KeyEvent.KEYCODE_DPAD_DOWN:	strResult = "DPadDown";	break;
		case KeyEvent.KEYCODE_DPAD_LEFT:	strResult = "DPadLeft";	break;
		case KeyEvent.KEYCODE_DPAD_RIGHT:	strResult = "DPadRight";break;
		case KeyEvent.KEYCODE_VOLUME_UP:	strResult = "VolumeUp";	break;
		case KeyEvent.KEYCODE_VOLUME_DOWN:	strResult = "VolumeDown";break;
		case KeyEvent.KEYCODE_POWER:		strResult = "Power";	break;
		case KeyEvent.KEYCODE_CAMERA:		strResult = "Camera";	break;
		case KeyEvent.KEYCODE_CLEAR:		strResult = "Clear";	break;

		default:
			strResult = "Unknown:" + key;
			break;
		}
		return strResult;
	}
	public static String FormatKeyEvent(KeyEvent event){
		StringBuilder sBuilder = new StringBuilder();
		switch(event.getAction()) {
		case KeyEvent.ACTION_DOWN:
			sBuilder.append("Down");
			break;
		case KeyEvent.ACTION_MULTIPLE:
			sBuilder.append("Multiple");
			break;
		case KeyEvent.ACTION_UP:
			sBuilder.append("Up");
			break;
		}
		return sBuilder.toString();
	}
	
}
