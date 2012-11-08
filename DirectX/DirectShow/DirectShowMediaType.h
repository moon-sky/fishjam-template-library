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
*     MEDIASUBTYPE_AVC1 -- H.264 bitstream without start codes
*     MEDIASUBTYPE_Avi -- 
*     MEDIASUBTYPE_ARGB32 -- 每个像素用32位表示，RGB分量各使用8位（剩下的8位用于表示Alpha通道值），
*       在内存中RGB各分量的排列顺序为：BGRA,通常可以使用RGBQUAD数据结构来操作一个像素
*     MEDIASUBTYPE_H264 -- H.264 bitstream with start codes.
*     MEDIASUBTYPE_MPEG1Audio -- 属于MEDIATYPE_Stream
*     MEDIASUBTYPE_MPEG1AudioPayload -- 属于 MEDIATYPE_Audio
*     MEDIASUBTYPE_Mpeg2 -- 属于 MEDIATYPE_Stream，其中同时包含音视频?
*     MEDIASUBTYPE_MPEG2_AUDIO -- 
*     MEDIASUBTYPE_MP3 -- 
*     MEDIASUBTYPE_PCM
*     MEDIASUBTYPE_RGB24 -- 内存中RGB各分量的排列顺序为 BGR， 通常可以使用RGBTRIPLE数据结构来操作一个像素
*     MEDIASUBTYPE_RGB8 -- 
*     MEDIASUBTYPE_RGB565 -- 
*     MEDIASUBTYPE_UYVY --  
*     MEDIASUBTYPE_AYUV --  带有一个Alpha通道，并且为每个像素都提取YUV分量(4:4:4)，图像数据格式为 A0 Y0 U0 V0 
*   formattype -- 格式细节，通过 pbFormat指针指向具体的数据结构(大小为cbFormat)，
*                其中包含了视频图象的大小、帧率，音频的采样频率、量化精度等
*     FORMAT_AnalogVideo   -- 
*     FORMAT_DvInfo        -- DVINFO(DV stream)
*     FORMAT_MPEGVideo     -- MPEG1VIDEOINFO
*     FORMAT_MPEG2Video    -- MPEG2VIDEOINFO
*     FORMAT_MPEGStreams   --   
*     FORMAT_VideoInfo     -- VIDEOINFOHEADER, 通常用于 MEDIATYPE_Video + MEDIASUBTYPE_RGBXXX时，通过 HEADER 宏可以直接访问其中的BITMAPINFOHEADER
*     FORMAT_VideoInfo2    -- VIDEOINFOHEADER2，增加了 dwPictAspectRatioX x dwPictAspectRatioY（显示比例，如 16x9） 等,
*     FORMAT_WaveFormatEx  -- PCM音频使用
*
*   bFixdSizeSamples -- 如为TRUE，表示ISampleSize有效，是每个Sample的大小固定；否则忽略lSampleSize值
*   lSampleSize -- 表示每个Sample的大小，如为0表示大小可变
*   bTemporalCompression -- 如值为FALSE，表示所有帧都是关键帧
*   pUnk -- ?
*
*************************************************************************************************/

/*************************************************************************************************
* 动态格式改变(Dynamic Format Change)
*   在 Transform 等方法内部调用检查 MediaType ?
*   CBasePin 有 CanReconnectWhenActive 方法，判断是否可以在激活(Paused/Running)状态下重连，默认情况下为 FALSE
*               BreakConnect
* 连接 -- 
*   IFilterMapper::EnumMatchingFilters -- 从系统中枚举出满足要求(Merit,Major,Sub等)的Filter
*   IGraphBuilder::AddSourceFilter -- 
*   IGraphBuilder::Render -- 从某个输出Pin处开始完成余下的graph构建。该方法会自动在输出pin后面添加必须的filter，直到renderer filter为止
*   IGraphBuilder::RenderFile -- 构建一个完整的文件回放graph
*   IFilterGraph::ConnectDirect -- 在两个pin之间进行直接连接，如果连接失败，则返回失败
*   IFilterGraph::Connect（GraphEdt上的方式?） -- 连接两个Pin，如果可能的话，直接连接它们，否则，在中间加入其它的filter来完成连接
*   IFilterGraph::Reconnect -- 重连接指定的Pin
*   IFilterGraph2::ReconnectEx -- 以指定的媒体类型重连接指定的Pin
*************************************************************************************************/
