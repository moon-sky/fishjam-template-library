#include "StdAfx.h"
#include "FTLGdiPlusTester.h"
#include <GdiPlus.h>
using namespace Gdiplus;
#include <atlpath.h>

void CFTLGdiPlusTester::test_Bitmap()
{
    BOOL bRet = FALSE;
    Status  sts = Gdiplus::Ok;

    TCHAR szPath[MAX_PATH] = {0};
    API_VERIFY(GetModuleFileName(NULL, szPath, _countof(szPath)) > 0);

    CPath pathImage(szPath);
    API_VERIFY(pathImage.RemoveFileSpec());

    pathImage.Append(TEXT("..\\res\\SimpleTest.bmp"));  // in Debug dir

    Bitmap* pBitmap = Bitmap::FromFile(pathImage.m_strPath, FALSE);
    CPPUNIT_ASSERT(pBitmap);
    if (pBitmap)
    {
        CPPUNIT_ASSERT(ImageTypeBitmap == pBitmap->GetType());
        CPPUNIT_ASSERT(pBitmap->GetWidth() == 100);
        CPPUNIT_ASSERT(pBitmap->GetHeight() == 100);

        BitmapData lockedBitmapData;
        Rect rcImage( 0, 0, pBitmap->GetWidth() , pBitmap->GetHeight() );
        GDIPLUS_VERIFY(pBitmap->LockBits(&rcImage, ImageLockModeRead, PixelFormat32bppARGB, &lockedBitmapData));
        if (Ok == sts)
        {
            CPPUNIT_ASSERT(PixelFormat32bppARGB == lockedBitmapData.PixelFormat);
            CPPUNIT_ASSERT(rcImage.Width        == lockedBitmapData.Width);
            CPPUNIT_ASSERT(rcImage.Height       == lockedBitmapData.Height);
            CPPUNIT_ASSERT(rcImage.Width * 4    == lockedBitmapData.Stride);  //100*4--要求按照32位ARGB的方式进行lock, 表示一行的字节数
            RGBQUAD* pClrFirstPixel = (RGBQUAD*)lockedBitmapData.Scan0;
            CPPUNIT_ASSERT(pClrFirstPixel->rgbReserved == 0xFF);              //本来是24位的Bmp位图，要求按照32位ARGB进行lock时, Alpha 会被填充为 0xFF(255)
            CPPUNIT_ASSERT(*(UINT*)pClrFirstPixel == 0xFFDFE9F5);             //第一个像素是BMP文件中最后一行的第一个像素

            GDIPLUS_VERIFY(pBitmap->UnlockBits(&lockedBitmapData));
        }

        GDIPLUS_VERIFY(pBitmap->LockBits(&rcImage, ImageLockModeRead, PixelFormat24bppRGB, &lockedBitmapData));
        if (Ok == sts)
        {
            CPPUNIT_ASSERT(PixelFormat24bppRGB == lockedBitmapData.PixelFormat);
            CPPUNIT_ASSERT(rcImage.Width        == lockedBitmapData.Width);
            CPPUNIT_ASSERT(rcImage.Height       == lockedBitmapData.Height);
            CPPUNIT_ASSERT(rcImage.Width * 3    == lockedBitmapData.Stride);        //100*3--要求按照24位ARGB的方式进行lock, 表示一行的字节数
            RGBTRIPLE* pClrFirstPixel = (RGBTRIPLE*)lockedBitmapData.Scan0;
            CPPUNIT_ASSERT(pClrFirstPixel->rgbtRed == 0xDF
                && pClrFirstPixel->rgbtGreen == 0xE9
                && pClrFirstPixel->rgbtBlue == 0xF5);                    //第一个像素是BMP文件中最后一行的第一个像素

            GDIPLUS_VERIFY(pBitmap->UnlockBits(&lockedBitmapData));
        }
        delete pBitmap;
    }

    CPPUNIT_ASSERT(TRUE);
}