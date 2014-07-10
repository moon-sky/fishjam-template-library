#include "StdAfx.h"
#include "CalcRect.h"
#include "CalcRectTester.h"
#include <ftldebug.h>


void CCalcRectTester::setUp()
{
}

void CCalcRectTester::tearDown()
{

}

LPCTSTR CCalcRectTester::GetZoomModeString(INT nZoomMode)
{
	switch (nZoomMode)
	{
	case CNPVPhotoCalcRect::E_ZOOM_AUTO_FIT_IF_BIGSIZE:
		return TEXT("AUTO_FIT_IF_BIGSIZE");
		break;
	case CNPVPhotoCalcRect::E_ZOOM_AUTO_FIT:
		return TEXT("AUTO_FIT");
		break;
	case CNPVPhotoCalcRect::E_ZOOM_WIDTH_FIT:
		return TEXT("WIDTH_FIT");
		break;
	case CNPVPhotoCalcRect::E_ZOOM_HEIGHT_FIT:
		return TEXT("HEIGHT_FIT");
		break;
	case CNPVPhotoCalcRect::E_ZOOM_ORIGINAL:
		return TEXT("ORIGINAL");
		break;
	case CNPVPhotoCalcRect::E_ZOOM_NORMAL:
		return TEXT("NORMAL");
		break;
	default:
		return NULL;
	}
}

void CCalcRectTester::test_CalcRectCheckInfo(LPCTSTR pszPromptText, CalcRectCheckInfo* pCheckInfo, int nCount)
{
	ATLTRACE(TEXT("Enter Test %s\n"), pszPromptText);
	for (int index = 0; index < nCount; index++ )
	{
		CNPVPhotoCalcRect* pCalcRect = CNPVPhotoCalcRect::GetPhotoCalcRectObject(
			pCheckInfo[index].nZoomMode, 
			pCheckInfo[index].pRcCurrent
			);
		if (pCalcRect)
		{
			CRect rcResult = pCalcRect->GetPhotoRect(
				&pCheckInfo[index].rcClient, 
				&pCheckInfo[index].szImage, 
				pCheckInfo[index].dZoomFactor,		
				FALSE,										//bFirst
				FALSE										//bFirstView
				);

			CFRectDumpInfo	rcDumpInfoClient(pCheckInfo[index].rcClient);
			CFRectDumpInfo	rcDumpInfoCheck(pCheckInfo[index].rcCheck);
			CFRectDumpInfo	rcDumpInfoResult(rcResult);

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

			SAFE_DELETE(pCalcRect);
		}
	}

	FTLTRACE(TEXT("Leave Test %s\n"), pszPromptText);
}

void CCalcRectTester::test_RectBigThanSize()
{
	CalcRectCheckInfo	calcRectCheckInfo[] = 
	{
		//Rect > Size
		{ 
			CNPVPhotoCalcRect::E_ZOOM_AUTO_FIT_IF_BIGSIZE,
			NULL,
			CRect(100, 200, 1024 + 100, 768 + 200),
			CSize(100, 100),
			CRect(462, 334, 562, 434),	//100x100
			1.0
		},

		{ 
			CNPVPhotoCalcRect::E_ZOOM_AUTO_FIT,
			NULL,
			CRect(100, 200, 1024 + 100, 768 + 200),
			CSize(100, 100),
			CRect(228, 200, 996, 968),	//768x768
			1.0
		},

		{ 
			CNPVPhotoCalcRect::E_ZOOM_WIDTH_FIT,
			NULL,
			CRect(100, 200, 1024 + 100, 768 + 200),
			CSize(100, 100),
			CRect(100, 72, 1124, 1096),	//1024x1024(same as width)
			1.0
		},

		{ 
			CNPVPhotoCalcRect::E_ZOOM_HEIGHT_FIT,
			NULL,
			CRect(100, 200, 1024 + 100, 768 + 200),
			CSize(100, 100),
			CRect(228, 200, 996, 968),	//768x768(same as height)
			1.0
		},

		{ 
			CNPVPhotoCalcRect::E_ZOOM_ORIGINAL,
				NULL,
				CRect(100, 200, 1024 + 100, 768 + 200),
				CSize(100, 100),
				CRect(462, 334, 562, 434),	//100x100(same as size)
				1.0
		},

		{ 
			CNPVPhotoCalcRect::E_ZOOM_NORMAL,
				NULL,
				CRect(100, 200, 1024 + 100, 768 + 200),
				CSize(100, 100),
				CRect(462, 334, 562, 434),	//100x100
				1.0
		}
	};

	test_CalcRectCheckInfo(TEXT("RectBigThanSize"), calcRectCheckInfo, _countof(calcRectCheckInfo));
}

void CCalcRectTester::test_RectSmallThanSize()
{
	CalcRectCheckInfo	calcRectCheckInfo[] = 
	{
		//Rect < Size
		{ 
			CNPVPhotoCalcRect::E_ZOOM_AUTO_FIT_IF_BIGSIZE,
			NULL,
			CRect(0, 0, 100, 100),
			CSize(1024, 768),
			CRect(0, 12, 100, 88),	//100x76
			1.0
		},

		{ 
			CNPVPhotoCalcRect::E_ZOOM_AUTO_FIT,
			NULL,
			CRect(0, 0, 100, 100),
			CSize(1024, 768),
			CRect(0, 12, 100, 88),	//100x76
			1.0
		},

		{ 
			CNPVPhotoCalcRect::E_ZOOM_WIDTH_FIT,
			NULL,
			CRect(0, 0, 100, 100),
			CSize(1024, 768),
			CRect(0, 12, 100, 88),	//100x76
			1.0
		},

		{ 
			CNPVPhotoCalcRect::E_ZOOM_HEIGHT_FIT,
			NULL,
			CRect(0, 0, 100, 100),
			CSize(1024, 768),
			CRect(-16, 0, 116, 100),	//132x100
			1.0
		},

		{ 
			CNPVPhotoCalcRect::E_ZOOM_ORIGINAL,
			NULL,
			CRect(0, 0, 100, 100),
			CSize(1024, 768),
			CRect(-924, -668, 100, 100),	//1024x768
			1.0
		},

		{ 
			CNPVPhotoCalcRect::E_ZOOM_NORMAL,
			NULL,
			CRect(0, 0, 100, 100),
			CSize(1024, 768),
			CRect(-924, -668, 100, 100),	//1024x768
			1.0
		},
	};

	test_CalcRectCheckInfo(TEXT("RectSmallThanSize"), calcRectCheckInfo, _countof(calcRectCheckInfo));
}
