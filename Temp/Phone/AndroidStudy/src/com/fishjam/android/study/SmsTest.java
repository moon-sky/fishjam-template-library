package com.fishjam.android.study;

import android.app.PendingIntent;
import android.content.Intent;
import android.telephony.SmsManager;
import android.test.ActivityUnitTestCase;
import android.test.AndroidTestCase;
import junit.framework.TestCase;

/**************************************************************************************************************************************
 * 调试时，使用模拟器只能向另一台模拟器发送短信，发送时的 "destinationAddress" 是模拟器窗口标题上的号码。
 *    
 *
 **************************************************************************************************************************************/

public class SmsTest extends AndroidTestCase {

	public void testSendSms(){
		SmsManager smsManager = SmsManager.getDefault();
		PendingIntent sentIntent = PendingIntent.getBroadcast(this.getContext(), 0, new Intent(), 0);
		smsManager.sendTextMessage("destinationAddress", "scAddress", "sendText", sentIntent, null);
	}
}
