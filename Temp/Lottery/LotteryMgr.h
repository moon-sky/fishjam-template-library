#pragma once


#define UM_ADD_LOTTERY_INFO         (WM_USER + 1)
#define UM_INIT_LOTTERY_COMPLETE    (WM_USER + 2)
#define UM_UPDATE_LOTTERY_INFO      (WM_USER + 3)

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
    BOOL Init(HWND hWndControl, const CSize& szThumbnail, LPCTSTR pszDirectory, LPCTSTR pszExtName = TEXT("*.JPG;*.png"), 
        BOOL bRecursive = FALSE);
    BOOL StopInit();

    BOOL Start(DWORD dwElapseTime);
    BOOL TogglePause();
    BOOL Stop();
    BOOL ResetSelectionState();
    BOOL IsPaused() { return m_threadLoop.HadRequestPause(); }
private:
    typedef std::list<LotteryInfoPtr>     LotteryInfoContainer;
    LotteryInfoContainer                    m_allLotteryInfos;
    FTL::CFCriticalSection                  m_csLock;
private:
    virtual FTL::FileFindResultHandle OnFindFile(LPCTSTR pszFilePath, const WIN32_FIND_DATA& findData, LPVOID pParam);

    CSize   m_szThumbnail;
    HWND    m_hWndControl;
    CAtlString m_strDirectory;
    CAtlString m_strExtName;
    BOOL        m_bRecursive;
    DWORD   m_dwElapseTime;

    LotteryInfoPtr  m_pCurLotterInfo;


    FTL::CFThread<FTL::DefaultThreadTraits> m_threadInit;
    static DWORD __stdcall _InitFunc(LPVOID lpThreadParameter);
    DWORD _innerInitFunc();

    FTL::CFThread<FTL::DefaultThreadTraits> m_threadLoop;
    static DWORD __stdcall _LotterLoopFunc(LPVOID lpThreadParameter);
    DWORD _innerLotterLoopFunc();
};
