package com.fishjam.storehelper;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.fishjam.storehelper.zxing.IntentIntegrator;
import com.fishjam.storehelper.zxing.IntentResult;
import com.fishjam.util.CrashHandler;

import android.R.anim;
import android.R.integer;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.GridView;
import android.widget.SimpleAdapter;
import android.widget.Toast;

public class MainActivityView extends Activity {
	private static final String TAG = MainActivityView.class.getName();
	
	StartIconInfo[] mStartIconInfos;
	GridView mViewStartIcons;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main_view);
		CrashHandler crashHandler = CrashHandler.getInstance();
		crashHandler.init(this);
		//crashHandler.init(getApplicationContext()); 
		
		mStartIconInfos = new StartIconInfo[]{
			new FindCarIconItem(this),
			new CameraParamIconItem(this),
			new ViewFloorImage(this),
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
				mStartIconInfos[position].onClick();
			}
		});

		SimpleAdapter adapterStartIcons = new SimpleAdapter(this, listItems, R.layout.start_icon, 
				new String[] { "Icon", "Name"}, 
				new int[] { R.id.imageView1, R.id.textView1 } 
		);
		mViewStartIcons.setAdapter(adapterStartIcons);
		
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		
		 IntentResult scanResult = IntentIntegrator.parseActivityResult(requestCode, resultCode, data);
		 if (scanResult != null) {
				//Bundle bundle = data.getExtras();
				//String scanResult = bundle.getString("result");
				Intent intent = new Intent(MainActivityView.this, PositionActivity.class);
				intent.putExtra(PositionActivity.KEY_CUR_POSITION, new int[] { 150, 200});
				int nFloor = 0;
				intent.putExtra(PositionActivity.KEY_FLOOR,  nFloor);
				startActivity(intent);
				
			//mTextView.setText(scanResult.toString());
		}
		//super.onActivityResult(requestCode, resultCode, data);
		
		 /*
		if (resultCode == RESULT_OK) {
			Bundle bundle = data.getExtras();
			String scanResult = bundle.getString("result");
			
			
			Intent intent = new Intent(MainActivityView.this, PositionActivity.class);
			intent.putExtra(PositionActivity.KEY_CUR_POSITION, new int[] { 150, 200});
			int nFloor = 0;
			intent.putExtra(PositionActivity.KEY_FLOOR,  nFloor);
			startActivity(intent);

			Toast.makeText(this,  scanResult, Toast.LENGTH_LONG).show();
		}
	   */
	}
}
