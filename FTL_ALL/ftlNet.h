#ifndef FTL_NET_H
#define FTL_NET_H

#pragma once

/*************************************************************************************************************************
* 已读例子
*   netds\http\AsyncServer(未看完，需要先看线程池、IO完成端口) -- 使用 IO完成端口 + 线程池 实现的异步HttpV2服务器
*   netds\http\httpauth -- HttpV2 服务器的身份认证流程(简单的NTLM)，调试时参数 http://localhost:80/
*   netds\http\HttpV2Server -- HttpV2 服务器(Post时无法读取上传的文件内容？)
*   netds\http\server -- 使用 winhttp 实现的简单 HTPPV1 服务器，支持GET和POST
*   netds\uri -- 演示创建并显示IUri接口属性的用法(需要定义 _WIN32_IE=0x0700，且使用 7.0 以上的SDK)
*   netds\winsock\mcastip -- 多播，有两个版本，分别使用 setsockopt 和 WSAJoinLeaf 实现
*   web\Wininet\CacheEnumerate -- 使用Wininet的Cache API枚举、删除URL相关的内容(-d 参数很危险，可能会把cookie删掉)
*   web\Wininet\httpauth -- 通过HTTP访问Web页面时，检测是否需要认证信息(代理、网页等) -- 不过没有测试出结果
*************************************************************************************************************************/

/*************************************************************************************************************************
* POST上传文件时的网络数据
* 注意：PostArgument 的参数名 和 参数值 之间是两个 <CRLF>，文件的信息(路径/Content-Type)和文件内容之间也是两个 <CRLF>
*       
*   Command: POST
*   URI: /api/file/upload.php
*   {RequestHeader} -- 
*     Content-Type: multipart/form-data; boundary=[Boundary]<CRLF> 
*     Content-Length: [总长度]<CRLF>
*     User-Agent:FTL Translate<CRLF>
*     Host: upload.mytest.com <CRLF>
*     Cookie: xxxxx <CRLF>
*       其他如 Accept、Cache-Control等 <CRLF>
*     <CRLF> -- 最后一项后还有一个 <CRLF> 表示 RequestHeader 的结束, 实际上这个地方多几个 <CRLF> 似乎也没有问题，只要长度计算正确就行
*   [PostArgument]
*     --[Boundary]<CRLF>Content-Disposition: form-data; name="参数名"<CRLF><CRLF>参数值<CRLF>
*   [LocalFile]
*     --[Boundary]<CRLF>Content-Disposition: form-data; name="file"; filename="文件路径"<CRLF>
*                       Content-Type:application/octet-stream<CRLF>  -- 注意：本项可选
*                 <CRLF>文件内容<CRLF>
*   [结束]
*     --[Boundary]--<CRLF>
* 
* 总长度计算方式: RequestHeader + PostArgument + 文件大小
*************************************************************************************************************************/

/*************************************************************************************************************************
* Http 下载性能测试(文件大小：12M) -- 有两个问题：1.下载 和 写文件在一起了
*   系统下载Buffer       ReadBuffer         时间
*      4K(默认)             64K              42S
*      4K(默认)             16K              33S
*      4K(默认)             4K               29S
*      4K(默认)             0K               22S
*
*      16K                  0K               24S
*      16K                 16K               28S
*
*      64K                 0K/4K             49S
*************************************************************************************************************************/

#pragma TODO(wsock32.lib 和 ws2_32.lib 的区别)
//CHttpFile 中有一个 m_pbWriteBuffer 来缓冲需要发送的数据, Write 时并不一定会真正的发送出去
//AfxParseURL -- MFC中解析，有没有ATL版本

//? InternetConnect + HttpOpenRequest(Get/Post 等) == InternetOpenUrl(可以直接处理 URL，简单些，但只能用于HTTP URL?)

#ifndef FTL_BASE_H
#  error ftlNet.h requires ftlbase.h to be included first
#endif

#include <list>
#include "ftlthread.h"
#include "ftlThreadPool.h"
#include "ftlSharePtr.h"
#include "ftlCom.h"
#include "ftlBuffer.h"

//目前两个版本有冲突，可以通过删除 winhttp.h 中冲突的部分，并将改后的文件放在工程中的方法来解决
#ifdef USE_WIN_HTTP
#  include <winhttp.h>	//加强版，通常编写需要 特别功能的客户端(如？) 和 服务器
#else
#  include <WinInet.h>	//基础版，通常编写客户端，抽象了Gopher，FTP，HTTP协议
#endif
#include <Iphlpapi.h>
#include <ws2tcpip.h>

#include <atlbase.h>
#include <atlstr.h>

//默认的网络数据BufferSize
#ifndef INTERNET_BUFFER_SIZE
#  define	INTERNET_BUFFER_SIZE	4096
#endif //INTERNET_BUFFER_SIZE

/*************************************************************************************************************************
* RFC -- http://www.rfc-editor.org/info/rfcXxx
*  1889/3550 -- RTP(Real-time Transport Protocol)
*  2616 -- HTTP(Hypertext Transfer Protocol)
*  6443 -- RFB(Remote Framebuffer Protocol), 通常用于远程控制

* Fiddler(http调试代理) -- 记录并检查所有你的电脑和互联网之间的http通讯，查看所有进出数据
* Httpwatch/Fiddler2 -- 专门监控Http传输的工具?
* Wireshark(网络抓包工具) -- 过滤：Capture->Options->Capture Filter->HTTP TCP port(80) 
* IECookiesView(www.nirsoft.net): 查看编辑Cookie
* netstat -- 系统提供的查询监听、绑定等信息的工具
*
* CAsyncSocketEx -- 代替MFC::CAsyncSocket的异步Socket类，可以通过从 CAsyncSocketExLayer 继承子类并AddLayer(xxx)
*   来支持代理(CAsyncProxySocketLayer) 和 SSL(CAsyncSslSocketLayer) 等, Layer通过链表结构保存，可支持多个。
*   http://www.codeproject.com/internet/casyncsocketex.asp
*   注意：1.目前代理实现中只有 SOCKS5/HTTP11 支持 用户名/密码 ?
*         2.其中重用时 closesocket 可能有Bug? MFC中的版本有 KillSocket 静态方法
*         3.在其原始实现里，每一个有CAsyncSocketEx的线程都会关联一个CAsyncSocketExHelperWindow，可以管理 1~N 个CAsyncSocketEx，
*           通过其静态的 m_spAsyncSocketExThreadDataList 链表变量管理(表面上是为了性能，但可能是细节上的过度优化?--数据传递比线程同步花的时间更多)
*         4.pProxyUser 等字符串数组的释放上有问题，应该是 delete []
*
* AtlUtil.h 中有不少辅助类
*************************************************************************************************************************/


/*************************************************************************************************************************
* 通信软件要考虑的：服务的初始化和分布,并发控制,流控制,错误处理,事件循环继承,容错等
* 设计上考虑无连接和面向连接协议时，主要涉及：延迟(latency)、可伸缩性(scalability)、可靠性(reliability)
*   无连接协议 -- 面向消息，每条消息独立寻址和发送，常用的有UDP和IP，常用于多媒体直播等允许一定程度数据丢失，但要求实时性的场合
*   面向连接协议 -- 提供可靠、有序、不重复的发送服务，为保证可靠性采用了对传送的每个数据字节编号、校验和计算、数据确认(ACK)、
*     超时重传、流量控制(通过滑动窗口由接受方控制发送方发送的数据量) 等一系列措施，常用于电子邮件等不允许数据丢失的场合。
*     还要作出如下设计选择：
*       数据成帧策略--面向消息(如 TP4和XTP)；面向字节流(如 TCP)，不保护消息边界
*       连接多路复用策略--多路复用(多个线程共享TCP连接，难于编程)；非多路复用(每个线程使用不同的连接，同步开销小，但伸缩性不好)
*
*   同步请求/应答协议 -- 每一个请求必须同步地收到一个应答，才能发送下一个请求，实现简单，适用于请求结果决定后续请求的情况；
*   异步请求/应答协议 -- 可以将请求连续发送至服务器，无需同步等待应答。能有效利用网络，大大提高性能，
*  
* IP地址(A~E ?)
*  
* 端口号
*   分为下面这三类：“已知”端口、已注册端口、动态和（或）私用端口。
*     0 ~ 1023 由 IANA(互联网编号分配认证)控制，是为固定服务保留的。
*     1024 ~ 49151 是 IANA 列出来的、已注册的端口，供普通用户的普通用户进程或程序使用。
*     49152 ~ 65535 是动态和（或）私用端口。
*
* 代理(Proxy) -- 分为 SOCKS4/5, HTTP/1.1 等
*************************************************************************************************************************/

/*************************************************************************************************************************
* TCP粘包(Stick Package)
*   发送方发送的若干包数据到接收方接收时粘成一包，接收缓冲区中后一包数据的头紧接着前一包数据的尾，可能造成粘在一起的数据包不完整，
*   若传输的数据为带结构的数据，需要做分包处理(如果是文件传输等连续数据流，就不必分包)
*   即：若双方建立长连接, 而且需要在连接后一段时间内发送不同的数据结构, 则需要考虑粘包的问题
*       TCP作为流，是远远不断的到目的地，接收方必须组包；UDP作为消息或数据报，一定是整包到达接收方，不会出现粘包问题。
*   
* 产生原因（ 注意：短连接时不可能出现粘包现象？如 Http 协议 ？)
*   发送方：TCP为提高传输效率，会收集到足够多数据后才发送(Nagle算法[、延迟应答?] 等)
*   接收方：接收方用户进程不及时从系统接收缓冲区接收数据，下一包数据到来时就接到前一包数据之后
*   
* 较好的对策：
*   接收方创建一预处理线程针对接收到的数据包进行预处理,将粘连的包分开
*************************************************************************************************************************/

/*************************************************************************************************************************
* ARP(Address Resolution Protocol)--地址转换协议，用于动态地完成IP地址向物理地址的转换。
* BOOTP(BooTstrap Protocol)--可选安全启动协议，使用UDP消息，提供一个有附加信息的无盘工作站，通过文件服务器上的内存影像来启动。
* Cookie(Cookies) -- 网站为了辨别用户身份、进行session跟踪而储存在用户本地终端上用分号(";")分开的键值对数据（通常经过加密）。
*   定义为 RFC2109（已废弃）和 RFC2965。最典型的应用是自动登录、购物车等
*   服务器端生成 -> 发送给UserAgent(浏览器) -> 浏览器保存成文本 -> 下次访问时发送 -> 服务器验证
*   生存周期 -- Cookie生成时指定的Expire，超出周期Cookie就会被清除。单位为？生存周期设置为"0"或负值表示关闭页面时马上清除。
*   安全问题：1.识别不精确。在同一台计算机上使用同一浏览器的多用户群，cookie不会区分他们的身份（除非使用不用的用户名登录OS）
*             2.网页臭虫的图片(透明且只有一个像素-以便隐藏)，将所有访问过此页面的计算机写入cookie，方便网站发送垃圾邮件
* DHT(Distributed Hash Table 分散式杂凑表) -- 可以使P2P网络完全不使用服务器，典型的有eMule中使用的 Kad，
* ICMP(Internet Control Message Protocol)--互连控制消息协议，主要用来供主机或路由器报告IP数据报载传输中可能出现的不正常情况。
* IP(Internet Protocol)--互联协议，规定了数据传输的基本单元(报文分组)以及所有数据在网际传递时的确切格式规范。
* LANA(LAN Adapter)Number -- 对应于网卡及传输协议的唯一组合
* MSS() -- 最大报文段长度，表示TCP传往另一端的最大块数据的长度。三次握手时，
* MTU(Maximum Transmission Unit) -- 最大传输单元 
* NetBios Name -- 微软网络中的机器名采用的便是NetBIOS名字，机器启动时，会将名字注册到本地的WINS。
*   通过 nbtstat 命令可以列出信息。
* OSI -- 开放系统互连，应用层、表示层、会话层、传输层、网络层、数据链路层、物理层；
* OOB(Out Of Band) -- 带外数据(紧急数据)，通过独立的逻辑信道来接收和处理。尽量不要使用。
* TCP(Transmission Control Protocol) -- 传输控制协议，面向连接，提供双向、有序、无重复并且无记录边界的数据流服务
* RARP(Reverse Address Resolution Protocol)--用于动态完成物理地址向IP地址的转换。
* RTP(Real-time Transport Protocol) -- 实时传输协议，目前解决实时流媒体传输的标准协议和关键技术，通常使用UDP进行传输，
*   开源实现由 LibRTP, JRtpLib 等，每一个RTP数据报都由头部(Header)和负载(Payload)两部分组成。
* RTCP(Real-time Transport Control Protocol) -- 用于实时监控 RTP 数据传输和服务质量，
* SCTP(Stream Control Transmission Protocol) -- 流控制传输协议, 2000年新定义的面向连接的传输层协议，对TCP的缺陷进行了一些完善。
*     在两个端点之间提供稳定、有序的数据传递服务（非常类似于 TCP），基于消息流, 可以保护数据消息边界(不会出现粘包现象)
*     最初是被设计用于在IP上传输电话，其连接可以是多宿主连接的(连接双方可声明多个地址，若当前连接失败，可切换到另一个地址)。
*     
* TTL -- 生存时间，表示一个数据包在丢弃之前，可在网络上存在多长时间，值为0时，包被丢弃
* UDP(User Datagram Protocol) -- 用户数据报协议，无连接，支持双向的数据流，但并不保证可靠、有序、无重复
* WINS -- Windows互联网命名服务器,维护着已注册的所有NetBIOS名字的一个列表
* WinSock -- 是网络编程接口，而不是协议，与协议无关。可以针对一种具体协议（如IP、TCP、IPX、lrDA等）创建套接字。
*  “面向消息”（保护消息边界--每次读取返回一个消息，如网络游戏的控制包）和 “面向流”（连续的数据传输，会尽量地读取有效数据）
*  “面向连接”和“无连接”
*   面向连接的协议同时也是流式协议，无连接协议几乎都是基于消息的。
* XMPP(The Extensible Messaging and Presence Protocol, 可扩展通讯和表示协议) -- 通常可用于服务类实时通讯(如 QQ?)
*************************************************************************************************************************/

/*************************************************************************************************************************
* NetBios -- 兼容于较老的操作系统(如Dos等)，适用于众多的网络协议(如TCP/IP、NETBEUI、IPX等)，即协议无关，
*   使用LANA区分网卡和协议。
* 重定向器
* 邮槽 -- 可在Windows机器之间实现广播和单向数据通信。
* 命名管道 -- 双向信道
*
*************************************************************************************************************************/

/*************************************************************************************************************************
* TODO: GetAdaptersAddresses 的Flags中有 unicast/anycast/multicast 等多种定义,该方法的IP_ADAPTER_ADDRESSES参数分很多版本
* 
* 广播和多播必须使用 UDP 套接字才能实现
*   
* 多播/组播(multicast) -- 一(多播源)对多(加入多播组中的主机)，典型应用是多人(音视频)会议，
*   多播地址：
*     IPV4 -- D类(224.0.0.0 ~ 239.255.255.255)，具体分为三种：局部链接多播地址、预留多播地址、管理权限多播地址
*       NTP(网络时间协议组) -- 224.0.1.1
*     IPV6 -- 前面是 ff12:: ?
*   编码流程
*   1.创建多播的Socket
*     WinSock1.1: socket(AF_INET[6], SOCK_DGRAM, IPPROTO_UDP);
*     WinSock2.2: WSASocket(AF_INET[6], SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 
*        WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF); //WSA_FLAG_OVERLAPPED
*        在控制层面和数据层面都是"无根的"，只存在叶节点，可以任意加入一个多播组；从一个叶节点发送的数据会传送到每一个叶节点（包括它自己）
*   2.bind -- 将创建好的套接字与本地地址和本地端口绑定起来
*   3.通过本地的一个网络接口加入指定的多播组
*     方法1:setsockopt -- 可以在一个socket上加入多个多播组
*       IPV4:ip_mreq   mreqv4;
*            mreqv4.imr_multiaddr.s_addr = ((SOCKADDR_IN *)group->ai_addr)->sin_addr.s_addr;
*		     mreqv4.imr_interface.s_addr = ((SOCKADDR_IN *)iface->ai_addr)->sin_addr.s_addr;
*            setsockopt(, IPPROTO_IP, IP_ADD_MEMBERSHIP, mreqv4); 
*       IPV6:ipv6_mreq mreqv6;
*			 mreqv6.ipv6mr_multiaddr = ((SOCKADDR_IN6 *)group->ai_addr)->sin6_addr;
*            mreqv6.ipv6mr_interface = ((SOCKADDR_IN6 *)iface->ai_addr)->sin6_scope_id;
*            setsockopt(, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, mreqv6); 
*     方法2: WSAJoinLeaf -- 只能加入一个多播组(因此可以直接 connect ?)
*            SOCKET rs = WSAJoinLeaf(xxx); 会返回一个socket(有什么用?)
*   4.设置发送数据的网络接口 -- 和 4 中的本地网络接口 有什么区别？
*       IPV4: optval = (char *) &((SOCKADDR_IN *)iface->ai_addr)->sin_addr.s_addr;
*             optlen = sizeof(((SOCKADDR_IN *)iface->ai_addr)->sin_addr.s_addr);
*             setsockopt(, IPPROTO_IP, IP_MULTICAST_IF, optval, optlen);
*       IPV6: optval = (char *) &((SOCKADDR_IN6 *)iface->ai_addr)->sin6_scope_id;
*             optlen   = sizeof(((SOCKADDR_IN6 *)iface->ai_addr)->sin6_scope_id);
*             setsockopt(, IPPROTO_IPV6, IPV6_MULTICAST_IF, optval, optlen);
*   5.其他可选项 -- setsockopt
*     IP_MULTICAST_LOOP|IPV6_MULTICAST_LOOP  -- 允许或禁止多播通信时发送出去的通信流量是否也能够在同一个套接字上被接收（即多播返回）
*     IP_MULTICAST_TTL|IPV6_MULTICAST_HOPS -- 设置多播传播的范围(TTL，缺省值是1 ?)
*   6.connect[可选] -- 连接到多播服务器。如果是 2.2 必须连接？然后可以通过 send/recv 来传递数据？
*   7.sendto/recvfrom(1.1) 或 send/recv(2.2) -- 向多播组地址发送或接受数据
*     注意：IPV6的sendto时，多播组地址的sin6_scope_id必须是0. ((SOCKADDR_IN6 *)resmulti->ai_addr)->sin6_scope_id = 0;
*   8.退出多播组：setsockopt(, IP_DROP_MEMBERSHIP
*    
* 广播(broadcast) -- 发送到网上所有的节点，网络广播有直播和点播两种主要播放形式
*   子网广播 -- 广播得看你的网络号是几位的，现在都是CIDR标记路由，主机号全1就是广播地址，分为单路广播和多路广播？
*    编码流程：
*    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, 1); -- 服务器端设置允许广播
*    sendto(xxx.255)
* 
* 多播路由协议 -- 如 PIM(协议独立多播)、DVMRP等
*   路由器可建立起从多播源节点到所有目的节点的多播路由表，从而实现在子网间转发多播数据包
* Internet组管理协议 -- IGMP
*************************************************************************************************************************/


/*************************************************************************************************************************
* Socket模式 -- 决定在对一个套接字调用时，那些WinSock函数的行为
*   阻塞模式(缺省)：在I/O操作完成前，执行操作的Winsock函数(如send和recv)会一直等下去，不会立即返回程序。
*   非阻塞模式：Winsock函数无论成功了(返回0)还是失败都会立即返回，失败时返回SOCKET_ERROR，错误码为 WSAEWOULDBLOCK，
      通常需要频繁的调用判断(效率低，应该使用Socket模型进行异步)。
*     ioctlsocket(,FIONBIO,&1) -- 注意：ipmsg的端口就使用了该模式
*   IO复用：在 select/poll/WaitForMultiXXX 等系统调用上阻塞，可以一次等待多个文件、端口的多个情况(读/写/异常)   
*
* Socket模型 -- 描述了一个应用程序如何对套接字上进行的I/O进行管理及处理
*   select(选择) -- 利用select函数判断套接字上是否存在数据，或者能否向一个套接字写入数据。
*     分三种：可读性、可写性、例外。函数返回满足条件的套接口的数目，然后通过再次判断socket是否是集合的一部分来确定是否可读写。
*     缺点：1.不能动态的调整(如增加、删除)Socket，
*           2.如进程捕获一个信号并从信号处理程序返回，等待一般会被中断（除非信号处理程序指定 SA_RESTART 且系统支持）
*           3.在Windows下缺省最多64个(FD_SETSIZE)，通过重定义宏可以扩大，Linux下是否有限制?
*     通过 FD_ZERO、 FD_SET 等宏对socket句柄设置监听的事件， 然后select(socket+1,xxx) 返回后通过 FD_ISSET 等宏判断是否有事件
*   WSAAsyncSelect(异步选择) -- 接收以Windows消息为基础的网络事件通知(即网络事件来了后用消息进行处理)，MFC中CAsyncSocket的方式，模式自动变为非阻塞。
*     如IPMsg中tcp用于建立连接，UDP用于读取数据：
*       ::WSAAsyncSelect(udp_sd, hWnd, WM_UDPEVENT, FD_READ)
*       ::WSAAsyncSelect(tcp_sd, hWnd, WM_TCPEVENT, FD_ACCEPT|FD_CLOSE)
*       然后在消息循环中处理对应消息时，使用宏 WSAGETSELECTERROR 和 WSAGETSELECTEVENT 从lParam获取 ErrCode 和 Event(如 FD_READ)，
*         进行对应的处理(如 OnReceive )，在结束前需要再次以 0作为 lEvent 参数调用 WSAAsyncSelect 以取消事件通知
*   WSAEventSelect(事件选择) -- 针对每一个套接字，使用WSACreateEvent创建一个事件(默认是手动重置)，并进行关联。
*     可用 WSAWaitForMultipleEvents 进行等待，最多支持64个；再用 WSAEnumNetworkEvents 获取发生的事件
*   Overlapped I/O(重叠式I/O) -- 具有较好的性能，使用 WSA_FLAG_OVERLAPPED 创建Socket((默认设置)，
*     并在相关Socket函数中加入 WSAOVERLAPPED 结构或使用"完成例程"，
*     调用 WSAResetEvent 重置事件(其事件是手动重置的)，通过 WSAGetOverlappedResult 获取
*     执行结果(其结果和不使用Overlapped调用ReadFile等函数时返回的结果一样)。
*     注意：请求Overlapped后，函数通常返回失败，错误码为WSA_IO_PENDING，但如果请求的数据已经在Cache中，会直接返回成功
*
*   IO Completion Port(完成端口) -- 和 Overlapped I/O 协同工作，可以在一个“管理众多的句柄且受制于I/O的程序(如Web服务器)”中获得最佳性能。
*     没有64个HANDLE的限制，使用一堆线程(通常可设置为 CPU个数*2+2 )服务一堆Events的性质，自动支持 scalable。
*     操作系统把已经完成的重叠I/O请求的通知放入完成端口的通知队列(一个线程将一个请求暂时保存下来)，等待另一个线程为它做实际服务。
*     线程池的所有线程 = 目前正在执行的 + 被阻塞的 + 在完成端口上等待的。
*     微软例子：web\Wininet\Async
*     使用流程：
*       1.产生一个I/O completion port -- hPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL...)。产生一个没有和任何文件Handle有关系的port
*       2.让它和文件handle产生关联 -- CreateIoCompletionPort(hFile,hPort...)，为每一个欲附加的文件Handle进行关联
*       3.产生一堆线程 -- for(CPU*2+2){ _beginthreadex }
*       4.让每一个线程在Completion Port上等待，返回后根据情况进行处理
*         GetQueuedCompletionStatus(,INFINITE)，可通过 CONTAINING_RECORD 宏取出其中指向扩展结构的指针
*       5.开始对着那个文件handle发出一些overlapped I/O请求 -- 如 ReadFile、WriteFile、DeviceIoControl等，
*       6.其他线程如果需要交互(如通知程序结束)，需要通过 PostQueuedCompletionStatus 向完成端口放入事件通知，从而唤醒各个线程,
*         然后 WaitForSingleObject(hPort, 超时时间)
*   epool -- 通过系统核上的虚设备控制数据，通过预注册的回调函数，由内核并发调度  
*       
*     临时关闭完成通知(想忽略结果时)：将 OVERLAPPED 的hEvent 的最低位置为1
*       overlap.hEvent = (HANDLE)((DWORD)overlap.hEvent | 0x1));
*     微软在 NET 中提供了 IHostIoCompletionManager
*   
*   Unix下还有
*     信号驱动I/O(SIGIO，由内核通知何时可以“启动”一个IO操作)
*     异步I/O(Posix 中的aio_系列函数,由内核通知IO何时“完成”，aio_read->信号处理)
* 
* 创建IP协议下的原始套接字，可用于ICMP(互联网控制消息协议)等
* Winsock API安装在“会话层”和“传送层”之间，提供了一种可为指定传输协议打开、计算和关闭会话的能力。
*
*************************************************************************************************************************/

/*************************************************************************************************************************
* 头文件
*   <netinet/in.h>  -- 定义IPv4/IPv6的地址结构
*   <arpa/inet.h> <== 其中有 inet_pton/inet_ntop
* 
* 结构
*   sockaddr_in/sockaddr_in6 -- 最基本的套接口地址结构，每个协议族都定义了它自己的套接口地址结构。其中地址和端口号必须以网络字节序保存
*     AF_INET/AF_INET6
*   SOCKADDR_STORAGE -- 代替 sockaddr ?拥有更多的信息
*
* socket API大致可以分为五类：
*   A.本地环境管理，其信息通常保存在OS内核或系统库中
*     1.socket -- 分配一个socket句柄，并返回给调用者
*     2.bind -- 将一个socket句柄和一个本地或远程地址关联起来
*     3.getsockname -- 返回socket绑定的本地地址
*     4.getpeername -- 返回socket绑定的远程地址
*     5.close/closesocket -- 释放socket句柄，使其可复用
*   B.连接的建立和终止
*     1.connect -- 主动在一个socket句柄上建立连接，如果想设置超时，利用ioctlsocket(socket, FIONBIO, &1)把socket设置为非堵塞的，
*         在connect时会立即返回，然后利用select函数等待 readfd 并设置超时值(TODO:是 writefd? )
*         fd_set rdevents; FD_ZERO(&rdevents); FD_SET(socket, &rdevents); struct timeval tv(nTimeOutSec, 0); 
*         nResult = select (socket+1, &rdevents, NULL, NULL);
*     2.listen -- 表示愿意被动侦听来自客户的连接请求
*     3.accept -- 工厂函数，响应客户请求，创建一个新的连接，如果没有请求，将阻塞
*     4.shutdown -- 有选择地终止双向连接中读取方或写入方的数据流
*   C.数据传送机制
*     1.send/recv/ReadFile/WriteFile -- 通过某一特定的I/O句柄，传送和接收数据缓冲区
*     2.sendto/recvfrom -- 交换无连接数据报，每一次调用都会关联接收方/发送方的网络地址
*     Unix/Linux平台专用函数
*     3.read/write -- 通过某一句柄，接收和传送数据缓冲区
*     4.readv/writev -- 分别支持“分散读取”和“集中写入”语义，以优化“模式切换”，简化内存管理
*     5.sendmsg/recvmsg -- 通用函数，包含其他数据传输函数的行为
*     recv -- 可以使用标志 MSG_WAITALL 来获取需要的所有数据
*   D.选项管理  -- 怎么进行带宽限制？
*     1.setsockopt/getsockopt -- 在协议栈的不同层 更改或得到 选项
*       常见的 SocketOption(SOL_SOCKET)
*         SO_DEBUG        turn on debugging info recording 
*         SO_ACCEPTCONN   socket has had listen()
*         SO_REUSEADDR    允许重用处于 TIME_WAIT 状态的套接字建立连接。
*         SO_KEEPALIVE    keep connections alive -- 可以(自动？)保持连接，如果对端崩溃后能立即知道（怎么知道？）
*         SO_DONTROUTE    just use interface addresses 
*         SO_BROADCAST    允许发送广播消息，之后可以通过？进行广播
*         SO_USELOOPBACK  bypass hardware when possible 
*         SO_LINGER       linger on close if data present 
*         SO_OOBINLINE    leave received OOB data in line 
*         SO_SNDBUF       发送缓冲区的大小
*         SO_RCVBUF       接收缓冲区的大小
*         SO_RCVTIMEO     接收超时的时间(Windows 不支持？)
*     2.ioctlsocket/WSAIoctl -- 控制socket的IO模式，
*         FIONBIO -- 控制是否采用阻塞模式, &1 表非阻塞模式，
*         FIONREAD -- 确认等待读取的数据长度
*         SIOCATMARK -- 确认是否所有的OOB数据都已经读取完毕
*   E.网络地址 -- 在可读性的名称(如域名)和低级网络地址(如IP)之间进行转换
*     1.getaddrinfo(需要freeaddrinfo释放)/getnameinfo -- HOST名 和 地址(IPV4/IPV6) 之间 协议无关的转换
*         struct addrinfo hints = { 0 }; hints.ai_family = AF_UNSPEC; hints.ai_socktype=SOCK_DGRAM; hints.ai_flags=AI_PASSIVE;
*         getaddrinfo("www.baidu.com", "http", &hints, &res);  // res 存放返回 addrinfo 结构链表的指针,然后可通过 res->ai_family/ai_socktype 等创建socket
*       gethostbyname/gethostbyaddr -- 处理主机名和 IPV4 地址之间的网络地址映射, buffer的 最大长度为 MAXGETHOSTSTRUCT
*       WSAAsyncGetHostByName/WSAAsyncGetHostByAddr  -- 异步获取 主机/地址 信息，避免线程阻塞
*         sockAddr.sin_addr.s_addr = ((LPIN_ADDR)((LPHOSTENT)pAsyncGetHostByNameBuffer)->h_addr)->s_addr;
*     2.getipnodebyname/getipnodebyaddr -- 处理主机名和 IPV6 地址之间的网络地址映射
*     3.getservbyname -- 通过具有可读性的名称标识服务
*       getpeername -- 
*     4.inet_aton/inet_ntoa -- 在点分十进制数串(如192.168.0.1)与对应的32位网络字节序二进制值间转换。只适用于IPv4
*       注意 inet_ntoa 返回的字符串存储在静态内存中（非线程安全）
*     5.inet_pton/inet_ntop <== presentaion(字符串表达)<->numeric(套接口地址结构中的二进制值)，适用于IPv4和IPv6 -- Vista后才支持?
*         InetNtop(AF_INET,&cliaddr.sin_addr, buff, sizeof(buff));
*         InetPton/inet_pton(AF_INET,TEXT("192.168.0.1"), &addr.sin_addr, ...); 
*     6.(推荐)WSAAddressToString/WSAStringToAddress -- 在字符和二进制方式之间转换地址，支持Xp
* 注意：
*   1.Socket API使用一种很基本的地址结构(sockaddr),根据地址簇不同，结构成员的占位也不同，
*     如IPV4的 sockaddr_in,使用前一般要全部清零，否则容易出现错误
*   2.注意网络字节序和主机字节序的问题
*     网络字节序：端口(addr.sin_port)、IP地址
*   3.默认情况下，大多数TCP/IP实现会使用Nagle算法，会在发送端中缓存少量依次发送的数据报，通常情况下能
*     缓解网络拥塞，但可能增加延迟并降低吞吐量。可通过 TCP_NODELAY 选项关闭，禁止数据合并。
*     适用于经常单向发送小数据报(比如 IPMsg )
*       :setsockopt(info->sd, SOL_SOCKET, TCP_NODELAY, (char *)&flg, sizeof(flg));
*   4.延迟应答(数据捎带ACK) -- 通常TCP在接收到数据时并不立即发送ACK。实际是推迟发送，以便将ACK与需要沿该方向发送的数据一起发送。
*     绝大多数实现采用的时延为200ms
*************************************************************************************************************************/

/*************************************************************************************************************************
* TransmitFile -- 在内核中高性能的传输文件数据
* TransmitPackets -- 在已有的连接socket上传输内存块，似乎不能有反馈和取消？
* WSAStartup/WSACleanup --初始化和清除
* WSAEnumProtocols -- 获得系统中安装的网络协议的相关信息
* WSASocket(地址家族,套接字类型,协议),如果前三个参数都是用 FROM_PROTOCOL_INFO ，并且指定 WSAPROTOCOL_INFO结构，则使用结构中的。
*   可以利用 WSAEnumProtocols 枚举，然后创建指定的Socket
*************************************************************************************************************************/

/*************************************************************************************************************************
* MFC中的网络相关类介绍
*  CAsyncSocket -- 异步非阻塞Socket封装类,通过创建CSocketWnd窗体并通过WSAAsyncSelect进行异步处理.
*    函数(如 Connect )调用后,返回 WSAEWOULDBLOCK,需要在 OnXXX函数(如OnConnect)中等待并判断处理结果.
*  CSocket -- 从 CAsyncSocket继承的同步阻塞封装类.在CAsyncSocket函数返回WSAEWOULDBLOCK后,
*    通过 PumpMessage 函数从当前线程的消息队列里取关心的消息,
*************************************************************************************************************************/

/*************************************************************************************************************************
* FTP
*    分为控制端口(21)和数据端口(主动时是20，比控制端口低一位)两个连接，vsftp对每一个连接会起两个vsftpd进程。
*    分为两种运行模式：
*         主动(PORT) -- 传数据时，客户端用PORT命令告诉服务器的21端口(客户端的IP)，然后服务器从其20端口主动连过去。所有服务器都支持。
*         被动(PASV) -- 传数据时，服务器用PASV命令告诉客户端(开了XXX端口)，由客户端连过去。大部分服务器都支持，需要指定端口范围，
*             并开防火墙。
*    注意：选择用 PASV 方式 还是 PORT 方式登录FTP服务，选择权在客户端，而不是服务端。
*************************************************************************************************************************/

/*************************************************************************************************************************
* WebBrowser
*   使用方式
*     CLSID_WebBrowser--内嵌的控件，对应的ActiveX为"Microsoft Web Browser"
*     CLSID_InternetExplorer--外部宿主Exe，即IE
*   架构
*     WebBrowser Host -- 宿主,即想重用WebBrowser Control的应用程序，如 CLSID_InternetExplorer
*     Shdocvw.dll(contains WebBrowser control) -- 包装并控制Webbrowser control，给上层宿主提供浏览能力
*     mshtml.dll -- 显示HTML文档的组件，也是一个active 文档服务器和其他控件(如脚本引擎、Plugin)的容器
*     ActiveXControl/Plugin/HTML
*  接口
*    IWebBrowser -- WebBrowser Control，提供加载和显示Web、Word等页面的基本功能
*      Navigate--导航到指定页面或文件，
*        PostData -- 将指定的数据(如 ?)通过POST事务发送到服务器，如不指定任何数据，将使用Get方法
*        Headers -- 发送HTTP头信息(如?)到服务器
*    IWebBrowserApp -- Internet Explorer,继承自 IWebBrowser，可以控制状态条,工具条等用户接口
*      GetProperty/PutProperty -- 获取或设置IE属性包(property bag),具体的属性包有哪些? 
*    IWebBrowser2 -- WebBrowser Control and Internet Explorer,继承自 IWebBrowserApp(操作IE时优选该接口)
*      ExecWB -- IOleCommandTarget::Exec的包装实现,允许WebBrowser开发者增加新的功能而不用创建新的接口，
*                如通过 ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER, 4, NULL) 实现"将字体改为最大"的功能
*                其他有 OLECMDID_PAGESETUP(打印设置)、OLECMDID_PRINT(打印) 等
*    DWebBrowserEvents2 -- 事件
*************************************************************************************************************************/

/*************************************************************************************************************************
* HTTP(Hypertext Transport Protocol) -- 超文本传输协议, RFC1945定义1.0, RFC2616定义广为使用的 1.1。是无状态的协议。
*   通常承载于TCP、TLS或SSL(即HTTPS，默认端口443)协议层之上，属于应用层协议，分为 POST(RFC1867)、GET(RFC???) 等
*   C#中使用 HttpWebRequest request = (HttpWebRequest)WebRequest.Create("http://www.baidu.com");
*            request.AddRange //可以指定资源的范围
*            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
*            Stream stream = response.GetResponseStream();
*            StreamReader sr = new StreamReader(stream);
*            string content = sr.ReadToEnd();
*
*  协议由三部分组成：协议头，具体内容以及协议尾， 必须是ASCII格式？
*    协议类型：
*    使用多个表单项（同时传?）传递数据(HTTP POST-MultiPartFormData),典型情况是写邮件时的多个附件。
*      此时服务器端 HTTP_REQUEST::Flags 会有 HTTP_REQUEST_FLAG_MORE_ENTITY_BODY_EXISTS
*      HTML 中通常是 <form action="http://xxx" method="post" enctype="multipart/form-data">
*                       <input type="file" name="uploadfile1"/>
*                       <input type="file" name="uploadfile2"/>
*                       <input type="submit" value="uploadfile"/>
*                    </form>
*    1. HttpAddRequestHeaders部分
*       使用 "Content-Type: multipart/form-data; boundary={boundary}" 声明使用多表单分，且指定分割表单中不同部分数据的符号
*       (可自定义或随即产生，但一般使用的是 --MULTI-PARTS-FORM-DATA-BOUNDARY )
*    2. Post参数部分：多个部分内容，有 Content-Disposition  和可选的 [Content-Type]， 如(参数信息、二进制原始信息)，
*       每一部分用 {boundary} CRLF 和 "Content-Disposition: form-data;" 分开, 注意二进制文件尾部也需要 CRLF，(二进制数据开始时需要两个 CRLF ?)
*    3. 使用 {boundary}-- 表示结束(注意后面多两个 "--" )
*    
*
*  连接类型( HTTP/1.1 默认使用带流水线的持久连接 )
*    非持久连接(NonPersistent Connection) -- HTTP/1.0，每一次数据传送都需要重新打开/关闭TCP连接
*    持久连接(Persistent Connection,长连接) -- 服务器在发出响应后让TCP连接持续打开，同一对客户/服务器之间的
*      后续请求和响应可通过这个连接发送，在连接闲置一段时间后关闭。又分为：
*      a.不带流水线 -- 收到前一个请求的响应后才发出新的请求
*      b.带流水线 两个版本
*
* HTML VERBS(字符串) -- 请求类型，在协议头部分，可以指定HTTP版本信息
*   CONNECT(1.1) -- 预留给能够将连接改为管道方式的代理服务器
*   DELETE(1.1) -- 请求删除指定的资源
*   GET(1.0) -- 请求获取特定的资源，不会向服务器提交数据(INTERNET_REQFLAG_NO_HEADERS)，如请求一个Web页面，
*               通常只包含URL中的 网页地址部分，如 GET /index.html HTTP/1.1\r\n
*               查询字符串（名称/值对）是在 GET 请求的 URL 中发送的(会被看到，有长度限制)，如 /test/demo_form.asp?name1=value1&name2=value2
*   HEAD(1.0) -- 向服务器请求获取与GET请求相一致的响应，只不过响应体不会被返回。
*                该方法可在不必传输整个响应内容的情况下，就获取包含在响应消息头中的元信息，
*                通常用于辅助作用(如搜索引擎获取信息，安全认证时传递认证信息，下载文件前判断资源是否有效 等)
*   LINK(1.1)
*   OPTIONS(1.1) -- 返回服务器针对特定资源所支持的HTTP请求方法
*   POST(1.0) -- 向指定资源提交数据，请求进行处理，如提交表单或上传文件，请求的数据(如 查询的键值对、上传的文件内容)被包含在请求的HTTP消息主体中，
*                该请求不会被缓存，不保留在浏览器历史记录中，不能被收藏为书签，对数据长度没有要求
*   PUT(1.1) -- 向指定资源位置上传其最新内容，如请求存储一个Web页面， 能替代POST中上传文件吗？
*   TRACE(1.1) -- 回显服务器收到的请求
*   UNLINK(1.1)
*
* 判断连接 HINTERNET 是否需要认证信息
*   1.HttpQueryInfo(hRequest, HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_STATUS_CODE, &dwStatus, &cbStatus, NULL);  //获取状态码
*   2.switch(dwStatus) {   //查询状态码
*       case HTTP_STATUS_DENIED(401):			// 拒绝访问，再查询 dwFlags = HTTP_QUERY_WWW_AUTHENTICATE
*       case HTTP_STATUS_PROXY_AUTH_REQ(407):	// 代理需要身份信息，再查询 dwFlags = HTTP_QUERY_PROXY_AUTHENTICATE
*     }
*     do  { bRet = HttpQueryInfo( hRequest, dwFlags, szScheme, &cbScheme, &dwIndex ); } while(bRet);  //可能有多次验证，因此需要循环？

* 使用HTTP获取数据的步骤: 
*   1.客户端建立连接(TCP)
*   2.客户端发送请求(HTTP/HTTPS)
*     内容包括：统一资源标识符（URL）、协议版本号，后边是MIME信息包括请求修饰符、客户机信息和可能的内容
*   3.服务器返回响应信息，格式为一个状态行，包括信息的协议版本号、一个成功或错误的代码，后边是MIME信息包括服务器信息、实体信息和可能的内容
* 
* 头域(Headers)，主要数据前的原信息(Meta Information)，用于向服务器提供本次请求的相关信息，格式为 "域名:域值"
*   每个描述部分用"CRLF"分开，结束处有两个"CRLF"，通过 HttpAddRequestHeaders 加入?
*   [M]Accept: text/plain, x/x(注意：此处本来是*号)，表示Client能够接收得种类和类型
*   [O]Accept-Charset: GBK, utf-8
*   [O]Accept-Encoding: gzip, deflate
*   [O]Accept-Language: zh-cn
*   [O]Accept-Ranges: bytes
*   [O]Cache-Control: 指定请求和响应遵循的缓存机制，如 no-cache, max-age, private 等
*   [M]Content-Type: 请求或返回的内容类型，对应于 form 中的 enctype
*      类型列表 -- rfc1341( http://www.ietf.org/rfc/rfc1341.txt )
*        application/x-www-form-urlencoded <== 
*        application/octet-stream <== 网络上传递二进制数据时
*        multipart/form-data; boundary=--{boundary} <== 表单数据传输文件内容
*        video/avi   <== AVI 
*        image/jpeg  <== JPG
*        image/x-png <== PNG
*        image/bmp   <== BMP
*        image/tiff  <== TIF
*        image/gif   <== GIF
*        text/plain  <== TXT
*        text/xml    <== XML
*        text/html;charset=gb2312  <== HTML
*        
*   []Content-Disposition: -- 不是HTTP标准，但广泛实现。
*     可传输二进制文件(前面必须有 --{boundary}), 其后接保存时的建议文件名。
      协议头时：form-data; name=\"attach_file\"; filename=\"xxxx\"    -- 其中的 "attach_file" 是HTML元素名(和服务器有关)
      协议尾时：form-data; name=\"icount\" + CRLF + CRLF + _T("1") + CRLF +   -- 其中的 icount 也是服务器相关的?
      \"submitted\"
*   [M]Content-Length: 内容长度（包括 头 + 数据 + 尾），如果直接访问文件地址的话，返回的是文件大小？
*   [O]Content-Transfer-Encoding: binary | Chunked(表示输出的内容长度不确定)
*   []Connection: Keep-Alive(持久连接), Close(非持久连接)
*   []Cookie : 分号";"分开的多个 "变量名=值" 的键值对
*   []Date: 表示消息发送的时间，由RFC822定义，是世界标准时。如 Sat, 26 May 2012 00:42:19 GMT
*   [O]Host: www.google.com[:80]，请求目标的网站，和 GET 等请求类型后的地址连起来就是目标地址
*   []Referer: http://源资源地址， 这可以允许服务器生成回退链表，可用来登陆、优化、cache等
*   []User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0) 或自定义的名字，是发出请求的用户身份信息
*                
* GET /文件地址 HTTP/1.1 -- 分块/断点续传，如果请求成功会返回 206(HTTP_STATUS_PARTIAL_CONTENT) OK
*   []Range: bytes=开始位置-结束位置, 请求实体的一个或者多个子范围，可通过逗号分隔多个范围
*   []Content-Range: bytes 开始位置-结束位置/总大小, 对Range 的响应
* 
* 请求主体(request-body)
*   其中可包含任意的数据，
* 
* 返回信息: 状态行(HTTP版本号 + 3位状态码 + 状态描述) + 头 + 响应体
*   状态码( HTTP_STATUS_OK 等 )
*     1xx消息 -- 请求已被服务器接收，继续处理
*       100 Continue -- 服务器仅接收到部分请求，客户端应该继续发送其余的请求。
*       101 Switching Protocols -- 服务器转换协议：服务器将遵从客户的请求转换到另外一种协议。
*     2xx成功 -- 请求已成功被服务器接收、理解并接受，如 200(OK，其后是对GET和POST请求的应答文档）
*       206(PARTIAL_CONTENT) -- 表示成功获取到部分数据，用于Range指定范围下载
*     3xx重定向 -- 需要后续操作才能完成这一请求，如 304(NOT MODIFIED--该资源在上次请求之后没有任何修改，通常用于浏览器的缓存机制)
*     4xx请求错误 -- 请求含有语法错误或无法被执行，如 401(UNAUTHORIZED),403(FORBIDDEN),404(NOT FOUND)
*     5xx服务器错误 -- 服务器在处理某个正确请求时发生错误，如 501(Not Implemented--服务器不能识别请求或未实现指定的请求)
*   返回的状态头
*     []Server: Apache/2.2.11 (Unix)
*     []Last-Modified: Wed, 23 May 2012 21:38:23 GMT
*     []ETag: "1d2d6-5c896800-4c0baf45c91c0"
*     []Accept-Ranges: bytes
*     []Content-Encoding: gzip, 响应体的编码方式，如gzip表示服务器端使用了gzip压缩，利于下载，但是必须client端支持gzip的解码操作
*       Vista之后WinINet可以支持解码( InternetSetOption(INTERNET_OPTION_HTTP_DECODING) )
*     []Content-Length: 1164371968
*     []Content-Range: bytes 388136960-1552508927/1552508928
*     []Expires: 告诉client绝对的过期时间，在这个时间内client都可以不用发送请求而直接从client的cache中获取，
*                可用于js/css/image的缓存性能
*     []Keep-Alive: timeout=5, max=100
*     []Last-Modified: 会后一次修改响应内容的日期和时间
*     []Server: BWS/1.0 响应客户端的服务器，可以看出是什么类型的Web服务
*
* 封装函数
*   URLDownloadToFile -- 指定URL直接下载文件，缺点：同步调用，不能下载大文件。
*
* WinINet API 函数(MFC 封装: CHttpFile) -- 通常只适用于客户端程序，服务器程序开发需要用 WinHTTP(升级版? 参见 "Porting WinINet Applications to WinHTTP" )
*   一般有三个 HINTERNET(可通过 GetHInternetHandleType 函数区分):
*     1.InternetOpen/WinHttpOpen 初始化的 WinINet 函数库句柄,得到Session句柄(对应 CInternetSession ), 
*       如果Flags有 INTERNET_FLAG_ASYNC 则表明是异步连接，其后的连接、数据交换都需要是异步的(需要通过 InternetSetStatusCallback 设置回调 且 通过完成端口来进行 ?)
*         需要在 INTERNET_STATUS_REQUEST_COMPLETE 事件响应中，根据状态进行处理(比如 )
*       异步时，HTTP 的 InternetConnect 会同步返回，FTP的 InternetConnect 会异步返回(ERROR_IO_PENDING)
*     2.InternetConnect(指定Server+Object+Port)/WinHttpConnect  
*       连接到指定 Server:Port 上的Connect句柄(对应 CHttpConnection )，可以指定用户名和密码
*     3.HttpOpenRequest/WinHttpOpenRequest 在连接句柄上打开的Request句柄(对应 CHttpFile 等)，要指定是 POST/GET 等，其后的数据交换在该句柄上进行
*       其 lplpszAcceptTypes 参数是指向LPCTSTR数组的指针，数组以一个NULL指针结束，指定了程序接受的内容的类型(NULL 表不接受任何类型的内容，空字符串等价于"text/*")
*   注意：如果采用 InternetOpen + InternetOpenUrl 的方式，则只需要两个句柄
* 
*   发送文件数据流程
*     HttpOpenRequest => HttpAddRequestHeaders => HttpSendRequestEx => loop InternetWriteFile( Fire OnProgress ) 
*       => HttpEndRequest => loop InternetReadFile(读取响应)
*
*   接收数据流程
*     HttpQueryInfo(HTTP_QUERY_CONTENT_LENGTH)     获取大小，静态网页可获取到大小，但asp/php等动态网页无法获取
*     HttpQueryInfo(HTTP_QUERY_STATUS_CODE)   获取当前的状态码
*     Loop InternetReadFile( Fire OnProgress ) ,如获取到大小，读取到指定大小；否则读取到 出错或 lpdwNumberOfBytesRead 返回 0
*     InternetQueryDataAvailable 判断是否还有数据（函数调用成功，且返回值为0 表示读取完毕，需要在 HttpSendRequest 之后才能调用）
* 
*   属性/状态
*     InternetGetConnectedState()
*     InternetQueryOption -- 
*     InternetSetOption/WinHttpSetOption -- 
*     InternetGetCookie
*     InternetSetCookie[Ex] -- 设置Cookie
*     HttpQueryInfo
*     InternetSetStatusCallback  -- 设置网络连接时各种事件的回调接口
*     InternetErrorDlg -- 弹出网络相关的错误对话框，如认证失败(httpauth 例子中测试没有出现):
*       dwError = ERROR_INTERNET_INCORRECT_PASSWORD; //GetLastError()
*       dwFlags = FLAGS_ERROR_UI_FILTER_FOR_ERRORS | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS | FLAGS_ERROR_UI_FLAGS_GENERATE_DATA;
*       if(InternetErrorDlg(GetDesktopWindow(), hRequest, dwError, dwFlags, NULL) == ERROR_INTERNET_FORCE_RETRY){ retry; }
*   连接控制
*     InternetOpen -- 初始化WinINet函数库，其返回的句柄用于后续的Connect,使用完毕后需要Close
*     InternetConnect -- 指定URL建立一个连接, 可知指定供 InternetSetStatusCallback 使用的回调参数(设计比较怪，为什么不在 InternetSetStatusCallback 中提供?)
*     InternetAttemptConnect
*     InternetCloseHandle(hRequest,hConnect, hOpen)
*     HttpOpenRequest -- 建立一个HTTP请求，不过这个函数不会自动把请求发送出去，需要调用 HttpSendRequest[Ex] 来发送
*       Https时dwFlags参数需要加上 INTERNET_FLAG_SECURE，并可以忽略特殊错误，如 INTERNET_FLAG_IGNORE_CERT_CN_INVALID|INTERNET_FLAG_IGNORE_CERT_DATE_INVALID
*     InternetLockRequestFile/InternetUnlockRequestFile -- 锁定/解锁资源文件
*   数据传递
*     InternetWriteFile -- 向打开的Http请求句柄中写入数据，通常在循环中进行
*     InternetReadFile -- 从Http请求句柄中读取数据，通常循环到 *lpdwNumberOfBytesRead 为 0 或 函数返回FALSE
*     InternetQueryDataAvailable -- 获取网络上还有的数据量，如果函数成功且返回的大小为0，表示没有数据了。
*     InternetSetFilePointer -- 可以指定读取的位置?
*     HttpAddRequestHeaders(xxx, HTTP_ADDREQ_FLAG_ADD) -- 向HTTP请求句柄中增加请求头，每个头都要以一个 CRLF 结束
*     HttpSendRequest -- 发送请求数据 (有一个限制? -- 所有需要发送的内容都要在一个buffer里面?)
*     WinHttpSendRequest
*     WinHttpReceiveResponse -- 接收响应
*     HttpSendRequestEx -- 通过 INTERNET_BUFFERS(注意要设置 dwStructSize) 结构体发送请求数据，
*       通常可用于 POST 发送文件，将 dwBufferTotal 设置为 Post等信息长度 + 文件大小(等价于设置 Content-Length )
*       使用的请求句柄需要带 INTERNET_FLAG_NO_CACHE_WRITE 参数获得
*     HttpEndRequest -- 结束HttpSendRequestEx初始化的HTTP请求
*   Cache管理
*     分为两种类型？
*       Groups -- ?
*       Entries -- 可以查找 visited:/cookie/""(CONTENT)/NULL(全部) 等各种网络访问过的内容缓存
*     FindFirstUrlCacheGroup -> 处理(如：DeleteUrlCacheGroup) -> FindNextUrlCacheGroup -> FindCloseUrlCache
*     FindFirstUrlCacheEntry/FindFirstUrlCacheEntryEx( INTERNET_CACHE_ENTRY_INFO::dwStructSize 系统返回后设置的 *lpdwEntryInfo 设置的，可变)
*       -> 处理(如：DeleteUrlCacheEntry) -> FindNextUrlCacheEntryEx -> FindCloseUrlCache
*     GetUrlCacheEntryInfo -> 
*
* HTTP服务器函数(winHttp)，目前有两个版本：1.0(XPSP2/Win2003)和2.0(Vista/Win2008)
*   HttpInitialize(1.0/2.0) -- 初始化服务器API，可以选择 配置 或 服务器 初始化。
*   创建HTTP请求队列的句柄并设置URL相关的资源(URL 必须满足 FullyQualifie，即带协议、端口、最后的斜线等，如 http://localhost:80/ )，分版本：
*     1.0
*       HttpCreateHttpHandle -- 创建请求队列，结束时需要 CloseHandle
*       HttpAddUrl -- 向请求队列中注册需要处理的URL，可多个
*     2.0:
*       HttpCreateServerSession -- 使用完毕后需要 HttpCloseServerSession
*       HttpCreateUrlGroup -- 创建UrlGroup，使用完毕后需要 HttpCloseUrlGroup
*       HttpAddUrlToUrlGroup -- 向UrlGroup中注册需要处理的URL，可多个
*       HttpCreateRequestQueue -- 创建请求队列，结束时需要 HttpCloseRequestQueue
*       HttpSetUrlGroupProperty(,HttpServerBindingProperty,) -- 把UrlGroup和请求队列绑定起来，还可设置其他信息(如Authentication等)
*   数据处理循环
*     HttpReceiveHttpRequest -- 从请求队列中获取可用的HTTP请求(可选择同步或异步)，根据 HTTP_REQUEST::Verb 等进行处理
*       HttpReceiveRequestEntityBody -- 接收关联在HTTP请求上的实体数据(如客户端上传的文件数据，但通过html实测发现没有读取到文件数据？)
*       HttpSendHttpResponse -- 发送响应，如果后续还有更多内容(如文件)需要发送，需要指定 HTTP_SEND_RESPONSE_FLAG_MORE_DATA
*       HttpSendResponseEntityBody -- 发送关联到HTTP响应上的实体数据(如通过文件句柄指定的文件)
*   HttpRemoveUrl(1.0)/HttpRemoveUrlFromUrlGroup(2.0) -- 从请求队列中停止URL的处理
*   HttpTerminate(1.0/2.0) -- 清除HTTP服务器API的资源
*************************************************************************************************************************/

namespace FTL
{
    #ifdef FTL_DEBUG
        //send 等函数成功时返回发送的字节数
        #define NET_VERIFY(x)	\
        {\
            rc = (x);\
            if(SOCKET_ERROR == rc)\
            {\
                int lastSocketError = WSAGetLastError();\
                REPORT_ERROR_INFO(FTL::CFNetErrorInfo, lastSocketError, x);\
                WSASetLastError(lastSocketError);\
            }\
        }
    #define NET_VERIFY_EXCEPT1(x, e1) \
        {\
            rc = (x);\
            if(SOCKET_ERROR == rc)\
            {\
                int lastSocketError = WSAGetLastError();\
                if((e1) != lastSocketError)\
                {\
                    REPORT_ERROR_INFO(FTL::CFNetErrorInfo, lastSocketError, x);\
                }\
                WSASetLastError(lastSocketError);\
            }\
        }
    #else
        #define NET_VERIFY(x)	\
            rc = (x);
        #define NET_VERIFY_EXCEPT1(x, e1) \
            rc = (x);
    #endif

    #define SAFE_CLASE_WSAEVENT(h)\
        if(WSA_INVALID_EVENT != (h))\
        {\
            WSACloseEvent((h));\
            (h) = WSA_INVALID_EVENT;\
        }

    #define SAFE_CLOSE_SOCKET(h)\
        if(INVALID_SOCKET != (h))\
        {\
            NET_VERIFY(closesocket((h)));\
            (h) = INVALID_SOCKET;\
        }\

	//向HTTP响应中增加预定义好的头(如 HttpHeaderContentType 的 "text/html" )信息
	#ifndef ADD_HTTP_RESPONSE_KNOWN_HEADER
	#  define ADD_HTTP_RESPONSE_KNOWN_HEADER(Response, HeaderId, RawValue)\
		{\
			(Response).Headers.KnownHeaders[(HeaderId)].pRawValue = (RawValue);\
			(Response).Headers.KnownHeaders[(HeaderId)].RawValueLength = (USHORT) strlen(RawValue);\
		} 
	#endif //ADD_HTTP_RESPONSE_KNOWN_HEADER

    FTLEXPORT class CFNetErrorInfo : public CFConvertInfoT<CFNetErrorInfo,int>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFNetErrorInfo);
    public:
        FTLINLINE explicit CFNetErrorInfo(int err);
        FTLINLINE virtual LPCTSTR ConvertInfo();
    };

	//IPV4/IPV6 兼容的地址 -- SOCKADDR_STORAGE
	class CFSocketAddress : public SOCKET_ADDRESS
	{
	public:
		FTLINLINE explicit CFSocketAddress();
		FTLINLINE explicit CFSocketAddress(const SOCKET_ADDRESS& addr);
		FTLINLINE explicit CFSocketAddress(const SOCKADDR_IN& addrv4);
		FTLINLINE explicit CFSocketAddress(const SOCKADDR_IN6& addrv6);
		FTLINLINE explicit CFSocketAddress(const SOCKADDR_STORAGE addrStorage);
		FTLINLINE explicit CFSocketAddress(LPCTSTR sAddr, USHORT usPort = 0);
		FTLINLINE ~CFSocketAddress();

        FTLINLINE BOOL   SetAddressPort(USHORT usPort);
		FTLINLINE BOOL 	 GetIPv4Address(in_addr& rAddrV4, USHORT& rPort);
		FTLINLINE BOOL	 GetIPV6Address(in6_addr& rAddrV6, USHORT& rPort);
		FTLINLINE LPCTSTR ToString(CFStringFormater& formater);
	protected:
		FTLINLINE VOID	  Init();
	};

	typedef std::map<tstring, tstring> CookieKeyValueMap;
    namespace FNetInfo
    {
		//FTLINLINE LPCTSTR GetSockAddrString(CFStringFormater& formater, SOCKADDR *sa, int len);

		//头中的CheckSum
		FTLINLINE USHORT CheckSum(USHORT *pBuffer, int size);

		FTLINLINE LPCTSTR GetCookieInfo(CFStringFormater& formater, LPCTSTR lpszUrl, LPCTSTR lpszCookieName);
		FTLINLINE DWORD GetCookieInfoMap(LPCTSTR pszCookies, CookieKeyValueMap& cookieMap);

        //获取协议的地址家族：如 AF_INET / AF_INET6
        FTLINLINE LPCTSTR GetAddressFamily(int iAddressFamily);

        //获取套接字类型
        FTLINLINE LPCTSTR GetSocketType(int iSocketType);

		//FTLINLINE LPCTSTR GetSocketAddrInfoString(CFStringFormater& formater, const PSOCKADDR_IN pSockAddrIn);
		//获取 ADDRINFO.ai_flags 对应的字符串
		FTLINLINE LPCTSTR GetAddrInfoFlagsString(CFStringFormater& formater, int aiFlags);

		//FTLINLINE LPCTSTR GetAddressInfoString(CFStringFormater& formater, LPSOCKADDR pSockAddr, DWORD dwAddressLength);
		//FTLINLINE LPCTSTR GetAddressInfoString(CFStringFormater& formater, SOCKET_ADDRESS& socketAddress);
		FTLINLINE LPCTSTR GetAddressInfoString(CFStringFormater& formater, const ADDRINFO& addrInfo, int nLevel = 0);

        //获取指定地址家族的协议： 如 AF_INETx 中的 IPPROTO_IP/IPPROTO_TCP/IPPROTO_UDP 等
        FTLINLINE LPCTSTR GetProtocolType(int iAddressFamily,int iProtocol);

        FTLINLINE LPCTSTR GetServiceFlagsType(DWORD dwServiceFlags);
        FTLINLINE LPCTSTR GetProviderFlagsType(DWORD dwProviderFlags);

		FTLINLINE LPCTSTR GetPerConnOptionListInfo(CFStringFormater& formater, const INTERNET_PER_CONN_OPTION_LIST& optList);
		FTLINLINE LPCTSTR GetHInternetHandleType(CFStringFormater& formater, const ULONG& HandleType);
		FTLINLINE LPCTSTR GetProxyInfoString(CFStringFormater& formater, const INTERNET_PROXY_INFO& proxyInfo);
		FTLINLINE LPCTSTR GetDiagnosticSocketInfoString(CFStringFormater& formater, const INTERNET_DIAGNOSTIC_SOCKET_INFO& diagSocketInfo);
		FTLINLINE LPCTSTR GetCacheTimeStampsString(CFStringFormater& formater, const INTERNET_CACHE_TIMESTAMPS& cacheTimeStamps);

#ifdef __WINCRYPT_H__
		FTLINLINE LPCTSTR GetCertChainContextString(CFStringFormater& formater, const PCCERT_CHAIN_CONTEXT& certChainContext);
#endif 

		FTLINLINE LPCTSTR GetReqestFlagString(CFStringFormater& formater, DWORD dwRequestFlags);
		FTLINLINE LPCTSTR GetSecurityFlagsString(CFStringFormater& formater, DWORD dwSecurityFlags);

		//获取 HINTERNET 的配置信息, 如果 dwOption 是-1， 则获取全部
		FTLINLINE LPCTSTR GetHInternetOption(CFStringFormater& formater, HINTERNET hInternet, DWORD dwOption = DWORD(-1));

		//通过 HttpQueryInfo 获取HTTP信息
		FTLINLINE LPCTSTR GetHttpQueryInfoString(CFStringFormater& formater, HINTERNET hInternet, DWORD dwInfoLevel = DWORD(-1));

        //获取本地地址，默认获取IPV4，
        FTLINLINE LONG GetLocalAddress(std::list<CFSocketAddress*>& lstAddress, ULONG Family = AF_INET, ULONG Flags = 0);

		//返回URL指定的文件大小和名字
		FTLINLINE BOOL GetUrlFileSizeAndFileName(
			__in LPCTSTR pszUrl, 
			__in LPCTSTR pszCookie, 
			__out PLONGLONG pFileSize, 
			__out LPTSTR pszOutFileName, 
			__in INT nMaxFileNameSize); 
    }

	//默认实现只是打印出事件日志，该类的变量必须作为 InternetConnect(dwContext) 参数传入
	//template<class T>
	FTLEXPORT class CFInternetStatusCallbackImpl
	{
	public:
		FTLINLINE CFInternetStatusCallbackImpl();
		FTLINLINE virtual ~CFInternetStatusCallbackImpl();

		FTLINLINE void SetParam(DWORD_PTR param);
		FTLINLINE BOOL Attach(HINTERNET hInternet);
		FTLINLINE BOOL Detach();
	protected:
		DWORD_PTR					m_param;
		HINTERNET					m_hInternet;

		FTLINLINE virtual void OnResolvingName(HINTERNET hInternet, DWORD_PTR dwContext, LPCTSTR lpszName, DWORD dwLength);
		FTLINLINE virtual void OnNameResolved(HINTERNET hInternet, DWORD_PTR dwContext, LPCTSTR lpszName, DWORD dwLength);
		FTLINLINE virtual void OnConnectingToServer(HINTERNET hInternet, DWORD_PTR dwContext, SOCKADDR* pSockAddr, DWORD dwLength);
		FTLINLINE virtual void OnConnectedToServer(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnSendingRequest(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnRequestSent(HINTERNET hInternet, DWORD_PTR dwContext, 
			DWORD* pdwSend, DWORD dwLength);
		FTLINLINE virtual void OnReceivingResponse(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnResponseReceived(HINTERNET hInternet, DWORD_PTR dwContext,
			DWORD* pdwResponse, DWORD dwLength);
		FTLINLINE virtual void OnCtlResponseReceived(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnPrefetch(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnClosingConnection(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnConnectionClosed(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnHandleCreated(HINTERNET hInternet, DWORD_PTR dwContext,
			INTERNET_ASYNC_RESULT* pAsyncResult, DWORD dwLength);
		FTLINLINE virtual void OnHandleClosing(HINTERNET hInternet, DWORD_PTR dwContext, DWORD* pTmpValue, DWORD dwLength);
		FTLINLINE virtual void OnDetectingProxy(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnRequestComplete(HINTERNET hInternet, DWORD_PTR dwContext,
			INTERNET_ASYNC_RESULT* pAsyncResult, DWORD dwLength);
		FTLINLINE virtual void OnRedirect(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnIntermediateResponse(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnUserInputRequired(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnStateChange(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnCookieSent(HINTERNET hInternet, DWORD_PTR dwContext, DWORD* pTmpValue, DWORD dwLength);
		FTLINLINE virtual void OnCookieReceived(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnPrivacyImpacted(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnP3pHeader(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnP3pPolicyRef(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnCookieHistory(HINTERNET hInternet, DWORD_PTR dwContext, 
			InternetCookieHistory* pCookieHistory, DWORD dwLength);
	private:
		INTERNET_STATUS_CALLBACK	m_pOldCallBack;
		FTLINLINE static VOID	CALLBACK _StatusCallbackProc(
			HINTERNET hInternet,
			DWORD_PTR dwContext,
			DWORD dwInternetStatus,
			LPVOID lpvStatusInformation,
			DWORD dwStatusInformationLength
			);
		FTLINLINE VOID	_InnerStatusCallbackProc(
			HINTERNET hInternet,
			DWORD dwInternetStatus, 
			LPVOID lpvStatusInformation, 
			DWORD dwStatusInformationLength);
	};


	//解决TCP粘包的一些类
	class IReceiveAdapter
	{
	public:
		//返回收到的包的长度
		virtual INT ReceiveData(BYTE* pBuffer, INT nLength) = 0;
	};
	class IPacketParserAdapter
	{
	public:
		//返回分析出的包的长度，每次分析一个包即可，会在 CFTCPReceiver::ParsePacket 中循环调用
		//注意：该函数会锁定 CFTCPReceiver，且 Buffer 不会拷贝(该函数退出后，可能被覆盖)
		//      因此需要尽快处理完毕，如果后续需要数据，需要自己拷贝保存
		virtual INT ParsePacket(BYTE* pBuffer, INT nLength) = 0;
	};

	//TCP缓冲，和每个数据接收端( 如 TCP Socket )绑定，作为接受数据的缓冲，目前提供了如下功能
	//a. TCP粘包问题处理
	//
	//通常的使用方式是?
	//  1.TCP接收类包含作为成员变量，并指定对应的 IReceiveAdapter、IPacketParserAdapter 实例
	//  2.if(tcpReceiver.ReceiveData() > 0 ) { tcpReceiver.ParsePacket(); } 
	class CFTCPReceiver
	{
	public:
		FTLINLINE CFTCPReceiver(IReceiveAdapter* pReceiveAdapter, IPacketParserAdapter* pParserAdapter, INT nBufferSize = INTERNET_BUFFER_SIZE);
		FTLINLINE ~CFTCPReceiver();

		FTLINLINE BOOL IsBufferAvalibe() const { return m_pBufferHeader ? TRUE : FALSE; }

		FTLINLINE INT ReceiveData();

		//循环处理Packet，处理完毕后会重新拷贝分析剩余的数据到Buffer头部
		FTLINLINE INT ParsePacket();

		//TODO:跳过指定长度的字节，不分析
		FTLINLINE INT SkipParseData(INT nLength); 
	private:
		CFCriticalSection	m_LockObject;		//保护环形缓冲的临界区
		IReceiveAdapter*		m_pReceiveAdapter;
		IPacketParserAdapter*	m_pParserAdapter;
		INT					m_nBufferSize;
		BYTE*				m_pBufferHeader;//缓冲区起始位置,会分配 nBufferSize 长度的Buffer
		BYTE*				m_pRead;		//当前未处理数据的起始位置
		BYTE*				m_pWrite;		//当前未处理数据的结束位置
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//使用线程池进行 HTTP 通信的网络传输类
	enum FTransferJobType
	{
		tjtUnknown = 0,
		tjtGet = 1,		//Not Implement
		tjtUpload,
		tjtDownload,
	};

	enum FTransferParamType
	{
		tptUnknown,
		tptPostArgument,    //Post 参数，每一个都格式化成 Content-Disposition: form-data; name=\"xxx\"\r\n\r\nValue 分开
		tptRequestHeader,	//HttpAddRequestHeaders,目前唯一需要外界提供的是 Cookie,
		tptLocalFile,		//本地文件，Value是文件路径, 上传时类似 tptPostArgument，下载时 vlaue 是文件的本地路径
	};
	enum FTransferOrder
	{
		toNormal,			//will send as normal order(post -> localfile -> End)
		toNeedCallback,		//ticket in nTalk maybe expired, so must send after file send
	};
	struct FTransferParam
	{
		FTransferParamType	paramType;
		CAtlString			strName;
		CAtlString			strValue;
		FTransferOrder		transferOrder;
		UINT				CodePage;	//Change Code Page, default is CP_UTF8
		FTLINLINE FTransferParam()
		{
			paramType = tptUnknown;
			strName.Empty();
			strValue.Empty();
			transferOrder = toNormal;
			CodePage = CP_UTF8;
		}
	};

	typedef std::list<FTransferParam>	TransferParamContainer;
	struct FTransferJobInfo
	{
		DISABLE_COPY_AND_ASSIGNMENT(FTransferJobInfo);
	public:
		FTLINLINE FTransferJobInfo();
		FTLINLINE FTransferJobInfo(LPCTSTR pszServerName, 
			LPCTSTR pszObjectName, 
			USHORT nPort = INTERNET_DEFAULT_HTTP_PORT);
		FTLINLINE ~FTransferJobInfo();

		//TODO: 参考 CHttpFile::AddRequestHeaders("Accept: */*");
		FTLINLINE void AddTransferParam(FTransferParamType paramType, const CAtlString& strName, const CAtlString& strValue, 
			FTransferOrder transOrder = toNormal, UINT codePage = CP_UTF8)
		{
			FTransferParam param;
			param.paramType = paramType;
			param.strName = strName;
			param.strValue = strValue;
			param.transferOrder = transOrder;
			param.CodePage = codePage;
			m_transferParams.push_back(param);
		}
		FTransferJobType	m_TransferJobType;
		CAtlString	m_strServerName;
		CAtlString	m_strObjectName;
		USHORT		m_nPort;

		DWORD_PTR	m_dwUserParam;
		TransferParamContainer	m_transferParams;
		CAtlStringA		m_strResponseData;		//没有转换过的网络反馈(在Upload中使用, Download 的需要保存成文件)
		CAtlString		m_strVerb;
	};

	typedef CFSharePtr<FTransferJobInfo> FTransferJobInfoPtr;

	class CFTransferJobBase 
		: public CFJobBase<FTransferJobInfoPtr>
		, public CFRWBufferT<CFTransferJobBase>
	{
	public:
		FTLINLINE CFTransferJobBase(const CAtlString& strAgent);
		FTLINLINE virtual ~CFTransferJobBase();
	public:
		//CFRWBufferT
		FTLINLINE BOOL ReadReal(PBYTE pBuffer, LONG nCount, LONG* pRead);
		FTLINLINE BOOL WriteReal(const PBYTE pBuffer, LONG nCount, LONG* pWrite);
	protected:
		//CFJobBase virtual function
		FTLINLINE virtual BOOL OnInitialize();
		FTLINLINE virtual BOOL Run();
		FTLINLINE virtual void OnFinalize();
		FTLINLINE virtual void OnCancelJob();
	protected:
		CAtlString				m_strAgent;

		HINTERNET	m_hSession;
		HINTERNET	m_hConnection;
		HINTERNET	m_hRequest;

		//如果想支持断点续传或多线程下载，需要使用 SetFilePointer，常用的 LARGE_INTEGER::QuadPart 是 LONGLONG
		LONGLONG		m_nTotalSize;
		LONGLONG		m_nCurPos;

	protected:
		FTLINLINE virtual BOOL _CheckParams() = 0;
		FTLINLINE virtual BOOL _SendRequest() = 0;
		FTLINLINE virtual BOOL _ReceiveResponse() = 0;
		FTLINLINE virtual void _OnClose(){}
	protected:
		FTLINLINE void _InitValue();

		FTLINLINE BOOL _Connect();
		FTLINLINE void _Close();

		//循环保证发送指定的 N 个字节数据
		//TODO: 参考 CHttpFile 的Buffer功能
		FTLINLINE BOOL _SendN(PBYTE pBuffer, LONG nCount, LONG* pSend);

	};

	class CFUploadJob : public CFTransferJobBase
	{
	public:
		FTLINLINE CFUploadJob(const CAtlString& strAgent);
	protected:
		FTLINLINE virtual BOOL _CheckParams();
		FTLINLINE virtual BOOL _SendRequest();
		FTLINLINE virtual BOOL _ReceiveResponse();
		FTLINLINE virtual void _OnClose();
	protected:
		//translate m_transferParams to m_postArgumentParams and m_postFileParams
		FTLINLINE virtual BOOL  _TranslatePostParams();
		FTLINLINE virtual void  _ClearPostParams();
		FTLINLINE virtual void	_AddRequestHeader(LPCTSTR pszName, LPCTSTR pszValue);

		FTLINLINE virtual LPCTSTR _GetMultiPartBoundary(BOOL bEnd) const;
		FTLINLINE virtual LONGLONG _CalcContentLength();
		FTLINLINE virtual BOOL _SetRequestHeader();
		FTLINLINE virtual BOOL _SendUploadData();

		FTLINLINE virtual BOOL _SendPostArgument(PBYTE pBuffer, DWORD dwBufferSize);
		FTLINLINE virtual BOOL _SendLocalFile(PBYTE pBuffer, DWORD dwBufferSize);
		FTLINLINE virtual BOOL _SendCallbackParam(PBYTE pBuffer, DWORD dwBufferSize);
		FTLINLINE virtual BOOL _SendEndBoundary(PBYTE pBuffer, DWORD dwBufferSize);

	protected:
		struct PostArgumentParam
		{
			DISABLE_COPY_AND_ASSIGNMENT(PostArgumentParam);
		public:
			LPSTR	pBuffer;
			INT		nBufferSize;
			FTransferOrder		transferOrder;
			PostArgumentParam()
			{
				pBuffer = NULL;
				nBufferSize = 0;
				transferOrder = toNormal;
			}
			~PostArgumentParam()
			{
				SAFE_DELETE_ARRAY(pBuffer);
			}
		};
		struct PostFileParam
		{
			DISABLE_COPY_AND_ASSIGNMENT(PostFileParam);
		public:
			//注意: pBuffer包括了最后的NULL(这样方便调试时查看), 但nBufferSize不包括最后的NULL(保证发送的数据不错误)
			LPSTR   pBuffer;
			DWORD	nBufferSize;	

			DWORD   dwFileSize;
			CAtlString strFilePath;
			PostFileParam()
			{
				pBuffer = NULL;
				nBufferSize = 0;
				dwFileSize = 0;
			}
			~PostFileParam()
			{
				SAFE_DELETE_ARRAY(pBuffer);
			}
		};
		std::list<PostArgumentParam*>	m_postArgumentParams;
		std::list<PostFileParam*>		m_postFileParams;
		PostArgumentParam*				m_pEndBoundaryPostParam;
	};

	//下载时必须通过 tptLocalFile 指定文件保存的本地路径
	class CFDownloadJob : public CFTransferJobBase
	{
	public:
		FTLINLINE CFDownloadJob(const CAtlString& strAgent);
	protected:
		CAtlString m_strLocalFilePath;
        FTLINLINE VOID SetDeleteWhenCancel(BOOL bDelete);
        FTLINLINE BOOL GetDeleteWhenCancel();
		FTLINLINE virtual BOOL _CheckParams();
		FTLINLINE virtual BOOL _SendRequest();
		FTLINLINE virtual BOOL _ReceiveResponse();
		FTLINLINE virtual BOOL _OnOpenTargetFile(HANDLE hFile) { UNREFERENCED_PARAMETER(hFile); return TRUE; }
    private:
        BOOL    m_bDeleteWhenCancel;
	};

	//并行下载
	class CFParallelDownloadJob : public CFDownloadJob
	{
	public:
		FTLINLINE CFParallelDownloadJob(const CAtlString& strAgent);
		FTLINLINE virtual BOOL _CheckParams();
	protected:
		LONG m_nBeginPos;
		LONG m_nEndPos;
		FTLINLINE virtual BOOL _OnOpenTargetFile(HANDLE hFile);


	};

	class IInternetTransferCallBack : public IFThreadPoolCallBack<FTransferJobInfoPtr>
	{
	public:
		//Download
		FTLINLINE virtual BOOL OnPromptSaveFile(__in LONG nJobIndex, __inout CFJobBase<FTransferJobInfoPtr>* pJob , 
			__in LONGLONG nTotalFileSize, __inout LPTSTR pszFileName, __in DWORD nBufferLenght) 
        {
            UNREFERENCED_PARAMETER(nJobIndex);
            UNREFERENCED_PARAMETER(pJob);
            UNREFERENCED_PARAMETER(nTotalFileSize);
            UNREFERENCED_PARAMETER(pszFileName);
            UNREFERENCED_PARAMETER(nBufferLenght);
            return TRUE; 
        }
	};

	class CFInternetTransfer
	{
	public:
		FTLINLINE CFInternetTransfer( );
		FTLINLINE virtual ~CFInternetTransfer( void );

		FTLINLINE BOOL IsStarted();
		FTLINLINE BOOL Start(IInternetTransferCallBack* pCallBack = NULL, // IFThreadPoolCallBack<FTransferJobInfoPtr>* pCallBack = NULL, 
			LONG nMinParallelCount = 1, 
			LONG nMaxParallelCount = 4, 
			LPCTSTR pszAgent = NULL);
		FTLINLINE BOOL Stop();
		FTLINLINE void Close();
		
		FTLINLINE LONG AddDownloadTask(FTransferJobInfoPtr pDownloadJobInfo);
		FTLINLINE LONG AddUploadTask(FTransferJobInfoPtr pUploadJobInfo);
		FTLINLINE LONG AddTask(CFTransferJobBase* pTransferJob);

		FTLINLINE BOOL CancelTask(LONG nJobIndex);
	protected:
		CAtlString		m_strAgent;
		CFThreadPool<FTransferJobInfoPtr>*		m_pThreadPool;
	};

	//////////////////////////////////////////////////////////////////////////

	//URL 地址解析似乎只用 URL_COMPONENTS(不需要定义成员?) + InternetCrackUrl 即可?
	//URL中不能包含某些特殊字符，因此需要转换成 %十六进制数据 的形式， 如："%" => "%25"; "?" => "%3F", "#" => "%23", "&" => "%26" 等
	class CFUrlComponents : public URL_COMPONENTS
	{
	public:
		FTLINLINE CFUrlComponents();
		FTLINLINE BOOL ParseUrl( LPCTSTR pstrURL, DWORD& dwServiceType,
			WORD& nPort, DWORD dwFlags );
	public:
		TCHAR m_szScheme[INTERNET_MAX_SCHEME_LENGTH];
		TCHAR m_szHostName[INTERNET_MAX_HOST_NAME_LENGTH];
		TCHAR m_szUserName[INTERNET_MAX_USER_NAME_LENGTH];
		TCHAR m_szPassword[INTERNET_MAX_PASSWORD_LENGTH];
		TCHAR m_szUrlPath[INTERNET_MAX_URL_LENGTH];
		TCHAR m_szExtraInfo[INTERNET_MAX_PATH_LENGTH];
	};

	//////////////////////////////////////////////////////////////////////////
#if 0
	#define __HTTP_VERB_GET			TEXT("GET")
	#define __HTTP_VERB_POST		TEXT("POST")
	#define __HTTP_ACCEPT_TYPE		TEXT("*/*")
	#define __HTTP_ACCEPT			TEXT("Accept: */*\r\n")
	#define __SIZE_BUFFER			1024

	//TODO:TimeOut
	class CFGenericHTTPClient
	{
	public:
		struct GenericHTTPArgument
		{							// ARGUMENTS STRUCTURE
			ATL::CAtlString	szName;
			ATL::CAtlString  szValue;
			DWORD	dwType;

			GenericHTTPArgument()
			{
				szName = TEXT("");
				szValue = TEXT("");
				dwType = 0;
			}
			bool operator==(const GenericHTTPArgument &argV)
			{
				if (argV.dwType == dwType
					&& argV.szName == szName
					&& argV.szValue == szValue)
				{
					return true;
				}
				return false;
				//return !lstrcmp(szName, argV.szName) && !_tcscmp(szValue, argV.szValue);
			}
		};

		enum RequestMethod
		{
			// REQUEST METHOD
			RequestUnknown = -1,
			RequestGetMethod = 0,
			RequestPostMethod,
			RequestPostMethodMultiPartsFormData
		};

		enum TypePostArgument
		{
			// POST TYPE
			TypeUnknown = -1,
			TypeNormal = 0,
			TypeBinary
		};

		FTLINLINE CFGenericHTTPClient();
		virtual ~CFGenericHTTPClient();

		// HTTP Method handler 
		BOOL Request(LPCTSTR szURL, LPCTSTR szAgent,
			RequestMethod nMethod = RequestGetMethod);
	protected:
		BOOL Connect(LPCTSTR szAddress, 
			WORD nPort = INTERNET_DEFAULT_HTTP_PORT,
			LPCTSTR szAgent = NULL,
			LPCTSTR pszCookie = NULL,
			LPCTSTR szUserAccount = NULL, 
			LPCTSTR szPassword = NULL);
		BOOL Close();

		BOOL RequestGet(LPCTSTR szURI);

	private:
		std::vector<GenericHTTPArgument>	m_vArguments;				// POST ARGUMENTS VECTOR
		std::vector<CAtlString>				m_vRequestHeaders;

		CFStringFormater	m_formaterHTTPResponseHeader;
		CFStringFormater	m_formaterHTTPResponseHTML;
		//TCHAR		m_szHTTPResponseHeader[__SIZE_HTTP_BUFFER];	// RECEIVE HTTP HEADR
		//TCHAR		m_szHTTPResponseHTML[__SIZE_HTTP_BUFFER];		// RECEIVE HTTP BODY

		HINTERNET	m_hOpen;				// internet open handle
		HINTERNET	m_hConnection;			// internet connection hadle
		HINTERNET	m_hRequest;				// internet request hadle
		
		DWORD		m_dwError;				// LAST ERROR CODE
		DWORD		m_dwResultSize;
	};
#endif 

	class CFWebBrowserDumper : public CFInterfaceDumperBase<CFWebBrowserDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFWebBrowserDumper);
	public:
		FTLINLINE explicit CFWebBrowserDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFWebBrowserDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//仅在IE中实现
	class CFWebBrowserAppDumper : public CFInterfaceDumperBase<CFWebBrowserAppDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFWebBrowserAppDumper);
	public:
		FTLINLINE explicit CFWebBrowserAppDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFWebBrowserAppDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFWebBrowser2Dumper : public CFInterfaceDumperBase<CFWebBrowser2Dumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFWebBrowser2Dumper);
	public:
		FTLINLINE explicit CFWebBrowser2Dumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFWebBrowser2Dumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

}

#ifndef USE_EXPORT
#  include "ftlNet.hpp"
#endif

#endif //FTL_NET_H