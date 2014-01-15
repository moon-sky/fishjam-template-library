package com.fishjam.storehelper;

import com.google.zxing.client.android.CaptureActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.ScrollView;

public class QRCodeActivity extends CaptureActivity {
	private ScrollView container = null;
	Button menuButton;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		super.onCreate(savedInstanceState);

		menuButton = (Button) findViewById(com.google.zxing.client.android.R.id.menu_encode);
		menuButton.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				//CommonFunction.openSwitchResult(QRCodeActivity.this);
			}
		});
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent intent) {
		// TODO Auto-generated method stub
		if (requestCode == 0) {
			if (resultCode == RESULT_OK) {
				
				//CommonFunction.dispatchSwitchResult(this, intent);
				// Handle successful scan
			} else if (resultCode == RESULT_CANCELED) {
				// Handle cancel
			}
		} else {
			return;
		}
	}

}
