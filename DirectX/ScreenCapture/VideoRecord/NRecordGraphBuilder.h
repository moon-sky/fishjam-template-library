#pragma once

#include "VideoRecordInclude.h"

class INRecordGraphBuilder
{
public:
	virtual ~INRecordGraphBuilder(){}
public:
	virtual HRESULT CreateVideoSrcFilter(IBaseFilter** ppSrcFilter, IBaseFilter** ppSmartTee, IPin** ppCapturePin, IPin** ppPreviewPin) = 0;
	virtual HRESULT CreateVideoEncoderFilter(IBaseFilter** ppFilter, IPin** ppInputPin, IPin** ppOutputPin) = 0;

	virtual HRESULT CreateAudioSrcFilter(IBaseFilter** ppSrcFilter) = 0;

	virtual HRESULT CreateMuxFilter(IBaseFilter** ppFilter, IPin** ppVideoInPin, IPin** ppAudioInPin, IPin** ppOutPin) = 0;
	virtual HRESULT CreateTargetFilter(LPWSTR pwszTargetFileName, IBaseFilter** ppFilter, IPin** ppInputPin) = 0;
};

//TODO: need factory?

//declar forward
struct NRecordParam;

class CDefaultRecordGraphBuilder : public INRecordGraphBuilder
{
public:
	CDefaultRecordGraphBuilder(IGraphBuilder* pGraphBuilder, const NRecordParam& param);

	//INRecordGraphBuilder
	virtual HRESULT CreateVideoSrcFilter(IBaseFilter** ppSrcFilter, IBaseFilter** ppSmartTee, IPin** ppCapturePin, IPin** ppPreviewPin);
	virtual HRESULT CreateVideoEncoderFilter(IBaseFilter** ppFilter, IPin** ppInputPin, IPin** ppOutputPin);

	virtual HRESULT CreateAudioSrcFilter(IBaseFilter** ppSrcFilter);

	virtual HRESULT CreateMuxFilter(IBaseFilter** ppFilter, IPin** ppVideoInPin, IPin** ppAudioInPin, IPin** ppOutPin);
	virtual HRESULT CreateTargetFilter(LPWSTR pwszTargetFileName, IBaseFilter** ppFilter, IPin** ppInputPin);
protected:
	CComPtr<IGraphBuilder>	m_spGraphBuilder;
	const NRecordParam&		m_Param;
};

