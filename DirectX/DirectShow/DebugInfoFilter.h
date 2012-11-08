#pragma once

#include "DirectShowDemo_h.h"
#include <ftlfile.h>

/*******************************************************************************************************
* 编写Filter时，建议将应用框架和应用逻辑分离，以设计一个结构清晰、扩展性良好的Filter项目
*   输入流 ==> Filter组件(<-> 应用逻辑块，通常有一个基本的应用逻辑控制基类指针) ==> 输出流
*                 
* 
*******************************************************************************************************/

//extern const IID IID_IFilterDebugInfo;
//extern const GUID CLSID_FilterDebugInfo;
extern const GUID CLSID_FilterDebugInfoProperty;
extern const GUID CLSID_GraphInfoProperty;

extern const AMOVIESETUP_FILTER sudDebugInfoFilter;

//#ifdef __cplusplus 
//extern "C" 
//{
//#endif
//    DECLARE_INTERFACE_(IFilterDebugInfo,IUnknown)
//    {
//        STDMETHOD(GetConnectedPin) (THIS_ 
//            BOOL bIsInput,
//            IPin** ppPin
//           ) PURE;
//    };
//
//#ifdef __cplusplus 
//}
//#endif

//class CDebugInfoInputPin : public CTransInPlaceInputPin
//{
//public:
//	CDebugInfoInputPin(CTransInPlaceFilter *pFilter, HRESULT *phr) 
//		: CTransInPlaceInputPin(TEXT("Input"), pFilter, phr, TEXT("Input"))
//	{
//
//	}
//
//};

//class CDebugInfoOutputPin : public CTransInPlaceOutputPin
//{
//
//};

struct DebugInfoParam
{
	DebugInfoParam()
	{
		m_nInputPinTypeIndex = -1;
		m_nOutputPinTypeIndex = -1;
		m_bDumpSample = FALSE;
		m_dwDumpSampleStartIndex = 0;
		m_dwDumpSampleLimitCount = 10;
		ZeroMemory(&m_bufDumpFilePath,sizeof(TCHAR) * _countof(m_bufDumpFilePath));
	}
	int                 m_nInputPinTypeIndex;       //用户选择的输入Pin的媒体类型索引
	int                 m_nOutputPinTypeIndex;      //用户选择的输出Pin的媒体类型索引
	BOOL                m_bDumpSample;              //是否进行Dump
	DWORD				m_dwDumpSampleStartIndex;   //开始Dump时的序号
	DWORD               m_dwDumpSampleLimitCount;   //Dump时的限制个数
	TCHAR               m_bufDumpFilePath[MAX_PATH];//Dump到的文件路径
};

class CDebugInfoFilter : public CTransInPlaceFilter,
    public ISpecifyPropertyPages,
    public IFilterDebugInfo,
    //public IFileSinkFilter,
    public CPersistStream
{
public:
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);
    STDMETHOD(NonDelegatingQueryInterface)(REFIID riid, void ** ppv);
    
	//重载该方法，知道Filter被加入 Graph
	virtual HRESULT STDMETHODCALLTYPE JoinFilterGraph(__in_opt  IFilterGraph *pGraph, __in_opt  LPCWSTR pName);

    //必须重载的基类纯虚函数
    virtual HRESULT CheckInputType(const CMediaType* mtIn); //UpStream Filter判断本Filter是否支持该媒体类型
    virtual HRESULT Transform(IMediaSample *pSample);

    //可选重载的基类虚函数
    //STDMETHODIMP Stop();
    //STDMETHODIMP Run(REFERENCE_TIME tStart);
    //STDMETHODIMP Pause();

    //这两个函数可以用于初始化和终止化
    virtual HRESULT StartStreaming();
    virtual HRESULT StopStreaming();

    virtual HRESULT CompleteConnect(PIN_DIRECTION dir,IPin *pReceivePin);

    //ISpecifyPropertyPages
    STDMETHOD(GetPages)(/* [out] */ CAUUID *pPages);

    //IFilterDebugInfo
    STDMETHOD(GetConnectedPin)(BOOL bIsInput, IPin** ppPin);
	STDMETHOD(SetAcceptMediaType)(AM_MEDIA_TYPE* pMediaType);
    STDMETHOD(GetFilterDebugParam)(/* [out][in] */FilterDebugParam* pFilterDebugParam);
    STDMETHOD(SetFilterDebugParam)(/* [in] */FilterDebugParam* pFilterDebugParam);

	STDMETHOD(GetFilterRunningInfo)(/*[in, out]*/ FilterRunningInfo* pFilterRunningInfo);
	STDMETHOD(SetFilterRunningInfo)(/*[in]*/ FilterRunningInfo* pFilterRunningInfo);

    // CPersistStream overrides
    HRESULT WriteToStream(IStream *pStream);
    HRESULT ReadFromStream(IStream *pStream);
    int SizeMax();
    STDMETHODIMP GetClassID(CLSID *pClsid);

   // //IFileSinkFilter
   // STDMETHOD(SetFileName)( 
   //     /* [in] */ LPCOLESTR pszFileName,
   //     /* [unique][in] */ const AM_MEDIA_TYPE *pmt);

   //STDMETHOD(GetCurFile)( 
   //     /* [out] */ LPOLESTR *ppszFileName,
   //     /* [out] */ AM_MEDIA_TYPE *pmt);

private:
	static				INT		s_InstanceCount;
	static				BOOL	s_HasAddtoRot;
    CCritSec            m_DebugInfoLock;
	DWORD				m_dwRegister;
	REFERENCE_TIME		m_llLastTimeStart;			//上一次MediaSample的开始时间
	REFERENCE_TIME		m_llLastTimeEnd;			//上一次MediaSample的结束时间
	FTL::CFElapseCounter	m_ElapseCounter;
	DebugInfoParam			m_DebugInfoParam;
	DWORD               m_dwCurrentDumpSampleIndex;
	DWORD				m_dwSampleCount;
	CMediaType*			m_pAcceptMediaType;
	
    CFStructuredStorageFile  m_StorageFile;

    //CFThread<>          m_DumpThread;
    //static DWORD __stdcall DumpThreadFun(void* pParam);

    //HWND m_hWnd;
    //HDC m_hDC;
    //long m_nWidth;
    //long m_nHeight;
    //BITMAPINFOHEADER m_BMI;
    //DWORD * m_pCheckers;

    //void _Clear( );

    CDebugInfoFilter( IUnknown * pUnk, HRESULT * phr );
    ~CDebugInfoFilter( );

    DECLARE_IUNKNOWN;

    HRESULT SetMediaType(PIN_DIRECTION direction,const CMediaType *pmt);
    //// override these to receive indication of when we change
    //// to Pause/Play (Active) or Stop (Inactive) state.
    //HRESULT Active();
    //HRESULT Inactive();
public:
};
