package com.fishjam.storehelper;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.ImageView.ScaleType;

public class PositionActivity extends Activity{

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		
		super.onCreate(savedInstanceState);
		
		PositionView positionView = new PositionView(this);
		positionView.setScaleType(ScaleType.CENTER);
		setContentView(positionView);
		Intent intent = getIntent();
		int [] curPosition = intent.getExtras().getIntArray("curPosition");
		if (curPosition != null) {
			positionView.SetPosition(curPosition[0], curPosition[1]);
		}
		
	}

}
