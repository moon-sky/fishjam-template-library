#ifndef DIRECT_SHOW_STUDY_H
#define DIRECT_SHOW_STUDY_H
#pragma once

//http://xbox360.tgbus.com/zixun/gzpj/201203/20120316134756.shtml
//http://www.cppblog.com/lovedday/archive/2007/09/24/32815.html

//COM => Filters => Pins => MediaTypes

/*************************************************************************************************
* 其前身是 ActiveMovie, 所以很多结构命名为 AM_XXX
* TODO: CSourceStream 等中的 pObjectName 和 pName 分别代表什么?
*
* 调试工具：
*   1. GraphEdit(graphedt.exe) <== DirectShow Graph Tool, 在 Vista或安装了Vista SDK 后需要注册SDK中 proppage.dll 才能远程连接。
*      TODO: 是否需要重新注册 System32 下的 quartz.dll ?
*   2. DXCapsViewer.exe -- 查看当前系统中DirectX设备(显卡、DSound声卡、DInput等) 的能力
*   3. VideoInspector(http://www.kcsoftwares.com) -- 可以查看各种媒体的信息
*
* DirectShow是一套完全基于COM的应用系统，SDK基类源代码中已经实现了Filter中的COM基本特性
* DirectShow 运行库的核心库是 quartz.dll，其中包含用于音频和视频播放的最重要的 COM 接口和 coclass。
* 
* 常用接口
*   IPin -- 进行Pin的连接、断开，信息获取，Stream管理(??)，
*     NewSegment(表明是一个新的Segment组??控制 CRefTime 的 m_tStart 和 m_tStop)
*
*   IMediaFilter -- 从 IPersist 继承，控制 停止、暂停、播放的状态转换 和 同步时钟的管理
*   IBaseFilter  -- 从 IMediaFilter 继承，可以枚举和查找其上的Pin，获取Filter的信息，能加入IFilterGraph
*   IFilterGraph -- 管理Filter（增删查），直接连接
*   IPinFlowControl -- 只有一个 Block 函数(???)
*   IMediaSample -- 进行实际内存传输和管理，其中包括 媒体类型、时间(Time)、媒体时间(MediaTime)、
*     SyncPoint(同步点) -- IsSyncPoint 返回 S_OK 表 TRUE，S_FALSE 表 FALSE)
*     Preroll(预转移) -- 传输速率与解码或者显示需要的数据速度不匹配而产生的buffer需求
*     Discontinuity -- 是否是连续数据,即判断是否存在Seek操作?
*   IMemAllocator -- 管理 IMediaSample 的内存池 
*   IMemInputPin -- （注意：不是从IPin上继承）管理 IMemAllocator，并能接收 IMediaSample

*   IEnumPins -- 枚举Filter上的Pin
*   IEnumMediaTypes -- 枚举Pin支持的媒体类型(注意：Filter上没有枚举的概念?参见 CEnumMediaTypes 的定义)
* 
* DirectShow 需要导出 DllEntryPoint 函数，
* Filter Graph 
*   IFilterGraph：管理其中的Filter，可进行直接连接
*   IGraphBuilder:进行智能连接
*     AddSourceFilter -- 给出源文件路径，智能加载相应的Source Filter
*       1.如果路径中有协议名(如http)，在 HKEY_CLASSES_ROOT\{protocol}\Extensions 中查询扩展名，如果
*         满足，使用对应CLSID的Filter，否则使用"Source Filter"指定的CLSID -- File Source (URL)；
*       2.如果不包含协议名，分析扩展名，HKEY_CLASSES_ROOT\Media Type\Extensions 匹配扩展名；
*       3.进行文件的校验字节判断：HKEY_CLASSES_ROOT\Media Type\{MEDIATYPE_Stream}\{subtype}，
*         其格式为"offset,cb,mask,val" -- 从 offset 开始的 cb 个字节，和 mask 进行位与后，同 val 是否相等
*       4.创建名为 File Source(Async.) 的Filter，其 MediaType 使用 { MEDIATYPE_Stream, MEDIASUBTYPE_None }
*     Render -- 从指定输出Pin开始进行智能连接
*       1.如果输入Pin支持 IStreamBuilder 接口，则由 IStreamBuilder::Render 完成智能连接
*       2.使用 IGraphConfig::AddFilterToCache 加入内部缓冲中的Filter进行试连接;
*       3.使用当前 Filter Graph 中还没有完全连接的Filter进行试连接；
*       4.使用 IFilterMapper2::EnumMatchingFilters 搜索大于 MERIT_DO_NOT_USE 的所有Filter进行试连接；
*     RenderFile --
* 
* Filter: SourceFilters(只有输出Pin) => TransformFilters => RenderingFilters(只有输入Pin)
*   Filter 必须加入到FilterGraph且介入到工作链路中才能发挥作用，否则必须移植成DMO
*   IBaseFilter：
* Pin：
*   IPin：
*
* Filter 的注册(由 AMovieDllRegisterServer2 函数控制)：
*  基本的COM信息(ThreadingMode为Both) 
*  Filter特有的信息(注册的类型目录、Pin数量、支持的媒体类型等，
*    按照以下层次定义注册所用的结构体信息
*      CFactoryTemplate
*       => AMOVIESETUP_FILTER -- 可通过重载 CBaseFilter::GetSetupData 返回，可支持动态注册(IAMovieSetup::Register)?
*        => AMOVIESETUP_PIN
*         => AMOVIESETUP_MEDIATYPE 
* 
*    通常注册在 CLSID_LegacyAmFilterCategory(DirectShow Filters) 类型下
*    所有的Category：HKEY_CLASSES_ROOT\CLSID\{DA4E3DA0-D07D-11d0-BD50-00A0C911CE86}\Instance -- CLSID_ActiveMovieCategories
*    指定Category下的所有Filter：HKEY_CLASSES_ROOT\CLSID\{Category}\Instance -- 如"{083863F1-70DE-11d0-BD40-00A0C911CE86}"
*    如何创建自己的类型目录：？？？
*
*  Merit值：Filter被智能连接时所使用的优先级，只有大于 MERIT_DO_NOT_USE(0x200000) 时，才可能被智能连接使用。
*
* 
* 智能连接(Intelligent Connect)
*   优先使用Cache中的(IGraphConfig::AddFilterToCache 将 Filter 从FilterGraph中移入Cache，智能连接成功的话再移回 FilterGraph)
*     -> 接着使用 Filter Graph 中的 
*       -> 最后通过注册表查找高于 MERIT_DO_NOT_USE 的
*   输出Pin -> 输入Pin，主要由CBasePin实现)：  TryMediaTypes、QueryAccept ?
*   IPin(出Pin)::Connect(入Pin)
*     -> 检查是否已经连接 和 是否能在非停状态下动态连接
*     -> 出Pin::AgreeMediaType(协商双方都支持的媒体类型，如不满足条件通常返回 VFW_E_TYPE_NOT_ACCEPTED )，失败的话BreakConnect
*       -> 如果是完全指定的媒体类型，调用 出Pin::AttemptConnection 直接连接；
*          否则按 输入Pin、输出Pin的顺序 枚举Pin上的媒体类型(EnumMediaTypes)，然后尝试连接 TryMediaType -- 内部调用 AttemptConnection )
*          在枚举时，会调用 出Pin::GetMediaType 方法，查看支持的类型，应该按照顺序安排可支持的格式(最优先的在0位置)
*         ->出Pin::AttemptConnection内部调用顺序（可以看出基类CBasePin的虚函数调用）：
*           ->出Pin::CheckConnect(检查两个Pin是否可以连接，如方向必须不同)[->失败的话BreakConnect]
*           ->出Pin::CheckMediaType(子类必须重载，检查是否能接收这种媒体类型)，如成功会设置m_Connected(连接的Pin)变量
*           ->出Pin::SetMediaType(子类可以重载，但需要先调用基类的方法，保存最后确定的媒体类型m_mt，然后再做处理)
*           ->入Pin::ReceiveConnection(查看连接的Pin是否能接收本Pin和对应的类型，内部逻辑和出Pin的AttemptConnection差不多)
*             ->入Pin::CheckConnect(先检查连接方向)
*             ->入Pin::CheckMediaType(子类必须重载，检查是否能接收这种媒体类型)
*             ->入Pin::SetMediaType(子类可以重载，但需要先调用基类的方法，保存最后确定的媒体类型，然后再根据类型m_mt做处理)
*             ->入Pin::CompleteConnect（输入Pin上的Complete)
*           ->出Pin::CompleteConnect(调用输出Pin上的CompleteConnect协商使用的MediaSample)。已由 CBaseOutputPin重载
*
* Pin使用的的MediaSample数据协商
*   CBaseOutputPin::CompleteConnect
*     ->DecideAllocator(协商内存分配器)
*       ->InputPin::GetAllocatorRequirements(获取输入Pin上的需求)
*       ->InputPin::GetAllocator (优先获取输入Pin的分配器，如果成功则返回)
*       ->DecideBufferSize(输出Pin子类必须重载，根据最后的m_mt，确定使用的分配器属性--内存大小、使用的
*         Sample数量等--通过 IMemAllocator::SetProperties 设置)
*       ->InputPin::NotifyAllocator(成功分配后调用输入Pin的NotifyAllocator通知最终使用的分配器)
*       如果输入Pin不提供分配器或者设置分配器失败
*       ->InitAllocator(由输出Pin创建标准的分配器，再执行相同的逻辑)
*         DecideBufferSize->InputPin::NotifyAllocator)
*
* 源Filter开始数据传输（推模式的 CSourceStream）
*   CAMThread::InitialThreadProc //源Filter的线程函数入口
*     ->CSourceStream::ThreadProc //进行数据传输的工作线程函数
*       ->GetRequest(初始化，等待 CMD_INIT 命令)
*       ->OnThreadCreate（子类可以重载该方法，进行初始化，分配资源等）
*       ->循环执行 GetRequest ,根据命令执行不同的操作(接收到 CMD_EXIT 事件后退出循环):
*         ->CMD_PAUSE(开始运行时，状态转换为 Stop->Pause->Run ,然后运行状态下始终在本函数中) => DoBufferProcessingLoop
*           ->OnThreadStartPlay，之后进入循环，直到接收到 CMD_STOP 事件
*           ->GetDeliveryBuffer(获取空的Sample Buffer)
*             ->m_pAllocator->GetBuffer
*           ->FillBuffer(子类必须重载，根据媒体类型是Video或Audio等填充数据)
*           ->CBaseOutputPin::Deliver(向输出Pin传送Sample)
*             ->m_pInputPin->Receive(调用所连接的输入Pin上的Receive方法，传送数据)
*       ->OnThreadDestroy (停止时进行终止化，释放分配的资源)
* 
* 数据传输结束
*   CBaseOutputPin::DeliverEndOfStream
*     ->CBaseInputPin::EndOfStream
*
* 问题：BeginFlush/EndFlush/Receive 等的关系、顺序
* 
* IMediaSample：数据传送单元，由分配器(Allocator管理)
* 数据传送：
*   推模式(Live Source等) -- CAMThread, Down Filter 的输入Pin上实现 IMemInputPin 接口
*   拉模式(File Source等) -- CPullPin，输出Pin上实现 IAsyncReader 接口，通常由 Splitter的输入Pin上的线程“拉”
*   通过 IOverlay/IOverlayNotify 接口(传统的Video Renderer) -- 没有实际的数据传输？
*
* 传送
*   COutputQueue -- 实现了Sample的队列式传送，可以工作在同步模式，也可以工作在异步模式(使用一个独立的线程)
*     在输出Pin上创建实例，并在其构造中指定这个输出Pin连接着的输入Pin指针。数据传送时调用COutputQueue对象的函数
* 
* 实现自定义接口(或支持系统提供的其他接口-- ISpecifyPropertyPages ) -- 
*   （注意：也可以使用IDL生成接口声明 -- 必须将 .idl 文件属性中的 "MkTypLib Compatible"设为 No 才能编译）
*   1.创建接口的头文件(注意需要使用 #ifdef __cplusplus extern "C" { #endif 包含后面的内容)，
*     使用 guidgen 工具生成定义IID（DEFINE_GUID(IID_IXXXX,xxx,xxxx,xxx);）
*   2.使用 DECLARE_INTERFACE_(IXXXX, IUnknown) 宏定义接口
*     STDMETHOD(方法名) (THIS_      //注意：THIS 宏用于没有参数的情况
*       CMediaType *pmt  //其他参数
*       ) PURE;
*   3.实现类继承该接口，并定义 DECLARE_IUNKNOWN 宏 来实现接口的 QueryInterface 等默认方法
*   4.重载 STDMETHODIMP CXXX::NonDelegatingQueryInterface(REFIID riid, void ** ppv); 方法，在其中判断QI的接口进行返回，
*     否则返回基类的 NonDelegatingQueryInterface 方法
*       if (IID_IXXXX == riid) {return GetInterface((IXXXX *) this, ppv);}
*       else { return 基类::NonDelegatingQueryInterface(riid,ppv); }
*     
* 属性页 -- 通常属性页只能直接获取到 IBaseFilter 的接口，如果想获取或控制更多，Filter需要实现自定义接口，并进行QI
*   1.创建属性页需要的标题字符串和对话框资源 -- 注意设置属性：Border--None；Style--Child；TitleBar--False；
*   2.从 CBasePropertyPage 继承，在调用基类的构造函数中，指定 DialogId 参数
*   3.定义并实现static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)并放入全局
*     变量 g_Templates 的数组 -- 和Filter一样
*   4.通常需要重载以下方法进行属性处理：
*     OnReceiveMessage -- 消息处理
*       在更改设置后，可以通过 m_bDirty = TRUE;m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY) 通知Apply有效
*     OnConnect(IUnknown *pUnknown) -- 通过IUnknown接口参数获取Filter的信息（可以自定义接口）,进行初始化
*     OnDisconnect -- 关闭属性窗口时释放相关资源(如果OnConnect 失败返回错误，也会自动调用该函数)
*     OnActivate -- 属性页激活时
*     OnDeactivate -- 切换属性页时，旧的属性页会取消激活，注意：此时原来的Wnd会被Destroy，但Property对象不会析构
*     OnApplyChanges -- 检查并保存设置值，并将m_bDirty设为FALSE
*   5.仿照实现自定义接口的方式， 在Filter中实现并导出 ISpecifyPropertyPages 接口
*       STDMETHODIMP CXXX::GetPages(CAUUID *pPages){
*         //返回属性页的信息(页数和每一页的GUID)
*         pPages->cElems = 1;  
*         pPages->pElems = (GUID *) CoTaskMemAlloc(sizeof(GUID) * 1);
*         pPages->pElems[0] = CLSID_MYPropertyPage; //指定属性页的GUID
*       }
* 
* 持久化 -- Filter 可以在 Graph 中保存并恢复属性等信息
*   1.从 pstream.h 文件中的 CPersistStream 类继承 -- 即实现 IPersistStream 接口;
*   2.通过 NonDelegatingQueryInterface 导出 IPersist 和 IPersistStream 接口
*   3.重载 GetClassID 方法，提供 Filter 的CLSID；
*   4.重载 ReadFromStream 和 WriteToStream 方法 进行持久化；
*     可以调用 WriteInt 和 ReadInt 方法，将整数用UNICODE的方法写入（每个整数占用12个WCHAR）
*   5.重载 SizeMax 方法返回需要保存的字节个数(UNICODE时，注意TCHAR是2个字节)；
*   6.当数据变动后，调用 CPersistStream::SetDirty(TRUE) 方法
*
*
* 硬件设备(Device Filter)
*   一般都要靠枚举来加入Graph, 其名字一般是在枚举时，通过IPropertyBag::Read(L"FriendlyName",&varName, 0)获得。
*   读取"Description"属性可以更详细地得到其他信息
* 
* 播放控制：
*   定位(Seeking) -- IMediaSeeking(定位操作从 Renderer->Source，数据丢弃从 Source->Renderer)
*     一般由推模式的SourceFilter上的输出Pin或拉模式的Parser/Splitter Filter 的输出Pin实现真正的定位操作。
*     系统中有 CSourceSeeking、CPosPassThru、CRendererPosPassThru 等提供定位功能。
*     CTransformOutputPin::NonDelegatingQueryInterface 处理了 IMediaSeeking 和 IMediaPosition(传递到Upper Filter)
*   快进/慢进的支持 通过IMediaSeeking::SetRate（或IMediaPosition::put_Rate）实现。
*     对于Rate改变的响应，主要工作在负责打时间戳的Filter上，有可能是Parser/Splitter Filter,
*     也有可能是Push模式下的Source Filter;这些Filter需要调用CBaseOutputPin::DeliverNewSegment 函数，
*     提示一个新格式的流的开始,然后根据新的Rate重新打好时间戳，
*   单帧播放 -- 通过Filter Graph Manager上获得的IVideoFrameStep接口来实现。当Step函数调用成功，
*     Filter Graph Manager会向应用程序发送一个EC_STEP_COMPLETE事件，Filter Graph也自动转入Paused状态
*
* 在Filtr Graph运行状态下，可以动态加入新的Filter，但不能删掉Filter，也不能断开Filter之间的Pin连接。
*   Filter从Filter Graph中删除必须在Stopped状态，删除前将这个Filter的Pin断开不是必要的！（连接着的Pin
*   可以通过IFilterGraph::Disconnect断开，并且连接两头的Pin都要调用一次！）
* 动态重建技术 -- 保持 Filter Graph 运行状态的同时实现重建
*   1.媒体类型的动态改变：
*   2.增加或删除Filter，重新进行相关Filter之间的连接：
*   3.对一条Filter链路(Filter Chain)进行操作：
*   
*
* 压缩/解压 -- ACM（Audio Compression Manager）和VCM（Video Compression Manager），可以通过 Windows Multimedia API 调用？
*   在DirectShow中都是通过包装Filter(Wrapper Filter)应用的，作为压缩用时，只能通过系统枚举取得。
*   ACM 作为解压用时注册在"DirectShow Filters"目录下，作为压缩用时注册在"Audio Compressors"目录
*     <msacm.h> 头文件中有 acmStreamOpen 等API函数直接使用
*   VCM 作为压缩用时，各压缩器注册在"Video Compressors"目录
*
* 性能度量
*   1.使用 <measure.h> 文件中的 MSR_XXX(怎么使用？)
*   
* 问题
*   1.为什么BaseClass中需要特意处理 UNICODE 宏？非UNICODE时不是成了重复定义？
*   
*************************************************************************************************/
GetSubtypeName
TODO:看看系统提供的Filter有哪些
 
#endif //DIRECT_SHOW_STUDY_H