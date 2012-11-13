
#include "ColorSpaceConverter.h"

class CBaseColorSpaceConvertImpl : public IColorSpaceConverter
{
public:
	CBaseColorSpaceConvertImpl();
	~CBaseColorSpaceConvertImpl();

	HRESULT SetConvertParam(GUID& rSubMediaTypeIn, GUID& rSubMediaTypeOut);
protected:
	GUID	m_guidSubMediaTypeIn;
	GUID	m_guidSubMediaTypeOut;
};

class CColorSpaceConvertRGB32ToNV12Impl : CBaseColorSpaceConvertImpl
{
public:
	HRESULT ConvertCoverSpace(IMediaSample* pMediaSampeIn, IMediaSample* pMediaSampleOut);
};