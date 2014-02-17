package com.fishjam.storehelper;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;

import com.fishjam.storehelper.FloorMapManager.MapData;
import com.fishjam.util.AStar;
import com.fishjam.util.DragImageView;
import com.fishjam.util.GestureImageView;
import com.fishjam.util.LogHelper;

public class PositionView extends  DragImageView{ // GestureImageView {
	private final static String TAG = PositionView.class.getSimpleName();
	
	Paint mPaint = new Paint();
	int    mCurFloor = -1;
	AStar mAStar;
	AStar.PosInfo[] mAStarRoute;
	Bitmap mUserBitmap;
	Bitmap mCarBitmap;
	
	PositionInfo mUserPositionInfo;
	PositionInfo mCarPositionInfo;
	
	int [] FloorNumber = {
			R.raw.floor_1,
			R.raw.floor_2
	};
	
	public PositionView(Context context) {
		super(context);
		init();
	}
	public PositionView(Context context, AttributeSet attrs) {
		super(context, attrs);
		init();
	}
	
	void init(){
		
		final Resources resources = getResources();
		mUserBitmap = BitmapFactory.decodeStream(resources.openRawResource(R.drawable.ic_launcher));
		mCarBitmap = BitmapFactory.decodeStream(resources.openRawResource(R.drawable.car_logo_benz));
	}

	public void setFloor(int iFloor) {
		Log.i(TAG, "setFloor," + mCurFloor + " to " + iFloor);
		if (mCurFloor != iFloor) {
			mCurFloor = iFloor;
			
			mUserPositionInfo = StoreInformation.Instance().mUserPositionInfo;
			mCarPositionInfo = StoreInformation.Instance().mCarPositionInfo;
			Log.i(TAG, "onDraw, UserPos=" + mUserPositionInfo.toString() + ",CarPos=" + mCarPositionInfo.toString());

			Resources resources = getResources();
			Bitmap bitmap = BitmapFactory.decodeStream(resources.openRawResource(FloorNumber[mCurFloor])).copy(Bitmap.Config.RGB_565, true);

			Log.i(TAG, "Bitmap Size=" + bitmap.getWidth() + "," + bitmap.getHeight());
			Canvas canvas = new Canvas(bitmap);
			
			if(mCurFloor == mUserPositionInfo.iFloor){
				Rect srcRect = new Rect(0, 0, mUserBitmap.getWidth(), mUserBitmap.getHeight());
				Rect dstRect = new Rect(
						(mUserPositionInfo.ptPos.y - 2) * GlobalConfig.TILE_WIDTH, 
						(mUserPositionInfo.ptPos.x - 2) * GlobalConfig.TILE_HEIGHT, 
						(mUserPositionInfo.ptPos.y + 2) * GlobalConfig.TILE_WIDTH, 
						(mUserPositionInfo.ptPos.x + 2) * GlobalConfig.TILE_HEIGHT);
				Log.i(TAG, "DrawUser, " + LogHelper.FormatRect(srcRect) + " to " + LogHelper.FormatRect(dstRect));
				canvas.drawBitmap(mUserBitmap, srcRect, dstRect, null);
			}
			
			if(mCurFloor == mCarPositionInfo.iFloor){
				Rect srcRect = new Rect(0, 0, mCarBitmap.getWidth(), mCarBitmap.getHeight());
				Rect dstRect = new Rect(
						(mCarPositionInfo.ptPos.y - 2) * GlobalConfig.TILE_WIDTH, 
						(mCarPositionInfo.ptPos.x - 2) * GlobalConfig.TILE_HEIGHT, 
						(mCarPositionInfo.ptPos.y + 2) * GlobalConfig.TILE_WIDTH, 
						(mCarPositionInfo.ptPos.x + 2) * GlobalConfig.TILE_HEIGHT);
				Log.i(TAG, "DrawCar, " + LogHelper.FormatRect(srcRect) + " to " + LogHelper.FormatRect(dstRect));
				canvas.drawBitmap(mCarBitmap, srcRect, dstRect, null);
			}
			
			Paint paintRoute = new Paint();
			paintRoute.setColor(Color.RED);
		
			//MapData maData = FloorMapManager.getInstance().getMapByFloor(iFloor);
			//AStar aStar = new AStar(maData.mTileData, mUserPositionInfo.ptPos.x, mUserPositionInfo.ptPos.y, 
			//		mCarPositionInfo.ptPos.x, mCarPositionInfo.ptPos.y, true);
					//6, 32, 30, 48, false);
			//AStar.PosInfo[] posInfos = aStar.getResult(true);
			AStar.PosInfo[] posInfos = StoreInformation.Instance().findWayToCar(mCurFloor);
			if (posInfos != null) {
				Log.i(TAG, "findWayToCar, length=" + posInfos.length);
				int nStartLine = 0;
				int nStartCol = 0;
				//StringBuilder stringBuilder = new StringBuilder();
				
				for (int i = posInfos.length - 1; i >=0 ; i--) {
					nStartLine = (posInfos[i].col + 0) * GlobalConfig.TILE_WIDTH;
					nStartCol = (posInfos[i].row + 0) * GlobalConfig.TILE_HEIGHT;
					//stringBuilder.append("=>[" + posInfos[i].row + "," + posInfos[i].col + "," + 
					//		aStar.getDirString(posInfos[i].dir)  +  "]");
					canvas.drawRect(nStartLine + GlobalConfig.TILE_WIDTH / 4 , nStartCol + GlobalConfig.TILE_HEIGHT / 4 ,
							nStartLine + GlobalConfig.TILE_WIDTH  * 3 / 4,  nStartCol + GlobalConfig.TILE_HEIGHT * 3 /4, 
							paintRoute );
				}
			}
			else{
				canvas.drawText("Not Found Route", 0, 0, paintRoute);
			}
			setImageBitmap(bitmap);
			

			
			//setmActivity((Activity)getContext());
			//dragImageView.setImageBitmap(bmp);
			//dragImageView.setmActivity(this);//×¢ÈëActivity.
			
			//ResetImageMatrix();
			postInvalidate();
		}
	}


	/*
	@Override
	public boolean onImageFling(MotionEvent e1, MotionEvent e2, float velocityX,
			float velocityY) {
		Log.i(TAG, "onImageFling, e1=" + LogHelper.FormatMotionEvent(e1) 
				+ ",e2=" + LogHelper.FormatMotionEvent(e2)
				+ ",velocityX=" + velocityX + ",velocityY=" + velocityY);
		
		if (Math.abs(velocityX) > Math.abs(velocityY)) {
			onFlipingX(velocityX);
		}
		else{
			onFlipingY(velocityY);
		}
		return true;
	}
	private void onFlipingY(float velocityY) {
		
	}
	private void onFlipingX(float velocityX) {
		if (Math.abs(velocityX) > 2000) {
			setFloor(mCurFloor == 0 ? 1 : 0);
		}
	}
	*/
	
}
