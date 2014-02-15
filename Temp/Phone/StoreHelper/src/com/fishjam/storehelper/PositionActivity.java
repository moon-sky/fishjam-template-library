package com.fishjam.storehelper;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.graphics.Rect;
import android.os.Bundle;
import android.util.Log;
import android.view.ViewTreeObserver;
import android.view.ViewTreeObserver.OnGlobalLayoutListener;
import android.view.Window;
import android.view.WindowManager;

public class PositionActivity extends Activity {
	private final static String TAG = PositionActivity.class.getSimpleName();
	
	//private int window_width, window_height;// 控件宽度
	private PositionView positionView;// 自定义控件
	//private int state_height;// 状态栏的高度
	private String strScanResult;
	//private ViewTreeObserver viewTreeObserver;

	private int mShowFloor = 0;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		if (checkScanResult()) {
			requestWindowFeature(Window.FEATURE_NO_TITLE);
			// setContentView(R.layout.main);
			/** 获取可見区域高度 **/
			//WindowManager manager = getWindowManager();
			//window_width = manager.getDefaultDisplay().getWidth();
			//window_height = manager.getDefaultDisplay().getHeight();

			positionView = new PositionView(this);
			// positionView.setScaleType(ImageView.ScaleType.CENTER);
			positionView.setFloor(mShowFloor);
			setContentView(positionView);

			// dragImageView = (DragImageView) findViewById(R.id.div_main);
			// Bitmap bmp = BitmapUtil.ReadBitmapById(this, R.drawable.huoying,
			// window_width, window_height);
			// 设置图片

//			/** 测量状态栏高度 **/
//			viewTreeObserver = positionView.getViewTreeObserver();
//			viewTreeObserver.addOnGlobalLayoutListener(new OnGlobalLayoutListener() {
//
//				@Override
//				public void onGlobalLayout() {
//					if (state_height == 0) {
//						// 获取状况栏高度
//						Rect frame = new Rect();
//						getWindow().getDecorView()
//								.getWindowVisibleDisplayFrame(frame);
//						state_height = frame.top;
//						Log.i("onGlobalLayout", "new window_h="
//								+ (window_height - state_height)
//								+ ",Screen_W=" + window_width);
//						positionView.setScreen_H(window_height
//								- state_height);
//						positionView.setScreen_W(window_width);
//					}
//				}
//			});
		}
		else {
			AlertDialog.Builder dlgBuilder = new AlertDialog.Builder(this);
			dlgBuilder
					.setMessage(R.string.UnsorportFormat)
					.setCancelable(false)
					.setPositiveButton("确定",
							new DialogInterface.OnClickListener() {

								@Override
								public void onClick(DialogInterface dialog,
										int which) {
									finish();
								}
							});
			AlertDialog alert = dlgBuilder.create(); // 创建对话框实例
			alert.show(); // 显示
		}
	}

	private boolean checkScanResult() {
		StoreInformation storeInformation = StoreInformation.Instance();
		//storeInformation.mCarPositionInfo.reset();
		storeInformation.mUserPositionInfo.reset();
		
		strScanResult = storeInformation.strScanResult;
		if(strScanResult != null && strScanResult.startsWith("http://www.storehelper.com/")){
			String strPos = strScanResult.substring("http://www.storehelper.com/".length());
			Log.i(TAG, "strPos=" + strPos);
			
			if (strPos.equals("CarPos1")) {
				Log.i(TAG, "Will Set As CarPos1");
				storeInformation.mCarPositionInfo.iFloor = 0;
				storeInformation.mCarPositionInfo.ptPos.set(8, 30);//16, 17);
				mShowFloor = 0;
			}
			else if(strPos.equals("CarPos2")){
				storeInformation.mCarPositionInfo.iFloor = 0;
				storeInformation.mCarPositionInfo.ptPos.set(47, 44);
				mShowFloor = 0;
			}
			else if(strPos.equals("UserPos1")){
				Log.i(TAG, "Will Set As UserPos1");
				storeInformation.mUserPositionInfo.iFloor = 0;
				storeInformation.mUserPositionInfo.ptPos.set(38, 10);
				mShowFloor = 0;
			}
			else if(strPos.equals("UserPos2")){
				Log.i(TAG, "Will Set As UserPos2");
				storeInformation.mUserPositionInfo.iFloor = 1;
				storeInformation.mUserPositionInfo.ptPos.set(35, 31);
				mShowFloor = 0;
			}
			else{
				Log.e(TAG, "UnHandled Scan Result: " + strPos);
			}
			return true;
		}
		return false;
	}

}
