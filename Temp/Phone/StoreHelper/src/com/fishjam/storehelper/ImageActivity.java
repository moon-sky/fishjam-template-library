package com.fishjam.storehelper;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Matrix;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.ImageView;


class ImageActivityIconInfo extends StartIconInfo{

	ImageActivityIconInfo(Activity activity) {
		super(activity);
		this.name = "TestImage";
		this.icon = R.drawable.bus_place;
	}

	@Override
	void onClick() {
		Intent intent = new Intent(mActivity, ImageActivity.class);
		mActivity.startActivity(intent);
	}
}

public class ImageActivity extends Activity {
	final static String TAG = ImageActivity.class.getSimpleName();
	
	ImageView mImageView;
	Matrix mMatrix;
	float mScale = 1.0f;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {

		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_image);
		
		mImageView = (ImageView)findViewById(R.id.imageView);
		mMatrix = new Matrix();
		mMatrix.setScale(mScale, mScale);
		mImageView.setScaleType(ImageView.ScaleType.MATRIX);
		mImageView.setImageMatrix(mMatrix);
		mImageView.setImageResource(R.drawable.ic_launcher);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		new MenuInflater(this).inflate(R.menu.test_image, menu);		
		return super.onCreateOptionsMenu(menu);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.itemZoomIn:
			mScale /= 2.0f;
			break;
		case R.id.itemZoomOut:
			mScale *= 2.0f;
			break;
		default:
			break;
		}

		mMatrix.reset();
		//mMatrix.setScale(mScale, mScale,160, 200);
		mMatrix.postScale(mScale, mScale);
		Log.i(TAG, "on Menu Zoom, " + mScale);
		//BitmapDrawable tmpDrawable = (BitmapDrawable)mImageView.getDrawable();
//		if(!tmpDrawable.getBitmap().isRecycled()){
//			tmpDrawable.getBitmap().recycle();
//		}
		
		mImageView.setImageMatrix(mMatrix);
		//mImageView.invalidate();
		//mImageView.setLayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT);
		//mImageView.postInvalidate();

		return super.onOptionsItemSelected(item);
	}

}
