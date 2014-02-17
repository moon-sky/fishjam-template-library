package com.fishjam.storehelper;

import android.R.integer;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.graphics.Rect;
import android.os.Bundle;
import android.util.Log;
import android.view.ViewTreeObserver;
import android.view.ViewTreeObserver.OnGlobalLayoutListener;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.Toast;

public class PositionActivity extends Activity {
	private final static String TAG = PositionActivity.class.getSimpleName();
	
	private int window_width, window_height;// 控件宽度
	private PositionView positionView;// 自定义控件
	private int state_height;// 状态栏的高度
	private String strScanResult;
	private ViewTreeObserver viewTreeObserver;

	private int mShowFloor = 0;
	private Button mPreFloorButton;
	private Button mNextFloorButton;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		if (checkScanResult()) {
			Log.i(TAG, "PositionActivity set layout");
			requestWindowFeature(Window.FEATURE_NO_TITLE);
			setContentView(R.layout.layout_position);// positionView);
			
			positionView = (PositionView)findViewById(R.id.positionView);
			positionView.setmActivity(this);
			//positionView.setFloor(mShowFloor);
			
			// setContentView(R.layout.main);
			/** 获取可見区域高度 **/
			WindowManager manager = getWindowManager();
			window_width = manager.getDefaultDisplay().getWidth();
			window_height = manager.getDefaultDisplay().getHeight();

			mPreFloorButton = (Button)findViewById(R.id.btnPreFloor);
			mPreFloorButton.setOnClickListener(new View.OnClickListener() {
				
				@Override
				public void onClick(View v) {
					switchFloor(-1);
					//Toast.makeText(PositionActivity.this, "PreFloorBtnClick", Toast.LENGTH_SHORT).show();
				}
			});
			
			mNextFloorButton = (Button)findViewById(R.id.btnNextFloor);
			mNextFloorButton.setOnClickListener(new View.OnClickListener() {
				
				@Override
				public void onClick(View v) {
					switchFloor(1);
					//Toast.makeText(PositionActivity.this, "NextFloorBtnClick", Toast.LENGTH_SHORT).show();
				}
			});

			switchFloor(0);

			/** 测量状态栏高度 **/
			viewTreeObserver = positionView.getViewTreeObserver();
			viewTreeObserver.addOnGlobalLayoutListener(new OnGlobalLayoutListener() {

				@Override
				public void onGlobalLayout() {
					if (state_height == 0) {
						// 获取状况栏高度
						Rect frame = new Rect();
						getWindow().getDecorView().getWindowVisibleDisplayFrame(frame);
						//state_height = frame.top;
						Log.i("onGlobalLayout", "new window_h="
								+ (window_height - state_height)
								+ ",Screen_W=" + window_width);
						positionView.setScreen_H(window_height
								- state_height);
						positionView.setScreen_W(window_width);
					}
				}
			});
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

	protected void switchFloor(int nChange) {
		int newTargetFloor = mShowFloor + nChange;
		final int totalFloorCount = FloorMapManager.getInstance().getMapCount();
		if (newTargetFloor >= 0 && newTargetFloor < totalFloorCount) {
			mShowFloor = newTargetFloor;
			positionView.setFloor(newTargetFloor);
		}
		mPreFloorButton.setEnabled( newTargetFloor >= 0);
		mNextFloorButton.setEnabled( newTargetFloor < totalFloorCount );
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
				mShowFloor = storeInformation.mCarPositionInfo.iFloor = 0;
				storeInformation.mCarPositionInfo.ptPos.set(31, 31);//16, 17);
			}
			else if(strPos.equals("CarPos2")){
				mShowFloor = storeInformation.mCarPositionInfo.iFloor = 1;
				storeInformation.mCarPositionInfo.ptPos.set(48, 32); //)(42, 43);
			}
			else if(strPos.equals("UserPos1")){
				Log.i(TAG, "Will Set As UserPos1");
				mShowFloor = storeInformation.mUserPositionInfo.iFloor = 0;
				storeInformation.mUserPositionInfo.ptPos.set(35, 16);
			}
			else if(strPos.equals("UserPos2")){
				Log.i(TAG, "Will Set As UserPos2");
				mShowFloor = storeInformation.mUserPositionInfo.iFloor = 1;
				storeInformation.mUserPositionInfo.ptPos.set(47, 5);
			}
			else{
				Log.e(TAG, "UnHandled Scan Result: " + strPos);
			}
			return true;
		}
		return false;
	}

}
