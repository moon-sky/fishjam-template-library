package com.fishjam.util;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.ScrollView;
import android.widget.TextView;

public class CrashInfoActivity extends Activity{
	public final static String CRASH_INFO_STRING = "CrashInfoString";
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(makeDisplayView());
	}
	
	View makeDisplayView(){
		Intent crashIntent =  getIntent();
		String strCrashInfo = crashIntent.getStringExtra(CRASH_INFO_STRING);			

		//RelativeLayout layout = new RelativeLayout(this);
		LinearLayout layout = new LinearLayout(this);
		layout.setOrientation(LinearLayout.VERTICAL);
		
		layout.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.FILL_PARENT,
				LinearLayout.LayoutParams.FILL_PARENT));

		TextView textView = new TextView(this);
		textView.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT,
				LinearLayout.LayoutParams.WRAP_CONTENT));
		
		textView.setText(strCrashInfo);

		ScrollView scrollView = new ScrollView(this);
		scrollView.addView(textView);
					
		Button btnClose = new Button(this);
		btnClose.setText("Close");
		btnClose.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT,
				LinearLayout.LayoutParams.WRAP_CONTENT));
		
		btnClose.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				finish();
			}
		});
		
		layout.addView(scrollView);
		layout.addView(btnClose);
		
		return layout;
	}
}