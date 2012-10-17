#ifndef DIRECT3D_STUDY_H
#define DIRECT3D_STUDY_H
#pragma once

/*****************************************************************************************************
* 临时学习的部分
*   矢量绘图 -- 应用在文本和图表的效果非常好，可以轻松的放大缩小，并且可以非常精确的控制图像的大小和位置。
*   位图绘图 -- 图像的质量取决于图像的分辨率(总的像素数量)和颜色的色深(每个象素的信息量)，一般使用压缩(JPEG/PNG等)的办法存储图片
*   
* 网络多个玩家的问题
*   延迟/同步 -- 需要保持多台PC的时钟同步
*   数据丢失 -- 在传输过程中，数据不可避免的会有丢失
*   可靠性 -- 计算机间的通讯有可能是间断的
* 两种网络模型
*   C/S -- 
*   P2P(peer-to-peer) -- 所有客户端都在游戏当中共同保存游戏的状态
*
* 精灵(游戏对象)--表示游戏中所有运动的部分，可以包括 玩家、敌人、背景活动的部分、子弹等
*   需要保存各种信息(即时位置、即时速度、即时方向、可见度 等)。
*   通常使用 不活动的精灵原型在运行时创造同样的精灵(clone 设计模式)
*
*****************************************************************************************************/

/*****************************************************************************************************
* 常见错误
*   D3DERR_INVALIDCALL -- 传递的参数不合要求
*****************************************************************************************************/


/*****************************************************************************************************
*
*
* 射线 -- 用一个起点和方向来描述
* 顶点 -- D3D中可以灵活的定义自己的顶点格式，使用可变顶点格式(FVF--Flexible Vertex Format)的组合标志进行描述(标志顺序必须和顶点结构顺序相同)，
*   然后通过创建顶点缓冲区并设置渲染源来显示基本的图形
*   例：struct ColorVertex
*       {
*           float _x, _y, _z;     // 位置
*           DWORD _color;         // 颜色
*       };
*       #define FVF_COLOR (D3DFVF_XYZ | D3DFVF_DIFFUSE)   //NORMAL--顶点法线， TEX1 -- 纹理坐标
*         D3DFVF_XYZ -- 定义的是3D坐标，(0,0)在屏幕的中心，单位为标准单位，范围为 [-1,1]，默认非光照
*         D3DFVF_XYZRHW -- 定义的是经过转换后的2D坐标，通常用于2D绘图。Reciprocal of Homogenous W()
*           (0,0)在屏幕的左上角,单位似乎是象素，范围为 [窗体宽,窗体高]，默认采用高洛夫的光照模式
* 
* 表面 -- Direct3D中默认顶点以顺时针方向（在观察坐标系中）形成的三角形为正面，以逆时针方向形成的三角形为背面。
* 纹理(Texturing) -- 为三角形赋予图象数据(表面贴图)，能够更细腻更真实地表现场景
* 内存池 -- 
*   D3DPOOL_DEFAULT(根据资源的类型和用途把它们放在最合适的地方, 将尽可能存放在显存中),
*     但在程序运行时会出现设备丢失的现象，需要程序员在设备丢失时释放其内存，当设备恢复时重新分配内存
*   D3DPOOL_MANAGED(资源将由Direct3D管理并且按设备的需要来指定放在系统内存还是放在AGP显存中) -- 性能不重要时的安全选择
*   D3DPOOL_SYSTEMMEM(资源位于系统内存中，在程序退出之前常驻内存，不会出现设备丢失的现象)
*
* 交换链 -- Direct3D通常创建2~3个表面组成一个集合(交换链)，使用页面切换技巧使两帧动画之间过度更平滑。
* 深度缓冲(Z-Buffering) -- 也是一个表面，但不存储图像数据，而是记录像素的深度信息，将确定哪一个像素最后被绘制出来。
*   深度值的个数=宽x高。为每一个像素计算深度值，并进行深度测试，从而只绘制最靠近照相机的像素，而不绘制被遮住的像素。
*   Direct3D使用的深度缓冲的范围是0~1（？在SetViewport中的参数中说明？）
* 顶点处理 -- 可以被软件(总被支持且永远可用)或硬件处理(显卡硬件必须支持，也称为 硬件几何转换和光源计算)。
* 渲染管线(Rendering Pipeline) -- 创建为3D世界进行几何描述的2D图形并设定一个虚拟照相机确定这个世界中哪一部分将被透视投影到屏幕上。
*   通常是通过矩阵变换实现的一系列坐标变换，使用 IDirect3DDevice9::SetTranform 提供变换矩阵。
*   Local Space(本地坐标系，简化了建模的过程，是3D模型最原始的坐标，也叫 Model Space--模型空间) 
*     -> World Space(世界坐标系，将所有的物体都通过平移、旋转、缩放后放到同一个世界坐标系中，D3DTS_WORLD)
*       -> View Space(视图坐标系，变换照相机的朝向和位置， 通过 D3DXMatrixLookAtLH 获得变换矩阵，再通过 D3DTS_VIEW)
*         -> Backface Culling(背面拣选，D3D通过删除背面多边形的处理来提高效率，使用 SetRenderState 设置)
*           -> Lighting(光源，定义在世界坐标系中然后被变换到视图坐标系中，增加了场景中物体的真实性)
*             -> Clipping(裁剪--拣选超出可视体范围的几何图形的过程，在可视体外的部分不被处理)
*               -> Projection(投影--将3D场景投影为2D图像表示，使用 D3DXMatrixPerspectiveFovLH 定义可视体的变换矩阵，然后使用 D3DTS_PROJECTION) 
*                 -> Viewport Transform(视口变换，转换投影窗口到显示屏幕上，使用 SetViewport 函数设置) 
*                   ->Rasterization(光栅化，计算需要显示的每个三角形中每个点的颜色值，将2D图像显示在显示器上，非常繁重，由硬件完成)
* 三角形网格(Triangle Mesh) -- 使用三角形网格近似表示一个物体(建模),即 XFile
*   D3DX提供了 D3DXCreate(Box/Sphere/Cylinder/Teapot/Polygon/Torus)等6个网格生成函数，也可以使用 D3DXCreateMesh[FVF] 创建一个空mesh
*   一个mesh由一个或数个子集(subset)组成，一个子集是使用相同属性(材质，纹理和渲染状态等)渲染的一组三角形。如一个房子可以由
*   地板、墙壁、天花板、窗户等多个subset组成。每一个子集有一个相对应的材质和纹理。
* 三角形 -- 三角形顶点的顺序是很重要
* 索引 -- 复杂图形中经常有重复的顶点，为了简化表示。创建一个顶点列表(包含所有不重复的顶点)和一个索引列表
*   (用顶点列中定义的值来表示每一个三角形的构造方式)
*   一个正方形由两个三角形组成 -- Vertex rect[6] = { {v0, v1, v2},{ v0, v2, v3}};
*     顶点列表：Vertex vertexList[4] = {v0, v1, v2, v3};
*     索引列表：WORD indexList[6] = { 0, 1, 2,  //三角形0
*                                     0, 2, 3}; //三角形1
* [顶点|索引]缓存 -- 一块连续的存储了[顶点|索引]数据的内存,能被放置在显存中(更快)，由IDirect3DVertexBuffer9接口定义
*   静态缓存 -- 通常被放置在显存中，能高效的处理，但从中读取和写入数据是很慢的，通常用于保存不需要被经常改变的数据(如地形和建筑)
*   动态缓存(D3DUSAGE_DYNAMIC) -- 通常被放在AGP内存中，能被很快的更新，但必须在渲染前被转移到显存中。通常用于经常更新的数据(如粒子系统)
* 照相机 -- 确定3D世界中的哪部分是可见的，因而需要将哪部分转换为2D图形，在世界坐标系的源点并朝向Z轴正方向。
* 摄像机 -- 实现一个Camera类，使我们可以更好地操作摄像机，并用来作为飞行模拟摄像机和第一人称视角摄像机 -- 具体参见代码
* 可视体 -- 由可视角度和前裁剪面（Near Plane）与后裁剪面（Far Plane）定义的一个棱台
* 渲染状态 -- 提供了多种渲染状态，它影响几何物体怎样被渲染
*
* XFile -- 使用3D建模工具(如 3DMax、LightWave3D、Maya 等)建造复杂的真实的mesh，D3DX提供了读和写XFile的函数
*   (D3DXLoadMeshFromX 和  )。DirectX SDK Extra 中有针对 3DMax、Maya 等软件导出 .X File 的Direct3D 工具包。
*   可以使用 MView.exe 查看
*  
* 界线容积(Bounding Volumes) -- 即mesh的边界范围，常用的有两种类型：立方体和球。长用来加速可见性测试、碰撞检测等。
*   D3DX提供了 D3DXComputeBoundingSphere/D3DXComputeBoundingBox 函数计算边界盒/边界球。
*
******************************************************************************************************/

/******************************************************************************************************
* 变换矩阵             缩放矩阵              旋转矩阵
*   1   0  0             sx 0  0               cos   x   sin
*   0   1  0             0  sy 0               -sin  x   cos
*   dx dy  0             0  0  1               0     0    1
* x2 = x1 + dx         x2=sx * x             
* y2 = y1 + dy         y2=xy * y
******************************************************************************************************/

/******************************************************************************************************
* C#工程中
*   项目--〉添加引用-〉把Microsoft.DirectX和Microsoft.DirectX.Driect3D两个命名空间添加到工程中
*   using Microsoft.DirectX;
*   using Microsoft.DirectX.Driect3D;
* 
* 调试 -- 输出向量或矩阵的信息？
*   定义 D3D_DEBUG_INFO 宏后可以查看调试信息（比如 IDirect3D9 中的 Version 字段）
* 计算FPS -- 每过约一秒的时间，就用帧数/秒数作为当前的FPS，然后将帧数和秒数清空，为下次计算做准备。
* 
* 数学基础
*   左手坐标系(Z向里)、右手坐标系(Z向外) -- 通常D3D中使用左手，而OpenGL使用右手？ 
*   向量(D3DXVECTOR3) -- 3维空间中的有向线段，标准基向量是沿坐标轴且长度为1的向量(单位向量)。实际就是(x,y,z),表示(0,0,0)到该点的线段
*     也有2D向量(D3DXVECTOR2)和4D向量(D3DXVECTOR4，第4个成员缺省是1[表示坐标]或0[表示向量]？反了？)。
*     标准化向量(D3DXVec3Normalize)：长度等于1的向量。
*     点积(D3DXVec3Dot) -- 两个向量的乘积，各维分别相乘，然后相加，结果是一个标量(数)。表示两个向量的点积是
*       它们的模和夹角的余弦之积(如果模都为1，则是夹角余弦，可求夹角)。如果两个向量点积为0，则两个向量垂直。
*     叉积(D3DXVec3Cross) -- P = U x V = [(UyVz - UzVy), (UzVx - UxVz), (UxVy - UyVx)]。
*       结果向量P同时垂直于U和V。
*     注意：向量相等(==)是浮点数比较，是近似相等（D3DXVECTOR3内部实现）
*   纹理坐标 -- 由水平方向的u轴和竖直方向v轴(向下为正)构成。  
*   矩阵(D3DXMATRIX -- 4x4的矩阵)
*     乘法 -- 能变换向量并且将不同向量转换到一起，A的列数必须等于B的行数，则A和B才能相乘。
*     求逆(D3DXMatrixInverse) -- 只有方阵才能求逆，矩阵和他的逆矩阵相乘得到一个单位矩阵。
*     转置(D3DXMatrixTranspose) -- 相互交换矩阵的行和列
*     单位矩阵(D3DXMatrixIdentity) -- 除了对角（左上到右下）以外所有成员都是0，对角都是1的方矩阵。
*       用单位矩阵乘以矩阵不会改变矩阵，
*   
*   矩阵的基本变换(使用矩阵-矩阵相乘的方式)
*     平移(D3DXMatrixTranslation) -- 注：通过平移不会改变向量
*     旋转(D3DXMatrixRotationX、D3DXMatrixRotationY、D3DXMatrixRotationZ) -- 注：单位是弧度
*     缩放(D3DXMatrixScaling) -- 
*     变换点向量(D3DXVec3TransformCoord) -- 变换点同时设置向量第4个成员为1(坐标) ？ D3DXVec3TransformCoordArray
*     变换方向向量(D3DXVec3TransformNormal) -- 变换向量并且设置第4个成员为0(向量) ？ D3DXVec3TransformNormalArray
*     投影(从齐次坐标空间映射到3D空间?) -- 
*     视图空间变换 -- 在世界坐标系中进行几何变换以便将照相机平移变换到坐标系的源点并把它的方向旋转至朝向Z轴的正方向
*   综合变换 -- 把多个变换(平移、旋转、缩放等)通过单位矩阵合并为一个矩阵，并使用矩阵乘法进行计算，
*     可以大大提高采用相同方式处理多个向量时的效率。
*
*   平面(D3DXPLANE) -- 通过一个3D向量n(法向量)和平面上的一个点p0来描述，使用4D向量 (n,d)来实现。
*     点p和平面(n,d)的关系(D3DXPlaneDotCoord)：
*       1.n·p + d == 0 ; 点p与平面共面
*       2.n·p + d >  0 ; 点p在平面的前面且在平面的正半空间里
*       3.n·p + d <  0 ; 点p在平面的背面且在平面的负半空间里
*     创建平面(D3DXPlaneFromPointNormal) -- 用指定法线和点创建平面
*             (D3DXPlaneFromPoints) -- 通过在平面上的3个点创立一个平面
*     标准化平面(D3DXPlaneNormalize) -- 法向量是标准(长度为1)的平面
*     变换平面(D3DXPlaneTransform) -- 和一个转换矩阵运算（平移、旋转、缩放）？注意：需要先标准化平面
*   
*   颜色 -- 使用RGB三部分来描述：D3DCOLOR(UBTYE型的ARGB)或D3DCOLORVALUE/D3DXCOLOR(0-1float型的ARGB)
*     图元的颜色是由构成它的顶点的颜色决定的
*     目前有2种着色处理模式：平面着色（flat，颜色均匀）和高洛德[平滑]着色（Gouraud，线性插值计算中间的颜色），
*       由SetRenderState(D3DRS_SHADEMODE...)设置
*
*   灯光 -- 使用灯光时，不再指定顶点的颜色，各个顶点都通过灯光引擎，根据灯光的颜色和类型、
*     物体材质(Material)及表面方向(surface normal)等计算顶点颜色。
*     有三种灯光：
*       环境光（Ambient）
*       漫反射（Diffuse Reflection），仅需要考虑灯光方向和表面的姿态
*       镜面反射（Specular Reflection），被用在物体上产生高光的地方
*
*   光源（D3D支持三种类型的光源），由 D3DLIGHT9 结构表示
*     点光源(point) -- 在世界坐标中有一个位置且向所有方向上都照射光线
*     方向光源(direction) -- 没有位置但是向指定方向发出平行光线
*     聚光灯(spotlight) -- 和手电筒的光类似，有位置并且发出的光在指定方向上按照圆锥形照射
*
*   纹理(Texture) -- 通常从图象文件中加载，有 width,height,miplevel,pixel format 等属性，使用纹理坐标(D3DFVF_TEX1)和几何体结合，
*     其坐标范围为：0.0 到 1.0
*     D3DXCreateTextureFromFile -- 从BMP等图片文件中加载纹理数据
*     设置纹理
*     m_pD3dDevice->SetTexture( 0, m_pD3dTexture );
*     m_pD3dDevice->SetTextureStageState( 0, D3DTSS_XXXX, YYYY);
* 
*     
*   物质材质(D3DMATERIAL9) -- 物体颜色将由物体反射回来的灯光颜色来决定
*     Diffuse -- 表面反射的漫射光颜色
*     Ambient -- 表面反射的环境光颜色
*     Specular -- 表面反射的镜面光颜色
*     Emissive -- 用来给表面添加颜色，它使得物体看起来就象是它自己发出的光一样
*     Power -- 指定锐利的镜面高光；它的值是高光的锐利值
*       定义红色物体的材质（只反射红光吸收其他颜色的所有光）
*         D3DMATERIAL9 red;
*         ::ZeroMemory(&red, sizeof(red));
*         red.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); // red
*         red.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); // red
*         red.Specular = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); // red
*         red.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f); // no emission
*         red.Power = 5.0f;
*     顶点法线(Vertex normals)/面法线(face normal) -- 描述方向的向量,D3D需要知道法线的方向以确定灯光照射到物体表面的角度。
*       注意：
*       1.顶点法线不一定和面法线相同(如球体--有很多小面组成)；
*       2.多个面上相同点的顶点法线不一定相同（如三棱锥的顶点在不同面上有不同的顶点法线）
* 
*   过滤 -- 纹理被映射到屏幕时，由于屏幕三角形和纹理三角形不一样大时，会产生变形。过滤（Filtering）是用来帮助这些变形变的平滑的技术。
*     D3D提供了三种过滤器(品质越好则越慢)，
*       Nearest point sampling(D3DTEXF_POINT) -- 缺省过滤器，效果最差，速度最快
*       Linear filtering( D3DTEXF_LINEAR)) -- 效果较好，推荐使用。
*       Anisotropic filtering( D3DTEXF_ANISOTROPIC) -- 效果最好，但处理时间也最长，必须设置 D3DSAMP_MAXANISOTROPY 等级。
*   混合 -- 通常用已经光栅化的像素光栅化同一位置的像素，这种技术允许我们完成多种特效。alpha混合能够被用来控制图元的透明度。
*       OutputPixel=SourcePixel x SourceBlendFactor + DestPixel x DestBlendFactor
*       默认关闭，通过设置 D3DRS_ALPHABLENDENABLE 开启
*     当使用混合时需要遵循以下规则：首先不使用混合绘制物体，然后根据物体离摄象机的距离使用混合对物体拣选，
*     利用z分量使用从后到前的顺序混合绘制物体。
*   
*   透明度 -- 由颜色中的Alpha部分指定，0表示完全透明，255时表示完全不透明，必须将源混合要素设置为 D3DBLEND_SRCALPHA，
*     目的混合要素为 D3DBLEND_INVSRCALPHA。
*   Alpha通道 -- 纹理数据中额外的设置位，保存每一个点的alpha值，可以使用DirectX纹理工具创建(DxTex.exe, 创建的纹理文件扩展名为 dds )
*
*   模版(Stencile)缓存 -- 一个远离屏幕的缓存，可用来完成一些特效(如实现镜子的反射效果？通过 D3DXMatrixReflect 创建反射矩阵)
*     在创建深度缓存的同时可以创建模板缓存，他们分享同一个离屏表面缓存，但是每个像素被指定到各自缓存内存片段中。
*     能够使用模版缓存，通过模版测试来阻止渲染后缓存中的某些部分
*     如 D3DFMT_D24S8 表示创建一个32位深度/模版缓存，其中24位为深度缓存，8位为模版缓存
*
*   阴影 -- 建立阴影矩阵的函数 D3DXMatrixShadow
*   字体 -- 可以使用 ID3DXFont(使用GDI来绘制文字，可以格式化)，CD3DFont(使用纹理三角形和Direct3D绘制，更快)或 D3DXCreateText 来操作
* Direct3D 概述
*   Application 
*     -> Direct3D 
*       -> HEL(硬件模拟层, 当硬件不支持说要求的性能时，使用软件模拟，D3DDEVTYPE_REF、D3DDEVTYPE_SW ？)
*       -> HAL(Hardware Abstraction Layer，硬件抽象层，由设备制造商实现,D3DDEVTYPE_HAL) 
*         -> Graphics Device。
*   Direct3D 使用 REF设备(D3DDEVTYPE_REF)模拟了所有的Direct3D API，允许编写并测试那些显卡不支持的Direct3D特性的代码，
*   真实代码中需要检测设备是否支持所需功能。
*   
*   设备能力 -- Direct3D支持的每一项特性都对应于D3DCAPS9结构的一个数据成员
* 
* 术语
*   Multisampling(D3DMULTISAMPLE_TYPE) -- 全屏抗锯齿(使表示图像的像素矩阵看起来平滑)
* 
*
* 初始化Direct3D
*   1.获得一个 IDirect3D9 接口指针  -- Direct3DCreate9(D3D_SDK_VERSION);
*   2.检查设备能力（如是否支持硬件顶点处理） -- IDirect3D9::GetDeviceCaps
*   3.初始化 D3DPRESENT_PARAMETERS 结构实例 -- 
*   4.创建 IDirect3DDevice9 对象 -- IDirect3D9::CreateDevice
******************************************************************************************************/


//镜子反射 -- 首先渲染镜子图元到模版缓存，然后渲染那些能被渲染到镜子里的反射茶壶
#if 0
void RenderMirror()
{
    //允许模版缓存和设置渲染状态
    Device->SetRenderState(D3DRS_STENCILENABLE, true);
    Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);  //让所有模版测试都通过
    Device->SetRenderState(D3DRS_STENCILREF, 0x1);
    Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
    Device->SetRenderState(D3DRS_STENCILWRITEMASK,0xffffffff);

    //假如深度测试失败了，指定D3DSTENCILOP_KEEP，表明不更新模版缓存入口 -- 不想渲染被“模糊”的反射像素
    Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

    //假如模版测试失败了
    Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

    //假如深度测试和模版测试都通过了，更新模版缓存入口，设置模版参考值为0x1
    Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

    //阻止渲染镜子代码
    // disable writes to the depth and back buffers
    Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
    Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
    Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    // draw the mirror to the stencil buffer
    Device->SetStreamSource(0, VB, 0, sizeof(Vertex));
    Device->SetFVF(Vertex::FVF);
    Device->SetMaterial(&MirrorMtrl);
    Device->SetTexture(0, MirrorTex);
    D3DXMATRIX I;
    D3DXMatrixIdentity(&I);
    Device->SetTransform(D3DTS_WORLD, &I);
    Device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);
    // re-enable depth writes
    Device->SetRenderState(D3DRS_ZWRITEENABLE, true);

    Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
    Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

    //计算在场景中反射位置的矩阵
    // position reflection
    D3DXMATRIX W, T, R;
    D3DXPLANE plane(0.0f, 0.0f, 1.0f, 0.0f); // xy plane
    D3DXMatrixReflect(&R, &plane);
    D3DXMatrixTranslation(&T,
        TeapotPosition.x,
        TeapotPosition.y,
        TeapotPosition.z);
    W = T * R;

    //清除深度缓存 -- 因为被反射的茶壶的深度比镜子的深度大，所以需要防止镜子的图元把被反射茶壶的图元弄模糊
    Device->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

    //混合被反射的茶壶和镜子
    Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
    Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

    //准备绘制被反射的茶壶
    Device->SetTransform(D3DTS_WORLD, &W);
    Device->SetMaterial(&TeapotMtrl);
    Device->SetTexture(0, 0);
    Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
    Teapot->DrawSubset(0);

    //改变背面拣选模式
    Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    Device->SetRenderState( D3DRS_STENCILENABLE, false);
    Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
} // end RenderMirror()


//摄像机类 -- 使用四个摄像机向量：Right、Up、Look、Position，用来为摄像机定义一个坐标系来描述在世界坐标中的对应关系。
//摄像机就能够按照下面六种方式变化(能够沿着三个轴移动以及饶着三个轴旋转)：
//  围绕right向量旋转（pitch倾斜）
//  围绕up向量旋转（yaw 偏航）
//  围绕look向量旋转（roll 滚转）
//  沿着right向量平移（strafe）
//  沿着up向量飞行（fly）
//  沿着look向量移动（move）

class Camera
{
public:
    enum CameraType { LANDOBJECT, AIRCRAFT };
    Camera();
    Camera(CameraType cameraType);
    ~Camera();

    void strafe(float units); // left/right
    void fly(float units);    // up/down
    void walk(float units);   // forward/backward	
    void pitch(float angle); // rotate on right vector
    void yaw(float angle);   // rotate on up vector
    void roll(float angle);  // rotate on look vector

    void getViewMatrix(D3DXMATRIX* V); 
    void setCameraType(CameraType cameraType); 
    void getPosition(D3DXVECTOR3* pos); 
    void setPosition(D3DXVECTOR3* pos); 
    void getRight(D3DXVECTOR3* right);
    void getUp(D3DXVECTOR3* up);
    void getLook(D3DXVECTOR3* look);
private:
    //两种摄像机模式，LANDOBJECT模式(限制在某些轴上的运动)和AIRCRAFT模式(空间中完全自由的移动)
    CameraType  _cameraType;
    D3DXVECTOR3 _right;
    D3DXVECTOR3 _up;
    D3DXVECTOR3 _look;
    D3DXVECTOR3 _pos;
};

#endif  //#if 0

#endif //DIRECT3D_STUDY_H