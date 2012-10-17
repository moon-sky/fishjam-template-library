#ifndef DIRECTSHOW_PERFORMANCE_H
#define DIRECTSHOW_PERFORMANCE_H
#pragma once

/*****************************************************************************************************
* 性能度量(performance measurement),当代码中定义了 DXMPERF/PERF 宏时，会在 amfilter.cpp 中自动包含 <dxmperf.h>
*   然后使用 PERFLOG_CTOR、PERFLOG_RUN 等宏在各个关键阶段打印日志。
* 
*   CVideoTransformFilter::RegisterPerfId
*     m_idSkip -- Video transform skip frame
&     m_idFrameType -- Video transform frame type
*     m_idLate -- Video transform lateness
*     m_idTimeTillKey -- Video transform estd. time to next key
*
* 质量控制(quality management) -- 自适应的反馈机制，
*   IQualityControl -- 进行质量控制，用于通知数据流的速度是否合适
*     Notify(IBaseFilter * pSender, Quality q);  
*       //q.Type -- Famine:速度太慢，Flood：速度太快；
*       //q.Proportion -- 请求调节的速度比率，以1000为基准：m_iRepeatTime = m_iRepeatTime*1000 / q.Proportion;
*       //q.Late -- 当前最新的Sample比其上的时间标记晚到了多少，负数表早到：m_rtSampleTime += q.Late;(直接跳过中间部分)
*       //q.TimeStamp -- 发送该消息时的时间，通常是Sample上的开始时间 -- 怎么用？

*****************************************************************************************************/



#endif //DIRECTSHOW_PERFORMANCE_H