#include "stdafx.h"
#include "NVideoRecord.h"
#include "NRecordGraph.h"

CNVideoRecord::CNVideoRecord(HWND hWndOwner)
{
	m_pRecordResult = NULL;
	m_pNRecordGraph = new CNRecordGraph();
}

CNVideoRecord::~CNVideoRecord()
{
	SAFE_DELETE(m_pNRecordGraph);
}


HRESULT CNVideoRecord::SetOption(const VideoRecordOption& option)
{
	HRESULT hr = S_OK;
	m_RecordOption = option;

	//TODO: Check Option
	return hr;
}

HRESULT CNVideoRecord::Record(IVideoRecordResult* pRecordResult)
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(pRecordResult, E_POINTER);
	m_pRecordResult = pRecordResult;

	HRESULT hr = E_FAIL;
	
	::MessageBox(NULL, TEXT("Choose Target Rect and wait Recording"), TEXT("In CNVideoRecord::Record"), MB_OK);

	//CVideoRecord Dlg(ChooseRect);
	//dlg.DoModal();
	CWindowDC wndDC(::GetDesktopWindow());
	//CDC memDC;
	//memDC.CreateCompatibleDC(wndDC);
	//CBitmap bmp;
	//bmp.CreateCompatibleBitmap(wndDC, 100, 100);
	//HBITMAP hOldBmp = memDC.SelectBitmap(bmp);
	//memDC.BitBlt(0, 0, 100, 100, wndDC, 0, 0, SRCCOPY);
	//memDC.SelectBitmap(hOldBmp);
	//m_pRecordResult->OnVideoRecordResult(SysAllocString(L"C:\\Users\\Public\\Videos\\Sample Videos\\Ò°Éú¶¯Îï.wmv"), bmp.Detach(), 0);

	return hr;
}