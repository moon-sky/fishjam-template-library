package com.fishjam.android.study;
import android.app.ListActivity;
import android.content.ContentResolver;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.provider.Contacts.People;
import android.widget.Adapter;
import android.widget.BaseAdapter;
import android.widget.ListAdapter;
import android.widget.SimpleCursorAdapter;
import junit.framework.TestCase;

/***************************************************************************************************************************************
* Uri
*      Uri uri = Uri.parse("content://contacts/people/1");  
* UriMatcher 
*     UriMatcher sUriMatcher = new UriMatcher(UriMatcher.NO_MATCH);
*     sUriMatcher.addURI(Employees.AUTHORITY, "employee", EMPLOYEE);
*     sUriMatcher.addURI(Employees.AUTHORITY, "employee/#", EMPLOYEE_ID);
*     switch(sUriMatcher.match(uri)){ case EMPLOYEE_ID:  String noteId= uri.getPathSegments().get(1);  };
*     
* 系统内置了大量的URI
*   Contacts.People.CONTENT_URI --
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * Android提供了多种数据存取方式：
 *   Preference -- 数据较少的配置信息等， 使用键值对的方式保存在 XML 文件中 ( /Android/data/<package>/shared_prefs/ 目录下 )
 *      SharedPreferences --  Context.getSharedPreferences(文件名，模式).edit().putString("key","Value");  修改后需要 commit();
 *   File (默认保存在 /data/<package>/files 下面)
 *     Context.openFileInput   -- 获得标准的Java文件输入流(FileInputStream)，然后循环读取 
 *     Context.openFileOutput -- 获得标准的Java文件输出流(FileOutputStream)。fos.write(content.getBytes()); fos.close();
 *     Resources.openRawResource -- 返回 InputStream
 *   SQLite (数据文件一般保存在 /data/<package>/databases/ 下，创建或打开时需要使用全路径 )
 *     SQLiteDatabase -- 数据库管理类，控制 创建、增删改 等
 *     SQLiteOpenHelper -- 辅助类，管理数据库创建和版本更新。继承后重载 onCreate/onUpgrade 等方法，可通过 getWritableDatabase/getReadableDatabase 获取对应的数据库
 *     Cursor(游标接口，提供便利查询结果的方法)
 *   网络
***************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 数据提供者(ContentProvider) -- 跨应用程序共享数据的唯一方法？ 对应的URI必须是 "content://" 开头的。
 *   Android中各个应用程序组件之间是相互独立的，彼此的数据不能共享(能访问其他程序的 DB/File 吗？)。 
 *     ContentProvider -- 检索和保存数据，允许一个应用程序访问其他应用程序（比如通信录）的数据，或者共享它自己的数据，其中包含了增删改、查询等操作
 *       如自定义时，需要重载的抽象方法: query/insert/update/delete/getType 等，然后在配置文件中声明。
 *       系统已经为常用的数据类型(音频、视频、图片、联系方式等)提供了大量的ContentProvider
 *       -- MediaStoreContentProvider -- 增加多媒体文件的操作的 ContentProvider
 *     ContentResolver -- 客户端访问时使用的类(Context.getContentResolver() )，其内部操作对应的 ContentProvider 来对数据进行操作。 
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
				new String[] { People.NAME },  new int[] {android.R.id.textName } );
		setListAdapter(adapter);
		*********************************************************************************************************/
		
	}
	
	//根据用户名从联系人管理程序中查询
	public void QueryUserInfoByName(){
	/********************************************************************************************************
	//TODO: 1.使用的方式已经被淘汰，应该有更合适的方式; 2.需要增加 READ_CONTACTS 的权限
	 
		String name = "userName";
		//创建查询数组
		String [] projectionStrings = new String[] { People._ID, People.NAME, People.NUMBER };
		Uri contactsUri = People.CONTENT_URI;  //查询Uri对象
		String[] args = { name };	//查询参数
		//根据姓名查询联系人
		Cursor managedCursor = managedQuery(contactsUri, projectionStrings, "name=?", args, People.NAME, " ASC" );
		if(managedCursor.moveToFirst()){
			String name1 = managedCursor.getString(1); 	//获得姓名
			String number = managedCursor.getString(2); //获得电话号码
		}
	*******************************************************************************************************/
	}
	
	public void DialTelTester(){
	/*******************************************************************************************************
	    //显示拨打电话，如使用 Intent.ACTION_CALL 则直接拨打电话
		String data = "tel:15184464231";
		Uri uri = Uri.parse(data);
		Intent intent = new Intent();
		intent.setAction(Intent.ACTION_DIAL);
		intent.setData(uri);
		startActivity(intent);
	*******************************************************************************************************/
	}
}
