#pragma once

/*************************************************************************************************
* CMediaType(描述处理的媒体类型，用三个GUID说明) -- 注意使用后需要 FreeMediaType/DeleteMediaType 释放相关内存：
*   如果三个GUID都指向特定的值，则称为“完全指定的媒体类型”，
*     只要有一个指定为GUID_NULL(MEDIATYPE_NULL/MEDIASUBTYPE_NULL)，则称为“不完全指定的媒体类型”
*
* 当把 CMediaType 赋值给 AM_MEDIA_TYPE 时默认是浅拷贝，赋值完毕后将 CMediaType 的 pbFormat/cbFormat/pUnk 赋值为NULL
*   可以将其所有权赋值给 AM_MEDIA_TYPE，且避免深拷贝时的性能损失（参见 CEnumMediaTypes::Next ）
*   或者用 CopyMediaType 进行深拷贝
*
*   说明：有些GUID会对应一个 FOURCCMap，比如 
*     FOURCCMap fccMap = FCC('AVI '); pMediaType->subtype = static_cast<GUID>(fccMap);
* 
*   majortype  -- 媒体类型：
*     MEDIATYPE_Audio、
*     MEDIATYPE_AnalogVideo、 
*     MEDIATYPE_Interleaved -- 音视频交叉的数据?比如 DV 录制的数据? 
*     MEDIATYPE_Stream -- 数据流，比如 MPEG2?
*     MEDIATYPE_Video -- 纯Video, 
*   subtype    -- 格式类型：
*     MEDIASUBTYPE_Avi -- 
*     MEDIASUBTYPE_ARGB32
*     MEDIASUBTYPE_MPEG1Audio -- 属于MEDIATYPE_Stream
*     MEDIASUBTYPE_MPEG1AudioPayload -- 属于 MEDIATYPE_Audio
*     MEDIASUBTYPE_Mpeg2 -- 属于 MEDIATYPE_Stream，其中同时包含音视频?
*     MEDIASUBTYPE_MPEG2_AUDIO -- 
*     MEDIASUBTYPE_MP3 -- 
*     MEDIASUBTYPE_PCM
*     MEDIASUBTYPE_RGB8 -- 
*     MEDIASUBTYPE_RGB565 -- 
*   formattype -- 格式细节，通过 pbFormat指针指向具体的数据结构(大小为cbFormat)，
*                其中包含了视频图象的大小、帧率，音频的采样频率、量化精度等
*     FORMAT_AnalogVideo   -- 
*     FORMAT_DvInfo        --  DVINFO(DV stream)
*     FORMAT_MPEGVideo     --  MPEG1VIDEOINFO
*     FORMAT_MPEG2Video    --  MPEG2VIDEOINFO
*     FORMAT_MPEGStreams   
*     FORMAT_VideoInfo     --  VIDEOINFOHEADER, 通过 HEADER 宏可以直接访问其中的BITMAPINFOHEADER
*     FORMAT_VideoInfo2    --  VIDEOINFOHEADER2
*     FORMAT_WaveFormatEx
*
*************************************************************************************************/
/*************************************************************************************************
* 动态格式改变
*   在 Transform 等方法内部调用检查 MediaType ?
*   CBasePin 有 CanReconnectWhenActive 方法，判断是否可以在激活(Paused/Running)状态下重连，默认情况下为 FALSE
*               BreakConnect
*   需要重连接 -- 
*     IFilterGraph::Reconnect -- 重连接指定的Pin
*     IFilterGraph2::ReconnectEx -- 以指定的媒体类型重连接指定的Pin
*************************************************************************************************/
