#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

struct CalcRectCheckInfo
{
	INT		nZoomMode;
	//LPRECT  pRcCurrent;
	RECT	rcClient;
	SIZE	szImage;
	RECT	rcCheck;
	//DOUBLE	dZoomFactor;
};

class CFTLGdiTester : public CPPUNIT_NS::TestFixture
{
public:
    CFTLGdiTester();
    ~CFTLGdiTester();
    virtual void setUp();
    virtual void tearDown();
public:

    CPPUNIT_TEST_SUITE( CFTLGdiTester );
    CPPUNIT_TEST( test_Clipping );
    CPPUNIT_TEST( test_AlphaBlend );
    CPPUNIT_TEST( test_GdiObjectInfoDump );
    CPPUNIT_TEST( test_GdiObjectChecker );
    CPPUNIT_TEST( test_Region );
	CPPUNIT_TEST( test_CompatibleBitmap );
	CPPUNIT_TEST( test_CalcPointPosQuadrant );

	//测试 CFCalcRect
	CPPUNIT_TEST( test_CalcRect_RectBigThanSize );
	CPPUNIT_TEST( test_CalcRect_RectSmallThanSize );
    CPPUNIT_TEST( test_CompareBitmapData );

    CPPUNIT_TEST( test_LargetBitmap );
    CPPUNIT_TEST( test_AttachBmpFile );

    CPPUNIT_TEST_SUITE_END();

    void test_Clipping();

    void test_AlphaBlend();

    void test_GdiObjectInfoDump();

    void test_GdiObjectChecker();

    //可以合并区域、比较区域、着色和返回区域内部、给区域画边框、得到区域的大小和测试光标是否位于区域的范围
    void test_Region();

    void test_CompatibleBitmap();

	void test_CalcPointPosQuadrant();

	void test_CalcRect_RectBigThanSize();
	void test_CalcRect_RectSmallThanSize();

    void test_CompareBitmapData();

	//使用 CFCanvs(CreateFileMapping) 支持大图片
	void test_LargetBitmap();
    void test_AttachBmpFile();
private:
    enum 
    {
        BITMAP_WIDTH = 100,
        BITMAP_HEIGHT = 100,
    };
    HDC         m_hDesktopDC;
    HDC         m_hDC;
    HBITMAP     m_hBitmap;
    HBITMAP     m_hOldBitmap;

    void        setBitmap();
    void        releaseBitmap();

	void test_CalcRectCheckInfo(LPCTSTR pszPromptText, CalcRectCheckInfo* pCheckInfo, int nCount);
	LPCTSTR GetZoomModeString(INT nZoomMode);
};
