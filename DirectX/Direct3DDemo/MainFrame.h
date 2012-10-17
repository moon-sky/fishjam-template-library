#pragma once

#if 0
//如果定义了这个宏，表明通过2D绘制，否则通过3D绘制
#define DRAW_FISHJAM_2D
#endif

#if 0
//如果定义了这个宏，可以通过将 InitDirect3D 的 hWnd 设置为一个D3D窗口(非全屏)句柄的方式，来对其进行截屏 
#define DX_SNAP
#endif

//自定义的 FVF 顶点结构 -- 感觉定义时最好还是用 D3DXXXX 进行定义？
struct FISHJAM_VERTEX
{
#ifdef DRAW_FISHJAM_2D
    FLOAT x, y, z, rhw; // The transformed position for the vertex
#else
    FLOAT x, y, z;      // The untransformed, 3D position for the vertex, 
                        //等价于 D3DXVECTOR3 position

    D3DXVECTOR3 normal; // The surface normal for the vertex，定义法线方向
#endif
    D3DCOLOR color;     // The vertex color
    //FLOAT       tu, tv;   // The texture coordinates
};

#ifdef DRAW_FISHJAM_2D
//RHW -- 表明已经被转换(transformed)过的坐标系
#define D3DFVF_FISHJAM_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
#else
#define D3DFVF_FISHJAM_VERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
                              //坐标     |法线         |漫反射(颜色)  |纹理
#endif

class CMainFrame : public CFrameWnd
{
    DECLARE_DYNCREATE(CMainFrame)
public:
    CMainFrame(void);
    ~CMainFrame(void);
    DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnPaint();

    /************************************************************************
    注意：SDK示例中采用如下的方式进行图象输出 -- 有消息时处理，无消息时输出，
          此处采用Timer的方式进行模拟输出
          哪一种更好？或者标准方式应该是什么？
    if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
    else
    {
        Render();
    }
    *************************************************************************/
    afx_msg void OnTimer(UINT_PTR nIDEvent);


    /*******************************************************************************************************
    *
    *******************************************************************************************************/
    HRESULT InitDirect3D(HWND hWnd);

    //初始化 vertex buffer，然后可以通过 Lock/Unlock 进行使用
    HRESULT InitVertexBuffer();

    //设置世界、视图、投影的转换矩阵
    HRESULT SetupMatrices();

    //设置场景中的灯光和材质
    HRESULT SetupLights();

    HRESULT SetTexture();

    //从 XFile 中加载 Mesh
    HRESULT SetupXFileMesh(LPCTSTR pszXFilePath);

    HRESULT Render();
private:
    IDirect3D9*                 m_pD3d;
    IDirect3DDevice9*           m_pD3dDevice;
	IDirect3DSurface9 *			m_pOffscreenPlainSurface;
    IDirect3DVertexBuffer9*     m_pD3dVertexBuffer;
    IDirect3DTexture9*          m_pD3dTexture;

    //使用预定义的Mesh(XFilee) -- 其中可能有多个 Material，后期需要将 SetupXFileMesh 做到FTL中！
    ID3DXMesh*                  m_pD3dMesh;
    DWORD                       m_dwNumMaterials;
    D3DMATERIAL9*               m_pMeshMaterials; // Materials for our mesh -- 使用时会是一个数组
    IDirect3DTexture9**         m_ppMeshTextures; // Textures for our mesh -- 数组方式，Mesh中可能有多个Material，每个对应一个Texture
public:
};
