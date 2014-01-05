package com.fishjam.android.study;
import android.test.AndroidTestCase;

/**************************************************************************************************************************************
 * TODO: id="@+id/MyTextView01" -- 前面的加号"+"什么意思？如果没有则加入？
 *
 * 资源管理器(ResourceManager)--提供访问非代码资源，如本地化字符串，图像，布局文件的能力。
 *   使用资源(resouces -- 其中有 attr/drawable/id/layout/string/style 等多种)
 *     Resources resources = getBaseContext().getResources();
 *     Drawable HippoDrawable = resources.getDrawable(R.drawable.white);
 *     mTextView01.setBackgroundDrawable(HippoDrawable); //设置背景色
 *   使用资源时，有三种引用形式：
 *     android:xxx/yyy,
 *     android:@xxxx/yyy  -- 如 @string/hello, @drawable/white
 *     android:?xxx
 *       @+id/myPassword
 *   
**************************************************************************************************************************************/
/**************************************************************************************************************************************
 * Android提供了基于XML的界面构建方式，将界面和应用程序逻辑分离.xml文件放在 res(存放所有非代码资源 -- 图片、本地化字符串、XML布局文件等)目录下。
 *   使用名为"R.java"的文件来定义所有资源的索引，自动生成，不要更改。
 *   其中分为 attr(属性), drawable(图片资源)、layout(页面布局)、string等多个静态内部类, public final class R { xxxx }，
 *   代码中引用资源时： R.资源类型.资源名称
 *   
 * 系统提供的资源： android.R.资源类型.资源名称, 如 android.R.anim.fade_in(系统预定义动画); android.R.string.cancel(系统预定义的字符串 -- 可自动支持多语言?)
 *   Context.getString()/
 *   资源文件中引用时： "@资源类型.资源名称"
 *   
 * 从资源中加载字符串
 *   CharSequence str_2 = getString(R.string.str_2);  或 String str = getString(R.string.str_2).toString();
 *   字符串资源可以通过 array/ item 的方式创建字符串数组，然后在 AlertDialog中通过 setItems(R.array.items_irdc_dialog, ...) 来使用
 *   或者通过 getResources().getStringArray(R.array.items_irdc_dialog) 返回 String[] 。
 *   示例： <array name="items_irdc_dialog"><item>中餐</item><item>日式火锅</item><item>巴西烧烤</item></array>
 *   
 * 颜色( #Alpha-Red-Green-Blue 格式), 如： #RGB, #ARGB, #RRGGBB, #AARRGGBB， 代码中获得颜色方法：Resources.getColor
 *   定义名为color.xml的文件,放在values目录下,然后可以main.xml中使用:如 android:background="@drawable/white" 
 *      <resources>  //在 graphics.Color 中定义了默认的颜色常数
 *        <drawable name="darkgray">#808080FF</drawable>
 *        <drawable name="white">#FFFFFFFF</drawable>
 *      </resources>
 *
 * 动画 -- ArrayAdapter<String> 合作 ? 
 *   ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, countriesStr);
 *   adapter.setDropDownViewResource(R.layout.myspinner_dropdown);  //myspinner_dropdown为自定义下拉菜单模式定义在res/layout目录下
 *   mySpinner.setAdapter(adapter);		//将ArrayAdapter添加Spinner对象中
 * 
 * 使用字体
 *   1.在assets目录中创建 fonts 子目录,加入 .ttf 文件;
 *   2.引用 android.graphics.Typeface;
 *   3.通过 creatFromAsset 创建Typeface对象
 *      m_Text.setTypeface(Typeface.createFromAsset(getAssets(),"fonts/HandmadeTypewriter.ttf"));
 *  
 * 菜单 -- 选项菜单(OptionMenu)、上下文菜单(ContextMenu -- 长时间按键不放时)、子菜单(SubMenu)
 *   MenuInflater --
 *   系统菜单 -- 重载 Activity 的 特定函数
 *      onCreate { mi = new MenuInflater(this); } 
 *      onCreateOptionsMenu(Menu menu){ mi.inflate(R.menu.test_menu, menu); return true; } 
 *        或通过 menu.add/addSubMenu 等方法动态添加
 *      onOptionsItemSelected(MenuItem item){ switch(item.getItemId()) { case R.id.about:  xxxx; break; } return true; }
 *    上下文菜单 -- 重载 onCreateContextMenu/onContextItemSelected,  然后调用 registerForContextMenu(xxx) 为指定视图注册
 *    
 *      
 * 使用尺寸 -- 单位(dimens)
 *   px(像素), in(英寸--基于屏幕的物理尺寸),mm(毫米), pt(点--英寸的1/72), dp(和密度无关的像素-相对屏幕物理密度的抽象单位),sp(和精度无关的像素)
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * XML -- 通过 getResources().getXml() 获得 XmlResourceParser 对象实例表示XML原始文件
 *   import org.xmlpull.v1.XmlPullParser;
 *   XmlResourceParser xrp = getResources().getXml(R.xml.testXmlFile);
 *   while(xrp.getEventType() != XmlResourceParser.END_DOCUMENT) { 	//未到文件尾则继续循环
 *     if(xrp.getEventType() == XmlResourceParser.START_TAG) { 	//如果是开始标签  
 *       String name = xrp.getName(); 	//获取标签名
 *       xrp.getAttributeValue(nIndex); 	//获取指定索引位置的属性值,更好的方法是根据属性名获取值
 *     }
 *     xrp.next();  //获取下一条记录
 *   }
**************************************************************************************************************************************/
public class ResourceTester  extends AndroidTestCase {

}
