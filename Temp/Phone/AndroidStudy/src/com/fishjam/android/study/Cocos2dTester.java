package com.fishjam.android.study;
import junit.framework.TestCase;

//http://wenku.baidu.com/view/af55f852ad02de80d4d84024.html
/***************************************************************************************************************************************
 * 注意：是 C/C++ 语言的
 * http://www.cocos2d-x.org/， 完全免费且可用于商业开发的开源游戏开发框架，基于OpenGL ES，支持硬件加速。
 *
 * Cocos2D -- 
 * Cocos2D-x -- C++移植版，支持多平台(Windows/iOS/Android等),
 * Cocos2D-iPhone -- http://code.google.com/p/cocos2d-iphone/, 其License 基于GNU LGPL v3 license并进行了扩展，
 *    可静态链接或直接使用源码，用于闭源的商业开发。
 * Cocos2D-JavaScript -- 基于HTML5 Canvas
 *
 * 独立编辑器
 *   Cocos2D-Python -- 所见即所得的世界编辑器 
 *   Particle Designer -- 
 *   SpriteSheet Editors
 *
 * 配置文件
 *   app.config.txt
 *   app.icf
 * 
 * 跨平台开发、编译 -- 与开发逻辑相关的文件放在Classes和Resource文件夹，和平台相关的代码(如程序启动入口)通常在各个Win32、linux等目录下
 *   Win32 -- VS
 *   Android -- ndk的方式编译成.so
 *
 * 库
 *   物理库
 *   音频引擎
 **************************************************************************************************************************************/

/**************************************************************************************************************************************
 * Actions(动作)--告诉精灵们该做什么。可组合的动作如移动(move),旋转(rotate),缩放(scale)等
 * Documents(文档)--编程指南 + API参考 + 视频教学 + 很多教你如何使用的简单测试例子
 * Effects(特效)--特效包括波浪(waves),旋转(twirl),透镜(lens)等
 * Flow control(流程控制)--管理不同场景(scenes)之间的流程控制
 * Menus(菜单)--创建内部菜单
 * Sprites(精灵)--快速而方便的精灵
 * Text Rendering(文本渲染)--支持标签和HTML标签动作
 * Tiled Maps(平面地图)--支持包括矩形和六边形平面地图
 * Transitions(转换)--从一个场景移动到另外一个不同风格的场景
 **************************************************************************************************************************************/

/*****************************************************************************************************
 * Director(导演) ==> Scene(场景) ==> Layer(层) ==> 内容元素(Label,Sprite,地图等)
 *   任何时间，只有一个Scene对象实例处于运行激活状态
 *
 * 很多类都有一个静态的 sharedXxx 工厂函数
 * CCApplication -- 通常需要继承子类(AppDelegate,应用程序代理),并在applicationDidFinishLaunching中初始化
 * CCDirector(导演) -- 负责游戏全过程的场景切换，已经预定义了单例
 *   runWithScene() -- 主程序启动时显示第一个场景
 *   pushScene --
 * CCEGLView -- 
 * CCFileUtils -- 
 * CCLayer -- 层，通常业务界面从该类继承，而且为了同时显示多层，需要透明或半透明
 *   系统已经提供了 CCMenu、CCLayerRGBA 等基类层
 * CCScene(场景) -- 构成整个游戏的流程的画面，大致可以分为以下几类：展示类、选项类、游戏
 * CCSprite(精灵) -- 可以不断变化的图片，整个游戏开发处理的主要对象，如飞机、人物等
 * workflow(流程，有类吗?)
*****************************************************************************************************/

public class Cocos2dTester extends TestCase{

}

