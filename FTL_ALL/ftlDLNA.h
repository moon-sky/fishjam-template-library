#ifndef FTL_DLNA_H
#define FTL_DLNA_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlDLNA.h requires ftlbase.h to be included first
#endif

//TODO: 在android系统中开发DLNA -- http://blog.csdn.net/gebitan505/article/details/19497545
//DLNA 的设备发现和控制层使用的UPnP协议标准： http://upnp.org/sdcps-and-certification/standards/device-architecture-documents/
//UPnP协议编程实践(一) http://www.ibm.com/developerworks/cn/linux/other/UPnP/part1/
//基于CyberGarage库的dlna开发 --  http://blog.csdn.net/lancees/article/details/8477513
//DLNA开发资料 -- http://download.csdn.net/download/geniuseoe2012/4969961


/*************************************************************************************************************************
* 多屏互动
*   核心技术 -- 把各个屏幕或者把各个设备关联起来的协议，主要是 Airplay, DLNA, Miracast
*   快播针对视频业务，开发了 SmartPlay 协议，方便的打通了各个设备
*************************************************************************************************************************/

/*************************************************************************************************************************
* 开源实现库
*   MiniDLNA -- https://github.com/azatoth/minidlna
*   SlickDLNA -- https://github.com/KernelCrap/android-dlna
*   UPnP SDK(基于其开发出DLNA) -- http://upnp.org/sdcps-and-certification/resources/sdks/
*     有很多选择，可支持 C,C++,Java等，其中比较好的：
*       Plutinosoft -- 
*       CyberGarage(海思也用这个) -- 
*     Android平台上: NDK下面编译出 jni库 => 应用层开发
*       注意：Android平台上的 MediaPlayer 对视频的处理能力相当的弱，若想对各种视频有良好的支持，需要移植视频播放器，
*              如：ffmpeg、gstream、vlc 等
*************************************************************************************************************************/


/*************************************************************************************************************************
* Airplay -- 苹果所有的操作系统都支持该协议。苹果原来是DLNA的成员，后退出自立门户，协议和DLNA比较类似。
*   新增了：镜像功能(手机上的内容直接显示在Apple TV上，比如游戏)，双屏游戏体验
*************************************************************************************************************************/

/*************************************************************************************************************************
* DLNA(Digital Living Network Alliance) -- 数字生活网络联盟，索尼、英特尔、微软等发起。http://www.dlna.org/
*   目标是共享室内和户外的可以互联的消费电子(CE)，PC及移动设备等构成的优先和无线网络，促进数字多媒体内容和服务的共享。
*   只适用于音频、视频、图片推送，将多媒体文件通过无线的方式传送到电视机或投影仪上面。
*
* DLNA 联网设备交互性指导方针(Networked Device Interoperability Guidelines -- 500$), 现在已有 1.5 版本
*   详述了可供搭建平台和软件基础的可以交互的基本模块。
*
* 设备类型(Device Category)和角色(Roles)：
*   HND(Home Network Devices) -- 指家庭设备，具有比较大的尺寸及较全面的功能
*     DMS(Digital Media Server) -- 数字媒体服务器，提供媒体获取、记录、存储和输出功能。DMS总包含DMP的功能，
*       存储媒体文件及向家庭网络中的DMP、DMR提供媒体数据的列表和内容。如 PC、机顶盒等
*     DMP(Digital Media Player) -- 数字媒体播放设备，能从DMS/M-DMS上查找并获取媒体内容并播放和渲染显示，如智能电视等
*     DMR(Digital Media Renderer) -- 数字媒体渲染设备，通过其他设备配置后，可以播放从DMS上的内容。与DMP的区别在于没有查找、浏览媒体的功能，如 显示器、音响等
*     DMC(Digital Media Controller) -- 查找DMS的内容并建立DMS与DMR之间的连接并控制媒体的播放，如遥控器
*     DMPr(Digital Media Printer) -- 数字媒体打印机，如 网络打印机，一体化打印机
*   MHD(Mobile Handled Devices) -- 手持设备，功能相对简化一些，支持的媒体格式也不同。
*     M-DMS(Mobile DMS), M-DMP, M-DMC
*     M-DMD(Mobile Digital Media Downloader) -- 移动多媒体下载设备。如随身音乐播放器，车载音乐播放器和智能电子相框等
*     M-DMU(Mobile Digital Media Uploader) -- 移动多媒体下载设备。如摄像设备和手机等。
*   HID(Home Interoperability Devices)| NID(Networked Infrastructure Devices) -- 联网支持设备。为其他设备种类间的互操作提供支持
*     MIU(Media Interoperability Unit) -- 媒体交互设备，提供媒体格式的转换以支持各种设备(HND 和 MHD)需要
*     M-NCF(Mobile Network Connectivity Function) -- 移动网络连接功能设备，提供各种设备接入移动网络的物理介质，DLNA的希望是全部实现无线化。
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
* 架构(DLNA Architecture)
*   1.媒体格式(Media Formats)：规定了如何对需要传输和播放的内容进行编码，对每一个设备种类，DLNA媒体格式模型
*     针对3类媒体类型(图片、音频、视频)定义了一组强制<>和可选[Optional]的媒体格式。
*       Home Devices -- Imaging<JPEG>[GIF,TIFF,PNG], Audio<LPCM>[MP3,AAC,WMA9,AC-3等], Video<MPEG2>[MPEG1,MPEG4,WMV9]
*       Mobile/Handheld Devices -- Imaging<JPEG>[GIF,TIFF,PNG], Audio<MP3,MPEG4 AAC>[MPEG4,WMA等], Video<MPEG4 AVC>[VC1,H263,MPEG4 AVC等]
*     如果DMS/M-DMS 设备自身的媒体格式可以被接受设备所支持的情况下，则直接传送；否则必须转换成接收端能支持的媒体格式。
*   2.媒体传输(Media Transport)模式：
*     a.流传输方式 -- 当媒体内容包含内置的需要匹配的时序且立刻被DMP/M-DMP/DMR 渲染播放时，或媒体内容是以固定的速率实时生成时。
*     b.交互传输方式 -- 当不包含内部时序信息的媒体内容被用来进行用户实时交互时，如需要立刻显示给用户图片；
*     c.后台传输方式 -- 当媒体内容不需要立即渲染播放或用户对其传输的优先级不要求的时候(主要用于M-DMD/M-DMU进行上传、下载非实时生成的内容时)
*     DLNA设备必须支持以 ★HTTP★ 作为基本传输方式传输多媒体内容，RTP作为可选的媒体传输方式，提供了用来优化媒体传输的System Usage扩展性
*   3.媒体管理(Media Management)：实现设备和应用对家庭网络中的媒体内容的识别、管理、分发和记录，由 UPnP AV Architecture 和 UPnP Printer Architecture 两个文档说明如何处理这些应用需求，
*     成为DLNA设备的媒体管理解决方案。
*   4.设备发现和控制：设备必须能够发现家庭网络上存在的其他设备和服务，并识别、配置和控制他们的功能和相应的能力。
*     使用 UpnPTM 控制设备协议架构(DCP Framework) 作为设备发现和控制的解决方案，来处理所有这些需求来简化网络设备的互联。
*   5.Network Connectivity(网络互联) -- 基于IPv4(/v6?)的各种连接，
*     802.3(Ethernet), 802.11a/g(WiFi), 802.11n(OFDM和MIMO), 802.15(Bluetooth)等,
*     通过 QoS优化共享网络的资源，使得视频流和音频流播放流畅
*
*   1.数字版权管理、内容保护 -- 包括链路保护(必须支持DTCP-IP，可选支持WMDRM-ND)和DRM。DLNA必须能保护商业数字内容不被未授权的使用和拷贝。
*************************************************************************************************************************/

/*************************************************************************************************************************
* Miracast -- 由Wi-Fi联盟于2012年所制定，以Wi-Fi直连为基础的无线显示标准，支持此标准的设备可通过无线方式分享视频画面。
*   缺点：技术较新，支持设备较少，技术不成熟。
* 
*************************************************************************************************************************/


/*************************************************************************************************************************
* UPnP(Universal Plug and play) -- 通用即插即用(http://upnp.org/)，是DLNA的核心
*   UPnP标准：http://upnp.org/resources/upnpresources.zip
* 
*   UPnP是一种用于 PC 机和智能设备(或仪器)的常见对等网络连接的体系结构，尤其是在家庭中。
*     以Internet 标准和技术(TCP/IP、HTTP和XML)为基础，
*       1.使这样的设备彼此可自动连接和协同工作。
*         示例：Win7打开网络服务的UPnP选项，并在家庭网络中共享一个装着视频的文件夹，则SmartTV可以自动找到该文件夹，并支持播放。
*       2.给家庭网内的devices做自动的NAT和端口映射(Port Mapping)--因为家庭网络里没有那么多IP。
*   文档：
*     UPnP Device Architecture 1.0 -- 说明设备是怎样通过UPnP来相互发现和控制，以及传递消息的
* 
*   AV规范定义的设备
*     MS(UPnP AV MediaServer)
*     MR(UPnP AV MediaRender)
*   AV服务：
*     CDS(Content Directory Service) -- 内容目录服务，枚举并列出可访问的内容，如视频、音乐盒图片等
*     CMS(Connection Manager Service) -- 决定媒体内容可以通过何种方式由 UPnP AV MediaServer 传输至 UPnp AV MediaRenderer
*     AVT(AV Transport Service) -- 控制媒体内容的传输，如播放、停止、暂停、查找等
*     RCS(Rendering Control Service) -- 控制以何种方式播放内容，如 音量、静音、亮度等
*   这个是什么？
*     SRS(Scheduled Recording Service) -- 
*   Printer 架构
*     定义了打印设备和关联的控制点应用之间的交互模型
*
* UPnP的工作过程
*   0.寻址(Addressing) -- 每个设备都应当是DHCP的客户,当设备首次与网络建立连接后,得到一个IP地址，一般来说路由器已有该功能
*   1.发现(Discovery) -- 当一个设备被添加到网络后，UPnP的发现协议允许该设备向网络上的Control Points(CPs)通知(advise)自己拥有的服务。
*     同理，当一个CP被添加到网络后，UPnP发现协议允许该CP搜索网络上可用的设备。
*     一般通过SSDP协议组播发送 设备和服务的基本信息(类型，唯一标识符，当前状态参数等)。
*     组播地址为“HOST: 239.255.255.250:1900” ? Response 是UDP单播
*   2.描述(Description) -- CP向设备发送请求信息，返回XML格式的描述(device + service),其中有更详细信息的URL
*   3.控制(Control) -- 通过向设备描述部分给出的Control URL发送控制信息来控制Device，并根据反馈进行处理。
*     沟通信息按照SOAP的格式来写(现在的版本是 SOAP 1.1 UPnP Profile)
*   4.事件(Eventing) -- 在服务进行的整个时间内，如状态发生了改变，可以产生一个事件，并向整个网络进行多播(multicast);
*     CP也可以事先向事件服务器订阅(subscribe)事件信息，保证将该CP感兴趣的事件及时准确地单播传送过来(unicast)。
*     事件的订阅和推送使用的协议是 GENA。
*   5.表达(Presentation) -- 遥控器的UI？
*     只要得到了设备的URL，就可以取得该设备表达的URL，取得该设备表达的HTML，然后可以将此HTML纳入CP的本地浏览器上。
*
* UPnP AV(Audio/Video) Architecture -- 提供了交互的机制、模型，但并没有规定采用何种技术来实现(具有广泛的适应性)，
*   AV架构对CP(Action通常由CP发出)的功能要求有10条：
*     1.发现AV设备
*     2.获得所需的内容列表
*     3.获得渲染器支持的协议/格式
*     4.比较/匹配协议/格式
*     5.配置服务器/渲染器
*     6.选择所需的内容
*     7.开始内容传输
*     8.调整渲染参数
*     9.重复：选择下一个内容
*     10.断开服务器和渲染器连接。
*   CP可以是MediaServer、MediaRenderer、RemoteControl，根据CP的角色，归纳出下面三种常见的AV设备交互模型：
*     2-Box Pull Model -- CP是MediaRenderer(如智能手机)，主动向MediaServer索取媒体内容，获得内容之后播放媒体
*     2-Box Push Model -- CP是MediaServer(如PC)，CP主动向MediaRenderer推送(push)媒体。
*     3-box model -- CP仅仅作为一个遥控器。也分为pull和push两种方式。
*       a -- pull方式时,CP向Renderer发送Server及Server上所需媒体内容的URL，让Renderer去取
*       b -- push方式时，CP向Server发Renderer的URL，让Server去向Renderer推送媒体内容
*************************************************************************************************************************/



namespace FTL
{
}
#endif //FTL_DLNA_H

#ifndef USE_EXPORT
#  include "ftlDLNA.hpp"
#endif 