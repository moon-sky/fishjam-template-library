// ComicServiceObj.h : Declaration of the CComicServiceObj

#pragma once
#include "resource.h"       // main symbols

#include "ComicService_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

#include "../ComicHelperProxy/ComicHelperProxy.h"
#include <atlfile.h>
#include <ftlThread.h>

// CComicServiceObj

class ATL_NO_VTABLE CComicServiceObj :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CComicServiceObj, &CLSID_ComicServiceObj>,
	public IDispatchImpl<IComicServiceObj, &IID_IComicServiceObj, &LIBID_ComicServiceLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CComicServiceObj();
    virtual ~CComicServiceObj();

DECLARE_REGISTRY_RESOURCEID(IDR_COMICSERVICEOBJ)


BEGIN_COM_MAP(CComicServiceObj)
	COM_INTERFACE_ENTRY(IComicServiceObj)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease();
private:
    BOOL                                    m_bIsWindows64;
    volatile BOOL                           m_bUserStopProcess;
    LONG                                    m_HeartBeatTicket;
    HANDLE                                  m_hEventStop;
    HANDLE                                  m_hHelperProxyProcess32;
    HANDLE                                  m_hEventProtectNotify32;

    HANDLE                                  m_hHelperProxyProcess64;
    HANDLE                                  m_hEventProtectNotify64;

    ProtectWndInfoFileMap32*                  m_pProtectWndInfoMap;
    FTL::CFThread<DefaultThreadTraits>*     m_pHelperProxyProcessMonitorThread;

    CAtlFileMapping<ProtectWndInfoFileMap32>  m_FileMap;

    static DWORD WINAPI                     _ProcessMonitorProc(LPVOID lpThreadParameter);
    DWORD                                   _InnerProcessMonitorProc();
    BOOL                                    _CreateProxyProcess(BOOL isWin64);
    BOOL                                    _StopHelperProxyProcess();
public:
	STDMETHOD(ProtectWnd)(OLE_HANDLE hWnd, OLE_COLOR clrDisabled);
	STDMETHOD(UnProtectWnd)(OLE_HANDLE hWnd);
    STDMETHOD(HeartBeat)(LONG curTicket);
};

OBJECT_ENTRY_AUTO(__uuidof(ComicServiceObj), CComicServiceObj)
