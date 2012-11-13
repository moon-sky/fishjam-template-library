#include "stdafx.h"
#include "ColorSpaceConverter.h"
#include "ColorSpaceConverterImpl.h"

CBaseColorSpaceConvertImpl::CBaseColorSpaceConvertImpl()
{
	m_guidSubMediaTypeIn = GUID_NULL;
	m_guidSubMediaTypeOut = GUID_NULL;
}

CBaseColorSpaceConvertImpl::~CBaseColorSpaceConvertImpl()
{

}

HRESULT CBaseColorSpaceConvertImpl::SetConvertParam(GUID& rSubMediaTypeIn, GUID& rSubMediaTypeOut)
{
	m_guidSubMediaTypeIn = rSubMediaTypeIn;
	m_guidSubMediaTypeOut = rSubMediaTypeOut;
	return S_OK;
}



HRESULT CColorSpaceConvertRGB32ToNV12Impl::ConvertCoverSpace(IMediaSample* pMediaSampeIn, IMediaSample* pMediaSampleOut)
{
	HRESULT hr = E_FAIL;

}

