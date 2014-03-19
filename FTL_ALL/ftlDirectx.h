
#ifndef FTL_DIRECTX_H
#define FTL_DIRECTX_H

#pragma once

#ifndef FTL_BASE_H
#  error ftldirectx.h requires ftlbase.h to be included first
#endif

//微软系统提供的DirectShow Filter:  http://technet.microsoft.com/zh-cn/library/dd375464

#include <DShow.h>
#pragma comment( lib, "Quartz.lib" )

#include <d3d9.h>
#include <list>

//#ifdef _D3D9_H_
//如果包含了 <d3d9.h> 文件，就定义使用 D3D9 的宏(使用D3D的错误处理等)
//#define USE_DIRECTX_9 
//#endif 

//#ifdef USE_DIRECTX_9
//#  include <dxerr.h>
//#  pragma comment( lib, "dxerr9.lib" )
//#endif //USE_DIRECTX_9

namespace FTL
{
//Class
    struct MediaSampleProperty;
    class CFDirectXErrorInfo;

//Function
//  DirectShowUtility(namespace)

}//namespace FTL

namespace FTL
{
    ////////////////////////////////////////////////////////////////////////////////////////
    //      Macro
    ////////////////////////////////////////////////////////////////////////////////////////

    //ARGB -- 最高位是Alpha表示是否透明
    #define ARGB16BIT555(a,r,g,b)   ( ((a %2)<<15) + ((r%32)<<10) + ((g%32)<<5) + (b%32) )
    //XRGB -- 最高位没有使用
    #define XRGB16BIT555(r,g,b)     ( ((r%32)<<10) + ((g%32)<<5) + (b%32) )
    //RGB565 -- 绿色是6位(人眼对绿色最敏感)
    #define RGB16BIT565(r,g,b)      ( ((r%32)<<11) + ((g%64)<<6) + (b%32) )

    //判断DirectInput中是否按下指定的兼，用法：if(DI_KEY_DOWN(keyState,DIK_ESCAPE))
    #define DI_KEY_DOWN(s,n) ( s[n] & 0x80 )
    #define DI_MOUSE_KEY_DOWN(m,n) ( m.rgbButtons[n] & 0x80 )

#  define DX_VERIFY(x)   \
    hr = (x);\
    if(S_OK != hr)\
    {\
        REPORT_ERROR_INFO(FTL::CFDirectXErrorInfo, hr,x);\
    }

#  define DX_VERIFY_EXCEPT1(x,hr1)   \
    hr = (x);\
    if(S_OK != hr && (hr1) != hr)\
    {\
        REPORT_ERROR_INFO(FTL::CFDirectXErrorInfo, hr,x);\
    }

#  define DX_VERIFY_EXCEPT2(x,hr1,hr2)   \
    hr = (x);\
    if(S_OK != hr && (hr1) != hr && (hr2) != hr)\
    {\
        REPORT_ERROR_INFO(FTL::CFDirectXErrorInfo, hr,x);\
    }


//#  define DX_VERIFY(x)   \
//    hr = (x);\
//    if(S_OK != hr)\
//    {\
//        DXUTTrace(__FILE__,(DWORD)__LINE__, hr,L#x);\
//    }


    /******************************************************************************************************************
    ******************************************************************************************************************/
    FTLEXPORT class CFDirectXErrorInfo : public CFConvertInfoT<CFDirectXErrorInfo,HRESULT>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFDirectXErrorInfo);
    public:
        FTLINLINE explicit CFDirectXErrorInfo(HRESULT error);
        FTLINLINE virtual LPCTSTR ConvertInfo();
    };

	//DirectShow 中 IMediaEvent::GetEvent 
	FTLEXPORT class CFDirectShowEventInfo : public CFConvertInfoT<CFDirectShowEventInfo, LONG>
	{
	public:
		FTLINLINE explicit CFDirectShowEventInfo(LONG lEvent, LONG_PTR lParam1, LONG_PTR lParam2);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	public:
		LONG_PTR m_lParam1;
		LONG_PTR m_lParam2;
	};

    struct MediaSampleProperty
    {
    public:
        FTLINLINE MediaSampleProperty();
    public:
        BOOL            IsSyncPoint;
        BOOL            IsPreroll;
        BOOL            IsDiscontinuity;
        LONG            Size;
        LONG            ActualDataLength;
        LONGLONG        MediaTimeStart;
        LONGLONG        MediaTimeEnd;
        REFERENCE_TIME  TimeStart;
        REFERENCE_TIME  TimeEnd;
        //AM_MEDIA_TYPE*  pMediaType;
    };

    //! DirectShow的功能函数，但基本都没有测试
    class CFDirectShowUtility
    {
	public:
		//把IGraphBuilder 注册到 Running Object Table (ROT)中，然后可通过 GraphEdt 进行查看
        FTLINLINE static HRESULT AddGraphToRot(IUnknown* pUnkGraph,DWORD* pdwRegister);
        FTLINLINE static HRESULT RemoveGraphFromRot(DWORD dwRegister);

		//拷贝MediaSample的数据
		FTLINLINE static HRESULT CopyMediaSample(IMediaSample *pSource, IMediaSample *pDest);

        //注意：IVideoWindow::put_MessageDrain 指定窗体来接收并处理Video窗口的鼠标和键盘消息
        //调用(尚未测试)：ToggleFullScreen(m_pVW,hMain,&hVideo); 和 ToggleFullScreen(m_pVW,hVideo,NULL); 
        FTLINLINE static HRESULT ToggleFullScreen(IVideoWindow *pVW,HWND hDrain, HWND *phOldDrain);

        //加入一个指定CLSID的Filter
        FTLINLINE static HRESULT AddFilterByCLSID(IGraphBuilder* pGraph, REFGUID clsid,
            LPCWSTR pName, IBaseFilter** ppFilter);

        //得到Filter上未连接的第一个满足majorType的Pin(如果 majorTyte 为 MEDIATYPE_NULL 则不判断majorType)
        FTLINLINE static HRESULT GetUnconnectedPin(IBaseFilter* pFilter,PIN_DIRECTION dir, REFGUID majorType, IPin ** ppPin);

        //!获得Filter上指定方向的第 dirIndex 个Pin -- 不管是否连接
        FTLINLINE static HRESULT GetPin(IBaseFilter* pFilter, PIN_DIRECTION dirRequest ,DWORD dirIndex, IPin** ppFoundPin);

        //!智能连接输出Pin到Filter(第一个未用的输入Pin) -- IGraphBuilder::Connect 是智能连接两个Pin
        FTLINLINE static HRESULT ConnectFilters(IGraphBuilder* pGraph, IPin *pOut, IBaseFilter* pDest);

        //! 智能连接两个Filter(第一个Filter的未用输出Pin -> 第二个Filter未用的输入Pin)
        FTLINLINE static HRESULT ConnectFilters(IGraphBuilder* pGraph, IBaseFilter* pSrc, IBaseFilter* pDest);

        //! 在Filter Graph 中的Filter中查找特定的某种接口(如DV视频解码的 IIPDVDec 等)
        FTLINLINE static HRESULT FindFilterInterface(IFilterGraph* pGraph, REFIID riid, void** ppUnk);

        //!查找指定Filter的Pin上实现的特定接口(如 IID_IReferenceClock 等)
        FTLINLINE static HRESULT FindPinInterface(IBaseFilter* pFilter, REFIID riid, void** ppUnk);

        //! 在Graph中的 Filter 和 Pin 上查找指定接口
        FTLINLINE static HRESULT FindInterfaceAnywhere(IFilterGraph *pGraph, REFIID riid, void** ppUnk);

        //! 获取指定Filter连接着的 上/下 一个Filter
        //! 注意：只能找第一个满足要求(方向)的
        FTLINLINE static HRESULT GetNextFilter(IBaseFilter* pFilter, PIN_DIRECTION dir, IBaseFilter** ppNext);

        //! 清除掉Filter Graph中所有的Filter
        FTLINLINE static HRESULT ClearAllFilters(IFilterGraph *pGraph);

        //从指定Filter开始，断开指定方向的所有Filter(并且可以从Graph中移除) -- 会递归
        FTLINLINE static HRESULT DisconnectDirFilters(IFilterGraph* pGraph, IBaseFilter* pFilter, 
            PIN_DIRECTION dir, BOOL bRemoveFromGraph = FALSE);

        //! 在进行格式转换时，如果不需要预览，可以设置不使用参考时钟，以使数据流中的Sample以最快速度传送。
        //  使用 IFilterGraph::SetDefaultSyncSource 恢复缺省的
        FTLINLINE static HRESULT SetNoClock(IFilterGraph *pGraph, IReferenceClock ** pOldClock = NULL);

        FTLINLINE static HRESULT RenderOutputPins(IGraphBuilder *pGB, IBaseFilter *pFilter);

        FTLINLINE static HRESULT GetMediaSampleProperty(IMediaSample* pSample,MediaSampleProperty* pProperty);

        //TODO:接口设计
        FTLINLINE static HRESULT GetMediaTime(IMediaSeeking *pMS);
        
		FTLINLINE static HRESULT GrapFrameFromMediaFile(LPCTSTR pszFilePath, HBITMAP& hBmp, LONGLONG nUnits = 0);
        //从Filter上抓图
		FTLINLINE static HRESULT SnapShotBitmap(IBasicVideo* pBasicVideo, HBITMAP& hBmpSnap);
        FTLINLINE static HRESULT SnapShotBitmap(IBasicVideo* pBasicVideo, LPCTSTR pszOutFile);

        //! 初始化进行音视频捕获的 Capture Graph Builder
        FTLINLINE static HRESULT InitCaptureGraphBuilder(IGraphBuilder **ppGraph, 
            ICaptureGraphBuilder2 ** ppCapture);

	public:
		//获取 WAVEFORMATEX::wFormatTag 对应的字符信息
        FTLINLINE static LPCTSTR GetWaveFormatExTagString(WORD wWaveFormatTag);

    };//DirectShowUtility

//#ifdef USE_DIRECTX_9
    class Direct3DUtility
    {
    public:
        //封装了应用程序的消息循环。它需要输入一个显示函数的函数指针，在空闲的时候调用显示场景（BusyLoop）
        //参数timeDelta为每一帧的间隔时间，用来控制每秒的帧数
        FTLINLINE static DWORD EnterMsgLoop( bool (*ptr_display)(float timeDelta));

        //调试时使用的函数
        FTLINLINE static DWORD DumpAdapterModes(IDirect3D9* pD3D);
    };

	struct HardwareMonikerInfo
	{
		//std::wstring	strFriendlyName;
		CLSID			clsid;
		WCHAR			wachFriendlyName[120];	//实际上DShow只能找到32字符长(注册表)
		IMoniker*		pMoniker;
		HardwareMonikerInfo()
		{
			clsid = CLSID_NULL;
			wachFriendlyName[0] = 0;
			pMoniker = NULL;
		}
		~HardwareMonikerInfo()
		{
			SAFE_RELEASE(pMoniker);
		}
	};

	//typedef CFSharePtr<HardwareMonikerInfo>	HardwareMonikerInfoPtr;

	typedef std::list<HardwareMonikerInfo*> HardwareMonikerInfoContainer;
	typedef HardwareMonikerInfoContainer::iterator	HardwareMonikerInfoContainerIter;

	class CFDShowHardwareMgr
	{
	public:
		FTLINLINE CFDShowHardwareMgr();
		FTLINLINE ~CFDShowHardwareMgr();

		FTLINLINE HRESULT Refresh(const CLSID* pDevClsid);
		FTLINLINE HRESULT Clear();

        //一般来说 riidResult 都是 IID_IBaseFilter
		FTLINLINE HRESULT GetBindObject(LPCWSTR pszName, REFIID riidResult, void **ppvResult);

		//TODO: Enum
		HardwareMonikerInfoContainer& GetAllHardwares()
		{
			return m_Hardwares;
		}
	protected:
		FTLINLINE HRESULT _AddNewMoniker(IMoniker* pMoniker);
	private:
		HardwareMonikerInfoContainer	m_Hardwares;

		enum
		{
			//DirectShow枚举器中枚举出来的 FriendlyName 的最大长度(包括结束符),会照成无法读取全部名称的问题
			//IMMDeviceEnumerator 接口能读取出完整的名称
			DS_FRIENDLY_NAME_MAX_LENGTH = 32,
		};
	};

//#endif //USE_DIRECTX_9
}
#endif //FTL_DIRECTX_H

#ifndef USE_EXPORT
#  include "ftlDirectx.hpp"
#endif