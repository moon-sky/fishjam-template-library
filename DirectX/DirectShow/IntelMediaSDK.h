#ifndef INTEL_MEDIA_SDK
#define INTEL_MEDIA_SDK
#pragma once

/*************************************************************************************************
* Intel的Media SDK是一个强大的视音频编解码库，它建立在Intel的集成显卡G45平台（提供硬件加速），
*   并能支持后继的其他Intel显卡产品。
*   免费、非开源。
* Media SDK支持VC1,H264和MPEG2格式在Intel显卡平台的硬件加速，并提供相应的软件编解码实现。
* 不仅提供了丰富的API以供客户二次开发，而且也提供了标准的DShow Filters。
*
* Media SDK 初始化编解码器的步骤：
*  1.创建和初始化一个编解码 Session -- MFXInit 函数，需要指定编解码的实现方式（自动选择、硬件或软件），
*    如果是Auto时，可以使用 MFXQueryIMPL 确认当前的实现方式。
*  2.创建和初始化解码器 -- MFXVideoDECODE_Init
*  3.创建和初始化编码器 -- MFXVideoENCODE_Init
*
* Media SDK 提供了异步机制(MFXVideoDECODE_DecodeFrameAsync、MFXVideoVPP_RunFrameVPPAsync、MFXVideoENCODE_EncodeFrameAsync)
*   等，比较适合硬件编解码（一个线程就可实现高性能）；软件编解码时可以使用流水线结构。

*************************************************************************************************/



#endif //INTEL_MEDIA_SDK