// MyTimer.h : Declaration of the CMyTimer

#pragma once
#include "resource.h"       // main symbols

#include "ATLCom.h"
#include "_IMyTimerEvents_CP.h"


// CMyTimer

class ATL_NO_VTABLE CMyTimer : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMyTimer, &CLSID_MyTimer>,
    //通过连接点向导自动加入的继承列表
    public IConnectionPointContainerImpl<CMyTimer>,
    //public IProvideClassInfo2Impl<&CLSID_MyTimer,&DIID__IMyTimerEvents>,  //实现连接点时，通常实现该接口，方便客户端获进行Advise
	public CProxy_IMyTimerEvents<CMyTimer>, 
	public IDispatchImpl<IMyTimer, &IID_IMyTimer, &LIBID_ATLComLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CMyTimer()
	{
        m_idTimer = 0;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MYTIMER)


BEGIN_COM_MAP(CMyTimer)
	COM_INTERFACE_ENTRY(IMyTimer)
	COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)      //连接点容器，用于查找连接点
END_COM_MAP()

//由连接点向导加入的连接点映射
BEGIN_CONNECTION_POINT_MAP(CMyTimer)
	CONNECTION_POINT_ENTRY(__uuidof(_IMyTimerEvents))
END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}
private:
    // 保存 TIMER 的ID号，为KillTimer()用
    UINT_PTR m_idTimer;

public:
    //连接点的测试函数
    STDMETHOD(SetTimer)(LONG uElapse);
    STDMETHOD(KillTimer)(void);
};

OBJECT_ENTRY_AUTO(__uuidof(MyTimer), CMyTimer)
