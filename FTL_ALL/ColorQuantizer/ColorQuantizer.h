#pragma once
#include <ftlBase.h>

/*******************************************************************************************************
* 人眼视觉对RGB三基色的灵敏度各不相同，在量化过程中可以根据不同颜色对象使用不同特性的压扩变换方法，以取得最佳量化效果。
* 三种颜色对亮度影响系数为 R:G:B=0.299:0.597:0.114, 通常用相对影响系数 2:1:4
*
* 颜色量化算法(如 24位真彩转256色 ) -- https://github.com/jsummers/imageworsener/issues/2
*   聚类算法(Clustering Algorithm) -- 
*     原理:选择若干颜色作为聚类中心，按照距离准则使每种颜色向各个中心聚集，从而得到分类。
*     优点:对一般的颜色图像进行量化处理均能得到较好的量化效果
*     缺点：量化过程的空间开销较大，而且对初始类中心的选取非常重要，特别对色彩分布不均匀的图像，量化效果不够理想。
*            实现时需要不断聚合和调整聚类中心，计算量打，效率较低。
*   中位切分算法(Median Cut Algorithm)。TODO:原理和CodeProject上介绍的似乎不一致
*     原理:在RGB彩色空间中，R、G、B三基色对应于空间的三个坐标轴，将每一坐标轴都量化为0～255,
*           形成一个边长为256的彩色立方体，所有可能的颜色都对应于立方体内的点。取每个小立方体的中心点颜色。
*     CodeProject上的原理:像素放置在RGB空间中，找出包含所有颜色的最小box; 然后选取最长的轴，取中间点分成两个box，并递归直到得到
*           需要的N个box，最后求平均值。
*     优点:图像质量可以接受
*     缺点:内存消耗大，速度慢
*   NeuQuant算法
*     原理:使用 Kohonen Neural Network(神经网络)
*     优点：可以在质量和效率之间进行自由选择，支持更高颜色位数
*   八叉树颜色量化算法(Octree Color Quantization Algorithm) 
*     原理:将图像中使用的RGB颜色值分布到层状的八叉树中。八叉树的深度可达9层，即根节点层加上分别表示8位的R、G、B值的每一位的8层节点。
*          较低的节点层对应于较不重要的RGB值的位(右边的位)，为了提高效率和节省内存，可以去掉最低部的2 ~ 3层，对量化结果不会有太大影响。
*     优点:效率高、效果好
*     缺点:内存消耗大
*   优化的调色板算法(Optimized Palette Algorithm) -- 
*     原理:使用系统调色板?
*     优点:非常快，几乎没有内存消耗
*     缺点:图像质量很差,只支持256色
*   流行色算法(Popularity Algorithm)
*     原理:对彩色图像中所有色彩出现的次数进行统计分析，从而选取频率最高的N(256)种颜色，为这N(256)种颜色建立调色板，
*          其余的颜色用最小距离原则映射到最临近的调色板颜色上。为了提高效率，通常也是分层小区域进行统计
*     优点:算法简单容易实现,对彩色数量小的图像变换效果好
*     缺点:一些出现频率较低，但对人眼视觉效果明显的信息将丢失(如高亮度斑点)
*   空间色彩量化(Spatial Color Quantization) -- 较新的算法? 
*     原理:结合了抖动(dithering),http://moonflare.com/code/scolorq/ -- 人眼对亮度的变化更敏感,因此需要减少对亮度的量化效果?但需要保留亮度急剧变化的部分(边缘)
*     优点:图像质量很好
*     缺点:速度慢
*   Uniform Quantization(直接量化法)
*     原理:转16位色：直接减少RGB三个颜色分量表示的位数，保留颜色分量的高位，去除低位。如555、565 和 444 分法,
*          转256色：颜色分量分成几个固定段，如  8-8-4(255色), 6-6-7(252色)，每个颜色被放置到相应的段中，然后每个段中的所有颜色相加计算平均值。
*     优点:简单快速，内存消耗低
*     缺点:没有选择性，比较盲目，质量差，量化的空间有限
*   Wu's Color Quantizer(Xiaolin Wu)
*     优点:图像质量最好
*     缺点:很慢，内存消耗大。TODO: 慢的原因是"scans and adds colors" 部分对 所有像素x颜色表像素 进行了二重循环，
*           来计算每一个像素最匹配的颜色表像素值，可以考虑用 分而治之的算法(距离最近的点对) 进行优化
*******************************************************************************************************/

namespace FTL
{
    //COLORREF 中一个元素(R/G/B)的无效值

    //#define INVALID_COLOR_META      (unsigned char)(-1)
    #define MAX_COLOR_MAP_COUNT (256)

    class IFColorQuantizer{
    public:
        virtual ~IFColorQuantizer();
        
        virtual BOOL SetBmpInfo(UINT nWidth, UINT nHeight, UINT nBpp, BYTE* pBmpData, UINT nBmpDataSize, BOOL bCopyMem) = 0;
        virtual BOOL ProcessQuantizer(UINT nWantClrCount, UINT *pResultClrCount) = 0;

        virtual COLORREF* GetPalette(UINT* pResultCount) = 0;
        virtual UCHAR* GetQuantizerResult(UINT* pBufferSize) = 0;
    };
}
