package com.fishjam.storehelper;

import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.content.res.Resources.Theme;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.os.Bundle;
import android.util.AttributeSet;
import android.util.Log;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager.LayoutParams;

class ViewCallbackTesterIconInfo extends StartIconInfo{

	ViewCallbackTesterIconInfo(Activity activity) {
		super(activity);
		this.name = "ActiviyCallback";
		this.icon = R.drawable.gas_station;
	}

	@Override
	void onClick() {
		Intent intent = new Intent(mActivity, CallbackTesterActivity.class);
		mActivity.startActivity(intent);
	}
}

public class CallbackTesterActivity extends Activity {
	private final static String TAG = CallbackTesterActivity.class.getSimpleName();

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.i(TAG, "onCreate -- ");
		
		super.onCreate(savedInstanceState);
	}

	@Override
	protected void onRestoreInstanceState(Bundle savedInstanceState) {
		Log.i(TAG, "onRestoreInstanceState -- ");
		super.onRestoreInstanceState(savedInstanceState);
	}

	@Override
	protected void onPostCreate(Bundle savedInstanceState) {
		Log.i(TAG, "onPostCreate -- ");
		super.onPostCreate(savedInstanceState);
	}

	@Override
	protected void onStart() {
		Log.i(TAG, "onStart -- ");
		super.onStart();
	}

	@Override
	protected void onRestart() {
		Log.i(TAG, "onRestart -- ");
		super.onRestart();
	}

	@Override
	protected void onResume() {
		Log.i(TAG, "onResume -- ");
		super.onResume();
	}

	@Override
	protected void onPostResume() {
		Log.i(TAG, "onPostResume -- ");
		super.onPostResume();
	}

	@Override
	protected void onNewIntent(Intent intent) {
		Log.i(TAG, "onNewIntent -- ");
		super.onNewIntent(intent);
	}

	@Override
	protected void onSaveInstanceState(Bundle outState) {
		Log.i(TAG, "onSaveInstanceState -- ");
		super.onSaveInstanceState(outState);
	}

	@Override
	protected void onPause() {
		Log.i(TAG, "onPause -- ");
		super.onPause();
	}

	@Override
	protected void onUserLeaveHint() {
		Log.i(TAG, "onUserLeaveHint -- ");
		super.onUserLeaveHint();
	}

	@Override
	public boolean onCreateThumbnail(Bitmap outBitmap, Canvas canvas) {
		Log.i(TAG, "onCreateThumbnail -- ");
		return super.onCreateThumbnail(outBitmap, canvas);
	}

	@Override
	public CharSequence onCreateDescription() {
		Log.i(TAG, "onCreateDescription -- ");
		return super.onCreateDescription();
	}

	@Override
	protected void onStop() {
		Log.i(TAG, "onStop -- ");
		super.onStop();
	}

	@Override
	protected void onDestroy() {
		Log.i(TAG, "onDestroy -- ");
		super.onDestroy();
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		Log.i(TAG, "onConfigurationChanged -- ");
		super.onConfigurationChanged(newConfig);
	}

	@Override
	public Object onRetainNonConfigurationInstance() {
		Log.i(TAG, "onRetainNonConfigurationInstance -- ");
		return super.onRetainNonConfigurationInstance();
	}

	@Override
	public void onLowMemory() {
		Log.i(TAG, "onLowMemory -- ");
		super.onLowMemory();
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		Log.i(TAG, "onKeyDown -- ");
		return super.onKeyDown(keyCode, event);
	}

	@Override
	public boolean onKeyLongPress(int keyCode, KeyEvent event) {
		Log.i(TAG, "onKeyLongPress -- ");
		return super.onKeyLongPress(keyCode, event);
	}

	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		Log.i(TAG, "onKeyUp -- ");
		return super.onKeyUp(keyCode, event);
	}

	@Override
	public boolean onKeyMultiple(int keyCode, int repeatCount, KeyEvent event) {
		Log.i(TAG, "onKeyMultiple -- ");
		return super.onKeyMultiple(keyCode, repeatCount, event);
	}

	@Override
	public void onBackPressed() {
		Log.i(TAG, "onBackPressed -- ");
		super.onBackPressed();
	}

	@Override
	public boolean onTrackballEvent(MotionEvent event) {
		Log.i(TAG, "onTrackballEvent -- ");
		return super.onTrackballEvent(event);
	}

	@Override
	public void onUserInteraction() {
		Log.i(TAG, "onUserInteraction -- ");
		super.onUserInteraction();
	}

	@Override
	public void onWindowAttributesChanged(LayoutParams params) {
		Log.i(TAG, "onWindowAttributesChanged -- ");
		super.onWindowAttributesChanged(params);
	}

	@Override
	public void onContentChanged() {
		Log.i(TAG, "onContentChanged -- ");
		super.onContentChanged();
	}

	@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		Log.i(TAG, "onWindowFocusChanged, hasFocus=" + hasFocus);
		super.onWindowFocusChanged(hasFocus);
	}

	@Override
	public void onAttachedToWindow() {
		Log.i(TAG, "onAttachedToWindow -- ");
		super.onAttachedToWindow();
	}

	@Override
	public void onDetachedFromWindow() {
		Log.i(TAG, "onDetachedFromWindow -- ");
		super.onDetachedFromWindow();
	}

	@Override
	public View onCreatePanelView(int featureId) {
		Log.i(TAG, "onCreatePanelView -- ");
		return super.onCreatePanelView(featureId);
	}

	@Override
	public boolean onCreatePanelMenu(int featureId, Menu menu) {
		Log.i(TAG, "onCreatePanelMenu -- ");
		return super.onCreatePanelMenu(featureId, menu);
	}

	@Override
	public boolean onPreparePanel(int featureId, View view, Menu menu) {
		Log.i(TAG, "onPreparePanel -- ");
		return super.onPreparePanel(featureId, view, menu);
	}

	@Override
	public boolean onMenuOpened(int featureId, Menu menu) {
		Log.i(TAG, "onMenuOpened -- ");
		return super.onMenuOpened(featureId, menu);
	}

	@Override
	public boolean onMenuItemSelected(int featureId, MenuItem item) {
		Log.i(TAG, "onMenuItemSelected -- ");
		return super.onMenuItemSelected(featureId, item);
	}

	@Override
	public void onPanelClosed(int featureId, Menu menu) {
		Log.i(TAG, "onPanelClosed -- ");
		super.onPanelClosed(featureId, menu);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		Log.i(TAG, "onCreateOptionsMenu -- ");
		return super.onCreateOptionsMenu(menu);
	}

	@Override
	public boolean onPrepareOptionsMenu(Menu menu) {
		Log.i(TAG, "onPrepareOptionsMenu -- ");
		return super.onPrepareOptionsMenu(menu);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		Log.i(TAG, "onOptionsItemSelected -- ");
		return super.onOptionsItemSelected(item);
	}

	@Override
	public void onOptionsMenuClosed(Menu menu) {
		Log.i(TAG, "onOptionsMenuClosed -- ");
		super.onOptionsMenuClosed(menu);
	}

	@Override
	public void onCreateContextMenu(ContextMenu menu, View v,
			ContextMenuInfo menuInfo) {
		Log.i(TAG, "onCreateContextMenu -- ");
		super.onCreateContextMenu(menu, v, menuInfo);
	}

	@Override
	public boolean onContextItemSelected(MenuItem item) {
		Log.i(TAG, "onContextItemSelected -- ");
		return super.onContextItemSelected(item);
	}

	@Override
	public void onContextMenuClosed(Menu menu) {
		Log.i(TAG, "onContextMenuClosed -- ");
		super.onContextMenuClosed(menu);
	}

	@Override
	protected Dialog onCreateDialog(int id) {
		Log.i(TAG, "onCreateDialog -- ");
		return super.onCreateDialog(id);
	}

	@Override
	protected Dialog onCreateDialog(int id, Bundle args) {
		Log.i(TAG, "onCreateDialog -- ");
		return super.onCreateDialog(id, args);
	}

	@Override
	protected void onPrepareDialog(int id, Dialog dialog) {
		Log.i(TAG, "onPrepareDialog -- ");
		super.onPrepareDialog(id, dialog);
	}

	@Override
	protected void onPrepareDialog(int id, Dialog dialog, Bundle args) {
		Log.i(TAG, "onPrepareDialog -- ");
		super.onPrepareDialog(id, dialog, args);
	}

	@Override
	public boolean onSearchRequested() {
		Log.i(TAG, "onSearchRequested -- ");
		return super.onSearchRequested();
	}

	@Override
	protected void onApplyThemeResource(Theme theme, int resid, boolean first) {
		Log.i(TAG, "onApplyThemeResource -- ");
		super.onApplyThemeResource(theme, resid, first);
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		Log.i(TAG, "onActivityResult -- ");
		super.onActivityResult(requestCode, resultCode, data);
	}

	@Override
	protected void onTitleChanged(CharSequence title, int color) {
		Log.i(TAG, "onTitleChanged -- ");
		super.onTitleChanged(title, color);
	}

	@Override
	protected void onChildTitleChanged(Activity childActivity,
			CharSequence title) {
		Log.i(TAG, "onChildTitleChanged -- ");
		super.onChildTitleChanged(childActivity, title);
	}

	@Override
	public View onCreateView(String name, Context context, AttributeSet attrs) {
		Log.i(TAG, "onCreateView -- ");
		return super.onCreateView(name, context, attrs);
	}

}
