#pragma once
#include "oaidl.h"

class CTimerSink : public IDispatch
{
public:
    CTimerSink(void);
    ~CTimerSink(void);
public:
    void SetResultWnd(CWnd * pWnd);
    // IUnknown
    STDMETHOD(QueryInterface)(const struct _GUID &iid,void ** ppv);
    STDMETHOD_(ULONG,AddRef)(void);
    STDMETHOD_(ULONG,Release)(void);
    // IDispatch
    STDMETHOD(GetTypeInfoCount)(unsigned int *);
    STDMETHOD(GetTypeInfo)(unsigned int,unsigned long,struct ITypeInfo ** );
    STDMETHOD(GetIDsOfNames)(const IID &,LPOLESTR *,UINT,LCID,DISPID *);
    STDMETHOD(Invoke)(long dispID,const struct _GUID &,unsigned long,unsigned short,struct tagDISPPARAMS * pParams,struct tagVARIANT *,struct tagEXCEPINFO *,unsigned int *);
private:
    CWnd*   m_pWndResult;
};
