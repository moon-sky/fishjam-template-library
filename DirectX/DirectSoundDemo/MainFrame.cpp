#include "Stdafx.h"
#include "MainFrame.h"
#pragma comment(lib,"dsound.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_DESTROY()
END_MESSAGE_MAP()


CMainFrame::CMainFrame(void)
{
    m_pdSound = NULL;
	m_pdSoundPrimaryBuffer = NULL;
    m_pdSoundBuffer = NULL;
    m_pdSoundNotify = NULL;
    m_pdSoundCapture = NULL;
} 

CMainFrame::~CMainFrame(void)
{
    SAFE_RELEASE(m_pdSoundCapture);
    SAFE_RELEASE(m_pdSoundNotify);
    SAFE_RELEASE(m_pdSoundBuffer);
	SAFE_RELEASE(m_pdSoundPrimaryBuffer);
    SAFE_RELEASE(m_pdSound);
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    HRESULT hr = E_FAIL;

    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    //创建主DirectSound对象 -- 如有多张声卡，需要使用枚举的方式先找到合适的声卡
    DX_VERIFY(DirectSoundCreate(
		NULL,//使用NULL表明使用默认的声卡(等价于 &DSDEVID_DefaultPlayback ?)
        &m_pdSound, NULL));
    
    //设定协作等级
    //  Normal -- 当应用有焦点时，可以播放声音，但不妨碍其他应用，DSound会创建一个隐含的22kHz、立体声、8位的默认主缓冲
    //  Priority -- 拥有所有硬件的入口，可以改变主混频器的设定，可要求硬件完成高级内存操作(如压缩),可在要
    //    改变主缓冲的数据格式(如16位)时使用
    //  Exclusive -- 同Priority,但只有当应用在前台时才能听得见
    //  Write_Primary -- 完全控制时才使用(如自己写声音混频器引擎时)
    DX_VERIFY(m_pdSound->SetCooperativeLevel(m_hWnd, DSSCL_NORMAL));

    //创建缓冲，准备缓冲区用于播放

    //声音的格式
    WAVEFORMATEX waveFormate = {0};
    waveFormate.wFormatTag = WAVE_FORMAT_PCM;
    waveFormate.nChannels = 1;  //MONO
    waveFormate.nSamplesPerSec = 11025; //11kHz
	waveFormate.wBitsPerSample = 8;
    waveFormate.nBlockAlign = (waveFormate.wBitsPerSample / 8) * waveFormate.nChannels; // 1
    waveFormate.nAvgBytesPerSec = waveFormate.nSamplesPerSec * waveFormate.nBlockAlign; // 11025
    waveFormate.cbSize = 0;

	//主缓冲
	DSBUFFERDESC  dsPrimaryBufferDesc = {0};
	dsPrimaryBufferDesc.dwSize = sizeof(dsPrimaryBufferDesc);
	dsPrimaryBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME; //| DSBCAPS_CTRL3D
	dsPrimaryBufferDesc.dwBufferBytes = 0;  //当是主缓冲时，该值必须是 0
	dsPrimaryBufferDesc.dwReserved = 0;
	dsPrimaryBufferDesc.lpwfxFormat = &waveFormate;
	DX_VERIFY(m_pdSound->CreateSoundBuffer(&dsPrimaryBufferDesc, &m_pdSoundPrimaryBuffer, NULL));

	//当应用程序有输入焦点时，DS会调用该函数设置主缓冲的格式( 非 DSSCL_NORMAL 时)
	//DX_VERIFY(m_pdSoundPrimaryBuffer->SetFormat(&waveFormate));

	//辅助缓冲
    DSBUFFERDESC  dsBufferDesc = {0};
    dsBufferDesc.dwSize = sizeof(dsBufferDesc);
    dsBufferDesc.dwFlags =  DSBCAPS_STATIC | DSBCAPS_LOCHARDWARE; 

	//缓冲区的大小, 22050表示 在11hKz, 8位、单声道时，够播放2秒的数据，
    dsBufferDesc.dwBufferBytes = waveFormate.nAvgBytesPerSec * 2;  // 2 seconds, 此处的值为 22050
	dsBufferDesc.dwReserved = 0;
    dsBufferDesc.lpwfxFormat = &waveFormate;

	//3D效果时的扬声器模拟算法(如 DS3DALG_HRTF_LIGHT )，当设置了 DSBCAPS_CTRL3D 时才需要设置该值
	dsBufferDesc.guid3DAlgorithm = DS3DALG_DEFAULT;
    DX_VERIFY(m_pdSound->CreateSoundBuffer(&dsBufferDesc, &m_pdSoundBuffer, NULL));

    //如读取WAV等文件，使用声音数据填充声音缓冲
	//如果声音数据太大的话，必须创建流播放方法，加载声音数据中的一小块，当这一小块播放完毕以后，
	//  再加载另外的小块数据进缓冲区，一直持续这个过程，直到声音被处理完毕。
    //m_pdSoundBuffer->Lock();
    //write
    //m_pdSoundBuffer->Unlock();
    return 0;
}

void CMainFrame::OnDestroy()
{

    CFrameWnd::OnDestroy();
}
