package com.fishjam.util;

import android.util.Log;
import android.view.View;
import android.view.ViewTreeObserver;

/***************************************************************************************************************************************
 * View变化监听器
 * 
 ***************************************************************************************************************************************/

public class DefaultViewTreeObserverHanlder implements
		ViewTreeObserver.OnGlobalFocusChangeListener,	//用于监听焦点的变化 
		ViewTreeObserver.OnGlobalLayoutListener,				//用于监听布局之类的变化，比如某个控件隐藏了
		ViewTreeObserver.OnPreDrawListener,					//用于在屏幕上画 View 之前，要做什么额外的工作，比如 setHint 等?
		ViewTreeObserver.OnScrollChangedListener,
		ViewTreeObserver.OnTouchModeChangeListener		//用于监听 Touch 和非 Touch 模式的转换  
{	
	private final static String TAG = DefaultViewTreeObserverHanlder.class.getSimpleName();

	private View mView;

	public DefaultViewTreeObserverHanlder(View view) {
		mView = view;
	}

	public void setupListener() {
		final ViewTreeObserver viewTreeObserver = mView.getViewTreeObserver();
		if (viewTreeObserver != null) {
			viewTreeObserver.addOnGlobalLayoutListener(this);
			viewTreeObserver.addOnGlobalFocusChangeListener(this);
			viewTreeObserver.addOnPreDrawListener(this);
			viewTreeObserver.addOnScrollChangedListener(this);
			viewTreeObserver.addOnTouchModeChangeListener(this);
		}
	}

	public void tearDownListener() {
		final ViewTreeObserver viewTreeObserver = mView.getViewTreeObserver();
		if (viewTreeObserver != null) {
			viewTreeObserver.removeGlobalOnLayoutListener(this);
			viewTreeObserver.removeOnGlobalFocusChangeListener(this);
			viewTreeObserver.removeOnPreDrawListener(this);
			viewTreeObserver.removeOnScrollChangedListener(this);
			viewTreeObserver.removeOnTouchModeChangeListener(this);
		}
	}

	@Override
	public void onGlobalFocusChanged(View oldFocus, View newFocus) {
		Log.i(TAG, "onGlobalFocusChanged, from " + oldFocus.toString() + " to " + newFocus.toString());
	}

	@Override
	public void onTouchModeChanged(boolean isInTouchMode) {
		Log.i(TAG, "onTouchModeChanged, isInTouchMode=" + isInTouchMode);

	}

	@Override
	public void onScrollChanged() {
		Log.i(TAG, "onScrollChanged");

	}

	@Override
	public boolean onPreDraw() {
		Log.i(TAG, "onPreDraw");
		
		return true;		//true 表示继续绘制， false 表示取消(会造成界面显示不完整)
	}

	@Override
	public void onGlobalLayout() {
		Log.i(TAG, "onGlobalLayout");
	}
}
