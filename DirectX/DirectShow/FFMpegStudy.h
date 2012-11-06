#pragma once

//CPP 中包含ffmpeg头文件时，需要使用 extern "C" 声明为C编译链接方式
#if 0
#  ifdef __cplusplus
extern "C" {
#  endif

#include <libavcodec\avcodec.h>  
#include <libavformat\avformat.h>  
#include <libswscale\swscale.h>  

#  ifdef __cplusplus
}
#  endif

#endif

//http://blog.sina.com.cn/s/blog_51396f890100o1yf.html
//http://www.cnblogs.com/billcoco/archive/2012/06/18/2553939.html -- FFMpeg写MP4文件例子分析
//http://hi.baidu.com/y11022053/item/6d4c34ba87c7b5f362388e9a  ffmpeg编码h264
//视频共享网站: fms(在线录制、播放)+ffmpeg(后台格式转换)

/* -- 带 h264 的编译方式
$ ./configure --enable-shared --enable-w32threads --disable-static --enable-memalign-hack 
--disable-yasm --enable-libx264 --enable-gpl --extra-ldflags="-L/usr/local/lib -W1,-add-stdcall-alies" --extra-cflags="-I/usr/local/include" --enable-swscale
*/

/*********************************************************************************************
* 缓冲区内存分配
*   1.av_image_alloc -- 推荐方式?
*   2.avpicture_get_size 获取大小后 av_malloc

* AVDictionary -- 一个键值对的数组，通常在获取或设置参数时使用
* 
* AVFrame
*   data/linesize -- FFMpeg内部以平面的方式存储原始图像数据，即将图像像素分为多个平面（R/G/B或Y/U/V），
*      data数组内的指针指向各个像素平面的起始位置，linesize数组则存放各个存贮各个平面的缓冲区的行宽
*   key_frame -- 该图像是否是关键帧
*   pict_type -- 该图像的编码类型：I(1)/P(2)/B(3)
*		
* AVStream -- 描述一个媒体流，其大部分信息可通过 avformat_open_input 根据文件头信息确定，其他信息可通过 avformat_find_stream_info 获取
*   典型的有 视频流、中英文音频流、中英文字幕流(Subtitle)，可通过 av_new_stream、avformat_new_stream 等创建
*   index -- 在AVFormatContext中流的索引，其值自动生成(AVFormatContext::streams[index])
*   nb_frames -- 流内的帧数目
*   r_frame_rate -- 
*   time_base -- 流的时间基准，是一个实数，该流中媒体数据的pts和dts都将以这个时间基准为粒度。通常，使用av_rescale/av_rescale_q可以实现不同时间基准的转换
* AVFormatContext -- 描述了一个媒体文件或媒体流的构成和基本信息，由 avformat_open_input 创建并初始化部分值，
*   但其他一些值(如 mux_rate、key 等)需要手工设置初始值，否则可能出现异常。
*   debug -- 调试标志
*   nb_streams/streams -- AVStream结构指针数组, 包含了所有内嵌媒体流的描述
* AVPacket -- 暂存解码之前的媒体数据（一个音/视频帧、一个字幕包等）及附加信息（解码时间戳、显示时间戳、时长等),
*   主要用于建立缓冲区并装载数据，
*   data/size/pos -- 数据缓冲区指针、长度和媒体流中的字节偏移量
*   flags -- 标志域的组合，1(AV_PKT_FLAG_KEY)表示该数据是一个关键帧, 2(AV_PKT_FLAG_CORRUPT)表示该数据已经损坏
*   destruct -- 释放数据缓冲区的函数指针，其值可为 [av_destruct_packet]/av_destruct_packet_nofree, 会被 av_free_packet 调用
*
* avformat_find_stream_info -- 获取必要的编解码器参数(如 AVMediaType、CodecID )，设置到 AVFormatContext::streams[i]::codec 中
* av_read_frame -- 从多媒体文件或多媒体流中读取媒体数据，获取的数据由 AVPacket 来存放
* av_seek_frame -- 改变媒体文件的读写指针来实现对媒体文件的随机访问，通常支持基于时间、文件偏移、帧号(AVSEEK_FLAG_FRAME)的随机访问方式，
* AVCodec
*   avcodec_find_decoder/avcodec_find_encoder -- 根据给定的codec id或解码器名称从系统中搜寻并返回一个AVCodec结构的指针
*   avcodec_alloc_context -- 
*   avcodec_open/avcodec_open2/avcodec_close -- 根据给定的 AVCodec 打开对应的Codec，并初始化 AVCodecContext/ 关闭Codec。
*   avcodec_alloc_frame -- 分配编解码时的 AVFrame 结构
*   avcodec_decode_video2 -- 解码一个视频帧，输入数据在AVPacket结构中，输出数据在AVFrame结构中。
*   avcodec_decode_audio4 -- 解码一个音频帧。输入数据在AVPacket结构中，输出数据在AVFrame结构中
*   
* 图像缩放和格式转换(sws_scale，早期是 img_convert)，如 RGB24 <-> YUV420P
*   转换算法的对比：http://www.cnblogs.com/acloud/archive/2011/10/29/sws_scale.html
*     SWS_FAST_BILINEAR 效率效果似乎都不错(默认推荐?); SWS_POINT 缩小时效率很高，但图像效果略差；
*   sws_getContext -- 根据输入、输出色彩空间的参数得到转换上下文
*   sws_scale -- 转换
*   
* 时间
*   设置编码后Packet的pts: pkt.pts = av_rescale_q(c->coded_frame->pts, c->time_base, video_st->time_base); 
*
* 文件操作
*   av_open_input_file -> av_find_stream_info(获取音视频流信息) -> 
*   avformat_write_header -> av_write_frame -> av_write_trailer
*   av_read_frame / av_write_frame
*********************************************************************************************/

/**********************************************************************************************
* MainConcept -- 商业的编解码库，Camtasia 即在使用) http://www.mainconcept.com, 似乎是 DivX 的所有者?
*   TechSmith Screen Capture Codec(C:\Windows\System32\tsccvid.dll)
* 
* ffdshow -- 一套免费的编解码软件，封装成了DirectShow的标准组件。
*   使用 libavcodec library 以及其他各种开放源代码的软件包(如 DivX、)，可支持H.264、FLV、MPEG-4等格式视频
*  
* ffmpeg -- 开源且跨平台的音视频解决方案，是一套编解码的框架，具有采集、解码、流化等功能
*   http://www.ffmpeg.org/ <== 正式官网 (git clone git://source.ffmpeg.org/ffmpeg.git ffmpeg)
*   http://www.ffmpeg.com.cn <== 中文网站，提供编译好的 FFmpeg SDK下载，有 FFmpeg SDK 开发手册
*   http://ffmpeg.zeranoe.com/builds/
*   http://bbs.chinavideo.org/viewthread.php?tid=1897&extra=page=1 <== Windows 下安装和编译
*   http://dranger.com/ffmpeg <== 英文的入门教材，但有些过时
* 模块
*   ffmpeg -i 输入文件 输出文件 -- 视频文件转换命令行工具,也支持经过实时电视卡抓取和编码成视频文件
*     ffmpeg.exe -h 显示详细帮助
*     例子： ffmpeg.exe -i snatch_1.vob -f avi -vcodec mpeg4 -acodec mp3 snatch.avi
*     -acodec codec		<== 指定音频编码，如 aac 
*     -an				<== 禁止audio
*     -aspect 16:9		<== 设置长宽比，如 4:3 或 16:9 等
*     -b <比特率>       <== 设置视频的比特率，分为 -b:a(自动?) 和 -b:v 两种
*     -bf <int>			<== 使用B帧
*     -f format			<== 指定文件格式, 如 avi/mp4 等(可用格式可通过 -formats 查看)
*     -g groupSize		<== 设置GOP大小，如设置成 300 意味着29.97帧频下每10秒就有INTRA帧
*     -qblur <float>	<== 指定视频量化规模，0. ~ 1.0(越小质量越好?)
*     -qscale <float>   <== 0.01~255 ?
*     -s WxH			<== 指定大小(宽x高)
*     -strict <int>     <== 指定对codec等的使用限制，实验性的codec(如mpeg4)默认是不启用的(Codec is experimental)，需要使用 -2 来启用
*     -t 持续时间		<== 指定时间长度，如 20.000 表示 20s
*     -threads <int>    <== 指定线程个数，默认为 auto ?
*     -vcodec codec		<== 指定视频编码，如 mpeg4/h264/libxvid 等(可用编码可通过 -codecs 查看)
*     -target type		<== 指定文件类型，如 vcd/dvd 等，很大程度上可以决定质量
*     -y                <== 直接覆盖输出文件，不再提示
*   ffserver -- 基于HTTP(RTSP正在开发中)用于实时广播的多媒体服务器.也支持时间平移(Time-Shifting)
*   ffplay -- 用 SDL和FFmpeg库开发的一个简单的媒体播放器(需要先安装 SDL 库才能编译)
*   libavcodec -- 包含了所有FFmpeg音视频编解码器的库.为了保证最优性能和高可复用性,大多数编解码器从头开发的
*   libavformat -- 包含了所有的普通音视频格式的解析器和产生器的库
*   libavutil -- 函数库，实现了CRC校验码的产生，128位整数数学，最大公约数，整数开方，整数取对数，内存分配，大端小端格式的转换等功能。
*   libswscale -- 格式转换的库，比如需要将 RGB 转换为 YUV
*   libpostproc -- 
* 
* 编译和调试(无法使用VC编译？)，可以用MSys+MinGW编译，但是编译出来的DLL是可以被VC使用的
*   http://blog.csdn.net/jszj/article/details/4028716  -- Windows 下编译
*   http://www.cnblogs.com/mcodec/articles/1659671.html -- 使用VC编译的方法
*   http://ffmpeg.org/trac/ffmpeg/wiki/How%20to%20setup%20Eclipse%20IDE%20for%20FFmpeg%20development -- 设置eclipse的环境
*     1.从 http://www.eclipse.org/downloads/ 下载 "Eclipse Classic"，或直接下载带 C/C++ 的版本
*     2.在线安装CDT插件: Help -> Install New Software -> http://download.eclipse.org/releases/indigo -> 搜索CDT
*     3.在线安装Git插件(http://www.vogella.de/articles/EGit/article.html), 
*     4.安装MinGW
*     4.Clone FFMpeg 的Git仓库: File->Import->Git->Projects From Git(git://source.ffmpeg.org/ffmpeg.git)
*     5.创建工程: File->New->Project-> C/C++ -> Makefile Project with Existing Code 
* 
*   http://ffmpeg.zeranoe.com/blog/  -- 自动下载和编译ffmpeg的脚本
*   编译选项 -- 注意：编译安装第三方库时，推荐使用 --prefix=/usr(默认是 /usr/local)，这样ffmpeg可以自动找到相关的库(否则需要手工指定)
*     ./configure --enable-w32threads --enable-shared --enable-static --enable-memalign-hack --enable-libx264 
*        --enable-libxvid --enable-gpl --extra-cflags=-I/usr/local/include --extra-ldflags=-L/usr/local/lib
*        && make && make install
*     --prefix=PREFIX           设置安装路径
*     --disable-debug			禁用调试信息,编译发布版本
*     --disable-mmx             
*     --disable-optimizations	禁止优化，一般用于调试版本
*     --disable-static			禁用静态库
*     --disable-stripping       一般用于调试版本
*     --disable-yasm            禁止使用yasm，否则可以安装 yasm后使用，(汇编编译器?)
*     --enable-amr_nb/--enable-amr_wb 支持手机的3gp格式(www.3gpp.org/ftp/Specs/archive/26_series/26.204/26204-510.zip，解压后的文件需要拷贝到 libavcodec/amrwb_float )
*     --enable-avisynth			加入 avisynth(视频文件后期处理工具)
*     --enable-faad				支持 faad2和faac(www.audiocoding.com), ./configure --prefix=/usr --with-mp4v2 --enable-shared
*     --enable-gpl				遵循gpl协议,当使用 x264/xvid 等遵循gpl协议的库时必须指定
*     --enable-libfaac          支持aac(不是免费的),需要先到 faac 目录下编译, ./bootstrap<CR> ./configure xxx<CR> make
*     --enable-libx264			使用x264作为h264编码器,表示要使用第3方库x264,此时mingw include 和 lib内必须已经有x264.h和libx264.a
*                               ftp://ftp.videolan.org/pub/x264/snapshots/ 下载(或者 git clone git://git.videolan.org/x264.git)
*                               然后 ./configure --prefix=/usr --enable-shared --enable-static --enable-win32thread && make && make install
*                               注意：在配置ffmpeg时需要通过 -I 和 -L 指定搜索目录，否则可能会报告"libx264 not found"的问题。
*     --enable-libxvid			启用 Xvid 编码器(www.xvid.org), MPEG-4 video的编解码方案
*                               其 build/win32 目录下有VC的工程, MingGw 下编译有 "process_begin: CreateProcess(NULL" 的错误,原因 ?
*     --enable-libmp3lame		使用mp3lame作为mp3编码器，需要先到 lame 目录下编译（sourceforge.net/projects/lame/files/）
*                               ./configure --enable-shared --prefix=/usr 
*     --enable-memalign-hack    启用内存对齐，Windows下必须
*     --enable-shared			编译动态库
*     --enable-static           编译静态库
*     --enable-small			启用文件最小功能
*     --enable-w32threads       加入多线程支持，会大大提升处理效率
*     --extra-cflags=-O0        额外的编译参数，如 -O0 表示不优化， -I/usr/local/include 表示头文件目录
*     --extra-ldflags=xxx       额外的链接参数，如 -L/usr/local/lib 表示链接目录
*     --enable-pthreads         linux下启用线程?
*  调试
*    av_log_set_callback -- 设置日志的回调函数
*    av_log_set_level -- 设置日志等级(如 AV_LOG_DEBUG )
*    av_dump_format -- 导出 FormatCtx 的信息
* 
*  编解码库（ffmpeg 只是一个框架，不同的编解码库在不同的网站？http://ffmpeg.zeranoe.com/builds/），
*    可通过 --extra-cflags 来指定头文件的文件夹?
*    libavfilter -- 替代libswscale，用于做图像的pixel format转换，基于DirectShow 机制？
*      AVFilterGraph -- 几乎完全等同与directShow中的fitlerGraph
*      AVFilter -- 代表一个Filter
*      AVFilterPad -- 代表一个filter的输入或输出口，等同于DShow中的Pin
*      AVFilterLink -- 代表两个连接的fitler之间的粘合物
*    libx264 
*    libfaac 
*    libmp3lame -- linux 下支持mp3
*    libogg 
*    libspeex 
*    libswscale -- 
*    libvorbis 
*    libtheora  
* 
*
*********************************************************************************************/

/*********************************************************************************************
*  基本概念
*    播放流程: video.avi(Container) -> 打开得到 Video_stream -> 读取Packet -> 解析到 Frame -> 显示Frame
*    Container -- 在音视频中的容器，一般指的是一种特定的文件格式(如 AVI/QT )，里面指明了所包含的音视频，字幕等相关信息
*    Stream -- 媒体流，指时间轴上的一段连续数据，如一段声音、视频或字幕数据
*    Frame -- Stream中的一个数据单元(AVFrame?)
*    Packet -- Stream中的Raw数据,包含了可以被解码成方便我们最后在应用程序中操作的帧的原始数据(注意Packet和Frame不一定相等)
*    CODEC -- 编解码器(Code 和 Decode)，如 Divx和 MP3,以帧为单位实现压缩数据和原始数据之间的相互转换
*     
*
*********************************************************************************************/

/*********************************************************************************************
* 函数说明
*   av_register_all     -- 注册所有容器格式(Format)和CODEC
*   av_open_input_file  -- 打开文件，avformat_close_input_file 关闭
*   av_find_stream_info -- 从文件中提取流信息
*     avformat_find_stream_info ?
*   avcodec_find_decoder-- 查找 decoder /encoder?
*   avcodec_open        -- 打开编解码器，使用完毕后需要通过 avcodec_close 关闭释放
*   avcodec_alloc_frame -- 为解码帧分配内存
*   av_read_frame       -- 从码流中提取出帧数据，一般需要在循环中进行
*   avcodec_decode_video-- 判断帧的类型，对于视频帧调用:


* 
*  源码分析
*    AVPicture
*     +-AVFrame
*    AVPacket -- av_read_frame读取一个包并且把它保存到AVPacket::data指针指向的内存中(ffmpeg分配?)
*    
*    AVFormatContext -- 通过其 ->streams->codec->codec_type 判断类型，如 CODEC_TYPE_VIDEO
*      AVInputFormat
*      AVOutputFormat
*    AVCodecContext -- 编解码器上下文，包含了流中所使用的关于编解码器的所有信息，
*      bits_per_coded_sample -- 
*      codec_name[32]、codec_type、codec_id、codec_tag -- 编解码器的名字、类型、ID、FOURCC 等信息
*      sample_fmt -- 音频的原始采样格式, 是 SampleFormat 枚举
*      time_base -- 采用分数(den/num)保存了帧率的信息,如 25fps(25/1) NTSC的 29.97fps(den/num= (framenum / 17982) / (framenum % 17982)) ?
*    AVCodec -- 通过 avcodec_find_decoder 查找， avcodec_open2 打开
*    SwsContext -- ?
* 
*    Demuxer -- 媒体格式解码器
*    struct AVCodec -- codec 的数据结构，全局变量 *first_avcodec, avcodec_register_all 方法注册全部 codec
*    解码播放：Media -> ByteIOContext -> Demuxer -> Decode -> YUV -> Display
*    格式转换：Media -> ByteIOContext -> Demuxer -> Decode -> YUV -> Encode -> Muxer -> ByteIOContex -> Media
*    格式转换，如 文件类型从 .flv -> mp4
*    编码类型，如 codec 从 mpeg4 -> H264
*    媒体录制：YUV -> Encode -> Muxer -> ByteIOContex -> Media
*
* 编码流程
*   av_register_all();  //注册了所有可用的文件格式和编解码器的库(可以通过 XXXPolicy.xml 文件来指定支持的格式 ?)
*   AVCodec *pCodecH264 = avcodec_find_encoder(CODEC_ID_H264); //查找h264编码器
*   AVCodecContext *pContext = avcodec_alloc_context3(pCodecH264); //获取初始的编码器上下文，后面设置编码器的参数
*   pContext->codec_id = pCodecH264->id; //CODEC_ID_H264
*   pContext->codec_type = AVMEDIA_TYPE_VIDEO;
*   pContext->bit_rate = 3000000; pContext->width=nWidth; pContext->height=nHeight;
*   pContext->time_base.num = 1; pContext->time_base.den = 25; //每秒25帧, 或 den=(int)(UNITS / pVih->AvgTimePerFrame);
*   pContext->gop_size = 12; //每12帧放一个GOP
*   pContext->thread_count = 2; //多线程个数
*   pContext->thread_type = FF_THREAD_SLICE;  //设置使用多个线程来同时编解码一帧
*   pContext->pix_fmt = PIX_FMT_YUV420P;  //设置颜色空间格式?
*   //pContext->flags |= CODEC_FLAG_GLOBAL_HEADER; //mp4,mov,3gp 等设置这个参数?
*   avcodec_open2(pContext, pCodecH264, NULL); //打开编码库，使用完毕后需要 avcodec_close
*   AVFrame *m_pRGBFrame = avcodec_alloc_frame();  //RGB帧数据
*   numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height); //计算需要的空间
*   rgb_buff = av_malloc(numBytes*sizeof(uint8_t));  //分配内存, av_malloc 可以保证内存对齐
*   avpicture_fill((AVPicture*)m_pRGBFrame, (uint8_t*)rgb_buff, PIX_FMT_RGB24, nWidth, nHeight); //填充原始的RGB数据
*  
*   //ffmpeg中大多数的视频格式好像只支持YUV格式的视频帧(AVFrame)，必须先把RGB的视频帧转成YUV视频帧(sws_scale),
*   //  m_pYUVFrame 中 data[0]是亮度,data[1]是色度,data[2] 也是色度?
*   //  但是播放时又需要转换为 RGB 格式 ?
*   SwsContext * scxt = sws_getContext(pContext->width, pContext->height, PIX_FMT_RGB24, 
*      pContext->width, pContext->height, PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL);  //旧的方式是 img_convert ?
*   sws_scale 有问题?( ctx,pFrameRGB->data,pFrameRGB->linesize,0,pCodecCtx->height,pFrame->data,pFrame->linesize);  
*   int iOutSize = avcodec_encode_video(m_pContext, pBufferOut, lOutBufferSize, m_pYuvFrame);//编码
*     //或 avcodec_encode_video2(c, &avpkt, m_pYUVFrame, &got_packet_ptr); 
*     //avcodec_decode_video/avcodec_decode_video2 把包转换为帧，得到完整的一帧时，会设置了帧结束标志frameFinished
*   sws_freeContext(scxt)
*   av_free_packet(&packet);
*    
*
* libavFilter 使用流程
*   AVFilterGraph *graph = avfilter_graph_alloc();
*   AVFilterContext *filt_src = NULL;
*   avfilter_graph_create_filter(&filt_src, &input_filter, "src",NULL, is, graph); //input_filter必须由调用者实现
*   AVFilterContext *filt_out = NULL;
*   avfilter_graph_create_filter(&filt_out, avfilter_get_by_name("buffersink"), "out", NULL, pix_fmts, graph);
*   avfilter_link(filt_src, 0, filt_out, 0); //连接source和sink
*   avfilter_graph_config(graph, NULL); //
*   实现input_filter
*     static AVFilter input_filter = { .name="ffplay_input", .priv_size=sizeof(FilterPriv), .init=input_init, xxx};
*     
*   
*********************************************************************************************/

/*********************************************************************************************
* H264
*   影响编解码效率的维度：
*     1.不同分辨率(Qcif/Cif)
*     2.不同纹理复杂程度 -- 帧内压缩?
*     3.不同运动剧烈程度 -- 帧间压缩?
*
*   各种开源H264编码器评测(http://wenku.baidu.com/view/1d65a3717fd5360cba1adbc1.html)
*     1.JM encoder -- H.264的官方源码(校验模型)。编码复杂度极高，不宜实用
*     2.T264 encoder -- 中国视频编码自由组织联合开发的264编解码器，编码器编码输出标准的264码流(其他解码器能用吗?)
*     3.x264 encoder -- 兼容264标准码流的编码器，注重实用。在不明显降低编码性能的前提下，降低编码的计算复杂度
*           (抛弃了264中一些对编码性能贡献微小但计算复杂度高的特性 -- 如多参考帧、证件预测中不必要的块模式、CABAC等)
*   各种开源H264解码器评测(http://blog.csdn.net/sunshine1314/article/details/712401)
*     解码速度：CoreAVC decoder > ffmpeg > IPP simple player > x264 decoder > t264 decoder > jm86 decoder
*     1.JM decoder -- H.264的官方源码(校验模型),支持特性好，但实用性差
*     2.T264 decoder -- 中国的开源项目，做过汇编优化，解码器只能解T264编码器生成的码流，
*     3.x264 decoder -- (ffmpeg 的早期雏形?)
*     4.ffmpeg libavcodec -- 
*     5.Intel IPP(Integrated Performance Primitives)

*********************************************************************************************/

// 填充RGB然后翻转，好转换为YUV  
#if 0
avpicture_fill((AVPicture*)m_pRGBFrame, (uint8_t*)rgb_buff, PIX_FMT_RGB24, nWidth, nHeight);  
m_pRGBFrame->data[0]  += m_pRGBFrame->linesize[0] * (nHeight - 1);  
m_pRGBFrame->linesize[0] *= -1;                     
m_pRGBFrame->data[1]  += m_pRGBFrame->linesize[1] * (nHeight / 2 - 1);  
m_pRGBFrame->linesize[1] *= -1;  
m_pRGBFrame->data[2]  += m_pRGBFrame->linesize[2] * (nHeight / 2 - 1);  
m_pRGBFrame->linesize[2] *= -1; 
sws_scale(scxt,m_pRGBFrame->data,m_pRGBFrame->linesize,0,c->height,m_pYUVFrame->data,m_pYUVFrame->linesize);
#endif