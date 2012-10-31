#pragma once

//设置内存出现错误时，弹出错误信息，然后结束
HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

//其中的 mfh264enc.dll 可以进行 H264 编码, mfAACEnc.dll 进行 AAC 编码?
/*************************************************************************************************
* Media Foundation -- 微软所推出新一代的基于COM的多媒体应用平台，用来替代 DirectShow, Windows Media SDK,DMO 等，
*   ?? 在Vista之后才能使用，需要 .NET4的支持 ??
* 
* 相对 DirectShow 的优势（微软建议开发人员编写 MFTs，取代 DirectShow Transforms）
*    1.可扩展的高清晰度内容和数字版权管理保护（DRM-protected）的内容
*    2.允许DirectX视频加速用于之外DirectShow的基础设施。支持DXVA 2.0
*    3.MF的可扩展性（extensibility），使不同的内容保护系统一起运作
*    4.可以使用多媒体类型计划程序服务（MMCSS）
*    
* 
* MF架构：
*   控制层(Control layer)
*     Media Session
*     Topology
*     Sequencer
*   核心层(Core layer)，也称为 媒体管道(pipeline)?
*     分三个组成部分：
*       a.媒体来源(Media Source)
*       b.媒体接收器(Media Sink)
*       c.媒体平台变换(Media Foundation Transforms -- MFT)
*   平台层(Platform layer)
*     Media Streams
*     Presentation Clock
*
* MFT -- 应用程序可以使用MFTs内的管道，或直接使用它们作为独立的对象，其种类如下:
*   音频和视频编解码器
*   音频和视频效果
*   Multiplexers 和 demultiplexers
*   色彩空间转换器
*************************************************************************************************/

/************************************************************************************************
* 流程 -- 最重要的似乎是 IMFMediaSession、IMFTopology、IMFTranscodeProfile、IMFMediaEvent 等几个接口?
*   #include <mfapi.h> 和 <mfidl.h>
*   CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
*   MFStartup(MF_VERSION);
*   {
*     1.得到文件对应的 IMFMediaSource
*       MFCreateSourceResolver -> IMFSourceResolver::CreateObjectFromURL -> QueryInterface 得到 IMFMediaSource
*     2.通过 MFCreateMediaSession 和 MFCreateTranscodeProfile 得到空的 Session 和 Profile
*       
*     
*   }
*   MFShutdown();
*************************************************************************************************/