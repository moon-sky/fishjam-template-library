#pragma once

有问题的代码：
CMediaType VideoType; //Strmbase.lib
VideoType.SetType( &MEDIATYPE_Video);
VideoType.SetSubtype(&MEDIASUBTYPE_RGB24);
hr = m_pGrabber->SetMediaType( &VideoType ); // shouldn't fail
hr = m_pCaptureBuilder2->SetOutputFileName(&MEDIASUBTYPE_Avi, L"C:\\test.avi", &pMux, &pSink); 
hr = m_pCaptureBuilder2->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Interleaved, pSmart, NULL, pMux);
                                                             &MEDIATYPE_Video,
/*****************************************************************************************************
* g_hInst -- Filter DLL的句柄，可用于加载DLL中的资源
* NAME -- 定义Filter、Pin等的名字，发布版本时会自动定义为NULL(保证不泄密)
*
* IMediaFilter
* +-CBaseMediaFilter //抽象基类
* +-IBaseFilter
*   +-CBaseFilter    
*     +-CSource
*     +-CTransformFilter
*       +-CVideoTransformFilter
*       +-CTransInPlaceFilter
*     +-CBaseRenderer  //不支持质量控制
*       +-CBaseVideoRenderer //支持质量控制
* 
* 1.创建方式：
*   在Filter类上定义静态的 CreateInstance 方法
*     static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *pHr);
*   在实现文件中定义框架需要使用的全局变量，在其中包含 Filter 和 Pin 的相关信息和创建方法
*     CFactoryTemplate g_Templates[] = {L"FilterNameXXX",&CLSID_FILTER_XXXX,CXXXX::CreateInstance,
*       NULL, &amSetupFilter}};  //其中 amSetupFilter 又是 AMOVIESETUP_FILTER 类型的变量
*     int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);
* 
*
* CBaseMediaFilter
*   包含可以用于共享的 m_pLock 临界区指针（由构造函数传入）
*
* CBaseFilter（注意不是从CBaseMediaFilter继承，但功能应该继承）
*   1.概要说明
*   2.必须重载的(纯)虚函数
*     a.GetPinCount / GetPin <== 返回Pin的信息
*   3.可选的重载函数
*     a.Run(REFERENCE_TIME tStart)/Pause/Stop -- 状态切换时的函数，重载时需要注意:
*       1).CAutoLock cObjectLock(m_pLock); -- 进行锁定；
*       2).调用基类的方法完成基类的功能；
*       3).状态迁移为：Stopped -> Paused(Active) -> Running -> Paused(Inactive) -> Stopped
*          状态转换时必须遵循“从下往上”的顺序(Render->Transform->Source)，否则可能丢失Sample或死锁
*   4.公有静态方法，由外界访问：static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);
*     在其中使用 new 私有的构造函数，来创建Filter
*   5.常用的方法
*     StreamTime -- 从Filter上获取当前的stream time，调用前需要先 CAutoLock cObjectLock(m_pLock);
* 
* SourceFilter -- 
*   1.概要说明
*     从 CSource 继承(LiveFilter，不支持Seeking)
*     需要创建输出Pin指针数组（m_paStreams）-- 在子类的构造函数中创建，由基类负责释放
*       m_paStreams = (CSourceStream **) new CBallStream*[1];
*       m_paStreams[0] = new CBallStream(phr, this, L"A Bouncing Ball!");
*   2.必须重载的(纯)虚函数
*   3.可选的重载函数
*   4.会被QI的接口(除 IBaseFilter、等外)
*     IReferenceClock --
*     IFileSourceFilter
*     IFileSinkFilter
*     IMediaPosition
*     IAMFilterMiscFlags
*     IVideoWindow
*     IBasicAudio
*     IAMDeviceRemoval
*     IAMOpenProgress
*   5.实时源(Live Source) -- 实时地接收数据，比如视频采集和网络广播。要求:
*     a.IAMFilterMiscFlags::GetMiscFlags 返回 AM_FILTER_MISC_FLAGS_IS_SOURCE；
*     b.至少有一个输出 pin 暴露 IAMPushSource，可通过该接口来进行同步
*     c.filter暴露 IKsPropertySet 接口，并具有一个 Capture pin（PIN_CATEGORY_CAPTURE）
*     
* 
* CTransformFilter -- 主要是Encoder和Decoder，对应媒体类型
*   1.概要说明 -- 使用单独的输出和输入缓冲区(copy-transform filter)，接收一个输入采样后，改写新的数据到一个输出采样
*     并且传递到下一个filter，有保护的成员变量 m_pInput、m_pOutput指明输入、输出Pin（如果有多个Pin应该如何？），
*     默认实现在GetPin函数中如果不存在就会创建，如果需要使用自己的Pin，可以在构造或GetPin中创建并赋值给 m_pInput/m_pOuput。
*   2.必须重载的(纯)虚函数
*     Transform(IMediaSample * pIn, IMediaSample *pOut);   //进行媒体的转换，由框架的线程循环调用，如返回S_FALSE表忽略一帧？
*        在Receive方法的内部同步传递输出采样。filter不建立任何工作线程来处理这个数据(通常TranformFilter都是同步的?)
*     CheckInputType(const CMediaType* mtIn); //检查是否支持mtIn的类型
*     CheckTransform(const CMediaType* mtIn, const CMediaType* mtOut); //检查是否支持从 mtIn 转变为 mtOut
*     DecideBufferSize(IMemAllocator * pAllocator,ALLOCATOR_PROPERTIES *pprop);//调用SetProperties设置适当的参数
*       优先获取输入Pin上的分配器(m_pInput->GetAllocator)，并从中获取已经设置好的参数
*     // GetMediaType(int iPosition, CMediaType *pMediaType);// 设置输出Pin能支持的格式
*   3.可选的重载函数
*     SetMediaType(PIN_DIRECTION direction,const CMediaType *pmt); //当连接尝试成功时，检查输入Pin上
*       是否支持输出Pin已经确定的媒体类型（说法和代码是否反了？），如果不支持，需要重连，如以下的示例代码：
*       CTransformFilter::SetMediaType(direction, pmt); //调用基类的方法完成连接
*       if( m_pInput->IsConnected() && m_pOutput->IsConnected() ) //如果输入Pin和输出Pin都连接上的
*       {
*         if (direction == PINDIR_OUTPUT && *pmt != m_pInput->CurrentMediaType())
*         {
*            FILTER_INFO fInfo;
*            QueryFilterInfo( &fInfo );           //获取FilterInfo，主要是 IFilterGraph 接口
*            fInfo.pGraph->Reconnect( m_pInput ); //进行重连
*            QueryFilterInfoReleaseGraph( fInfo );//释放获取的 IFilterGraph 接口
*         }
*       }
*     Receive(IMediaSample *pSample); //
*     StartStreaming/StopStreaming  -- 用于应用逻辑的初始化和终止化
*     
* CVieoTransformFilter
*   1.概要说明
*     主要为视频解码器设计的，包括如downstram renderer在接下来失败将丢帧的功能等
*  
* CTransInPlaceFilter
*   1.概要说明
*     为修改在原始缓冲区中的数据而设计的filter，接收一个采样后，在内部改变采样数据并且传递该采样到下一级。
*     输入、输出Pin使用相同的媒体类型。通常需要将Pin说明为Filter的友元。
*     如果Filter不更改数据(FALSE==m_bModifiesData)，输入、输出Pin会使用相同的Allocator
*   2.必须重载的(纯)虚函数
*     Transform(IMediaSample *pSample);     //就地处理数据(读或修改)
*     CheckInputType(const CMediaType *mtIn);   //检查是否支持mtIn的类型
*   3.可选的重载函数
*   4.注意
*     在Render时可能有性能问题 -- 需要在缓冲区上的read-modify-write操作，如内存位于显存上，则会非常慢
*
* CBaseRenderer
*   1.概要说明
*     简单的Renderer基类，管理单输入Pin
*   2.必须重载的(纯)虚函数
*     DoRenderSample(IMediaSample *pMediaSample);
*     CheckMediaType(const CMediaType *pmt);
*   3.可选的重载函数
*
* Video Render
*   1.Video Render(传统的,Merit=0x00400000) -- 显示时总是创建一个视频窗口，如要显示在指定的UI窗口中，
*     需要修改视频窗口的风格，并将其指定为UI窗口的子窗口。
*   2.Overlay Mixer -- 
*   VMR(Video Mixing Renderer) -- 通过DirectX使用了显卡的图形处理能力(视频合成和显示时不占用CPU),
*     有些显卡在设置为24位色的情况下不能执行Direct3D操作。
*     3.VMR-7 -- 使用DirectDraw7技术，仅在WinXP中(后？)获得，WinXP的默认视频显示Renderer(Merit=0x00800001)。
*       实现了真正的无窗口模式显示
*     4.VMR-9 -- 使用Direct3D 9技术。在任何安装了DirectX9的操作系统上都能使用，不是默认的Renderer
*       (Merit=0x00200000)。不支持Video Port(视频数据在内核模式下直接传送到显卡的显存)的使用。
*       比如 KMPlayer 不能截图
* 
* Muxer Filter -- 将多路输入流合并成一路。如AVI Mux将视频和音频流合成为一个AVI格式的字节流
*   一个Muxer的例子，其中有可重用的 BaseMux，svn://dev.monogram.sk/public/libmonodshow/trunk
*   http://blog.monogram.sk/janos/2008/08/23/how-to-make-a-directshow-muxer-filter-part-1/
*   多个输入Pin，一个Muxing Thread从连接到输入Pin的 Interleaver unit的Queue 中获取数据，处理后传输到输出Pin中
*   输出Pin必须实现IStream接口(能在访问输出文件时进行随即访问)
* 
*   实现时考虑（Pause/Resume 后） -- 尚未确认
*   前提：Muxer Filter接收的Sample只有Audio打上了绝对时间戳，Video的是相对的  -- 绝对和相对？
*   1.Video Sample先到达Muxer，没法从Audio中得到正确的偏移，
*   2.Audio Sample先到达Muxer，给Resume后第一帧的Video Sample的时间戳进行调整，否则播放该文件时会出现某处被卡住的现象
*
* Splitter(分离器) Filter
*   对应封装格式, 分析数据并将其分解成多个流(如音频、视频)，常见的有 CLSID_MPEG1Splitter、CLSID_AviSplitter 等，
* 
* Filter可以实现的系统接口（加入GE后，系统会进行QI的接口）
*   #.IAMDeviceRemoval
*   #.IAMFilterMiscFlags
*   #.IAMOpenProgress
*   #.IBaseFilter
*   #.IBasicAudio
*   #.IBasicVideo
*   #.IFileSinkFilter  -- 写文件，GE中需要选择保存文件
*   #.IFileSinkFilter2
*   #.IFileSourceFilter -- 读文件，GE中需要选择输入文件
*   #.IKsPropertySet
*   #.IMediaPosition
*   #.IMediaSeeking
*   #.IPersistPropertyBag
*   #.IReferenceClock
*   #.ISpecifyPropertyPages
*   #.IVideoWindow
*
* 系统提供可以直接使用的 Filter，一般可以通过 AddFilterByCLSID 加入 GraphBuilder 
*   wmcodecdsp.h 头文件中包含了系统默认可用的编解码器, 对应的 CLSID_XXX 可以通过 GraphEdt 找到后 UuidFromString 得到
*     UuidFromString(TEXT("205769BC-B121-4CA8-A4E6-84A20EF253B7"), &CLSID_DebugInfoFilter);
*
*   CLSID_AsyncReader(File Srouce(Async.)) -- 完成一般的文件读操作并将数据作为字节流传递下去.通过输出Pin上的 IAsyncReader 提供数据
*   CLSID_AviDest(AVI Mux Filter) -- 接收多个输入流，转换为AVI格式，设置其输出Pin。
*     因为AVI文件格式使用固定的帧率而没有时间戳，所以Filter假设Sample在近似正确的时间内到达。
*     如果Sample没有在合适的时间间隔内到达，Filter会插入一个长度为0的空样本，来表示一个丢失的帧。
*   CLSID_AviSplitter -- 从文件的源过滤器（File Source(Async)）拉数据，然后分解到视频和音频流。
*   CLSID_CMP3DecMediaObject -- Windows Media MP3 Decoder ?
*   CLSID_CMPEG2AudioEncoderMFT -- MPEG2 Audio Encoder ?
*     http://msdn.microsoft.com/en-us/library/windows/desktop/hh162908(v=VS.85).aspx
*   CLSID_Colour(Colour space convertor) -- 颜色空间转换，似乎无法控制（自动选择适配的颜色空间），而且输出主要是RGB和YV12。
*     http://msdn.microsoft.com/en-us/library/aa926076.aspx
*   CLSID_FileWriter -- 文件输出，其中实现了 IFileSinkFilter
*   CLSID_FrameRateKeeper -- 没有找到这个Filter！按照IMediaSample的MediaTime速率往下层filter传输数据。
*   CLSID_MP3ACMCodecWrapper -- MS的MP3编码器,Win8才支持
*      http://msdn.microsoft.com/en-us/library/windows/desktop/hh162907(v=vs.85).aspx
*   CLSID_NullInPlace
*   CLSID_NullRenderer -- 只进不出的Filter
*   CLSID_InfTee -- 一进多出的filter，把Input Pin传过来的IMediaSample拷贝多份到Output Pin，输出Pin的数据同输入完全一样
*   CLSID_SampleGrabber 
*   CLSID_SmartTee -- 智能的一入二出的分路Filter，用于 Capture(数据完全等同于输入) + Preview(数据视系统资源、上下层filter流量而做适当丢弃)。
*   CLSID_Tee -- 标准的一进二出分路Filter
*   CLSID_VideoMixer -- 没有找到这个Filter！把两个Input按照时间戳合成为一路视频
******************************************************************************************************/

/******************************************************************************************************
*   CLSID_VideoRenderer -- 最原始的渲染器，它接收到来自解码器解码后的数据流，在显示设备上显示，基本上不能调用到显卡硬件特性，
*      全靠CPU来完成渲染任务，但Win7下能获取到 IDriectDrawVideo 接口
*   CLSID_VideoRendererDefault -- 会自动根据操作系统选择，
*     Win7 下是 CLSID_VideoMixingRenderer
*   VMR(Video Mixing Renderer) -- 能够将若干路视频流合并输出到显示设备上，并且能够很好地调用显卡硬件的拉伸，
*     颜色空间变换等硬件功能，以减少对 CPU 资源的占用率。画面质量取决于显卡硬件。无法直接对正在播放的视频截图。
*     根据渲染模式的不同分为"窗口化(Windowed),"无窗口"(Windowless),"未渲染"(Renderless)模式。
*     根据 DirectX 版本的不同，可以分为：
*       CLSID_VideoMixingRenderer -- ?VMR7,基于2D的DirectDraw7, 其Pin名为VMR，Pin上多了 IVMRVideoStreamControl
*       CLSID_VideoMixingRenderer9 -- 基于3D的Direct3D9, 可以支持视频特效（Video Effects）和视频变换(Video Transitions)
*   CLSID_EnhancedVideoRenderer -- Media Foundation 使用的Render(EVR), 支持 DXVA 2.0，支持 IEVRFilterConfig 等接口，
*     可通过 IMFGetService 进一步获取其他接口
*
******************************************************************************************************/
//Render uses Direct3D or DirectDraw for rendering samples
//Video Mixing Renderer 7（只支持WINXP）使用 DirectDraw7 表面
//Video Mixing Renderer 9使用 Directx9 Direct3D API函数(参考Texture3D Sample)
