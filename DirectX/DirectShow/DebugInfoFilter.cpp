#include "stdafx.h"
#include "DebugInfoFilter.h"

#include <WindowsX.h>
#include <initguid.h>  //使得 DEFINE_GUID 定义为常量，参见 http://support.microsoft.com/kb/130869/en-us
#include "DirectShowDemo_i.c"

// {205769BC-B121-4ca8-A4E6-84A20EF253B7}
//DEFINE_GUID(CLSID_FilterDebugInfo, 
//            0x205769bc, 0xb121, 0x4ca8, 0xa4, 0xe6, 0x84, 0xa2, 0xe, 0xf2, 0x53, 0xb7);
//
//
// {205769BD-B121-4ca8-A4E6-84A20EF253B7}
DEFINE_GUID(CLSID_FilterDebugInfoProperty, 
            0x205769bd, 0xb121, 0x4ca8, 0xa4, 0xe6, 0x84, 0xa2, 0xe, 0xf2, 0x53, 0xb7);

// {205769BE-B121-4ca8-A4E6-84A20EF253B7}
DEFINE_GUID(CLSID_GraphInfoProperty, 
			0x205769be, 0xb121, 0x4ca8, 0xa4, 0xe6, 0x84, 0xa2, 0xe, 0xf2, 0x53, 0xb7);

// {F62AD8B9-AFA6-41bf-9ADE-5A83D80FF3BD}
//DEFINE_GUID(IID_IFilterDebugInfo,
//            0xF62AD8B9,0xAFA6,0x41bf,0x9A,0xDE,0x5A,0x83,0xD8,0x0F,0xF3,0xBD);

// Filter 的注册信息，如果需要，也可以定义Pin的注册信息，如

const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
    &MEDIATYPE_NULL,        // Major type
    &MEDIASUBTYPE_NULL      // Minor type
};
const AMOVIESETUP_PIN psudPins[] =
{
    {
        L"Input",           // String pin name
        FALSE,              // Is it rendered
        FALSE,              // Is it an output
        FALSE,              // Allowed none
        FALSE,              // Allowed many
        &CLSID_NULL,        // Connects to filter
        L"Output",          // Connects to pin
        1,                  // Number of types
        &sudPinTypes 
    },     // The pin details
    { 
        L"Output",          // String pin name
        FALSE,              // Is it rendered
        TRUE,               // Is it an output
        FALSE,              // Allowed none
        FALSE,              // Allowed many
        &CLSID_NULL,        // Connects to filter
        L"Input",           // Connects to pin
        1,                  // Number of types
        &sudPinTypes        // The pin details
    }
};


const AMOVIESETUP_FILTER sudDebugInfoFilter =
{
    &CLSID_DebugInfoFilter, // Filter CLSID
    L"Debug Info Filter",     // Filter name
	MERIT_DO_NOT_USE,
    //MERIT_PREFERRED + 0x88888,       // Its merit
    2,                      // Number of pins
    psudPins                // Pin details
};

static int g_Count = 0;
CUnknown* CDebugInfoFilter::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)
{
    ASSERT(phr);
	if (g_Count > 0)
	{
		*phr = E_OUTOFMEMORY;
		return NULL;
	}

    CDebugInfoFilter *pNewDebugInfoFilter = new CDebugInfoFilter(lpunk, phr);
    if (pNewDebugInfoFilter == NULL) 
	{
        if (phr)
		{
            *phr = E_OUTOFMEMORY;
		}
    }
    return pNewDebugInfoFilter;
}

CDebugInfoFilter::CDebugInfoFilter( IUnknown * pUnk, HRESULT * phr )
:CTransInPlaceFilter(NAME("Debug Info Filter"),pUnk,CLSID_DebugInfoFilter,phr)
,CPersistStream(pUnk, phr)
,m_dwCurrentDumpSampleIndex(0)
,m_dwSampleCount(0)
{
    //DbgSetModuleLevel(LOG_TIMING|LOG_TRACE|LOG_LOCKING|LOG_ERROR, //LOG_MEMORY
    //    1);
	m_ElapseCounter.Stop();
    m_bModifiesData = false;  //不会修改Sample，输入/输出Pin使用同一个Allocator
	m_llLastTimeStart = 0;
	m_llLastTimeEnd = 0;
	m_pAcceptMediaType = NULL;
	g_Count++;
}

CDebugInfoFilter::~CDebugInfoFilter( )
{
	g_Count--;
	SAFE_DELETE(m_pAcceptMediaType);
}

HRESULT CDebugInfoFilter::CheckInputType(const CMediaType* mtIn)
{
    //if (FALSE == IsStopped())
    //{
    //    //不允许动态改变媒体格式
    //    return VFW_E_WRONG_STATE;
    //}
	HRESULT hr = S_OK;
	DisplayType(TEXT("CheckInputType"), mtIn);
	if (m_pAcceptMediaType)
	{
		//if set accept mediatype, then check it
		if(*m_pAcceptMediaType == *mtIn)
		{
			hr = S_OK;
		}
		else
		{
			hr = VFW_E_NO_ACCEPTABLE_TYPES;
		}
	}

	//如果输出Pin上有连接，则让输出pin连接的输入Pin检查，否则直接返回S_OK接受连接

	//if(m_pOutput && m_pOutput->IsConnected())
	//{
	//	CComPtr<IPin> pConnected = m_pOutput->GetConnected();
	//	if (pConnected)
	//	{
	//		hr = pConnected->QueryAccept(mtIn);
	//	}
	//}
    return hr;
}

//在输入Pin成功连接后取得媒体类型描述
HRESULT CDebugInfoFilter::CompleteConnect(PIN_DIRECTION dir,IPin *pReceivePin)
{
    HRESULT hr = E_FAIL;
    DX_VERIFY(__super::CompleteConnect(dir,pReceivePin));
    if (SUCCEEDED(hr) && dir == PINDIR_INPUT)// && m_pInput->IsConnected())
    {
        ASSERT(m_pInput);
        CMediaType mt = m_pInput->CurrentMediaType();
		DisplayType(TEXT("CDebugInfoFilter::CompleteConnect"), &mt);
    }
    return hr;
}

HRESULT CDebugInfoFilter::Transform(IMediaSample *pSample)
{
    CHECK_POINTER_RETURN_VALUE_IF_FAIL(pSample,E_POINTER);
    CAutoLock cObjectLock(&m_csReceive);

    HRESULT hr = S_OK;

	REFERENCE_TIME llTimeStart = 0;
	REFERENCE_TIME llTimeEnd = 0;
	DX_VERIFY_EXCEPT1(pSample->GetTime(&llTimeStart, &llTimeEnd), VFW_E_SAMPLE_TIME_NOT_SET);
	if (SUCCEEDED(hr))
	{
		long nDataLength = pSample->GetActualDataLength();
		FTLTRACEEX(FTL::tlDetail, TEXT("CDebugInfoFilter::Transform, length=%d, StartTime=%f ms, EndTime=%f ms, Start-LastEnd=%lld\n"),
			nDataLength, float(llTimeStart)/10000, float(llTimeEnd)/10000, llTimeStart - m_llLastTimeEnd);

		if (llTimeStart < m_llLastTimeEnd)
		{
			// StartTime 必须 >= m_llLastTimeEnd, 否则在 AVI Mux 时会导致 VFW_E_START_TIME_AFTER_END 的错误
			FTLTRACEEX(FTL::tlWarning, TEXT("%s Warning!! -- VFW_E_START_TIME_AFTER_END, m_llLastTimeEnd(%lld) Should < llTimeStart(%lld)\n"),
				__FILE__LINE__,
				m_llLastTimeEnd, llTimeStart);
		}
		m_llLastTimeStart = llTimeStart;
		m_llLastTimeEnd = llTimeEnd;
	}

    if(m_DebugInfoParam.m_bDumpSample && m_StorageFile.IsOpen())
	{
		if((0 == m_DebugInfoParam.m_dwDumpSampleLimitCount
			|| (m_DebugInfoParam.m_dwDumpSampleStartIndex <= m_dwCurrentDumpSampleIndex 
			&& m_dwCurrentDumpSampleIndex < m_DebugInfoParam.m_dwDumpSampleStartIndex + m_DebugInfoParam.m_dwDumpSampleLimitCount)))
		{
			//IMediaSample2 pSample2; //优先使用该接口？
			LONG lDataLength = pSample->GetActualDataLength();
			//if (lDataLength > 0)
			{
				TCHAR strSampleNum[20] = {0};
				StringCchPrintf(strSampleNum,_countof(strSampleNum),TEXT("%d"),m_dwCurrentDumpSampleIndex);

				COM_VERIFY(m_StorageFile.CreateStorage(strSampleNum,TRUE));

				CComPtr<IStream> pStreamPrperty;
				MediaSampleProperty property;
				DX_VERIFY(CFDirectShowUtility::GetMediaSampleProperty(pSample,&property));
				COM_VERIFY(m_StorageFile.CreateStream(TEXT("Property"),&pStreamPrperty));
				COM_VERIFY(pStreamPrperty->Write(&property,sizeof(property),NULL));


				BYTE *pBuffer = NULL;
				COM_VERIFY(pSample->GetPointer(&pBuffer));
				CComPtr<IStream> pStreamData;
				COM_VERIFY(m_StorageFile.CreateStream(TEXT("Data"),&pStreamData));
				COM_VERIFY(pStreamData->Write(pBuffer,lDataLength,NULL));

				COM_VERIFY(m_StorageFile.ExitStorage());
			}
		}
	}

	m_dwCurrentDumpSampleIndex++;//TODO:Maybe Overflow?
	m_dwSampleCount++;
    return S_OK;
}

HRESULT CDebugInfoFilter::SetMediaType(PIN_DIRECTION direction,const CMediaType *pmt)
{
    HRESULT hr = E_FAIL;
    DisplayType(TEXT("SetMediaType"),pmt);
    DX_VERIFY(__super::SetMediaType(direction,pmt));
    return hr;
}

STDMETHODIMP CDebugInfoFilter::NonDelegatingQueryInterface(REFIID riid, void ** ppv)
{
    CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppv,E_POINTER);

    HRESULT hr = E_FAIL;
    if (IID_IFilterDebugInfo == riid)
    {
        DX_VERIFY(GetInterface((IFilterDebugInfo*)this,ppv));
    }
    else if (IID_ISpecifyPropertyPages == riid) 
    {
        DX_VERIFY(GetInterface((ISpecifyPropertyPages *) this, ppv));
    }
    //else if(IID_IPersist == riid)
    //{
    //    DX_VERIFY(GetInterface((IPersist*)this,ppv));
    //}
    //else if(IID_IFileSinkFilter == riid)
    //{
    //    DX_VERIFY(GetInterface((IFileSinkFilter*)this,ppv));
    //}
    else if(IID_IPersistStream == riid)
    {
        DX_VERIFY(GetInterface((IPersistStream*)this,ppv));
    }
    else 
    {
        hr = __super::NonDelegatingQueryInterface(riid,ppv);
    }
    return hr;
}

HRESULT CDebugInfoFilter::StartStreaming()
{
    HRESULT hr = S_OK;
    DX_VERIFY(CTransInPlaceFilter::StartStreaming());
    CAutoLock cObjectLock(&m_DebugInfoLock);
	m_ElapseCounter.Start();
	if (m_DebugInfoParam.m_bDumpSample && m_DebugInfoParam.m_bufDumpFilePath[0] != TEXT('\0'))
    {
        //if (!m_StorageFile.IsOpen())
        {
            COM_VERIFY(m_StorageFile.CreateDocFile(m_DebugInfoParam.m_bufDumpFilePath));
        }
    }
	m_llLastTimeStart = 0;
	m_llLastTimeEnd = 0;
    return hr;
}

HRESULT CDebugInfoFilter::StopStreaming()
{
    HRESULT hr = S_OK;
    CAutoLock cObjectLock(&m_DebugInfoLock);
	m_ElapseCounter.Stop();

    if (m_StorageFile.IsOpen())
    {
        m_StorageFile.Close();
    }
    DX_VERIFY(CTransInPlaceFilter::StopStreaming());
    return hr;
}

//STDMETHODIMP CDebugInfoFilter::Run(REFERENCE_TIME tStart)
//{
//    CAutoLock cObjectLock(m_pLock);
//    HRESULT hr = E_FAIL;
//    DX_VERIFY(CTransInPlaceFilter::Run(tStart));
//    return hr;
//}
//
//STDMETHODIMP CDebugInfoFilter::Pause()
//{
//    CAutoLock cObjectLock(m_pLock);
//    HRESULT hr = E_FAIL;
//    
//    if (m_bDumpSample && m_bufDumpFilePath[0] != TEXT('\0'))
//    {
//        if (!m_StorageFile.IsOpen())
//        {
//            COM_VERIFY(m_StorageFile.CreateDocFile(m_bufDumpFilePath));
//        }
//    }
//    DX_VERIFY(CTransInPlaceFilter::Pause());
//    return hr;
//}
//
//STDMETHODIMP CDebugInfoFilter::Stop()
//{
//    CAutoLock cObjectLock(m_pLock);
//
//    HRESULT hr = E_FAIL;
//    if (m_bDumpSample && m_StorageFile.IsOpen())
//    {
//        m_StorageFile.Close();
//    }
//
//    DX_VERIFY(CTransInPlaceFilter::Stop());
//    return hr;
//}


//ISpecifyPropertyPages
STDMETHODIMP CDebugInfoFilter::GetPages(CAUUID *pPages)
{
    pPages->cElems = 2;  
    pPages->pElems = (GUID *) CoTaskMemAlloc(sizeof(GUID) * pPages->cElems);
	if (NULL == pPages->pElems)
	{
		return E_OUTOFMEMORY;
	}
    pPages->pElems[0] = CLSID_FilterDebugInfoProperty; //指定属性页的GUID
	pPages->pElems[1] = CLSID_GraphInfoProperty; //指定属性页的GUID
    return S_OK;
}

//IDebugInfoFilter
STDMETHODIMP CDebugInfoFilter::GetConnectedPin(BOOL bIsInput, IPin** ppPin)
{
    CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppPin,E_POINTER);
    
    CAutoLock   locker(&m_DebugInfoLock);

    FTLASSERT(m_pInput);
    FTLASSERT(m_pOutput);
    if (bIsInput)
    {
        if (m_pInput->IsConnected())
        {
            *ppPin = m_pInput->GetConnected();
            (*ppPin)->AddRef();
        }
    }
    else
    {
        if (m_pOutput->IsConnected())
        {
            *ppPin = m_pOutput->GetConnected();
            (*ppPin)->AddRef();
        }
    }
    if (*ppPin)
    {
        return S_OK;
    }
    return VFW_E_NOT_CONNECTED;
}

STDMETHODIMP CDebugInfoFilter::SetAcceptMediaType(AM_MEDIA_TYPE* pMediaType)
{
	HRESULT hr = S_OK;

	CMediaType* pNewAcceptMediaType = new CMediaType(*pMediaType);
	if (!pNewAcceptMediaType)
	{
		hr = E_OUTOFMEMORY;
	}
	else
	{
		SAFE_DELETE(m_pAcceptMediaType);
		m_pAcceptMediaType = pNewAcceptMediaType;
	}

	return hr;
}

STDMETHODIMP CDebugInfoFilter::GetFilterDebugParam(/* [out][in] */FilterDebugParam* pFilterDebugParam)
{
    CHECK_POINTER_RETURN_VALUE_IF_FAIL(pFilterDebugParam,E_POINTER);
    HRESULT hr = S_OK;

    CAutoLock   locker(&m_DebugInfoLock);

    SAFE_FREE_BSTR(pFilterDebugParam->pstrDumpFilePath);

    if (m_DebugInfoParam.m_bufDumpFilePath[0] != TEXT('\0'))
    {
        pFilterDebugParam->pstrDumpFilePath = T2BSTR(m_DebugInfoParam.m_bufDumpFilePath);
    }
    pFilterDebugParam->bDumpSample = m_DebugInfoParam.m_bDumpSample;
	pFilterDebugParam->dwDumpSampleStartIndex = m_DebugInfoParam.m_dwDumpSampleStartIndex;
    pFilterDebugParam->dwDumpSampleLimitCount = m_DebugInfoParam.m_dwDumpSampleLimitCount;

    return hr;
}

STDMETHODIMP CDebugInfoFilter::SetFilterDebugParam(/* [in] */FilterDebugParam* pFilterDebugParam)
{
    CHECK_POINTER_RETURN_VALUE_IF_FAIL(pFilterDebugParam,E_POINTER);
    HRESULT hr = S_OK;

    CAutoLock lock(&m_DebugInfoLock);

    m_DebugInfoParam.m_bDumpSample = pFilterDebugParam->bDumpSample;

	m_DebugInfoParam.m_dwDumpSampleStartIndex = pFilterDebugParam->dwDumpSampleStartIndex;
	m_DebugInfoParam.m_dwDumpSampleLimitCount = pFilterDebugParam->dwDumpSampleLimitCount;
    if (pFilterDebugParam->pstrDumpFilePath)
    {
        COM_VERIFY(StringCchCopy(m_DebugInfoParam.m_bufDumpFilePath,_countof(m_DebugInfoParam.m_bufDumpFilePath),
            OLE2CT(pFilterDebugParam->pstrDumpFilePath)));
    }
    
    m_dwCurrentDumpSampleIndex = 0;
    return hr;
}

STDMETHODIMP CDebugInfoFilter::GetFilterRunningInfo(/*[in, out]*/ FilterRunningInfo* pFilterRunningInfo)
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(pFilterRunningInfo, E_POINTER);
	CAutoLock lock(&m_DebugInfoLock);

	pFilterRunningInfo->llElapse = m_ElapseCounter.GetElapseTime();
	pFilterRunningInfo->dwSampleCount = m_dwSampleCount;
	return S_OK;
}

STDMETHODIMP CDebugInfoFilter::SetFilterRunningInfo(/*[in]*/ FilterRunningInfo* pFilterRunningInfo)
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(pFilterRunningInfo, E_POINTER);
	CAutoLock lock(&m_DebugInfoLock);
	m_dwSampleCount = pFilterRunningInfo->dwSampleCount;
	m_ElapseCounter.Reset();

	return S_OK;
}

// CPersistStream overrides
HRESULT CDebugInfoFilter::WriteToStream(IStream *pStream)
{
    CHECK_POINTER_RETURN_VALUE_IF_FAIL(pStream,E_POINTER);

    HRESULT hr = E_FAIL;
	DX_VERIFY(pStream->Write(&m_DebugInfoParam, sizeof(m_DebugInfoParam), NULL));
 //   DX_VERIFY(pStream->Write(&m_nInputPinTypeIndex,sizeof(m_nInputPinTypeIndex),NULL));
 //   DX_VERIFY(pStream->Write(&m_nOutputPinTypeIndex,sizeof(m_nOutputPinTypeIndex),NULL));
 //   DX_VERIFY(pStream->Write(&m_bDumpSample,sizeof(m_bDumpSample),NULL));
 //	  DX_VERIFY(pStream->Write(&m_dwDumpSampleStartIndex,sizeof(m_dwDumpSampleStartIndex),NULL));
 //   DX_VERIFY(pStream->Write(&m_dwDumpSampleLimitCount,sizeof(m_dwDumpSampleLimitCount),NULL));
 //   DX_VERIFY(pStream->Write(m_bufDumpFilePath,_countof(m_bufDumpFilePath)*sizeof(TCHAR),NULL));
    return hr;
}

HRESULT CDebugInfoFilter::ReadFromStream(IStream *pStream)
{
    CHECK_POINTER_RETURN_VALUE_IF_FAIL(pStream,E_POINTER);

    HRESULT hr = E_FAIL;
	DX_VERIFY(pStream->Read(&m_DebugInfoParam, sizeof(m_DebugInfoParam), NULL));
 //   DX_VERIFY(pStream->Read(&m_nInputPinTypeIndex,sizeof(m_nInputPinTypeIndex),NULL));
 //   DX_VERIFY(pStream->Read(&m_nOutputPinTypeIndex,sizeof(m_nOutputPinTypeIndex),NULL));
 //   DX_VERIFY(pStream->Read(&m_bDumpSample,sizeof(m_bDumpSample),NULL));
 //   DX_VERIFY(pStream->Read(&m_dwDumpSampleStartIndex,sizeof(m_dwDumpSampleStartIndex),NULL));
 //	  DX_VERIFY(pStream->Read(&m_dwDumpSampleLimitCount,sizeof(m_dwDumpSampleLimitCount),NULL));
 //   DX_VERIFY(pStream->Read(m_bufDumpFilePath,_countof(m_bufDumpFilePath)*sizeof(TCHAR),NULL));
    return hr;
}

int CDebugInfoFilter::SizeMax()
{
	int sizeMax = sizeof(m_DebugInfoParam);
    //int sizeMax = sizeof(m_nInputPinTypeIndex) + sizeof(m_nOutputPinTypeIndex)
    //    + sizeof(m_bDumpSample) + sizeof(m_dwDumpSampleStartIndex) + sizeof(m_dwDumpSampleLimitCount) 
    //    + _countof(m_bufDumpFilePath) * sizeof(TCHAR);

    return sizeMax;
}

//进行持久化时，给Graph提供Filter的CLSID
STDMETHODIMP CDebugInfoFilter::GetClassID(CLSID *pClsid)
{
    CHECK_POINTER_RETURN_VALUE_IF_FAIL(pClsid,E_POINTER);
    *pClsid = CLSID_DebugInfoFilter;
    return NOERROR;
}

////IFileSinkFilter
//STDMETHODIMP CDebugInfoFilter::SetFileName(LPCOLESTR pszFileName, const AM_MEDIA_TYPE *pmt)
//{
//    CHECK_POINTER_RETURN_VALUE_IF_FAIL(pszFileName,E_POINTER);
//    HRESULT hr = E_FAIL;
//    if (pszFileName)
//    {
//        USES_CONVERSION;
//        COM_VERIFY(StringCchCopy(m_bufDumpFilePath,_countof(m_bufDumpFilePath),COLE2T(pszFileName)));
//    }
//    return hr;
//}
//
//STDMETHODIMP CDebugInfoFilter::GetCurFile(LPOLESTR *ppszFileName, AM_MEDIA_TYPE *pmt)
//{
//    CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppszFileName,E_POINTER);
//    CHECK_POINTER_RETURN_VALUE_IF_FAIL(pmt,E_POINTER);
//    HRESULT hr = E_FAIL;
//    *ppszFileName = NULL;
//    if (m_bufDumpFilePath[0] != TEXT('\0'))
//    {
//        *ppszFileName = SysAllocString(m_bufDumpFilePath);
//    }
//    return hr;
//}
