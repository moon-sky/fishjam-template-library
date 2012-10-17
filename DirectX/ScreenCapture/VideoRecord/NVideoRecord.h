#pragma once

#include "../ICapture.h"
#include "VideoRecordInclude.h"

//declare forward
class CNRecordGraph;

class CNVideoRecord : public INVideoRecord
{
public:
	CNVideoRecord(HWND hWndOwner);
	virtual ~CNVideoRecord();

	virtual HRESULT SetOption(const VideoRecordOption& option);
	virtual HRESULT Record(IVideoRecordResult* pRecordResult);
	//virtual HRESULT Pause();
	//virtual HRESULT Resume();
	//virtual HRESULT Stop(BOOL bCancel);

	//virtual HRESULT OnStatusChange(RecordStatus oldStatus, RecordStatus newStatus)
private:
	VideoRecordOption		m_RecordOption;
	IVideoRecordResult		*m_pRecordResult;
	CNRecordGraph	        *m_pNRecordGraph;
};