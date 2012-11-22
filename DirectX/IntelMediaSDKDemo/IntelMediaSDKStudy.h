#ifndef INTEL_MEDIA_SDK
#define INTEL_MEDIA_SDK
#pragma once
IMemAllocator
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
*   1.Intel Media SDK 似乎只支持NV12(英特尔定义的视频格式，它在英特尔硬件平台上是原生态支持)
*     MSDK格式转化: http://software.intel.com/zh-cn/blogs/2010/07/26/msdkyv12nv12/
*                   http://software.intel.com/en-us/articles/using-unsupported-color-formats-with-the-intel-media-software-development-kit/
*   2.其Filter 对 ALLOCATOR_PROPERTIES 要求比较高，比如 cBuffers 一般都要10左右(为了异步时能多处理几个)，
*     cbBuffer 一般都要比算出来的理论值大，否则就过不了 CheckInput，
*     而且似乎Sample的Enc中会自动更改高宽(32字节对齐，比如 320x240=> 320x256)
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
*
* 插件扩展
*   通过 mfxPlugin(C++是MFXPlugin)给用户提供扩展的机会,Sample 中提供了 CSCPlugin(从 RGB32 => NV12 转换)
*   
**************************************************************************************************/

/**************************************************************************************************
* 使用特殊的 MFX_TIME_STAMP, 通过 ConvertReferenceTime2MFXTime / xxxx 转换，
*   定义了 MFX_TIME_STAMP_FREQUENCY(90000) 常量
*   
* CSCProcessor -- Color Space Converter - converts frames from RGB4 to NV12,
* 
* 源码分析
*   1.mfxSession -- MediaSDK的核心,通过 MFXInit初始化,使用完毕后通过MFXClose关闭,可通过MFXVideoSession类操作。
*     其函数分为以下几个部分（各部分被封装成对应的 MFXVideoXXX 类进行简化处理）
*     基本操作:   如 QueryXXX, 子Session管理，优先级(mfxPriority)管理
*     VideoCore:  设置 BufferAllocator、FrameAllocator; 设置或获取 Handle(?)；同步操作(SyncPoint) 等
*     VideoEncode:进行Encode的操作，通过 mfxVideoParam 设置相关参数，通过 EncodeFrameAsync 启动异步编码？
*     VideoDecode:进行Decode的操作，通过 DecodeHeader 先解码头部？通过 DecodeFrameAsync 启动异步解码？
*       其输入可支持 部分帧、单帧或多帧，比特流中的time stamp必须是帧数据的第一个字节[的位置]
*     VideoVPP:   通过 RunFrameVPPAsync 启动异步 VPP，会返回一个同步点  
*   n.mfxBitstream -- 数据流?
*   n.mfxHandleType/mfxHDL -- 似乎是控制是否使用硬件、及硬件类型(D3D9,D3D11,VA_DISPLAY)的东西?
*     mfxIMPL 对应的 MFX_IMPL_XXX 枚举值
*   n.mfxInfoMFX -- 是很多配置编解码信息的结构体(其中包括了 H264、MPEG-2、JPEG配置的union)
*       CodecId -- 编解码的ID
*       CodecProfile -- MFX_PROFILE_AVC_STEREO_HIGH | xxx;
*       TargetUsage --
*       RateControlMethod -- MFX_RATECONTROL_CBR|
*       mfxU32  reserved[3];
*   n.mfxInfoVPP -- 其内部包含了两个 mfxFrameInfo 结构体，分别表示输入和输出的媒体帧信息。
*   n.mfxFrameInfo -- 媒体的帧信息(其作用类DS中的Format?)
*       AspectRatioW/AspectRatioH -- 比例?
*       ChromaFormat -- MFX_CHROMAFORMAT_YUV420
*       FourCC -- MFX_FOURCC_NV12
*       FrameRateExtN/FrameRateExtD -- 帧率的分数表示，可以通过SDK中提供的 ConvertFrameRate 函数设置
*       Height/Width -- MSDK_ALIGN16 或 MSDK_ALIGN32 宏 调整后的高度和宽度
*       PicStruct -- ?
*   n.mfxSyncPoint -- 同步点，是一个指针，
*   n.mfxVideoParam
*       IOPattern -- 指定内存模型？
*         启动了D3D的话，一般是：MFX_IOPATTERN_IN_VIDEO_MEMORY|MFX_IOPATTERN_OUT_VIDEO_MEMORY;
*         没有启动D3D的话，一般是：MFX_IOPATTERN_IN_SYSTEM_MEMORY|MFX_IOPATTERN_OUT_SYSTEM_MEMORY;
*
* Pipeline
*   Init{
*     FileRead::Init -> InitFileWrite -> Session::init -> CreateAllocator -> InitMfxEncParams 
*     -> InitMfxVppParams -> new  MFXVideoENCODE -> [new MFXVideoVPP(如果需要更改大小或FourCC) -> ] 
*     -> ResetMFXComponents { MFXVideoENCODE::Init -> MFXVideoVPP::Init -> CEncTaskPool::Init }
*   }
*   for(...) 
*   {
*     Run{
*       GetFreeTask -> GetFreeSurface(输入的空闲surface) -> FileReader::LoadNextFrame(读入文件数据)
*       -> MFXVideoVPP::RunFrameVPPAsync(&m_pVppSurfaces[nVppSurfIdx],调用VPP处理) -> MFXVideoENCODE::EncodeFrameAsync(编码)
*       -> MFXVideoVPP::RunFrameVPPAsync(NULL,再次调用VPP处理) -> MFXVideoENCODE::EncodeFrameAsync(再次编码)
*     }
*   }
*   Close{
*     DeleteFrames -> DeleteAllocator -> Close TaskPool/Session/FileReader
*   }
**************************************************************************************************/




#endif //INTEL_MEDIA_SDK