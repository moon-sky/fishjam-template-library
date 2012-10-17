#ifndef DIRECTX_STUDY_H
#define DIRECTX_STUDY_H

/*****************************************************************************************************
* DirectX 9.0 的成员
*   1.DirectX Graphics -- 集成了以前的 DirectDraw(处理2D图像) 和 Direct3D
*       DirectDraw(7.0以后逐步和3D集成) -- 使用页面切换的方法提供动画，直接访问图形协处理器内存的管理。
*       Direct3D -- 提供了高层和底层的3D硬件接口。
*   2.DirectInput -- 输入服务（鼠标、键盘、游戏杆等，支持力反馈设备）。
*      按下键盘上的键时，产生两串数据：扫描码VK_(WM_KEYDOWN) 和 ASCII码(WM_CHAR)；
*      GetAsynKeyState <== 跟踪状态表中该键的最后已知状态
*      GetKeyboardState <==
*      GetKeyState
*   3.DirectPlay -- 多人网络游戏的通信、组织，支持会话、休息室等概念
*   4.DirectSetup -- 提供自动安装DirectX组件的API
*   5.DirectMusic -- 支持MIDI音乐合成和播放，具有DLS(可下载声音)系统
*   6.DirectSound -- 音频捕捉、回放、音效处理、硬件加速、直接设备访问
*   7.DirectShow -- 处理各种格式的媒体文件的回放、音视频采集等高性能的多媒体应用
*   8.DirectX Media Objects -- DirectShow Filter 的简化模型，提供更方便的流数据处理方案
*
* DirectX 10 -- 最核心的是“统一运算架构”，显卡不仅可以进行顶点运算，还能进行复杂的逻辑运算(GPU)？
*
* Managed DirectX -- Microsoft.DirectX.AudioVideoPlayback.Video 
*     http://www.microsoft.com/downloads/details.aspx?FamilyId=77960733-06E9-47BA-914A-844575031B81&displaylang=en
*     http://www.microsoft.com/downloads/details.aspx?FamilyId=8AF0AFA9-1383-44B4-BC8B-7D6315212323&displaylang=en
*****************************************************************************************************/

/*****************************************************************************************************
* DXErr.exe -- DX 的错误码查询
* PIXWin.exe -- D3D的调试分析工具，可生成调用记录，并分析D3D API的调用和执行情况，OpenGL中对应的是 GLIntercept
* 
* 
*****************************************************************************************************/

#endif //DIRECTX_STUDY_H