package com.fishjam.storehelper;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.ImageView.ScaleType;

public class PositionActivity extends Activity{
	static final String KEY_CUR_POSITION = "curPosition";
	static final String KEY_FLOOR = "Floor";
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		
		super.onCreate(savedInstanceState);
		
		PositionView positionView = new PositionView(this);
		positionView.setScaleType(ScaleType.CENTER);
		setContentView(positionView);
		Intent intent = getIntent();
		int [] curPosition = intent.getExtras().getIntArray(KEY_CUR_POSITION);
		int nFloor = intent.getExtras().getInt(KEY_FLOOR);
		if (curPosition != null) {
			PositionInfo posInfo = new PositionInfo();
			posInfo.nFloor = nFloor;
			posInfo.ptPosition.set(curPosition[0], curPosition[1]);
			positionView.SetPosition(posInfo);
		}
		
	}

}
