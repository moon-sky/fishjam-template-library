#pragma once

/***********************************************************************************************
* 视频编码格式 + 封装格式(容器)
*   最流行的高清视频编码格式有：H264/AVC，VC-1，MPEG-2
*   封装格式 -- 将已经编码好的视频文件和音轨文件打包在一起，并按照一定规则建立排序和索引，
*     便于播放器或播放软件来索引播放。常见的封装格式有 MKV、AVI、MOV、TS、PS 等。
*     高清媒体文件的扩展名一般就是媒体的封装格式。
*     TS(Transport Stream) -- 蓝光原盘中采用了TS封装。支持几乎所有编码的高清视频和音轨文件
*     PS(Program Stream) -- 封装的技术基本和 TS 相同，但不是流媒体。不支持流式播放，在原先的HDDVD中使用(标准争霸失败后如何?)
*     MKV(Matroska) -- 一种新兴的多媒体封装格式，可以将各类视频编码、16 条或以上不同格式的音频和语言不同的字幕封装在一个文件内，
*       具有开放源代码、音视频编码丰富、网络亲和性好等优势。正逐渐成为高清视频的主流视频格式。H264 编码与 X264 编码目前大多采用 MKV 封装格式。

*
* 视觉上的彩色可用亮度、色调和饱和度来描述,任何一种彩色光都是这三个特征的综合效果。
*   亮度 -- 光作用于人眼时所引起的 明亮程度 的感觉，与北观察物体的发光强度相关。
*   色调 -- 但人眼看到一种或多种波长的光时所产生的彩色感觉，反映颜色的种类，是决定颜色的基本特性。
*     不透明物体的色调是反射光谱综合作用的效果，透明物体的色调试透过该物体的光谱综合作用的效果。
*   饱和度 -- 颜色的纯度，即掺入白光的程度，或指颜色的深浅程度。对于同一色调的彩色光，饱和度越深，
*     颜色越鲜明(越纯)。如红光加入白光后冲淡为粉红色，饱和度降低。
* 三原色原理 -- 色度学的最基本原理，自然界常见的各种颜色光，都可由红(R)、绿(G)、蓝(B)三种颜色光
*   按不同比例相配置而成。同理，绝大多数颜色光也可以分解成红绿蓝三种色光。注意，三原色的选择不唯一，
*   可以选择其他三种颜色，但要求三种颜色相互独立。人眼对红绿蓝三种色光最敏感。
*
* 色彩空间(Color Space) -- 更详细的信息可以参见 ffmpeg中 PixelFormat 的注释
*   CMYK色彩空间 -- 印刷色彩模式
*   HSL -- 工业界的标准，代表 色调、饱和度、亮度。
*   RGB色彩空间 -- 显示器等要求输入RGB三个彩色分量。不管多媒体系统采用什么形式的彩色空间标识，最后
*     的输出一定要转换成RGB彩色空间表示。AVPicture 中只使用 data[0](指向RGB数据) 和 linesize[0](指向长度) 
*   YUV色彩空间 -- 主要用于电视广播。亮度信号Y、色差信号 R-Y(即U)和B-Y(即V)，通过矩阵变换电路从RGB得到，
*     发送端将亮度和色差三个信号分别进行编码，用同一信道发送出去。即分解成 色度 + 灯光
*     优点：
*       1.可以解决彩电、黑白电视使用同一信号源都能正确显示的问题，且适合于图像处理领域
*       2.而且相对RGB来说更节省空间（如1个RGB24点需要 3x8=24bits ==> YCbCr411 只需 (8 + 8/4 + 8/4) = 12bits，图像数据压缩一半）
*       3.色度频道的采样率可比 Y 频道低，同时不会明显降低视觉质量
*     YUVTools --编辑、查看YUV文件的工具，也提供了一些Sample媒体文件。 http://www.sunrayimage.com/index.html
*     YUV 格式通常有两大类：
*       packed(打包格式) -- YUV分量存放在同一个数组中，通常是几个相邻的像素组成一个宏像素(macro-pixel)，宏像素组的布局取决于格式，
*         如422表示一个宏像素为4个字节，实际表示2个像素
*       planar(平面格式) -- 使用三个数组分开存放YUV三个分量，就像是一个三维平面一样
*     AVPicture 中使用三个通道
*       data[0-2] -- 分别指向YUV分量首行数据的指针，YUV420P时其值分别是 pSrc+0, pSrc+ m_nSrcW*m_nSrcH, pSrc+m_nSrcW*m_nSrcH*5/4;
*       linesize[0-2] -- 分别是YUV分量一行数据的字节长度，linesize[3]无用, YUV420P时其值分别是 m_nSrcW, m_nSrcW/2, m_nSrcW/2
*     YUV420 -- 实质是 420:402 的交叉，对每行扫描线来说，只有一种色度分量以2:1的抽样率存储。进行隔行扫描，相邻的扫描行存储不同的色度分量。
*               色度的抽样率是4:1，通常用于PAL的DV数据中
*     YCbCr 4:2:2 -- 每个色差信道的抽样率是亮度信道的一半
*     YCbCr 4:1:1(YUV411) -- 水平方向上对色度进行4:1抽样, 通常用于NTSC的DV数据，低端用户和消费类产品中。
*           每个点保存一个8bit的亮度值(也就是Y值), 每 2x2 个点保存一个Cr和Cb值
*     YCbCr 4:4:4 -- YUV三个信道的抽样率相同，因此在生成的图像里，每个象素的三个分量信息完整
*     YUV420P -- 通常的视频序列都是YUV420格式
*     YUVJ420P
*     YUV422P -- 每像素16位，
*     YUV444P -- 每像素32位，相对RGB24还增加了8位?
*
*  YUV存储方式的另一个文章( Chroma subsampling -- 色度抽样， 如 4:2:0 )
*     4:4:4 格式，每像素 32 位，打包格式
*       AYUV -- 带有一个Alpha通道，并且为每个像素都提取YUV分量，图像数据格式为 A0 Y0 U0 V0 
*     4:2:2 格式，每像素 16 位，打包格式，每个宏像素都是编码为四个连续字节的两个像素
*       YUY2 -- 用于 Microsoft DirectX Video Acceleration (DirectX VA) 的首选 4:2:2 像素格式
*       UYVY -- 格式与YUY2相同，只是字节顺序相反
*     4:2:0 格式，每像素 16 位，平面格式，色度频道在水平方向和垂直方向上都要以系数 2 来进行再次采样，会出现未采样的区域?
*       IMC1 -- 
*       IMC3 
*     4:2:0 格式，每像素 12 位，平面格式，色度频道在水平方向和垂直方向上都要以系数 2 来进行再次采样
*       NV12 -- 用于DirectX VA的首选4:2:0像素格式。Intel Media SDK 只支持这种格式。
*       IMC2 -- 4:2:0 中的次选像素格式。格式与 IMC1 相同，只是 V (Cr) 和 U (Cb) 行在半跨距边界处进行了交错，
*               能够更加高效地利用地址空间。 色度地址空间缩小了一半，因此整体地址空间缩小了 25%。
*       IMC4 -- 与 IMC2 相同，只是 U (Cb) 和 V (Cr) 行进行了交换
*       YV12 
*  
*  YCbCr视频存储格式(是YUV经过缩放和偏移的版本，主要用于数字视频。Y指亮度分量，Cb指蓝色色度分量，而Cr指红色色度分量)
*    人的肉眼对 Y分量 更敏感，可通过对色度分量进行子采样来减少色度分量，在不较大影响质量的前提下，减少容量。
*    4:2:0 -- 每4个像素有4个亮度分量，2个色度分量 (YYYYCbCr)，仅采样奇数扫描线，是便携式视频设备(MPEG-4)以及电视会议(H.263)最常用格式。
*    4:2:2 -- 每4个像素有4个亮度分量，4个色度分量(YYYYCbCrCbCr)，是DVD、数字电视、HDTV 以及其它消费类视频设备的最常用格式.
*    4:4:4 -- 全像素点阵(YYYYCbCrCbCrCbCrCbCr)，用于高质量视频应用、演播室以及专业视频产品
* 
*  空间转换 -- 是否正确? 
*    RGB <==> YUV444 <==> YUV422 <== YUV420
*
* 流式视频(Streaming Video)格式 -- 采用“边传边播”的方式。
*   很多视频数据要求通过Internet来进行实时传输和播放(比如说在线实况转播)，目前使用较多的是：
*     RM(Real Media)、MOV(QuickTime)、ASF(Advanced Streaming Format)
***********************************************************************************************/

/***********************************************************************************************
* asx -- playlist
* 
* 常用视频格式介绍
*   AVCHD(Advanced Video Codec High Definition，使用M2T？) -- http://www.avchd-info.org/
*     a brand new high definition (HD) digital video camera recorder format recording 
*     1080i and 720p signals onto certain media by using highly efficient codec technologies.
*     索尼和松下于2006年5月11日共同推出的面向高清晰便携摄像机的新记录规格。采用 AVC/H264视频编码格式，
*     记录高清 (HD) 视频。可提供最高达 1080/50i 的视频信号，当进行1080与720的高清摄像时，音频格式则
*     为 AC-3 编码。使用Dolby Digital或线性PCM系统压缩音频数据。 AVCHD 格式拥有非常高效的编码系统，
*     根据拍摄质量设置不同，其拍摄码流率从 5Mbps 到 15Mbps ，即使在 2GB 的记忆棒上，用标准 SP 模式也能拍摄35分钟。
*     目前 AVCHD 支持硬盘、光盘、闪存等(非线性存储)存储介质，而采用 DVD 光盘介质 AVCHD 摄像机拍出的光盘
*     可以直接在蓝光播放机和 PS3 上播放(目录格式采用 BDMV 格式)。
*     在向BD、DVD刻录时规定系统Bitrate的上限：BD--24M bps 以下，DVD--18M bps 以下。
*   AVI(Audio Video Interleave) -- AVI文件标准未规定如何对视频进行捕获、压缩及播放，仅规定了视频和音频在硬盘上，
*     因此可以采用DivX、MP3等多种编码来压缩音视频流。因为当时没有流式播放需要，将索引布置在打包文件的尾部，
*     因此不适合流式播放。只能支持有限的几种音视频编码形式，且AVI不能支持音频的VBR动态比特率编码。
*     问题：
*       1.分析文件的结果和 010Editor 看出来的不一致？AviRiff.h 文件中有定义，查看确认
*       2.交互格式 什么意思(非交互格式时  MainAVIHeader::dwInitialFrames 为0)
*       3.AVI File Header LIST chunk 的大小必须填充到 AVI_HEADERSIZE(2048) ?
*       Remember that avi files do not have a per-frame timestamp, only a fixed frame rate, so on receiving a frame, 
*         the mux has only two choices: place it in the next "slot", or insert "dropped" frame indicators in the next slot(s) 
*         and place it further on. If the frame is not timestamped, or has a timestamp that is in the past, it cannot do anything.
*
*     如何存储(数据帧交错存放)。通常一个AVI文件可以包含多个不同类型的媒体流(一般是一个音频一个视屏)，
*     整个AVI文件的结构为(详情参见vfw.h)：一个RIFF头 + 两个列表(hdrl描述媒体流格式、movi保存媒体流数据) + 一个可选的索引块。
*         hdrl -- header List,描述AVI文件中各个流的格式信息
*           avih(MainAVIHeader) --  主AVI信息头数据，记录AVI文件的全局信息，比如流的数量(dwStreams)、视频图像的宽和高等
*           strl -- Stream List，文件中有几个流，就有几个strl
*             strh(AVIStreamHeader) -- stream Header, 流的头信息数据
*               其中的fccType指定了流的格式(如 "vids"), fccHandler(如"DIB ")
*             strf( BITMAPINFOHEADER 或 WAVEFORMAT|WAVEFORMATEX) -- stream Format, 流的格式信息数据
*             strd -- 可选的额外的头信息数据(StreamHandlerData)，如 保存编解码器需要的一些配置信息
*             strn -- 可选的流的名字(StreamName)
*         movi -- 保存真正的媒体流数据（视频图像帧数据或音频采样数据等）
*           rec -- 
*           可以将数据块直接嵌在"movi"列表里面，也可以将几个数据块分组成一个"rec "列表后再编排进"movi"列表(一组rec相关的数据应该一次性读入内存处理)。
*           数据块使用了四字符码来表征它的类型，格式为：2字节的流编号 + 2字节的类型码，如 00db 表示第一个非压缩视频数据
*             类型码:
*               "db" -- 非压缩视频帧数据(RGB DIB bits)
*               "dc" -- 压缩视频帧(RLE8 compressed DIB bits)
*               "pc"(AVIPALCHANGE) -- 改用新的调色板(Palette Change), 在AVI数据序列中间可以定义新的调色板
*               "wb" -- 音频数据(Wave Bytes)
*               "??" -- 文字流数据块
*         idx1(AVIINDEXENTRY) -- 可选的AVI索引块数据，为AVI文件中每一个媒体数据块进行索引，并且记录它们在文件中的偏移
*       TODO: 流类型的 FOURCC(在什么地方?)
*         vids -- Video Stream
*         auds -- Audio Stream
*         mids -- MIDI Stream
*         txts -- Text Stream
*         JUNK -- 表示特殊的数据块，用于内部数据的队齐（填充），应用程序应该忽略这些数据块的实际意义
*         INFO -- 字符串信息? 其下有 ICMT 块(比如 野生动物.wmv 转出来的avi文件)
*   BDAV
*   Div3~Div5 -- DivX 3.11是黑客破解MS MPEG4后编写的，
*   Divx -- 也称为DVDrip，DivX公司具有专利的一种MPEG-4技术视频编解码器(codec),只有免费（不是自由）的版本和商用版本，
*     其最初的开发者(DXN)利用LGPL协议窃取了开源社区的贡献，因此开源社区在其最后一个版本基础上开发出了 XVid。
*     综合了MPEG-4与MP3各方面的技术，压缩后将视频与音频合并并加上相应的外挂字幕文件而形成的视频格式，画质直逼DVD，
*     但体积只有DVD的几分之一，对机器要求也不高，号称DVD杀手。
*   H.26x -- 由ITU-T国际组织发布，有多种标准体系：
*     H.261 -- 满足ISDN视频服务)、
*     H.263 -- 为低码流通信设计，满足局域网、无线网络视频服务)，用于取代H.261(在其基础上做了一些改善)
*     H.263+ -- H.263建议的第二版，进一步提高了压缩编码性能，并改进了可扩展性(允许多显示率、多速率及多分辨率），
*       增强了视频信息在易误码、易丢包异构网络环境下的传输)
*     H.264 -- 
*     数据流具备严格的语法规则，各标准之间具有兼容性，是一个开放体系，可以对其进行改进，提高其性能。
*   H.264/MPEG-4 AVC（Advanced Video Coding) -- 由ITU-T与ISO/IEC联合进行开发的视频编解码方案，通常音频使用AAC,
*     是MPEG-4标准的扩展(采用更高压缩率)。H.264影片的“容器”可以是AVI，MP4，MPG等(如MOV更改扩展名后可以播放)。
*     相对于先期的视频压缩标准，引入了很多先进的技术，包括4x4证书变换、空域内的帧内预测等。
*     问题：
*       1.有很多种SubType：MEDIASUBTYPE_AVC1、MEDIASUBTYPE_H264、MEDIASUBTYPE_h264、MEDIASUBTYPE_X264、MEDIASUBTYPE_x264 等
*         http://technet.microsoft.com/zh-cn/library/dd757808
*   HDV -- 以Mini DV磁带为存储介质
*   HDV 1080i
*   HEVC -- 下一代HDTV上的格式?也就是H.265?支持 QVGA(320x240)到Ultra HDTV 4320p(7680x4320).
*   MICROMV
*   MOV(AppleQuickTime) -- 是一种流文件格式，  +QuickTime Viewer(浏览器Plugin，播放)，
*     提供了自动速率选择功能（能自己选择不同的链接速率下载并播放影像），但过于封闭。
*   MPEG(Moving Pictures Experts Group)--动态图象专家组，专门负责指定视频和音频标准。其制定了MPEG-1、MPEG-2、
*     MPEG-4、MPEG-7 等标准(注：MPEG-3是为HDTV设计的，但MPEG-2已经能支持，所以被放弃了)。
*     MPEG的兼容性好，压缩比高(200:1)，音视频失真小。
*   MPEG-1(动态图象和伴音的编码) -- 1992年为工业标准而设计。适用于CD-ROM、Video-CD等，针对SIF标准的分辩率
*     (NTSC为 352x240 pixel，PAL为352x288 pixel)的图象进行压缩。也用于数字电话网络上的视频传输(如ADSL、VOD等)。
*   MPEG-2(运动图象及其伴音的编码)--由MPEG工作组1994年发布的视频和音频压缩国际标准，向后兼容MPEG1,
*     通常用来为广播信号提供视频和音频编码(用于数字电视)。经少量修改后，也成为DVD产品的核心技术，也应用于HDTV高清电视传输。
*     NTSC制式下分辩率可达720x486，一个MPEG-2系统流一般包括两个基本元素：视频数据 + 时间戳 和 音频数据 + 时间戳。
*     MPEG-2视频通常包含多个GOP。支持隔行扫描和逐行扫描。
*   MPEG2-PS(Program Stream)--程序流，存储具有固定时长的节目，如DVD电影，可添加字幕等一些程序操作，SD使用MPEG2-PS格式？
*   MPEG2-TS(Transport Stream)--传输流，主要应用于实时传送的节目，比如实时广播的电视节目。从视频流的
*     任一片段开始都是可以独立解码的。（还可细分为 HDV、MMV、AVC，AVCHD和BDAV使用TS_AVC格式？)
*   MPEG-4 -- 由MPEG工作组1998年发布的用于音频、视频信息的压缩编码标准。包含了MPEG-1及MPEG-2的绝大部份功能。
*     主要应用于视频电话(Video Phone)等。是★第一个★具有交互性的动态图象标准，更适于交互式音视频服务及远程监控。
*     是一个公开的平台，各公司、机构均可以根据MPEG-4标准开发不同的制式，因此出现了WMV 9、Quick Time、DivX、Xvid 等基于MPEG-4技术的视频格式。
*     有配置(profile)和级别(level)的概念，定义了MPEG-4应用于不同平台时的功能集合。
*     ？MS 开发的 MPEG-4 Codec(MS MPEG4 V1、MS MPEG4 V2、MS MPEG4 V3）-- V3画质最好
*   MPEG-7(Multimedia Content Description Interface) -- 多媒体内容描述接口，可以对各种不同类型的多媒体信息进行标准化的描述，并将该描述与所描述的内容相
*     联系，以实现快速有效的搜索。
*   NTSC/PAL -- 非HD素材，NTSC制式，Height / Width = 720 / 480, Framerate / FramerateBase = 30000 / 1001; 
*                         PAL 制式, Height / Width = 720 / 576, Framerate / FramerateBase = 25000 / 1000
*   OGG/OGM -- 
*   VC-1 -- 微软基于 WMV-9 编码技术，在2003年提出的编码算法，2006年成为国际标准，吸收了MPEG-2和H264编码的优点，
*     编解码复杂性仅有H264的一半(压缩时间更短)，得到BD蓝光、SMPTE等的支持。
*     微软并建立了 WMV-HD 高清资源站点，大量发布采用 VC-1 编码压制的 720P、1080P 宣传片及演示片。
*     但因为 中庸的技术(与H264相比无明显编码优势)、封闭平台(仅限Windows)、后发(2006年才成为国标),其应用前景不确定。
*   X264 -- H264编码的一个开源分支，符合H264标准，其功能在于编码，不作为解码器使用。
*     最大特点是注重实用，在不明显降低编码性能的前提下，努力降低编码的计算复杂度，X264 对于 H264 编码中的
*     一些复杂编码特性做了折衷处理，压缩体积小，画质高，得到广泛认可。
*   Xvid -- 一个开放源代码(GPL V2)的MPEG-4视频编解码器,主要竞争对手是DivX，官方站点只提供源代码下载。
*     Xvid提供了一些配置方案(Profile)来对编码复杂度进行一定的限制，如 Simple Profile(SP)、
*     Advanced Simple Profile(ASP)、Unrestricted等
***********************************************************************************************/

/***********************************************************************************************
* 视频的字符叠加
*   一般每个非压缩格式的视频Sample都是一帧图象数据，将指定位置的图象像素值替换为字符图象的像素值。
*   高效做法是：
*     内存中建立二色位图，在位图上使用GDI画出字符内容 -- 获得文字的点阵信息(0表背景，1表有字)，
***********************************************************************************************/

/***********************************************************************************************
* 媒体文件转化时的大小估计
*   1.MPEG -- (Video+ Audio)*(2048/2012)*(秒数/8)
*     说明：2048/2012=计算系统控制流本身的位速率，如果使用TMPGEnc上，需要修正为 (2048/2018)
*     如：视频1150Kbps,声音224Kbps，15秒的MPEG文件。(1150+224)*(2048/2012)*(15/8)=2614KB
*     参考页：https://secure.pegasys-inc.com/faq/view.php?lang=ja&faqid=33
***********************************************************************************************/

/***********************************************************************************************
//将32位BMP中的每一位都设置ALPHA值
for(int y=0; y < height; y++)
{
    for ( int x = 0; x < width; x++)
    {
        ((UINT32*)pvBits)[x + y * width] |= 0xFF000000L; 
    }
}
***********************************************************************************************/

/***********************************************************************************************
*
* Video 媒体类型(AM_MEDIA_TYPE::pbFormat,将Video的每一帧处理为一个BMP图片？) 
*   其到 bmiHeader 部分也被定义为 VIDEOINFOHEADER ，FORMAT_VideoInfo/FORMAT_MPEGVideo 时会采用这种格式
*   typedef struct tagVIDEOINFO {
*     RECT rcSource;        // Portion of the input video to use
*     RECT rcTarget;        // Where the video should be displayed
*     DWORD dwBitRate;      // Approximate data rate in bits per second
*     DWORD dwBitErrorRate; // Bit error rate for this stream
*     REFERENCE_TIME AvgTimePerFrame;   // The desired average time per frame, in 100-nanosecond units
*     BITMAPINFOHEADER bmiHeader;  //其中一帧的位图信息
*     union {
*       RGBQUAD bmiColors[iPALETTE_COLORS]; // 256个Colour palette
*       DWORD dwBitMasks[iMASK_COLORS];     // 3个true-color bitmasks
*       TRUECOLORINFO TrueColorInfo;        // Both of the above
*     };
*   } VIDEOINFO; //注意：VIDEOINFOHEADER 就是 VIDEOINFO 结构中包括 bmiHeader 在内的前面的结构(即不包含union部分)
*   
* 媒体数据(IMediaSample::GetPointer)
***********************************************************************************************/

