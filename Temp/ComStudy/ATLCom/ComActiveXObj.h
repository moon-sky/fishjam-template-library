// ComActiveXObj.h : Declaration of the CComActiveXObj
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "ATLCom.h"
#include "_IComActiveXObjEvents_CP.h"

class CComActiveXObjLic
{
protected:
   static BOOL VerifyLicenseKey(BSTR bstr)
   {
      return !lstrcmpW(bstr, L"ComActiveXObj license");
   }

   static BOOL GetLicenseKey(DWORD dwReserved, BSTR* pBstr) 
   {
   	  if( pBstr == NULL )
 		return FALSE;
      *pBstr = SysAllocString(L"ComActiveXObj license"); 
      return TRUE;
   }

   static BOOL IsLicenseValid() 
   {  
	   return TRUE; 
   }
};

// CComActiveXObj
class ATL_NO_VTABLE CComActiveXObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CStockPropImpl<CComActiveXObj, IComActiveXObj>,
	public IPersistStreamInitImpl<CComActiveXObj>,      
	public IOleControlImpl<CComActiveXObj>,
	public IOleObjectImpl<CComActiveXObj>,
	public IOleInPlaceActiveObjectImpl<CComActiveXObj>,
	public IViewObjectExImpl<CComActiveXObj>,
	public IOleInPlaceObjectWindowlessImpl<CComActiveXObj>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CComActiveXObj>,
	public CProxy_IComActiveXObjEvents<CComActiveXObj>, 
	public IObjectWithSiteImpl<CComActiveXObj>,
	public IServiceProviderImpl<CComActiveXObj>,
	public IPersistStorageImpl<CComActiveXObj>,
	public ISpecifyPropertyPagesImpl<CComActiveXObj>,   //组件支持属性页特性，通过其 GetPages 方法返回所有属性页的 GUID
	public IQuickActivateImpl<CComActiveXObj>,
	public IDataObjectImpl<CComActiveXObj>,
	public IProvideClassInfo2Impl<&CLSID_ComActiveXObj, &__uuidof(_IComActiveXObjEvents), &LIBID_ATLComLib>,
	public IPropertyNotifySinkCP<CComActiveXObj>,       //提供属性变化通知
	public CComCoClass<CComActiveXObj, &CLSID_ComActiveXObj>,
	public CComControl<CComActiveXObj>
{
public:
	CContainedWindow m_ctlButton;

#pragma warning(push)
#pragma warning(disable: 4355) // 'this' : used in base member initializer list

	CComActiveXObj()
		: m_ctlButton(_T("Button"), this, 1)
	{
		m_bWindowOnly = TRUE;
	}

#pragma warning(pop)

DECLARE_CLASSFACTORY2(CComActiveXObjLic)

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)

DECLARE_REGISTRY_RESOURCEID(IDR_COMACTIVEXOBJ)

BEGIN_COM_MAP(CComActiveXObj)
	COM_INTERFACE_ENTRY(IComActiveXObj)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IServiceProvider)
END_COM_MAP()

BEGIN_PROP_MAP(CComActiveXObj)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	PROP_ENTRY("BackColor", DISPID_BACKCOLOR, CLSID_StockColorPage)
	PROP_ENTRY("Caption", DISPID_CAPTION, CLSID_NULL)
	PROP_ENTRY("Enabled", DISPID_ENABLED, CLSID_NULL)
	PROP_ENTRY("FillColor", DISPID_FILLCOLOR, CLSID_StockColorPage)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CComActiveXObj)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
	CONNECTION_POINT_ENTRY(__uuidof(_IComActiveXObjEvents))
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CComActiveXObj)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
	COMMAND_CODE_HANDLER(BN_CLICKED, OnBNClicked)
	CHAIN_MSG_MAP(CComControl<CComActiveXObj>)
ALT_MSG_MAP(1)
	// Replace this with message map entries for superclassed Button
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnBNClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO : Add your code here.
		return 0;
	}

	LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT lRes = CComControl<CComActiveXObj>::OnSetFocus(uMsg, wParam, lParam, bHandled);
		if (m_bInPlaceActive)
		{
			if(!IsChild(::GetFocus()))
				m_ctlButton.SetFocus();
		}
		return lRes;
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		RECT rc;
		GetWindowRect(&rc);
		rc.right -= rc.left;
		rc.bottom -= rc.top;
		rc.top = rc.left = 0;
		m_ctlButton.Create(m_hWnd, rc);
		return 0;
	}

	STDMETHOD(SetObjectRects)(LPCRECT prcPos,LPCRECT prcClip)
	{
		IOleInPlaceObjectWindowlessImpl<CComActiveXObj>::SetObjectRects(prcPos, prcClip);
		int cx, cy;
		cx = prcPos->right - prcPos->left;
		cy = prcPos->bottom - prcPos->top;
		::SetWindowPos(m_ctlButton.m_hWnd, NULL, 0,
			0, cx, cy, SWP_NOZORDER | SWP_NOACTIVATE);
		return S_OK;
	}

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] = 
		{
			&IID_IComActiveXObj,
		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IComActiveXObj
	OLE_COLOR m_clrBackColor;
	void OnBackColorChanged()
	{
		ATLTRACE(_T("OnBackColorChanged\n"));
	}
	CComBSTR m_bstrCaption;
	void OnCaptionChanged()
	{
		ATLTRACE(_T("OnCaptionChanged\n"));
	}
	BOOL m_bEnabled;
	void OnEnabledChanged()
	{
		ATLTRACE(_T("OnEnabledChanged\n"));
	}
	OLE_COLOR m_clrFillColor;
	void OnFillColorChanged()
	{
		ATLTRACE(_T("OnFillColorChanged\n"));
	}
	STDMETHOD(_InternalQueryService)(REFGUID guidService, REFIID riid, void** ppvObject)
	{
		return E_NOTIMPL;
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}
};

OBJECT_ENTRY_AUTO(__uuidof(ComActiveXObj), CComActiveXObj)
