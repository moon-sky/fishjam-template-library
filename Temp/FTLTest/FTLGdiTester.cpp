#include "StdAfx.h"
#include "FTLGdiTester.h"
#include <WindowsX.h>
#include <ftlGdi.h>
#include <ftlDebug.h>

CFTLGdiTester::CFTLGdiTester()
{
    m_hDesktopDC = ::GetWindowDC(NULL);
    m_hDC = NULL;
    m_hBitmap = NULL;
    m_hOldBitmap = NULL;
}

CFTLGdiTester::~CFTLGdiTester()
{
    ::ReleaseDC(NULL, m_hDesktopDC);
}

void CFTLGdiTester::setUp()
{
    m_hDC = ::CreateCompatibleDC(m_hDesktopDC);
    API_ASSERT(NULL != m_hDC);
}

void CFTLGdiTester::tearDown()
{
    BOOL bRet = FALSE;
    API_VERIFY(::DeleteDC(m_hDC));
    m_hDC = NULL;
}

void CFTLGdiTester::setBitmap()
{
    m_hBitmap = :: CreateCompatibleBitmap(m_hDesktopDC, BITMAP_WIDTH, BITMAP_HEIGHT);
    API_ASSERT(NULL != m_hBitmap);
    m_hOldBitmap = (HBITMAP)::SelectObject(m_hDC, (HGDIOBJ)m_hBitmap);
}

void CFTLGdiTester::releaseBitmap()
{
    m_hBitmap = (HBITMAP)::SelectObject(m_hDC, (HGDIOBJ)m_hOldBitmap);
    ::DeleteObject(m_hBitmap);
    m_hBitmap = NULL;
}

void CFTLGdiTester::test_Clipping()
{
    BOOL bRet = FALSE;

    CRect rcClip;
    rcClip.SetRectEmpty();
    
    //刚创建好的时候是一个 [0,0] x [1,1] 的矩形
    const CRect rcInit(0,0,1,1);
    int nClipType = GetClipBox(m_hDC, rcClip);
    CPPUNIT_ASSERT(SIMPLEREGION  == nClipType);
    CPPUNIT_ASSERT(rcInit == rcClip);


    //选入位图后，裁减区是位图的大小 [0,0] x [100,100]
    setBitmap();
    const CRect rcBitmap(0,0, BITMAP_WIDTH, BITMAP_HEIGHT);
    nClipType = GetClipBox(m_hDC, rcClip);
    CPPUNIT_ASSERT(SIMPLEREGION  == nClipType);
    CPPUNIT_ASSERT(rcBitmap == rcClip);

    //通过原裁减区与指定矩形的“与”运算，计算新的裁减区
    //此时因为输入的是空矩形 [0,0]x[0,0]，所以新的裁减区为空 -- 
    nClipType = ::IntersectClipRect(m_hDC, 0,0,0,0);
    CPPUNIT_ASSERT(COMPLEXREGION == nClipType);    
    nClipType = GetClipBox(m_hDC, rcClip);
    CPPUNIT_ASSERT(NULLREGION == nClipType);
    const CRect rcEmpty(0,0,0,0);
    CPPUNIT_ASSERT(rcEmpty == rcClip);

    //创建和位图一样大的区域，并作为裁减区 -- 裁减区是位图的大小 [0,0] x [100,100]
    HRGN hRgnBmp = ::CreateRectRgnIndirect(rcBitmap);
    nClipType = ::SelectClipRgn(m_hDC, hRgnBmp);
    CPPUNIT_ASSERT(SIMPLEREGION == nClipType);
    nClipType = GetClipBox(m_hDC, rcClip);
    CPPUNIT_ASSERT(SIMPLEREGION  == nClipType);
    CPPUNIT_ASSERT(rcBitmap == rcClip);

    //从裁减区中排除掉左上角 [0,0]x[50,50] 的矩形(空了一个洞)，形成一个复杂的裁减区
    nClipType = ExcludeClipRect(m_hDC, 0,0,50,50);
    CPPUNIT_ASSERT(COMPLEXREGION == nClipType);
    nClipType = GetClipBox(m_hDC, rcClip);      //但此时包围裁减区的矩形仍然为 rcBitmap
    CPPUNIT_ASSERT(COMPLEXREGION == nClipType);
    CPPUNIT_ASSERT(rcBitmap == rcClip);

    //再从裁减区中减去左下角的矩形 -- 最后剩下的就只有右半边的矩形裁减区了
    nClipType = ExcludeClipRect(m_hDC, 0,50,50,100);
    CPPUNIT_ASSERT(COMPLEXREGION == nClipType); //此时刚裁减完，返回值仍然是 复杂裁减区
    nClipType = GetClipBox(m_hDC, rcClip);      //但此时包围裁减区的矩形仍然为 rcBitmap
    CPPUNIT_ASSERT(SIMPLEREGION == nClipType);  //★ 此时通过重新计算，已经可以计算出是简单矩形 ★
    const CRect rcRightHalfBitmap(50,0,100,100);
    CPPUNIT_ASSERT(rcRightHalfBitmap == rcClip);

    //将剩下的右半边的矩形和占据位图整个矩形的区域进行XOR，则会剩下左半边的矩形为裁减区
    nClipType = ::ExtSelectClipRgn(m_hDC, hRgnBmp, RGN_XOR);
    CPPUNIT_ASSERT(SIMPLEREGION == nClipType);
    nClipType = GetClipBox(m_hDC, rcClip);
    CPPUNIT_ASSERT(SIMPLEREGION == nClipType);
    const CRect rcLeftHalfBitmap(0,0,50,100);
    CPPUNIT_ASSERT(rcLeftHalfBitmap == rcClip);

    
    //★ 资料说 SelectClipRgn(xx,NULL)后结果为NULL，但实测后是 rcBitmap ?
    //   使用 RGN_COPY 将 NULL 选为裁减区，则新的裁减区为空 -- 为什么不是这样
    //::SelectClipRgn(m_hDC,NULL);
    //nClipType = ::ExtSelectClipRgn(m_hDC, NULL, RGN_COPY);
    //CPPUNIT_ASSERT(NULLREGION == nClipType);
    //nClipType = GetClipBox(m_hDC, rcClip);
    //CPPUNIT_ASSERT(NULLREGION == nClipType);
    //CPPUNIT_ASSERT(rcEmpty == rcClip);


    API_VERIFY(DeleteObject(hRgnBmp));
    releaseBitmap();
}

void CFTLGdiTester::test_GdiObjectInfoDump()
{
    BOOL bRet = FALSE;
    FTL::CFGdiObjectInfoDump gdiObjDump;

    //测试 OBJ_BITMAP
    HBITMAP hBmp = ::CreateCompatibleBitmap(m_hDesktopDC, 100,100);
    CHECK_BITMAP_SELECTABLE(hBmp);

    //刚刚创建的位图，没有 Dimension ? 需要设置的话，有什么用 ?
    SIZE szBitmapDimension = {0};
    API_VERIFY(::GetBitmapDimensionEx(hBmp, &szBitmapDimension));
    FTLTRACE(TEXT("%d,%d\n"), szBitmapDimension.cx, szBitmapDimension.cy);
    CPPUNIT_ASSERT(0 == szBitmapDimension.cx && 0 == szBitmapDimension.cy);

    CPPUNIT_ASSERT(OBJ_BITMAP == ::GetObjectType(hBmp));
   //刚创建的，能获取到信息
    CPPUNIT_ASSERT(gdiObjDump.GetGdiObjectInfo(hBmp));
    FTLTRACE(TEXT("%s\n"), gdiObjDump.GetGdiObjectInfoString());
    API_VERIFY(::DeleteObject(hBmp));
    //删除以后就获取不到了
    CPPUNIT_ASSERT(!gdiObjDump.GetGdiObjectInfo(hBmp));

    //测试 Canvas 中的 CreateDIBSection 创建的 DIBSECTION
    CFCanvas canvas;
    API_VERIFY(canvas.Create(NULL, 100, 100, 32));
    HBITMAP hDibBitmap = canvas.GetMemoryBitmap();
    
#if 0
    //因为这个位图已经被选入canvas 的MemoryDC中，所以不是"Selectable"的 -- 如果启用这句的话，会出现断言
    CHECK_BITMAP_SELECTABLE(hDibBitmap);
#endif 

    CPPUNIT_ASSERT(OBJ_BITMAP == ::GetObjectType(hDibBitmap));
    CPPUNIT_ASSERT(gdiObjDump.GetGdiObjectInfo(hDibBitmap));
    FTLTRACE(TEXT("%s\n"), gdiObjDump.GetGdiObjectInfoString());
    canvas.Release();
    //释放了DIB，但因为 hDibBitmap 还指向那原来的位置，获取信息(不能获取到)
    FTLASSERT(hDibBitmap != NULL);
    CPPUNIT_ASSERT(!gdiObjDump.GetGdiObjectInfo(hDibBitmap));


    HPEN hStockPen = GetStockPen(NULL_PEN);
    CHECK_GDIOBJECT_VALIDATE(hStockPen);
    CPPUNIT_ASSERT(gdiObjDump.GetGdiObjectInfo(hStockPen));
    FTLTRACE(TEXT("%s\n"), gdiObjDump.GetGdiObjectInfoString());

    HBRUSH hStockBrush = GetStockBrush(NULL_BRUSH);
    CPPUNIT_ASSERT(gdiObjDump.GetGdiObjectInfo(hStockBrush));
    FTLTRACE(TEXT("%s\n"), gdiObjDump.GetGdiObjectInfoString());

    HFONT hStockFont = GetStockFont(DEVICE_DEFAULT_FONT);
    CPPUNIT_ASSERT(gdiObjDump.GetGdiObjectInfo(hStockFont));
    FTLTRACE(TEXT("%s\n"), gdiObjDump.GetGdiObjectInfoString());

    HPALETTE hStockPalette = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
    CPPUNIT_ASSERT(gdiObjDump.GetGdiObjectInfo(hStockPalette));
    FTLTRACE(TEXT("%s\n"), gdiObjDump.GetGdiObjectInfoString());
   
}

void CFTLGdiTester::test_GdiObjectChecker()
{
    BOOL bRet = FALSE;

    HDC hMemoryDC = ::CreateCompatibleDC(m_hDesktopDC);
    HBITMAP hBmp = ::CreateCompatibleBitmap(m_hDesktopDC, 100,100);

    //刚刚创建的位图, 是有效的, 且是没有选入任何DC的
    FTL::CFGdiObjectChecker   checker(hMemoryDC);
    CPPUNIT_ASSERT(checker.IsValidate(hBmp));
    CPPUNIT_ASSERT(!checker.IsSelected(hBmp));

    //将位图选入 MemoryDC 中
    HBITMAP hOldBmp = SelectBitmap(hMemoryDC, hBmp);
    CPPUNIT_ASSERT(checker.IsSelected(hBmp));

#ifdef CHECK_NORMAL
    //测试正常流程 -- 先将 BITMAP 从MemoryDC中选出，再进行释放
    SelectBitmap(hMemoryDC, hOldBmp);

    CPPUNIT_ASSERT(checker.IsValidate(hBmp));
    //已经选出来了
    CPPUNIT_ASSERT(!checker.IsSelected(hBmp));

    //释放以后，位图则不再有效
    API_VERIFY(::DeleteObject(hBmp));
    CPPUNIT_ASSERT(!checker.IsValidate(hBmp));

    API_VERIFY(::DeleteDC(hMemoryDC));

#else // defined( CHECK_DELETE_BEFORE_SELECTOUT)
	UNREFERENCED_PARAMETER(hOldBmp);

    //在没有从MemoryDC中选出 BITMAP 的情况下，直接 DeleteObject, API会返回TRUE -- 但实际上该 BITMAP 尚未释放。
    CPPUNIT_ASSERT(::DeleteObject(hBmp));

    //虽然调用了 ::DeleteObject, 但因为BITMAP未从MemoryDC中选出，因此位图资源仍未释放，且为选中状态
    CPPUNIT_ASSERT(checker.IsValidate(hBmp));  
    CPPUNIT_ASSERT(checker.IsSelected(hBmp));

    //释放内存DC -- 此时系统会自动将BITMAP的资源释放掉，不会出现资源泄漏 (但这是一种不好的使用方式)
    API_VERIFY(::DeleteDC(hMemoryDC));
    CPPUNIT_ASSERT(!checker.IsValidate(hBmp));  
    CPPUNIT_ASSERT(0 == ::GetObjectType(hBmp));  //因为hBmp 已经释放，所以无法再获取到其类型
    //ERROR_ACCESS_DENIED
#endif
    

}

void CFTLGdiTester::test_Region()
{
    BOOL bRet = FALSE;
    CRect rcRegion;


    //[0,0] x [100,100] 矩形的四个点
    POINT ptPolygon[] = { {0,0}, {0,100}, {100,100}, {100,0}};

    HRGN hRgnPolygon = CreatePolygonRgn(ptPolygon, _countof(ptPolygon), ALTERNATE);
    int rgnType = ::GetRgnBox(hRgnPolygon, rcRegion);
    CPPUNIT_ASSERT(SIMPLEREGION == rgnType);
    CPPUNIT_ASSERT(0 == rcRegion.left);
    CPPUNIT_ASSERT(0 == rcRegion.top);
    CPPUNIT_ASSERT(100 == rcRegion.right);
    CPPUNIT_ASSERT(100 == rcRegion.bottom);

    //[0,0] x [100,100] 矩形的四个点 和 [150,150]->[150,200]->[200,150] 的三角形
    POINT ptPolyPolygon[] = { 
         {0,0}, {0,100}, {100,100}, {100,0},
         {150,150}, {150,200}, {200,150}
    };

    INT nPolyCount[] = {4, 3};
    HRGN hRgnPolyPolygon = ::CreatePolyPolygonRgn(ptPolyPolygon, nPolyCount, _countof(nPolyCount), ALTERNATE);
    rgnType = ::GetRgnBox(hRgnPolyPolygon, rcRegion);
    CPPUNIT_ASSERT(COMPLEXREGION == rgnType);
    //返回的矩形是包含区域(一个矩形 + 一个三角)的最小内切矩形
    CPPUNIT_ASSERT(0 == rcRegion.left);
    CPPUNIT_ASSERT(0 == rcRegion.top);
    CPPUNIT_ASSERT(200 == rcRegion.right);
    CPPUNIT_ASSERT(200 == rcRegion.bottom);

    //释放创建的区域资源
    API_VERIFY(::DeleteObject(hRgnPolygon));
    API_VERIFY(::DeleteObject(hRgnPolyPolygon));
}

void CFTLGdiTester::test_CompatibleBitmap()
{
    //总结：创建兼容位图时，★不能★使用才创建好的内存兼容DC

    BOOL bRet = FALSE;
    HBITMAP hDesktopBmp = NULL;
    BITMAP bmpInfo = {0};

    FTL::HDCProperty prop;
    FTL::CFGdiUtil::GetHDCProperty(m_hDesktopDC, &prop);
    FTLTRACE(TEXT("%s\n"), prop.GetPropertyString(HDC_PROPERTY_GET_DEVICE_CAPS));

    //直接获取桌面DC中的位图对象
    hDesktopBmp = (HBITMAP)::GetCurrentObject(m_hDesktopDC,OBJ_BITMAP);
    API_ASSERT(NULL != hDesktopBmp);
    CPPUNIT_ASSERT(NULL != hDesktopBmp);

#pragma TODO(Why CAN NOT Get Desktop bitmap info)
    //获取桌面DC中位图对象的属性 -- 为什么能获取到位图句柄，但不能获取其属性信息？
    //  测试中发现 ::GetObject 返回失败 -- 错误码为 ERROR_INSUFFICIENT_BUFFER(122), 可能是不能跨进程获取 ?
#if 0
    //CHECK_BITMAP_SELECTABLE(hDesktopBmp);
    API_VERIFY( 0 != ::GetObject(hDesktopBmp, sizeof(bmpInfo), &bmpInfo));
    CPPUNIT_ASSERT(0 == bmpInfo.bmWidthBytes
        && 0 == bmpInfo.bmPlanes
        && 0 == bmpInfo.bmBitsPixel);
    hDesktopBmp = NULL;
#endif 

    //创建和桌面DC的兼容位图 -- 支持32位真彩
    hDesktopBmp = ::CreateCompatibleBitmap(m_hDesktopDC, 10,10);
    ::GetObject(hDesktopBmp, sizeof(bmpInfo), &bmpInfo);
    CPPUNIT_ASSERT(40 == bmpInfo.bmWidthBytes
        && 1 == bmpInfo.bmPlanes
        && 32 == bmpInfo.bmBitsPixel);

    //创建和桌面兼容的内存DC，并在其基础上创建兼容位图 -- 由于内存DC中没有选入位图，因此兼容位图只支持最基本的黑白两色
    HDC hMemDcFromDesktop = ::CreateCompatibleDC(m_hDesktopDC);
    HBITMAP hMemDCBmp = ::CreateCompatibleBitmap(hMemDcFromDesktop, 10,10);
    ::GetObject(hMemDCBmp, sizeof(bmpInfo), &bmpInfo);
    CPPUNIT_ASSERT(2 == bmpInfo.bmWidthBytes
        && 1 == bmpInfo.bmPlanes
        && 1 == bmpInfo.bmBitsPixel);
    ::DeleteObject(hMemDCBmp);

    //将创建好的32位兼容位图选入兼容内存DC，然后重新创建兼容位图 -- 此时兼容位图也能支持32位真彩
    HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDcFromDesktop, hDesktopBmp);
    hMemDCBmp = ::CreateCompatibleBitmap(hMemDcFromDesktop, 10,10);
    API_VERIFY(0 != ::GetObject(hMemDCBmp, sizeof(bmpInfo), &bmpInfo));
    CPPUNIT_ASSERT(40 == bmpInfo.bmWidthBytes
        && 1 == bmpInfo.bmPlanes
        && 32 == bmpInfo.bmBitsPixel);

	SelectObject(hMemDcFromDesktop, hOldBmp);
	API_VERIFY(::DeleteObject(hMemDCBmp));
    API_VERIFY(::DeleteDC(hMemDcFromDesktop));
    API_VERIFY(::DeleteObject(hDesktopBmp));
}

void CFTLGdiTester::test_CalcPointPosQuadrant()
{
	CPPUNIT_ASSERT_EQUAL(posTopLeft, CFGdiUtil::CalcPointPosQuadrant(CRect(1,1,5,3), CPoint(1, 0)));
	CPPUNIT_ASSERT_EQUAL(posTopRight, CFGdiUtil::CalcPointPosQuadrant(CRect(1,1,5,3), CPoint(5, 0)));
	CPPUNIT_ASSERT_EQUAL(posRightTop, CFGdiUtil::CalcPointPosQuadrant(CRect(1,1,5,3), CPoint(10, 0)));
}


LPCTSTR CFTLGdiTester::GetZoomModeString(INT nZoomMode)
{
	switch (nZoomMode)
	{
	case CFCalcRect::modeAutoFitIfBigSize:
		return TEXT("AutoFitIfBigSize");
	case CFCalcRect::modeAutoFit:
		return TEXT("AutoFit");
	default:
		ATLASSERT(FALSE);
		return NULL;
	}
}

void CFTLGdiTester::test_CalcRectCheckInfo(LPCTSTR pszPromptText, CalcRectCheckInfo* pCheckInfo, int nCount)
{
	ATLTRACE(TEXT("Enter Test %s\n"), pszPromptText);
	for (int index = 0; index < nCount; index++ )
	{
		CFCalcRect* pCalcRect = CFCalcRect::GetCalcRectObject(
			(FTL::CFCalcRect::ZoomMode)pCheckInfo[index].nZoomMode
			//pCheckInfo[index].pRcCurrent
			);
		if (pCalcRect)
		{
			CRect rcResult = pCalcRect->GetFitRect(
				pCheckInfo[index].rcClient,
				pCheckInfo[index].szImage
				//pCheckInfo[index].dZoomFactor,		
				//FALSE,										//bFirst
				//FALSE										//bFirstView
				);
			SAFE_DELETE(pCalcRect);

			FTL::CFRectDumpInfo	rcDumpInfoClient(pCheckInfo[index].rcClient);
			FTL::CFRectDumpInfo	rcDumpInfoCheck(pCheckInfo[index].rcCheck);
			FTL::CFRectDumpInfo	rcDumpInfoResult(rcResult);

			FTLTRACE(TEXT("\tCheck [%d] Mode=%s(%d), rcClient=%s, Size=%dx%d, rcCheck=%s, rcResult=%s\n"), 
				index,
				GetZoomModeString(pCheckInfo[index].nZoomMode),
				pCheckInfo[index].nZoomMode,
				rcDumpInfoClient.GetConvertedInfo(),
				pCheckInfo[index].szImage.cx, pCheckInfo[index].szImage.cy,
				rcDumpInfoCheck.GetConvertedInfo(),
				rcDumpInfoResult.GetConvertedInfo()
				);
			CPPUNIT_ASSERT(rcResult == pCheckInfo[index].rcCheck);
		}
	}

	FTLTRACE(TEXT("Leave Test %s\n"), pszPromptText);
}


void CFTLGdiTester::test_CalcRect_RectBigThanSize()
{
	CalcRectCheckInfo	calcRectCheckInfo[] = 
	{
		//Rect > Size
		{ 
			CFCalcRect::modeAutoFitIfBigSize,
			//NULL,
			CRect(100, 200, 1024 + 100, 768 + 200),
			CSize(100, 100),
			CRect(562, 534, 662, 634),	//100x100
			//1.0
		},

		{ 
			CFCalcRect::modeAutoFit,
			//NULL,
			CRect(100, 200, 1024 + 100, 768 + 200),
			CSize(100, 100),
			CRect(228, 200, 996, 968),	//768x768
			//1.0
		},
	};

	test_CalcRectCheckInfo(TEXT("RectBigThanSize"), calcRectCheckInfo, _countof(calcRectCheckInfo));
}

void CFTLGdiTester::test_CalcRect_RectSmallThanSize()
{

}


void CFTLGdiTester::test_CompareBitmapData()
{
    int nWidth = 100, nHeight = 100;

    BOOL bRet = FALSE;
    HWND hWndDesktop = GetDesktopWindow();
    HDC hDcDesktop = GetDC(hWndDesktop);

    int nBpps[] = {32, 24, 16 }; 
    int nCmpResultSize = nWidth * nHeight * 4;
    CFMemAllocator<byte> cmpResult(nCmpResultSize);

    for (int i = 0; i< _countof(nBpps); i++)
    {
        FTL::CFCanvas canvas1, canvas2;

        API_VERIFY(canvas1.Create(hWndDesktop, nWidth, nHeight, nBpps[i]));
        API_VERIFY(canvas2.Create(hWndDesktop, nWidth, nHeight, nBpps[i]));

        API_VERIFY(BitBlt(canvas1.GetCanvasDC(), 0, 0, nWidth, nHeight, hDcDesktop, 0, 0, SRCCOPY));
        API_VERIFY(BitBlt(canvas2.GetCanvasDC(), 0, 0, nWidth, nHeight, hDcDesktop, 0, 0, SRCCOPY));
        
        CDC memDC;
        memDC.Attach(canvas2.GetCanvasDC());
        memDC.FillSolidRect(0, 0, 5, 5, RGB(255, 0, 0));

        memDC.FillSolidRect(nWidth - 5, nHeight - 5, 5, 5, RGB(255, 0, 0));

        memDC.Detach();

        FTL::CFStringFormater formaterName1, formaterName2;
        formaterName1.Format(TEXT("Canvas1_%d.bmp"), nBpps[i]);
        formaterName2.Format(TEXT("Canvas2_%d.bmp"), nBpps[i]);

        FTL::CFGdiUtil::SaveBitmapToFile(canvas1.GetMemoryBitmap(), formaterName1.GetString());
        FTL::CFGdiUtil::SaveBitmapToFile(canvas2.GetMemoryBitmap(), formaterName2.GetString());
        RECT rcMinDiff = {0};
        int nCmpResult = CFGdiUtil::CompareBitmapData(nWidth, nHeight, nBpps[i], 
            canvas1.GetBuffer(), canvas2.GetBuffer(), &rcMinDiff,
            cmpResult.GetMemory(), nCmpResultSize, 0xFF);
        FTLTRACE(TEXT("nBpp=%d, nCmpResult=%d\n"), nBpps[i], nCmpResult);

        CPPUNIT_ASSERT(nCmpResult == 50);
    }
 
    ReleaseDC(hWndDesktop, hDcDesktop);
}

void CFTLGdiTester::test_LargetBitmap()
{
	BOOL bRet = FALSE;
	CFCanvas	canvas;
	CRect rcLarge;
	rcLarge.SetRect(0, 0, 5124, 5124);

	API_VERIFY(canvas.Create(NULL, rcLarge.Width(), rcLarge.Height(), 32));
	HBITMAP hDibBitmap = canvas.GetMemoryBitmap();
	CPPUNIT_ASSERT(hDibBitmap != NULL);

    
    CWindowDC wndDC(CWnd::FromHandle(GetDesktopWindow()));
    ::StretchBlt(canvas.GetCanvasDC(), 0, 0, rcLarge.Width(), rcLarge.Height(), wndDC.GetSafeHdc(), 0, 0, 1080, 1080, SRCCOPY);

	//CDC memDC;
	//API_VERIFY(memDC.Attach(canvas.GetCanvasDC()));
	//memDC.FillSolidRect(&rcLarge, RGB(255, 0, 0));
	//memDC.Detach();

    API_VERIFY(canvas.SaveToBmpFile(TEXT("LargeBitmap.bmp")));
	//API_VERIFY(CFGdiUtil::SaveBitmapToFile(canvas.GetMemoryBitmap(), TEXT("LargeBitmap.bmp")));


	canvas.Release();
}