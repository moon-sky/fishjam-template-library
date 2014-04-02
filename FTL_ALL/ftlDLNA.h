#ifndef FTL_DLNA_H
#define FTL_DLNA_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlDLNA.h requires ftlbase.h to be included first
#endif

//TODO:
//  1.AllCast的 app中VCR控制 -- 什么意思?

//开发注意:
//0.开发语言:C/C++(以平台独立的代码来编程); Windows(DLL); Android(.so); iOS(Framework)
//1.需要支持 Naver Media Player 和 NaverTV，且平台为 Windows/Android/IOS 等
//  在各个平台上使用的多媒体库 -- 获取媒体 大小、长度 等信息
//2.支持多种设备和协议 -- DLNA/AirPlay/ChromeCast 等协议 和 SmartTV/XBox/ChromeCast/AppleTV 等设备
//  重点是协议，设备只需要进行详细测试
//3.支持的媒体类型 -- Photo/Audio/Video
//4.支持的特性 -- 本地媒体(DMS)、远程媒体(DMC)，本地播放(DMR)
//                 local 文件, HTTP URL, HLS VOD URL, HLS LIVE URL
//5.格式 -- 不转换源文件的格式,直接传送。只有 HLS Source 需要转换为可以播放的格式
//6.网络传输协议 -- 必须支持 HTTP 和 HLS，并且需要留下扩展的接口


//DLNA 的设备发现和控制层使用的UPnP协议标准： http://upnp.org/sdcps-and-certification/standards/device-architecture-documents/
//UPnP协议编程实践(一) http://www.ibm.com/developerworks/cn/linux/other/UPnP/part1/
//基于CyberGarage库的dlna开发 --  http://blog.csdn.net/lancees/article/details/8477513
//DLNA开发资料 -- http://download.csdn.net/download/geniuseoe2012/4969961
//网络相关的一些知识 -- http://www.h3c.com.cn/MiniSite/Technology_Circle/Net_Reptile/The_Five/

/*************************************************************************************************************************
* 调试时的协议跟踪
*   1.UDP -- 查找、发现媒体服务器时, 使用 SSDP
*   2.HTTP -- 控制交互，发送 Action 时，使用 SOAP
*   3.RTSP(?) -- 可选的媒体传输协议
*
* DLNA的媒体播放过程(控制点从 MediaServer 上选择文件，在 MediaRender 上播放)
*   1.通过SSDP找到 MediaServer 和 MediaRender
*     示例:a.使用 M-SEARCH 查找; b.接收到设备定时发送的 ssdp:alive 
*   2.使用 ContentDirectory::Browse 或 Search 从 MediaServer 上浏览媒体信息，返回值为 DIDL-Lite 规范的XML
*     示例:ObjectID(0);BrowseFlag(BrowseDirectChildren);Filter(*);StartingIndex(0);RequestedCount(0);SortCriteria(空)
*     从中可以获取到 传输协议(protocolInfo), 连接地址URL 等信息
*   3.使用 ConnectionManager::GetProtocolInfo 分别获取Server和Render的传输协议(protocolInfo)和支持的数据格式列表，分为 Source和Sink,
*     返回值为逗号分开的 http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_SM 或 http-get:*:audio/mp3:* 等
*   4.匹配媒体文件和播放器支持都支持的传输协议和数据格式 -- 一般来说，两个都是列表，需要查找最合适的
*   5.通过 ConnectionManger::PrepareForConnection 配置Render，得到 AVTransport::InstanceId, 之后通过该值进行状态管理
*     示例(执行成功后会返回 InstanceId, 该值可通过 ConnectionManger::GetCurrentConnectionIDs 确认)
*          RemoteProtocolInfo(http-get:*:video/x-ms-wmv:*);PeerConnectionManager(空字符串);PeerConnectionID(-1);Direction(Input)
*   6.使用 AVTransport::SetAVTransportURI 向 MediaRender 发送要播放媒体文件的信息
*     示例:
*       InstanceID(前面的返回值);CurrentURI(指定 "Wildlife in HD" 对应的URL);
*   7.通过 AVTransport::play/stop 和 RenderingControl::SetVolume 等来控制媒体播放
*     示例:播放，暂停，声音和亮度调节
*       Seek: Unit(REL_TIME); Target(00:00:05) -- 定位到第5秒
*       GetPositionInfo -- 可以获得总长度、当前位置等
*   8.播放完毕后需要通过 ConnectionManager::ConnectionComplete 关闭连接
*************************************************************************************************************************/

/*************************************************************************************************************************
* 多屏互动
*   核心技术 -- 把各个屏幕或者把各个设备关联起来的协议，主要是 Airplay, Chromecast, DLNA, Miracast, [ Wi-Fi Direct, WiDi ?]
*   快播针对视频业务，开发了 SmartPlay 协议，方便的打通了各个设备
* 常见软硬件
*   AllCast -- Android上的软件，对设备的覆盖率也十分高(支持Chromecast, SmartTV, XBox, AppleTV 等)
*   EzCast -- 类似ChromeCast的硬件
*   AllShare(三星), Smart Share(LG)
*
* 注意：
*   1.媒体服务器为了缩短视频播放时的缓冲时间，通常会把长视频分成小段，需要减少分段视频之间切换造成的停滞时间而平滑过渡
*   2.注意需要考虑"AV follow me"的问题 -- 视频在电视和手机上能自由切换(来回切换时需要续点播放)
*************************************************************************************************************************/

/*************************************************************************************************************************
* 开源实现库 -- 提供UPnP栈
*   MiniDLNA -- https://github.com/azatoth/minidlna
*   SlickDLNA -- https://github.com/KernelCrap/android-dlna
*   UPnP SDK(基于其开发出DLNA) -- http://upnp.org/sdcps-and-certification/resources/sdks/
*     有很多选择，可支持 C,C++,Java等，其中比较好的：
*       Cling -- http://4thline.org/projects/cling, Android上比较好的开发工具，支持客户端和服务器端。许可协议为LGPL或CDDL
*       CyberGarage/CyberLink(海思也用这个) -- http://www.cybergarage.org/net/upnp,
*         出现时间比较早，支持C/C++/Java/ObjectC 等多平台(每种平台有单独的实现)，多语言支持不好，性能有问题
*         使用BSD协议，支持商业开发。
*       Intel 的 openTools -- http://opentools.homeip.net/
*       Platinum -- 跨平台的C++实现,支持 Windows,Linux,Mac,iPhone,Android 等，XBMC即使用该库。双lincense
*         http://blog.csdn.net/lancees/article/details/9865411
*       Plutinosoft -- 
*     Android平台上: NDK下面编译出 jni库 => 应用层开发
*       注意：Android平台上的 MediaPlayer 对视频的处理能力相当的弱，若想对各种视频有良好的支持，需要移植视频播放器，
*              如：ffmpeg、gstream、vlc 等
*************************************************************************************************************************/


/*************************************************************************************************************************
* Airplay -- 苹果所有的操作系统都支持该协议。苹果原来是DLNA的成员，后退出自立门户，协议和DLNA比较类似。
*   新增了：镜像功能(手机上的内容直接显示在Apple TV上，比如游戏)，双屏游戏体验
*   官方SDK -- https://developer.apple.com/library/ios/documentation/AudioVideo/Conceptual/AirPlayGuide/Introduction/Introduction.html
*************************************************************************************************************************/

/*************************************************************************************************************************
* Chromecast -- https://developers.google.com/cast/
*   Google推出的电视棒产品(HDMI+WiFi)。其上运行简化的Chrome操作系统，插在HDMI接口上，通过手机控制？
*     流程:1.向Google服务器发送URL;2.服务器获取资源(如视频、网页);3.将资源传回Chromecast;4.Chromecast将资源输出到电视上
*   开发：Google Cast SDK
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
*
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
*
*   
*************************************************************************************************************************/

/*************************************************************************************************************************
* Miracast(WiFi Display) -- 由Wi-Fi联盟于2012年所制定，以Wi-Fi直连为基础的无线显示标准，支持此标准的设备可通过无线方式分享视频画面。
*   缺点：技术较新，支持设备较少，技术不成熟。
* 
*************************************************************************************************************************/

/*************************************************************************************************************************
* Platinum UPnP SDK -- http://www.plutinosoft.com/platinum
*   优点:
*     1.跨平台(通过 Neptune 库实现)的统一代码，
*   缺陷:
*     1.功能不全 -- 比如缺少 ConnectionManger::PrepareForConnection
*     2.似乎不支持IPV6
*   编译：Neptune 和 Platinum 都编译成静态库
*     1.Windows:Platinum\Build\Targets\x86-microsoft-win32-vs2008\Platinum.sln
*     2.Android JNI: NDK + Cygwin
*       2.1 设置 ANDROID_NDK_ROOT 环境变量;
*       2.2 更改 Neptune\Source\Core\NptConfig.h 文件，在 __CYGWIN__ 的配置中加入 #define NPT_CONFIG_HAVE_GETADDRINFO
*           否则会报告无法链接 NPT_NetworkNameResolver::Resolve 的错误
*       2.3 进入 Platinum 目录后执行 scons target=x86-unknown-cygwin build_config=Debug, 真正的Android时应该 arm-android-linux
*           可选的target看在 Neptune\Build\Boot.scons 中查看(也可通过 scons --help 查看) 
*       2.4 进入 Platinum/Source/Platform/Android/module/platinum， 执行 ndk-build NDK_DEBUG=1
*************************************************************************************************************************/

/*************************************************************************************************************************
* XBMC(http://xbmc.org/) -- XBOX Media Center (源码 -- git://github.com/xbmc/xbmc.git )
*   开源媒体中心软件(GPL), 最初为XBox设计，现在已可在Linux、OSX、Windows、Android4.0系统运行, 支持 DLNA,Airplay 等协议
*   相关工具的下载(上层目录有每日编译等): http://www2.frugalware.org/mirror/xbmc.org/build-deps/win32/
*   Windows编译( http://wiki.xbmc.org/index.php?title=HOW-TO:Compile_XBMC_for_Windows )：
*     1.环境: VS2010 + DirextX2009_08以后 + Git + Nullsoft + JRE
*     2.project\BuildDependencies\DownloadBuildDeps.bat -- 自动联网下载编译所需的依赖项
*     3.project\BuildDependencies\DownloadMingwBuildEnv.bat -- 下载编译ffmpeg所需的依赖项
*     4.project\Win32BuildSetup\buildmingwlibs.bat -- 编译 ffmpeg 库
*     5.二选一的编译(一般调试开发时选第二个)
*       5.1.project\Win32BuildSetup\BuildSetup.bat -- 直接编译一个打包文件
*       5.2.project\Win32BuildSetup\extract_git_rev.bat -- 编译后会在系统信息窗体中显示git版本
*     6.project\VS2010Express\XBMC for Windows.sln -- VS2010进行编译，选择DirectX版本(OpenGL版本已经被弃用)
*   目录结构分析:
*     addons -- 附加元件，如XBMC的皮肤,屏幕保护,可视化效果等
*     project -- 项目工程文件，主要有
*************************************************************************************************************************/


/*************************************************************************************************************************
* 
* 树型架构：
*   +-Root Device > 物理或逻辑Device > Service > Action(Arguments) 和 Variable
*   +-Control Point
*
* UPnP(Universal Plug and play) -- 通用即插即用(http://upnp.org/)，主要用于实现设备的智能互联互通，是DLNA的核心
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
*     CDS(Content Directory Service) -- 内容目录服务，枚举并列出可访问的内容，如视频、音乐盒图片等，
*       Browser -- 浏览媒体文件
*     CMS(Connection Manager Service) -- 连接管理服务(Server和Render都必须支持)
*     AVT(AV Transport Service) -- 控制媒体内容的传输，如播放、停止、暂停、查找等，智能电视就需要实现这个，
*       GetPositionInfo -- 可以获取到媒体的长度、当前位置等；TODO: Windows Media Service 的 GetMediaInfo 不能获取到长度
*       SetAVTransportURI -- 设置要播放的媒体的URL
*       支持的订阅Event:LastChange(播放状态改变),
*     RCS(Rendering Control Service) -- 控制以何种方式播放内容，如 音量、静音、亮度等
*   这个是什么？
*     SRS(Scheduled Recording Service) -- 
*   Printer 架构
*     定义了打印设备和关联的控制点应用之间的交互模型
*
* UPnP的工作过程
*   0.寻址(Addressing) -- 每个设备都应当是DHCP的客户,当设备首次与网络建立连接后,通过 DHCP Discover + DHCP Offer 得到一个IP地址，
*     一般来说路由器已有该功能。如没有响应，则使用 Auto-IP 完成IP地址的设置 (169.254/169.16)。也可以使用静态配置的IP地址。
*   1.发现(Discovery) -- 当一个设备被添加到网络后，UPnP的发现协议允许该设备向网络上的Control Points(CPs)通知(advise)自己拥有的服务。
*     同理，当一个CP被添加到网络后，UPnP发现协议允许该CP搜索网络上可用的设备。
*     一般通过SSDP协议组播发送 设备和服务的基本信息(类型，唯一标识符，当前状态参数等)。
*     搜索: "HOST: 239.255.255.250:1900" 发 "M-SEARCH",
*     通知: "HOST: 239.255.255.250:1900" 发 "NOTIFY", 如是响应，则使用UDP的单播发送
*     该步骤后可知设备或服务的UPnP类型，UUID和设备描述的URL。
*   2.描述(Description.xml) -- CP向设备发送请求信息，返回XML格式的描述(device + service),其中有更详细信息的URL，通常会多次递归查询，
*     描述文档 -- 主要分设备描述、服务描述等。包括：制造商信息,版本等；可被设备采用的图标的URL地址；设备列表；服务列表等，
*     服务器端，设备需要运行一个标准的HTTP服务(可以是完全的Web服务器也可以是迷你服务器，主要是提供HTTP检索)
*   3.控制(Control) -- 通过向设备描述部分给出的Control URL发送控制信息来控制Device，并根据反馈进行处理。
*     沟通信息按照SOAP的格式来写(现在的版本是 SOAP 1.1 UPnP Profile), 使用HTTP的POST和M-POST命令，传输XML文档
*   4.事件(Eventing) -- 在服务进行的整个时间内，如状态发生了改变，可以产生一个事件，并向整个网络进行多播(multicast);
*     CP也可以事先向事件服务器订阅(SUBSCRIBE)事件信息，保证将该CP感兴趣的事件及时准确地单播传送过来(unicast)。
*     事件的订阅和推送使用的协议是 GENA。
*   5.界面描述(Presentation) -- 遥控器的UI？
*     只要得到了设备的URL，就可以取得该设备表达的URL，取得该设备表达的HTML，然后可以将此HTML纳入CP的本地浏览器上。
*     既允许设备供应商提供基于浏览器的用户界面和编程控制接口，也允许开发人员定制自己的设备界面。
* 
* Control Point和Device交互时使用的协议：
*   SOAP:   Control
*   HTTP:   Description
*   HTTPMU: Notify/Search
*   HTTPU:  Search Response
*   GENA:   Event
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
*
*
* 格式
*   设备类型 -- 描述格式一般为 urn:schemas-upnp-org:device:uuid-device。其中 uuid-device 为UPnP工作委员会定义的标准设备类型。
*     uuid 值为 1 表示:
*     device值为 0 表示:
*     设备制造商也可以指定其他的名字，一般为 urn:domain-name:device:uuid-device，其中 uuid-device为制造商定义的标准设备类型，domain-name字段为设备制造商注册的域名
*     常见设备类型(deviceType):InternetGatewayDevice(一般是路由器等网关设备)
*   服务类型 -- 表示服务的统一资源名，一般格式为 urn:schemas-upnp-org:service:serviceType:version
*     UPnP设备制造商可以指定附加服务，格式为 urn:domain-name:service:serviceType:version
*     常见服务： 扫描仪(scanner),
*
* UPnP在NAT中的应用 
*   用户通过NAT接入网络 + 同时需要使用 BitComment、电骡(eMule)等P2P类软件。UPnP能自动的把P2P软件侦听的端口号映射到公网上，
*     以便公网上的用户能对NAT私网侧发起连接，大大的提高下载效率与速度
*   前提条件：
*     1.NAT网关设备必须打开UPnP功能
*     2.操作系统(如WinXP)必须打开UPnP功能：
*       a.添加删除程序->Windows组件->网络服务->UPnP用户界面;
*       b.Windows防火墙->例外->UPnP框架
*       c.服务-> 启动 "SSDP Discovery Service" 和 “Universal Plug and Play Device Host” 服务
*       然后可在“网络连接”中看到多了Internet网关，等应用程序运行时添加端口后，属性中可以看到具体的信息。
*     3.应用软件必须打开UPnP功能,如 BitComment 中:选项->高级->网络连接->允许使用UPnP自动端口映射
*       然后在"全局统计“标签页中可以看到NAT端口映射已添加(对外IP: TCP监听端口:25118, UDP监听端口:25118)
*   
*************************************************************************************************************************/

/*************************************************************************************************************************
* UPnP协议分析
-------------------------------------------------------------------------------------
|                      UPnP设备制造商定义                                    |
|                          UPnP论坛定义                                       |
|                       UPnP设备定义的结构                                   |
|      SSDP, GENA              ||    SOAP(设备控制,设备描述), GENA(设备事件)     |
|   HTTPU/HTTPMU(设备发现)    ||                                                   |
|                              UDP                                                  |
|                              IP                                                   |
-------------------------------------------------------------------------------------
*  HTTPU/HTTPMU -- HTTP协议的变种,格式沿袭了HTTP协议，但通过UDP而非TCP来承载，并且可用于组播进行通信
*
* SSDP(CyberLink 中的 SSDPPacket):
*   主动通知(NOTIFY * HTTP/1.1\r\n): 设备加入到网络中时，向网络上的控制点告知它提供的服务，并且定期发送
*     HOST:239.255.255.250:1900\r\n -- 表示是广播
*     LOCATION: 根设备的描述URL，如 http://192.168.1.1:2800/InternetGatewayDevice.xml\r\n
*     NT: 服务类型，如 upnp:rootdevice\r\n
*     NTS:ssdp:alive\r\n
*     USN:uuid:aaaabbbb-cccc-dddd-eeeeffffgggghhhh::upnp:rootdevice\r\n -- 表示不同服务的统一服务名，标示相同类型服务能力
*   查询(M-SEARCH * HTTP/1.1\r\n): 控制点加入到网络中时，寻找网络上感兴趣的设备,对应 SSDPPacket::isDiscover()
*     HOST: 239.255.255.250:1900\r\n -- 多播地址
*     MAN:"ssdp:discover"\r\n -- 固定格式，必须有引号，没有参数，表示查询
*     MX:3\r\n -- 设置设备响应最长等待时间，设备响应在0和这个值之间随机选择响应延迟的值
*     ST: 搜索目标(Search Target)，可选类型如下：
*         ssdp:all -- 所有设备和服务
*         upnp:rootdevice -- 仅搜索网络中的根设备
*         uuid:device-UUID -- 查询UUID标识的设备
*         urn:schemas-upnp-org:device:device-type:version -- 查询device-Type字段指定的设备类型，设备类型和版本由UPNP组织定义
*         urn:schemas-upnp-org:service:service-type:version -- 查询service-Type字段指定的服务类型，服务类型和版本由UPNP组织定义
*   响应(HTTP/1.1 200 OK) -- 设备发现自己满足控制点搜索的目标，进行相应
*     EXT:\r\n -- 空节点，向控制点确认MAN头域已经被设备理解
*     LOCATION:http://xxxx -- 包含根设备描述的URL地址
*     SERVER:Wireless N Router WR845N, UPnP/1.0\r\n -- 包含操作系统名，版本，产品名和产品版本信息。
*     ST: -- 本响应对应的搜索目标，内容和意义与查询请求的相应字段相同
*     USN: -- uuid:xxxx, 
*   NTS(?) -- 生存期过程中的消息?
*     ssdp:alive --
*     ssdp:byebye
*     upnp:propchange
*
*   控制报文: 控制点先发送一个控制行为请求，要求设备开始服务，然后再按设备的URL发送相应的控制消息
*     1.POST /EmWeb/UPnP/Control/4 HTTP/1.1 -- 对目的地址，POST Soap 格式的命令
*       SOAPAction: "urn:schemas-upnp-org:service:WANIPConnection:1#GetSpecificPortMappingEntry"
*       <soap:Envelope> + <soap:Header> + <soap:Body>
*
*   事件订阅和通知 -- 放在XML中，使用GENA格式
*     NT: upnp:event -- 
*     SEQ
*
*   TODO:
*     
*************************************************************************************************************************/

/*************************************************************************************************************************
* UPnP的常见实例分析
*   1.AllShare
*     设备: 
*       <deviceType>urn:schemas-upnp-org:device:MediaServer:1</deviceType>
*       服务: 
*         a.<serviceType>urn:schemas-upnp-org:service:ConnectionManager:1</serviceType> 
*         Action列表(3个):
*           a1.GetCurrentConnectionIDs
*           a2.GetCurrentConnectionInfo
*           a3.GetProtocolInfo
*         b.<serviceType>urn:schemas-upnp-org:service:ContentDirectory:1</serviceType>
*         Action列表(31个):
*           b1.Browse
*           b2.GetFeatureList -- 
*   2.Windows7 中的 "SSDP Discovery" + "Windows Media Player Network Sharing Service"
*     开启方式:
*       a.启动对应的服务
*       b.控制面板\网络和 Internet\网络和共享中心 中设置当前网络的连接为家庭网络(让设备可以安全的互相访问),
*         "共享密码"可在"家庭组"中查看; 家庭组中,选中"将我的图片、音乐和视频输出到我的家庭网络上的所有设备";
*         "网络和共享中心\媒体流选项\流媒体选项"中允许设备访问
*       c.Windows Media Player -- "媒体流"菜单中勾选"允许远程控制我的播放器"和"自动允许设备播放我的媒体",
*         并保持程序的运行。
*   3.支持DLNA的三星智能电视
*     设备:
*       <deviceType>urn:schemas-upnp-org:device:MediaRenderer:1</deviceType>  -- 标准的Render，一般手机共享播放时的搜索目标就是这个
*       服务:
*       <deviceType>urn:samsung.com:device:MainTVServer2:1</deviceType>
*       <deviceType>urn:samsung.com:device:MainTVServer2:1</deviceType>
*************************************************************************************************************************/

/*************************************************************************************************************************
* Cling -- Android 上
*  限制:
*    1.(TODO:可能只是模拟器的问题)Android不支持接收UDP组播,但可以发送UDP组播。因此，可以发现正运行的设备，但不能发现搜寻后新开启的设备。
*  主要接口和类
*    interface AndroidUpnpService -- 
*************************************************************************************************************************/

/*************************************************************************************************************************
* CyberLink () -- 优点：简单；缺点：效率低，内部使用string(非UTF-8), 功能不完善，有较多bug
*   默认情况下同时支持 IPV4/IPV6, 可通过 UPnP::SetEnable() 函数控制
*   C++ 版本
*     1.源码: CyberLinkForCC + HttpEngineForCC + expat
*     2.编译:
*       a. linux: ./boostrap -enable-libxml2<CR> ./configure<CR> make
* 
*   ControlPoint的回调接口(需要调用对应的 addXxxxListener 方法注册后才能生效):
*     DeviceChangeListener -- 有 deviceAdded/deviceRemoved 方法，可以知道 Device 的增减
*     EventListener -- 有 eventNotifyReceived 方法，可以接收到注册过的事件通知(变量+值)
*     NotifyListener -- 有 deviceNotifyReceived 方法，可以接收到设备的SSDP通知消息
*     SearchResponseListener -- 有 deviceSearchResponseReceived 方法，可以接收到对搜索进行相应的SSDP消息
*   
* 线程:
*   HttpServer -- 本地的Http服务器
*   SSDPNotifySocket -- 从 HttpMusocket 接收
*   SSDPSearchResponseSocket -- 
*   Disposer -- 每60秒循环一次，移除超时设备(removeExpiredDevices)
*************************************************************************************************************************/

/*************************************************************************************************************************
* Intel openTools
*   Device Builder -- 代码生成工具，
*   Device Sniffer -- SSDP调试工具，监听和显示网络上的所有SSDP信息
*   Device Spy -- 是一个 Control Point，在网络上搜索所有设备，提供调用方法和服务的通用方法
*************************************************************************************************************************/

/*************************************************************************************************************************
* Android 平台开发
*   利用NDK将SDK编译出JNI，
*************************************************************************************************************************/

namespace FTL
{
}
#endif //FTL_DLNA_H

#ifndef USE_EXPORT
#  include "ftlDLNA.hpp"
#endif 