#pragma once

/*************************************************************************************************
* 依靠filter，媒体数据的内存空间可以通过多个途径来分配：在堆上、在DirectDraw表面（surface）、在共享GDI内存或使用其它的分配机制。
* 
* Pin使用的的MediaSample数据协商(两个连接的Pin中,必须有一个Pin提供一个分配器，Pin还负责协商分配器创建的缓冲数和每个缓冲的尺寸)
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
* IMemAllocator -- 负责分配内存空间, 管理 IMediaSample 的内存池，所有的Filter都释放了Samle后，
*   才能返回给分配器的缓冲池(即不要随便在filter中保存IMediaSample的指针?)
* IMemInputPin -- （注意：不是从IPin上继承）管理 IMemAllocator，并能接收 IMediaSample
* IMediaSample
*************************************************************************************************/
