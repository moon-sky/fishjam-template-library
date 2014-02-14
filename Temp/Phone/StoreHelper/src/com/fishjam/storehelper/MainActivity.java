package com.fishjam.storehelper;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

import com.fishjam.storehelper.zxing.IntentIntegrator;
import com.fishjam.storehelper.zxing.IntentResult;

public class MainActivity extends Activity {
	private TextView mTextView;
	private Button mbtnView;
	private Button mbtnScan;
	private Button mbtnViewFloor;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		mTextView = (TextView)findViewById(R.id.textView1);
		mTextView.setText("Wait For Result");
		
		mbtnScan = (Button)findViewById(R.id.btnScan);
		mbtnScan.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				IntentIntegrator intentInte = new IntentIntegrator(MainActivity.this);
				intentInte.initiateScan();
			}
		});

		mbtnView = (Button)findViewById(R.id.btnViewImage);
		mbtnView.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(MainActivity.this, ImageActivity.class);
				startActivity(intent);
			}
		});
		
		mbtnViewFloor = (Button)findViewById(R.id.btnViewFloor);
		mbtnViewFloor.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(MainActivity.this, PositionActivity.class);
				intent.putExtra("curPosition", new int[] { 100, 200});
				startActivity(intent);
			}
		});
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent intent) {
		 IntentResult scanResult = IntentIntegrator.parseActivityResult(requestCode, resultCode, intent);
		 if (scanResult != null) {
			mTextView.setText(scanResult.toString());
		}
		super.onActivityResult(requestCode, resultCode, intent);
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
