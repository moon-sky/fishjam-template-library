#ifndef INTEL_MEDIA_SDK
#define INTEL_MEDIA_SDK
#pragma once

/*************************************************************************************************
* Intel的Media SDK是一个强大的视音频编解码库，它建立在Intel的显卡平台上（提供硬件加速），并提供相应的软件编解码实现。
*   免费、非开源。
*   介绍信息：http://software.intel.com/en-us/articles/intel-media-software-development-kit-intel-media-sdk/
* 
* 注意：
*   1.Intel Media SDK 目前只支持NV12(英特尔定义的视频格式，它在英特尔硬件平台上是原生态支持)
*     MSDK格式转化: http://software.intel.com/zh-cn/blogs/2010/07/26/msdkyv12nv12/
* 
* Media SDK支持 VC1,H264 和 MPEG2 格式在Intel显卡平台的硬件加速。
* 在Media SDk中，对数字视频的一些关键核心的编码、解码算法来自于Intel IPP库，并在此基础上对处理性能进行了优化和提高。
* 提供了丰富的API以供客户二次开发，很容易扩展，从而支持任何第三方的图形硬件解决方案（graphics hardware solutions）和编解码方案，
*   提供了标准的DShow Filters。
* 
* SDK的编程结构是通过库的分发层（Dispatcher layer）暴露给应用程序的 -- 提供了通用的数字视频预处理、编解码的接口，
* 该层提供了 重定向功能。
*   1.分发器（dispatcher）定位当前的显卡的设备和驱动；
*   2.根据定位去SDK中查找最合适的实现方式；
*   3.如果没有找到适合的平台相关的实现方式，dispatcher会定位到SDK中用软件实现的方式
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

*************************************************************************************************/



#endif //INTEL_MEDIA_SDK