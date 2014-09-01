
#ifndef FTL_GDI_H
#define FTL_GDI_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlGdi.h requires ftlbase.h to be included first
#endif
#include <atlimage.h>

//http://www.codeproject.com/KB/GDI/anieffect.aspx
//字体模拟 http://html.nhndesign.com/markup_tools/preview
//调试工具：GdiWatch, http://www.189works.com/article-9611-1.html
//字体工具(FontCreator/FontForge) -- 可以创建或编辑 ttf 等字体文件
//图标库 -- http://findicons.com/pack

/*
.FON(光栅字体)--针对特定的显示分辨率以不同大小存储的位图,并不是以矢量描述的,放大以后会出现锯齿,但显示速度非常快
.FON(矢量字体)--基于矢量的数学模型定义,一些windows应用程序会在较大尺寸的屏幕显示中自动使用矢量字体来代替光栅字体的显示
.PFM(PostScript字体)--基于Adobe PostScript矢量语言的描述,需要安装"Adobe Type Manger"(ATM)软件才能使用
.TTF(TrueType字体)--用的最多,由一种数学模式来进行定义的基于轮廓技术的字体，比基于矢量的字体更容易处理，保证了屏幕与打印输出的一致性。
                    可以随意缩放、旋转而不必担心会出现锯齿

DOS等传统字符界面用点阵模板显示文字
GUI中的文字一般是利用轮廓字体中的控制点数据经计算后再绘制出来,
  Windows采用基于二次贝塞尔曲线的TrueType轮廓字体(*.ttf)

DPI-- dots per Inch(每英寸的点数,打印分辨率使用的单位，LOGPIXELSY)，一般来说是 96(较小)/120(中等)/150(较大)/600(激光打印机)
PPI -- Pixels Per Inch(每英寸的像素数，图像分辨率所使用的单位)
  TODO: http://hi.baidu.com/kingcham/item/b3653ce0c69756216dabb8cd

1物理英寸 = 25.4mm; 1磅 = 1/72 英寸 = 25.4/72 mm
VGA中, 1逻辑英寸=96像素, 300dpi的打印机，1英寸300像素

颜色空间(Color Space) -- 微软在 CDrawingManager 中提供了各种颜色空间转换的函数(如RGBtoHSL),但依赖于MFC
  RGB -- Red, Green, Blue, 科学研究一般不采用RGB颜色空间(难以进行数字化的调整),是最通用的面向硬件的彩色模型
  HSL -- Hue(色调 0~239), Saturation(饱和度,0~240), Lum(亮度,0~240)，能更好的数字化处理
  CMY -- 工业印刷采用的颜色空间

COLORREF -- 0x00bbggrr, 四字节，RGB 只能设置3个字节，自定义一个扩展的 RGBA, 测试向Canvas上手动生成RGBA的数据
RGB(r,g,b)==#bbggrr

取颜色的宏：RGB565_MASK_RED、RGB555_MASK_RED 等， 
  R = (wPixel & RGB565_MASK_RED) >> 11;//取值范围0-31
  R = (wPixel & RGB555_MASK_RED) >> 10; // 取值范围0-31


默认的绘图模式为 MM_TEXT(逻辑单位为像素，不打印输出时屏幕绘图的最佳绘图模式 -- 不用转换)
CScrollView中是通过改变逻辑坐标原点的相对位置、改变ViewportExt等的大小来实现移动、放大缩小的

PtInRect、Rectangle -- 等函数的矩形区域不包括矩形的右边界和底边界,
所以DrawClient中GetHandleRect中返回的是 left-3, 7(左3右4，保证真正有效的高宽为3)

强制清屏 -- 先用当前背景色填充窗口，再请求操作系统重绘窗口（调用OnDraw或OnPaint函数）
  1.Invalidate(使窗口内容无效) + UpdateWindow(更新窗口)
  2.RedrawWindow

图片框架控件 -- Type属性为Frame的图片控件(Picture Control), 可直接在Dialog的OnPaint中直接绘制
其他控件 -- 必须先按顺序依次调用CWnd类的Invalidate和UpdateWindow函数后，再开始用DC画图
            CWnd* pWnd = GetDlgItem(IDC_COLOR);  
            pWnd->Invalidate(); // 使控件窗口无效
            pWnd->UpdateWindow(); // 更新控件窗口 
            pDC->FillRect(&rect, &brush); // 绘制填充矩形

移动图形(如TrackBox) -- 需要在不破坏原有背景图案的基础上移动这些图形，常采用异或画图方法。
  移动图形的过程为：SetROP2(R2_XORPEN); 异或画图、在原位置再异或化图（擦除）、在新位置异或画图

字体 -- 设备无关的,主要是TrueType类型的轮廓字体
  CreatePointXXX -- 从Windows的物理字体库中选择一个与所设置参数最匹配的字体
  AddFontResourceEx -- 从ttf等字体文件中加载字体到系统，返回加到系统中的字体个数
  GetStockObject(DEVICE_DEFAULT_FONT/DEFAULT_GUI_FONT/SYSTEM_FONT) -- 获取系统中的默认字体
  */

/*****************************************************************************************************
* GDI -- 1985年推出
* GDI+ -- 2001年随WinXP推出
* WPF(Windows Presentation Foundation) -- 2006年随Vista及.Net3推出,统一了桌面和浏览器等客户端应用程序的图形界面
* 
* 绘图前获得客户区的大小(GetClientRect),在其中绘制，但需要offsetviewport? 否则Scroll后会出问题
* 
* 屏幕DC -- CDC dc; dc.Attach(::GetDC(NULL)); xxx, ::ReleaseDC(NULL,dc.Detach()); //GetWindowDC 也是一样的
*
* CDC 
*   成员函数
*     PatBlt -- 由DC中的刷与位图按(无源位图的)光栅操作创建位图图案
*     MaskBlt -- 用指定的掩膜和光栅操作组合源位图与目标位图
*     PlgBlt(Parallelogram Block transfer, 平行四边形块传送)--从源位图矩形向目标位图的平行四边形进行颜色数据的位块传送
*     FloodFill -- 用当前刷(包括位图刷)从指定点开始填充以指定颜色为边界的区域
*   子类
*     CClientDC -- GetDC
*     CWindowDC -- ::GetWindowDC
*     CPaintDC -- ::BeginPaint
*     CMetaFileDC --
*     CMemoryDC(WTL中) -- 
*   
* CGDIObject -- 局部变量退出时会自动DeleteObject，可通过SelectStockObject选入之前保存的旧对象或预定义对象保证对象删除时未被使用
*   CPen
*   CBrush
*     CreatePatternBrush -- 位图画刷
*     SetBrushOrg -- 位图画刷在填充时默认以客户区的左上角为原点的(设备坐标?)，这样在客户区不同位置绘制多个矩形时，
*       画刷不会出现偏差(自动延续)。但如果是滚动窗体，则必须调用 SetBrushOrg 使用设备坐标设置画刷原点(使用示例见GuiTest) 
*       其值有两种方式获取(测试好像都OK)
*       1.CPoint pt(0,0); pDC->LPtoDP(&pt); dcMem.SetBrushOrg(pt.x,pt.y);
*       2.在 WTL.CZoomScrollImpl 中 dcMem.SetBrushOrg( -m_ptOffset.x , -m_ptOffset.y);
*       注意：因为有映射误差，所以在绘制时需要放到绘制范围(似乎放大两个逻辑坐标值即可)--例子是rcClient,最好用rcClip?
*         CRect rcClient;GetClientRect(rcClient);dc.DPtoLP(rcClient);rcClient.InflateRect(2,2);

*   CFont -- 设备无关的字体(TrueType类型的轮廓字体),其创建函数并不"创建"一个全新的字体，
*     而是从Windows的物理字体库中选择一个与所设置参数最匹配的字体.
*     ClearType -- Vista/Win7使用的技术，让显示器RGB各个次像素也放光，对色调进行微调，可达到实际分辨率以上的文字显示效果。
*       使得字体更加平滑(WinXP默认未启用:显示->外观->效果->使用下列方式使屏幕字体的边缘平滑)
*     MacType(前身为GDI++) -- 可以接管Windows系统的字体渲染功能，让文字看起来更加平滑、清晰。
*     字体大小单位( WTL::CDialogBaseUnits::InitDialogBaseUnits )， 屏幕浏览时常用px(如网页)，打印输出时常用pt(如Word)
*       1.point/pt(磅,1/72逻辑英寸) -- 96DPI时一个像素为 72/96=0.75磅(9pt=12px)， 如 ITextFont::SetSize 等, 通常称为"绝对长度"，但会随系统字体大小设置而改变(如 96DPI、120DPI)
*         nPointSize -- 通常是十分之一磅， 如 CreatePointFont 时的单位, CFontDialog::GetSize ，1pt = 10 nPointSize
*       2.pixel/px() -- 像素，和屏幕分辨率有关，通常称为“相对长度”
*       3.twips(1/1440 逻辑英寸或 1/20磅) -- 如 CHARFORMAT::yHeight
*       4.字号 -- 如 初号、小初 等，优点是使用简单方便（无需关心字体的实际尺寸），缺点是字体的大小受字号的限制，
*                 太小、太大或字号等级间的字体无法用字号表示
*       5.逻辑单位，如 LOGONT::lfHeight， 注意：使用正的高度得到的字体比使用负值得到的字体小
*           <0 时,高度被转化为设备单位，大小相对于字体的字符高度 -- ? Windows 根据绝对值查找
*                 查找的字体高度 = lfHeight - InternalLeading ???
*           =0 时,使用合理的默认高度
*           >0 时,高度被转化为设备单位,大小相对于字体的网格高度, 如 nPointSize 为8, LOGPIXELSY 为96，计算出来为 -10.666?
*                 查找的字体高度 = -lfHeight ???
*       转换(MM_TEXT)：
*          逻辑坐标 => 磅: nPointSize = LOGFONT::lfHeight * 72 / dpiY;
*          磅 => 逻辑坐标: LOGFONT::lfHeight = -MulDiv(nPointSize, pDC->GetDeviceCaps(LOGPIXELSY), 72) = nPointSize*dpiY/72
*          磅 => twips:    nTwips = nPointSize * 20;
* 
*       6.Em -- 字体的逻辑坐标高度, 一个em表示一种特殊字体的大写字母M的高度，也是相对度量单位，
*           GdiPlus::Font 构造时使用, LOGFONT::lfHeight 为负值时(如字体对话框中选择 8 等)，
*           1em=16px
*
*       //对话框上的单位： http://blog.csdn.net/dclchj/article/details/5938497
*       nPointSize(CreatePointFont) -- 字体大小,以0.1点（像素/墨点/磅数）为单位。
*         5号宋体字的该值是105,磅数为 10.5
*     lpszFaceName -- 字体名称字符串的指针，常用的有"宋体","Times New Roman"等
*   CRgn
*   CPallete
* 注意：每次GetDC获得的都是一个全新的临时默认配置的DC，原来选入的各种GDI对象全部作废
*   -- 但自己CreateComptableDC创建的成员变量可以一直保存；而HDC在窗口的存在期间一直有效
* 
* 
* 区域(Region) -- 创建好后通过 SelectObject 选入 HDC 即是裁减区域?
*  CombineRgn(合并区域)
*    RGN_AND --两个区域的交集
*    RGN_COPY--两个区域的第一个区域作为新的区域 
*    RGN_DIFF--第一个区域的部分减去两个区域的交集
*    RGN_OR  --两个区域的并集
*    RGN_XOR --两个区域的并集减去两个区域的交集
*  InvertRgn -- 使区域的颜色反色。在彩色屏幕上，这个反转依赖于产生屏幕颜色的技术类型
*  FrameRgn -- 为区域周围画一个边界，并指定边界的宽度和刷子的模式
*  PathToRegion -- 将路径转换为区域，但在此过程中，路径将被破坏
*
* 裁减区域(ClipRegion) -- 通过设置裁减区域，即使在全部逻辑作标上作图，也能大幅提高绘图效率。
*   BeginPaint获取的DC包含裁减区域(性能高)，GetDC、CreateDC获取的DC默认不包含裁减区域(性能低)
*   
*   IntersectClipRect -- 合并当前裁减区域和客户区的矩形区域
*   ExcludeClipRect -- 从当前裁减区中去除某矩形
*   ExtSelectClipRgn -- 通过指定的方式(如 RGN_AND/RGN_OR 等)把一个特定的区域与当前的剪切区域合并在一起
*
* 图象空间变换 -- 使用 具有9个整数的局部像素域(称为模版)的来完成，通常是对 像素点及周围共9个点的颜色值依次乘以模版的值并相加、平均
*   柔化(Smooth)--使图象上任一像素与其相邻像素的颜色值的大小不会出现陡变，常用于除去图象中点状噪声，会降低图象的对比度。
*           可用每个像素和周边(包括本身)的9个像素的加权平均值取代。常用模版为 二维高斯(Gauss)。
*           { 1,2,1,  2,4,2,  1,2,1 } x 1/16
*   锐化(Sharp)--增强图象边缘效果，通过增强高频分量减少图象中的模糊，也称为高通滤波。常用模版为拉普拉斯(Laplacian) 模版。
*           { -1,-1,-1,  -1,9,-1,  -1,-1,-1 } x 1
*****************************************************************************************************/

/*****************************************************************************************************
* 绘图性能(避免闪烁 -- flick)
*   1.只绘制一次
*   2.WM_ERASEBKGND -- 如果处理了该消息返回1，否则系统会调用默认处理机制。但似乎无论返回TRUE或FALSE，都不会闪烁？(只要不调用系统默认的处理即可)
*   3.不要绘制不需要的(Clip) -- Windows会自动剪切掉外面的？测试Invalidate后绘制到外面？怎么测试的？忘了
*     GetClipBox -- 始终和 PAINTSTRUCT::rcPaint 一致
*     GetUpdateRgn --
*     GetUpdateRect(BeginPaint后即为空)
*     PAINTSTRUCT::rcPaint
*   4.CS_VREDRAW,CS_HREDRAW -- 经过测试,有这两个选项，在窗体缩小时也会重绘，否则在缩小时不重绘,放大时两个的无效区都是全Client
*   5.WS_CLIPCHILDREN, WS_CLIPSIBLINGS -- 排除子窗体、兄弟窗体(注意Zorder)的位置
*   6.ExcludeClipRect -- 排除特定区域(绘制后将无效 -- 不会闪烁,常用于重叠绘制)
*   7.Double Buffering -- 常过度使用，如果每次重绘时创建MemoryDC会很慢(1024x768x32/8=3M),内存密集的
*     CWindowDC dcDesktop(NULL/m_hWnd); CDC dcMem; dcMem.CreateCompatibleDC(dcDesktop);
*     bitmap.CreateCompatibleBitmap(dcDesktop,cx,cy); //注意，参数不能是 dcMem，否则只能是二色图
*   LockWindowUpdate
*
* 按需绘制
*   0.if(!IntersectRect(xx)){return;} -- 判断本控件的坐标矩形和需要绘制的矩形是否有交集，如果没有直接返回，避免绘制
*   1.OnDraw 中,pDC->GetClipBox 获得需要绘制的逻辑坐标
*   2.生成rcClip的副本rect，转成设备坐标，用其高、宽创建位图
*   3.offsetViewportOrg( - rect.left, -rect.right); -- 为了保证滚动窗体滚动后也OK。
*     选入的bitmap才能在正确的偏移
*   4.selectBitmap 后使用逻辑坐标绘图, 
*   5.IntersectClipRect( rcClient )
*   6.将memoryDC、pDC的viewPortOrg、WinOrg等都重设为(0,0)，映射模式为MM_TEXT
*   7.Bitblt(rect.left, rect.right, .... ,&memDC, 0,0, SRCCOPY)
*
* 控制通过鼠标改变窗体大小时是否立即重绘：Display  -> 外观 -> Effects -> Show window contents while dragging 
*****************************************************************************************************/

/*****************************************************************************************************
* 坐标系统  -- 完整的转换?世界坐标系空间 =(SetWorldTransform)=>页面空间(逻辑坐标?)=(SetMapMode)=>设备空间(设备坐标)
*   世界坐标(world space)
*   逻辑坐标(page space ,logical coordinates) -- 坐标系称为Window，使用逻辑单位(如 0.1英寸)
*     长久保存的对象位置，GDI绘制(如 GetClipBox)，使用逻辑坐标是所见即所得的基础
*     GDI使得用户只需在逻辑坐标上绘图即可( 逻辑坐标绘图 ==(MapMode映射)=> 物理坐标 )
*   设备坐标(device coordinates) -- 坐标系称为ViewPort，面向物理设备，以设备能表示的最小长度(即像素)为单位
*     CWindow成员函数使用(如 InvalidateRect), Mouse, 位置测试(如 PtInRect等)
*     三种设备坐标系：客户区(CClientDC, MouseMove)、窗口区(CWindowDC)、屏幕坐标(如 GetCursorPos、SetBrushOrg等)
*       转换：ScreenToClient、ClientToScreen
*     注意：Viewport(不可见)和物理坐标(可见)是不同的，相同的是都是象素为单位，只需要考虑位移问题
*     
* 绘制时代码中使用逻辑坐标，然后Windows通过映射模式的值,自动转换为设备坐标,并显示在设备(如显示器、打印机)上
* 
* WindowOrg 总是被映射为 ViewporOrg -- 注意：虽然称为原点，但其值不一定为0
*
* Win9x使用实际英寸?WinNT以后使用基于逻辑英寸的映射方式，比实际的英寸要大。1逻辑英寸=100逻辑单位 ?有 96 - 100，
*   否则，普通的10磅文本(约3.53毫米)非常小（只影响显示，不影响打印）
* 
*   SetWorldTransform -- 可以 缩小一半(eM11=0.5,eM22=0.5)、旋转、X轴反射(eM11=1,eM22=-1)、Normal(eM11=1,eM22=1)等
*     需要通过 SetGraphicsMode(GM_ADVANCED) 启用世界坐标
*     page space中的点坐标(x'和y')， world space中的点坐标(x,y)
*     x' = x * eM11 + y * eM21 + eDx, 
*     y' = x * eM12 + y * eM22 + eDy, 
*     
*   SetWindowOrgEx(x,y) -- 把窗口的(x,y)点映射到视口的原点.
*     这个window是看不见的,可以假想有这么一个window, 提供GDI画图函数以逻辑坐标来画图,
*     这个坐标系也叫 page space 坐标系,有自定义原点与单位长度( 逻辑单位,如0.1mm,从而可以与设备无关 )
*     通常用于绘制时不从左上角绘制（如居中绘制时，需要 SetWindowOrgEx([-?]m_rcDrawTarget.left, [-?]m_rcDrawTarget.top);
*   SetViewportOrgEx(x,y) -- 把窗口的原点(中心点)映射到视口的(x,y)点.
*     物理设备坐标总是以左上角为原点,以像素为单位,X向右,Y向下 
*     viewport与之的区别是原点可以由SetViewportOrgEx改变(滚动效果?),以及大小可以由SetViewportExtEx改变(放大、缩小?),
*     这个坐标系也叫device space坐标系,viewport也是看不到的,真的能看到的只有物理坐标系(都是象素为单位，只考虑位移)
*     通常用于有滚动条时设置偏移，如 WTL::CZoomScrollImpl::SetViewportOrg(-m_ptOffset.x, -m_ptOffset.y);
*   默认映射下：SetViewportOrg(x,y) = SetWindowOrg(-x,-y)
*     
*   SetWindowExtEx    //设置逻辑坐标系统中对应的逻辑单位个数，如 100 等
*   SetViewportExtEx  //设置设备坐标系统中对应的象素个数, 如 GetDeviceCaps(hdc, LOGPIXELSX)表示1逻辑英寸对应的96个象素
*     或 GetClientRect 取得的客户区像素个数.两者通过 ViewExt/WinExt 的比值算出1逻辑单位对应的设备坐标单位，
*     如上面的比值表示100个逻辑单位对应96个象素(1逻辑英寸)。
*     因此，通常的设置中，两者的绝对值不重要，关键是比值。
* 
*  使用CDC函数 SetViewPortOrg 和 SetWindowOrg 可以改变原点
*  不管对窗口和视口原点作什么改变，设备点(0,0)始终是客户区的左上角
*  
* 映射模式 -- 确认GDI绘制时Windows系统如何从逻辑自动映射到设备(如屏幕或打印机的像素点)
*     逻辑单位的实际大小和增长方向，设备单位和方向在选择设备(显示器、打印机)时已经确定了
*   模式                   逻辑坐标单位                 
*   MM_TEXT               象素(同设备坐标)              x右向增加,y向下增加( x+,y+ )
*   MM_LOENGLISH           0.01物理英寸(0.254mm)        x右向增加,y向下减少( x+,y- )
*   MM_HIENGLISH           0.001物理英寸                x右向增加,y向下减少( x+,y- )
*   MM_LOMETRIC            0.1物理毫米                  x右向增加,y向下减少( x+,y- )
*   MM_HIMETRIC            0.01物理毫米                 x右向增加,y向下减少( x+,y- )
*   MM_TWIPS         1/1440物理英寸(1/20磅,或0.176mm)   x右向增加,y向下减少( x+,y- )
*   MM_ISOTROPIC        可以改变比例因子和原点          纵横比1：1
*   MM_ANISOTROPIC      可以改变比例因子和原点          比例因子可以分别改变
* 
* 注意：
*   1.默认情况下(即没有用SetWindowExt/SetViewportExt设定一个逻辑单位对应多少个像素的时候，
*     MM_ANISOTROPIC和MM_ISOTROPIC使用MM_LOMETRIC映射模式)
*   2.使用 SetLogicalMapMode 可以设置逻辑坐标是逻辑单位(比如 0.01逻辑英寸)
*   3.AtlWin.h 中已经提供了 HIMETRIC_PER_INCH、MAP_PIX_TO_LOGHIM 等宏进行转换，
*     如? hmWidth = MAP_PIX_TO_LOGHIM(pxWidth, GetDeviceCaps(hdc, LOGPIXELSX));
* 
* 核心公式(比值相同,也可说是一逻辑单位对应的像素个数):
*   (xViewPort - xViewportOrg)/(xWindow-xWindowOrg) = xViewExt/xWindowExt
*   注意：逻辑原点WindowOrg总是映射成设备原点(ViewportOrg)，但其值不一定为0
*   MyLPToDP: xViewPort = (xWindow-xWindowOrg)*(xViewExt/xWindowExt)+xViewportOrg
*             yViewPort = (yWindow-yWindowOrg)*(yViewExt/yWindowExt)+yViewportOrg
*   MyDPToLP: xWindow = (xViewport-xViewportOrg)*(xWindowExt/xViewExt)+xWindowOrg
*             yWindow = (yViewport-yViewportOrg)*(yWindowExt/yViewExt)+yWindowOrg
*
* 映射误差(逻辑单位和设备单位不能整除造成)
*   可能的后果：客户区的部分未刷新；对象间出现间距；对象在屏幕的不同位置会缩小或放大一个象素等。
*   避免方式：1.如只是屏幕作图(不打印?)，使用 MM_TEXT，不进行映射最好?
*             2.尽量选择窗口范围和视口范围比可整除的映射模式，如逻辑英寸的MM_TWIPS是1440/96,简化后为15/1(从设备到逻辑时无误差)
*   因为可能存在映射误差，为了避免缩放时对用户产生影响(主要是放大时，此时一个逻辑单位对应N个设备单位，如50倍时一个逻辑单位对应50个设备单位),
*     如果涉及逻辑坐标和设备坐标转换、偏移时,最好转换为较小单位并进行计算，从而减少误差(如放大时在设备坐标下计算偏移等)
*   GDI+有浮点坐标，是否还存在这个问题?
*
* 坐标转换(坐标映射):
* 
*   Windows鼠标消息获得鼠标光标的坐标是设备坐标,CDC,CRect等的成员函数需要使用逻辑坐标才可以正常工作。
*   Windows GDI 负责逻辑坐标到设备坐标的换算。假定设置了设备环境映射模式和有关的参数,
*   CDC函数 LPtoDP 和 DPtoLP 可以在两个系统之间进行换算。
*   重要规则如下：
*     1.假定CDC成员函数具有逻辑坐标参数
*     2.假定CWnd成员函数具有设备坐标参数
*     3.在设备坐标中进行所有的键击测试操作。在设备坐标中定义区域。
*       像CRect::PtInRect这样的函数,只有在使用设备坐标时,才能保证结果的正确性。
*     4.在逻辑坐标中存储长期使用的值。如果在设备坐标中存储一个点的坐标,只要用户在一个窗口中滚动,
*       该点的坐标就不再有效了。
*   
* 物理坐标:用于映射模式时存储用户的坐标值,与映射模式无关?
*
*****************************************************************************************************/

//GDI对象 -- GDI对象的句柄都是Windows内部使用的对象，不能使用CHECK_POINTER_XXX(IsBadReadPtr)一类的函数进行判断
/*****************************************************************************************************
* 文字：
* GetTextMetrics -- 获取字体相关信息：平均字符宽度、Family 等属性
* GetTextExtentPoint32 -- 计算获取指定字符串的高度和宽度
* SetTextJustification -- 可以在绘制的字符串中自动增加空格( 计算出每行的空格数 )。
*   specifies the amount of space should add to the break characters in a string of text
*   (可以 nBreakExtra=输出宽度-字符的总宽度)，
*   每次输出结束后使用SetTextJustification(hdc, 0, 0);恢复默认参数
*   DrawText 
*     DT_CALCRECT -- 计算并调整矩形的宽与高
*     DT_NOCLIP -- 不剪裁，可提高显示速度(会画到矩形外?)
*     DT_SINGLELINE -- 单行显示文本，忽略回车和换行符，必须指定该值才能使用 DT_VCENTER?
*     DT_TABSTOP -- 设置Tab的字符位数(默认为8)，该数放在nFormat的高位字节中，同时用 DT_EXPANDTABS 扩展
*     DT_WORDBREAK -- 文本行超过矩形区域的宽时换行
*   DrawTextEx -- 增加了 LPDRAWTEXTPARAMS(设置Tab大小，左右边界等) 选项,
*   ExtTextOut -- 可以指定文字输出的间隔 ，例如：
*     int space[]   = {20, 25, 30, 35, 40, 45, 50, 55, 60};
*     ExtTextOut(hdc, 20, 80, ETO_CLIPPED, &rect, "ExtTextOut", 10, space); // 间隔逐渐增大
*   TabbedTextOut -- 可以指定位数后开始Tab定位输出字符
*     int tabList[] = {120, 280, 380};
*     TabbedTextOut(hdc, 20, 170, TEXT("Tabbed\t\tText\tOut"), 16, 3, tabList, 1);
*   PolyTextOut -- 
*   问题: GDI里没有能控制行间距的文本输出方式?
*
* RTF -- Rich Text Format,由微软公司开发的跨平台文档格式,是一个很好的文件格式转换工具，用于在不同应用程序之间进行格式化文本文档的传送
*   使用 wordpad.exe 可以很方便的生成RTF文档，标准的RTF文件应该仅包含7位ASCII码字符。 
*   四种格式
*     a.未格式化文本(unformatted text)
*     b.控制字(control words) -- 标记打印机控制符的格式化命令, "\[a-z的小写字母字符], 如文件开始处的 "\rtf1" 等
*     c.控制符号(control symbols)
*     d.组(group)
*   文件组成（三部分)
*     a.标志: \rtf1 或 \urtf1
*     b.文件头 -- 字体表，文件表，颜色表等，每个表由一对大括号括起来
*     c.正文
*
* 位图 -- 
*   注意：创建兼容位图时，兼容DC不能是才创建好的内存DC -- 否则位图只有黑白两色
*   CBitmap -- 功能较弱，只能显示出在资源中的图标、位图、光标及图元文件
*     LoadBitmap -- 从资源装载位图,注意：颜色会被裁剪为屏幕的颜色位深，并且不能直接访问像素(即 DIBSection )
*     LoadOEMBitmap -- 装载预定义位图(如 OBM_CLOSE )
*     LoadMappedBitmap -- 从资源装载位图，并将颜色映射到当前系统颜色
*     CreateBitmap -- 创建具有指定高宽和图案的 DDB 内存位图
*     CreateDiscardableBitmap -- 创建与指定DC兼容的可废弃位图(什么是可废弃位图?)
*     SetBitmapBits(SetDIBits) -- 将指定的缓冲区的位图数据复制到CBitmap中(该函数已过时,应用SetDIBits)
*     GetBitmapBits(GetDIBits) -- 将CBitmap中的位图数据复制到指定的缓冲区(该函数已过时,应用GetDIBits)
*     SetBitmapDimensionEx -- 设置位图的高和宽(以0.1毫米为单位，为什么不是像素?和 CreateXXX 中的宽高有什么关系?)
*   CImage(atlimage.h) -- MFC/ATL共享的新类(本质使用的是 Gdiplus::Bitmap)，同时支持 DDB/DIB,
*       能从外部磁盘中加载并显示 JPEG/GIF/BMP/PNG等格式的图象文件，支持 Exit 信息?
*       其Draw方法自动支持透明色或Alpha通道(综合了StretchBlt、TransparentBlt、AlphaBlend等函数)
*       注意：如果在DLL中使用 CImage，可能会造成死锁 --静态的 CImage::CInitGDIPlus 变量，如果没有在 DllMain 调用前调用
*             CImage::ReleaseGDIPlus 进行释放的话，会因为析构该变量时在 DLLMain 中调用 GdiplusShutdown 死锁。
*             参见:http://support.microsoft.com/kb/322909
*     AlphaBlend -- 支持象素级的颜色混合(源位图和目标位图使用Alpha混合功能)，实现透明和半透明的效果
*       AlphaFormat为0 -- 所有像素使用同样的常量Alpha
*                  为AC_SRC_ALPHA -- 每一个像素必须有自己的alpha通道，即是32-bpp的DC
*     Draw -- 多种重载方式，综合了 StretchBlt、TransparentBlt、AlphaBlend等函数的功能，自动支持透明色或Alpha通道
*     GetBits -- 获取图片中的位图数据
*     Load/LoadFromResource -- 从 文件/资源 加载图象
*     IsIndexed -- 判断是否是索引色(有调色板的伪彩色)
*     PlgBlt -- 使一个矩形区域的位图映射到一个平行四边形区域中，且可能使用位屏蔽操作。
*     TransparentBlt -- 在目标区域中产生透明图象
*     SetTransparentColor -- 设置某种颜色是透明色
*     MaskBlt -- 在目标区域中产生源位图和屏蔽位图合成后的效果
*     GetExporterFilterString -- 获取支持的图象文件的过滤字符串,可用于打开、保存对话框
*     Save -- 如果指定 GUID_NULL则格式根据扩展名确定，其他可指定 GUID_BMPFILE/GUID_PNGFILE/GUID_JPEGFILE 等
*     注意：其GetDC/ReleaseDC在遇到问题(如GDI对象失效)造成GDI对象错误时会报告断言，帮助快速发现问题
*   GUI对象失效检测 GDI对象的 有效性
*     HBITMAP 等是Windows内部对象(the internal index of system objects)，
*     其值不是地址(直接查看对应值的地址失败,且调用 IsBadReadPtr 等函数时会出现 Access Violation错误)
*     对删除(DeleteObject)后的HBITMAP(野指针?)调用GetObject会返回0(失败),且GetLastError为ERROR_INSUFFICIENT_BUFFER。
*     被Select时也会返回NULL，但此时GetLastError为0。
*     HBITMAP 同一时间只能选入一个DC中，第二次Select时会返回NULL，但GetLastError为0
* 
*  CreateDIBSection -- 创建可直接访问的DIB，可直接访问位图的位信息，创建DIBSECTION
*    LoadImage(, LR_DEFAULTSIZE | LR_CREATEDIBSECTION) -- 加载图像资源，并且创建可直接访问像素的 DIBSection
*  CreateDIBitmap -- 从DIB创建DDB，创建BITMAP
* 
* 画笔
*   创建方法:CreatePen/CreatePenIndirect/ExtCreatePen
*   // 创建Geometric画笔( LOGBRUSH lb )
*     lb.lbStyle = BS_HATCHED ; 
*     lb.lbHatch = HS_DIAGCROSS;  
*     lb.lbColor = RGB(255, 0, 0);
*     ExtCreatePen(PS_GEOMETRIC | PS_DASH | PS_ENDCAP_SQUARE | PS_JOIN_ROUND,10, &lb, 0, NULL);
*
* 画刷
*   创建方法:GetStockObject(LTGRAY_BRUSH)/CreateSolidBrush/CreateHatchBrush/CreatePatternBrush/CreateBrushIndirect
*
* 绘图 
* SetArcDirection --  设置椭圆弧绘制方向； 
* RoundRect -- 圆角矩形
* Ellipse -- 椭圆
* Pie -- 饼图
* Arc -- 绘制椭圆弧
* PolyBezier -- 绘制Bezier样条
* PolyDraw -- 绘制一直线，后面紧接着一Bezier样条
* PolyPolyline -- 一次绘制四条连续直线组成图形
* POINT pplPoint[] = 
* { 
*   200,300,280,300,280,380,200,380, 200, 300,
*   200,300,140,270,70,220,150,220,190,270,280,300,
*   200,380,140,320,70,300,70,220,
*   140,320,140,270,190,270
*   };
* DWORD value[] = {5, 6, 4, 3};
* PolyPolyline(hdc, pplPoint, value, 4);
*
* ExtFloodFill中
*   FLOODFILLBORDER  -- 填充到颜色的边界，用于填充内部颜色不同但边界颜色相同的区域(效果同FloodFill)
*   FLOODFILLSURFACE -- 填充所有颜色为crColor的点，直到碰到非crColor的点，
*     用于填充内部颜色相同但边界颜色可以不同的区域。
*****************************************************************************************************/

/*****************************************************************************************************
* 透明绘制
*   AlphaBlend -- 显示有透明度的图象， Alpha 指定所绘制颜色与背景颜色的混合程度
*     显示颜色 = 源像素颜色*Alpha/255 + 背景颜色*(255-Alpha)/255( Alpha 为0时完全透明，255时完全不透明 ) 
* 
* 半透明显示(snagit抓图时显示的效果)
*   1.Bitblt 一个完整的原始位图到 dcTran
*   2.生成一个内存DC(dcAlpha), 填入全黑 -- dcAlpha.FillSolidRect(RGB(0,0,0))
*   3.使用 dcTran.AlphaBlend( xxx, dcAlpha, 0,0, (AC_SRC_OVER, 0, 50, 0)) 将全黑的图像采用 50 的Alpha通道覆盖到原始位图上
*     BLENDFUNCTION 的第四个参数如果是 AC_SRC_ALPHA 表示啥意思? 似乎是在 UpdateLayeredWindow 函数中使用的
*   4.选择高亮时，就先绘制 dcTran，然后在高亮位置处粘贴指定大小的bg
*
* 缓冲绘图API，Vista以后支持 ( atltheme 中有 CBufferedPaintBase、CBufferedPaint 辅助类)
*   BufferedPaintInit/BufferedPaintUnInit -- 初始化和终止话，每个线程独立
*   BeginBufferedPaint -- 开始缓冲绘图，指定最终的缓存将要绘制的位置
*     BufferedPaintSetAlpha
* 
* 获取桌面DC
*   ::GetDC(NULL) 或 ::GetWindowDC(NULL) -- 当前测试出来好像没有差别，注意最后 ::ReleaseDC
*****************************************************************************************************/

namespace FTL
{
    //ARGB1555 与 RGBA8888 ?  RGBA 是大端序， ARGB 是小端序?

    //对应的内存结构： 0xAABBGGRR -- 
    //  问题：gdi+中是 Color::MakeARGB,内存结构是 0xAARRGGBB(对) 或 0xBBGGRRAA，究竟哪种对

#define MAKE_RGBA(r,g,b,a)  ((COLORREF)((((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))|(((DWORD)(BYTE)(a))<<24)))
#define GetAValue(rgba)     (LOBYTE((rgba)>>24))

    #ifdef FTL_DEBUG
        //判断位图是否可选入MemoryDC:1.要有效; 2.没有被选入别的DC中
    #define CHECK_BITMAP_SELECTABLE(h)   \
        {\
        FTL::CFGdiObjectChecker checker;\
        FTLASSERT(checker.IsValidate(h));\
        FTLASSERT(!checker.IsSelected(h));\
    }

    #define CHECK_GDIOBJECT_VALIDATE(h) \
        {\
        FTL::CFGdiObjectChecker checker;\
        FTLASSERT(checker.IsValidate(h));\
    }
    #else
    #  define CHECK_BITMAP_SELECTABLE(h)      __noop
    #  define CHECK_GDIOBJECT_VALIDATE(h)     __noop
    #endif 


    class CFGdiObjectInfoDump
    {
    public:
        FTLINLINE CFGdiObjectInfoDump();
        FTLINLINE ~CFGdiObjectInfoDump();
        FTLINLINE BOOL GetGdiObjectInfo(HGDIOBJ hgdiObj);
        FTLINLINE LPCTSTR GetGdiObjectInfoString();
    private:
        LPCTSTR m_pszGdiObjectInfo;
    };

    class CFGdiObjectChecker
    {
    public:
        FTLINLINE CFGdiObjectChecker(HDC hDCTarget = NULL);
        FTLINLINE ~CFGdiObjectChecker();
        FTLINLINE BOOL IsValidate(HGDIOBJ hGdiObj);
        FTLINLINE BOOL IsSelected(HBITMAP hGdiObj);
    private:
        HDC     m_hDCCompatible;
    };

    //必然会获取的信息
#define HDC_PROPERTY_GET_MAPMODE        0x00000000
#define HDC_PROPERTY_GET_WINDOW_INFO    0x00000001
#define HDC_PROPERTY_GET_VIEWPORT_INFO  0x00000002
#define HDC_PROPERTY_GET_MODE           0x00000004
#define HDC_PROPERTY_GET_DEVICE_CAPS    0x00000008

#define HDC_PROPERTY_GET_BITMAP         0x00000010

#define HDC_PROPERTY_GET_ALL            0xFFFFFFFF
#define HDC_PROPERTY_GET_DEFAULT        0x00000010

    struct HDCProperty
    {
    public:
        FTLINLINE HDCProperty();
        FTLINLINE ~HDCProperty();
    public:
        FTLINLINE LPCTSTR GetPropertyString(DWORD dwPropertyGet = HDC_PROPERTY_GET_DEFAULT);
		HWND	m_hWnd;
        HDC     m_hSafeHdc;
        int     m_nGraphicsMode;
        int     m_nMapMode;
        int     m_nArcDirection;
        
        //可填充绘图函数的条纹或图案刷的起点：（默认值为客户区左上角的坐标原点（0, 0））
        POINT   m_ptBrushOrg;

        POINT   m_ptWindowOrg;
        SIZE    m_szWindowExt;                  //逻辑坐标(如 2000,-2000)
        POINT   m_ptViewportOrg;
        SIZE    m_szViewportExt;                //设备坐标(如 WM_SIZE 中获取到的大小)

        int     m_nDeviceCapsDriverVersion;     //(DRIVERVERSION),如 0x4000
        int     m_nDeviceCapsTechnology;        //(TECHNOLOGY), 如 1
        int     m_nDeviceCapsHorzsize;          //(HORZSIZE), 如 337
        int     m_nDeviceCapsVertsize;          //(VERTSIZE), 如 270
        int     m_nDeviceCapsHorzres;           //屏幕宽(HORZSIZE),如 1024
        int     m_nDeviceCapsVertres;           //屏幕高(VERTSIZE),如 768
        int     m_nDeviceCapsLogPixelsX;        //水平上每逻辑英寸的像素数, 如 96 或 120
        int     m_nDeviceCapsLogPixelsY;        //垂直上每逻辑英寸的像素数, 如 96 或 120
        int     m_nDeviceCapsBitsPixel;         //颜色深度(BITSPIXEL),如 32
        int     m_nDeviceCapsPlanes;            //(PLANES), 如 1
        int     m_nDeviceCapsNumBrushes;        //(NUMBRUSHES), 如 -1
        int     m_nDeviceCapsNumPens;           //(NUMPENS), 如 -1
        int     m_nDeviceCapsNumFonts;          //(NUMFONTS), 如 0
        int     m_nDeviceCapsNumColors;         //(NUMCOLORS), 如 -1
        int     m_nDeviceCapsAspectX;           //(ASPECTX), 如 36
        int     m_nDeviceCapsAspectY;           //(ASPECTY), 如 36
        int     m_nDeviceCapsAspectXY;          //(ASPECTXY), 如 51
        int     m_nDeviceCapsPDeviceSizeReserved;   //(PDEVICESIZE), 如 0
        int     m_nDeviceCapsClipCaps;          //(CLIPCAPS), 如 1
        int     m_nDeviceCapsSizePalette;       //(SIZEPALETTE), 如 0
        int     m_nDeviceCapsNumReserved;       //(NUMRESERVED), 如 20
        int     m_nDeviceCapsColorRes;          //(COLORRES), 如 24
        int     m_nDeviceCapsPhysicalWidth;     //(PHYSICALWIDTH), 如 0
        int     m_nDeviceCapsPhysicalHeight;    //(PHYSICALHEIGHT), 如 0
        int     m_nDeviceCapsPhysicalOffsetX;   //(PHYSICALOFFSETX), 如 0
        int     m_nDeviceCapsPhysicalOffsetY;   //(PHYSICALOFFSETY), 如 0
        int     m_nDeviceCapsVRefresh;          //(垂直刷新率), 如 60
        int     m_nDeviceCapsScalingFactorX;    //(SCALINGFACTORX), 如 0
        int     m_nDeviceCapsScalingFactorY;    //(SCALINGFACTORY), 如 0
        int     m_nDeviceCapsBltAlignment;      //(BLTALIGNMENT), 如 0
        int     m_nDeviceCapsShadeBlendCaps;    //(SHADEBLENDCAPS), 如 3
        int     m_nDeviceCapsRasterCaps;        //(RASTERCAPS), 如 0x7E99
        int     m_nDeviceCapsCurveCaps;         //(CURVECAPS), 如 0x1FF
        int     m_nDeviceCapsLineCaps;          //(LINECAPS), 如 0xFE
        int     m_nDeviceCapsPolygonalCaps;     //(POLYGONALCAPS), 如 0xFF
        int     m_nDeviceCapsTextCaps;          //(TEXTCAPS), 如 0x7807(TC_VA_ABLE|TC_RA_ABLE...)
        int     m_nDeviceCapsColorMgmtCaps;     //(COLORMGMTCAPS), 如 2
        COLORREF m_BkColor;
        int     m_BkMode;
        int     m_nROP2;
        int     m_nStretchBltMode;
        int     m_nPolyFillMode;

        //--如果调用了该函数，映射从世界坐标系空间开始，否则映射在页面空间中进行？
        //MSDN 有例子
        XFORM   m_xForm;    

        //文本的对齐方式 -- TODO,获取信息
        int     m_TextAlign;
        RECT    m_rcClipBox;
        POINT   m_ptCurrentPosition;

        //HBITMAP m_hCurBitmap;
        BITMAP  m_curBitmapInfo;
        //LOGPEN  m_curPenInfo;      //GetCurrentFont
    private:
        LPCTSTR m_pszProperty;
        DWORD   m_dwOldGetProperty;
    };
    
    struct BitmapProperty
    {
    public:
        FTLINLINE BitmapProperty(const BITMAP& bitmap);
        FTLINLINE ~BitmapProperty();
        FTLINLINE LPCTSTR GetPropertyString();

        BITMAP    m_bitmap;
    private:
        LPCTSTR     m_pszProperty;
    };

	//X坐标向右，Y坐标向下 -- 点相对于矩形的象限位置
	enum PointPosQuadrant
	{
		//posTop,
		posTopRight = 0,
		posRightTop,
		//posRight,
		posRightBottom,
		posBottomRight,
		//posBottom,
		posBottomLeft,
		posLeftBottom,
		//posLeft,
		posLeftTop,
		posTopLeft,
	};

    FTLEXPORT class CFGdiUtil
    {
    public:
		//计算一个点对应特定矩形的相对关系
		FTLINLINE static PointPosQuadrant CalcPointPosQuadrant(const RECT& rect, const POINT& point);

        //计算并按指定的间隔(行间距)绘制文本
		FTLINLINE static int DrawInterValText(HDC hdc, LPCTSTR lpchText, int cchText,	LPRECT lprc, UINT format, POINT ptInterval);

		FTLINLINE static BOOL LoadPNGFromResource(CImage& image, HMODULE hModule, UINT nIDResource, LPCTSTR pszType = TEXT("PNG"));
        FTLINLINE static BOOL LoadPNGFromResource(Gdiplus::Image *& pImage, HINSTANCE hInstance, UINT nIDResource, LPCTSTR pszType = TEXT("PNG"));

        FTLINLINE static LPCTSTR GetGraphicsModeString(int nGraphicsMode);

        //背景模式 -- 影响有空隙的图元中的空隙用什么办法填充
        FTLINLINE static LPCTSTR GetBkModeString(int nBkMode);
        
        FTLINLINE static LPCTSTR GetMapModeString(int nMapMode);
        FTLINLINE static LPCTSTR GetMapModeDescString(int nMapMode);

        //绘图模式(Raster OPeration，光栅操作) -- 前景色（及原有色）的混合方式
        //  决定当前画图的笔和刷的颜色（pbCol）如何与原有图的颜色（scCol）相结合而得到结果像素色（pixel）
        FTLINLINE static LPCTSTR GetROP2String(int fnDrawMode);

        //拉伸模式,影响 StretchBlt 函数的操作效果
        FTLINLINE static LPCTSTR GetStretchBltModeString(int nStretchMode);

        //获取BitBlt等函数使用的模式
        FTLINLINE static LPCTSTR GetBitBltRopString( DWORD dwRop );

        //多边形填充方式 -- SetPolyFillMode
        FTLINLINE static LPCTSTR GetPolyFillModeString(int nPolyFillMode);

        //画弧方向 -- 确定Arc、Chord、Pie等函数的画弧方向
        FTLINLINE static LPCTSTR GetArcDirectionString(int nArcDirection);

        //获取Region的Type
        FTLINLINE static LPCTSTR GetRegionType(int nType);

        FTLINLINE static LPCTSTR GetTextAlignString(int nFlags);
        FTLINLINE static BOOL    GetHDCProperty(HDC hdc, HDCProperty* pProperty);

		FTLINLINE static LPCTSTR GetTextMetricsInfo(HDC hdc, CFStringFormater& strFormater , HFONT hFont = NULL);

		//获取LogFont结构对应的信息
		FTLINLINE static LPCTSTR GetLogFontInfo(CFStringFormater& strFormater, LOGFONT* pLogFont);

		//根据传入的 10*磅， 得到对应的字号(如 初号、小初等)
		FTLINLINE static LPCTSTR GetFontNumberByPointSize(int nPointSize);
		
        //在HDC上绘制坐标系统的信息(映射模式、9个标准点的坐标)，从而帮助调试
        FTLINLINE static BOOL   DrawCoordinate(HDC hdc, const RECT& rcClient, BOOL bDrawText = TRUE,
            BOOL bDrawClipbox = FALSE);
		
		//通过九宫格图片的拉伸效果进行绘制
		//FTLINLINE static BOOL DrawNineBlockImage(HDC hdc, )

        //设置基于 一逻辑单位对应的逻辑大小 进行映射
        enum LogicalMapMode
        {
            lmmLoEnglish,   //0.01  逻辑英寸 -- SetWindowExtEx(100)
            lmmHiEnglish,   //0.001 逻辑英寸 -- SetWindowExtEx(1000)
            lmmLoMetric,    //0.1   逻辑毫米 -- SetWindowExtEx(254)
            lmmHiMetric,    //0.01  逻辑毫米 -- SetWindowExtEx(2540)
            lmmTwips        // 1/1440逻辑英寸(逻辑缇) ? -- SetWindowExtEx(1440)
        };
        //注意实现中的 xWinExt/yWinExt 等参数可以取正负，即能转换 X/Y 轴的方向，
        //然后可通过 OffsetWindowOrg(-x,y)移动原点，使得屏幕平均显示四个象限等(参见 MFC 中的 DrawClient )
        FTLINLINE static BOOL   SetLogicalMapMode(HDC hdc, LogicalMapMode logicalMapmode);

		FTLINLINE static BOOL SaveBitmapToFile(HBITMAP hBmp, LPCTSTR pszFilePath);
		FTLINLINE static BOOL SaveDCImageToFile(HDC hdc, LPCTSTR pszFilePath);

		//将位图中指定颜色的点全部转换为特定的颜色(做透明色?) -- 如将 light gray(RGB (192, 192, 192)) 替换成 ::GetSysColor (COLOR_BTNFACE)
		//  ReplaceBitmapColor(hBmp, RGB(192,192,192), ::GetSysColor (COLOR_BTNFACE));
		//http://www.codeguru.com/cpp/controls/toolbar/article.php/c2537/FullFeatured-24bit-Color-Toolbar.htm
		FTLINLINE static BOOL ReplaceBitmapColor(HBITMAP hBmp, COLORREF clrFrom, COLORREF clrTo);

        //比较位图的图像数据, 返回值 >= 0 表示不同颜色的数目, < 0 表示失败
        FTLINLINE static int ComapreBitmapData(int nWidth, int nHeight, int bpp, void* pBmp1, byte* pBmp2, byte* pOutResult, int nResultSize);
    };

	//根据指定的方式，计算一个RECT内(居中)包含一个SIZE时的RECT位置，通常用于窗体内居中显示图片或视频
	FTLEXPORT class CFCalcRect
	{
	public:
		enum ZoomMode
		{
			modeAutoFitIfBigSize,		
			modeAutoFit,					//根据高宽比例自动内填充矩形
			//modeWidthFit,
			//modeHeightFit,
			//modeOriginal,
			//E_ZOOM_AUTO_FIT_IF_BIGSIZE,
			//E_ZOOM_AUTO_FIT,
			//E_ZOOM_WIDTH_FIT,
			//E_ZOOM_HEIGHT_FIT,
			//E_ZOOM_ORIGINAL,
			//E_ZOOM_NORMAL
		};
		FTLINLINE static 	CFCalcRect*	GetCalcRectObject( ZoomMode nZoomMode );
	public:
		//只计算居中对齐时的值，非居中(如左上角对齐)的方式很简单
		virtual CRect GetFitRect( const CRect& rcMargin, const CSize& szContent ) = 0; 
	};

	FTLEXPORT class CFAutoFitIfBigSizeCalcRect : public CFCalcRect
	{
	public:
		FTLINLINE virtual CRect GetFitRect( const CRect& rcMargin, const CSize& szContent );
	};

	FTLEXPORT class CFAutoFitCalcRect : public CFCalcRect
	{
	public:
		FTLINLINE virtual CRect GetFitRect( const CRect& rcMargin, const CSize& szContent );
	};

    FTLEXPORT class CFCanvas
    {
    public:
        FTLINLINE CFCanvas();
        FTLINLINE ~CFCanvas();
        FTLINLINE BOOL Create(HWND hWnd, int width, int heigth, int bpp = 32);//bpp -- bits-per-pixel
        FTLINLINE VOID Release();
        FTLINLINE int GetWidth() const { return m_width; }
        FTLINLINE int GetHeight() const { return m_height; }
        FTLINLINE int GetBpp() const { return m_bpp; }
        FTLINLINE int GetPitch() const { return m_width * m_bpp >> 3; }
        FTLINLINE BOOL  IsCanvasChanged( const RECT& rc , int bpp = 32 );
        FTLINLINE BYTE* GetBuffer() { return m_pBuffer; }
        FTLINLINE HDC   GetCanvasDC() const { return m_hCanvasDC; }
        FTLINLINE  operator HDC() const { return m_hCanvasDC; }
        FTLINLINE HBITMAP GetMemoryBitmap() const { return m_hMemBitmap; }
        FTLINLINE HANDLE CopyToHandle();
        FTLINLINE DWORD GetSize();
    private:
        HDC     m_hCanvasDC;
        HBITMAP m_hMemBitmap;
        HBITMAP m_hOldBitmap;
        union {
            BITMAPINFO  m_bmpInfo;
            BYTE m_ReserveSpace[sizeof(BITMAPINFO) + 0xFF * sizeof(RGBQUAD)];
        };
        
        BYTE*   m_pBuffer;
        int     m_width;
        int     m_height;
        int     m_bpp;
    };

    template <typename T>
    //按需绘制 的模版类 -- 考虑如何实现
    class CClipDrawer
    {
    public:
        void OnPaint(HDC hDC);
    };

    template <typename T>
    FTLEXPORT class CFGdiResourceGuardT
    {
    public:
        FTLINLINE CFGdiResourceGuardT(HDC hdc, T hNewObject);
        FTLINLINE ~CFGdiResourceGuardT();
        BOOL      IsValide();
    private:
        HDC m_hDC;
        T   m_hOldObject;
    };

    template <typename T>
    CFGdiResourceGuardT<T>::CFGdiResourceGuardT(HDC hdc, T hNewObject)
        :m_hDC(hdc)
    {
        m_hOldObject = (T)::SelectObject(hdc, (T)hNewObject);
    }

    template <typename T>
    CFGdiResourceGuardT<T>::~CFGdiResourceGuardT()
    {
        ::SelectObject(m_hDC, m_hOldObject);
    }

    template <typename T>
    BOOL CFGdiResourceGuardT<T>::IsValide()
    {
        return (NULL != m_hOldObject && HGDI_ERROR != m_hOldObject);
    }

    //对 region 作特化
    //CFGdiResourceGuardT<HRGN>::IsValide()
    //{
    //}

#ifdef __ATLGDI_H__
    //ATL 提供的标准 CMemoryDC 不支持 Zoom 和 Scroll(至少位图创建方式没有转换为设备坐标的像素--位图的宽高单位都是像素)
	FTLEXPORT class CFScrollZoomMemoryDC : public CDC
    {
    public:
        // Data members
        HDC m_hDCOriginal;
        RECT m_rcPaint;
        HBITMAP m_hBmp;
        HBITMAP m_hBmpOld;
    public:
        //pRectPaint is logical units -- default is GetClipBox
        FTLINLINE CFScrollZoomMemoryDC(HDC hDC, RECT* pRectPaint);
        FTLINLINE ~CFScrollZoomMemoryDC();
        //operator CFScrollZoomMemoryDC*()
        //CFScrollZoomMemoryDC* operator->()
    };
#endif //__ATLGDI_H__

	class CFMMTextDCGuard
	{
	public:
		explicit CFMMTextDCGuard(HDC hdc)
			:m_hDC(hdc)
		{
			BOOL bRet = FALSE;
			ATLASSERT(m_hDC);
			API_VERIFY(::SetWindowOrgEx(m_hDC, 0,0, &m_ptOldWindowOrg));
			API_VERIFY(::SetViewportOrgEx(m_hDC, 0,0, &m_ptOldViewPortOrg));
			API_VERIFY(::SetViewportExtEx(m_hDC, 1,1, &m_szOldViewPortExt));
			API_VERIFY(::SetWindowExtEx(m_hDC, 1,1,&m_szOldWindowExt));
			m_nOldMapMode = ::SetMapMode(m_hDC, MM_TEXT);

			//FTLTRACE(TEXT("CFMMTextDCGuard, MapMode=%d,")
			//	TEXT("szOldWindowExt={%d,%d}, szOldViewPortExt={%d,%d},")
			//	TEXT("ptOldWindowOrg=[%d,%d], ptOldViewPortOrg=[%d,%d]\"),
			//	m_nOldMapMode,
			//	m_szOldWindowExt.cx, m_szOldWindowExt.cy, m_szOldViewPortExt.cx, m_szOldViewPortExt.cy,
			//	m_ptOldWindowOrg.x, m_ptOldWindowOrg.y,m_ptOldViewPortOrg.x, m_ptOldViewPortOrg.y);

		}
		~CFMMTextDCGuard()
		{
			BOOL bRet = FALSE;
			::SetMapMode(m_hDC, m_nOldMapMode);
			API_VERIFY(::SetWindowExtEx(m_hDC, m_szOldWindowExt.cx,m_szOldWindowExt.cy,NULL));
			API_VERIFY(::SetViewportExtEx(m_hDC, m_szOldViewPortExt.cx,m_szOldViewPortExt.cy, NULL));
			API_VERIFY(::SetViewportOrgEx(m_hDC, m_ptOldViewPortOrg.x, m_ptOldViewPortOrg.y, NULL));
			API_VERIFY(::SetWindowOrgEx(m_hDC, m_ptOldWindowOrg.x,m_ptOldWindowOrg.y, NULL));
		}
	private:
		HDC		m_hDC;
		int		m_nOldMapMode;
		SIZE	m_szOldWindowExt;
		SIZE    m_szOldViewPortExt;
		POINT   m_ptOldViewPortOrg;
		POINT   m_ptOldWindowOrg;
	};
}

#endif //FTL_GDI_H

#ifndef USE_EXPORT
#  include "ftlGdi.hpp"
#endif