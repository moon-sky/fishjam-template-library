// FanxianBand.h : Declaration of the CFanxianBand

#pragma once
#include "resource.h"       // main symbols

#include "FanXianIEPlugin_i.h"
#include "_IFanxianBandEvents_CP.h"

#include <list>


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

#define ENABLE_INVOKE_DUMP_INFO	0


// CFanxianBand

class ATL_NO_VTABLE CFanxianBand :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFanxianBand, &CLSID_FanxianBand>,
	public IConnectionPointContainerImpl<CFanxianBand>,
	public CProxy_IFanxianBandEvents<CFanxianBand>,
	public IObjectWithSiteImpl<CFanxianBand>,
	//public IInputObject,
	//public IDeskBand,
	public IDispatchImpl<IFanxianBand, &IID_IFanxianBand, &LIBID_FanXianIEPluginLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispEventImpl<ID_FANXIANBAND, CFanxianBand, & DIID_DWebBrowserEvents2, & LIBID_SHDocVw, 1, 0>
{
	DISABLE_COPY_AND_ASSIGNMENT(CFanxianBand);

public:
	CFanxianBand()
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_FANXIANBAND)

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CFanxianBand)
		COM_INTERFACE_ENTRY(IFanxianBand)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY(IObjectWithSite)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CFanxianBand)
		CONNECTION_POINT_ENTRY(__uuidof(_IFanxianBandEvents))
	END_CONNECTION_POINT_MAP()

	BEGIN_SINK_MAP(CFanxianBand)
		//SINK_ENTRY_EX(ID_FANXIANBAND, DIID_DWebBrowserEvents2, DISPID_NEWWINDOW2, OnNewWindow2)
		SINK_ENTRY_EX(ID_FANXIANBAND, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2, OnBeforeNavigate2)
		//SINK_ENTRY_EX(ID_FANXIANBAND, DIID_DWebBrowserEvents2, DISPID_NAVIGATECOMPLETE2, OnNavigateComplete2)
		//SINK_ENTRY_EX(ID_FANXIANBAND, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
		//SINK_ENTRY_EX(ID_FANXIANBAND, DIID_DWebBrowserEvents2, DISPID_NAVIGATEERROR, OnNavigateError)
	END_SINK_MAP()

	BEGIN_CATEGORY_MAP(CFanxianBand)
		IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
		IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
	END_CATEGORY_MAP()

	//DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(RegisterEventHandler)(BOOL inAdvise);

	//	IObjectWithSite -- IWebBrowser2
	STDMETHOD(SetSite)(IUnknown *pUnkSite);

	//	IDispEventImpl
	STDMETHOD(OnBeforeNavigate2)(IDispatch*, VARIANT*, VARIANT*, VARIANT*, VARIANT*, VARIANT*, VARIANT_BOOL*) ;

#if ENABLE_INVOKE_DUMP_INFO
	//调试版本时打印详细信息
	STDMETHOD(Invoke)(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,DISPPARAMS FAR* pDispParams,VARIANT FAR* pVarResult,
		EXCEPINFO FAR* pExcepInfo, unsigned int FAR* puArgErr);
#endif

	BOOL GetWebBrowserPtr( IUnknown* pUnkSite );
private:
	CComQIPtr<IWebBrowser2>  m_spWebBrowser2;        //浏览器组件的指针
	std::list<DWORD>    m_Cookies;                                      //保存与浏览器的连接ID

	HWND				m_hReBar;
};

OBJECT_ENTRY_AUTO(__uuidof(FanxianBand), CFanxianBand)
