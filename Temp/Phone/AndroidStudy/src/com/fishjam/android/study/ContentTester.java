package com.fishjam.android.study;
import android.app.ListActivity;
import android.content.ContentResolver;
import android.database.Cursor;
import android.provider.Contacts.People;
import android.widget.Adapter;
import android.widget.BaseAdapter;
import android.widget.ListAdapter;
import android.widget.SimpleCursorAdapter;
import junit.framework.TestCase;

/***************************************************************************************************************************************
 * 数据提供者(ContentProvider) -- 允许一个应用程序访问其他应用程序（比如通信录）的数据，或者共享它自己的数据
 *   ContentResolver -- 数据提供折? 返回Cursor对象 ? 
 *   
 * Adapter -- 适配器接口
 *   BaseAdapter
 *   ArrayAdapter
 *   SimpleCursorAdapter 
 *
 **************************************************************************************************************************************/

public class ContentTester  extends TestCase{
	public void ListActivityCursorTester(){
		/*********************************************************************************************************
		//class MyActivity extends ListActivity
		
		Cursor cursor = getContentResolver().query(People.CONTENT_URI, null, null, null, null);
		startManagingCursor(c);
		ListAdapter adapter = new SimpleCursorAdapter(this,  android.R.layout.simple_list_item_1,  c,  
				new String[] { People.NAME },  new int[] {android.R.id.text1} );
		setListAdapter(adapter);
		*********************************************************************************************************/
	}
}
