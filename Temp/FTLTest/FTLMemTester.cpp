#include "StdAfx.h"
#include "FTLMemTester.h"
#include "ftlMem.h"
#include <atlfile.h>
#include <atlimage.h>
#include <GdiPlus.h>
using namespace Gdiplus;

class CMyMemLeak : public FTL::CFMemCheckBase
{
public:
    CMyMemLeak()
    {
        m_SomeValue = 10;
    }
private:
    int m_SomeValue;
    
};

void CFTLMemTester::_AllocLargeMeomories(int nBLockSize, int nBlockCount, UINT nFlags)
{
    BOOL bRet = FALSE;
    for (int i = 0; i < nBlockCount; i++)
    {
        HGLOBAL hGlobal = GlobalAlloc(nFlags, nBLockSize);
        API_VERIFY(hGlobal != NULL);
        GLOBAL_FLAGS_CHECK(hGlobal, 0, 0);
        CPPUNIT_ASSERT(nBLockSize == GlobalSize(hGlobal));
        m_AllocMemories.push_back(hGlobal);
    }
}

void CFTLMemTester::_ClearAllocMeomories()
{
    BOOL bRet = FALSE;
    int nCount = m_AllocMemories.size();
    for (int i = 0; i < nCount; i++)
    {
        HGLOBAL hGlobal = m_AllocMemories[i];
        GLOBAL_FLAGS_CHECK(hGlobal, 0, 0);
        API_VERIFY(NULL == GlobalFree(hGlobal));
    }
}


void CFTLMemTester::test_MemLeakBase()
{
    CMyMemLeak* pLeak = new CMyMemLeak();

    //此处还没有释放，因此会检测到内存泄漏
    //CHECK_OBJ_MEM_LEAK(CMyMemLeak);
    
    SAFE_DELETE(pLeak);
}

void CFTLMemTester::test_GlobalFunctions()
{
    const INT nBufferSize = 1024;
    BOOL bRet = FALSE;
    HGLOBAL hGlobalMoveable = NULL;
    HGLOBAL hGlobalFix = NULL;

    {
        //测试 GMEM_FIXED 类型的内存
        hGlobalFix = GlobalAlloc(GMEM_FIXED| GMEM_ZEROINIT, nBufferSize);
        GLOBAL_FLAGS_CHECK(hGlobalFix, 0, 0);

        LPBYTE pBufFix = (LPBYTE)GlobalLock(hGlobalFix);
        HGLOBAL hGlobalGet = GlobalHandle(pBufFix);

        CPPUNIT_ASSERT((LPBYTE)hGlobalFix == pBufFix);      //分配 Fix 类型的内存时，返回的句柄就是指针
        CPPUNIT_ASSERT((LPBYTE)hGlobalGet == pBufFix);

        StringCchCopy((LPTSTR) pBufFix, nBufferSize / sizeof(TCHAR), TEXT("FTLMemTester"));

        GLOBAL_FLAGS_CHECK(hGlobalFix, 0, 0);   //Fix 的不会增加Lock数,因此是否调用 Lock/Unlock 都无所谓，不过为了避免更换参数后的Bug，建议还是调用后使用

        API_VERIFY(GlobalUnlock(hGlobalFix));   //多次调用也不会造成错误，不过建议保持一致
        //API_VERIFY(GlobalUnlock(hGlobalFix));

        GLOBAL_FLAGS_CHECK(hGlobalFix, 0, 0);


        API_VERIFY(NULL == GlobalFree(hGlobalFix)); 
#if 0
        //对于 Fix 的HGLOBAL，释放后不能再次使用，否则会出现内存访问错误
        UINT nFlags = GlobalFlags(hGlobalFix);      
        CPPUNIT_ASSERT(GMEM_INVALID_HANDLE == nFlags);
        nFlags = GlobalFlags(hGlobalGet);           //指向同一个内存块，因此也无效
        CPPUNIT_ASSERT(GMEM_INVALID_HANDLE == nFlags);
#endif 

    }

    {
        //测试 GMEM_MOVEABLE 类型的内存
        hGlobalMoveable = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, nBufferSize);
        CPPUNIT_ASSERT(NULL != hGlobalMoveable);
        if (hGlobalMoveable)
        {
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 0, 0);      //刚分配，尚未有任何锁定

            LPVOID pLock1 = GlobalLock(hGlobalMoveable);
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 1, 0);
            LPVOID pLock2 = GlobalLock(hGlobalMoveable);
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 2, 0);
            LPVOID pLock3 = GlobalLock(hGlobalMoveable);
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 3, 0);
            LPVOID pLock4 = GlobalLock(hGlobalMoveable);
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 4, 0);

            //多次锁定，返回的地址是一样的
            CPPUNIT_ASSERT(pLock1 == pLock2 && pLock2 == pLock3 && pLock3 == pLock4);
            CPPUNIT_ASSERT(pLock1 != (hGlobalMoveable));    //分配 Moveable 类型的内存时，返回的是句柄
            //API_VERIFY(NULL == GlobalFree(hGlobalMoveable));    //注意：如果在这个地方进行释放，则会自动进行完全解锁,所有指针将不再可用

            //开始解锁
            API_VERIFY(GlobalUnlock(hGlobalMoveable));    //还锁定了3次, 返回值为TRUE(1)
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 3, 0);
            API_VERIFY(GlobalUnlock(hGlobalMoveable));    //还锁定了2次，返回值为TRUE(1)
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 2, 0);
            API_VERIFY(GlobalUnlock(hGlobalMoveable));    //还锁定了1次，返回值为TRUE(1)
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 1, 0);

            bRet = GlobalUnlock(hGlobalMoveable); 
            CPPUNIT_ASSERT((FALSE == bRet) && (GetLastError() == 0));  //全部解锁，返回值为FALSE, 但GetLastError为0
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 0, 0);

            bRet = GlobalUnlock(hGlobalMoveable);                      //已经全部解锁后再次解锁的话，错误码为 ERROR_NOT_LOCKED
            CPPUNIT_ASSERT((FALSE == bRet) && (GetLastError() == ERROR_NOT_LOCKED));
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 0, 0);

            API_VERIFY(NULL == GlobalFree(hGlobalMoveable));    //释放
#if 1
            //对于 Movable 的HGLOBAL，释放后可以获取其对应的 Flags
            INT nFlags = GlobalFlags(hGlobalMoveable);
            CPPUNIT_ASSERT(GMEM_INVALID_HANDLE == nFlags);
#endif 
        }
    }

    {
        const INT ALLOC_COUNT = 100;
        LPBYTE pLockAddres[ALLOC_COUNT] = {0};

        //测试 Moveable 的内存是否可以移动
        HGLOBAL hGlobalMove = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_DISCARDABLE, 100 * 1024 * 1000); //先分配100M
        for (int i = 0; i < ALLOC_COUNT; i++)
        {
            pLockAddres[i] = (LPBYTE)GlobalLock(hGlobalMove);
            GLOBAL_FLAGS_CHECK(hGlobalMove, 1, 0);
            _AllocLargeMeomories(1 * 1024 * 1000, 10, GMEM_FIXED | GMEM_ZEROINIT);
            API_VERIFY_EXCEPT1(GlobalUnlock(hGlobalMove), NO_ERROR);
            GLOBAL_FLAGS_CHECK(hGlobalMove, 0, 0);
        }

        BOOL bSameAddress = TRUE;
        LPBYTE pCheckAddress = pLockAddres[0];
        for (int i = 1; i < ALLOC_COUNT; i++)
        {
            if (pCheckAddress != pLockAddres[i])
            {
                bSameAddress = FALSE;
                CPPUNIT_ASSERT(FALSE); //怎么能到这里?
                break;
            }
        }

        _ClearAllocMeomories();

        //CPPUNIT_ASSERT(FALSE == bSameAddress);      //会移动 -- 实测: Win7 + VS2008 时，分配了1G的内存，仍然不移动

        API_VERIFY(NULL == GlobalFree(hGlobalMove));
    }
}

void CFTLMemTester::test_CreateStreamOnHGlobal()
{
    BOOL bRet = FALSE;
    HRESULT hr = E_FAIL;

    const INT LOOP_COUNT = 1;
    const INT IMAGE_WIDTH = 100;
    const INT IMAGE_HEIGHT = 100;

    for(int i = 0; i < LOOP_COUNT; i++)
    {
        CAtlFile file;
        COM_VERIFY(file.Create(TEXT("res\\SimpleTest.bmp"), GENERIC_READ, 0, OPEN_EXISTING));
        if (SUCCEEDED(hr))
        {
            ULONGLONG nFileSize = 0;
            COM_VERIFY(file.GetSize(nFileSize));

            CPPUNIT_ASSERT(30054 == nFileSize); //图片文件的大小

            HGLOBAL hBuffer = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, nFileSize);
            API_VERIFY(hBuffer != NULL);
            if (hBuffer)
            {
                GLOBAL_FLAGS_CHECK(hBuffer, 0, 0);
                BYTE* pBuffer = (BYTE*)::GlobalLock(hBuffer);
                API_VERIFY(pBuffer != NULL);
                GLOBAL_FLAGS_CHECK(hBuffer, 1, 0);

                COM_VERIFY(file.Read(pBuffer, nFileSize));
                file.Close();

                API_VERIFY(0 == GlobalUnlock(hBuffer)); //解锁后锁定次数为0

                CImage image;
                Bitmap* pGdiPlusBmp = NULL;
                {
                    CComPtr<IStream>    spStream;
                    GLOBAL_FLAGS_CHECK(hBuffer, 0, 0);
                    COM_VERIFY(CreateStreamOnHGlobal(hBuffer, TRUE, &spStream)); //设置为TRUE,表示 IStream 释放时自动释放对应内存
                    COM_VERIFY(image.Load(spStream));
                    GLOBAL_FLAGS_CHECK(hBuffer, 0, 0);
                    LPVOID pImageBits = image.GetBits();
                    CPPUNIT_ASSERT(pImageBits != pBuffer);
                    
                    //更改原图的颜色，不会更改到Image
                    LPBYTE pBuffer2 = (BYTE*)::GlobalLock(hBuffer);
                    CPPUNIT_ASSERT(pBuffer == pBuffer2);  //两次锁定后位置不变。TODO: 似乎没有这种保证?
                    GLOBAL_FLAGS_CHECK(hBuffer, 1, 0);

                    BITMAPFILEHEADER* pBmpFileHeader = (BITMAPFILEHEADER*)pBuffer;
                    LPBYTE pSrcBits = pBuffer + pBmpFileHeader->bfOffBits;
                    for (int y = 0; y < IMAGE_HEIGHT; y++)
                    {
                        for(int x = 0; x < IMAGE_WIDTH; x++){
                            *pSrcBits++ = 0xFF;
                            *pSrcBits++ = 0;
                            *pSrcBits++ = 0;
                        }
                    }

                    {
                        //TODO:如果启用这部分代码，则 GlobalUnlock 会返回FALSE.为什么?
                        pGdiPlusBmp = Bitmap::FromStream( spStream );
                        GLOBAL_FLAGS_CHECK(hBuffer, 1, 0);
                        CPPUNIT_ASSERT(NULL != pGdiPlusBmp);
                        if (pGdiPlusBmp)
                        {
                            CPPUNIT_ASSERT(pGdiPlusBmp->GetWidth() == IMAGE_WIDTH);
                            CPPUNIT_ASSERT(pGdiPlusBmp->GetHeight() == IMAGE_HEIGHT);
                        }
                        GLOBAL_FLAGS_CHECK(hBuffer, 1, 0);
                        delete pGdiPlusBmp;
                        pGdiPlusBmp = NULL;
                        GLOBAL_FLAGS_CHECK(hBuffer, 1, 0);

                        API_VERIFY(0 == GlobalUnlock(hBuffer));
                        GLOBAL_FLAGS_CHECK(hBuffer, 0, 0);

                        //spStream.Detach();
                    }
                }//出了 IStream 的范围，释放后 hBuffer 也会释放，但 Image 的内容会保留
                INT nFlags = GlobalFlags(hBuffer);
                CPPUNIT_ASSERT(GMEM_INVALID_HANDLE == nFlags);

                //GLOBAL_FLAGS_CHECK(hBuffer, 1, 0);

                //SIZE_T sizeCheck = GlobalSize(hBuffer);
                //CPPUNIT_ASSERT(sizeCheck == nFileSize);

                //API_VERIFY(0 == GlobalUnlock(hBuffer)); //完全释放
                bRet = (NULL == GlobalFree(hBuffer));  //如果 CreateStreamOnHGlobal 为TRUE，此处进行释放的话，会失败，错误原因为 ERROR_INVALID_HANDLE
                CPPUNIT_ASSERT((FALSE == bRet) && (GetLastError() == ERROR_INVALID_HANDLE));

                //SIZE_T sizeCheckAfterFree = GlobalSize(hBuffer);
                //CPPUNIT_ASSERT(sizeCheckAfterFree == 0);

                CPPUNIT_ASSERT(image.GetWidth() == IMAGE_WIDTH);
                CPPUNIT_ASSERT(image.GetHeight() == IMAGE_HEIGHT);

                //LPVOID pImageBuf = image.GetBits();

                //CPPUNIT_ASSERT(IsBadReadPtr(pBuffer, nFileSize));
                //CPPUNIT_ASSERT(FALSE == IsBadReadPtr(pImageBuf, nFileSize));

                COM_VERIFY(image.Save(TEXT("res\\saveAfterReleaseImage.bmp"), GUID_NULL));
            }
        }
    }
}