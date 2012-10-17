#ifndef DIRECTDRAW_STUDY_H
#define DIRECTDRAW_STUDY_H

/***************************************************************************************************
* DirectDraw 有不同的版本，如 IDirectDraw/IDirectDraw4/IDirectDraw7 等，需要通过QI获取
*   
*
* DirectDraw，沟通了2D图形的显卡和D3D所依赖的帧缓冲层
*   IDirectDraw(0,2,4,7等) -- 
*     GetCaps -- 获取HEL和HAL的能力
*   IDirectDrawSurface(0,2,3,4,7等) -- 代表实际显示画面，通过 GetDC 后进行绘制
*     GetPixelFormat -- 获取画面的象素点格式(if(dwFlags & DDPF_RGB){dwRGBBitCount})
*     Blt/BltFast -- 利用硬件进行图形变换( 目的->Blt(源) )，可以进行 填充(DDBLT_COLORFILL)、缩放、
*       旋转(DDBLT_ROTATIONANGLE,只能在硬件上实现)等
*     SetColorKey -- 设置关键色
*   IDirectDrawPalette -- 用于处理使用256色的调色板方式
*     1.创建调色板数据结构的数组; PALETTEENTRY palette[256]; 对每一个进行赋值(peFlags=PC_NOCOLLAPSE)
*     2.通过DirectDraw对象创建调色板对象，会立即映射到硬件VGA调色板寄存器中：
*       m_pDDraw->CreatePalette(DDPCAPS_8BIT|DDPCAPS_ALLOW256| DDPCAPS_INITIALIZE, palette, &m_pDrawPatlette, NULL)
*     3.把调色板对象附在画图表面上，对象中的所有数据都会以适当的颜色表现出来; m_pDDSPrimary->SetPalette(...)
*       SetEntries -- 设置调色板的值(可以来自于位图)
*   IDirectDrawClipper -- 用于剪切DirectDraw光栅和位图操作，在对出了DD可视界面边界的位图进行着色时，
*     可以避免无用的计算和内存覆盖，通常连接到后台缓冲页面上
*     SetClipList -- 设置剪切列表
*   IDirectDrawGammaControl -- 控制主画面的红、女、蓝色扫描，可以很容易地获得许多效果（水下、屏幕闪烁、黑暗等）
*   IDirectDrawColorControl -- 控制亮度、对比度、色调、饱和度、锐化等
***************************************************************************************************/


/***************************************************************************************************
* 
* Flip -- 后台缓冲绘制后交换指针(光栅化的硬件寻址系统直接指向不同的内存接口，省去了拷贝过程)，但只能在全屏独占模式下使用，
*   对应有 双缓冲(标准)、三缓冲(性能更佳) 等，如果显存不足，Flip将通过内存拷贝的方式模拟，速度很慢
*   
* 有三种内存页面： off(缓存) -> back(绘制) -> primary(显示)
*   1.主页面(Primary Surface) -- 始终可以看到的页面，直接对应被显卡更新的视频内存，必须和视频模式相同(大小、色深)，
*     主页面只可建立一个
*   2.后台缓冲页面(Back Buffer) -- 主页面的缓冲区，大小、色深和主画面完全一致，在显示图像的时候，要先在后背页面绘制，
*     然后交换(Flip)为主页面使其变为可见。一般情况下也是一个(除非使用多缓冲)，会作为主画面的页面交换链
*   3.屏外页面(Off Screen Plain) -- 可以视为“储藏箱”，在初始化的时候，可以先将要用到的图片读入到屏外页面，
*     以后在需要的时候，直接从屏外页面复制(BltFast)到后台缓冲页面。可以为每个经常使用的图片建立一个屏外页面。
*     必须设置 宽、高，通常可以设置关建色来透明制作精灵
*
* 关键色(DDCOLORKEY，创建画面时需要设置 DDSD_CKSRCBLT/DDSD_CKDESTBLT等) -- 用作透明的颜色，可是一个或一定范围
*
* 两种显示模式：
*   全屏模式 -- 独占视频硬件，可直接对视频硬件进行操作，
*   窗体模式 -- 其他程序也可能需要更新自己的客户窗体区域，因此对视频硬件的控制受到限制，主画面是整个屏幕（小心绘制到别的窗体中）
*     通过GetWindowRect找出窗口的用户区左上角在屏幕上的坐标，然后利用它作为原点绘制象素
*
* 两种绘图模式：
*   Lock后直接操作内存 -- 速度快，但比较复杂
*   GetDC后使用GDI -- 速度慢，但可以使用标准GDI，易于和窗口模式一致
*
* 
* 使用流程：
*   1.创建 DirectDraw 对象的一个实例来表示显卡 -- DirectDrawCreate(Ex),第一个参数表示显示设备，为NULL表示
*     使用系统的缺省显示设备；
*   2.设置协作级别和显示模式
*     SetCooperativeLevel -- 设定和Windows协作级别(避免和其他程序、Windows系统冲突)
*       DDSCL_ALLOWMODEX -- 允许使用Mode X(800x600 等)模式，必须和 全屏独占 模式组合使用
*       DDSCL_ALLOWREBOOT -- 全屏独占 模式时允许用户进行热启动(可以检测到 Ctrl+Alt_Del )
*       DDSCL_EXCLUSIVE -- 使用独占方式，使程序完全控制显示设备，其它的应用程序不能同时共享显示设备，必须和全屏模式一起使用
*       DDSCL_FULLSCREEN -- 将程序设为全屏模式，其他程序的GDI将不允许在屏幕上绘图(注意，此时还有标题栏等，但可以挡住任务栏)
*       DDSCL_FPUSETUP -- 应用程序尽可能持有设置的FPU使得D3D性能保持最优
*       DDSCL_NORMAL -- 以普通应用程序窗口方式运行，此时 只能创建块写(Blt等)方式的表面，不能创建翻转表面(Flippable Surface)，
*         但可以有屏外页面
*       DDSCL_MULTITHREADED -- 请求多线程安全DirectDraw行为
*       DDSCL_NOWINDOWCHANGES -- 表示在激活状态下，不允许DirectDraw最小化或恢复窗口？
*     SetDisplayMode -- 选择显示模式（分辨率等），应该先用IDirectDraw::EnumDisplayMode方法检测用户的显示设备的性能
*   3.创建一个或多个 DirectDrawSurface 实例进行显示，可以在页面之间翻转(Flip)显示；
*     指定 dwCaps (如果有backbuffer，必然是 "PRIMARY | COMPLEX | FLIP")
*       DDSCAPS_SYSTEMMEMORY -- 只使用系统内存(默认情况下，先使用显存，不够时自动使用内存) 
*       DDSCAPS_VIDEOMEMORY -- 只使用显存(显存不足会返回 DDERR_OUTOFVIDEOMEMORY 错误)
*       DDSCAPS_LOCALVIDMEM -- 在本机内存中，同时要求 DDSCAPS_VIDEOMEMORY
*       DDSCAPS_PRIMARYSURFACE -- 是主画面，代表着使用者时刻可以看到的部分
*       DDSCAPS_COMPLEX -- 复杂画面，由一个主画面和一个或多个用来创建分页链的后部缓冲构成
*       DDSCAPS_BACKBUFFER -- 分页结构的后部缓冲
*       DDSCAPS_FLIP -- 该画面是一个分页结构画面的一部分，当CreateSurface时，产生一个前端缓冲或一个/多个后部缓冲
*       DDSCAPS_OFFSCREENPLAIN -- 该画面是一个视频外的画面，如覆盖图、文本、alpha画面等，
*   4.绘制
*     锁定后台缓冲区——使用标准的 GDI 函数绘图——解锁后台缓冲区——翻转表面
*     翻转时的返回值
*       DDERR_SURFACELOST -- 表面丢失，需要用IDirectDrawSurface::Restore 方法恢复该表面，再次Flip
*       DDERR_WASSTILLDRAWING -- 前一次的表面翻转还未发生

* DirectDraw有一个后台等待进程，它俘获所有发往该窗口的消息，用这些消息来确定应用程序何时终止。
* 
*
* 注意：
*   1.给DirectDraw的矩形的坐标包含左上角，不包含右下角。即如要填充 640x480，需要传递 641x481;
***************************************************************************************************/

/***************************************************************************************************
* 查询表 -- 16、24、32位色彩模式下时，预先定义好需要的RGB颜色表，在使用时根据关系直接查找，避免对颜色
*   进行 提取、乘除、组合等复杂操作。以空间换时间。类似的有 sin/cos 等(float t=(float)ang*PI/180)
*
* 绘制2D
*   斜率 <== (y2-y1)/(x2-x1)，表示x坐标增加1个单位，y坐标要增加的的单位。斜率为0是水平直线，斜率无穷大为垂直直线
*   直线 <== Bresenham 算法
*   图形剪切 <== Cohen-Sutherland 算法
*     有两种方式：
*       图象空间方式，即象素点模式，图象被光栅化后，由过滤器来决定某一象素点是否在可视区内，适用于单个象素
*       对象空间方式
***************************************************************************************************/

#endif //DIRECTDRAW_STUDY_H
