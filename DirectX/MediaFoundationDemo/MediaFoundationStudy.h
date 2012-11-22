#pragma once
//Media Foundation 介绍 http://hi.baidu.com/neil_danky/item/79b5b15f5a735111da1635af
//http://blogs.msdn.com/b/mf/archive/2009/12/02/mfsimpleencode.aspx
//mfcopy http://blogs.msdn.com/b/mf/archive/2009/12/16/mfcopy.aspx

//设置内存出现错误时，弹出错误信息，然后结束
HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

//其中的 mfh264enc.dll 可以进行 H264 编码, mfAACEnc.dll 进行 AAC 编码?

//硬件编解码: MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS

/*************************************************************************************************
* 调试
*   1.Sample 中有 TopoEdit 工具(类似 GraphEdt 工具)
*   2.MFTrace.exe  MyProgram.exe >> trace.txt
*     http://social.msdn.microsoft.com/Forums/zh-SG/mediafoundationdevelopment/thread/137b2208-8601-4633-88f9-30cc79cb7794
*   3.ParseTopologies.cmd  -- ？same url as up
*   4.TextAnalysisTool.NET
*     http://blogs.msdn.com/b/mf/archive/2010/09/09/analyzing-media-foundation-traces.aspx
*
*************************************************************************************************/


/*************************************************************************************************
*
* 
* Media Foundation -- 微软所推出新一代的基于COM的多媒体应用平台，用来替代 DirectShow, Windows Media SDK,DMO 等，
*   在Vista之后作为DirectShow的替代被引入，需要 .NET4的支持 ??
* 
* 相对 DirectShow 的优势（微软建议开发人员编写 MFTs，取代 DirectShow Transforms）
*    1.可扩展的高清晰度内容和数字版权管理保护（DRM-protected）的内容 -- 媒体内容保护
*    2.允许DirectX视频加速用于之外DirectShow的基础设施。支持DXVA 2.0 , Win7中支持更多格式、视频捕获设备和硬件编解码等。
*    3.MF的可扩展性（extensibility），使不同的内容保护系统一起运作
*    4.可以使用多媒体类型计划程序服务（MMCSS）
* 
* MF架构：
*   控制层(Control layer)
*     Media Session
*     Topology
*     Sequencer
*   核心层(Core layer)，也称为 媒体管道(pipeline,异步处理媒体数据)
*     分三个组成部分：
*       a.媒体来源(Media Source)
*       b.媒体平台变换(Media Foundation Transforms -- MFT)
*       c.媒体接收器(Media Sink) -- 分为 renderer(显示或呈现) 和 archive(保存) 两类
*   平台层(Platform layer)
*     Media Streams
*     Presentation Clock
*
* MFT -- 实现IMFTransform的COM对象，替代DMO，常见的MFT CLSID定义在 wmcodecdsp.h 中
*   应用程序可以在pipeline中使用，或直接使用它们作为独立的对象，
*   其种类如下(TopoEdit.exe -> Topology -> Add Transform 可查看):
*     音频和视频编解码器, 如 "H264 Encoder MFT"
*     音频和视频效果
*     Multiplexers 和 demultiplexers
*     色彩空间转换器
*
* 使用硬件
*   硬件设备在Media Foundation中展现时使用代理对象，在管道中使用它就象在使用基于软件的组件一样
*
*
* 低级API接口(mfapi.h + mfidl.h)
*   
* 高级API接口
*   MFPlay -- 用于音频和视频的播放。它们支持的典型播放操作(停止，暂停，查找，评价控件，音量及向前)
*   Source Reader -- 从媒体文件中取出原始数据或解码数据(如从视频文件中获取到缩略图或从网络摄像机中获取到实时的视频帧)
*   Sink Writer -- 通过一些未经压缩或编码的数据来创建媒体文件(如可以重新编码或混合一个音频文件)
*   Transcode -- 完成常见的音视频转码任务
*************************************************************************************************/

/************************************************************************************************
* 流程 -- 最重要的似乎是 IMFMediaSession、IMFTopology、IMFTranscodeProfile、IMFMediaEvent 等几个接口?
*   #include <mfapi.h> 和 <mfidl.h>
*   CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
*   MFStartup(MF_VERSION);
*   {
*     1.得到文件对应的 IMFMediaSource
*       MFCreateSourceResolver -> IMFSourceResolver::CreateObjectFromURL -> QueryInterface 得到 IMFMediaSource
*     2.通过 MFCreateMediaSession 和 MFCreateTranscodeProfile 得到空的 Session 和 Profile，并设置合适的值
*     3.运行 Encoding Session 进行编码
*     
*   }
*   MFShutdown();
*************************************************************************************************/

//IMFAttributes *pMFAttributes = NULL;
//hr_SW = MFCreateAttributes( &pMFAttributes, 100 );
//hr_SW = pMFAttributes->SetGUID( MF_TRANSCODE_CONTAINERTYPE, MFTranscodeContainerType_MPEG4 );//video
//hr_SW = pMFAttributes->SetUINT32( MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, true ); //==> for hardware encode
//hr_SW = pMFAttributes->SetUINT32( MF_READWRITE_DISABLE_CONVERTERS , false );
  //==> for hardware encode, msdn says if I would like set MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS true, this parameter should be false.

//hr_SW = MFCreateFile( MF_ACCESSMODE_READWRITE, MF_OPENMODE_DELETE_IF_EXIST, MF_FILEFLAGS_NONE, L"c:\\xxx.mp4", &pStream );
//hr_SW = MFCreateSinkWriterFromURL( NULL, pStream, pMFAttributes, &pSinkWriter ); // ==> Here, I got wrong return value.

//hr = MFTRegisterLocalByCLSID( CLSID_MF_H264EncFilter, MFT_CATEGORY_VIDEO_ENCODER, L"Intel\xae Media SDK H.264 Encoder MFT", NULL, 0, NULL, 0, NULL);