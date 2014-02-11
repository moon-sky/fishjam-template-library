package com.fishjam.util;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.text.format.Time;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ScaleGestureDetector;

public class DragImageSurfaceView extends SurfaceView implements
		SurfaceHolder.Callback, ScaleGestureDetector.OnScaleGestureListener //, Runnable
{
	private static final String TAG = DragImageSurfaceView.class.getSimpleName();
	private SurfaceHolder mSurfaceHolder;
	private Canvas mCanvas;
	private Paint mPaint;
	private int mScreenW, mScreenH;
	private Time mTime;
	private ScaleGestureDetector mScaleGestureDetector;

	public DragImageSurfaceView(Context context) {
		super(context);
		
		mPaint = new Paint();
		mPaint.setColor(Color.BLUE);
		mPaint.setAntiAlias(true);
		
		mSurfaceHolder = this.getHolder();
		mSurfaceHolder.addCallback(this);
		mScaleGestureDetector = new ScaleGestureDetector(context, this);

		this.setKeepScreenOn(true); 	//保持屏幕常量
		this.setFocusable(true);		//设置焦点后才能响应 onKeyDown 等方法
		this.setFocusableInTouchMode(true);	//触屏模式获取焦点
		this.setClickable(true);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		Log.i(TAG, "-----onTouchEvent, action=" + event.getAction() + ", Pos="
				+ event.getX() + "," + event.getY());

		switch (event.getAction()) {
		case MotionEvent.ACTION_DOWN:
			drawOnSurfaceHolder();
			break;
		default:
			break;
		}

		return mScaleGestureDetector.onTouchEvent(event);
		// return super.onTouchEvent(event);
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		//在创建时激发，一般在这里调用画图的线程
		Log.i(TAG, "surfaceCreated");

		
		//构造函数时 View 尚未真正创建，那个时候得到的 Width/Height 为 0 
		mScreenW = this.getWidth();
		mScreenH = this.getHeight();
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
		Log.i(TAG, "surfaceChanged, format=" + format + ",width=" + width + ", height=" + height);
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		//销毁时激发，一般在这里将画图的线程停止、释放。 
		Log.i(TAG, "surfaceDestroyed");
	}

	@Override
	public boolean onScale(ScaleGestureDetector detector) {
		Log.i(TAG, "onScale, factor=" + detector.getScaleFactor());

		return true;
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

	protected void drawOnSurfaceHolder() {

		Canvas canvas = mSurfaceHolder.lockCanvas();
		if (canvas != null) {
			try
			{
				//canvas.save();
				
				canvas.drawColor(Color.WHITE);// getContext().getResources().getColor(android.R.color.primary_text_light));
				mTime.setToNow();
				canvas.drawText(mTime.format("%H%M%S"), 0, 0, mPaint);
				canvas.drawText("fishjam test", 30, 30, mPaint);
			}finally{
				//canvas.restore();
				mSurfaceHolder.unlockCanvasAndPost(canvas);
				
				//canvas = mSurfaceHolder.lockCanvas(new Rect(0, 0, 0, 0));
				//mSurfaceHolder.unlockCanvasAndPost(canvas);
			}
		}
	}
}
