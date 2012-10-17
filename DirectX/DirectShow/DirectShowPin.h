#pragma once
/*****************************************************************************************************
*
* CBasePin   //从 IPin 和 IQualityControl继承
* +-CBaseInputPin  //实现 IMemInputPin
*   +-CRenderedInputPin
*   +-CTransformInputPIn
*     +-CTransInPlaceInputPin
* +-CBaseOutputPin //输出Pin，通过IMemInputPin接口把数据传递给连接的InputPin
*   +-CSourceStream
*   +-CTransformOutputPin
*     +-CTransInPlaceOutputPin
*   +-CDynamicOutputPin //实现 IPinFlowControl 接口，可以阻塞(Block)?
*       从名字上来看，可以动态改变输出? ChangeOutputFormat/DynamicReconnect
*       有辅助类 CAutoUsingOutputPin(构造中StartUsingOutputPin,析构中StopUsingOutputPin)
*
* CBasePin
*   1.概要说明
*     通过成员变量 m_mt 保存了当前连接使用的 CMediaType,
*   2.必须重载的(纯)虚函数
*     a.CheckMediaType(const CMediaType *pMediaType); -- 检查是否支持指定的媒体类型，
*       比较Type、SubType、Format(将返回值转换为类型信息,如VIDEOINFO)等 
*   3.可选的重载函数
*     Active/Inactive/Run -- 状态转换时的通知函数
*     EndOfStream(); //没有数据传送时调用，一般可以用于结束处理(如 写结束标志，发 EC_COMPLETE 消息等 ?)
*       只在InputPin上调用,OutputPin上调用 DeliverEndOfStream 
*     GetMediaType(int iPosition,CMediaType*); //返回可支持的媒体类型，基类的默认实现返回错误
*     Notify(IBaseFilter* pSender, Quality); //IQualityControl接口通知数据流的速度
*     SetMediaType(const CMediaType *); //Filter之间协商成功后，通过该函数通知Pin。
*       注意子类中先调用基类的方法保存最后确定的媒体类型，然后再根据类型(m_mt)做处理
*   4.会被QI的接口
*     IPin
*     IQualityControl
* 
* CBaseInputPin  
*   1.概要说明
*       从 IMemInputPin 接口继承（由 CBaseOutputPin 使用 ）
*       GetAllocator --返回 IMemAllocator 接口，如没有会创建缺省的内存分配器
*       NotifyAllocator -- 通知使用的IMemAllocator,
*       Receive -- OutputPin调用该方法传递数据
*   2.必须重载的(纯)虚函数
*   3.可选的重载函数
*     
*
* CRenderedInputPin
*   1.概要说明 -- 用于 Render Filter 的输入Pin
*   2.必须重载的(纯)虚函数
*     a.CheckMediaType(const CMediaType *);
*   3.可选的重载函数
*
* CBaseOutputPin  
*   1.处理 IMemAllocator 和 IMemInputPin 接口，若输入Pin不提供内存分配器，则输出Pin提供
*   2.必须重载的(纯)虚函数
*     DecideBufferSize(IMemAllocator * pAlloc,ALLOCATOR_PROPERTIES * ppropInputRequest);
*       根据协商好的媒体类型(m_mt)，确定使用的分配器属性--内存大小(cbBuffer)、使用的Sample数量(cBuffers)等，
*       并通过 IMemAllocator::SetProperties 设置，
*       (注意：函数可能返回成功，但没有成功分配需要的内存，因此设置完以后需要检查真实的设置项是否满足要求)
*   3.可选的重载函数
*     CompleteConnect -- 完成连接时会调用，可用于协商分配器
*     DecideAllocator -- 可使用自定义的 IMemAllocator
*
* CSourceStream
*   1.概要说明 --  使用推模式，由独立线程生成数据。SourceFilter上对应的输出Pin
*   2.必须重载的(纯)虚函数
*     a.FillBuffer(IMediaSample *pms) -- 填充 IMediaSample ，提供具体的数据
*       pms->GetPointer(&pData); //获取到需要填充 pms->GetSize() 个字节的 pData 缓冲区，填入数据后
*       pms->GetActualDataLength();
*       填充数据 ...
*         Video时 -- pData 是 BITMAPINFOHEADER::biSizeImage 大小的内存块，其中需要填充的是位图信息？
*         Audio时 -- pData 是 WAVEFORMATEX 的什么？
*       pms->SetTime(...);pms->SetSyncPoint(TRUE); //(非压缩视频)设置同步信息，表明每一帧的有效时间范围
*     b.DecideBufferSize(IMemAllocator * pAlloc,ALLOCATOR_PROPERTIES * ppropInputRequest); -- 见CBaseOutputPin
*     c.GetMediaType(int iPosition, CMediaType *pMediaType) -- 返回这个Pin可以支持的媒体类型，应该按优先次序提供(0最好)
*       iPosition < 0 ：return E_INVALIDARG; 
*       iPosition > 最大支持个数：return VFW_S_NO_MORE_ITEMS; 这样框架就不会再次调用该函数了
*       iPosition 是有效值：
*         1).类型指针 *pti = pMediaType->AllocFormatBuffer(sizeof(类型信息)); //分配媒体类型(如VIDEOINFO)需要的内存
*         2).pti->XXXX = .... ; //按照需要给类型赋值
*         3).pMediaType->SetType/SetSubtype/SetFormatType/SetSampleSize 设置可以使用的格式信息
*     d.CheckMediaType(const CMediaType *pMediaType) -- 
*     e.SetMediaType(const CMediaType *pMediaType); -- 设置协商好的媒体类型，并设置必要的参数，此后连接即建立
*   3.可选的重载函数
*     a.OnThreadCreate -- 推线程创建时
*       OnThreadDestroy
*       OnThreadStartPlay
*     b.Notify(IBaseFilter * pSender, Quality q); -- 处理质量控制通知，可以调整数据源的位置？
*   4.会被QI的接口(参见 CBaseOutputPin)
*       IMediaSeeking
*       IMediaPosition
* 
* CTransformInputPin
*   通常输入PIN没有首选的媒体类型。它全部依赖上一级filter所使用的的媒体类型。
*   
* CTransformOutputPin
*   1.概要说明
*     在大多数情况下，Transform Filter 必须在设置输出类型前决定输入类型
*   2.必须重载的(纯)虚函数
*   3.可选的重载函数
*
* CDynamicOutputPin -- 可以动态改变输出？
*   1.概要说明
*   2.必须重载的(纯)虚函数
*   3.可选的重载函数
*      
* CPullPin
*   1.概要说明 -- 辅助输入Pin实现“拉”模式的线程类，从CAMThread继承，与其相连的上一级的输出Pin必须实现
*     IAsyncReader接口。作为输入CBasePin子类的成员变量使用(Active中启动拉数据线程，Inactive中停止线程)
*   2.必须重载的(纯)虚函数
*     Receive(IMediaSample*) -- 处理Sample，并将Sample通过输出Pin往下传送
*     BeginFlush/EndFlush/EndOfStream
*     OnError(HRESULT hr)
*   3.可选的重载函数
*****************************************************************************************************/

