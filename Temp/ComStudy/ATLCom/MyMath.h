// MyMath.h : Declaration of the CMyMath

#pragma once
#include "resource.h"       // main symbols

#include "ATLCom.h"
#include "ScriptSupport.h"          
#include "_IMyTimerEvents_CP.H"     //连接点向导会自动生成该文件的内容

#define USE_FREE_THREADED_MARSHALER   //使用“聚合”的方式重用系统提供的自由线程管理调度器
//如果使用了Both模型，需要将组件和自由线程管理调度器聚合在一起,否则的话，在客户端使用时可能出错
//  如：客户端使用 COINIT_APARTMENTTHREADED 初始化时，会出现“Access violation reading location”的错误


#define MAX_CALLBACK_COUNT  10
//#define USE_IPERSISTSTREAMINITIMPL

// CMyMath

class ATL_NO_VTABLE CMyMath : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CMyMath, &CLSID_MyMath>,
	public IDispatchImpl<IMyDispatch, &IID_IMyDispatch, &LIBID_ATLComLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
    //public ISupportErrorInfoImpl<&CLSID_MyMath>,
    public ISupportErrorInfo,
    //public IMyMath,
    public IMyMath2,     //由于IMyMath2 继承自IMyMath，因此继承列表中不再需要 IMyMath
#ifdef USE_IPERSISTSTREAMINITIMPL
    public IPersistStreamInitImpl<CMyMath>, //也可以从ATL提供的接口实现中继承，再添加必要的映射即可
#else
    public IPersistStreamInit,              // 手工添加持续性接口 -- 用于计算素数时的暂停
#endif
    public IActiveScriptSiteImpl<CMyMath>,   //自己做的 ScriptSite 实现模版，尚未仔细测试
    public IComponentInfo                   //使用自定义结构的接口
{
public:
	CMyMath();
    ~CMyMath();

DECLARE_REGISTRY_RESOURCEID(IDR_MYMATH)


BEGIN_COM_MAP(CMyMath)
    //在这个地方加入接口，客户端才能进行 QueryInterface
    COM_INTERFACE_ENTRY(IMyMath)
    COM_INTERFACE_ENTRY(IMyMath2)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IMyDispatch)

    // 手工添加接口映射表入口
    COM_INTERFACE_ENTRY(IPersistStreamInit)
    // 表示如果要取得 IPersistStream 指针，则返回 IPersistStreamInit 指针
    COM_INTERFACE_ENTRY_IID(IID_IPersistStream, IPersistStreamInit)
    // 表示如果要取得 IPersist 指针，则返回 IPersistStremInit 指针
    COM_INTERFACE_ENTRY_IID(IID_IPersist, IPersistStreamInit)
    COM_INTERFACE_ENTRY(IActiveScriptSite)
    COM_INTERFACE_ENTRY(IComponentInfo)
#ifdef USE_FREE_THREADED_MARSHALER
    COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal,m_pUnkMarshaler.p)
#endif
END_COM_MAP()


// 手工添加属性映射表，这是 IPersistXXXImpl 所必须的。在写 ActiveX 的时候，ATL 向导会帮我们添加属性映射表
#ifdef USE_IPERSISTSTREAMINITIMPL
BEGIN_PROP_MAP(CMyMath)
    // 参数："属性名称", 接口属性序号(见IDL文件), 属性页对话窗
    PROP_ENTRY("str", 1, CLSID_NULL)
    PROP_ENTRY("integer", 2, CLSID_NULL)
    //PROP_ENTRY_EX
    //PROP_PAGE
    //PROP_DATA_ENTRY
END_PROP_MAP()
#endif

// ISupportsErrorInfo
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()
    DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
#ifdef USE_FREE_THREADED_MARSHALER
        return CoCreateFreeThreadedMarshaler(GetControllingUnknown(),
            &m_pUnkMarshaler.p);
#else
		return S_OK;
#endif
	}
	
	void FinalRelease() 
	{
#ifdef USE_FREE_THREADED_MARSHALER
        m_pUnkMarshaler.Release();
#endif
	}

private:
    // 存储回调接口指针的数组
    IMyMathCallBack* m_pCallBack[MAX_CALLBACK_COUNT];
    // 从这里开始计算素数
    LONG m_nNow;

    // 内部持续性数据是否已经变化了
    BOOL m_bDirty;

#ifdef USE_FREE_THREADED_MARSHALER
    CComPtr<IUnknown> m_pUnkMarshaler;
#endif
    
public:
#ifdef USE_IPERSISTSTREAMINITIMPL
    // 这个成员变量，是 IPersistXXXImpl 所必须的，其作用和 m_bDirty 相同
    BOOL m_bRequiresSave;	// 表示属性数据是否已经改变而需要保存
#endif

    STDMETHOD(Add)(LONG n1, LONG n2, LONG* pResult);
    STDMETHOD(Mul)(LONG n1, LONG n2, LONG* pResult);
    STDMETHOD(Sum)(LONG nArraySize, LONG nArray[], LONG* plResult);

    STDMETHOD(GetPrim)(LONG *plCount,LONG **ppLData);
    
    //使用指定枚举的方法
    STDMETHOD(Compute)(/*[in]*/mathOPERATION enumOp, /*[in]*/LONG lOp1, /*[in]*/LONG lOp2,/*[out,retval]*/ LONG* plResult);

    STDMETHOD(Upper)(BSTR str, BSTR* pVal);
    STDMETHOD(DemoMarshalLongOperation)(BSTR strParam);

    //CallBack
    STDMETHOD(Sub)(LONG n1, LONG n2);
    STDMETHOD(MyAdvise)(IMyMathCallBack* pCallBack, LONG* pdwCookie);
    STDMETHOD(MyUnAdvise)(LONG dwCookie);

    //用于演示持续性的接口方法
    STDMETHOD(GetNext)(LONG* pVal);

    STDMETHOD(GetComponentInfo)(COMPONENT_INFO** ppInfo);

    // IPersist
    STDMETHOD(GetClassID)(/*[out]*/CLSID * pClassID);
    // IPersistStream
    STDMETHOD(IsDirty)(void);
    STDMETHOD(Load)(/*[in]*/IStream *pStm);
    STDMETHOD(Save)(/*[in]*/IStream *pStm,/*[in]*/BOOL fClearDirty);
    STDMETHOD(GetSizeMax)(/*[out]*/ULARGE_INTEGER *pcbSize);
    // IPersistStreamInit
    STDMETHOD(InitNew)(void);
    STDMETHOD(LengthyComOperation)(LONG nTimeout);
};

OBJECT_ENTRY_AUTO(__uuidof(MyMath), CMyMath)
