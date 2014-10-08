package com.fishjam.android.study;

import junit.framework.Assert;
import android.test.AndroidTestCase;


/**************************************************************************************************************************************
* Android源码 -- https://android.googlesource.com/
* 
*   Launcher -- HomeScreen, 启动Android手机，第一眼看到的应用程序。<category android:name="android.intent.category.HOME"/> 
**************************************************************************************************************************************/

public class AndroidSrcCodeTest  extends AndroidTestCase{
	public void testSave() throws Throwable
	{
		int i=4+8;
		Assert.assertEquals(12,i);
	}
}