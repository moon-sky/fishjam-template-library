package com.fishjam.android.study;
import android.test.AndroidTestCase;

/***************************************************************************************************************************************
* BroadcastReceiver -- 接收广播消息，可以接收其他组件通过 sendBroadcast, sendStickyBroadcast, sendOrderedBroadcast 等发送的消息。
*   onReceive -- 重写该方法，接收系统的广播消息
*   
*   注册：
*     1.AndroidManifest.xml 文件中使用 <receiver ... /> 配置。
*     2.代码中通过 Context.registReceiver() 来注册
***************************************************************************************************************************************/

public class BroadcastReceiverTester extends AndroidTestCase{


}
