#pragma once

#include "ColorQuantizerBase.h"

/*******************************************************************************************************
* a.giflib 中的量化算法?
* 2.http://www.codeproject.com/Articles/109133/Octree-Color-Palette
*
* 八叉树颜色量化算法
*   1.扫描图像的所有像素，将它们的数据累加到相应的节点中；遇到新的颜色则创建一个叶子节点，并将此像素的颜色数据存入其中。
*   2.如果叶子节点数目大于目标调色板所要的颜色数，就将部分叶子节点合并到父节点中，并将父节点转化为叶子节点，
*     在其中存放颜色分量的累加值及其像素出现的次数。同时删除原来被合并的叶子节点。
*   3.所有像素处理结束，遍历八叉树，将叶子节点的各颜色值求平均值作为节点的颜色分量值读出并存入目标调色板。
*   4.再次遍历所有像素，通过每个像素的颜色值与调色板中选中的256色运算，求得一个最接近像素颜色值的调色板颜色，把该像素换相应的调色板颜色索引
*******************************************************************************************************/

namespace FTL
{
    class CFOctreeColorQuantizer : public CFColorQuantizerBase
    {
    public:
        CFOctreeColorQuantizer();
        virtual ~CFOctreeColorQuantizer();
    protected:
        virtual BOOL OnPrepare();
        virtual BOOL OnProcessQuantizer(UINT colorCount, UINT *pResultClrCount);
        virtual void OnFinish();
    private:
        class CFOctreeNode{
        private:

            static BYTE s_MASK[8];// = new Byte[] { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
            unsigned char Red;
            unsigned char Green;
            unsigned char Blue;

            int pixelCount;
            int paletteIndex;

        };
    };
}
