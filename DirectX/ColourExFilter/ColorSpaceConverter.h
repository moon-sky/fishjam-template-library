#pragma once

class IColorSpaceConverter
{
public:
	virtual ~IColorSpaceConverter() {}
public:
	HRESULT SetConvertParam(GUID& rSubMediaTypeIn, GUID& rSubMediaTypeOut) = 0;
	HRESULT ConvertCoverSpace(IMediaSample* pMediaSampeIn, IMediaSample* pMediaSampleOut) = 0;
}