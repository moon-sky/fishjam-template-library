#ifndef INTEL_MEDIA_SDK
#define INTEL_MEDIA_SDK
#pragma once

/*************************************************************************************************
* 术语
*   MVC(Multi-View Video Coding)
*   VPP(Video Processing)，指 Pre-Post processing ?
*
* Intel的Media SDK是一个强大的视音频编解码、处理库，它建立在Intel的显卡平台上（提供硬件加速），
*   并提供相应的软件编解码实现，免费、非开源。
*   介绍信息：http://software.intel.com/en-us/articles/intel-media-software-development-kit-intel-media-sdk/
* 
* 注意：
*   1.Intel Media SDK 目前只支持NV12(英特尔定义的视频格式，它在英特尔硬件平台上是原生态支持)
*     MSDK格式转化: http://software.intel.com/zh-cn/blogs/2010/07/26/msdkyv12nv12/
*
*
* Media SDK支持 VC1,H264 和 MPEG2 格式在Intel显卡平台的硬件加速。
* 在Media SDk中，对数字视频的一些关键核心的编码、解码算法来自于Intel IPP库，并在此基础上对处理性能进行了优化和提高。
* 提供了丰富的API以供客户二次开发，很容易扩展，从而支持任何第三方的图形硬件解决方案（graphics hardware solutions）和编解码方案，
*   提供了标准的DShow Filters。
* 
* SDK的编程结构是通过库的分发层（Dispatcher layer,即 Encode/Decode/VPP 等）暴露给应用程序的
*   提供了通用的数字视频预处理、编解码的接口，该层提供了重定向功能。
*   1.分发器（dispatcher）定位当前的显卡的设备和驱动；
*   2.根据定位去SDK中查找最合适的实现方式(如硬件加速)，并在调用中重定向(函数指针?)到合适的函数调用
*   3.如果没有找到适合的平台相关的实现方式，dispatcher会定位到SDK中用软件实现的方式
*
*
* Media SDK 初始化编解码器的步骤(也提供了简单封装好的C++接口，如 MFXVideoSession/MFXVideoENCODE 等 )：
*  1.创建和初始化一个编解码 mfxSession -- MFXInit 函数，需要指定编解码的实现方式（自动选择、硬件或软件），
*    如果是Auto时，可以使用 MFXQueryIMPL 确认当前的实现方式。
*  2.创建和初始化解码器 -- MFXVideoDECODE_Init
*  3.创建和初始化编码器 -- MFXVideoENCODE_Init
*  n.使用完毕后使用 MFXClose 关闭Session
*
* Media SDK 提供了异步机制(MFXVideoDECODE_DecodeFrameAsync、MFXVideoVPP_RunFrameVPPAsync、MFXVideoENCODE_EncodeFrameAsync)
*   等，比较适合硬件编解码（一个线程就可实现高性能）；软件编解码时可以使用流水线结构。
*
* 通过 CodecPreset 提供了预定义的一些编解码器参数
* 
* 注册表的 HKCU\Software\Intel\Media SDK Sample Filters 中会保存默认值，也可以通过代码进行设置
*   IsHWMfxlib -- 1表示使用平台相关的MediaSDK实现
*   IsDecodePartiallyAccelerated/IsEncodePartiallyAccelerated -- 是否并行加速?(MFX_WRN_PARTIAL_ACCELERATION)
*   FramesEncoded -- (已?)编码的帧数(限制还是统计值？)
*   RealBitrate -- 编码流中的真实比特率
**************************************************************************************************/

/**************************************************************************************************
* 源码分析
*   1.mfxSession -- MediaSDK的核心,通过 MFXInit初始化,使用完毕后通过MFXClose关闭,可通过MFXVideoSession类操作。
*     其函数分为以下几个部分（各部分被封装成对应的 MFXVideoXXX 类进行简化处理）
*     基本操作:   如 QueryXXX, 子Session管理，优先级(mfxPriority)管理
*     VideoCore:  设置 BufferAllocator、FrameAllocator; 设置或获取 Handle(?)；同步操作(SyncPoint) 等
*     VideoEncode:进行Encode的操作，通过 mfxVideoParam 设置相关参数，通过 EncodeFrameAsync 启动异步编码？
*     VideoDecode:进行Decode的操作，通过 DecodeHeader 先解码头部？通过 DecodeFrameAsync 启动异步解码？
*       其输入可支持 部分帧、单帧或多帧，比特流中的time stamp必须是帧数据的第一个字节[的位置]
*     VideoVPP:   通过 RunFrameVPPAsync 启动异步 VPP?  
*   2.mfxHandleType/mfxHDL -- 似乎是控制是否使用硬件、及硬件类型的东西?
*   3.mfxSyncPoint -- 
*   4.mfxBitstream -- 数据流?
**************************************************************************************************/




#endif //INTEL_MEDIA_SDK