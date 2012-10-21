#include "StdAfx.h"
#include "ScreenCaptureImpl.h"


BOOL CScreenCaptureImplBase::GetMouseOverLay()
{ 
	return m_bMouseOverlay; 
}

VOID CScreenCaptureImplBase::SetMouseOverLay(BOOL bMouseOverLay) 
{
	m_bMouseOverlay = bMouseOverLay; 
}

BOOL CScreenCaptureImplBase::_OverlayMouseToScreen(HDC hdc, LPRECT lpRect)
{
	BOOL bRet = FALSE;
	POINT point = {0};
    API_VERIFY(::GetCursorPos(&point));
    if (!PtInRect(lpRect, point))
    {
        return TRUE;
    }
    CURSORINFO CursorInfo;
    CursorInfo.cbSize = sizeof(CURSORINFO);

    API_VERIFY(GetCursorInfo(&CursorInfo));
	if (bRet)
    {
        API_VERIFY(::DrawIconEx(hdc, CursorInfo.ptScreenPos.x - lpRect->left, CursorInfo.ptScreenPos.y - lpRect->top , CursorInfo.hCursor,
                     32, 32, 0, NULL, DI_NORMAL));
        ReleaseDC(NULL, hdc);
    }
	return bRet;
}


//////////////////////////////////////////////////////////////////////////  
/// @brief      创建一幅缓存图像。  
/// @param[out]  pHBitmap 生成的位图句柄。  
/// @param[out]  pData  生成的位图中的数据位置。  
/// @param[in]  hDC 目标DC句柄。  
/// @param[in]  width 位图宽度。  
/// @param[in]  height 位图高度。  
/// @param[in]  bitCount 每个像素占多少个bit。  
/// @attention  创建的位图是无压缩格式的。  
/// @author    Loongee  
/// @date      2010/01/15  
//////////////////////////////////////////////////////////////////////////  
#if 0
void CreateBufferBitmap(HBITMAP* pHBitmap, void** pData,   
                        HDC hDC, LONG width, LONG height, WORD bitCount)  
{  
    BITMAPINFO& bmpInfo = *(BITMAPINFO*)new BYTE[sizeof(bmpInfo) + sizeof(RGBQUAD) * 3];  
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  
    bmpInfo.bmiHeader.biWidth = width;  
    bmpInfo.bmiHeader.biHeight = height;  
    bmpInfo.bmiHeader.biPlanes = 1;  
    bmpInfo.bmiHeader.biBitCount = bitCount;  
    if (bitCount == 16)  
    {  
        bmpInfo.bmiHeader.biCompression = BI_BITFIELDS;  
        *(DWORD *)(&bmpInfo.bmiColors[0]) = 0xF800;  
        *(DWORD *)(&bmpInfo.bmiColors[1]) = 0x07E0;  
        *(DWORD *)(&bmpInfo.bmiColors[2]) = 0x001F;  
    }  
    else  
    {  
        bmpInfo.bmiHeader.biCompression = BI_RGB;  
    }  
    bmpInfo.bmiHeader.biSizeImage = 0;  //DIBSIZE(bmpInfo.bmiHeader);   
    bmpInfo.bmiHeader.biXPelsPerMeter = 1000;  
    bmpInfo.bmiHeader.biYPelsPerMeter = 1000;  
    bmpInfo.bmiHeader.biClrUsed = 0;  
    bmpInfo.bmiHeader.biClrImportant = 0;  
    *pHBitmap = CreateDIBSection(hDC, &bmpInfo, DIB_RGB_COLORS  
        , pData, NULL, 0);  
    delete[] (BYTE*)&bmpInfo;  
}
#endif 