#include "Stdafx.h"
#include <ddraw.h>
#include ".\mainframe.h"


#pragma comment(lib, "ddraw.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_TIMER()
    ON_WM_DESTROY()
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()


CMainFrame::CMainFrame(void)
{
    m_pDDraw            = NULL;
    m_pDDSPrimary       = NULL;
#ifdef FULLSCREEN_RUNNING
    m_pDDSBack          = NULL;
#endif 
    m_pDDSBackground    = NULL;
} 

CMainFrame::~CMainFrame(void)
{
    SAFE_RELEASE(m_pDDSBackground);
#ifdef FULLSCREEN_RUNNING
    SAFE_RELEASE(m_pDDSBack);
#endif
    SAFE_RELEASE(m_pDDSPrimary);
    SAFE_RELEASE(m_pDDraw);
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    HRESULT hr = E_FAIL;
    //初始化随机数
    srand(GetTickCount());

    //RECT rcClient = { 0,0,800,600 };
    //AdjustWindowRectEx( &rcClient, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL, GetWindowExStyle(hWnd));
    //lpCreateStruct->cx = 800;
    //lpCreateStruct->cy = 600;

    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

#if 0
    //使用COM API 的方式创建 DDraw
    DX_VERIFY(CoCreateInstance(CLSID_DirectDraw, NULL, CLSCTX_ALL, IID_IDirectDraw, (LPVOID *)&m_pDDraw));
    DX_VERIFY(m_pDDraw->Initialize(NULL));
#endif 

    //创建DirectDraw对象 -- 常见的错误有 ：DDERR_INVALIDDIRECTDRAWGUID
    //DX_VERIFY(DirectDrawCreate(NULL,//显卡驱动的GUID，NULL代表默认硬件
    //    &m_pDDraw,
    //    NULL));

    DX_VERIFY(DirectDrawCreateEx(NULL, (LPVOID *)&m_pDDraw, IID_IDirectDraw7, NULL));
    if (SUCCEEDED(hr))
    {
        //检查不出其他接口，DirectX不再使用 QI ？
        //COM_DETECT_INTERFACE_FROM_REGISTER(m_pDDraw);
        COM_DETECT_INTERFACE_FROM_LIST(m_pDDraw);

        //CComPtr<IDirectDraw7> spDdClipper;
        //DX_VERIFY(m_pDDraw->QueryInterface(IID_IDirectDraw7, (LPVOID *)&spDdClipper));
    }
    else
    {
        return -1;
    }

#ifdef FULLSCREEN_RUNNING
    DWORD dwCooperativeLevel = DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWMODEX | DDSCL_ALLOWREBOOT;
#else
    DWORD dwCooperativeLevel = DDSCL_NORMAL;
#endif
    //设置协作级别是以窗口显示还是全屏
    ASSERT(::IsWindow(m_hWnd));
    COM_VERIFY(m_pDDraw->SetCooperativeLevel(m_hWnd,dwCooperativeLevel));

    //设置显示模式 -- 透明地(不涉及硬件)控制视频模式转换
    COM_VERIFY(m_pDDraw->SetDisplayMode(1280,   //dwWidth
        1024,                                   //dwHeight
        32,                                     //dwBPP( bits per pixel, 8,16,24,32)
        0,                                      //dwRefreshRte(刷新频率，0 用默认值)
        0                                       //dwFlags
        ));


    DDSURFACEDESC2              ddsd = {0};
    ddsd.dwSize = sizeof(ddsd);
    // ddsd.lPitch <== 代表每行的字节数，用于安全的访问内存，使用方式为： ddsd.lpSurface[x+y*ddsd.lPitch]=color;
    //    ( 通常是 宽度 * (dpp/8)再4字节对齐？ )
    // ddsd.lpSurface <== 取得指向画面所在的内存(系统内存或显存)的指针
    // ddsd.ddckCKDestBlt <== 控制目标颜色，用于在进行位图转换操作时控制写入的颜色
    // ddsd.ddckCKSrcBlt <== 表示原来的颜色，可对位图设定透明颜色？
    // ddsd.ddpfPixelFormat <== 画面的象素格式

#ifdef FULLSCREEN_RUNNING
    //指出ddsd中有效填充的参数：创建带有一个后台缓冲区的主页面(这种画面就是复杂-COMPLEX-画面)
    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;// | DDSD_HEIGHT | DDSD_WIDTH;
    //ddsd.dwHeight = 1024;
    //ddsd.dwWidth = 1280;

    //指定了一个主页面(DDSCAPS_PRIMARYSURFACE)，一个翻转页面(DDSCAPS_FLIP)和一个复杂页面 (DDSCAPS_COMPLEX)。
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    //ddsd.ddsCaps.dwCaps2 <== 保留用于3D图形
    ddsd.dwBackBufferCount = 1; //后台缓冲页的个数，本处设置为1。大多数场合需要创建一个主画面和一个后台缓冲的分页链
    DX_VERIFY(m_pDDraw->CreateSurface(&ddsd, &m_pDDSPrimary, NULL));

    //获取关联的后台缓冲的页面 -- 三缓冲时怎么用？书上说硬件以循环的方式翻动，对开发透明
    DDSCAPS2                    ddscaps = {0};
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER; 
    DX_VERIFY(m_pDDSPrimary->GetAttachedSurface(&ddscaps, &m_pDDSBack)); 
#else
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    DX_VERIFY(m_pDDraw->CreateSurface(&ddsd, &m_pDDSPrimary, NULL));
#endif 
    
    //创建屏外页面
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwHeight = 480; //设置屏外页面的大小，可以设置成与图片大小相同
    ddsd.dwWidth = 640;
    DX_VERIFY(m_pDDraw->CreateSurface( &ddsd, &m_pDDSBackground, NULL ));

    //加载位图  －－ 注意：此处的代码可能有错误
    //hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap,IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
    //DeleteObject(hbm);
    //DDCopyBitmap(lpDDSOne, hbm, 0, 0, 640, 480);
    //lpDDPal = DDLoadPalette(lpDD, szBackground);	//SDK中Ddutil.h的函数，从Back.bmp文件中创建一个DirectDrawPalette对象
    //lpDDSPrimary->SetPalette(lpDDPal);	//设置调色板
    //DDReLoadBitmap(m_pDDSBackground, szBackground);  // 将位图调入后台缓冲区（调用DDCopyBitmap函数将文件拷贝到后台缓冲区并延展为适当的尺寸）

    //此时通常需要在主页面上绘制初始的信息
    m_TimerID = SetTimer(0,1000, NULL);
    return 0;
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
    HRESULT hr = E_FAIL;
    HDC hdc = NULL;
    //获取设备上下文的句柄且锁定该表面
    //锁定表面的内存（不管是整个表面还是其中的一部分）能确保你的应用程序和系统不会同时访问这块内存。
    //  m_pDDSPrimary->GetDC() <==> m_pDDSPrimary->ReleaseDC()
    //  m_pDDSPrimary->Lock() <==> m_pDDSPrimary->Unlock()  -- 之后可以进行内存的读写操作
    //    DDLOCK_READONLY -- 只读方式锁住
    //    DDLOCK_SURFACEMEMORYPTR -- 应返回一个指向RECT顶端的有效指针(lpSurface[x+y*ddsd.lPitch])
    //      lPitch在 1280x1024,32位色 时是 5120, ddsd.ddpfPixelFormat.dwRGBBitCount 是 32
    //      所以，赋值得时候是否可以用 (DWORD*)lpSurface[x+y*ddsd.lPitch/(dwRGBBitCount/8)] = RGB32() ?
    //    DDLOCK_WRITEONLY -- 可写的方式锁住  
#ifdef FULLSCREEN_RUNNING
    DDSURFACEDESC2 ddsd = {0};
    ddsd.dwSize = sizeof(ddsd);
    DX_VERIFY(m_pDDSBack->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL));
    if (DDERR_SURFACELOST == hr)
    {
        //尝试恢复
        DX_VERIFY_EXCEPT1(m_pDDSBack->Restore(), DDERR_IMPLICITLYCREATED);
        if (FAILED(hr))
        {
            return ;
        }
        DX_VERIFY_EXCEPT1(m_pDDSBack->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL), DDERR_SURFACELOST);
        if (FAILED(hr))
        {
            //尝试恢复不成功，就退出本次绘制
            return ;
        }
    }
    DWORD* pSurfaceColor = ((DWORD*)ddsd.lpSurface);
    //注意：dwRGBBitCount 有可能是15(XRGB555)，因此，这种方法不对
    DWORD dwOffsetSize = ddsd.lPitch / (ddsd.ddpfPixelFormat.dwRGBBitCount / 8);
    for (DWORD y = 0; y < ddsd.dwHeight; y++)
    {
        for (DWORD x = 0; x < ddsd.dwWidth; x++)
        {
            //pSurfaceColor[x + y * dwOffsetSize] = RGB( rand() % 256 , rand() %256, rand() % 256);

            //小端系统中的颜色：A B G R ? 
            pSurfaceColor[x + y * dwOffsetSize] = RGB( 255,//  -- 蓝色
                //255 * (x+1) / ddsd.dwWidth , 
                //255 * (y+1)/ddsd.dwHeight , 
                0,   //绿色
                0);  //红色
        }
    }

    //DX_VERIFY(m_pDDSBack->GetDC(&hdc));
    //SetBkColor( hdc, RGB( 0, 0, 255 ) );
    //SetTextColor( hdc, RGB( 255, 255, 0 ) );
    //SYSTEMTIME st = {0};
    //GetSystemTime(&st);
    //TCHAR buf[20] = {0};
    //StringCchPrintf(buf,_countof(buf),TEXT("%02d:%02d:%02d"),st.wHour,st.wMinute,st.wSecond);
    //RECT rect;
    //GetClientRect(&rect);
    //DrawText(hdc,buf,-1,&rect,DT_CENTER|DT_VCENTER);
    ////解锁表面并释放句柄。
    //COM_VERIFY(m_pDDSBack->ReleaseDC(hdc));

    DX_VERIFY(m_pDDSBack->Unlock(NULL));

    //将屏外表面位位块传输到后台缓冲区
    //m_pDDSBack->BltFast( 0, 0, m_pDDSBackground, &rcRect, DDBLT_WAIT | DDBLT_KEYSRC );

    //主表面和后表面交换
    DX_VERIFY(m_pDDSPrimary->Flip(NULL,DDFLIP_WAIT));
#else
    //窗口模式

#endif 

    CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnDestroy()
{
    KillTimer(m_TimerID);
    SAFE_RELEASE(m_pDDSBackground);
#ifdef FULLSCREEN_RUNNING
    SAFE_RELEASE(m_pDDSBack);
#endif 
    SAFE_RELEASE(m_pDDSPrimary);
    SAFE_RELEASE(m_pDDraw);

    CFrameWnd::OnDestroy();
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (KEY_DOWN(VK_ESCAPE))
    {
        PostMessage(WM_CLOSE,0,0);
    }

    CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
