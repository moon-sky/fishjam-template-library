#pragma once

class IFColorSpaceConverter
{
public:
	virtual ~IFColorSpaceConverter() {}
public:
	virtual HRESULT SetConvertParam(GUID& rSubMediaTypeIn, GUID& rSubMediaTypeOut) = 0;
	virtual HRESULT ConvertCoverSpace(IMediaSample* pMediaSampeIn, IMediaSample* pMediaSampleOut) = 0;
};