#ifndef FTL_MULTI_MEDIA_H
#define FTL_MULTI_MEDIA_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlMultiMedia.h requires ftlbase.h to be included first
#endif

//媒体格式检查工具
//  mediaInfo -- http://mediainfo.sourceforge.net/
//  KMP 等播放器自带
//  Fresh View ?

/*********************************************************************************************
* 远程监控和屏幕录制时，只传输屏幕变化的部分：
*   1.驱动级的 Mirror Driver(Tight VNC) -> 核心模式gdi。
*     驱动截屏技术是一种标准技术，为微软公司所推荐，直接从其管理的DIB位图（表面）中将截取区域的图形数据拷贝到应用程序，显著的降低了一次截屏的时间消耗。
*     缺陷:需要安装驱动程序，每次截屏时候屏幕会出现明显的刷新
*   2.GDI+下的算法方式
*     a.分块异或屏传(灰鸽子屏幕传输) -- 前后保存两次bmp位图，把屏幕分成若干块并编号，前后两副位图分别按对应编号块逐个像素点做异或(XOR)操作，
*       若异或后的结果全是零,证明两个被分块的位图相等，不为零则两个被分块的位图不相等，
*       不相等则把异或的结果进行压缩，并发送。
*     b.固定块隔行扫描(Radmin影子远程控制系统)--前后保存两次bmp位图，把屏幕分成若干块并编号，前后两副位图分别按对应编号块对比，
*       隔若干行(一般是10行)对比前后两幅图像的一行中的像素点是否相同,若不同则压缩发送当前块中的图像。
*       为避免每次扫描固定的行，每次重新扫描时会对上次扫描的行编号+n行(百叶窗技术)
*     c.动态分块隔行扫描(Radmin的优化)--由屏幕的变化区域动态确定要发送的矩形
*     d.热点追踪(动态分块隔行扫描 + 跟踪鼠标的操作)
*   3.RDP传输协议(远程桌面)
* 压缩解压缩(TSP木马选用的LZW) -- JPEG、Huffman、RLE(Run Length Encode)、LZW等
*********************************************************************************************/

/*********************************************************************************************
* 为了使用OEM资源(如 LoadOEMIcon等)，需要在 stdafx.h 的开始处 #define OEMRESOURCE
* 
* BMP(Bitmap) -- 位图(光栅图象),BMP 文件通常不压缩，因此不太适合通过 Internet 传输。
* Exif(Exchangeable Image File) -- 可交换图像文件，适用于数码相机拍摄的照片的文件格式。
*   含有根据 JPEG 规格压缩的图像 和 拍摄日期、快门速度、曝光时间、相机型号 等照片和相机的信息。
* Gif(Graphics Interchange Format) -- 版权所有，使用时需要授权(已于Xx年过期?)。用于在 Web 页中显示图像的通用格式,适用于在颜色之间有清晰边界的图片。
*   采用无损压缩。可指定一种颜色为透明色(图像将具有显示它的任何 Web 页的背景色)。
*   在单个文件中存储一系列 GIF 图像可以形成一个动画 GIF，基于颜色列表，图像数据是对应于颜色列表的索引值，每像素最多能存储8位(256色)。
*   内部分成许多存储块，用来存储多幅图像或是决定图像表现行为的控制块。通过 LZW 压缩算法压缩图像数据。
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
* DIB(Device-independent bitmap),设备无关位图，可通过API或直接更改数据位进行更改
*   位图文件信息 -- BMP文件由文件头、位图信息头、颜色信息(如依赖调色板时才存在?)和位图数据四部分组成。
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
* DFB(Device-format bitmap) -- 设备格式位图  == DDB(Device-dependent bitmap) -- 设备相关位图。在其上绘图支持硬件加速
*   由硬件设备管理的位图，制造商可以以他希望的方式管理图像，其格式由设备制造商确定。通常必须通过API访问(不能直接更改图像的位)
*   即 CreateBitmapXxx 创建的位图？
**********************************************************************************************/

/*********************************************************************************************
* 图像处理原理：
*   DIB/DDB -- 是针对使用调色板(4、8位色)的系统来说的，在16、24、32位颜色时，存储的是实际的颜色，都是设备无关的
*   逻辑调色板(CDC中)修改后，需要 realize 使最终颜色映射到系统调色板(Windows中)，才能生效产生效果
*   获取系统调色板的信息：
*     PALETTEENTRY m_Palette[iPALETTE_COLORS];
*     GetSystemPaletteEntries(hdc, 0, iPALETTE_COLORS, (LPPALETTEENTRY) &m_Palette);
* 
* 图像透明  -- (? TrueColor + Alpha, 共 32 bit )
*    支持的文件件格式：CUR、GIF(256色)、ICO、PNG(推荐，从GIF扩展而来?)
*********************************************************************************************/

/*********************************************************************************************
* COM 中使用 IPicture 管理图片，可以使用 CPictureHolder 的辅助类
*   从文件加载：hr = OleLoadPicturePath(T2OLE(szPath),NULL,0,RGB(255,255,255),IID_IPicture,(LPVOID*)&pPicture);
*   从内存加载：hr = OleLoadPicture(pStream, nSize, FALSE, IID_IPicture, (LPVOID *)&m_pPicture));
*   绘制:
*     pPicture->get_Width(&hmWidth);
*     int nWidth  = MulDiv(hmWidth, pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_PER_INCH); //MAP_LOGHIM_TO_PIX
*     int nHeight = MulDiv(hmHeight,pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_PER_INCH);
*     hr = pPicture->Render(dcMem, 0, 0, nWidth, nHeight, 0, hmHeight, hmWidth, -hmHeight, NULL);
*********************************************************************************************/


/*********************************************************************************************
* 图形库：
*   CxImage -- 开源、完全免费的图像操作类库，可快捷地存取、显示、转换各种图像。
*              支持图像的多重操作(线性滤波、中值滤波、直方图操作、旋转缩放、区域选取、阈值处理、alpha混合等)
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
* 
* CMemDC -- Keith Rule 所开发的内存DC，可以大幅加快绘图的效率，TextProgressCtrl.cpp 中使用的已经是较老的版本
*********************************************************************************************/

/*********************************************************************************************
* giflib -- 查看、生成gif的开源库(gif版本为 87a)
*   Gif有多个版本(87a,89a 等，参见 gif89.txt)；可存放一帧或多帧图像数据，并可以存放图像控制信息(动画)，其图像基于调色板，最多只能有255种颜色。
*   格式:文件头(Header) + 数据部分( 控制标识符[Control Block] + [图像数据块Data Sub-blocks] + [扩展数据块 -- 89a] ) + 文件尾(Trailer, 0x3B[";"]的字符)
*         每个数据块大小为[0..255],第一个字节为该数据块的大小,计算大小时不包括该字节(空数据块有一个字节，即标志大小的 0x00),
*   
*   windows下编译: Cygwin + autogen.sh -- 有少部分错误，TODO: MinGW无法编译.
*   util下的demo程序(一个.c文件生成一个可执行文件)，所有的程序都可通过 -h 查看帮助
*     gifasm.c -- 合并多个gif文件为一个动画文件，例: gifasm.exe -A 100 src01.gif src02.gif > result.gif
*       注意：其实现中通过设置GlobalColorMap为第一个图片的值，并搜索其他图片的数据，找到最接近的颜色索引，生成了一个ReMapColor，
*             这样避免每张图片生成局部ColorMap，有效减少文件体积。
*             代码中搜索：Compute the remapping of this image's color map to first one
*     gifflip.c -- 反转gif,可横向(-x) 或 纵向(-y) 翻转。TODO: 动画?
*     giftext.c -- 打印出gif的信息。例：giftext fire.gif， 可知其中有33张图片,
*     text2gif.c -- 根据输入的文本，生成gif文件。例：text2gif -s 8 -c 255 0 0 -t "hello world" > text2gif.gif
*   类型和结构:
*     ColorMapSize[1,8]: 色深，如 6位色深表示有 2^6=64 种颜色
*     ColorResolution - 8 ?
*     ColorMapObject(颜色映射对象，实际上就是调色板?) -- 分为全局(GifFileType.SColorMap)和局部(GifFileType.Image.ColorMap)两种类型，
*       其中有 ColorCount 个 GifColorType(RGB三元色)类型的Colors内存块
*     GifColorType -- 代表颜色列表的一个条目，由三个字节组成，按R、G、B的顺序排列
*     GifImageDesc -- 图像数据块，其中的 Interlace 表示是否使用交织方式排列，如果是的话，需要 4 x 每隔4行 的方式进行处理 ?
*                     其中的SortFlag(似乎giflib未z支持?)表示紧跟着的颜色列表是否分类排列
*     GifFileType -- gif文件类型的句柄指针，其中有大小、ColorMap 等信息，★每次解析或输出时的图像数据块都在其 Image 变量内★
*     GifPixelType -- 每个像素的类型，为 unsigned char(256色)
*     GifRecordType--数据块的类型，解码时通过 DGifGetRecordType 获取。其类型分为:
*       SCREEN_DESC_RECORD_TYPE
*       IMAGE_DESC_RECORD_TYPE(以','[0x2C]开始)--图像数据块,可通过 DGifGetImageDesc 和 EGifPutImageDesc 操作，每行的数据需要通过 DGifGetLine/EGifPutLine 操作(注意 Image.Interlace 的影响)
*       EXTENSION_RECORD_TYPE (以'!'[0x21]开始)--扩展块，实现一些辅助功能，会影响其后的图像数据解码。比较重要的是: 图像控制扩展块(GRAPHICS_EXT_FUNC_CODE)
*       TERMINATE_RECORD_TYPE (固定的';'[0x3B])--表明到了结束块的位置，可跳出 do{ DGifGetRecordType }while(RecordType != TERMINATE_RECORD_TYPE) 的循环
*   函数，主要分为两类: Decode(函数名以D开始) + Encode(函数名以E开始)
*     DGifSlurp
*     EGifCloseFile -- 关闭打开的文件句柄并释放资源
*     EGifOpenFileHandle -- 根据文件句柄打开输出文件，如 (1) 表示stdout
*     EGifOpenFileName -- 根据文件名打开输出文件
*     EGifOpen -- 通过回调的方式打开输出文件，可以适配各式各样的数据输出方式比如网络 等
*	  EGifPutScreenDesc -- 设置逻辑屏幕标识符的信息，包括大小(SWidth * SHeight), 色深(ColorResolution[1..8])、背景色索引(SBackGroundColor)、
*       GlobalColorMap、像素宽高比(Pixel Aspect Radio -- TODO:未直接设置?) 等参数信息
*     EGifPutImageDesc -- 设置一张Image图片数据的参数信息
*     EGifPutLine -- 设置当前行的像素值?
*     EGifPutPixel -- 设置当前位置的像素值?
*     EGifSpew --
*     EGifPutCode|EGifPutCodeNext -- 
*   辅助函数:
*     QuantizeBuffer|GifQuantizeBuffer -- 将RGB量化为调色板颜色，参考 QuantizeRGBBuffer 的实现方式
*     
*   基于颜色列表的图像数据(Table-Based Image Data),采用LZW算法压缩
*     LZW编码长度(LZW Minimum Code Size) -- 也就是要压缩的数据的位数，其值一般等于图像的色深,需要根据该值初始化编译表
*       图像数据在压缩前有两种排列格式：
*         连续的--从左到右、从上到下的顺序排列图像的光栅数据 
*         交织的--创建四个通道(pass)保存数据，每个通道提取不同行的数据：
*           Pass1提取从第0行开始每隔8行的数据 -- [0, 8, 16, 24, ...]
*           Pass2提取从第4行开始每隔8行的数据 -- [4, 12, 20, 28,...]
*           Pass3提取从第2行开始每隔4行的数据 -- [2, 6, 10, 14, ...]
*           Pass4提取从第1行开始每隔2行的数据 -- [1, 3, 5, 7, 9,...]
*     图像数据(Image Data) -- 由一或多个DataSubBlock 组成, 每个SubBlock为：一字节长度+最多255字节的Data
*
*   GIF89 扩展块(extension function), 放在一个图像块(图像标识符)或文本扩展块的前面，用来控制跟在它后面的第一个图像(或文本)的渲染(Render)形式。
*     可通过 EGifPutExtensionFirst => EGifPutExtensionNext => EGifPutExtensionLast 或 EGifPutExtension 两套函数进行处理
*     PLAINTEXT_EXT_FUNC_CODE(0x1--1) --图形文本扩展,用来绘制一个简单的文本图像。由用来绘制的纯文本数据(7-bit ASCII字符)和控制绘制的参数等组成。
*       在文本框(GridBox)控制的矩形范围内使用全局颜色表的颜色进行绘制。
*       unsigned char plain[12] = {块大小, GridLeft(2byte), GridTop(2byte), GridWidth(2byte),GridHeight(2byte), CellWidth, CellHeight, ForegroundColorIndex, BackColorIndex};
*     GRAPHICS_EXT_FUNC_CODE(0xF9-249) --图像控制扩展块。unsigned char Extension[4] = { 长度, 标志字节, delay低位, delay高位, 透明色 }
*       标志字节-- 里面设置了 处置方法(Disposal Method)、用户输入标志(Use Input Flag)、透明色标志 等
*       延时(delay time) -- 单位为 1/100 秒, 如果值不为1，表示暂停规定的时间后再继续往下处理数据流
*       透明色索引(transparent color index) -- 当图像解码时，遇到同样的颜色值，表示为透明部分? 怎么半透明?
*     COMMENT_EXT_FUNC_CODE(0xFE-254)--注释扩展，可以用来记录图形、版权、描述等任何的非图形和控制的纯文本数据(7-bit ASCII字符),
*       注释扩展并不影响对图像数据流的处理，推荐放在数据流的开始或结尾。通过 EGifPutComment 设置
*     APPLICATION_EXT_FUNC_CODE(0xFF-255)--应用程序扩展,由应用程序自己使用。应用程序可以在这里定义自己的标识、信息等
*       unsigned char app[11] = {11, AppI(8byte), AuthCode(3byte) } + data
*
*   Bug更改:
*     1.GifFilePrivateType 的初始化: memset(Private, '\0', sizeof(GifFilePrivateType));
*********************************************************************************************/

/*********************************************************************************************
* 声音：
*   waveOutGetNumDevs(); // 获取声卡的个数
*********************************************************************************************/
namespace FTL
{
	class CFMultiMediaUtil
	{
	public:
		//从注册表 HCRT 下根据文件扩展名，读取类型, 如 .jpg 是 image/jpeg
		FTLINLINE static BOOL GetContentType(LPCTSTR pszFileName, LPTSTR pszContentType, DWORD dwCharSize);
	};

/**************************************************************************************
	//giflib 中处理 GRAPHICS_EXT_FUNC_CODE 的示例代码(延时 + 透明色):
	//TODO:真实代码中的定义和实现方式?
	UINT32 delay = 0;
    if( ExtCode == GRAPHICS_EXT_FUNC_CODE 
        && Extension[0] == GIF_CONTROL_EXT_SIZE) {   //#define GIF_CONTROL_EXT_SIZE 4
        delay = (Extension[3] << 8 | Extension[2]) * 10;
    }
     
    // handle transparent color
    if( (Extension[1] & 1) == 1 ) { trans_color = Extension[4]; } else{ trans_color = -1;}
**************************************************************************************/   
   
}//namespace FTL

#endif //FTL_FILE_H

#ifndef USE_EXPORT
#  include "ftlMultiMedia.hpp"
#endif