#include "ColorSpaceConverter.h"

class CBaseColorSpaceConvertImpl : public IFColorSpaceConverter
{
public:
	CBaseColorSpaceConvertImpl();
	~CBaseColorSpaceConvertImpl();

	virtual HRESULT SetConvertParam(GUID& rSubMediaTypeIn, GUID& rSubMediaTypeOut);
protected:
	GUID	m_guidSubMediaTypeIn;
	GUID	m_guidSubMediaTypeOut;
};

class CColorSpaceConvertRGB32ToNV12Impl : public CBaseColorSpaceConvertImpl
{
public:
	virtual HRESULT ConvertCoverSpace(IMediaSample* pMediaSampeIn, IMediaSample* pMediaSampleOut);
};