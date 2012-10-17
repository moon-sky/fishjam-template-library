#pragma once

#include "VideoRecordInclude.h"

//declare forward
class	CRecordNotifyProxy;

class CNRecordGraph
{
public:
	CNRecordGraph();
	~CNRecordGraph();
	
	HRESULT Init(const NRecordParam& param, IRecordGraphNotify* pNotify);
	HRESULT Uninit();

	HRESULT Start();
	HRESULT Pause();
	HRESULT Resume();
	HRESULT Stop();

	HRESULT GetCurrentPosition(LONGLONG *pCurrent);

	RecordStatus	GetStatus() const
	{
		//CAutoLock	lock(&m_csLock);
		return		m_status;
	}

protected:
	HRESULT			_BuildGraph();
	HRESULT			_ConnectGraph();
	HRESULT			_TearDownGraph();

private:

#ifdef FTL_DEBUG
	DWORD								m_dwGraphRegister;
#endif
	NRecordParam	m_Param;
	IRecordGraphNotify*	m_pNotify;
	CRecordNotifyProxy* m_pRecordNotifyProxy;
	RecordStatus		m_status;

	//CCritSec		m_csLock;

	//ULONG			m_VidPID;
	//ULONG			m_AudPID;
	
	CComPtr<ICaptureGraphBuilder2>	m_spCaptureGraphBuilder;
	CComPtr<IGraphBuilder>			m_spGraphBuilder;
	CComPtr<IMediaControl>			m_spMediaControl;
	CComPtr<IVideoWindow>			m_spPreviewVideoWindow;
	CComPtr<IMediaSeeking>			m_spMediaSeeking;

	CComPtr<IBaseFilter>			m_spSmartTee;

	CComPtr<IBaseFilter>			m_spVideoSrcFilter;
	CComPtr<IBaseFilter>			m_spAudioSrcFilter;
	CComPtr<IBaseFilter>			m_spVideoEncodeFilter;
	CComPtr<IBaseFilter>			m_spMuxFilter;
	CComPtr<IBaseFilter>			m_spTargetFilter;
};