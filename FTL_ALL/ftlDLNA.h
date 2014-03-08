#ifndef FTL_DLNA_H
#define FTL_DLNA_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlDLNA.h requires ftlbase.h to be included first
#endif

/*************************************************************************************************************************
* DLNA(Digital Living Network Alliance) -- 数字生活网络联盟，目标是共享室内和户外的可以互联的消费电子(CE)，PC及移动设备等构成的优先和无线网络，
*   促进数字多媒体内容和服务的共享。
* DLNA 联网设备交互性指导方针(Networked Device Interoperability Guidelines) 像素了可供搭建平台和软件基础的可以交互的基本模块。
*
* 设备类型(Device Category)和角色(Roles)：
*   HND(Home Network Devices)
*     DMS(Digital Media Server) -- 数字媒体服务器，存储媒体文件及向家庭网络中的DMP、DMR提供媒体数据的列表和内容
*     DMP(Digital Media Player) -- 数字媒体播放设备，可以浏览和播放网络上的数字媒体内容
*     DMR(Digital Media Renderer) -- 数字媒体描绘设备
*     DMC(Digital Media Controller)
*     DMPr(Digital Media Printer)
*   MHD(Mobile Handled Devices)
*     M-DMS(Mobile DMS), M-DMP, M-DMC
*     M-DMD(Mobile Digital Media Downloader)
*     M-DMU(Mobile Digital Media Uploader)
*   HID(Home Interoperability Devices) 或者是？ Home Infrastructure Device，为其他设备种类间的互操作提供支持
*     MIU(Media Interoperability Unit) -- 桥接媒体格式层，负责内容转换，媒体交互单元，提供 HND 和 MHD 间的媒体格式转换，
*     M-NCF(Mobile Network Connectivity Function) -- 负责桥接网络连接层
*   交互方式： HND <=> [ HID <=> ] MHD
*     要进行HND和MHD设备之间的交互，需要HND或MHD满足对方的要求。但大多数情况下无法满足这样的条件，所以需要HID设备提供桥接和内容转换服务。

* 设备能力(Device Capabilities)和角色(Roles) -- 用工具能查询到？
*   +PU+(Push Controller) -- 发送本地内容到DMR
*   +PR1+/+PR2+(Printing Controller-1/-2) -- 控制DMPr进行图像内容打印
*   +UP+(Upload Controller) -- 具有Upload功能，向DMS或M-DMS发送内容
*   +DN+(Download Controller) -- 从DMS或M-DMS下载内容到本地
*
* 设备功能：
*   UPnP CP(Control Point) -- 基于UPnP设备架构的设备发现和控制
*   MSD(UPnP AV MediaServers) -- 提供访问媒体内容的方式
*   MSCP(UPnP AV MediaServer Control Point) -- 用于浏览和选择MSD提供的媒体内容的控制器
*   MRD(UPnP AV MediaRenderer) -- 提供播放媒体内容的方式
*   MRCP(UPnP AV MediaRenerer Control Point) -- 用于选择MRD播放内容的控制器
*   PrD(UPnP Printer Device) -- 提供打印图像的能力
*   PrCP(UPnP Printer Control Point) -- 创建打印任务的控制器以选择需要由PrD打印的内容
*   MTS(Media Transport Server) -- 
*   MTC(Media Transport Client)
*
* 架构
*   1.Networking and Connectivity(网络和连通性) -- 基于IPv4(/v6?)的各种连接，802.3/802.11/蓝牙等,
*     通过 QoS优化共享网络的资源，使得视频流和音频流播放流畅
*   2.设备发现和控制：设备必须能够发现家庭网络上存在的其他设备和服务，并识别、配置和控制他们的功能和相应的能力。
*     使用 UpnPTM 控制设备协议架构(DCP Framework) 作为设备发现和控制的解决方案，来处理所有这些需求来简化网络设备的互联。
*   3.媒体管理：实现设备和应用对家庭网络中的媒体内容的识别、管理和分发，由 UPnP Audio/Video 和 UPnP Printer 处理这些应用需求，
*     成为DLNA设备的媒体管理解决方案。
*   4.媒体格式：规定了如何对需要传输和播放的内容进行编码，对每一个设备种类，DLNA媒体格式模型针对3类媒体类型(图片、音频、视频)定义了一组强制和可选的媒体格式。
*       Home Devices -- Imaging<JPEG>, Audio<LPCM>, Video<MPEG2>
*       Mobile/Handheld Devices -- Imaging<JPEG>, Audio<MP3,MPEG4>, Video<MPEG4 AVC>
*     如果DMS/M-DMS 设备自身的媒体格式可以被接受设备所支持的情况下，则直接传送；否则必须转换成接收端能支持的媒体格式。
*   5.媒体传输格式：
*     a.流传输方式 -- 当媒体内容包含内置的需要匹配的时序且立刻被DMP/M-DMP/DMR 渲染播放时，或媒体内容是以固定的速率实时生成时。
*     b.交互传输方式 -- 当不包含内部时序信息的媒体内容被用来进行用户实时交互时，如需要立刻显示给用户图片；
*     c.后台传输方式 -- 当媒体内容不需要立即渲染播放或用户对其传输的优先级不要求的时候(主要用于M-DMD/M-DMU进行上传、下载非实时生成的内容时)
*     DLNA设备必须支持以 ★HTTP★ 作为基本传输方式传输多媒体内容，RTP作为可选的媒体传输方式，提供了用来优化媒体传输的System Usage扩展性
*   6.数字版权管理、内容保护 -- 包括链路保护(必须支持DTCP-IP，可选支持WMDRM-ND)和DRM。DLNA必须能保护商业数字内容不被未授权的使用和拷贝。
*
* UPnP
*   AV规范定义的设备
*     UPnP AV MediaServer
*     UPnP AV MediaRender
*   AV服务：
*     CDC(Content Directory Service) -- 内容目录服务，枚举可访问的内容，如视频、音乐盒图片等
*     Connection Manager Service -- 决定媒体内容可以通过何种方式由 UPnP AV MediaServer 传输至 UPnp AV MediaRenderer
*     AV Transport Service -- 控制媒体内容的传输，如播放、停止、暂停、查找等
*     Rendering Control Service -- 控制以何种方式播放内容，如 音量、静音、亮度等
*   Printer 架构
*     定义了打印设备和关联的控制点应用之间的交互模型
*************************************************************************************************************************/


namespace FTL
{
}
#endif //FTL_DLNA_H

#ifndef USE_EXPORT
#  include "ftlDLNA.hpp"
#endif 