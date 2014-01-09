package com.fishjam.storehelper;

import android.app.Activity;
import android.os.Bundle;
import android.widget.ImageView;

public class ImageActivity extends Activity {
	ImageView imageView;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_image);
		
		imageView = (ImageView)findViewById(R.id.imageView);
		imageView.setImageResource(R.drawable.ic_launcher);
	}

}
