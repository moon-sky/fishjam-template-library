#ifndef FTL_MULTI_MEDIA_H
#define FTL_MULTI_MEDIA_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlMultiMedia.h requires ftlbase.h to be included first
#endif

/*********************************************************************************************
* 为了使用OEM资源(如 LoadOEMIcon等)，需要在 stdafx.h 的开始处 #define OEMRESOURCE
* 
* BMP(Bitmap) -- 位图(光栅图象),BMP 文件通常不压缩，因此不太适合通过 Internet 传输。
* Exif(Exchangeable Image File) -- 可交换图像文件，适用于数码相机拍摄的照片的文件格式。
*   含有根据 JPEG 规格压缩的图像 和 拍摄日期、快门速度、曝光时间、相机型号 等照片和相机的信息。
* Gif(Graphics Interchange Format) -- 用于在 Web 页中显示图像的通用格式,适用于在颜色之间有清晰边界的图片。
*   采用无损压缩。可指定一种颜色为透明色(图像将具有显示它的任何 Web 页的背景色)。
*   在单个文件中存储一系列 GIF 图像可以形成一个动画 GIF，但每像素最多能存储8位，只能显示256种颜色。
* ICON() -- 图标是大小固定的小方型图像，且每一个图标资源中可含为多种显示设备和用途准备的具有不同大小和位数的多个图像。
*   图标由一个图片和一个用于产生透明区的掩膜组成，标准的图标为32×32像素的256色图像，扩展图标为 256×256像素的32位色。
* JPEG(Joint Photographic Experts Group) -- 联合图像专家组，将人眼难以察觉的(高频?)信息在压缩中丢弃，
*   可控制压缩级别，不支持透明或动画，JPEG 压缩不适用于线条图形、纯色块和清晰边界(边界处会模糊)。
*   JPEG 是一种压缩方案，不是一种文件格式。JPEG 文件交换格式 (JFIF)是一种文件格式，常用于存储和传输已根据JPEG方案压缩的图像。
* PNG(Portable Network Graphics) -- 可移植网络图形。从GIF扩展而来，无损压缩，可存储24位真彩，
*   支持Alpha通道，能够逐渐显示一副图像(网络显示时越来越清晰)，可包含伽玛校正和颜色校正信息，以便图像可在各种各样的显示设备上精确地呈现。
* TIFF(Tagged Image File Format) -- 标签图像文件格式，是一种灵活的和可扩展的格式，能以每像素任意位来存储图像，
*   并可以使用各种各样的压缩算法。单个的多页 TIFF 文件可以存储数幅图像，可存储 制造商、主机、压缩类型 等元数据。
*   可以把与图像相关的信息(如制造商、打印方向等)存储在文件中并使用标签来排列这些信息，可以根据需要通过批准和添加新标签来扩展 TIFF 格式。
*
* MetaFile -- 矢量图形,存储为一系列绘图命令和设置的图像，其命令和设置可以存储在内存中或保存到文件或流。
*   WMF(Windows MetaFile)--
*   EMF(Enhanced MetaFile) -- 增强型元文件
*   EMF+ -- EMF 的扩展，可存储GDI+记录
*********************************************************************************************/

/*********************************************************************************************
* 位图--点阵图或光栅图,由像素阵列组成
*   设备相关位图(DDB--Device Dependent Bitmap) -- 图像的显示(CBitmap),依赖于硬件的调色板,使用:LoadBitmap
*   设备无关位图(DIB--Device Independent Bitmap) -- 图像的存储(CImage),自己带有颜色信息，颜色模式与设备无关。
*     是一种文件格式，使用:CImage 或 LoadImage+FromHandle。 256级以下的DIB有自己的颜色表，像素的颜色独立于系统调色板。
*     DIB所包含的图像信息主要有：颜色格式、分辨率、调色板和压缩标志.
*     对应的结构 -- BITMAPFILEHEADER、BITMAPINFO、BITMAPINFOHEADER
*     DIB的输出？：SetDIBitsToDevice/StetchDIBits 等，GDI函数会把DIB转换成DDB ？
*   颜色深度(BPP -- bits-per-pixel) -- 位图的每个像素用若干二进制位表示，二进制位的个数称为位图的颜色深度，
*     常见的颜色深度为 1,2,4,8,15,16,24(全彩),32(带Alpha通道)。
*   低于或等于8位（256色）的位图，都为索引色，保存调色板中的颜色表项的索引值,需要调色板。
* 图标 -- 大小固定的方型小位图，在一个程序的图标资源中，一般包含多个不同大小和位数的图像
* 
* GetBitmapBits
* BITMAP 结构中：
*   bmWidth -- 宽度,像素个数(设备坐标?), 必须>0
*   bmHeight -- 高度,像素个数(设备坐标?), 必须>0
*   bmWidthBytes -- 每条扫描线(即每行像素)的字节数,必须能被2(4?)整除（即每行字对齐，不足的位补0）
*   bmPlanes -- 位面数, = 颜色深度?
*   bmBitsPixel -- 颜色深度
*   bmBits -- 指向存放位图数据的字符(字节)数组的指针
*
*********************************************************************************************/

/*********************************************************************************************
* DIB 位图文件信息 -- BMP文件由文件头、位图信息头、颜色信息(如依赖调色板时才存在?)和位图数据四部分组成。
*   DIB 是标准的 Windows 位图结构,BMP文件包含有一个DIB。 
*   1.BMP文件头数据结构含有BMP文件的类型、文件大小和位图起始位置等信息。 
*     typedef struct tagBITMAPFILEHEADER
*     {
*       WORD  bfType;       //位图文件的类型，必须为'BM'
*       DWORD bfSize;       //位图文件的大小，以字节为单位，包括BITMAPFILEHEADER的14个字节
*       WORD  bfReserved1;  //位图文件保留字，必须为0
*       WORD  bfReserved2;  //位图文件保留字，必须为0
*       DWORD bfOffBits;    //位图数据的起始位置，以相对于位图文件头的偏移量表示，以字节为单位，
*                           //可用来计算 BitMapInfoHeader 结构和后面的颜色表组合起来的尺寸
*     } BITMAPFILEHEADER;
*     
*   2.BMP位图信息头数据用于说明设备无关位图(DIB)的尺寸和颜色格式等信息。
*       更新的系统应该用 BITMAPV4HEADER / BITMAPV5HEADER。
*     typedef struct tagBITMAPINFOHEADER{
*       DWORD biSize;       // 本结构所占用字节数
*       LONG  biWidth;      // 位图的宽度，以像素为单位，如果biCompression是BI_JPEG或BI_PNG，表示解压后的宽度。
*       LONG  biHeight;     // 位图的高度，以像素为单位
*         //如果是正数，位图是左下到右上；
*         //如果是负数(此时不能压缩)，位图是左上到右下。
*         //如果biCompression是BI_JPEG或BI_PNG，表示解压后的高度。
*       WORD  biPlanes;     // 目标设备的平面个数，必须为1
*       WORD  biBitCount;   // 每个像素所需的位数，必须是以下数据之一：
*         //0(JPEG或PNG格式)
*         //1(RGB1,单色图),
*         //4(RGB4,16色),
*         //8(RGB8,256色),
*         //16(RGB565，全局变量bits565，掩码为0x7C00|0x03E0|0x001F
*         //   RGB555,全局变量bits555，掩码为 0xF800|0x07E0|0x001F)
*         //24(RGB24，不压缩)
*         //32（RGB32，掩码为 0x00FF0000|0x0000FF00|0x000000FF）之一。
*       DWORD biCompression;// 位图压缩类型，和像素位数相关。如24或32位的像素不需要压缩。必须是以下数据之一：
*         //BI_RGB(不压缩)
*         //BI_RLE8(压缩)，BI_RLE4(压缩)
*         //BI_BITFIELDS(不压缩，颜色表是Color-mask)之一。
*         //BI_JPEG、BI_PNG （表明是 JPEG 或 PNG）
*       DWORD biSizeImage;  //位图的大小(仅用于压缩，BI_RGB格式时为0)，以字节为单位。
*         //实际的位图数据占用的字节数 biSizeImage=biWidth' * biHeight，
*         //  biWidth' 是biWidth 按照4的整倍数调整后的结果(进位？)
*       LONG  biXPelsPerMeter;  // 目标设备的水平分辨率，像素个数/米 -- MulDiv(PerMeter, 254, 10000) = 96 = GetDeviceCaps(hdc, LOGPIXELSX)
*       LONG  biYPelsPerMeter;  // 目标设备的垂直分辨率，像素个数/米
*       DWORD biClrUsed;    // 位图实际使用的颜色表中的颜色数。
*         //0表示根据biBitCount确认的最大个数
*         //如果biBitCount 小于16，biClrUsed 表明使用的颜色表个数；
*         //如果biBitCount 大于等于16，biClrUsed表示为了优化性能使用的系统调色表个数；
*       DWORD biClrImportant;   //位图中重要的颜色数，0表示所有颜色都重要
*     } BITMAPINFOHEADER;
*       
*   3.颜色表 -- 在需要单色，16色，256色的情况下使用，对于不以调色板方式存储的位图，则无此项信息。
*     用于说明位图中的颜色，共有biClrUsed个表项，每一个表项是一个RGBQUAD类型的结构，定义一种颜色。
*     RGBQUAD结构的定义如下: 
*     typedef struct tagRGBQUAD {
*       BYTErgbBlue;    // 蓝色的亮度(值范围为0-255)
*       BYTErgbGreen;   // 绿色的亮度(值范围为0-255)
*       BYTErgbRed;     // 红色的亮度(值范围为0-255)
*       BYTErgbReserved;// 保留，必须为0 -- 32位真彩时表示图像透明度信息的Alpha通道，0表全透明，255表不透明
*     } RGBQUAD;
*     颜色表中RGBQUAD结构数据的个数有biBitCount来确定:
*       当biBitCount=1,4,8时，分别有2,16,256个表项;
*       当biBitCount=24,32时，没有颜色表项。
*     位图信息头和颜色表组成DIB位图的完整信息，BITMAPINFO结构定义如下:
*     typedef struct tagBITMAPINFO {
*       BITMAPINFOHEADER bmiHeader; // 位图信息头
*       RGBQUAD bmiColors[1]; // 颜色表
*       //获取颜色表指针：pColor = ((BYTE*)pBitmapInfo + (WORD)(pBitmapInfo->bmiHeader.biSize));
*     } BITMAPINFO;
*   4.位图数据 ImageDate（二维数组）
*     对于用到调色板的位图，实际的图象数据ImageDate为该象素的颜色在调色板中的索引值；
*     对于真彩色图，图象数据则为实际的R、G、B值。
*     位图数据每一行的字节数必须为4的整倍数，如果不是，则需要补齐。 
*       m_DIBWidthInBytes = ((m_Size.cx+31)>>3) & ~3; 或者：(m_Size.cx + 31)/32 * 4
*     在GDI环境中，位图文件中的数据是从下到上、从左到右方式存储的(图像倒过来?) -- biHeight > 0；
*     在DirectDraw环境中，位图文件中的数据是从上到下、从左到右方式存储的 -- biHeight < 0；
*     根据需要绘制的点的位置获取在位图数组中的偏移量
*       BYTE *pData = pFrame + ( m_y * m_iImageWidth * iPixelSize) + m_x * iPixelSize; //其中iPixelSize是每一个像素的字节数(1~4)
*
**********************************************************************************************/

/*********************************************************************************************
* 图像处理原理：
*   DIB/DDB -- 是针对使用调色板(4、8位色)的系统来说的，在16、24、32位颜色时，存储的是实际的颜色，都是设备无关的
*   逻辑调色板(CDC中)修改后，需要 realize 使最终颜色映射到系统调色板(Windows中)，才能生效产生效果
*
* 图像透明  -- (? TrueColor + Alpha, 共 32 bit )
*    支持的文件件格式：CUR、GIF(256色)、ICO、PNG(推荐，从GIF扩展而来?)
*********************************************************************************************/

/*********************************************************************************************
* COM 中使用 IPicture 管理图片，可以使用 CPictureHolder 的辅助类
*   hr=OleLoadPicturePath(T2OLE(szPath),NULL,0,RGB(255,255,255),IID_IPicture,(LPVOID*)&pPicture);
*********************************************************************************************/


/*********************************************************************************************
* 图形库：
*   CxImage -- 开源、完全免费的图像操作类库，可快捷地存取、显示、转换各种图像。
*     CxImage的变量中，pDib(背景图像)，pAlpha(透明层)，pSelection(被选中的层)，pLayers(可扩展的通用层)
*     ximacfg.h 文件中通过 CXIMAGE_SUPPORT_XXX 的宏可以控制CxImage库支持那些图像格式
*     CxImage -- 图像的基类，有各种格式图像的子类；CxFile <== CxIOFile/CxMemFile；CImageIterator
*       CxImageICO -- 支持对VistaPNG图标的读写
*       CxImageBMP -- 支持对32位图像的读写(支持Alpha通道)
*       CxImageJPG -- 提供JPEG图像格式的二次采样，如 Encode_SubSample_422等
*     编译问题：
*       1.702 -- 会报告 CCM_SETWINDOWTHEME 没有定义的错误，为什么？
*         自定义一个并且把stdafx.h中的WINVER等宏定义注释就编译OK了
*   OpenIL 
*   FreeImage
*   PaintLib
*   GdiPlus
*
* CMemDC -- Keith Rule 所开发的内存DC，可以大幅加快绘图的效率，TextProgressCtrl.cpp 中使用的已经是较老的版本
*********************************************************************************************/

/*********************************************************************************************
* ffdshow -- 一套免费的编解码软件，封装成了DirectShow的标准组件。
*   使用 libavcodec library 以及其他各种开放源代码的软件包(如 DivX、)，可支持H.264、FLV、MPEG-4等格式视频
*  
* ffmpeg -- 开源且跨平台的音视频解决方案，是一套编解码的框架，具有采集、解码、流化等功能（http://www.ffmpeg.org/）
*   http://www.ffmpeg.com.cn, http://ffmpeg.zeranoe.com/builds/
* 模块
*   ffmpeg -i 输入文件 输出文件 -- 视频文件转换命令行工具,也支持经过实时电视卡抓取和编码成视频文件
*   ffserver -- 基于HTTP(RTSP正在开发中)用于实时广播的多媒体服务器.也支持时间平移(Time-Shifting)
*   ffplay -- 用 SDL和FFmpeg库开发的一个简单的媒体播放器(需要先安装 SDL 库才能编译)
*   libavcodec -- 包含了所有FFmpeg音视频编解码器的库.为了保证最优性能和高可复用性,大多数编解码器从头开发的
*   libavformat -- 包含了所有的普通音视格式的解析器和产生器的库
* 
* 编译和调试(无法使用VC编译？)，可以用MSys+MinGW编译，但是编译出来的DLL是可以被VC使用的
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
*   编译选项
*     ./configure --enable-shared --disable-static --enable-memalign-hack --disable-yasm
*     --disable-debug			禁用调试信息,编译发布版本
*     --disable-optimizations	
*     --enable-shared			编译动态库
*     --disable-static			禁用静态库
*     --enable-gpl				遵循gpl协议,必须指定，因为x264是gpl协议
*     --enable-libfaac     
*     --enable-libx264			使用x264作为h264编码器,表示要使用第3方库x264,此时mingw include 和 lib内必须已经有x264.h和libx264.a
*     --enable-libmp3lame		使用mp3lame作为mp3编码器（http://sourceforge.net/projects/lame/files/）
*     --enable-memalign-hack    启用内存对其，Windows下必须
*     --enable-small			启用文件最小功能
*  调试
*    av_log_set_callback
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
*  源码分析
*    Demuxer -- 媒体格式解码器
*    struct AVCodec -- codec 的数据结构，全局变量 *first_avcodec, avcodec_register_all 方法注册全部 codec
*    解码播放：Media -> ByteIOContext -> Demuxer -> Decode -> YUV -> Display
*    格式转换：Media -> ByteIOContext -> Demuxer -> Decode -> YUV -> Encode -> Muxer -> ByteIOContex -> Media
*    格式转换，如 文件类型从 .flv -> mp4
*    编码类型，如 codec 从 mpeg4 -> H264
*    媒体录制：YUV -> Encode -> Muxer -> ByteIOContex -> Media
*
* 编码流程
*   av_register_all();
*   avcodec_register_all();
*   AVFrame *m_pRGBFrame = avcodec_alloc_frame();  //RGB帧数据
*   AVCodec *pCodecH264 = avcodec_find_encoder(CODEC_ID_H264); //查找h264编码器
*   AVCodecContext *pContext = avcodec_alloc_context3(pCodecH264); //获取初始的编码器上下文，后面可以更改
*   pContext->bit_rate = 3000000; pContext->width=nWidth;
*   AVRational rate; rate.num = 1; rate.den = 25;  pContext->time_base = rate; //每秒25帧
*   pContext->gop_size = 10; //每10帧放一个GOP
*   pContext->thread_count = 1; //多线程个数(TODO?有什么限制)
*   avcodec_open2(pContext, pCodecH264, NULL); //打开编码库
*   SwsContext * scxt = sws_getContext(pContext->width, pContext->height, PIX_FMT_BGR24, 
*      pContext->width, pContext->height, PIX_FMT_YUV420P, SWS_POINT, NULL, NULL, NULL);
*   avpicture_fill((AVPicture*)m_pRGBFrame, (uint8_t*)rgb_buff, PIX_FMT_RGB24, nWidth, nHeight); 
*   avcodec_encode_video2(c, &avpkt, m_pYUVFrame, &got_packet_ptr); //编码
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


namespace FTL
{
}//namespace FTL

#endif //FTL_FILE_H

#ifndef USE_EXPORT
#  include "ftlMultiMedia.hpp"
#endif