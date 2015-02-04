// FBhoObject.h : Declaration of the CFBhoObject

#pragma once
#include "resource.h"       // main symbols

#include "FBhoTester_i.h"
#include "_IFBhoObjectEvents_CP.h"
#include <list>

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CFBhoObject

class ATL_NO_VTABLE CFBhoObject :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFBhoObject, &CLSID_FBhoObject>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CFBhoObject>,
	public CProxy_IFBhoObjectEvents<CFBhoObject>,
	public IObjectWithSiteImpl<CFBhoObject>,
	public IDispatchImpl<IFBhoObject, &IID_IFBhoObject, &LIBID_FBhoTesterLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CFBhoObject()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FBHOOBJECT)


BEGIN_COM_MAP(CFBhoObject)
	COM_INTERFACE_ENTRY(IFBhoObject)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IObjectWithSite)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CFBhoObject)
	CONNECTION_POINT_ENTRY(__uuidof(_IFBhoObjectEvents))
END_CONNECTION_POINT_MAP()
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
    STDMETHOD(RegisterEventHandler)(BOOL inAdvise);
    STDMETHOD(SetSite)(IUnknown *pUnkSite);
    STDMETHOD(Invoke)(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,DISPPARAMS FAR* pDispParams,VARIANT FAR* pVarResult,
        EXCEPINFO FAR* pExcepInfo, unsigned int FAR* puArgErr);
    
private:
    CComQIPtr<IWebBrowser2, &IID_IWebBrowser2>  m_spWebBrowser2;        //浏览器组件的指针
    std::list<DWORD>    m_Cookies;                                      //保存与浏览器的连接ID
};

OBJECT_ENTRY_AUTO(__uuidof(FBhoObject), CFBhoObject)
