#ifndef DIRECT_SHOW_HARDWARE_H
#define DIRECT_SHOW_HARDWARE_H
#pragma once

//注意：一些第三方的Filter会在Debug中枚举时抛出异常(int 3)来防止破解
//  
/*********************************************************************************************
* 内核流
*    WDM设备支持内核流，在内核流中数据在内核模式下被彻底流化而永远不需要切换到用户模式下去，
*    从而避免了在内核模式和用户模式之间切换的巨大开销，内核流允许高的比特率而不消耗CPU的时间。
*    比如:硬件采集视频 -> 硬件压缩 -> D3D硬件加速显示 (整个流中的数据不会传入到用户模式)
*
* 自己创建的Filter可以在 DllRegisterServer 函数实现中通过 CLSID_FilterMapper2 的 
*   IFilterMapper2::RegisterFilter 注册到系统的Category中
*   需要声明 REGFILTER2 rf2FilterReg 变量
*
* 
* DirectShow 通过对硬件提供WrapperFilter(CLSID_AVICo/CLSID_ACMWrapper 等)，使得用户模式(Ring3)下的应用程序能操作内核模式(Ring0)的硬件。
*   DirectShow提供一个叫KsProxy的filter，它可以实现任何类型的WDM流驱动,硬件商通过提供一个Ksproxy plug-in(被聚合的COM对象，
*     通过 IKsPropertySet 接口来支持驱动的自定义属性)来扩展KsProxy,以使其支持自己的功能
*
* CLSID_CaptureGraphBuilder2 -- 进行音视频采集,获得ICaptureGraphBuilder2接口，使用SetFilterGraphg关联到IGraphBuilder上.
*   1.使用 SetOutputFileName 设置输出文件的路径，可以得到 multiplexer 和 FileWriter 的Filter
*     pCapture->SetOutputFileName(&MEDIASUBTYPE_Avi, strFileName, &pOutMutex, &pOutSink);
*   2.可以使用RenderStream对指定类型的Pin完成后续连接(如 PIN_CATEGORY_PREVIEW 指定 Preview PIn， 
*       _CAPTURE 指定捕获， Codec 指定编码器， _CC或_VBI 指定CC );
*     g_pCapture->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pSrcCaptureFilter, pVideoCodecFilter, pMultiplexer);
*   3.智能构建PreviewPin：
*     a.有VideoPort Pin 时，连接 Overlay Mixer -> Video Renderer
*     b.有PreviewPin时，直接连接 Video Renderer；
*     c.只有Capture Pin 时，自动插入一个SmartTee Filter 进行分路，然后连接到 Video Renderer.
*   4.自动加入采集卡Filter之前的辅助Filter(如用于选择视频输入端子的Analog Video Crossbar Filter),
*     并完成连接。这些Filter之间的连接不是通过媒体类型来标识的，而是通过媒介(Medium)的GUID来标识的。
*   5.辅助方法，如FindPin、FindInterface(通过 IKsPropertySet::QuerySupported ?查看，也就是说 Filter 需要支持这个接口？ )
*        查找Filter上是否支持CC等Pin
*          FindPin(pVideoCap, PINDIR_OUTPUT, 
*            &PIN_CATEGORY_CC,   // &PIN_CATEGORY_VBI
*            NULL, FALSE, 0, &pPin);
*        查找CapFilter上的压缩接口(IAMVideoCompression, 通常硬件 WDM TVTuners 会提供?) 和 配置接口(IAMStreamConfig)
*        FindInterface(&PIN_CATEGORY_CAPTURE,
*          &MEDIATYPE_Interleaved,  //&MEDIATYPE_Video,
*          pVideoCap,
*          IID_IAMVideoCompression, (void **).pVideoCompress); // IID_IAMStreamConfig, IID_IAMDroppedFrames
*        调用该方法之后会自动连接相关的Filer ?
*   6.开始捕获(两种方式：ControlStream 或 IMediaControl::Run )
*        g_pCapture->ControlStream(&PIN_CATEGORY_CAPTURE, NULL, NULL, NULL, &stop, 0, 0);
*   7.自动处理 EC_REPAINT 问题（IMediaEvent::CancelDefaultHandling(EC_REPAINT)，防止写文件异常）
*     
* KsProxy.ax -- 代表各种使用WDM(Windows Drive Mode)驱动的音视频设备，比如USB的摄像头通过WDM Video Capture Filter封装。
* 
* 在程序中可以通过系统枚举器来枚举系统中安装的所有硬件，创建这些代表硬件的Filter，也需要通过枚举(不能直接创建)。
* 枚举过程 [ HKEY_CURRENT_USER\Software\Microsoft\ActiveMovie\devenum ]
*   1.创建系统枚举器组件对象(CLSID_SystemDeviceEnum),并获得 ICreateDevEnum 接口，如返回S_FALSE表示类型下数据为空(或不存在?)
*   2.使用其 CreateClassEnumerator 方法，为指定的Filter注册类型目录创建一个枚举器，获得IEnumMoniker接口；
*     其他的注册类型目录有很多，如：
*     CLSID_ActiveMovieCategories -- 所有注册类型的目录(GraphEdt的类型树)
*     CLSID_LegacyAmFilterCategory -- 所有注册的DirectShowFilter
*     CLSID_AudioCompressorCategory -- 音频压缩
*     CLSID_AudioInputDeviceCategory -- 音频捕捉设备
*     CLSID_AudioRendererCategory -- 音频输出
*     CLSID_LegacyAmFilterCategory -- DirectShow Filters
*     CLSID_VideoCompressorCategory -- 视屏压缩
*     CLSID_VideoInputDeviceCategory -- 视屏捕捉设备
*   3.使用IEnumMoniker::Next 接口方法枚举设备标识(Device Moniker)，获得IMoniker接口；第一个得到的将是控制面板中的首选设备。
*   4.使用 IMoniker::BindToStorage(如 NULL,NULL,IID_IPropertyBag,&pPropertyBag)后，
*     可以访问设备标识的属性集(如 L"FriendlyName", L"CLSID", L"FilterData", 等); IMoniker::GetDisplayName
*   5.使用 IMoniker::BindToObject,可以将设备标识生成一个IBaseFilter，然后加入FilterGraph;
*     5.1.也可以通过IFilterGraph2::AddSourceFilterForMoniker 直接创建Filter并加入FilterGraph
*   例子代码
*     CComPtr<ICreateDevEnum> pSysDevEnum;
*     CoCreateInstance(CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC,IID_ICreateDevEnum,(void**)pSysDevEnum);
*     CComPtr<IEnumMoniker> pEnumCat;
*     pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,&pEnumCat,0);
*     pEnumCat->Reset();
*     CComPtr<IMoniker> pMoniker;
*     while (pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK) //最后一个(或空)会返回 S_FALSE
*     {
*       CComPtr<IPropertyBag> pPropBag;
*       pMoniker->BindToStorage(NULL,NULL,IID_IPropertyBag,(void**)&pPropBag);
*       if(SUCCEEDED(hr){
*         VARIANT varName; 
*         VariantInit(&varName); //varName.vt = VT_BSTR;
*         pPropBag->Read(L"FriendlyName",&varName,NULL); //获得 FriendlyName 属性 varName.bstrVal
*         VariantClear(&varName);
*       }
*       CComPtr<IBaseFilter> pFilter;
*       pMoniker->BindToObject(NULL,NULL,IID_IBaseFilter,(void**)&pFilter); //
*       pMoniker.Release();
*     }
*********************************************************************************************/

/*********************************************************************************************
* 音频采集 -- Audio Capture Filter,内部使用 waveInXXX 的函数簇
*   从 CLSID_AudioInputDeviceCategory 枚举，可以通过 InfTee 将单输入分割成用于预览和保存的输出流,
*     或使用ICaptureGraphBuilder2::RenderStream(&PIN_CATEGORY_PREVIEW,...)会根据需要自动出入一个SmartTeeFilter
*   输入Pin或Filter上使用 IAMAudioInputMixer 接口进行参数设置，如音量、高低音参数
*   输出Pin上可以使用 IAMStreamConfig 接口获取或设置媒体信息(如 采样频率、声道数等)
*   通过输出Pin上的 IAMBufferNegotiation 接口设置音频采集缓冲的大小，以减少声音的延迟。
*
*********************************************************************************************/

/*********************************************************************************************
* 视频采集 -- 视频采集卡、电视接收卡、摄像头、录象机(VTR)等
*   通常有CapturePin(写文件,MediaSample上有时间戳)和PreviewPin(预览，会自动丢帧，不打时间戳)
*     可以通过 ICaptureGraphBuilder2::FindInterface 尝试得到
*   根据使用驱动程序的不同，有VFW(过时，CLSID_VfwCapture)和WDM(最新,)两种卡，
*   VFW 
*     IAMVfwCaptureDialogs --显示设置对话框(只能给用户显示预定义的对话框)
*   WDM -- 在capture和Preview的Pin上可以分别设置参数
*     IAMFilterMiscFlags
*     IAMCrossbar -- 可以有多个输入、输出Pin，可配置各个pin的连接方式?
*     IAMVideoProcAmp
*     IAMAnalogVideoDecoder
*     输出(还是输入?)Pin需要实现
*      IKsPropertySet -- amCap例子中确认是否是模拟视频输入Pin(ANALOGVIDEOIN)，使用Crossbar:
*        Get(AMPROPSETID_Pin,AMPROPERTY_PIN_CATEGORY, NULL, 0, &guid, sizeof(GUID), &dw); 
*        if(guid == PIN_CATEGORY_ANALOGVIDEOIN) {  } 
*      IAMStreamConfig -- 获取可用的音视频格式(GetFormat)，获取或设置当前的媒体格式
*   模拟电视接收 -- 使用接口 IAMTVTuner
*     TV Tuner -- 用于电视频道控制与接收；
*     TV Audio -- 用于电视声音的解码输出；
*     Crossbar -- 用于采集卡输入端子的选择(常见的有 Video Tuner、Video Composite 和 S-Video)
*                 多输入、多输出端口中选择连接的线路?
*********************************************************************************************/

/*********************************************************************************************
* 音视频编码器一般有三种：
*   1.Native codecs -- 如 MP3 的 LAME DirectShow wrapper
*   2.ACM/VCM codecs
*   3.DMO(DirectX Media Object) codecs
*
* 视频编码压缩(VideoCodec) -- 通过 CLSID_VideoCompressorCategory 枚举
*     Microsoft RLE -- MS为AVI格式开发的一种编码，使用RLE无损压缩算法，采用8位的编码方式，只能支持到256色，
*       通常只用于压缩动画或者是计算机合成的图像等具有大面积色块的素材
*     Microsoft Video 1 -- 用于对模拟视频进行压缩，是一种有损压缩方案，最高仅达到256色,一般不用于编码
*   设置音视频压缩属性
*     IAMStreamConfig -- 
*     IAMVideoCompression(输出Pin支持)， 可设置帧率，压缩质量等
*       GetInfo -- 获得Filter默认的帧率和质量等, pCapabilities 的值是 CompressionCaps
*     IConfigInterleaving -- 设置交错模式
*     IConfigAviMux -- 设置主流(SetMasterStream)和AVI兼容性的索引(SetOutputCompatibilityIndex,这样旧程序才能快速定位)
*       AviMux默认创建AVI 2.0的index(indx),设为TRUE可以创建AVI 1.0的index(idx1)
* 编码器(未确认):
*   Xvid -- AVI
*   h264 -- MOV
*   wmv9 -- WMV
*********************************************************************************************/

#endif //DIRECT_SHOW_HARDWARE_H