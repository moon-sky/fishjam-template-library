#pragma once


#define UM_ADD_LOTTERY_INFO         (WM_USER + 1)
#define UM_UPDATE_LOTTERY_INFO      (WM_USER + 2)

struct LotteryInfo
{
    CAtlString strPicturePath;
    CImage     imgThumbnail;

    BOOL bSelection;
    LotteryInfo(const CAtlString& strPath, BOOL bSel)
    {
        this->strPicturePath = strPath;
        bSelection = bSel;
    }
    ~LotteryInfo()
    {
        if (!imgThumbnail.IsNull())
        {
            imgThumbnail.Destroy();
        }
    }
};
typedef CFSharePtr<LotteryInfo> LotteryInfoPtr;

class CLotteryMgr : public IFileFindCallback
{
public:
    CLotteryMgr(void);
    ~CLotteryMgr(void);

public:
    DWORD Init(HWND hWndControl, const CSize& szThumbnail, LPCTSTR pszDirectory, LPCTSTR pszExtName = TEXT("*.JPG;*.png"));
    BOOL Start(DWORD dwElapseTime);
    BOOL TogglePause();
    BOOL Stop();
    BOOL ResetSelectionState();
    BOOL IsPaused() { return m_thread.HadRequestPause(); }
private:
    typedef std::list<LotteryInfoPtr>     LotteryInfoContainer;
    LotteryInfoContainer                    m_allLotteryInfos;
    FTL::CFCriticalSection                  m_csLock;
private:
    virtual FTL::FileFindResultHandle OnFindFile(LPCTSTR pszFilePath, const WIN32_FIND_DATA& findData, LPVOID pParam);

    CSize   m_szThumbnail;
    HWND    m_hWndControl;
    DWORD   m_dwElapseTime;

    LotteryInfoPtr  m_pCurLotterInfo;

    FTL::CFThread<FTL::DefaultThreadTraits> m_thread;
    static DWORD __stdcall _LotterLoopFunc(LPVOID lpThreadParameter);
    DWORD _innerLotterLoopFunc();
};
