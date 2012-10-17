#include "Stdafx.h"
#include "MainFrame.h"
#include <ftlDirectx.h>
#include <WinUser.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_WM_TIMER()
END_MESSAGE_MAP()


CMainFrame::CMainFrame(void)
{
    m_pD3dDevice = NULL;
    m_pD3d = NULL;
    m_pD3dVertexBuffer = NULL;
	m_pOffscreenPlainSurface = NULL;
    m_pD3dTexture = NULL;
    m_pD3dMesh = NULL;
    m_pMeshMaterials = NULL;
    m_ppMeshTextures = NULL;
} 

CMainFrame::~CMainFrame(void)
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    HRESULT hr = E_FAIL;
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;
    
    DX_VERIFY(InitDirect3D(m_hWnd));  //(HWND)0x00141B48
    DX_VERIFY(InitVertexBuffer());

    Direct3DUtility::DumpAdapterModes(m_pD3d);

    SetTimer(1,1,NULL);
    return 0;
}



void CMainFrame::OnDestroy()
{
    BOOL bRet = FALSE;
    API_VERIFY(KillTimer(1));


    SAFE_DELETE_ARRAY(m_pMeshMaterials);
    if (m_ppMeshTextures)
    {
        for (DWORD i = 0; i < m_dwNumMaterials; i++ )
        {
            SAFE_RELEASE(m_ppMeshTextures[i]);
        }
        SAFE_DELETE_ARRAY(m_ppMeshTextures);
    }

    SAFE_RELEASE(m_pD3dMesh);
    SAFE_RELEASE(m_pD3dTexture);
    SAFE_RELEASE(m_pD3dVertexBuffer);
	SAFE_RELEASE(m_pOffscreenPlainSurface);
    SAFE_RELEASE(m_pD3dDevice);
    SAFE_RELEASE(m_pD3d);

    CFrameWnd::OnDestroy();
}


HRESULT CMainFrame::InitDirect3D(HWND hWnd = NULL)
{

    HRESULT hr = E_FAIL;
    //1.获得一个 IDirect3D9 接口指针
    m_pD3d = ::Direct3DCreate9(D3D_SDK_VERSION);
    if (m_pD3d)
    {
        //2.检查设备能力（如是否支持硬件顶点处理）
		D3DCAPS9 caps;
        ZeroMemory(&caps, sizeof(caps));
        D3DDEVTYPE deviceType = D3DDEVTYPE_HAL;
        DX_VERIFY(m_pD3d->GetDeviceCaps(D3DADAPTER_DEFAULT, // 主显示设备
            deviceType, // 设备类型，一般是D3DDEVTYPE_HAL.
            &caps)); // 返回填充后的D3DCAPS9 结构，包含主显示设备的能力
        
        // 是否可以使用硬件顶点处理?
        DWORD dwBehaviorFlags = 0;
        if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
        {
            // 是，支持硬件顶点处理
            dwBehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
        }
        else
        {
            // 不，只能用软件顶点处理
            dwBehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        }
        
		//获取适配器的显示模式(高宽、刷新率、格式等)
		D3DDISPLAYMODE ddm = {0};
		DX_VERIFY(m_pD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &ddm));

        //3.初始化 D3DPRESENT_PARAMETERS(提交参数) 结构实例,用于创建 D3DDevice，其大部分参数是0
        D3DPRESENT_PARAMETERS d3dpp;
        ZeroMemory(&d3dpp, sizeof(d3dpp));

		d3dpp.Windowed = TRUE;                          //设为true则为窗口模式，false则为全屏模式
        d3dpp.BackBufferWidth = ddm.Width;              //后备缓冲表面的宽度（以像素为单位），显示器分辨率的宽度
        d3dpp.BackBufferHeight = ddm.Height;            //后备缓冲表面的高度（以像素为单位），显示器分辨率的高度
        d3dpp.BackBufferFormat = ddm.Format;			// D3DFMT_X8R8G8B8;   //后备缓冲表面的像素格式
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;       //指定表面在交换链中是如何被交换的(通常DISCARD是最有效的)
        //d3dpp.BackBufferCount = 1;                    //后备缓冲表面的数量，通常设为“1”，即只有一个后备表面
        d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;	//全屏抗锯齿的类型
        //d3dpp.MultiSampleQuality = 0;                   //全屏抗锯齿的质量等级(多重采样数量)
        d3dpp.hDeviceWindow = hWnd;                     //与设备相关的窗口句柄

        //控制 ZBuffer 的
        d3dpp.EnableAutoDepthStencil = TRUE;            //设为TRUE，D3D将自动创建深度/模版缓冲
        d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;      //D3DFMT_D16;    //深度/模版缓冲的格式

        d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER; //附加特性( D3DPRESENTFLAG )
        d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; //显示适配器刷新屏幕的频率
        d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; //提交间隔：D3DPRESENT_INTERVAL_IMMEDIATE(立即交换)

        //4.创建 IDirect3DDevice9 对象
        DX_VERIFY(m_pD3d->CreateDevice(
            D3DADAPTER_DEFAULT,                     //指定对象要表示的物理显示设备(默认) -- 除非有多个显卡
            D3DDEVTYPE_HAL,                         //设备类型(硬件抽象层设备或参考设备)
            hWnd,                                   //关联到设备上的窗口句柄
            dwBehaviorFlags,                        //顶点处理类型（硬件/软件）
            &d3dpp,                                 // present parameters
            &m_pD3dDevice));                        // returned created device
		
		if(FAILED(hr))
		{
			return hr;
		}

#ifndef DRAW_FISHJAM_2D
        // Turn off culling, so we see the front and back of the triangle
        DX_VERIFY(m_pD3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE ));

        // Turn off D3D lighting, since we are providing our own vertex colors
        //m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

        // Turn on ambient lighting -- 打开漫反射灯光, 此处设置为红色
        //m_pD3dDevice->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB(255,0,0) );

        // Turn on the zbuffer
        DX_VERIFY(m_pD3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE ));
#endif

#ifdef DX_SNAP
		Sleep(5000);
		
		for (int i = 0; i < 20; i++)
		{
			CString strSnapName;
			strSnapName.Format(TEXT("SnapDirectX_%03d.PNG"), i);

			DX_VERIFY(m_pD3dDevice->CreateOffscreenPlainSurface(ddm.Width, ddm.Height, 
				D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, 
				&m_pOffscreenPlainSurface, 
				NULL));

			if (m_pOffscreenPlainSurface)
			{
				COM_VERIFY(m_pD3dDevice->GetFrontBufferData(0, m_pOffscreenPlainSurface));
				if (hWnd)
				{
					WINDOWINFO windowInfo = {0};
					windowInfo.cbSize = sizeof(WINDOWINFO) ;

					::GetWindowInfo(hWnd, &windowInfo) ;
					RECT* pRect = &windowInfo.rcWindow;
					// Save the screen date to file
					DX_VERIFY(D3DXSaveSurfaceToFile(strSnapName, 
						D3DXIFF_PNG, 
						m_pOffscreenPlainSurface, 
						NULL, 
						pRect));
					FTLTRACE(TEXT("D3DXSaveSurfaceToFile, FileName=%s, Rect=(%d,%d) - (%d,%d)\n"),
						strSnapName, pRect->left, pRect->top, pRect->right, pRect->bottom);
				}
				else
				{
					DX_VERIFY(D3DXSaveSurfaceToFile(strSnapName, 
						D3DXIFF_PNG, 
						m_pOffscreenPlainSurface, 
						NULL, 
						NULL));
				}
				m_pOffscreenPlainSurface->Release();
				m_pOffscreenPlainSurface = NULL;

				Sleep(1000);
			}
		}
#endif

	}
    return hr;
}

HRESULT CMainFrame::InitVertexBuffer()
{
    HRESULT hr = E_FAIL;

#if 0
    //初始化三角形的三个向量
    FISHJAM_VERTEX vertices[] =
    {
#ifdef DRAW_FISHJAM_2D
        /****************************************************************************
        x, y, z, rhw, color －－什么坐标系(左手、右手)？
        ------------------->X
        |
        |       红(150,50)
        |
        |
        |  蓝(50,250) 绿(250,250)
        V
        Y
        /****************************************************************************/
        { 150.0f,  50.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255,0,0) }, //红(最上面)
        { 250.0f, 250.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0,255,0) }, //绿(最右边)
        {  50.0f, 250.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0,0,255) }, //蓝(最左边)
#else

        /****************************************************************************
        x, y, z, color －－什么坐标系(左手、右手)？
        ------------------->X
        |红(-1,-1)        
        |       
        |      (0,0)
        |
        |  
        V
        Y
        /****************************************************************************/
        { -1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(255,0,0) }, //红
        {  1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(0,255,0) }, //绿
        {  0.0f,  1.0f, 0.0f, D3DCOLOR_XRGB(0,0,255) }, //蓝
#endif
};
    DX_VERIFY(m_pD3dDevice->CreateVertexBuffer( _countof(vertices) * sizeof(FISHJAM_VERTEX),
        0, D3DFVF_FISHJAM_VERTEX,
        D3DPOOL_DEFAULT, 
        &m_pD3dVertexBuffer, 
        NULL ));

    //填充 vertex buffer：先通过 Lock 获取到向量的访问地址(可能在 device memory"显卡"中？)，然后再拷贝进数据
    FISHJAM_VERTEX* pVertices = NULL;
    DX_VERIFY( m_pD3dVertexBuffer->Lock( 0, sizeof(vertices), (void**)&pVertices, 0 ) );
    CopyMemory( (void*)pVertices, vertices, sizeof(vertices) );
    DX_VERIFY(m_pD3dVertexBuffer->Unlock());

#else
    //圆柱形的向量 
    DX_VERIFY( m_pD3dDevice->CreateVertexBuffer( 50*2*sizeof(FISHJAM_VERTEX),
        0, D3DFVF_FISHJAM_VERTEX,
        D3DPOOL_DEFAULT, 
        &m_pD3dVertexBuffer, 
        NULL ) );

    // Fill the vertex buffer. We are algorithmically generating a cylinder
    // here, including the normals, which are used for lighting.
    FISHJAM_VERTEX* pVertices = NULL;
    DX_VERIFY( m_pD3dVertexBuffer->Lock( 0, 0, (void**)&pVertices, 0 ) );
    for( DWORD i=0; i < 50; i++ )
    {
        FLOAT theta = (2*D3DX_PI*i)/(50-1);
        pVertices[2*i+0].x = sinf(theta);
        pVertices[2*i+0].y = -1.0f;
        pVertices[2*i+0].z = cosf(theta);
        pVertices[2*i+0].normal   = D3DXVECTOR3( sinf(theta), 0.0f, cosf(theta) );
        pVertices[2*i+0].color =  D3DCOLOR_XRGB(255,0,0);
        //pVertices[2*i+0].tu       = ((FLOAT)i)/(50-1);
        //pVertices[2*i+0].tv       = 1.0f;

        pVertices[2*i+1].x = sinf(theta);
        pVertices[2*i+1].y = 1.0f;
        pVertices[2*i+1].z = cosf(theta);
        pVertices[2*i+1].normal   = D3DXVECTOR3( sinf(theta), 0.0f, cosf(theta) );
        pVertices[2*i+1].color =  D3DCOLOR_XRGB(0,255,0);
        //pVertices[2*i+1].tu       = ((FLOAT)i)/(50-1);
        //pVertices[2*i+1].tv       = 0.0f;

    }
    DX_VERIFY(m_pD3dVertexBuffer->Unlock());
#endif
    return hr;
}

HRESULT CMainFrame::SetupXFileMesh(LPCTSTR pszXFilePath)
{
    /*****************************************************************************************
    * 绘制时应该使用如下方式进行绘制 -- 分别绘制每个子 Material 的 Material 和 Texture
    for( DWORD i = 0 ,i < m_dwNumMaterials,i++ )
    {
        m_pD3dDevice->SetMaterial( &m_pMeshMaterials[i] );
        m_pD3dDevice->SetTexture( 0, m_ppMeshTextures[i] );
        // Draw the mesh subset
        m_pD3dMesh->DrawSubset( i );  //在 D3DXLoadMeshFromX 时，m_pD3dMesh 已经和 m_pD3dDevice 建立起关联了
    }
    *****************************************************************************************/

    HRESULT hr= E_FAIL;
    CComPtr<ID3DXBuffer> pD3DXMtrlBuffer;

    //从指定的XFile(Tiger.x)中加载Mesh
    DX_VERIFY(D3DXLoadMeshFromX( pszXFilePath, D3DXMESH_SYSTEMMEM, 
        m_pD3dDevice, NULL, 
        &pD3DXMtrlBuffer, NULL, &m_dwNumMaterials, 
        &m_pD3dMesh ));

    // We need to extract the material properties and texture names from the 
    // pD3DXMtrlBuffer

    //此处使用了强制类型转换，始终都是 D3DXMATERIAL 类型吗？能够获取到其中的 MeshMaterialList 信息，但其他信息无法获取？
    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

    m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
    if( m_pMeshMaterials == NULL )
        return E_OUTOFMEMORY;
    m_ppMeshTextures  = new IDirect3DTexture9*[m_dwNumMaterials];
    if( m_ppMeshTextures == NULL )
        return E_OUTOFMEMORY;

    for( DWORD i=0; i<m_dwNumMaterials; i++ )
    {
        // Copy the material
        m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;

        //如果有子Texture的文件，对应加载？
        m_ppMeshTextures[i] = NULL;
        if( d3dxMaterials[i].pTextureFilename != NULL && 
            strlen(d3dxMaterials[i].pTextureFilename) > 0 )
        {
            // Create the texture
            DX_VERIFY(D3DXCreateTextureFromFile( m_pD3dDevice, 
                CA2T(d3dxMaterials[i].pTextureFilename), 
                &m_ppMeshTextures[i] ));
        }
    }

    //SAFE_RELEASE(pD3DXMtrlBuffer);
    return hr;
}

HRESULT CMainFrame::SetupMatrices()
{
    HRESULT hr = E_FAIL;

    //本例中，只围绕Y-axis旋转
    D3DXMATRIXA16 matWorld;
    D3DXMatrixIdentity( &matWorld );

    // Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
    // every 1000 ms. To avoid the loss of precision inherent in very high 
    // floating point numbers, the system time is modulated by the rotation 
    // period before conversion to a radian angle.
#if 0
    //三角形的旋转方式
    UINT  iTime  = timeGetTime() % 1000;
    FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
    D3DXMatrixRotationX( &matWorld, fAngle );
#endif
    D3DXMatrixRotationX( &matWorld, timeGetTime()/500.0f );
    DX_VERIFY(m_pD3dDevice->SetTransform( D3DTS_WORLD, &matWorld ));

    // Set up our view matrix. A view matrix can be defined given an eye point,
    // a point to look at, and a direction for which way is up. Here, we set the
    // eye five units back along the z-axis and up three units, look at the
    // origin, and define "up" to be in the y-direction.
    D3DXVECTOR3 vEyePt( 0.0f, 3.0f,-5.0f );
    D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
    DX_VERIFY(m_pD3dDevice->SetTransform( D3DTS_VIEW, &matView ));

    // For the projection matrix, we set up a perspective transform (which
    // transforms geometry from 3D view space to 2D viewport space, with
    // a perspective divide making objects smaller in the distance). To build
    // a perpsective transform, we need the field of view (1/4 pi is common),
    // the aspect ratio, and the near and far clipping planes (which define at
    // what distances geometry should be no longer be rendered).
    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
    DX_VERIFY(m_pD3dDevice->SetTransform( D3DTS_PROJECTION, &matProj ));

    return hr;
}


HRESULT CMainFrame::SetupLights()
{
    HRESULT hr = E_FAIL;
    // Set up a material. The material here just has the diffuse and ambient
    // colors set to yellow. Note that only one material can be used at a time.
    D3DMATERIAL9 mtrl;
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
    mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
    mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
    mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
    DX_VERIFY(m_pD3dDevice->SetMaterial( &mtrl ));

    // Set up a white, directional light, with an oscillating direction.
    // Note that many lights may be active at a time (but each one slows down
    // the rendering of our scene). However, here we are just using one. Also,
    // we need to set the D3DRS_LIGHTING renderstate to enable lighting
    D3DXVECTOR3 vecDir;
    D3DLIGHT9 light;
    ZeroMemory( &light, sizeof(D3DLIGHT9) );
    light.Type       = D3DLIGHT_DIRECTIONAL;
    light.Diffuse.r  = 1.0f;
    light.Diffuse.g  = 1.0f;
    light.Diffuse.b  = 1.0f;
    vecDir = D3DXVECTOR3(cosf(timeGetTime()/350.0f),
        1.0f,
        sinf(timeGetTime()/350.0f) );
    D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );
    light.Range       = 1000.0f;
    DX_VERIFY(m_pD3dDevice->SetLight( 0, &light ));
    DX_VERIFY(m_pD3dDevice->LightEnable( 0, TRUE ));
    DX_VERIFY(m_pD3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE ));

    // Finally, turn on some ambient light.
    DX_VERIFY(m_pD3dDevice->SetRenderState( D3DRS_AMBIENT, 0x00202020 ));

    return hr;
}

HRESULT CMainFrame::SetTexture()
{
    HRESULT hr = E_FAIL;

    //之前需要用 D3DXCreateTextureFromFile 从图片文件中加载 Texture，并且要设置 VertexBuffer 中 D3DFVF_TEX1 相关的数据

    DX_VERIFY(m_pD3dDevice->SetTexture( 0, m_pD3dTexture ));
    DX_VERIFY(m_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE ));
    DX_VERIFY(m_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE ));
    DX_VERIFY(m_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE ));
    DX_VERIFY(m_pD3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE ));
    return hr;
}

HRESULT CMainFrame::Render()
{
    HRESULT hr = E_FAIL;
    CHECK_POINTER_RETURN_VALUE_IF_FAIL(m_pD3dDevice, E_POINTER);

    // Clear the backbuffer to a blue color
    DX_VERIFY(m_pD3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
        D3DCOLOR_XRGB(0,0,255), 1.0f, 0 ));

    // Begin the scene
    DX_VERIFY(m_pD3dDevice->BeginScene());
    if( SUCCEEDED( hr ))
    {
#ifndef DRAW_FISHJAM_2D
        DX_VERIFY(SetupLights());
        DX_VERIFY(SetupMatrices());
#endif

        //通过Stream的方式绘制 vertices，先指定 Source
        DX_VERIFY(m_pD3dDevice->SetStreamSource( 0, m_pD3dVertexBuffer, 0, sizeof(FISHJAM_VERTEX)));

        //设置FVF的格式，使得D3D知道 m_pD3dVertexBuffer 中 Vertex 的格式
        DX_VERIFY(m_pD3dDevice->SetFVF( D3DFVF_FISHJAM_VERTEX ));
        //输出

        //DX_VERIFY(m_pD3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1));  //输出三角形
        DX_VERIFY(m_pD3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2*50-2)); //输出圆柱
        
                                               
        // End the scene
        DX_VERIFY(m_pD3dDevice->EndScene());
    }

    // Present the back buffer contents to the display
    DX_VERIFY(m_pD3dDevice->Present( NULL, NULL, NULL, NULL ));

    return hr;
}

void CMainFrame::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // Do not call CFrameWnd::OnPaint() for painting messages
    Render();
    //ValidateRect( NULL );
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    Render();
    //CFrameWnd::OnTimer(nIDEvent);
}
