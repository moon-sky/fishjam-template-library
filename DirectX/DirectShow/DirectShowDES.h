#ifndef DIRECT_SHOW_DES_H
#define DIRECT_SHOW_DES_H
#pragma once

/*********************************************************************************************************
* DES(DirectShow Editing Services) -- 视频非线性编辑，介于应用程序和DirectShow Core API之间。主要有以下几个部分：
*   TimeLine -- 组织各个媒体源、音视频效果、过渡处理等的信息集合，代表了最终的视频编辑作品；
*   XML Parser -- 将TimeLine结构转化为XML格式的文件保存，或者从XML格式文件生成对应的TimeLine；
*   Render Engine -- 将TimeLine实际转化为DirectShow Filter实现的输出控制引擎
*   Media Locator -- 用于媒体文件的定位
*
* DES 内置了 100多种SMPTE过渡(Transition)处理，以及微软IE自带的各种音视频效果(IAMTimelineEffect)、
*   过渡(IAMTimelineTrans)组件
*  
*   TimeLine(标识一个视频编辑的项目 -- CLSID_AMTimeline, IAMTimeline, 然后使用CreateEmptyNode创建各种DES对象)
*     => Audio/Video Group(每个组输出单一格式的媒体流，所有的组构成一个TimeLine，音频和视频是分开处理的 -- IAMTimelineGroup)
*       1=>n Composition(复合节点，可以对子复合节点或Track进行各种复杂的编辑 -- IAMTimelineComp)
*         1=>n Composition/Track(每个Track有统一的媒体格式输出；视频的优先级依次降低-0最高,音频只是简单合成 -- IAMTimelineTrack) 
*           1=>n Source(即媒体源，提供最基本的素材 -- IAMTimelineSrc::SetMediaName)
*
* DES有三种时间：
*   1.时间线时间(TimeLine Time):相对于整个时间线项目的时间，也称为项目时间 -- IAMTimelineObj::SetStartStop(2)，
*     注意：不能给同一个Track上的不同媒体源设置重叠的时间线时间，否则会引起不确定的错误。
*   2.媒体时间(Media Time):相对于媒体源的时间，如果媒体源是一个文件，则指相对于文件开头的位置时间，
*     必须设置(包括开始和结束) -- IAMTimelineSrc::SetMediaTimes(2)
*   3.父对象时间(Parent Time):相对于时间线中已经编排的某个对象的时间，
* 可以在高优先级的Track上设置过渡处理对象，默认的过渡方向是从低优先级过渡到高优先级。
*
* DES有两种帧率：
*   1.组输出帧率:组上设置的帧率
*   2.媒体源帧率:媒体源本身的帧率，一般不能更改
*   当媒体源上设置的媒体时间长度和时间线时间长度不等时，会形成快放或慢放的效果 
*     播放速率 = (媒体结束时间 - 媒体开始时间)/(时间线结束时间 - 时间线开始时间) -- 注：和输出帧率无关
*
* DES的帧边界处理
*   DES会自动将所有的时间就近取舍维帧边界(如30ftps的，第一帧0.0333s后，第二帧将为 0.0666s，如果随机定位
*   到 0.05s 可能引起异常)
*   解决方法:
*     1.调用媒体源上的 IAMTimelineObj::GetStartStop获得原始设定的开始、结束时间；
*     2.调用IAMTimelineObj::FixTimes将这两个时间转换成帧边界时间，以修正后的帧边界时间进行定位。
*   
* DES预定义的缩放模式：
*   1.RESIZEF_STRETCH(默认模式) -- 拉伸填充。
*   2.RESIZEF_CROP -- 不进行缩放。源太小时，周围填成黑色；太大时将超出的部分剪裁掉。
*   3.RESIZEF_PRESERVEASPECTRATIO -- 保持宽高比率缩放。内切，不足部分加黑边。
*   4.RESIZEF_PRESERVEASPECTRATIO_NOLETTERBOX -- 保持宽高比率缩放。外切，超过部分被裁减。
*
* 媒体探测器组件(Media Detector,CLSID_MediaDet)
*   有信息获取(初始状态)和抓图(EnterBitmapGrabMode)两种模式，
*   
* DES支持自定义的SourceFilter，将其作为DES的媒体源参与到视频编辑项目中 -- 要求支持随即定位(输出Pin实现IMediaSeeking)
*
* 音视频效果和过渡
*   使用Transform Object技术(?)，
*********************************************************************************************************/

#endif //DIRECT_SHOW_DES_H