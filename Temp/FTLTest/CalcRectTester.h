#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

struct CalcRectCheckInfo
{
	INT		nZoomMode;
	LPRECT  pRcCurrent;
	RECT	rcClient;
	SIZE	szImage;
	RECT	rcCheck;
	DOUBLE	dZoomFactor;
};

class CCalcRectTester : public CPPUNIT_NS::TestFixture
{
public:
    virtual void setUp();
    virtual void tearDown();

	void test_RectBigThanSize();
	void test_RectSmallThanSize();

    CPPUNIT_TEST_SUITE( CCalcRectTester );

    CPPUNIT_TEST( test_RectBigThanSize);
	CPPUNIT_TEST( test_RectSmallThanSize);

    CPPUNIT_TEST_SUITE_END();
    //DECLARE_DEFAULT_TEST_CLASS(CFTLComDetectTester);
private:
	void test_CalcRectCheckInfo(LPCTSTR pszPromptText, CalcRectCheckInfo* pCheckInfo, int nCount);
	LPCTSTR GetZoomModeString(INT nZoomMode);
};
