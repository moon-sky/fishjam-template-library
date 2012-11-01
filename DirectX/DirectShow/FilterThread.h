#ifndef FILTER_THREAD_H
#define FILTER_THREAD_H

#pragma once

/**************************************************************************************************
* 通常来说，一个Filter应该暂停(阻塞)于以下俩个位置:
*   IMemAllocator::GetBuffer(本Filter使用的MediaSample被占完了) 
*   IMemInputPin::Receive(推模式中下级Filter不能接收数据)
*   IAsyncReader::Request?(拉模式中是否有类似的?)
*
* Filter 默认都是Both型的COM组件，可通过 COINIT_MULTITHREADED 初始化COM( AMovieSetupRegisterServer )
* 使用 CAutoLock + CCritSec 的方式进行同步处理
*   CBaseFilter、CBasePin 都有保护成员变量 CCritSec *m_pLock; 该变量通常由子类定义后通过构造函数传入使用
*   CTransformFilter 拥有 m_csFilter(状态锁)、m_csReceive(数据锁) 两个锁变量，
*     并将 m_csFilter 传给父类的 m_pLock；m_csFilter（先锁定） => m_csReceive(后锁定)
*     状态锁(主线程)：Pause、SetPositions、Connect、BeginFlush 等
*     数据锁(数据线程)：Receive、GetBuffer 等
*   CBasePin 的 m_pLock  通常使用对应Filter的锁
* 
* 使用 DbgLockTrace 宏 可以将 CCritSec 设置为跟踪状态，调试版本下容易找到死锁
* 
* 如果自己有数据，只访问自己时，最好用自己的 CCritSec，这样免得和系统的冲突，可以提高效率？
*   使用Filter的：CAutoLock cAutoLock(m_pFilter->pStateLock());
*     CBallStream::GetMediaType
*     CBallStream::DecideBufferSize
*     CBallStream::SetMediaType
*   使用自己的(m_Ball、m_rtSampleTime)：CAutoLock cAutoLockShared(&m_cSharedState);
*     CBallStream::FillBuffer、
*     CBallStream::OnThreadCreate
*   
* Filter 通过m_State保存三种状态(FILTER_STATE)，可以通过重载的 CBaseFilter::Stop、Pause、Run 等方法来支持暂停、继续等.
*   Filter Graph Manager按从下游Filter到上游Filter的次序来完成所有的状态转换(即从Render开始，Source 结束)，这种顺序可以防止数据丢失或Graph死锁
*   1. State_Stopped
*   2. State_Paused
*   3. State_Running
*      其中 Running/Paused 统称 Active(CBaseFilter::IsActive返回TRUE)，//这时一般不可以动态改变属性？
*
*      Control                     Filter                     
*         Run                      Pause 方法, 从 State_Stopped(0) => State_Paused(1)，对所有连接着的Pin调用 Active 方法
*                                    如是Transform，会调用 StartStreaming
*                                  Run   方法, 从 State_Paused(1) => State_Running(2)，对所有连接着的Pin调用 Run 方法
*         Pause                    Pause 方法，从 State_Running(2) => State_Paused(1)
*         Resume(实际是Run?)       Run   方法，从 State_Paused(1) => State_Running(2)
*         Stop                     Pause 方法，从 State_Running(2) => State_Paused(1)
*                                  Stop  方法，从 State_Paused(1) => State_Stopped(0)，对所有连接着的Pin调用 Inactive 方法
*                                    如是Transform，会调用 StopStreaming
**************************************************************************************************/

#endif //FILTER_THREAD_H