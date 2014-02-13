package com.fishjam.util;

import android.content.Context;
import android.graphics.Matrix;
import android.graphics.PointF;
import android.support.v4.view.GestureDetectorCompat;
import android.util.AttributeSet;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
import android.view.View;
import android.widget.ImageView;

public class GestureImageView extends ImageView {
	private final String TAG =GestureImageView.class.getSimpleName();
	
	float mCurScaleFactor = 1.0f;
	float mLastScaleFactor = 1.0f;
	
	PointF mCurTranslate = new PointF(0.0f, 0.0f);
	PointF mLastTranslate = new PointF(0.0f, 0.0f);
	
	float mMinScaleFactor = 0.5f;
	float mMaxScaleFactor = 2.0f;
	
	Matrix mMyMatrix = new Matrix();
	// GestureDetector mDetector;
	ScaleGestureDetector mScaleGestureDetector;
	GestureDetectorCompat mGestureDetector;

	public GestureImageView(Context context) {
		this(context, null, 0);
	}

	public GestureImageView(Context context, AttributeSet attrs) {
		this(context, attrs, 0);
	}
	
	public GestureImageView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);

		setScaleType(ScaleType.MATRIX);

		mScaleGestureDetector = new ScaleGestureDetector(getContext(), mScaleGestureListener);
		mGestureDetector = new GestureDetectorCompat(getContext(), mGestureListener);

		setOnTouchListener(mTouchListener);
	}

	public void SetMinMaxScaleFactor(float minFactor, float maxFactor){
		mMinScaleFactor = minFactor;
		mMaxScaleFactor = maxFactor;
	}
	public void ResetImageMatrix()
	{
		mCurScaleFactor = 1.0f;
		mLastScaleFactor = 1.0f;
		mLastTranslate.set(0.0f, 0.0f);
		mCurTranslate.set(0.0f, 0.0f);
	}
	public boolean onImageFling(MotionEvent e1, MotionEvent e2, float velocityX,
			float velocityY) {
		return false;
	}
	
	void calcImageMatrix(){
		float totalScaleFactor = mCurScaleFactor * mLastScaleFactor; 
		
		if (totalScaleFactor > mMaxScaleFactor) {
			totalScaleFactor = mMaxScaleFactor;
		}else if (totalScaleFactor < mMinScaleFactor) {
			totalScaleFactor = mMinScaleFactor;
		}

		float totalTranslateX = mLastTranslate.x + mCurTranslate.x;
		float totalTranslateY = mLastTranslate.y + mCurTranslate.y;
		
		Log.i(TAG, "SetScaleFactor, mCurScaleFactor=" + mCurScaleFactor + ",lastFactor=" + mLastScaleFactor + ",total=" + totalScaleFactor
				+ ", Translate=[" + LogHelper.FormatPointF(mLastTranslate) + "]+[" + LogHelper.FormatPointF(mCurTranslate)
				+ "="+ totalTranslateX +"," + totalTranslateY);
		
		mMyMatrix.reset();

		mMyMatrix.postTranslate(totalTranslateX, totalTranslateY);
		mMyMatrix.preScale(totalScaleFactor, totalScaleFactor);
		
		//Log.i(TAG, "ImageMaxtrix=" + LogHelper.FormatMatrix(mMyMatrix));
		setImageMatrix(mMyMatrix);
	}

	private final View.OnTouchListener mTouchListener = new OnTouchListener() {

		@Override
		public boolean onTouch(View v, MotionEvent event) {
			boolean bResult = false;
			bResult = mScaleGestureDetector.onTouchEvent(event);
			//Log.i(TAG, "onTouch: " + LogHelper.FormatMotionEvent(event) +",Scale return=" + bResult);
			
			//if (!bResult) {
				bResult = mGestureDetector.onTouchEvent(event) | bResult;
			//}
			return bResult;
		}
	};

	private final ScaleGestureDetector.OnScaleGestureListener mScaleGestureListener = new ScaleGestureDetector.SimpleOnScaleGestureListener() {

		@Override
		public boolean onScale(ScaleGestureDetector detector) {
			float curScaleFactor = detector.getScaleFactor();
			Log.i(TAG, "onScale, Span=" + detector.getCurrentSpan() + ", Factor=" + curScaleFactor);
			//if ((curScaleFactor / mLastFactor) < 0.9f || (mLastFactor / curScaleFactor) < 0.9f) 
			{
				mCurScaleFactor = curScaleFactor;
				calcImageMatrix();
			}
			return false;
		}

		@Override
		public boolean onScaleBegin(ScaleGestureDetector detector) {
			float curScaleFactor = detector.getScaleFactor();
			Log.i(TAG, "onScaleBegin, Focus=" + detector.getFocusX() + "x" + detector.getFocusY() 
					+ ", curScaleFactor="+curScaleFactor + ", mLastFactor=" + mLastScaleFactor);
			//if ((curScaleFactor / mLastFactor) < 0.9f || (mLastFactor / curScaleFactor) < 0.9f){
				return true;
			//}
			//return false;
		}

		@Override
		public void onScaleEnd(ScaleGestureDetector detector) {
			mLastScaleFactor *= detector.getScaleFactor();;
			Log.i(TAG, "onScaleEnd, Factor=" + detector.getScaleFactor() + ",LastScale=" + mLastScaleFactor);
			super.onScaleEnd(detector);
		}

	};

	private final GestureDetector.SimpleOnGestureListener mGestureListener = new GestureDetector.SimpleOnGestureListener() {

		@Override
		public boolean onSingleTapUp(MotionEvent e) {
			Log.i(TAG, "onSingleTapUp, Pos=" + e.getX() + "x" + e.getY());
			return true; //super.onSingleTapUp(e);
		}

		@Override
		public void onLongPress(MotionEvent e) {
			Log.i(TAG, "onLongPress, Pos=" + e.getX() + "x" + e.getY());
			super.onLongPress(e);
		}

		@Override
		public boolean onScroll(MotionEvent e1, MotionEvent e2,
				float distanceX, float distanceY) {
			mCurTranslate.x = e2.getX() - e1.getX();
			mCurTranslate.y = e2.getY() - e1.getY();
			//mCurScaleFactor = 1.0f;
			
			Log.i(TAG, "onScroll, e1=" + LogHelper.FormatMotionEvent(e1) + ",e2=" + LogHelper.FormatMotionEvent(e2)
					+ ", distanceXY=" + distanceX + "," + distanceY
					+ ", CurTranslate=" + LogHelper.FormatPointF(mCurTranslate));

			calcImageMatrix();
			
			return true; //super.onScroll(e1, e2, distanceX, distanceY);
		}

		@Override
		public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX,
				float velocityY) {
			boolean bRet = GestureImageView.this.onImageFling(e1, e2, velocityX, velocityY);
			if (bRet) {
				return bRet;
			}
			Log.i(TAG, "onFling, Pos=" + e1.getX() + "x" + e1.getY() + ", velocityXY=" + velocityX + "," + velocityY);
			return true;//super.onFling(e1, e2, velocityX, velocityY);
		}

		@Override
		public void onShowPress(MotionEvent e) {
			Log.i(TAG, "onShowPress, Pos=" + e.getX() + "x" + e.getY());
			super.onShowPress(e);
		}

		@Override
		public boolean onDown(MotionEvent e) {
			Log.i(TAG, "onDown, Pos=" + e.getX() + "x" + e.getY());
			mCurScaleFactor = 1.0f;
			mLastTranslate = mCurTranslate;
			mCurTranslate.set(0.0f, 0.0f);
			return true; //super.onDown(e);
		}

		@Override
		public boolean onDoubleTap(MotionEvent e) {
			Log.i(TAG, "onDoubleTap, Pos=" + e.getX() + "x" + e.getY());
			return true;
		}

		@Override
		public boolean onDoubleTapEvent(MotionEvent e) {
			Log.i(TAG, "onDoubleTapEvent, Pos=" + e.getX() + "x" + e.getY());
			mCurScaleFactor = mLastScaleFactor = 1.0f;
			mCurTranslate.set(0.0f, 0.0f);
			mLastTranslate.set(0.0f, 0.0f);
			calcImageMatrix();
			return true;
			//return super.onDoubleTapEvent(e);
		}

		@Override
		public boolean onSingleTapConfirmed(MotionEvent e) {
			Log.i(TAG, "onSingleTapConfirmed, Pos=" + e.getX() + "x" + e.getY());
			return true;
		}

	};

}
