#ifndef DIRECT_SHOW_DMO_H
#define DIRECT_SHOW_DMO_H
#pragma once

/*************************************************************************************************
* DMO(DirectX Media Objects)：另一种数据流处理解决方案，实现了 IMediaObject 接口。
*   使用流(Stream)，输入到输出的转换方式有"复制--IMediaObject::ProcessInput/ProcessOutput"
*   和"就地--IMediaObjectInPlace::Process"处理两种。
*   DMO的使用者分配数据内存，填入数据后送给DMO(同步)处理，等DMO处理完后再取出处理过的数据。
* 
* 优点 -- 微软推荐使用DMO替换Filter(编码器和解码器的推荐解决方案)
*   1.可以不用加入FilterGraph就能使用其功能。
*   2.在DirectShow中可以使用DMO Wrapper Filter 包装具体的DMO参与到FilterGraph中。
*   3.基于DirectSound的应用程序也可以使用各种声音效果的DMO(参见 dmodemo 程序)
* 
* 设置媒体类型注意：
*   1.对于解码DMO，一般先设置输入流上的媒体类型，然后在输出流上选择一种输出格式；
*     对于编码DMO，一般先设置输出流上的媒体类型，然后在输入流上设置一种输入格式；
*
* 在DirectShow中使用DMO -- 系统提供Wrapper，聚合DMO
*   1.使用CoCreateInstance创建DMO Wrapper Filter实例(CLSID_DMOWrapperFilter),
*     获得IBaseFilter接口，以后通过该接口和Graph交互；
*   2.获取 IDMOWrapperFilter 接口，并调用其 Init 方法，指定DMO的CLSID和类型目录
*     (如 DMOCATEGORY_VIDEO_ENCODER / DMOCATEGORY_AUDIO_EFFECT )
*   限制：
*     a.DMOWrapperFilter只支持单输入、单输出的Transform型DMO;
*     b.DMOWrapperFilter的输入Pin只支持使用IMemInputPin接口传送数据(推模式);
*     c.当前的DES(DirectShow Editing Services)不支持DMO的效果(Effect)和过渡(Transition)
*
* 通过系统枚举的方法创建DMO -- DMOEnum
*   通过 IDMOWrapperFilter 接口使用(<dmodshow.h>)
*   
* DMO的开发 -- ATL方式的COM组件(有没有辅助开发的类？)
*  1.支持聚合(可以被DMOWrapperFilter聚合)，线程模型为Both;
*  2.实现IMediaObject接口 -- 已经有IMediaObjectImpl实现,接口需要重载的函数的内部函数名为Internal##XXX -- 由宏INTERNAL_CALL实现
*    class CMyDMO : public IMediaObjectImpl<CMyDMO, 1(输入流个数),2(输出流个数)>
*                 , public IMediaObjectInPlace
*    使用 LockIt lck(static_cast<_DERIVED_ *>(this)); 进行自动范围锁
*  3.如果是音效DMO，必须实现IMediaObjectInPlace接口(可以在DirectMusic或DirectSound中使用)
*
* 测试：dmotest.exe -- 没有找到？
* 注册：DllRegisterServer 中调用 DMORegister 函数注册DMO的类型信息，_Module.RegisterServer 注册COM信息
* 
* //1.创建
*   IMediaObject* pDMO = NULL;
*   CoCreateInstance(CLSID_XXX,...(void**)&pDMO);
* //2.对各个输入输出流进行媒体类型的设置--先取得各个流上的首选(Preferred)媒体类型
*   DMO_MEDIA_TYPE mt = {0}; //媒体类型
*   MoInitMediaType(&mt,sizeof(WAVEFORMATEX));
*   mt.majortype = MEDIATYPE_Audio;
*   mt.subtype = MEDIASUBTYPE_PCM;
*   mt.formattype = FORMAT_WaveFormatEx;
*   WAVEFORMATEX* pWave = reinterpret_cast<WAVEFORMATEX*>(mt.pbFormat);
*   //...
*   pDMO->SetInputType(0,&mt,0); //给流0设置媒体类型
*   MoFreeMediaType(&mt);
**************************************************************************************************/


#endif //DIRECT_SHOW_DMO_H