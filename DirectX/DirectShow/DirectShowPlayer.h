#ifndef DIRECT_SHOW_PLAYER_H
#define DIRECT_SHOW_PLAYER_H
#pragma once

/****************************************************************************************************
* CLSID_DvdGraphBuilder -- DVD播放和导航，支持 IDvdGraphBuilder 接口；也可以使用 MSWebDVD ActiveX 控件
* CLSID_FilterGraph -- DirectShow 的播放组件库，播放、截取、转换等都需要创建，支持如下接口：
*   IAMGraphStreams
*   IAMOpenProgress
*   IAMStats
*   IBasicAudio
*   IBasicVideo
*   IBasicVideo2
*   IFilterChain
*   IFilterGraph
*   IFilterGraph2
*   IFilterMapper
*   IFilterMapper2
*   IFilterMapper3
*   IGraphBuilder -- 
*   IGraphConfig -- 
*   IGraphVersion
*   IKsQualityForwarder{97EBAACB-95BD-11D0-A3EA-00A0C9223196}
*   IMediaControl -- 媒体播放转换控制接口，其中有 Run/Pause/Stop/StopWhenReady 等方法
*   IMediaEvent
*   IMediaEventEx
*   IMediaEventSink
*   IMediaFilter
*   IMediaPosition
*   IMediaSeeking
*   IMarshal
*   IObjectWithSite
*   IPersist
*   IPersistStream
*   IQueueCommand
*   IRegisterServiceProvider
*   IResourceManager
*   IServiceProvider
*   IVideoFrameStep
*   IVideoWindow -- 
*   {877E4352-6FEA-11d0-B863-00AA00A216A1}
*   
* 媒体文件的播放 -- 都是使用 COINIT_APARTMENTTHREADED 初始化COM库(主线程？)
*   #.音频控制(IBasicAudio)
*     VOLUME_FULL=0L;VOLUME_SILENCE=-10000L; 要设置音量，需要使用对数方法计算 log10(vol)/pow(10.0,pos)
*   #.视频控制(Windowed Mode时嵌套在APP UI中的方式)
*     1.播放前的初始化
*       1.1.使用 IVideoWindow 的 put_Owner((OAHWND)m_hWnd); //设置播放窗体的父窗体,
*       1.2.put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);// | WS_CLIPCHILDREN); //更改播放窗体的风格
*       1.3.m_StaticVideoWindow.ModifyStyle(0,WS_CLIPCHILDREN); 防止绘制视频窗口(如避免菜单等，但全屏时回来无法正常显示?需要刷新?)
*       1.4.处理父对话框的WM_ERASEBKGND消息 -- 否则可能会被频繁的刷新成 Gray 背景色， 直接返回即可？
*           CRect rect;
*           m_StaticVideoWindow.GetWindowRect(&rect);  //获取Video播放窗体的范围
*           ScreenToClient(&rect);
*           pDC->ExcludeClipRect(&rect);    //排除视频的播放区域 -- 不要刷背景
*           return CDialog::OnEraseBkgnd(pDC);
*       1.5.处理父窗体的 WM_MOVE 消息，并传递到VideoWindow中(如果使用硬件overlay时,使得Render更新Overlay位置，避免黑屏?)
*            IVideoWindow::NotifyOwnerMessage((OAHWND)hWnd, msg, wParam, lParam);
*       1.6.处理父对话框的WM_SIZE消息，计算视频窗口的大小和位置，并通过 SetWindowPosition 设置(初始化时需要直接设置一次?)
*     2.使用 IVideoFrameStep 步进(先用 CanStep 方法进行检查)，必须处在暂停状态下(Paused)才能工作
*     3.停止播放后(IMediaControl::Stop)，对 IVideoWindow 调用 put_Visible(OAFALSE); put_Owner(NULL); 来释放窗体；
*     4.在消息循环中将消息传给Video窗体以便其处理一些系统消息(见 PlayWnd Sample)
*       //Pass this message to the video window for notification of system changes
*       IVideoWindow::NotifyOwnerMessage((LONG_PTR) hWnd, message, wParam, lParam); 
*     5.播放位置调节
*       REFERENCE_TIME rtNew = (g_rtTotalTime * dwPosition) / 100;
*       hr = g_pMediaSeeking->SetPositions(&rtNew, AM_SEEKING_AbsolutePositioning, NULL,AM_SEEKING_NoPositioning);
*     6.图片抓取
*       6.1. 使用 CFDirectShowUtility::SnapShotBitmap 方法，如果使用的是传统的VideoRenderer，抓图是不可靠的(使用了
*            DirectDraw加速，返回码为？？)，必须处于暂停状态。但如果使用的是VMR，就没有该限制。
*       6.2. 使用Sample中的GrabbleFilter，抓其中的IMediaSample -- 注意后面不要连接标准的VideoRenderer，Grabble是
*            TransInPlace，输入和输出Pin使用相同的数据缓存，可能位于显存上，从显存读取数据比直接从主存中慢很多。
*       6.3. 将抓去的数据直使用GDI函数显示出来
*            VIDEOINFOHEADER* pVideoInfoHeader = ...;
*            SetDIBitsToDevice(hdc, 0,0,
*              pVideoInfoHeader->bmiHeader.biWidth,
*              pVideoInfoHeader->bmiHeader.biHeight,
*              0,0,0,
*              pVideoInfoHeader->bmiHeader.biHeight,
*              (BITMAPINFO*)&pVideoInfoHeader->bmiHeader,
*              DIB_RGB_COLORS);
*
* 事件通知处理(Event Notification) -- 由FilterGraphManager处理或转发给应用程序处理。使用IMediaEvent(Ex)接口
*   FilterGraphManager使用一个消息队列来通知应用程序发生在Filter Graph中的事件?
*   事件处理流程：
*     1.Filter在发生特殊事件时，向FilterGraphManager发出一个事件 -- 使用 CBaseFilter::NotifyEvent
*     2.FGManager对一些特殊的事件有默认的处理方法(可以使用IMediaEvent::CancelDefaultHandling取消默认处理),
*       否则会放到事件队列中，等待上层应用程序处理；
*     3.应用程序获知有事件发生后(通过自定义消息或事件同步对象Event)，使用GetEvent等方法读取事件，并进行处理
*       if(m_pME) //由于使用队列，是异步处理方式，可能在退出时接收到消息，因此处理前需要先判断指针是否还有效
*       {
*         while(SUCCEEDED(m_pME->GetEvent(&evCode,...)) //循环处理所有的消息
*         {
*           switch(evCode){
*             case EC_COMPLETE: Clip播放完毕的事件处理，可以直接控制FilterGraph的状态，通常是 IMediaControl::Stop
*                  但最好通过 PostMessage 的方式转发消息来处理？
*             case EC_ERRORABORT: 出错处理(如磁盘空间满)
*             case EC_DEVICE_LOST: 热插拔设备(如USB、1394等)脱离(lParam2为0)或重新接入(lParam2为1)系统。
*                  全新设备接入时需要使用 WM_DEVICECHANGE 消息相应
*             case EC_REPAINT: 视频窗口要求重画当前图象帧
*             case EC_USERABORT: 
*             case ...
*           }
*           m_pME->FreeEventParams(...); //释放关联的资源
*         }
*       }
*   使用自定义的消息方式
*     1.自定义消息，如  #define WM_GRAPHNOTIFY  (WM_USER+100)
*     2.使用 IMediaEventEx::SetNotifyWindow((OAHWND)m_hWnd, WM_GRAPHNOTIFY, 0); 注册窗体和消息
*     3.在消息循环的 WM_GRAPHNOTIFY 处理代码中，获取并处理消息 -- ON_MESSAGE(WM_GRAPHNOTIFY...)
*     4.释放接口前，记住使用 pME->SetNotifyWindow((OAHWND)NULL, 0, 0) 取消窗体的注册
*   使用事件同步对象 -- FGManager内部创建的事件同步对象，当事件队列中有尚未处理的事件时为有效状态
*     1.使用 IMediaEvent::GetEventHandle((OAEVENT*)&hEvent) 得到事件同步对象的句柄;
*     2.使用 WaitFor 函数簇进行等待
*   循环等待
*     1.使用 IMediaEvent::WaitForCompletion(等待时间, 返回值) -- 如果结束会返回 EC_COMPLETE
*
****************************************************************************************************/

// File filter for OpenFile dialog
#define FILE_FILTER_TEXT \
    TEXT("Video Files (*.avi; *.qt; *.mov; *.mpg; *.mpeg; *.m1v)\0*.avi; *.qt; *.mov; *.mpg; *.mpeg; *.m1v\0")\
    TEXT("Windows Media Files (*.asf,*.wmv,*.wma)\0*.*.asf,*.wmv,*.wma\0")\
    TEXT("Audio files (*.wav; *.mpa; *.mp2; *.mp3; *.au; *.aif; *.aiff; *.snd)\0*.wav; *.mpa; *.mp2; *.mp3; *.au; *.aif; *.aiff; *.snd\0")\
    TEXT("MIDI Files (*.mid, *.midi, *.rmi)\0*.mid; *.midi; *.rmi\0") \
    TEXT("Image Files (*.jpg, *.bmp, *.gif, *.tga)\0*.jpg; *.bmp; *.gif; *.tga\0") \
    TEXT("All Files (*.*)\0*.*;\0\0")

#endif//DIRECT_SHOW_PLAYER_H