// ComCallerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ComCaller.h"
#include "ComCallerDlg.h"

#include "StorageFileUtil.h"
#include "..\ATLCom\ATLCom_i.c"
#include ".\comcallerdlg.h"
#include "..\AtlServiceDemo\AtlServiceDemo_i.h"
#include "..\AtlServiceDemo\AtlServiceDemo_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//定义该宏的话，会通过 CoMarshal 传递指针，这样子线程中获得的接口实际上是代理的接口(地址不一样)
#define DEMO_COM_MARSHAL

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CComCallerDlg dialog

CComCallerDlg::CComCallerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CComCallerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_dwCallbackCookie = 0;
    m_dwCPCookie = 0;
}

void CComCallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CComCallerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_VIEW_STORAGE_FILE, OnBnClickedBtnViewStorageFile)
    ON_BN_CLICKED(IDC_BTN_DRAW_PIC, OnBnClickedBtnDrawPic)
    ON_BN_CLICKED(IDC_BTN_CALL_SERVICE_EXE_MULTI, OnBnClickedBtnCallServiceExeMulti)

    ON_BN_CLICKED(IDC_BTN_CALL_MYMATH_ADD, OnBnClickedBtnCallMymathAdd)
    ON_BN_CLICKED(IDC_BTN_CALL_MYMATH2_MUL, OnBnClickedBtnCallMymath2Mul)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BTN_VIEW_DISPATCH, OnBnClickedBtnViewDispatch)
    ON_BN_CLICKED(IDC_BTN_IMPORT_UPPER, OnBnClickedBtnImportUpper)
    ON_BN_CLICKED(IDC_BTN_DISPATCH_API, OnBnClickedBtnDispatchApi)
    ON_BN_CLICKED(IDC_BTN_USE_CCOMDISPATCHDRIVER, OnBnClickedBtnUseCcomdispatchdriver)
    ON_BN_CLICKED(IDC_BTN_ERROR_HANDLE, OnBnClickedBtnErrorHandle)
    ON_BN_CLICKED(IDC_BTN_SUB_AND_CALLBACK, OnBnClickedBtnSubAndCallback)
    ON_BN_CLICKED(IDC_BTN_TIMER_CONNECTPOINT, OnBnClickedBtnTimerConnectpoint)
    ON_BN_CLICKED(IDC_BTN_PERSIST, OnBnClickedBtnPersist)
    ON_BN_CLICKED(IDC_BTN_DEMO_MARSHAL, OnBnClickedBtnDemoMarshal)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BTN_SUM_ARRAY, OnBnClickedBtnSumArray)
    ON_BN_CLICKED(IDC_BTN_GET_COMPONENT_INFO, OnBnClickedBtnGetComponentInfo)
    ON_BN_CLICKED(IDC_BTN_DETECT_INTERFACE, OnBnClickedBtnDetectInterface)
    ON_BN_CLICKED(IDC_BTN_MONIKER, OnBnClickedBtnMoniker)
    ON_BN_CLICKED(IDC_BTN_WIA, OnBnClickedBtnWia)
    ON_BN_CLICKED(IDC_BTN_LENGTHY_COM_OPERATION, OnBnClickedBtnLengthyComOperation)
    ON_BN_CLICKED(IDC_BTN_EVENT_HOOK, &CComCallerDlg::OnBnClickedBtnEventHook)
END_MESSAGE_MAP()

// 做个假的就可以，因为反正这个对象在程序结束前是不会退出的
STDMETHODIMP_(ULONG) CComCallerDlg::AddRef()
{
    return 1;
}

STDMETHODIMP_(ULONG) CComCallerDlg::Release()
{
    return 0;
}

STDMETHODIMP CComCallerDlg::QueryInterface(const struct _GUID &iid,void ** ppv)
{
    *ppv=this;
    return S_OK;
}

STDMETHODIMP CComCallerDlg::Fire_Result(LONG nResult)
{
    CString str;
    str.Format(TEXT("Com Sub Fire_Result: %d"),nResult);
    SetWindowText(str);
    return S_OK;
}

// CComCallerDlg message handlers

BOOL CComCallerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    HRESULT hr = E_FAIL;
    COM_VERIFY(m_spMyMath2.CoCreateInstance(CLSID_MyMath));
    COM_VERIFY(m_spMyTimer.CoCreateInstance(CLSID_MyTimer));

    m_spMyMath2->MyAdvise(this,&m_dwCallbackCookie);
    //COM_DETECT_INTERFACE_FROM_LIST(m_spMyMath2);
    //COM_DETECT_INTERFACE_FROM_LIST(m_spMyTimer);

    CComPtr<IUnknown> spUnk;
    COM_VERIFY(spUnk.CoCreateInstance(CLSID_ComActiveXObj));
    if (spUnk)
    {
        //COM_DETECT_INTERFACE_FROM_REGISTER(spUnk);
        //COM_DETECT_INTERFACE_FROM_LIST(spUnk);
    }

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CComCallerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CComCallerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CComCallerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CComCallerDlg::OnClose()
{
    CDialog::OnClose();
}

void CComCallerDlg::OnBnClickedBtnViewStorageFile()
{
    //::CoInitialize(NULL);	// COM 初始化
    // 如果是MFC程序，可以使用AfxOleInit()替代


    CStorageFileUtil storage;
    storage.CreateDemoStorageFile(TEXT("c:\\a.stg"));
    storage.DumpStorageFileInfo(TEXT("c:\\a.stg"));

    //CoUninitialize();
}

void CComCallerDlg::OnBnClickedBtnDrawPic()
{
    ::CoInitialize(NULL);  // COM 初始化
    HRESULT hr;
    CFile file;
    CWnd* pBtnDraw = GetDlgItem(IDC_BTN_DRAW_PIC);
    _ASSERT(pBtnDraw);

    CDC* pDC = pBtnDraw->GetDC();

    file.Open( TEXT(".\\res\\ComCaller.ico"), CFile::modeRead | CFile::shareDenyNone );  // 读入文件内容
    DWORD dwSize = (DWORD)file.GetLength();
    HGLOBAL hMem = ::GlobalAlloc( GMEM_MOVEABLE, dwSize );
    LPVOID lpBuf = ::GlobalLock( hMem );
    file.Read( lpBuf, dwSize );
    file.Close();
    ::GlobalUnlock( hMem );

    IStream * pStream = NULL;
    IPicture * pPicture = NULL;

    // 由 HGLOBAL 得到 IStream，参数 TRUE 表示释放 IStream 的同时，释放内存
    hr = ::CreateStreamOnHGlobal( hMem, TRUE, &pStream );
    ASSERT ( SUCCEEDED(hr) );

    hr = ::OleLoadPicture( pStream, dwSize, TRUE, IID_IPicture, ( LPVOID * )&pPicture );
    ASSERT(hr==S_OK);

    long nWidth,nHeight;  // 宽高，MM_HIMETRIC 模式，单位是0.01毫米
    pPicture->get_Width( &nWidth );    // 宽
    pPicture->get_Height( &nHeight );  // 高

    ////////原大显示//////
    //CSize sz( nWidth, nHeight );
    //pDC->HIMETRICtoDP( &sz );  // 转换 MM_HIMETRIC 模式单位为 MM_TEXT 像素单位
    //pPicture->Render(pDC->m_hDC,0,0,sz.cx,sz.cy,
    //    0,nHeight,nWidth,-nHeight,NULL);

    ////////按按钮窗口尺寸显示////////
    CRect rect;	
    pBtnDraw->GetClientRect(&rect);
    pPicture->Render(pDC->m_hDC,0,0,rect.Width(),rect.Height(),
    	0,nHeight,nWidth,-nHeight,NULL);

    if ( pPicture ) pPicture->Release();// 释放 IPicture 指针
    if ( pStream ) pStream->Release();  // 释放 IStream 指针，同时释放了 hMem

    ::CoUninitialize();

    pBtnDraw->ReleaseDC(pDC);
}

void CComCallerDlg::OnBnClickedBtnCallServiceExeMulti()
{
    HRESULT hr = E_FAIL;
    CComPtr<IAtlServiceDemoObject> spServiceDemoObject;
    COM_VERIFY(spServiceDemoObject.CoCreateInstance(CLSID_AtlServiceDemoObject));
    LONG result = 0;
    if(spServiceDemoObject)
    {
        COM_VERIFY(spServiceDemoObject->Multi(10,20,&result));
        CString str;
        str.Format(TEXT("Service Exe::Multi -- 10 * 20 = %d"), result);
        AfxMessageBox(str);
    }
}

void CComCallerDlg::OnBnClickedBtnCallMymathAdd()
{
    HRESULT hr = E_FAIL;

    CComPtr<IMyMath> spMyMath;
    COM_VERIFY(spMyMath.CoCreateInstance(CLSID_MyMath));
    LONG result = 0;
    if (spMyMath)
    {
        COM_VERIFY(spMyMath->Add(10,20,&result));

        CString str;
        str.Format(TEXT("MyMath::Add -- 10 + 20 = %d"),result);
        AfxMessageBox(str);
    }
}

void CComCallerDlg::OnBnClickedBtnCallMymath2Mul()
{
    HRESULT hr = E_FAIL;
    CComPtr<IMyMath>    spMyMath;
    CComQIPtr<IMyMath2> spMyMath2;
    COM_VERIFY(spMyMath.CoCreateInstance(CLSID_MyMath));
    spMyMath2 = spMyMath;

    if (spMyMath && spMyMath2)
    {
        LONG resultAdd = 0;
        LONG resultMul = 0;

        COM_VERIFY(spMyMath->Add(10,20,&resultAdd));
        COM_VERIFY(spMyMath2->Mul(10,20,&resultMul));

        CString str;
        str.Format(TEXT("MyMath2::Add -- 10 + 20 = %d\r\nMyMath2::Mul -- 10 * 20 = %d"),
            resultAdd,resultMul);
        AfxMessageBox(str);

        spMyMath.Release();
        spMyMath2.Release();
    }
}

void CComCallerDlg::OnBnClickedBtnViewDispatch()
{
    CComPtr<IMyMath> spMyMath;
    spMyMath.CoCreateInstance(CLSID_MyMath);
    LONG l = 0;
    spMyMath->Add(10,20,&l);

    //HRESULT hr = E_FAIL;
    //CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY,TEXT("Executable File(*.exe;*.tlb)|*.exe;*.tlb|All Files(*.*)|*.*"));
    //if (dlg.DoModal() == IDOK)
    //{
    //}
}

//使用 import 导入类型库
#ifdef _DEBUG
    #import "..\ATLCom\Debug\ATLCom.tlb"
#else
    #import "..\ATLCom\Release\ATLCom.tlb"
#endif

void CComCallerDlg::OnBnClickedBtnImportUpper()
{
    HRESULT hr = E_FAIL;
    ATLComLib::IMyDispatchPtr spMyDispathc;
    //COM_VERIFY(spMyDispathc.CreateInstance(CLSID_MyMath));
    COM_VERIFY(spMyDispathc.CreateInstance(TEXT("ATLCom.MyMath")));

    _bstr_t  bstrInput(TEXT("hello"));
    _bstr_t  bstrOutput;
    bstrOutput = spMyDispathc->Upper(bstrInput);
    USES_CONVERSION;

    CString str;
    str.Format(TEXT("Upper('hello') = %s"),OLE2CT(bstrOutput));
    AfxMessageBox(str);
}

void CComCallerDlg::OnBnClickedBtnDispatchApi()
{
    CLSID clsid;				// 通过 ProgID 得到 CLSID
    HRESULT hr = E_FAIL;
    COM_VERIFY(::CLSIDFromProgID( L"ATLCom.MyMath", &clsid ));
    ASSERT( SUCCEEDED( hr ) );	// 如果失败，说明没有注册组件

    IDispatch * pDisp = NULL;	// 由 CLSID 启动组件，并得到 IDispatch 指针
    COM_VERIFY(::CoCreateInstance( clsid, NULL, CLSCTX_ALL, IID_IDispatch, (LPVOID *)&pDisp ));

    LPOLESTR pwFunName = L"Upper";	// 准备取得 Upper 函数的序号 DispID
    DISPID dispID;					// 取得的序号，准备保存到这里
    COM_VERIFY(pDisp->GetIDsOfNames(		// 根据函数名，取得序号的函数
        IID_NULL,
        &pwFunName,					// 函数名称的数组
        1,							// 函数名称数组中的元素个数
        LOCALE_SYSTEM_DEFAULT,		// 使用系统默认的语言环境
        &dispID ));					// 返回值
    ASSERT( SUCCEEDED( hr ) );		// 如果失败，说明组件根本就没有 ADD 函数

    VARIANTARG v[1];					// 调用 Upper("hello") 函数所需要的参数
    v[0].vt = VT_BSTR;	v[0].bstrVal = _bstr_t("hello");	// 第一个参数，BSTR("hello")

    DISPPARAMS dispParams = { v, NULL, _countof(v), 0 };	// 把参数包装在这个结构中
    VARIANT vResult;			// 函数返回的计算结果

    hr = pDisp->Invoke(			// 调用函数
        dispID,					// 函数由 dispID 指定
        IID_NULL,
        LOCALE_SYSTEM_DEFAULT,	// 使用系统默认的语言环境
        DISPATCH_METHOD,		// 调用的是方法，不是属性
        &dispParams,			// 参数
        &vResult,				// 返回值
        NULL,					// 不考虑异常处理
        NULL);					// 不考虑错误处理
    ASSERT( SUCCEEDED( hr ) );	// 如果失败，说明参数传递错误
    ASSERT(vResult.vt == VT_BSTR);

    USES_CONVERSION;
    CString str;			// 显示一下结果
    str.Format(TEXT("Upper('hello') = %s"), OLE2T(vResult.bstrVal));
    AfxMessageBox( str );

    pDisp->Release();		// 释放接口指针
    
}

void CComCallerDlg::OnBnClickedBtnUseCcomdispatchdriver()
{
    CLSID clsid;				// 通过 ProgID 取得组件的 CLSID
    HRESULT hr = ::CLSIDFromProgID( L"ATLCom.MyMath", &clsid );
    ASSERT( SUCCEEDED( hr ) );	// 如果失败，说明没有注册组件

    CComPtr < IUnknown > spUnk;	// 由 CLSID 启动组件，并取得 IUnknown 指针
    hr = ::CoCreateInstance( clsid, NULL, CLSCTX_ALL, IID_IUnknown, (LPVOID *)&spUnk );
    ASSERT( SUCCEEDED( hr ) );

    CComDispatchDriver spDisp( spUnk );	// 构造 IDispatch 的智能指针(实际就是 CComQIPtr<IDispatch> )
    CComVariant v1("hello"), vResult;	// 参数
    hr = spDisp.Invoke1(	// 调用2个参数的函数
        L"Upper",			// 函数名是 Add
        &v1,				// 第一个参数，值为"hello"
        &vResult);			// 返回值
    ASSERT( SUCCEEDED( hr ) );	// 如果失败，说明或者没有 ADD 函数，或者参数错误
    ASSERT(vResult.vt == VT_BSTR);
    
    spDisp.Release();

    USES_CONVERSION;
    CString str;			// 显示一下结果
    str.Format(TEXT("Upper('hello') = %s"), OLE2T(vResult.bstrVal));
    AfxMessageBox( str );

}

void CComCallerDlg::OnBnClickedBtnErrorHandle()
{
    HRESULT hr = E_FAIL;
    CComPtr<IMyMath2>   spMyMath2;
    COM_VERIFY(spMyMath2.CoCreateInstance(CLSID_MyMath));
    COM_VERIFY(spMyMath2->Mul(10,20,NULL));
    if (FAILED(hr))
    {
        CComQIPtr < ISupportErrorInfo > spSEI = spMyMath2;	// 组件是否提供了 ISupportErrorInfo 接口？
        if( spSEI )	// 如果支持，那么
        {
            COM_VERIFY(spSEI->InterfaceSupportsErrorInfo( IID_IMyMath2 ));	// 是否支持 Ixxx 接口的错误处理？
            if( SUCCEEDED( hr ) )
            {	// 支持，太好了。取出错误信息
                CComQIPtr < IErrorInfo > spErrInfo;		// 声明 IErrorInfo 接口
                hr = ::GetErrorInfo( 0, &spErrInfo );	// 取得接口
                if( SUCCEEDED( hr ) )
                {
                    CComBSTR bstrDes;
                    spErrInfo->GetDescription( &bstrDes );	// 取得错误描述
                    AfxMessageBox(_bstr_t(bstrDes));
                    //......	// 还可以取得其它的信息
                    
                }
            }
        }
    }
}


void CComCallerDlg::OnBnClickedBtnSubAndCallback()
{
    //由于需要通过回调的方式返回，最好不要使用局部变量的方式创建COM对象，否则可能因为生存期出问题
    HRESULT hr = E_FAIL;
    SetWindowText(TEXT(""));
    COM_VERIFY(m_spMyMath2->Sub(20,10));
}


//连接点的示例程序
void CComCallerDlg::OnBnClickedBtnTimerConnectpoint()
{
    HRESULT hr = E_FAIL;
    
    CComQIPtr<IConnectionPointContainer> spContainer( m_spMyTimer );
    ASSERT( spContainer );		// 组件没有提供连接点功能

    if (!m_spCPTimer)
    {
        COM_VERIFY(spContainer->FindConnectionPoint(__uuidof(_IMyTimerEvents), &m_spCPTimer ));
        ASSERT( m_spCPTimer );			// 没有找到连接点接口

        COM_VERIFY(m_spCPTimer->Advise( &m_TimerSink, &m_dwCPCookie ));
        ASSERT( SUCCEEDED( hr ) );	// 连接失败
        m_TimerSink.SetResultWnd( this );
        COM_VERIFY(m_spMyTimer->SetTimer(1000));
    }
    else
    {
        COM_VERIFY(m_spMyTimer->KillTimer());
        COM_VERIFY(m_spCPTimer->Unadvise(m_dwCPCookie));
        m_spCPTimer.Release();
    }

}

void CComCallerDlg::OnBnClickedBtnPersist()
{
    //如果要保存、恢复的话，使用 CStorageFileUtil 的方法，创建 IStorage，并在其中的 IStream 中读写持久化信息
    HRESULT hr = E_FAIL;
    LONG lVal = 0;
    COM_VERIFY(m_spMyMath2->GetNext(&lVal));
    CComQIPtr<IPersistStreamInit>    spPSI = m_spMyMath2;
    ASSERT(spPSI);
    //spPSI->Save(..);
    //spPSI->Load(...);

    SetDlgItemInt(IDC_BTN_PERSIST,lVal);
}

DWORD ApartMentMsgWaitForMultipleObject(HANDLE *hHandle,DWORD dwWaitCout, DWORD dwMilliseconds)
{
    BOOL bQuit = FALSE;
    DWORD dwRet;

    while(!bQuit)
    {
        int rc;
        rc = ::MsgWaitForMultipleObjects(dwWaitCout,//需要等待的对象数量
            hHandle,// 对象树组
            FALSE, //等待所有的对象
            (DWORD)dwMilliseconds,//等待的时间
            (DWORD)(QS_ALLINPUT | QS_ALLPOSTMESSAGE)//事件类型
            );
        //等待的事件激发
        if( rc ==  WAIT_OBJECT_0 )
        {
            dwRet = rc;
            bQuit = TRUE;
        }
        //其他windows消息
        else if( rc == WAIT_OBJECT_0 + dwWaitCout )			
        {
            MSG msg;
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage (&msg);
                DispatchMessage(&msg);
            }			
        } 
    }
    return dwRet;
}

DWORD WINAPI MyThreadProc( LPVOID lpThreadParameter )
{
    /*******************************************************************************************************
    * 测试结论
    *   参见 ComThreadingModel.h 中的“最佳实例”
    *   
    * 测试步骤 -- 注意：ThreadingModel 包括了 CComXXXThreadModel的设置
    *   一、ThreadingModel为 Apartment(CComSingleThreadModel)
    *     1.主线程和子线程都用 COINIT_APARTMENTTHREADED -- 即 CoInitialize(NULL);
    *       a.不定义 DEMO_COM_MARSHAL 宏时(直接传递) -- 函数重入，    地址相同，  不同线程 <== 出错，违反了STA的模型
    *       b.定义 DEMO_COM_MARSHAL 宏(进行Marshal)  -- 函数互斥执行，地址不相同，同一线程
    *     2.主线程和子线程都用 COINIT_MULTITHREADED
    *       a.不定义 DEMO_COM_MARSHAL 宏时(直接传递) -- 函数互斥执行，地址相同，  同一线程 <== 此时两个线程在同一MTA中，Marshal只是简单传递指针
    *       b.定义 DEMO_COM_MARSHAL 宏(进行Marshal)  -- 函数互斥执行，地址相同，  同一线程
    *     3.主线程用 COINIT_APARTMENTTHREADED(STA) ，子线程用 COINIT_MULTITHREADED(MTA)
    *       a.不定义 DEMO_COM_MARSHAL 宏时(直接传递) -- 函数重入，    地址相同，  不同线程
    *       b.定义 DEMO_COM_MARSHAL 宏(进行Marshal)  -- 函数互斥执行，地址不相同，同一线程
    *     4.主线程用 COINIT_MULTITHREADED(MTA)，子线程用 COINIT_APARTMENTTHREADED(STA)
    *       a.不定义 DEMO_COM_MARSHAL 宏时(直接传递) -- 地址相同，子线程调用时出错，RPC_E_WRONG_THREAD(0x8001010E)
    *           <== 即STA线程不能调用其他线程(MTA)创建的对象
    *       b.定义 DEMO_COM_MARSHAL 宏(进行Marshal)  -- 函数互斥执行，地址不相同，同一线程
    *   二、ThreadingModel为 Free(CComMultiThreadModel)
    *     1.主线程和子线程都用 COINIT_APARTMENTTHREADED -- 即 CoInitialize(NULL);
    *       a.不定义 DEMO_COM_MARSHAL 宏时(直接传递) -- 地址相同，子线程调用时出错，RPC_E_WRONG_THREAD(0x8001010E)
    *       b.定义 DEMO_COM_MARSHAL 宏(进行Marshal)  -- 函数重入，    地址不相同，不同线程
    *     2.主线程和子线程都用 COINIT_MULTITHREADED
    *       a.不定义 DEMO_COM_MARSHAL 宏时(直接传递) -- 函数重入，    地址相同，  不同线程
    *       b.定义 DEMO_COM_MARSHAL 宏(进行Marshal)  -- 函数重入，    地址相同，  不同线程
    *     3.主线程用 COINIT_APARTMENTTHREADED(STA) ，子线程用 COINIT_MULTITHREADED(MTA)
    *       a.不定义 DEMO_COM_MARSHAL 宏时(直接传递) -- 地址相同，子线程调用时出错，RPC_E_WRONG_THREAD(0x8001010E)
    *       b.定义 DEMO_COM_MARSHAL 宏(进行Marshal)  -- 函数重入，    地址不相同，不同线程
    *     4.主线程用 COINIT_MULTITHREADED(MTA)，子线程用 COINIT_APARTMENTTHREADED(STA)
    *       a.不定义 DEMO_COM_MARSHAL 宏时(直接传递) -- 函数重入，    地址相同，  不同线程
    *       b.定义 DEMO_COM_MARSHAL 宏(进行Marshal)  -- 函数重入，    地址不相同，不同线程
    *   四、ThreadingModel为 Both(CComMultiThreadModel) -- 
    *     0.主线程使用 COINIT_APARTMENTTHREADED 时，若不定义 USE_FREE_THREADED_MARSHALER 宏启用 
    *          Free-Threaded Marshaler 支持，会出错(Access violation reading location) -- 问题：Vista下测试Free时出错
    *     1.主线程和子线程都用 COINIT_APARTMENTTHREADED -- 即 CoInitialize(NULL);
    *       a.不定义 DEMO_COM_MARSHAL 宏时(直接传递) -- 
    *       b.定义 DEMO_COM_MARSHAL 宏(进行Marshal)  -- 
    *     2.主线程和子线程都用 COINIT_MULTITHREADED
    *       a.不定义 DEMO_COM_MARSHAL 宏时(直接传递) -- 
    *       b.定义 DEMO_COM_MARSHAL 宏(进行Marshal)  -- 
    *     3.主线程用 COINIT_APARTMENTTHREADED(STA) ，子线程用 COINIT_MULTITHREADED(MTA)
    *       a.不定义 DEMO_COM_MARSHAL 宏时(直接传递) -- 
    *       b.定义 DEMO_COM_MARSHAL 宏(进行Marshal)  -- 
    *     4.主线程用 COINIT_MULTITHREADED(MTA)，子线程用 COINIT_APARTMENTTHREADED(STA)
    *       a.不定义 DEMO_COM_MARSHAL 宏时(直接传递) -- 
    *       b.定义 DEMO_COM_MARSHAL 宏(进行Marshal)  -- 
    *   三、ThreadingModel为 Neutral(CComMultiThreadModel) -- 注意：测试时没有选择 COM+ 支持
    *     1.主线程和子线程都用 COINIT_APARTMENTTHREADED -- 即 CoInitialize(NULL);
    *       a.不定义 DEMO_COM_MARSHAL 宏时(直接传递) -- 地址相同，子线程调用时出错，RPC_E_WRONG_THREAD(0x8001010E)
    *       b.定义 DEMO_COM_MARSHAL 宏(进行Marshal)  -- 函数重入，    地址相同，  不同线程
    *     2.主线程和子线程都用 COINIT_MULTITHREADED
    *       a.不定义 DEMO_COM_MARSHAL 宏时(直接传递) -- 函数重入，    地址相同，  不同线程
    *       b.定义 DEMO_COM_MARSHAL 宏(进行Marshal)  -- 函数重入，    地址相同，  不同线程
    *     3.主线程用 COINIT_APARTMENTTHREADED(STA) ，子线程用 COINIT_MULTITHREADED(MTA)
    *       a.不定义 DEMO_COM_MARSHAL 宏时(直接传递) -- 地址相同，子线程调用时出错，RPC_E_WRONG_THREAD(0x8001010E)
    *       b.定义 DEMO_COM_MARSHAL 宏(进行Marshal)  -- 函数重入，    地址相同，  不同线程
    *     4.主线程用 COINIT_MULTITHREADED(MTA)，子线程用 COINIT_APARTMENTTHREADED(STA)
    *       a.不定义 DEMO_COM_MARSHAL 宏时(直接传递) -- 地址相同，子线程调用时出错，RPC_E_WRONG_THREAD(0x8001010E)
    *       b.定义 DEMO_COM_MARSHAL 宏(进行Marshal)  -- 函数重入，    地址相同，  不同线程
    *******************************************************************************************************/
    HRESULT hr = E_FAIL;
    COM_VERIFY(CoInitializeEx(NULL,COINIT_MULTITHREADED));
    {
        Sleep(1000);

        CComPtr<IMyMath2> spMyMath2;
#ifdef DEMO_COM_MARSHAL
        IStream* pStream = (IStream*)lpThreadParameter;
        COM_VERIFY(CoGetInterfaceAndReleaseStream(pStream,IID_IMyMath2,(void**)&spMyMath2));
#else
        spMyMath2 = (IMyMath2*)lpThreadParameter;
#endif
        ATLTRACE(TEXT("Sub Thread,ThreadID=%d,CoCurProcess=%d,spMyMath = 0x%08x\n"),
            GetCurrentThreadId(), CoGetCurrentProcess(),spMyMath2);

        LONG lResult = 0;
        COM_VERIFY(spMyMath2->DemoMarshalLongOperation(CComBSTR(TEXT("Sub Thread"))));
    }
    CoUninitialize();
    return 0;
}

void CComCallerDlg::OnBnClickedBtnDemoMarshal()
{
    //显示对接口进行Marshal的操作 -- 默认情况为测试 STA
    //  如果不是使用Marshal，而是直接传递指针，则两个线程中调用 DemoMarshalLongOperation 会重入
    HRESULT hr = E_FAIL;
    LONG lResult = 0;

    //COM_VERIFY(CreateStreamOnHGlobal(NULL,TRUE,&spStream));
    //SHCreateMemStream

    ATLTRACE(TEXT("Main Thread,ThreadID=%d,CoCurProcess=%d,spMyMath2 = 0x%08x\n"),
        GetCurrentThreadId(), CoGetCurrentProcess(),m_spMyMath2);
#ifdef DEMO_COM_MARSHAL
    IStream* pStream = NULL;        //由于CoGetInterfaceAndReleaseStream会释放，因此这里不需要再释放了 
    COM_VERIFY(CoMarshalInterThreadInterfaceInStream(IID_IMyMath2,m_spMyMath2,&pStream));
    HANDLE hThread = CreateThread(NULL,0,MyThreadProc,pStream,0,NULL);
#else
    HANDLE hThread = CreateThread(NULL,0,MyThreadProc,m_spMyMath2,0,NULL);
#endif

    COM_VERIFY(m_spMyMath2->DemoMarshalLongOperation(CComBSTR(TEXT("Main Thread"))));
    //WaitForSingleObject(hThread,INFINITE);  //由于Wait后会使得消息循环无法继续，因此会死锁？用 ProcessExplorer 查看
    //ApartMentMsgWaitForMultipleObject(&hThread,1,INFINITE);  
    CloseHandle(hThread);
}

void CComCallerDlg::OnDestroy()
{
    // TODO: Add your message handler code here and/or call default
    HRESULT hr = E_FAIL;
    if (0 != m_dwCPCookie && m_spCPTimer)
    {
        m_spCPTimer->Unadvise(m_dwCPCookie);
        m_spCPTimer.Release();
        m_dwCPCookie = 0;
    }

    if (0 != m_dwCallbackCookie && m_spMyMath2)
    {
        COM_VERIFY(m_spMyMath2->MyUnAdvise(m_dwCallbackCookie));
        m_spMyMath2.Release();
    }
    m_spMyTimer.Release();

    __super::OnDestroy();

    // TODO: Add your message handler code here
}

void CComCallerDlg::OnBnClickedBtnSumArray()
{
    LONG lArray[] = {1,2,3,4,5,6,7,8,9,10};
    LONG lResult = 0;
    HRESULT hr = E_FAIL;
    COM_VERIFY(m_spMyMath2->Sum(_countof(lArray),lArray,&lResult));
    SetDlgItemInt(IDC_BTN_SUM_ARRAY,lResult,TRUE);
}

void CComCallerDlg::OnBnClickedBtnGetComponentInfo()
{
    HRESULT hr = E_FAIL;
    CComQIPtr<IComponentInfo>  spInfo = m_spMyMath2;
    if (spInfo)
    {
        COMPONENT_INFO* pInfo = NULL;
        COM_VERIFY(spInfo->GetComponentInfo(&pInfo));
        if (SUCCEEDED(hr))
        {
            USES_CONVERSION;
            CString strInfo;
            strInfo.Format(TEXT("Author = %s\n Major=%d \n Minor = %d"),
                OLE2CT(pInfo->Author),pInfo->Major,pInfo->Minor);
            SysFreeString(pInfo->Author);
            CoTaskMemFree(pInfo);
            AfxMessageBox(strInfo);
        }
    }
}

void CComCallerDlg::OnBnClickedBtnDetectInterface()
{
    HRESULT hr = E_FAIL;
    IUnknown *pTestUnk = NULL;
    hr = ::CoCreateInstance(CLSID_MyMath, NULL, CLSCTX_INPROC_SERVER,
        IID_IUnknown, (void**)&pTestUnk);
    if (pTestUnk)
    {
        TRACE(TEXT("Before COM_DETECT_INTERFACE_FROM_REGISTER\n"));
        
        COM_DETECT_INTERFACE_FROM_REGISTER(pTestUnk);

        TRACE(TEXT("Before COM_DETECT_INTERFACE_FROM_LIST\n"));
        COM_DETECT_INTERFACE_FROM_LIST(pTestUnk);

        //CComQIPtr<IWebBrowserApp> spWebApp = pTestUnk;
        ////IWebBrowserApp* spWebApp;
        //COM_VERIFY(spWebApp->put_Visible(VARIANT_TRUE));
        //CComVariant varFlags(navOpenInNewWindow);
        //CComVariant varTargetFrameName(_BLANK);
        //CComVariant varPostData()

        //有错
        //COM_VERIFY(spWebApp->Navigate(CComBSTR("http://www.baidu.com"),&varFlags,&varTargetFrameName,NULL,NULL));
        pTestUnk->Release();
    }

    CComPtr<IUnknown> InternetExplorer;
    COM_VERIFY(InternetExplorer.CoCreateInstance(CLSID_InternetExplorer));
    COM_DETECT_INTERFACE_FROM_REGISTER(InternetExplorer);
    COM_DETECT_INTERFACE_FROM_LIST(InternetExplorer);

}

void CComCallerDlg::OnBnClickedBtnMoniker()
{
    //根据之前保存的COM组件的信息文件加载COM组件，目前上有问题：怎么保存可供使用的信息文件？
    HRESULT hr = E_FAIL;
    CFileDialog dlg(TRUE);
    if (dlg.DoModal() == IDOK)
    {
        USES_CONVERSION;
        CComPtr<IMoniker>   spMoniker;
        CComPtr<IMyMath2>   spMyMath2;
        //IMoniker*   spMoniker = NULL;
        //IMyMath2*     spMyMath2 = NULL;
        COM_VERIFY(CreateFileMoniker(T2COLE(dlg.GetFileName()),&spMoniker));
        if (SUCCEEDED(hr))
        {
            
            COM_VERIFY(BindMoniker(spMoniker, 0, IID_IMyMath2, (void **)&spMyMath2));
            if(SUCCEEDED(hr)) 
            {
                LONG lResult = 0;
                COM_VERIFY(spMyMath2->Add(1,2,&lResult));
                Sleep(2000);
            }
        }
    }
    

}

//WIA(Windows Image Acquisition) -- 既是应用程序接口(API)也是设备驱动程序接口(DDI)
void CComCallerDlg::OnBnClickedBtnWia()
{
    
}

void CComCallerDlg::OnBnClickedBtnLengthyComOperation()
{
    //http://support.microsoft.com/kb/248019/en-us
    //如果MFC客户端调用COMServer中的冗长操作的话，会出现“OLE Server Busy”对话框，通过以下代码可以防止出现
    //问题：始终没有出现"OLE Server Busy"对话框？

	COleMessageFilter* pFilter = AfxOleGetMessageFilter();
    AfxOleGetMessageFilter()->SetMessagePendingDelay(1000); //set the wait period on outgoing COM calls
    //AfxOleGetMessageFilter()->EnableNotRespondingDialog(FALSE); //disable the Not Responding dialog box
    //AfxOleGetMessageFilter()->EnableBusyDialog(FALSE);//disable the busy dialog box from appearing after the COM call times out
    
    m_spMyMath2->LengthyComOperation(5000);

}

[event_source(native)]
class CMyEventSource {
public:
    __event void OnValueChanged(int nNewValue);
};

[event_receiver(native)]
class CMyEventReceiver
{
public:
    void OnValueChangedHandler(int nValue) {
        TRACE(TEXT("[%d] In CMyEventReceiver1::OnValueChangedHandler, this=%p\n"), 
            GetCurrentThreadId(),this);
    }

    void HookEvent(CMyEventSource* pSource) {
        BOOL bRet = FALSE;
        __hook(&CMyEventSource::OnValueChanged, pSource, &CMyEventReceiver::OnValueChangedHandler, this);
    }
    void UnHookEvent(CMyEventSource* pSource) {
        __unhook(&CMyEventSource::OnValueChanged, pSource, &CMyEventReceiver::OnValueChangedHandler);
    }
};


void CComCallerDlg::OnBnClickedBtnEventHook()
{
    CMyEventSource source;
    CMyEventReceiver receiver1;
    CMyEventReceiver receiver2;

    receiver1.HookEvent(&source);
    receiver2.HookEvent(&source);
    TRACE(TEXT("[%d] Before raise OnValueChanged\n"), GetCurrentThreadId());
    __raise source.OnValueChanged(123);
    TRACE(TEXT("[%d] After raise OnValueChanged\n"), GetCurrentThreadId());

    receiver2.UnHookEvent(&source);
    receiver1.UnHookEvent(&source);

}
