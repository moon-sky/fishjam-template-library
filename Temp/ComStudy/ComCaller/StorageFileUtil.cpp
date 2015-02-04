#include "StdAfx.h"
#include "StorageFileUtil.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif

CStorageFileUtil::CStorageFileUtil(void)
{
}

CStorageFileUtil::~CStorageFileUtil(void)
{
}

HRESULT CStorageFileUtil::DumpStorageFileInfo(LPCTSTR pszFilePath)
{
    HRESULT hr;

    USES_CONVERSION;
    LPCOLESTR lpwFileName = T2COLE( pszFilePath );	// 转换T类型为宽字符

    CComPtr<IStorage>   spStorage;
    COM_VERIFY(::StgIsStorageFile( lpwFileName ));// 是复合文件吗？
    if( FAILED(hr) )
    {
        return hr;
    }

    // 打开复合文件
    COM_VERIFY(::StgOpenStorage(lpwFileName,NULL,STGM_READ | STGM_SHARE_DENY_WRITE,0,0,&spStorage));

    CComPtr<IEnumSTATSTG> spEnum;
    COM_VERIFY(spStorage->EnumElements( 0, NULL, 0, &spEnum ));

    STATSTG statstg;
    while( NOERROR == spEnum->Next( 1, &statstg, NULL) )
    {
        // statstg.type 保存着对象类型 STGTY_STREAM 或 STGTY_STORAGE
        // statstg.pwcsName 保存着对象名称
        // ...... 还有时间，长度等很多信息。请查看 MSDN
        TRACE(TEXT("type = %d, Name = %S\n"),statstg.type,statstg.pwcsName);

        ::CoTaskMemFree( statstg.pwcsName );	// 释放名称所使用的内存（注6）
    }
    
    spEnum.Release();
    spStorage.Release();
    
    return hr;
}

HRESULT CStorageFileUtil::CreateDemoStorageFile(LPCTSTR pszFilePath)
{
    HRESULT hr;		// 函数执行返回值
    IStorage *pStg = NULL;	// 根存储接口指针
    IStorage *pSub = NULL;	// 子存储接口指针
    IStream *pStm = NULL;	// 流接口指针

    USES_CONVERSION;
    LPCOLESTR lpwFileName = T2COLE( pszFilePath );	// 转换T类型为宽字符

    hr = ::StgCreateDocfile(	// 建立复合文件
        lpwFileName,	// 文件名称
        STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE,	// 打开方式
        0,		// 保留参数
        &pStg);		// 取得根存储接口指针
    ASSERT( SUCCEEDED(hr) );	// 为了突出重点，简化程序结构，所以使用了断言。
    // 在实际的程序中则要使用条件判断和异常处理

    hr = pStg->CreateStorage(	// 建立子存储
        L"SubStg",	// 子存储名称
        STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE,
        0,0,
        &pSub);		// 取得子存储接口指针
    ASSERT( SUCCEEDED(hr) );

    hr = pSub->CreateStream(	// 建立流
        L"Stm",		// 流名称
        STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE,
        0,0,
        &pStm);		// 取得流接口指针
    ASSERT( SUCCEEDED(hr) );

    hr = pStm->Write(		// 向流中写入数据
        "Hello",		// 数据地址
        5,		// 字节长度(注意，没有写入字符串结尾的\0)
        NULL);		// 不需要得到实际写入的字节长度
    ASSERT( SUCCEEDED(hr) );

    if( pStm )	pStm->Release();// 释放流指针
    if( pSub )	pSub->Release();// 释放子存储指针
    if( pStg )	pStg->Release();// 释放根存储指针
    
    return hr;
}