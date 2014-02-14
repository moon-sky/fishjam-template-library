package com.fishjam.storehelper;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.MotionEvent;

import com.fishjam.util.AStar;
import com.fishjam.util.DragImageView;

public class PositionView extends DragImageView {
	private final static String TAG = PositionView.class.getSimpleName();
	
	Paint mPaint = new Paint();
	int    mCurFloor = -1;
	AStar mAStar;
	
	//StoreInformation	mStoreInformation;
	
	int [] FloorNumber = {
			R.raw.floor_1,
			R.raw.floor_2
	};
	
	public PositionView(Context context) {
		super(context);
		//setAdjustViewBounds(true);
	}
	public PositionView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub
		return super.onTouchEvent(event);
	}

	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		StoreInformation storeInformation = StoreInformation.Instance();
		Point ptCurPos = storeInformation.mCurPositionInfo.ptPos;
		Point ptCarPos = storeInformation.mCarPositionInfo.ptPos;

		Resources resources = getResources();
		if (ptCurPos.x >= 0 && ptCurPos.y >= 0) {
			mPaint.setColor(resources.getColor(R.color.red));
			canvas.drawCircle(ptCurPos.x, ptCurPos.y, 10, mPaint);
		}
		if (ptCarPos.x >= 0 && ptCarPos.y >= 0) {
			mPaint.setColor(resources.getColor(R.color.grgray));
			canvas.drawCircle(ptCarPos.x, ptCarPos.y, 10, mPaint);
		}
	}

	public void setFloor(int iFloor) {
		if (mCurFloor != iFloor) {
			mCurFloor = iFloor;
			
			Resources resources = getResources();
			Drawable drawFloor =  resources.getDrawable(FloorNumber[mCurFloor]);
			setImageDrawable(drawFloor);
			
			setmActivity((Activity)getContext());
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
