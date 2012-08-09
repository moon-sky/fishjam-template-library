#ifndef FTL_NET_H
#define FTL_NET_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlNet.h requires ftlbase.h to be included first
#endif

#include "ftlBase.h"
#include "ftlthread.h"
#include <WinInet.h>

/*************************************************************************************************************************
* 网络抓包工具
*   Wireshark -- 过滤：Capture->Options->Capture Filter->HTTP TCP port(80) 
*************************************************************************************************************************/


/*************************************************************************************************************************
* 通信软件要考虑的：服务的初始化和分布,并发控制,流控制,错误处理,事件循环继承,容错等
* 设计上考虑无连接和面向连接协议的时，主要涉及：延迟(latency)、可伸缩性(scalability)、可靠性(reliability)
*   无连接协议 -- 面向消息，每条消息独立寻址和发送，常用的有UDP和IP，常用于多媒体直播等允许一定程度数据丢失，但要求实时性的场合
*   面向连接协议 -- 提供可靠、有序、不重复的发送服务，为保证可靠性采用了对传送的每个数据字节编号、校验和计算、数据确认(ACK)、
*     超时重传、流量控制(通过滑动窗口由接受方控制发送方发送的数据量) 等一系列措施。
*     常用于电子邮件等不允许数据丢失的场合，还要作出如下设计选择：
*     数据成帧策略--面向消息(如 TP4和XTP)；面向字节流(如 TCP)，不保护消息边界
*     连接多路复用策略--多路复用(多个线程共享TCP连接，难于编程)；非多路复用(每个线程使用不同的连接，同步开销小，但伸缩性不好)
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
*************************************************************************************************************************/

/*************************************************************************************************************************
* TCP粘包(发送方发送的若干包数据到接收方接收时粘成一包，接收缓冲区中后一包数据的头紧接着前一包数据的尾)，可能造成粘在一起的数据包不完整，
* 若传输的数据为带结构的数据，需要做分包处理(如果是文件传输等连续数据流，就不必分包)
*   发送方原因：TCP为提高传输效率，会收集到足够多数据后才发送(Nagle算法)
*   接收方原因：接收方用户进程不及时从系统接收缓冲区接收数据，下一包数据到来时就接到前一包数据之后
* 
* 较好的对策：
*   接收方创建一预处理线程，对接收到的数据包进行预处理，将粘连的包分开
*************************************************************************************************************************/

/*************************************************************************************************************************
* ARP(Address Resolution Protocol)--地址转换协议，用于动态地完成IP地址向物理地址的转换。
* BOOTP(BooTstrap Protocol)--可选安全启动协议，使用UDP消息，提供一个有附加信息的无盘工作站，通过文件服务器上的内存影像来启动。
* Cookie(Cookies) -- 网站为了辨别用户身份、进行session跟踪而储存在用户本地终端上的键值对数据（通常经过加密）。
*   定义为 RFC2109（已废弃）和 RFC2965。最典型的应用是自动登录、购物车等
*   服务器端生成 -> 发送给UserAgent(浏览器) -> 浏览器保存成文本 -> 下次访问时发送 -> 服务器验证
*   生存周期 -- Cookie生成时指定的Expire，超出周期Cookie就会被清除。单位为？生存周期设置为"0"或负值表示关闭页面时马上清除。
*   安全问题：1.识别不精确。在同一台计算机上使用同一浏览器的多用户群，cookie不会区分他们的身份（除非使用不用的用户名登录OS）
*             2.网页臭虫的图片(透明且只有一个像素-以便隐藏)，将所有访问过此页面的计算机写入cookie，方便网站发送垃圾邮件
* ICMP(Internet Control Message Protocol)--互连控制消息协议，主要用来供主机或路由器报告IP数据报载传输中可能出现的不正常情况。
* IP(Internet Protocol)--互联协议，规定了数据传输的基本单元(报文分组)以及所有数据在网际传递时的确切格式规范。
* LANA(LAN Adapter)Number -- 对应于网卡及传输协议的唯一组合
* MTU(Maximum Transmission Unit) -- 最大传输单元 
* NetBios Name -- 微软网络中的机器名采用的便是NetBIOS名字，机器启动时，会将名字注册到本地的WINS。
*   通过 nbtstat 命令可以列出信息。
* OSI -- 开放系统互连，应用层、表示层、会话层、传输层、网络层、数据链路层、物理层；
* OOB(Out Of Band) -- 带外数据(紧急数据)，通过独立的逻辑信道来接收和处理。尽量不要使用。
* TCP(Transmission Control Protocol) -- 传输控制协议，面向连接
* RARP(Reverse Address Resolution Protocol)--用于动态完成物理地址向IP地址的转换。
* TTL -- 生存时间，表示一个数据包在丢弃之前，可在网络上存在多长时间，值为0时，包被丢弃
* UDP(User Datagram Protocol) -- 用户数据报协议，无连接
* WINS -- Windows互联网命名服务器,维护着已注册的所有NetBIOS名字的一个列表
* WinSock -- 是网络编程接口，而不是协议，与协议无关。可以针对一种具体协议（如IP、TCP、IPX、lrDA等）创建套接字。
*  “面向消息”（保护消息边界--每次读取返回一个消息，如网络游戏的控制包）和 “面向流”（连续的数据传输，会尽量地读取有效数据）
*  “面向连接”和“无连接”
*   面向连接的协议同时也是流式协议，无连接协议几乎都是基于消息的。
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
* 组播用的是D类IP地址224.0.0.0 -- 需要对方加入你的组播组
* 子网广播 -- 广播得看你的网络号是几位的，现在都是CIDR标记路由，主机号全1就是广播地址
*************************************************************************************************************************/


/*************************************************************************************************************************
* Socket模式 -- 决定在随一个套接字调用时，那些WinSock函数的行为
*   阻塞模式(缺省)：在I/O操作完成前，执行操作的Winsock函数(如send和recv)会一直等下去，不会立即返回程序。
*   非阻塞模式：Winsock函数无论成功了(返回0)还是失败都会立即返回，失败时返回SOCKET_ERROR，错误码为 WSAEWOULDBLOCK，
      通常需要频繁的调用判断(效率低，应该使用Socket模型进行异步)。
*     ioctlsocket(,FIONBIO,&1) -- 注意：ipmsg的端口就使用了该模式
*   IO复用：在 select/poll/WaitForMultiXXX 等系统调用上阻塞，可以一次等待多个文件、端口的多个情况(读/写/异常)   
*
* Socket模型 -- 描述了一个应用程序如何对套接字上进行的I/O进行管理及处理
*   select(选择) -- 利用select函数判断套接字上是否存在数据，或者能否向一个套接字写入数据。
*     分三种：可读性、可写性、例外。函数返回后，通过再次判断socket是否是集合的一部分来确定是否可读写。
*     缺点：不能动态的调整(如增加、删除)Socket，如进程捕获一个信号并从信号处理程序返回，等待一般会被中断（除非信号处理程序指定 SA_RESTART 且系统支持）
*   WSAAsyncSelect(异步选择) -- 接收以Windows消息为基础的网络事件通知(即网络事件来了后用消息进行处理)，MFC中CAsyncSocket的方式，模式自动变为非锁定。
*     如IPMsg中tcp用于建立连接，UDP用于读取数据：
*       ::WSAAsyncSelect(udp_sd, hWnd, WM_UDPEVENT, FD_READ)
*       ::WSAAsyncSelect(tcp_sd, hWnd, WM_TCPEVENT, FD_ACCEPT|FD_CLOSE)
*   WSAEventSelect(事件选择) -- 针对每一个套接字，使用WSACreateEvent创建一个事件(默认是手动重置)，并进行关联。
*     可用 WSAWaitForMultipleEvents 进行等待，最多支持64个；再用 WSAEnumNetworkEvents 获取发生的事件
*   Overlapped I/O(重叠式I/O) -- 具有较好的性能，使用 WSA_FLAG_OVERLAPPED 创建Socket((默认设置)，
*     并在相关Socket函数中加入 WSAOVERLAPPED 结构或使用"完成例程"，
*     调用 WSAResetEvent 重置事件(其事件是手动重置的)，通过 WSAGetOverlappedResult 获取
*     执行结果(其结果和不使用Overlapped调用ReadFile等函数时返回的结果一样)。
*     注意：请求Overlapped后，函数通常返回失败，错误码为WSA_IO_PENDING，但如果请求的数据已经在Cache中，会直接返回成功
*   IO Completion Port(完成端口) -- 和 Overlapped I/O 协同工作，可以在一个“受制于I/O的程序”中获得最佳性能。
*     没有64个HANDLE的限制，使用一堆线程(通常可设置为 CPU个数*2+2 )服务一堆Events的性质，自动支持 scalable。
*     操作系统把已经完成的重叠I/O请求的通知放入完成端口的通知队列(一个线程将一个请求暂时保存下来)，等待另一个线程为它做实际服务。
*     线程池的所有线程 = 目前正在执行的 + 被阻塞的 + 在完成端口上等待的。
*     使用流程：
*       1.产生一个I/O completion port -- hPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL...)。产生一个没有和任何文件Handle有关系的port
*       2.让它和文件handle产生关联 -- CreateIoCompletionPort(hFile,hPort...)，为每一个欲附加的文件Handle进行关联
*       3.产生一堆线程 -- for(CPU*2+2){ _beginthreadex }
*       4.让每一个线程在Completion Port上等待 -- GetQueuedCompletionStatus，可通过 CONTAINING_RECORD 宏取出其中指向扩展结构的指针
*       5.开始对着那个文件handle发出一些overlapped I/O请求 -- 如 ReadFile、WriteFile、DeviceIoControl等，
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
*
* socket API大致可以分为五类：
*   A.本地环境管理，其信息通常保存在OS内核或系统库中
*     1.socket -- 分配一个socket句柄，并返回给调用者
*     2.bind -- 将一个socket句柄和一个本地或远程地址关联起来
*     3.getsockname -- 返回socket绑定的本地地址
*     4.getpeername -- 返回socket绑定的远程地址
*     5.close/closesocket -- 释放socket句柄，使其可复用
*   B.连接的建立和终止
*     1.connect -- 主动在一个socket句柄上建立连接，如果想设置超时，利用ioctlsocket把socket设置为非堵塞的，在connect时会立即返回，
*         然后利用select函数等待 readfd 并设置超时值
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
*   D.选项管理
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
*     2.ioctlsocket -- 控制socket的IO模式，比如 FIONBIO(控制是否采用阻塞模式)
*   E.网络地址 -- 在可读性的名称(如域名)和低级网络地址(如IP)之间进行转换
*     1.gethostbyname/gethostbyaddr -- 处理主机名和 IPV4 地址之间的网络地址映射
*     2.getipnodebyname/getipnodebyaddr -- 处理主机名和 IPV6 地址之间的网络地址映射
*     3.getservbyname -- 通过具有可读性的名称标识服务
*       getpeername -- 
*     4.inet_aton/inet_ntoa -- 在点分十进制数串(如192.168.0.1)与对应的32位网络字节序二进制值间转换。只适用于IPv4
*       注意 inet_ntoa 返回的字符串存储在静态内存中（非线程安全）
*     5.inet_pton/inet_ntop <== presentaion(字符串表达)<->numeric(套接口地址结构中的二进制值)，适用于IPv4和IPv6。
*         inet_ntop(AF_INET,&cliaddr.sin_addr, buff, sizeof(buff));
*         inet_pton(AF_INET,"192.168.0.1", &addr.sin_addr, ...); 
*
* 注意：
*   1.Socket API使用一种很基本的地址结构(sockaddr),根据地址簇不同，结构成员的占位也不同，
*     如IPV4的 sockaddr_in,使用前一般要全部清零，否则容易出现错误
*   2.注意网络字节序和主机字节序的问题
*     网络字节序：端口(addr.sin_port)、IP地址
*   3.默认情况下，大多数TCP/IP实现会使用Nagle算法，会在发送端中缓存少量依次发送的数据报，通常情况下能
*     缓解网络拥塞，但可能增加延迟并降低吞吐量。可通过 TCP_NODELAY 选项关闭，禁止数据合并。
*     适用于经常单向发送小数据报(比如 IPMsg )
*       :setsockopt(info->sd, SOL_SOCKET, TCP_NODELAY, (char *)&flg, sizeof(flg));

*************************************************************************************************************************/

/*************************************************************************************************************************
* TransmitFile -- 在内核中高性能的传输文件数据
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
*   通常承载于TCP、TLS或SSL(即HTTPS，默认端口443)协议层之上，分为 POST(RFC1867) 和 GET(RFC???)
*
* HTML VERBS(字符串)
*   POST
*   GET
*   HEAD
*   PUT
*   LINK
*   DELETE
*   UNLINK
*
*
* 使用HTTP获取数据的步骤: 
*   1.客户端建立连接
*   2.客户端发送请求
*     内容包括：统一资源标识符（URL）、协议版本号，后边是MIME信息包括请求修饰符、客户机信息和可能的内容
*   3.服务器返回响应信息，格式为一个状态行，包括信息的协议版本号、一个成功或错误的代码，后边是MIME信息包括服务器信息、实体信息和可能的内容
* 
* 头域(域名:域值)，每个描述部分用"\r\n"分开，结束处有两个"\r\n"
*   [M]Accept: text/plain, x/x(注意：此处本来是*号)
*   [O]Accept-Encoding: gzip, deflate
*   [O]Accept-Language: zh-cn
*   [O]Cache-Control: 指定请求和响应遵循的缓存机制，如 no-cache, max-age 等
*   [M]Content-Type: multipart/form-data; boundary=--MULTI-PARTS-FORM-DATA-BOUNDARY
*     类型列表 -- rfc1341( http://www.ietf.org/rfc/rfc1341.txt )
*     //AVI video/avi
*     //JPG image/jpeg
*     //PNG image/x-png
*     //BMP image/bmp
*     //TIF image/tiff
*     //GIF image/gif
*     //TXT text/plain
*     //XML text/xml
*   []Content-Disposition: -- 可传输二进制文件(前面必须有 ----MULTI-PARTS-FORM-DATA-BOUNDARY )
      协议头时：form-data; name=\"attach_file\"; filename=\"xxxx\"    -- 其中的 "attach_file" 是各个网站不同的?
      协议尾时：form-data; name=\"icount\" + CRLF + CRLF + _T("1") + CRLF + 
      \"submitted\"
*   [M]Content-Length: 内容长度（包括 头 + 数据 + 尾）
*   [O]Content-Transfer-Encoding: binary
*   []Connection: Keep-Alive
*   []Cookie : ?? 是分号";"分开的多个 "变量名=值" 的键值对
*   []Date: 表示消息发送的时间，由RFC822定义，是世界标准时。如 Sat, 26 May 2012 00:42:19 GMT
*   [O]Host: 192.168.0.8，连接请求中必须包含该域?
*   []Referer: http://源资源地址， 这可以允许服务器生成回退链表，可用来登陆、优化、cache等
*   []User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0) 或自定义的名字，是发出请求的用户信息
                
* GET /文件地址 HTTP/1.1
*   []Range: bytes=388136960-   (格式是怎么样的?)
*   []Content-Range: bytes 开始位置-结束位置/总位置 (不确定是否是这个格式)

* 返回信息
*   
*   []Server: Apache/2.2.11 (Unix)
*   []Last-Modified: Wed, 23 May 2012 21:38:23 GMT
*   []ETag: "1d2d6-5c896800-4c0baf45c91c0"
*   []Accept-Ranges: bytes
*   []Content-Length: 1164371968
*   []Content-Range: bytes 388136960-1552508927/1552508928
*   []Keep-Alive: timeout=5, max=100
*
*     InternetGetConnectedState()
*     InternetOpen
*     InternetConnect
*     InternetAttemptConnect
*     HttpOpenRequest
*     HttpSendRequest
*     HttpQueryInfo
*     InternetCloseHandle(hRequest,hConnect, hOpen)
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
    #define FTL_DEBUG_EXCEPT1(x, e1) \
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
        #define FTL_DEBUG_EXCEPT1(x, e1) \
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

    FTLEXPORT class CFNetErrorInfo : public CFConvertInfoT<CFNetErrorInfo,int>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFNetErrorInfo);
    public:
        FTLINLINE explicit CFNetErrorInfo(int err);
        FTLINLINE virtual LPCTSTR ConvertInfo();
    };

    namespace NetInfo
    {
        //获取协议的地址家族：如 AF_INET / AF_INET6
        FTLINLINE LPCTSTR GetAddressFamily(int iAddressFamily);

        //获取套接字类型
        FTLINLINE LPCTSTR GetSocketType(int iSocketType);

        //获取指定地址家族的协议： 如 AF_INETx 中的 IPROTO_IP/IPROTO_TCP/IPROTO_UDP 等
        FTLINLINE LPCTSTR GetProtocolType(int iAddressFamily,int iProtocol);

        FTLINLINE LPCTSTR GetServiceFlagsType(DWORD dwServiceFlags);
        FTLINLINE LPCTSTR GetProviderFlagsType(DWORD dwProviderFlags);

        //获取本地的IP地址
        FTLINLINE LONG GetLocalIPAddress();
    }

    enum FSocketType
    {
        stTCP,
        stUDP,
    };

    enum FSocketMode
    {
        smClose,
        smAccept,
        smRead,
        smWrite,
        smPending
    };

    typedef struct tagFOVERLAPPED
    {
        OVERLAPPED				overLapped;
        WSABUF					dataBuf;
        FSocketMode             socketMode;
        volatile UINT	        nSession;
    }FOVERLAPPED, *LPFOVERLAPPED;

    FTLEXPORT template <typename T>
    class CFSocketT
    {
    public:
        FTLINLINE CFSocketT();
        FTLINLINE virtual ~CFSocketT();
        FTLINLINE int Open(FSocketType st, BOOL bAsync);
        FTLINLINE int Close();
        FTLINLINE int Shutdown(INT how);

        FTLINLINE int IoCtl(long cmd, u_long* argp);

    //protected:
        SOCKET              m_socket;
        FSocketType         m_socketType;
        BOOL                m_bASync;
        CFCriticalSection   m_lockObject;
        volatile UINT       m_nSession;
    };

    FTLEXPORT template <typename T>
    class CFClientSocketT : public CFSocketT<T>
    {
    public:
        //Client
        FTLINLINE int Connect(LPCTSTR pszAddr, INT nSocketPort);

        //Data
        FTLINLINE int Send(const BYTE* pBuf, INT len, DWORD flags);
        FTLINLINE int Recv(BYTE* pBuf, INT len, DWORD flags);
        FTLINLINE int Associate(SOCKET socket, struct sockaddr_in * pForeignAddr);
    protected:
        struct sockaddr_in		m_foreignAddr;
    };

    FTLEXPORT template <typename T>
    class CFServerSocketT : public CFSocketT< T >
    {
    public:
        FTLINLINE CFServerSocketT();
        FTLINLINE ~CFServerSocketT();
        //Server
        FTLINLINE int StartListen(INT backlog, INT nMaxClients);
        FTLINLINE int Bind(USHORT listenPort, LPCTSTR pszBindAddr);
        FTLINLINE CFClientSocketT<T>* Accept();
    protected:
        CFMemCacheT<CFClientSocketT <T> >*    m_pClientSocketPool;
    };

    FTLEXPORT template < typename T>
    class CFNetClientT
    {
    public:
        CFNetClientT(FSocketType st = stTCP);
        virtual ~CFNetClientT();
        int Create();
        int Destroy();
        int Connect(LPCTSTR pszAddr);
    protected:
        CFSocketT<T>*    m_pClientSocket;
        FSocketType      m_socketType;
    };

    FTLEXPORT template< typename T >
    class CFNetServerT
    {
    public:
        CFNetServerT(FSocketType st = stTCP);
        virtual ~CFNetServerT();
    public:

        /*
        * param [in] listenPort 
        * param [in] backlog 正在等待连接的最大队列长度
        */
        int Create(USHORT listenPort, LPCTSTR pszBindAddr = NULL );
        //int Close();
        int Destroy();
        int Start(INT backlog, INT nMaxClients);
        int Stop();
        //CFSocketT<T>* Accept();
    protected:
        FSocketType                 m_socketType;
        CFServerSocketT<T>*         m_pServerSocket;

        //CFThreadPool<DWORD>*    m_pIoServerThreadPool;
        HANDLE                  m_hIoCompletionPort;
        HANDLE                  m_hServerThread;
        HANDLE                  *m_pWorkerThreads;

        DWORD                   getNumberOfConcurrentThreads();
        static unsigned int __stdcall serverThreadProc( LPVOID lpThreadParameter );
        static unsigned int __stdcall workerThreadProc( LPVOID lpThreadParameter );
        unsigned int     doServerLoop();
        unsigned int     doWorkerLoop();
    };

    class CFSocketUtils
    {
    public:
        static FTLINLINE size_t readn(int fd, void* vptr, size_t n);
        static FTLINLINE size_t writen(int fd, const void* vptr, size_t n);
    };

	typedef std::map<tstring, tstring> CookiKeyValueMap;
	class CFNetUtil
	{
	public:
		FTLINLINE static LPCTSTR GetCookieInfo(CFStringFormater& formater, LPCTSTR lpszUrl, LPCTSTR lpszCookieName);
		FTLINLINE static DWORD GetCookieInfoMap(LPCTSTR pszCookies, CookiKeyValueMap& cookieMap);
	};

	class CUrlComponents : public URL_COMPONENTS
	{
	public:
		FTLINLINE CUrlComponents();
		FTLINLINE BOOL ParseUrl( LPCTSTR pstrURL, DWORD& dwServiceType,
			WORD& nPort, DWORD dwFlags );
	private:
		TCHAR m_szScheme[INTERNET_MAX_SCHEME_LENGTH];
		TCHAR m_szHostName[INTERNET_MAX_HOST_NAME_LENGTH];
		TCHAR m_szUserName[INTERNET_MAX_USER_NAME_LENGTH];
		TCHAR m_szPassword[INTERNET_MAX_PASSWORD_LENGTH];
		TCHAR m_szUrlPath[INTERNET_MAX_URL_LENGTH];
		TCHAR m_szExtraInfo[INTERNET_MAX_PATH_LENGTH];
	};

	//////////////////////////////////////////////////////////////////////////
	#define __HTTP_VERB_GET			TEXT("GET")
	#define __HTTP_VERB_POST		TEXT("POST")
	#define __HTTP_ACCEPT_TYPE		TEXT("*/*")
	#define __HTTP_ACCEPT			TEXT("Accept: */*\r\n")
	#define __SIZE_BUFFER			1024

#if 0
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
	enum DOWMLOAD_END_CODE
	{
		DOWN_OK = 0,
		DOWN_ERROR,
		DOWN_CANCEL
	};

	class IFDownloadCallbackEvent
	{
	public:

		virtual void OnDownloadProgress( LPCTSTR URL, LPCTSTR Path, int nCurrent ) = 0;
		virtual void OnDownloadEnd( LPCTSTR URL, LPCTSTR Path, DOWMLOAD_END_CODE errorCode ) = 0;
	};

#if 0
	class CFHttpDownloader
	{
	public:
		CFHttpDownloader( void );
		~CFHttpDownloader( void );
		void	HttpDownAsync();
		BOOL	HttpDown();

		void	Cancel()
		{
			//m_bContinue = FALSE;
		}
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