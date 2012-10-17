#ifndef DXUT_STUDY_H
#define DXUT_STUDY_H
#pragma once

/*****************************************************************************************************
* DXUT -- 一套由SDK提供的D3D应用程序框架，通过回调机制调用由用户实现的回调函数
*   通过 DXUT 生成D3D程序框架：DirectX Sample Browser -> 选中"EmptyProject"的"Installl Project"
*     实际上就是将 Sample 中的 EmptyProject 改个名字拷贝到新的路径下(由于有ncb的问题，生成框架时不要打开该工程)
*
*   DXUT框架支持Direct3D设备和窗口一一对应（一个设备只能对应一个窗口），不支持同时使用多个设备
*     或显示多个Direct3D窗口的高级应用程序。
*
* 运行时序：由 DXUTSetCallbackXXXXX 等函数设置回调函数的指针
*   启动：
*     IsD3D9DeviceAcceptable -- 对渲染设备的特征进行检查，查看设备是否支持需要的功能
*     ModifyDeviceSettings   -- 修改渲染设备的设置
*     OnD3D9CreateDevice()   -- DXUT框架创建了Direct3D设备之后调用，可创建所有内存池类型为 D3DPOOL_MANAGED 或
*       D3DPOOL_SYSTEMMEM的资源，这些资源不需要程序员进行管理
*     OnD3D9ResetDevice      -- 创建所有内存池类型为 D3DPOOL_DEFAULT 的资源，在设备从丢失状态恢复时也会调用
*  
*   运行：
*     MsgProc -- 处理各类消息，并在空闲时反复调用 OnFrameMove 和 OnFrameRender 两个函数进行场景渲染
*       OnFrameMove   -- 通常编写为用户输入的响应
*       OnFrameRender -- 进行渲染的代码
*       OnD3D9LostDevice  -- 在应用程序运行期间，当Direct3D设备变为丢失状态时(如Alt+Tab键离开一个全屏模式的应用程序)调用，
*         释放 OnD3D9ResetDevice 中创建的设备资源
*       OnD3D9ResetDevice -- 在设备从丢失状态恢复时调用
* 
*   退出：
*     OnD3D9LostDevice 
*     OnD3D9DestroyDevice  -- 释放由函数 OnD3D9CreateDevice 创建的资源
*
* 全局函数
*   DXUTInit         -- 分析命令行，并且进行初始化
*   DXUTCreateWindow -- 创建窗口，可通过 DXUTSetCallbackMsgProc 设置窗体消息回调函数。创建的窗口句柄可由DXUTGetHWND函数获取
*   DXUTCreateDevice -- 创建D3D设备，简化了 IDirect3D9::CreateDevice 的大部分参数，扩展 DXUTCreateDeviceFromSettings，
*                       可通过 pCallbackIsDeviceAcceptable 回调函数拒绝任何硬件不支持或不想要的组合
*   DXUTGetD3DObject -- 获取 IDirect3D9 指针对象
*   DXUTSetWindow    -- 设置应用程序中创建的窗口，注意需要通过 DXUTStaticWndProc 函数将窗口消息传递给框架。
*   DXUTMainLoop     -- 进入消息循环直到程序退出，处理窗口消息、更新和渲染场景、处理设备事件
*
*   DXUTSetCursorSettings -- 为全屏模式下光标的用法设置选项
*   DXUTSetMultimonSettings -- 为框架如何在 多显示器(multiple monitors) 配置中工作设置选项
*   DXUTToggleFullscreen -- 使应用程序在窗口模式和全屏模式间切换
*   DXUTToggleREF -- 使应用程序在硬件抽象层和参考设备间切换
*
*   DXUTFindValidDeviceSettings -- 使用高度灵活的方法从枚举集合中选择最好的设备
*     1.D3DDEVTYPE_HAL(使用硬件加速) 
*     2.全屏模式时选择桌面格式所对应的适配器格式(窗口和全屏切换时快)
*     3.适配器格式所对应的后台缓冲区格式       
* 
*   DXUTGetFPS -- 获取当前每秒提交的帧数
*   DXUTGetFrameStats -- 获取一个指向字符串的指针，该字符串包括每秒帧数、分辨率、后台缓冲区格式、深度缓冲区格式
*   DXUTGetDeviceStats -- 获取一个指向字符串的指针，该字符串包括当前设备类型、顶点运算行为和设备名
*   DXUTSetConstantFrameTime -- 启用或禁用固定帧时间(默认是每秒30帧，fTimePerFrame为0.0333f)
*   DXUTSetTimer -- 添加一个新的计时器
*   DXUTPause -- 将框架的内部计数器和（或）渲染过程设为暂停状态
*   DXUTIsKeyDown -- 检查当该函数调用时，键盘上指定的某个键( VK_F1, VK_LSHIFT, VK_RCONTROL, VK_RMENU 等)是否按下
*   DXUTIsMouseButtonDown -- 检查当该函数调用时，指定的鼠标键(VK_LBUTTON, VK_RBUTTON, VK_MBUTTON, VK_XBUTTON1 等)是否按下
*
* 
*   高分辨率计时器
*   为自动测试提供命令行支持
*****************************************************************************************************/

/*****************************************************************************************************
* 设备事件
*    改变(DXUTSetCallbackDeviceChanging) -- 设置回调函数来修改Direct3D设备的创建设置，应用程序可以返回FALSE，拒绝改变该设备。
*    创建(DXUTSetCallbackDeviceCreated) -- 当应用程序初始化和重新创建设备时，在Direct3D设备创建之后立即调用
*    重设(DXUTSetCallbackDeviceReset) -- 当Direct3D设备丢失又被重新设置后立即调用
*    丢失(DXUTSetCallbackDeviceLost) -- 如 Alt+Tab键离开一个全屏模式的应用程序，或者按下Ctrl+Alt+Del键，
*      或者启动了另一个全屏3D应用程序，应用程序应 释放所有不能在设备丢失时存在的Direct3D资源对象。
*      当设备返回时，应用程序必须调用函数 IDirect3DDevice9::Reset()，并重新创建所有不能在Reset()函数中存在的对象。
*    销毁(DXUTSetCallbackDeviceDestroyed) -- 当应用程序终止或重新创建设备时，Direct3D设备被销毁后，立即调用
* 
* 帧事件(渲染过程中的每一帧被调用)
*   DXUTSetCallbackFrameMove -- 在每一帧开始时调用一次，通常用于处理场景更新(坐标变换)
*   DXUTSetCallbackFrameRender -- 在每一帧结束或窗口需要重画时调用，完成对场景的渲染，该函数返回后，
*     框架将调用 Present 来显示交换链中下一缓冲区的内容
*
* 消息事件
*   DXUTSetCallbackKeyboard -- 处理键盘事件
*   DXUTSetCallbackMouse -- 处理鼠标事件
*   DXUTSetCallbackMsgProc -- 处理窗口消息
*****************************************************************************************************/

/*****************************************************************************************************
* 文本绘制 -- 提供了类 CDXUTTextHelper 来简化文本显示
*   初始化 ID3DXSprite 和 ID3DXFont 对象 -- 可以在 OnCreateDevice 中用 D3DXCreateFont 创建
*   显示文本: DrawTextLine 等
*   释放 ID3DXSprite 和 ID3DXFont 对象
*
* 控件 -- 通常先在窗口中加载对话框，然后在对话框中添加响应的控件，由对话框来管理控件
*   CDXUTDialogResourceManager -- 对话框资源管理器类，一般需要先处理全局消息以更新GUI，然后进入各个对话框的消息处理函数
*   CDXUTDialog -- 
*     AddButton
*     AddComboBox
*     AddCheckBox
*     AddEditBox
*     AddRadioButton
*     AddSlider
*   CD3DSettingsDlg -- DXUT封装好的一个专门用于Direct3D渲染设备设置的对话框类
*****************************************************************************************************/

#endif //DXUT_STUDY_H
