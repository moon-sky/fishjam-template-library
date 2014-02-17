package com.fishjam.storehelper;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.GridView;
import android.widget.SimpleAdapter;

import com.fishjam.util.CrashHandler;
import com.fishjam.util.LogHelper;

public class MainActivityView extends Activity {
	private static final String TAG = MainActivityView.class.getSimpleName();
	
	private static final int MENU_ITEM_ID_ENABLE_CRASH_HANDLER = Menu.FIRST + 1;
	private static final int MENU_ITEM_ID_EXIT =  Menu.FIRST + 2;
	
	
	StartIconInfo[] mStartIconInfos;
	GridView mViewStartIcons;
	SharedPreferences mSharedPreferences;
	boolean mbEnableCrashHandler;
	int mCurSelIconIndex = -1;
	//StoreInformation mStoreInformation;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		Log.i("Display", LogHelper.FormatDisplay(getWindowManager().getDefaultDisplay()));
		
		mSharedPreferences = getSharedPreferences(GlobalConfig.SHARE_PREFERENCE_NAME, MODE_PRIVATE);
		mbEnableCrashHandler = mSharedPreferences.getBoolean(GlobalConfig.CONFIG_ENABLE_CRASH_HANDLER,  true);

		Log.i(TAG, "onCreate mbEnableCrashHandler=" + mbEnableCrashHandler);
		
		if (mbEnableCrashHandler) {
			//CrashHandler crashHandler = CrashHandler.getInstance();
			//crashHandler.init(this);
			//crashHandler.init(getApplicationContext()); 
		}
		setContentView(R.layout.activity_main_view);

		//≥ı ºªØ
		StoreInformation.Instance(this);
		
		mStartIconInfos = new StartIconInfo[]{
			//new SaveCarIconItem(this),
			new FindCarIconItem(this),
			new CameraParamIconItem(this),
			new ViewFloorImage(this),
			new TestGestureIconInfo(this),
			new ImageActivityIconInfo(this),
			new ViewCallbackTesterIconInfo(this),
			new AStarIconItem(this),
		};
		
		ArrayList<Map<String, Object>> listItems = new ArrayList<Map<String,Object>>();
		for (int i = 0; i < mStartIconInfos.length; i++) {
			HashMap<String, Object> listItem = new HashMap<String, Object>();
			listItem.put("Icon", mStartIconInfos[i].icon);
			listItem.put("Name", mStartIconInfos[i].name);
			listItems.add(listItem);
		}
			
		mViewStartIcons = (GridView)findViewById(R.id.view_start_icons);
		mViewStartIcons.setOnItemClickListener(new OnItemClickListener() {
			@Override
			public void onItemClick(AdapterView<?> parent, View view,
					int position, long id) {
				mCurSelIconIndex = position;
				Log.i(TAG, "onItemClick,mCurSelIconIndex=" + mCurSelIconIndex);
				mStartIconInfos[position].onExecute();
			}
		});

		SimpleAdapter adapterStartIcons = new SimpleAdapter(this, listItems, R.layout.start_icon, 
				new String[] { "Icon", "Name"}, 
				new int[] { R.id.imageView1, R.id.textView1 } 
		);
		mViewStartIcons.setAdapter(adapterStartIcons);
	}
	
	
	@Override
	protected void onPause() {
		StoreInformation.Instance(this).Save();
		super.onPause();
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		Log.i(TAG, "onActivityResult, mCurSelIconIndex=" + mCurSelIconIndex);
		if (mCurSelIconIndex != -1) {
			if(mCurSelIconIndex >= 0 && mCurSelIconIndex < mStartIconInfos.length){
				mStartIconInfos[mCurSelIconIndex].onActivityResult(requestCode, resultCode, data);
			}
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		menu.add(0,  MENU_ITEM_ID_ENABLE_CRASH_HANDLER,  0,  R.string.menu_enable_crashhandler);
		menu.add(0, MENU_ITEM_ID_EXIT, 0, R.string.menu_quit);
		return super.onCreateOptionsMenu(menu);
	}

	@Override
	public boolean onPrepareOptionsMenu(Menu menu) {
		Log.i(TAG, "Get mbEnableCrashHandler=" + mbEnableCrashHandler);
		
		MenuItem item =  menu.findItem(MENU_ITEM_ID_ENABLE_CRASH_HANDLER);
		if (item != null) {
			if (mbEnableCrashHandler) {
				item.setTitle(R.string.menu_disable_crashhandler);
			}
			else{
				item.setTitle(R.string.menu_enable_crashhandler);
			}
		}
		
		return super.onPrepareOptionsMenu(menu);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case MENU_ITEM_ID_ENABLE_CRASH_HANDLER:
			mbEnableCrashHandler = !mbEnableCrashHandler;
			mSharedPreferences.edit().putBoolean(GlobalConfig.CONFIG_ENABLE_CRASH_HANDLER, mbEnableCrashHandler).commit();
			
			Log.i(TAG, "Set mbEnableCrashHandler=" + mbEnableCrashHandler);
			break;
		case MENU_ITEM_ID_EXIT:
			finish();
			break;
		}
		return super.onOptionsItemSelected(item);
	}
	
}
