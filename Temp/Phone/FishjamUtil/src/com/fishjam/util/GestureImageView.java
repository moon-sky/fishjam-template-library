package com.fishjam.util;

import android.content.Context;
import android.graphics.Matrix;
import android.graphics.Point;
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
	
	float mMinScaleFactor = 0.25f;
	float mMaxScaleFactor = 4f;
	
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
	
	void calcImageMatrix(){
		if (mCurScaleFactor > mMaxScaleFactor) {
			mCurScaleFactor = mMaxScaleFactor;
		}else if (mCurScaleFactor < mMinScaleFactor) {
			mCurScaleFactor = mMinScaleFactor;
		}

		float totalScaleFactor = mCurScaleFactor * mLastScaleFactor; 
		Log.i(TAG, "SetScaleFactor, mCurScaleFactor=" + mCurScaleFactor + ",lastFactor=" + mLastScaleFactor + ",total=" + totalScaleFactor);

		mMyMatrix.reset();

		mMyMatrix.postTranslate(mCurTranslate.x, mCurTranslate.y);
		mMyMatrix.postScale(totalScaleFactor, totalScaleFactor);
		setImageMatrix(mMyMatrix);
	}

	private final View.OnTouchListener mTouchListener = new OnTouchListener() {

		@Override
		public boolean onTouch(View v, MotionEvent event) {
			//Log.i(TAG, "onTouch: " + LogHelper.FormatMotionEvent(event));
			boolean bResult = false;
			bResult = mScaleGestureDetector.onTouchEvent(event);
			bResult = mGestureDetector.onTouchEvent(event) | bResult;

			return bResult;
		}
	};

	private final ScaleGestureDetector.OnScaleGestureListener mScaleGestureListener = new ScaleGestureDetector.SimpleOnScaleGestureListener() {

		@Override
		public boolean onScale(ScaleGestureDetector detector) {
			float curScaleFactor = detector.getScaleFactor();
			Log.i(TAG, "onScale, Span=" + detector.getCurrentSpan() + ", Factor=" + detector.getScaleFactor());
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
			Log.i(TAG, "onScaleEnd, Factor=" + detector.getScaleFactor());

			mLastScaleFactor *= detector.getScaleFactor();;
			super.onScaleEnd(detector);
		}

	};

	private final GestureDetector.SimpleOnGestureListener mGestureListener = new GestureDetector.SimpleOnGestureListener() {

		@Override
		public boolean onSingleTapUp(MotionEvent e) {
			Log.i(TAG, "onSingleTapUp, Pos=" + e.getX() + "x" + e.getY());
			return super.onSingleTapUp(e);
		}

		@Override
		public void onLongPress(MotionEvent e) {
			Log.i(TAG, "onLongPress, Pos=" + e.getX() + "x" + e.getY());
			super.onLongPress(e);
		}

		@Override
		public boolean onScroll(MotionEvent e1, MotionEvent e2,
				float distanceX, float distanceY) {
			Log.i(TAG, "onScroll, e1=" + LogHelper.FormatMotionEvent(e1) + ",e2=" + LogHelper.FormatMotionEvent(e2)
					+ ", distanceXY=" + distanceX + "," + distanceY);
			mCurTranslate.x = e2.getX() - e1.getX();
			mCurTranslate.y = e2.getY() - e1.getY();
			calcImageMatrix();
			
			return super.onScroll(e1, e2, distanceX, distanceY);
		}

		@Override
		public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX,
				float velocityY) {
			Log.i(TAG, "onFling, Pos=" + e1.getX() + "x" + e1.getY() + ", velocityXY=" + velocityX + "," + velocityY);
			return super.onFling(e1, e2, velocityX, velocityY);
		}

		@Override
		public void onShowPress(MotionEvent e) {
			Log.i(TAG, "onShowPress, Pos=" + e.getX() + "x" + e.getY());
			super.onShowPress(e);
		}

		@Override
		public boolean onDown(MotionEvent e) {
			Log.i(TAG, "onDown, Pos=" + e.getX() + "x" + e.getY());
			return super.onDown(e);
		}

		@Override
		public boolean onDoubleTap(MotionEvent e) {
			Log.i(TAG, "onDoubleTap, Pos=" + e.getX() + "x" + e.getY());
			return super.onDoubleTap(e);
		}

		@Override
		public boolean onDoubleTapEvent(MotionEvent e) {
			Log.i(TAG, "onDoubleTapEvent, Pos=" + e.getX() + "x" + e.getY());
			return super.onDoubleTapEvent(e);
		}

		@Override
		public boolean onSingleTapConfirmed(MotionEvent e) {
			Log.i(TAG, "onSingleTapConfirmed, Pos=" + e.getX() + "x" + e.getY());
			return super.onSingleTapConfirmed(e);
		}

	};
}
