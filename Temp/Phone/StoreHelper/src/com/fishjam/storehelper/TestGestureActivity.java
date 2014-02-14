package com.fishjam.storehelper;

import android.app.Activity;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;

import com.fishjam.util.AStar;
import com.fishjam.util.CrashHandler;
import com.fishjam.util.LogHelper;

//http://blog.csdn.net/pathuang68/article/details/6992085 -- 手势后通过Matrix缩放、移动图片(为何不用 ScaleGestureDetector ? 版本问题？ )


class TestGestureIconInfo extends StartIconInfo{

	TestGestureIconInfo(Activity activity) {
		super(activity);
		this.name = "TestGesture";
		this.icon = R.drawable.bus_service;
	}

	@Override
	void onExecute() {
		CrashHandler.getInstance().testShowCrashActivity(mActivity);
		//Intent intent = new Intent(mActivity, TestGestureActivity.class);
		//mActivity.startActivity(intent);
	}
}

public class TestGestureActivity extends Activity{ // implements OnTouchListener{
	final static String TAG = TestGestureActivity.class.getSimpleName();
	
	private static final int GRID_WIDTH = 8;
	private static final int GRID_HEIGHT = 8;
	
	PositionView mImageView;
	FloorMapManager mFloorMapManager;
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		Log.i(TAG, "onKeyDown , keyCode=" + keyCode +",event=" + LogHelper.FormatKeyEvent(event));
		return super.onKeyDown(keyCode, event);
	}

	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		return super.onKeyUp(keyCode, event);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.test_gesture);
		//mDetector = new ScaleGestureDetector(this, mGestureListener);
		mImageView = (PositionView)findViewById(R.id.imageViewTest);
		StoreInformation storeInformation = StoreInformation.Instance();
		Resources resources = getResources();
		Bitmap bitmap = BitmapFactory.decodeStream(resources.openRawResource(R.raw.floor_1)).copy(Bitmap.Config.RGB_565, true);
		Log.i(TAG, "Bitmap Size=" + bitmap.getWidth() + "," + bitmap.getHeight());
		Canvas canvas = new Canvas(bitmap);
		Paint paintRoute = new Paint();
		paintRoute.setColor(Color.RED);

		mFloorMapManager = FloorMapManager.getInstance();
		
		AStar aStar = new AStar(mFloorMapManager.getMapByFloor(0).mTileData, 6, 32, 30, 48, false);
		AStar.PosInfo[] posInfos = aStar.getResult(true);
		if (posInfos != null) {
			int nStartLine = 0;
			int nStartCol = 0;
			StringBuilder stringBuilder = new StringBuilder();
			
			for (int i = posInfos.length - 1; i >=0 ; i--) {
				nStartLine = posInfos[i].col * GRID_WIDTH;
				nStartCol = posInfos[i].row * GRID_HEIGHT;
				stringBuilder.append("=>[" + posInfos[i].row + "," + posInfos[i].col + "," + 
						aStar.getDirString(posInfos[i].dir)  +  "]");
				canvas.drawRect(nStartLine + GRID_HEIGHT / 4 , nStartCol + GRID_WIDTH / 4 ,
						nStartLine + GRID_WIDTH  * 3 / 4,  nStartCol + GRID_HEIGHT * 3 /4, 
						paintRoute );
			}
		}
		else{
			canvas.drawText("Not Found Route", 0, 0, paintRoute);
		}
		mImageView.setImageBitmap(bitmap);
		
		/*
		ZoomControls zoomControls = (ZoomControls)findViewById(R.id.zoomControls);

		zoomControls.setOnZoomInClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				Matrix matrix = new Matrix();
				mImageView.SetScaleFactor(2.0f);
			}
		});
		zoomControls.setOnZoomOutClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Matrix matrix = new Matrix();
				mImageView.SetScaleFactor(0.5f);
			}
		});
		*/
	}
}
