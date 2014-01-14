package com.fishjam.storehelper;

import com.fishjam.storehelper.zxing.IntentIntegrator;
import com.fishjam.storehelper.zxing.IntentResult;

import android.os.Bundle;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {
	private TextView mTextView;
	private Button btnView;
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent intent) {
		 IntentResult scanResult = IntentIntegrator.parseActivityResult(requestCode, resultCode, intent);
		 if (scanResult != null) {
			mTextView.setText(scanResult.toString());
		}
		super.onActivityResult(requestCode, resultCode, intent);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		mTextView = (TextView)findViewById(R.id.textView1);
		mTextView.setText("Wait For Result");
		Button btn1 = (Button)findViewById(R.id.button1);
		btn1.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				IntentIntegrator intentInte = new IntentIntegrator(MainActivity.this);
				intentInte.initiateScan();
			}
		});

		btnView = (Button)findViewById(R.id.btnViewImage);
		
		btnView.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(MainActivity.this, ImageActivity.class);
				startActivity(intent);
			}
		});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
