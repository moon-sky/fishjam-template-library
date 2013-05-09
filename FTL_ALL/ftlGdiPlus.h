
#ifndef FTL_GDIPLUS_H
#define FTL_GDIPLUS_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlGdiPlus.h requires ftlbase.h to be included first
#endif

#include <GdiPlus.h>

/*****************************************************************************************************
* 使用GDI+前需要 GdiplusStartup 初始化， 使用完毕后通过 GdiplusShutdown 终止化
* Gdi+ 有 1.0 和 1.1 两个版本(现在是否有更新的?)
*
* GDI和GDI+的区别：
*   ★GDI是有状态的，GDI+是无状态的， DC <==> Graphics 对象
*   ★GDI的形状轮廓绘制和填充使用同一个函数，比如Rectangle(轮廓用画笔，填充用画刷)，
*     而GDI+是分开的，如 DrawRectangle 和 FillRectangle
*     GDI的坐标是整型，在转换时会丢失精度，GDI+里面的坐标可以采用REAL(float)，因此在缩放等处理中不会丢失精度
/*****************************************************************************************************/

/*****************************************************************************************************
* GDI+ 的坐标系统 -- GDI+ 使用三个坐标空间
*    全局 -- DrawLine 等方法参数的坐标值位于全局坐标空间中
*    页面 -- 原点总在工作区的左上角
*    设备 -- 显示器或打印机 等?
*  坐标通过变形系列才能显示在屏幕上
*    全局坐标 =(全局变形,Graphics::XXXTransform)=> 
*       页面坐标 =(页面变形,PageUnit+PageScale)=> 
*         设备坐标
*    PageUnit -- 指定像素以外的其他度量单位
*    全局变形应用于给定的 Graphics 对象绘制的每个项目的变形，通常用于设置坐标系统，
*      GDI+提供了 MultiplyTransform、RotateTransform、ScaleTransform 和 TranslateTransform 等
*    绘制对象上可以做局部变形(对象的Transform) -- 通常用于旋转和缩放在新坐标系统上绘制的对象
*    
*  变形也可以通过矩阵形式进行表达 -- GDI+中通过Matrix保存变形矩阵, 变形参数 MatrixOrderAppend ?
*    旋转(Rotate/RotateAt) -- 单位为度
*    缩放(Multiply/Scale)：[x,y] X [变换矩阵(2x2)]
*    平移(Shear/Translate)： [x,y] + [偏移向量(1x2)]
*    仿射变形 -- 使用3x3的矩形(像素最后一维始终为1)，将旋转,缩放,平移一起进行(似乎矩阵值的意义不对?)：
*                [ X 缩放 , X 投影?, 0 ]
*      [x,y,1] X [ Y 投影?, Y 缩放 , 0 ]
*                [ X 平移 , Y 平移 , 1 ]
*      变形矩阵中， [0,0]到[2x2]处为线性部分， [3,1]到[3,2]处为平移部分
*      因为最后一列的值固定，Matrix构造中只需要指定前两列的6个值
*    复合变形 -- 多个变形序列(如旋转、缩放、平移)矩阵相乘得到，注意矩阵的顺序会影响结果
*      
*  默认坐标系统(GDI 的 MM_TEXT) -- 原点是在左上角，并且 x 轴指向右边，y 轴指向下边，单位为像素
*  变形 -- GDI+提供了全局变形和页面变形，以便您可以使绘制的项目变形（旋转、缩放、平移，等等） 
*
* GDI+对GDI的很多性能进行了优化 -- 但据说gdi+直接使用CPU，因此无硬件优化，如开启反锯齿的情况下绘制简单曲线也会
*   导致效率问题。因此需要区分静态、动态场景以减少重绘时的工作量。
*   Gdi+中的 DrawImage 的性能极其糟糕，相对应 GDI 中的BitBlt是快速高效的内存拷贝函数。
*     解决方法为：GDI+的绘图函数绘制到内存DC中，然后使用位拷贝的方式进行绘制显示。
*       WGF(windows graphic foundation)--有人实现的一个 GDI/GDI+ 的双缓冲绘图机制
*       http://www.pudn.com/downloads219/sourcecode/windows/bitmap/detail1030808.html
* GDI中可应用于区域的唯一变形是平移，而GDI+ 在全局坐标中存储区域，且允许区域发生任何可存储在变形矩阵中的变形（例如缩放）?
* 浮点数坐标 ? PointF 
* 
* 三个部分
*   2D矢量图(2-D vector graphics)
*   图像(Imaging)
*   印刷格式(Typography) -- 版式关系到使用各种字体、字号和样式来显示文本
* 
* 相关类:
*   Graphics(核心)--可在构造函数中通过HDC或静态的FromImage等初始化，具有用于绘制直线、矩形、路径、图形、图像和文本的方法
*     DrawPath -- 绘制路径对象(可以一次性绘制出整个路径对象序列包含的直线、矩形、多边形和曲线等)
*     DrawImage -- 绘制图像，但性能似乎很差(最好用GDI绘制?)
*     质量属性
*       CompositingMode -- 
*       CompositingQuality --
*       InterpolationMode -- 
*       PixelOffsetMode -- 
*       SmoothingMode -- 控制平滑模式(是否使用消除锯齿的技术)，如 AntiAlias 反锯齿
*       TextRenderingHint -- 柔化文本的锯齿边缘，如 SystemDefault(使用用户已经配置的任何字体平滑设置), SingleBitPerPixelGridFit(速度最快但质量最低),
*         AntiAliasGridFit(更好的质量但较低的速度), ClearTypeGridFit(在LCD显示屏上具有最好的质量), 
*     图形容器(BeginContainer/EndContainer) -- 对 Graphics 对象做出的任何状态更改都属于容器,
*       不会改写Graphics对象的现有状态(如 剪辑区域、变形和质量设置 等)
*
*   Brush
*     SolidBrush(固定色)/HatchBrush(阴影)/TextureBrush(纹理画刷--位图)/GradientBrush(渐变,分线性Linear和路径Path)
*   CachedBitmap -- 通过缓存绘制的图形来优化显示，注意判断 DrawCachedBitmap 方法的返回值，如果不为OK说明显示属性有变化（比如？），
*     需要重新创建缓存位图。
*   GraphicsPath -- 路径对象，可创建待绘制项目序列，路径可视为由各种绘图函数产生的轨迹
*     AddLine/AddEllipse/AddBezier/AddPath/AddString--在路径中增加路径对象
*     Flatten -- 拉平路径，可将路径中的各条贝塞尔样条转化为一连串直线
*   Image -- 抽象基类
*     >DrawImage -- 其中的 UnitPixel 等参数意义?
*     Bitmap -- 加载、保存和处理光栅图像，如 BMP、JPEG、PNG、ICON 等
*       Clone -- 制作副本，可指定原矩形参数来指定要复制的原始位图的部分，返回的结果需要delete?
*       BitmapData的
*     Metafile -- 记录、显示和保存图元文件(图形命令的序列，矢量图像)，如 EMF
*       MetafileHeader、MetaHeader -- 检查图元文件头中存储的数据
*       注意：GDI+支持EMF/EMF+，但不支持WMF
*
*     GetRawFormat -- 获得格式的GUID
*   Matrix -- 存储并操作 Graphics 对象的变形矩阵对象，用于平移、旋转、缩放和翻转等。通过被变形对象的Transform调用。
*   Pen
*     SetDashStyle--指定直线的特性
*     StartCap 和 EndCap -- 属性指定线条两端的外观
*     LineJoin -- 属性可用于指定连接的线相互间是斜接的（连接时形成锐角）、斜切的、圆形的还是截断的
*   Region -- 常用于
*       1.剪辑(将绘制限制到显示区域的一个特定区域) -- 设置 Graphics.Clip 属性
*       2.命中检测(确定按下鼠标按钮时光标是否在屏幕的特定区域)
*     Complement -- ? 不在该区域的所有点的集合
*     Exclude -- 补集，包含在第一个区域中而不在第二个区域中的所有点
*     Intersect -- 交集,同时属于两个区域的所有点的集合
*     Union -- 并集，属于两个区域中任意一个的所有点的集合
*     Xor -- 异或，只属于一个区域或另一个区域但不同时属于两个区域的所有点。
*   StringFormat -- 
*   LineJoin -- 指定用于连接两个线条的样式
* 
* ImageEncoder -- GetImageEncoders,可获得对应的CLSID
* Gdi+的错误码， Gdiplus::OK 
*
* 相对GDI的新功能
*   Alpha Blending(混合) -- 可以指定填充颜色的透明度。透明色与背景色相混合 填充色越透明，背景色的透出程度就越高。
*   子像素消除锯齿(用楼梯状来表示直线的技术) -- 可以使文本在 LCD 屏幕上呈现时显得比较平滑
*     消除锯齿 -- 通过将路线上的像素设为纯色或前景色和背景色的混合色（取决于和直线的接近程度），可以生成视觉上更感平滑的直线
*   渐变画刷(GradientBrush) -- 填充图形、路径和区域的线性渐变画笔和路径渐变画笔
*     LinearGradientBrush -- 线性渐变画笔
*     PathGradientBrush -- 路径渐变画笔
*   基数样条(Cardinal Splines,木工画线的方法) -- 基数样条是一连串单独的曲线，这些曲线连接起来形成一条较大的曲线。
*     基数样条平滑地(没有锐角)通过数组中的每一个点，比通过连接直线创建的路径更精准。
*     DrawCurve/DrawClosedCurve，样条由点的数组和张力参数(tension)指定。tension 值:
*       0 -- 表示对应于无限的物理张力(直线)
*       1 -- 没有物理张力,样条采用最小完全弯曲的路径
*       1<tension -- 类似压缩的弹簧，被挤压着采用较长的路径，值越大，路径越长
*   贝塞尔样条(DrawBezier) -- 由四个点指定的曲线：两个终点(p1和p2)和两个控制点(c1和c2)。
*     控制点像磁铁一样，在某些方向上拉拽曲线并影响曲线弯曲的方式，但曲线不经过控制点。
* 
* 区域
*   GDI+ 中的 Region 类提供一个接收 Rectangle 对象的构造函数和另一个接收 GraphicsPath 对象的构造函数。
*   如想创建非矩形(如椭圆)区域，需要创建一个包含对应形状的GraphicsPath对象，然后将其传递至 Region 构造函数。
*****************************************************************************************************/

namespace FTL
{

    # define GDIPLUS_VERIFY(x)   \
    sts = (x);\
    if(Gdiplus::Ok != sts)\
    {\
        REPORT_ERROR_INFO(FTL::CFGdiPlusErrorInfo, sts, x);\
    }


    FTLEXPORT class CFGdiPlusErrorInfo : public CFConvertInfoT<CFGdiPlusErrorInfo, Gdiplus::Status>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFGdiPlusErrorInfo);
    public:
        FTLINLINE explicit CFGdiPlusErrorInfo(Gdiplus::Status status);
        FTLINLINE virtual LPCTSTR ConvertInfo();
    };

    class CFAutoGdiplus
	{
	public:
		FTLINLINE CFAutoGdiplus();
		FTLINLINE ~CFAutoGdiplus();
	private:
		Gdiplus::GdiplusStartupInput	m_gdiplusStartupInput;
		ULONG_PTR						m_gdiplusToken;
	};

	class CFGdiPlusUtil
	{
	public:
		//通过九宫格图片的拉伸效果进行绘制 -- TODO:验证和实现GDI版本
		//strFileName.Format(_T("Chat_other_bubble_%d.png"), i);
		//m_pImgOtherBubble[i]  = new Image(GetImagePath(strFileName));

		FTLINLINE static BOOL DrawNineBlockImage(Gdiplus::Graphics* pGraphics, Gdiplus::Image* pImages[9], const Gdiplus::Rect* pRect);
	};
}

#endif //FTL_GDIPLUS_H

#ifndef USE_EXPORT
#  include "ftlGdiPlus.hpp"
#endif