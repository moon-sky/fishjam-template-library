// MyMath.cpp : Implementation of CMyMath

#include "stdafx.h"
#include "MyMath.h"
#include ".\mymath.h"

// CMyMath

CMyMath::CMyMath()
{
    for( int i=0; i < MAX_CALLBACK_COUNT; i++)	// 初始化所有的回调接口为 NULL
    {
        m_pCallBack[i] = NULL;
    }
    m_nNow = 0;
    m_bDirty = FALSE;
}

CMyMath::~CMyMath()
{
}

STDMETHODIMP CMyMath::InterfaceSupportsErrorInfo(REFIID riid)
{
    static const IID* arr[] = 
    {
        &IID_IMyMath2
    };

    for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
    {
        if (InlineIsEqualGUID(*arr[i],riid))
            return S_OK;
    }
    return S_FALSE;
}

STDMETHODIMP CMyMath::Add(LONG n1, LONG n2, LONG* pResult)
{
    if (pResult)
    {
        *pResult = n1 + n2;
        return S_OK;
    }
    else
    {
        return E_INVALIDARG;
    }
}

STDMETHODIMP CMyMath::Mul(LONG n1, LONG n2, LONG* pResult)
{
    HRESULT hr = E_FAIL;
    if (pResult)
    {
        *pResult = n1 * n2;
        return S_OK;
    }
    else
    {
#if 1
//方法1 -- 原理
        //介绍 ISupportErrorInfo 的设置方法
        CComQIPtr< ICreateErrorInfo> spCEI;     // 声明 ICreateErrorInfo 接口指针
        if (SUCCEEDED(::CreateErrorInfo( &spCEI )))
        {
            spCEI->SetGUID( IID_IMyMath2 );		// 发生错误的接口IID, 如果是系统定义的错误，也可以使用 GUID_NULL

            LPOLESTR pProgID = NULL;
            hr = ::ProgIDFromCLSID( CLSID_MyMath, &pProgID );	// 通用的方法：取得 ProgID
            if (SUCCEEDED(hr))
            {
                spCEI->SetSource(pProgID);
                ::CoTaskMemFree( pProgID );		// 释放内存
            }
            //或
            //spCEI->SetSource( L"ATLCom.MyMath" );	// ProgID 可以在 rgs 文件中看到
            

            // 如果你的组件同时提供了帮助文件，那么就可以：
            spCEI->SetHelpContext( 0 );		// 设置帮助文件的主题号
            //spCEI->SetHelpFile( L"xxx.hlp" );	// 设置帮助文件的文件名

            spCEI->SetDescription( L"错误的参数" );

            CComQIPtr < IErrorInfo > spErrInfo = spCEI; // QueryInterface 得到 IErrorInfo
            if( spErrInfo )
            {
                ::SetErrorInfo( 0, spErrInfo );	// 这时调用者就可以得到错误信息了
            }
        }
        return E_INVALIDARG;
#else
//方法2 -- 使用ATL提供的包装函数
        return Error( L"错误的参数",IID_IMyMath2,E_INVALIDARG);
#endif 
    }
}

STDMETHODIMP CMyMath::Sum(LONG nArraySize, LONG nArray[], LONG* plResult)
{
    *plResult = 0;
    while (nArraySize)
    {
        *plResult += nArray[--nArraySize];
    }
    return S_OK;
}

STDMETHODIMP CMyMath::GetPrim(LONG *plCount,LONG **ppLData)
{
    return E_NOTIMPL;
}

STDMETHODIMP CMyMath::DemoMarshalLongOperation(BSTR strParam)
{
    USES_CONVERSION;
    //ObjectLock lock(this);

    //this->Lock();

    ATLTRACE(TEXT(">>> Enter CMyMath::DemoMarshalLongOperation, lParam=%s,ThreadID=%d,CoCurProcess = %d\n"),OLE2CT(strParam),
        GetCurrentThreadId(),CoGetCurrentProcess());
    //Just Do Slepp
    Sleep(3000);

    ATLTRACE(TEXT("<<< Leave CMyMath::DemoMarshalLongOperation, lParam=%s,ThreadID=%d,CoCurProcess = %d\n"),OLE2CT(strParam),
        GetCurrentThreadId(),CoGetCurrentProcess());

    //this->Unlock();

    return S_OK;
}


STDMETHODIMP CMyMath::Upper(BSTR str, BSTR* pVal)
{
    *pVal = NULL;	// 永远初始化返回值是个好习惯

    CComBSTR s(str);
    s.ToUpper();	// 转换为大写

    *pVal = s.Copy();

    return S_OK;
}

STDMETHODIMP CMyMath::Sub(LONG n1, LONG n2)
{
   LONG nResult = n1 - n2;
   for (int i = 0; i< MAX_CALLBACK_COUNT; i++)
   {
       if( m_pCallBack[i] )						// 如果回调接口有效
           m_pCallBack[i]->Fire_Result( nResult );	// 则发出事件/通知
   }
    return S_OK;
}

STDMETHODIMP CMyMath::MyAdvise(IMyMathCallBack* pCallBack, LONG* pdwCookie)
{
    if( NULL == pCallBack )		// 居然给我一个空指针？！
        return E_INVALIDARG;

    for( int i=0; i<10; i++)	// 寻找一个保存该接口指针的位置
    {
        if( NULL == m_pCallBack[i] )	// 找到了
        {
            m_pCallBack[i] = pCallBack;	// 保存到数组中
            m_pCallBack[i]->AddRef();	// 指针计数器 +1

            *pdwCookie = i+1;			// cookie 就是数组下标+1
            // +1 的目的是避免使用0，因为0表示无效状态

            return S_OK;
        }
    }
    return E_OUTOFMEMORY;	// 超过10个连接，内存不够用啦
}

STDMETHODIMP CMyMath::MyUnAdvise(LONG dwCookie)
{
    if( dwCookie<1 || dwCookie>10 )		// 这是谁干的呀？乱给参数
        return E_INVALIDARG;

    if( NULL == m_pCallBack[ dwCookie -1 ] )	// 参数错误，或该接口指针已经无效了
        return E_INVALIDARG;

    m_pCallBack[ dwCookie - 1 ]->Release();	// 指针计数器 -1
    m_pCallBack[ dwCookie -1 ] = NULL;		// 空出该下标的数组元素

    return S_OK;
}

// IPersist
STDMETHODIMP CMyMath::GetClassID(/*[out]*/CLSID * pClassID)
{
    *pClassID = GetObjectCLSID();
    return S_OK;
}
// IPersistStream
STDMETHODIMP CMyMath::IsDirty(void)
{
    if( m_bDirty )	
        return S_OK;
    else			
        return S_FALSE;
}

STDMETHODIMP CMyMath::Load(/*[in]*/IStream *pStm)
{
    return pStm->Read( &m_nNow, sizeof(m_nNow), NULL);
}

STDMETHODIMP CMyMath::Save(/*[in]*/IStream *pStm,/*[in]*/BOOL fClearDirty)
{
    if( fClearDirty )
    {
        m_bDirty = FALSE;
    }
    return pStm->Write( &m_nNow,sizeof(m_nNow), NULL );
    return S_OK;
}

STDMETHODIMP CMyMath::GetSizeMax(/*[out]*/ULARGE_INTEGER *pcbSize)
{
    //pcbSize->LowPart = 需要保存数据长度的低位;
    //pcbSize->HighPart = 需要保存数据长度的高位;// 一般都是0，难道你的数据长度都超过了 4G？
    pcbSize->LowPart = sizeof(m_nNow);
    pcbSize->HighPart = 0;

    return S_OK;
}

// IPersistStreamInit
STDMETHODIMP CMyMath::InitNew(void)
{
    //内部属性数据默认初始化;
    //设置或清除内部表示数据变化的变量;
    m_nNow = 0;
    m_bDirty = TRUE;

    return S_OK;
}
STDMETHODIMP CMyMath::GetNext(LONG* pVal)
{
    while(1)	// 计算 m_nNow 后的第一个素数
    {
        m_nNow ++;
        long i;
        for( i = 2; i<m_nNow; i++ )
        {
            if( m_nNow % i == 0 )	break;	//能够整除，则不是素数了
        }
        if( i == m_nNow )	break;	// 找到了，停止计算
    }

    *pVal = m_nNow;
    m_bDirty = TRUE;	// 内部持续性数据，已经改变了

    return S_OK;
}

STDMETHODIMP CMyMath::Compute(mathOPERATION enumOp, LONG lOp1, LONG lOp2, LONG* plResult)
{
    switch (enumOp)
    {
    case mathAdd:
        return Add(lOp1,lOp2,plResult);
    case mathSub:
        *plResult = lOp1 - lOp2;
        return S_OK;
    case mathMul:
        return Mul(lOp1,lOp2,plResult);
    case mathDiv:
        if(lOp2 == 0)
        {
            return Error("divide by zero",IID_IMyMath2,E_INVALIDARG);
        }
        *plResult = lOp1/lOp2;
        return S_OK;
    default:
        return E_INVALIDARG;
    }
    return S_OK;
}

STDMETHODIMP CMyMath::GetComponentInfo(COMPONENT_INFO** ppInfo)
{
    *ppInfo = (COMPONENT_INFO*)CoTaskMemAlloc(sizeof(COMPONENT_INFO));
    if (*ppInfo)
    {
        ZeroMemory(*ppInfo,sizeof(COMPONENT_INFO));
        (*ppInfo)->Major = 1;
        (*ppInfo)->Minor = 0;
        TCHAR buf[100] = {0};
        if (LoadString(_AtlBaseModule.GetResourceInstance(),IDS_AUTHOR,buf,_countof(buf)))
        {
            USES_CONVERSION;
            (*ppInfo)->Author = SysAllocString(T2W(buf));
        }
        return S_OK;
    }
    else
    {
        return E_OUTOFMEMORY;
    }
}
STDMETHODIMP CMyMath::LengthyComOperation(LONG nTimeout)
{
    // TODO: Add your implementation code here
    Sleep(nTimeout);
    return S_OK;
}
