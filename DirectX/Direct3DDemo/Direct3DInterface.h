#ifndef DIRECT3D_INTERFACE_H
#define DIRECT3D_INTERFACE_H
#pragma once

/*****************************************************************************************************
* 常见Direct3D的接口
*   IDirect3D9 -- 最基本的接口，用于获得物理设备的信息和创建 IDirect3DDevice9 等接口。
*     CheckDeviceMultisampleType -- 检查是否支持全屏抗锯齿
*     GetDeviceCaps -- 获得设备的特性(如 D3DCAPS9::DevCaps &  D3DDEVCAPS_HWTRANSFORMANDLIGHT 
*       -- 判断是否支持硬件顶点处理[硬件几何变换和光源计算])
*   IDirect3DDevice9 -- 代表显示3D图形的物理设备
*     BeginScene/EndScene -- 开始和结束绘制场景(所有绘制方法都必须在这两个方法之间)
*     Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0) -- 清空后备表面和深度/模版缓冲
*     CreateIndexBuffer -- 创建索引缓存
*     CreateVertexBuffer -- 创建顶点缓存
*     DrawPrimitive/DrawIndexedPrimitive --绘制图元(点、线、三角形等)，通过传送几何信息到达渲染管线使用顶点/索引缓存进行绘制。
*     LightEnable -- 打开或关闭灯光
*     Present(0, 0, 0, 0) -- 进行后备表面的交换
*     SetFVF -- 设置顶点格式（需要根据自定义的顶点结构来设置），如  D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1
*     SetIndices -- 设置索引缓存，每次只能使用一个
*     SetLight -- 注册灯光
*     SetRenderState -- 设置渲染状态(如 Cull、灯光、环境光、深度缓冲、透明纹理等)，直到下一次改变渲染状态为止
*       如使用框架进行填充的模式,SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME)
*     SetStreamSource -- 设置资源流与一个顶点缓存挂钩，此流就是一个流入渲染管线的几何信息的流。
*     SetTexture -- 设置纹理，能设置八个纹理，它们能够组合起来创建更多细节的图象(多重纹理)
*     SetTextureStageState -- 设置纹理的状态(比如使用Alpha通道)
*     SetTranform -- 设置变换矩阵
*     SetViewport -- 设置视口(窗口模式运行时，显示部分可能只占屏幕的一部分，由窗口坐标系描述)
*     SetTexture -- 设置纹理
*     SetSamplerState -- 设置纹理过滤器
*   ID3DXBaseMesh -- 包含一个用来存储网格顶点的顶点缓存和一个用来定义这些顶点怎样连接在一起组成网格三角形的索引缓存。
*     DrawSubset -- 绘制图形(网格的子集)
*     GetAttributeTable/SetAttributeTable -- 获取/设置 属性表
*     GetFVF -- 获得顶点的格式
*     GetNumVertices -- 返回顶点缓存中的顶点数
*     GetNumBytesPerVertex -- 返回一个顶点所占的字节数
*     GetNumFaces -- 返回在mesh中的面（三角形）数
*     GetVertexBuffer/GetIndexBuffer -- 获得顶点/索引缓存
*     LockVertexBuffer/LockIndexBuffer -- 锁定顶点/索引缓存，用于读写数据
*     LockAttributeBuffer/UnlockAttributeBuffer -- 锁定/解锁属性缓存
*     OptimizeInplace/Optimize -- 将Mesh的顶点和索引进行重组来优化。
*   ID3DXBuffer -- 将数据存储到邻接内存块中，可以使用 D3DXCreateBuffer 方法创建
*     GetBufferPointer -- 返回一个指向开始数据的指针
*     GetBufferSize -- 返回在缓存中的字节大小

*   ID3DXFont -- 使用GDI来绘制文字(能够联合字体句柄和格式化字体)，通过 D3DXCreateFontIndirect、D3DXCreateFont 等函数创建。
*     DrawText -- 
*   ID3DXMesh -- 网格数据
*   ID3DXPMesh -- 渐进(Progressive)网格，允许我们通过简化边缩减转换（edge collapse transformations，ECT）来简化mesh。
*     每执行一次ECT就移除一个顶点和一或2个面，该过程可逆(顶点分裂)。可以控制不同的精细级别(如远处时低精度，近处高精度)。
*     使用 D3DXGeneratePMesh 函数根据输入的原始mesh创建，
*     GetMaxFaces/GetMinFaces -- 返回渐进网格能够被设置的 最大/最小 面数
*     GetMaxVertices/GetMinVertices -- 返回渐进网格能够被设置的 最大/最小 顶点数
*     SetNumFaces/SetNumVertices -- 允许我们设置 面/顶点 的个数，以便让mesh简化或复杂化。注意调整后的面/顶点数可能并不是设定的数。
*     TrimByFaces/TrimByVertices -- 设置新的最小和最大面/顶点数。注意其值必须在[GetMinFaces，GetMaxFaces]等之中
*   IDirect3DSurface9 -- 表面(像素点阵)，主要用来存储2D图形数据
*     LockRect/UnlockRect -- 获得/释放指向表面内存的指针，对任一个像素点进行读、写操作
*     GetDesc -- 通过填充D3DSURFACE_DESC结构来返回表面的描述信息
*   IDirect3DSwapChain -- 交换链，DX9时可以更改窗口(但性能差，使用内存多)，DX10中交换链在创建时就绑定到窗口
*   IDirect3DTexture9 -- 纹理，通常由 D3DXCreateTextureFromFile(BMP,DDS,DIB,JPG,PNG,TGA) 等函数创建纹理数据
*   IDirect3DVertexBuffer9/IDirect3DIndexBuffer9 -- 顶点缓存/索引缓存
*     Lock -- 获得访问缓存所需的指针，使用完毕后必须用 Unlock 解锁
*     GetDesc -- 获得描述信息(通过CreateVertexBuffer/CreateIndexBuffer创建时传入的信息)
* 
*****************************************************************************************************/



#endif //DIRECT3D_INTERFACE_H