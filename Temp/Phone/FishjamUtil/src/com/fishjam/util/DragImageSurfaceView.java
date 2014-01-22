package com.fishjam.util;

import java.util.Date;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.renderscript.RenderScript.ContextType;
import android.text.format.Time;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ScaleGestureDetector;


public class DragImageSurfaceView extends SurfaceView implements
		SurfaceHolder.Callback, ScaleGestureDetector.OnScaleGestureListener {
	
	private static final String TAG = DragImageSurfaceView.class.getSimpleName();
	private Paint mPaint;
	
	
	private ScaleGestureDetector mScaleGestureDetector;
	public DragImageSurfaceView(Context context) {
		super(context);
		
		mPaint = new Paint();
		mPaint.setColor(Color.BLUE);
		mScaleGestureDetector = new ScaleGestureDetector(context, this);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		Log.i(TAG, "-----onTouchEvent, action=" + event.getAction() + ", Pos=" + event.getX() + "," + event.getY());
		
		switch (event.getAction()) {
		case MotionEvent.ACTION_DOWN:
			drawOnSurfaceHolder();
			break;
		default:
			break;
		}

		return mScaleGestureDetector.onTouchEvent(event);
		//return super.onTouchEvent(event);
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		// TODO Auto-generated method stub
		Log.i(TAG, "surfaceCreated");
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
		Log.i(TAG, "surfaceChanged, format=" + format +",width=" + width + ", height=" + height);
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		// TODO Auto-generated method stub
		Log.i(TAG, "surfaceDestroyed");

	}

	@Override
	public boolean onScale(ScaleGestureDetector detector) {
		Log.i(TAG, "onScale, factor=" + detector.getScaleFactor());
		
		return false;
	}

	@Override
	public boolean onScaleBegin(ScaleGestureDetector detector) {
		Log.i(TAG, "onScaleBegin");
		return true;
	}

	@Override
	public void onScaleEnd(ScaleGestureDetector detector) {
		Log.i(TAG, "onScaleEnd");
	}

	private void drawOnSurfaceHolder(){
		SurfaceHolder holder = getHolder();
		Canvas canvas = holder.lockCanvas();
		if(canvas != null){
			Time time = new Time();
			time.setToNow(); 
			canvas.drawColor(getContext().getResources().getColor(android.R.color.holo_red_light));
			//canvas.drawText(time.format("%H%M%S"), 0, 0, mPaint);
			canvas.drawText("fishjam test", 10, 10, mPaint);
			holder.unlockCanvasAndPost(canvas);
			
			canvas = holder.lockCanvas(new Rect(0, 0, 0, 0));
			holder.unlockCanvasAndPost(canvas);
		}
	}
}
