#include "StdAfx.h"
#include "LotteryMgr.h"

CLotteryMgr::CLotteryMgr()
{
    m_szThumbnail.SetSize(100, 100);
    m_hWndControl = NULL;
    m_dwElapseTime = 200;
    m_bRecursive = FALSE;
}

CLotteryMgr::~CLotteryMgr(void)
{
    m_threadInit.StopAndWait();
    m_threadLoop.StopAndWait();
}

FTL::FileFindResultHandle CLotteryMgr::OnFindFile(LPCTSTR pszFilePath, const WIN32_FIND_DATA& findData, LPVOID pParam)
{
    HRESULT hr = E_FAIL;
    BOOL    bRet = FALSE;
    Gdiplus::Status sts = Gdiplus::Ok;

    if (ftwtContinue != m_threadInit.GetThreadWaitType(INFINITE))
    {
         return rhStop;
    }

    if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
    {
        //find result file
        FTLTRACE(TEXT("OnFindFile: %s\n"), pszFilePath);

        CImage img;
        COM_VERIFY(img.Load(pszFilePath));
        if (SUCCEEDED(hr))
        {
            LotteryInfoPtr pLotteryInfo(new LotteryInfo(pszFilePath, FALSE));
            API_VERIFY(pLotteryInfo->imgThumbnail.Create(m_szThumbnail.cx, m_szThumbnail.cy, 32, CImage::createAlphaChannel));
            if (bRet)
            {
                CImageDC dc(pLotteryInfo->imgThumbnail);
                Gdiplus::Bitmap bmp((HBITMAP)img, NULL);
                Gdiplus::Graphics graphic(dc);
                GDIPLUS_VERIFY(graphic.DrawImage(&bmp, 0, 0, m_szThumbnail.cx, m_szThumbnail.cy));

                SendMessage(m_hWndControl, UM_ADD_LOTTERY_INFO, (WPARAM)pLotteryInfo.get(), NULL);

                m_allLotteryInfos.push_back(pLotteryInfo);
            }
        }
    }
    
    return rhContinue;
}

BOOL CLotteryMgr::Init(HWND hWndControl, const CSize& szThumbnail, LPCTSTR pszDirectory, 
                       LPCTSTR pszExtName /* = TEXT(*.jpg;*.png) */,
                       BOOL bRecursive /* = FALSE*/)
{
    BOOL bRet = FALSE;
    API_VERIFY(m_threadInit.StopAndWait());

    m_szThumbnail = szThumbnail;
    m_hWndControl = hWndControl;
    m_strDirectory = pszDirectory;
    m_strExtName = pszExtName;
    m_bRecursive = bRecursive;

    API_VERIFY(m_threadInit.Start(_InitFunc, this));
    return bRet;
}

DWORD CLotteryMgr::_InitFunc(LPVOID lpThreadParameter)
{
    CLotteryMgr* pThis = reinterpret_cast<CLotteryMgr*>(lpThreadParameter);
    return pThis->_innerInitFunc();
}
DWORD CLotteryMgr::_innerInitFunc()
{
    m_allLotteryInfos.clear();

    BOOL bRet = FALSE;
    CFFileFinder finder;
    finder.SetCallback(this, NULL);
    API_VERIFY(finder.Find(m_strDirectory, m_strExtName, m_bRecursive));
    DWORD dwCount = m_allLotteryInfos.size();
    SendMessage(m_hWndControl, UM_INIT_LOTTERY_COMPLETE, dwCount, NULL);
    return dwCount;
}

BOOL CLotteryMgr::StopInit()
{
    BOOL bRet = FALSE;
    API_VERIFY(m_threadInit.StopAndWait());
    return bRet;
}
BOOL CLotteryMgr::Start(DWORD dwElapseTime)
{
    BOOL bRet = FALSE;
    m_dwElapseTime = dwElapseTime;
    
    API_VERIFY(m_threadLoop.Start(_LotterLoopFunc, this));
    return bRet;
}

DWORD CLotteryMgr::_LotterLoopFunc(LPVOID lpThreadParameter)
{
    CLotteryMgr* pThis = reinterpret_cast<CLotteryMgr*>(lpThreadParameter);
    return pThis->_innerLotterLoopFunc();
}

DWORD CLotteryMgr::_innerLotterLoopFunc()
{
    FTLTRACE(TEXT("Enter _innerLotterLoopFunc\n"));
    DWORD dwResult = 0;
    FTLThreadWaitType waitType = ftwtContinue;
    while(ftwtContinue == (waitType = m_threadLoop.GetThreadWaitType(INFINITE)))
    {
        for (LotteryInfoContainer::iterator iter = m_allLotteryInfos.begin();
            iter != m_allLotteryInfos.end();
            ++iter)
        {
            //CFAutoLock<CFLockObject> autoLock(&m_csLock);
            m_pCurLotterInfo = *iter;
            if (!m_pCurLotterInfo->bSelection)
            {
                SendMessage(m_hWndControl, UM_UPDATE_LOTTERY_INFO, (WPARAM)m_pCurLotterInfo.get(), NULL);

                //wait for pause
                if (m_threadLoop.HadRequestPause())
                {
                    waitType = m_threadLoop.GetThreadWaitType(INFINITE);
                    FTLASSERT(!m_pCurLotterInfo->bSelection);
                    m_pCurLotterInfo->bSelection = TRUE;
                }
                else
                {
                    //sleep for elapse
                    waitType = m_threadLoop.SleepAndCheckStop(m_dwElapseTime);
                }

                if (waitType != ftwtTimeOut 
                    && waitType != ftwtContinue)
                {
                    break;
                }
            }
        }
    }

    FTLTRACE(TEXT("Leave _innerLotterLoopFunc\n"));
    return 0;
}

BOOL CLotteryMgr::TogglePause()
{
    BOOL bRet = FALSE;
    if (m_threadLoop.HadRequestPause())
    {
        API_VERIFY(m_threadLoop.Resume());
    }
    else
    {
        API_VERIFY(m_threadLoop.Pause());
    }
    return bRet;
}

BOOL CLotteryMgr::Stop()
{
    BOOL bRet = FALSE;
    API_VERIFY(m_threadLoop.StopAndWait());
    return bRet;
}

BOOL CLotteryMgr::ResetSelectionState()
{
    BOOL bRet = TRUE;

    for (LotteryInfoContainer::iterator iter = m_allLotteryInfos.begin();
        iter != m_allLotteryInfos.end();
        ++iter)
    {
        LotteryInfoPtr pCurLotterInfo = *iter;
        pCurLotterInfo->bSelection = FALSE;
    }
    return bRet;
}