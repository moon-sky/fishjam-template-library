
#ifndef MULTI_MEDIA_GLOSSARY_H
#define MULTI_MEDIA_GLOSSARY_H
//多媒体基本概念 --  http://blog.csdn.net/hhhbbb/article/details/7619857

/******************************************************************************************************
* 音视频编码基本概念
*   在相同的采样率下，记录低频的信息远比高频的详细(低频时每次振动采样的次数比高频时多)，
*   较高的采样率只有相对模拟信号的时候才有用，而对数字信号无用(即用48KHz对44.1KHz的CD采样无法提高音质,保持相同的44.1KHz效果最好)
*   人耳能听到声音的频率介于 20Hz~20KHz 之间，最容易听到的就是4KHz，音频编码时通常过滤很多听不到的声音，
*   以简化信息复杂度，增加压缩比 -- 根据“人耳听觉心理模型”，采用VBR技术进行编码
*
******************************************************************************************************/

/******************************************************************************************************
* 1080/50i -- ???电视原本都是隔行的,N制60HZ,就是60I了,P制50HZ,就是50I
* 
* 1080i -- 采用隔行(interlace)扫描技术的高清规格，采用2:1隔行扫描，把1920×1080像素的一帧分成两场扫描。
* 1080p -- 逐行1080扫描，每帧画面都有1080线。1080p模式仅在HDMI连接输出时可实现。
* 1080/50i -- 中国高清电视的制作和播出标准，1080/24P 为节目制作和交换标准。
* 24P TRUE CINEMA -- 电影是以每秒24帧来拍摄的，蓝光播放器可以支持24帧播放，可享受到仿佛置身剧院般的影视体验。
* 720p  -- 采用逐行(progressive)扫描技术的高清规格，按1280x720像素编码
* AACS(Advanced Access Content System) -- (蓝光)版权保护，负责保护光碟的内容，是数码版权保护中重要的一环
* ABR(Average Bitrate) -- 平均比特率，是VBR的一种插值参数。LAME针对CBR不佳的文件体积比和VBR生成文件大小不定的特点独创了这种编码模式。
*   在指定的文件大小内，低频和不敏感频率使用相对低的流量，高频和大动态表现时使用高流量，作为VBR和CBR的一种折衷选择。
* ADSL(Asymmetrical Digital Subscriber Line) -- 非对称数字用户线。
* ASF(Advanced Streaming Format)--高级流媒体格式，微软定义的媒体文件类型，只是一种容器，本身没有规定音视频的压缩格式，在ASF文件中，
*   可以包含任何格式的压缩(包括MPEG-4)或非压缩的数据。需要使用专门的Windows Media Format SDK 开发。
*   如果生成的Windows Media 文件只包含音频，一般使用.wma扩展名，如果包含了视频，一般使用.wmv扩展名
* ASP(Advanced Simple Profile) -- [MPEG4]高级简洁框架，其中有 Baseline/Main/High 等多种?
* AVCHD(Advanced Video Codec High Definition) -- 用摄像机纪录高清影像的一个规格，通过文件系统对应各种各样的媒体。
*   在向BD、DVD刻录时Bitrate有上限：BD 24Mbps 以下；DVD 18Mbps 以下。
* AVI -- 
* AVIndex(?) --管理Sony制DSC、HandyCam拍摄的Contents的Database。
*   管理内容分为 Content(AVCHD HD、MPEG2 SD、DCF静止画和 MPLS ) 和 PlayList(AVCHD HD、MPEG2 SD)。
*   可以保存 用户信息(收藏、ShareMark、Event) 和 系统信息(笑脸、小孩、连写1~N)
*   DSC管理静止画(JPEG)、动画(MPEGX,MP4[AVC/M4V])，如果坏了的话，Set启动时会自动修复；
*   HandyCam管理AVCHD(MPEG2-TS(AVC))、SD(MPEG2-PS)。即使坏了，也不会自动修复，因此有 Salvage(？) 的功能。
*   有三个文件：AVF_INFO文件夹中的AVIN0001.INP(Property信息)、AVIN0001.BNP、AVIN0001.INT(Thumbnail Data)
* BD-J -- 
* BD+ -- 一个微型虚拟机器储存于蓝光光碟，给予获得授权的蓝光光碟播放器可以播放蓝光光碟。
* BDAV -- 
* BDMV(Blu-ray Disc Movie)--蓝光光盘存储视频时的格式，也称做BD-Video。
* Blu-ray Disc（蓝光光碟，英文简称BD） -- 由SONY及松下电器等企业组成的"蓝光光碟联盟"（Blu-ray Disc Association,
*   前身是 Blu-ray Disc Founders -- BDF）策划的次世代光碟规格。采用波长405纳米(nm)的蓝色激光光束来进行读写操作。
*   容量：25 GB (单层)，50 GB (双层)，100 GB (四层)，200 GB (八层)。
*   加入了Java技术(BD-J)，蓝光光碟播放器跟蓝光光碟可以实现复杂的交互式功能。
*   蓝光光碟主要以3种方式进行数位版权管理（DRM）: AACS、BD+、ROM Mark；
*   蓝光光碟使用CDFS格式，可以用蓝光烧录机写入资料，分为BD-R（单次烧录）及BD-RE（多次烧录）格式。
* BRAVIA -- 
* Bus -- 总线类型，目前有ATAPI,SCSI,IEEE1394,USB,USB2 等几种。
* CBR(Constants Bit Rate) -- 固定码率，文件从头到尾都是一种位速率，文件体积很大，但音质相对于VBR和ABR不会有明显的提高。
* CC(Closed Caption) -- 隐藏的带有解释意味的字幕, IAMAnalogVideoEncoder 接口控制
* CC(ConnectionCondition) -- AVCHD 中使用的，表明两个Content连接的条件，通过枚举 CCType 进行表示(保存在PlayItem中？)。分为：
*   CC1(一个不分割或已经分开的媒体mts，在PC或Set上--时间比较短，不需要分割？)
*   CC1n6(一个已经合并的M2TS，在PC上，需要是NTFS格式)
*   CC6(在FAT32或Set上，分割出的多个小于2G的文件)
*   在 PlayListMark 中保存，其中还有 clpi、PlayItem 等信息，如果导入打上了手动分割点的素材时，需要进行剪切。
*   分割素材时使用包含在Clpi中的 EpMap(pts 和 spn 的列表) 的信息，分割时只能按 GOP 单位进行，从规格上说Size必须是6144Byte的倍数。
*   分割点到EPMap边界之间用Null Packet(除Header以外全部0xFF的Packet)覆盖。
* CD -- 采用780纳米波长
* ConvertedContent -- 管理实际进行写返回处理的类，(即目的？)
* CUDA(Compute Unified Device Architecture) -- 显卡厂商NVidia推出的运算平台。
*   nVidia的H.264/AVC 硬件GPU编码? 解码是 DXVA? ATI 的是 OpenCL? Intel第三代CPU上是Quick Sync Video?
* DCF(Design rule for Camera File system)--数码相机的统一记录格式。
*   http://it.jeita.or.jp/document/publica/standard/exif/english/jeida49e.htm#a
* DCIM(Digital Camera In Memory ?)--数码相机用于存储照片的文件夹
* DDM(Data Disc Maker) -- 数据光盘
* Deinterlace(反交错)-- 将传统模拟电视隔行625线提高到720P的高像素级别,去除运动图像带来的消除图象边缘模糊现象,
*   成倍提高细节锐利度,使画面更富层次感和真实感。
* DLNA(Digital Living Network Alliance) --
* DRM(Digital Rights Management) -- 数字权限保护
* DSC(Digital Still Camera)--数码照相机。
* DTS(Decode Time Stamp) -- 解码时间戳，标识读入内存中的Bit流在什么时候开始送入解码器中进行解码，在解码阶段使用，受GOP中B-帧的影响,
*   声音的DTS和PTS相同，某些视频编码格式（采用了双向预测技术，即有B帧时）DTS和PTS不一致
* DV(Digital Video) -- 数字摄像机，使用 DV-AVI 2.0 + LPCM 的形式保存？
* DVD(Digital Video Disc) -- 数字视频光盘。采用650纳米波长的红光读写器。可用分辨率为：720x480(NTSC制式)、720x576(PAL制式)等。
*   DVD-5 -- 容量4.7GB的单面单层DVD片。DVD-9 -- 容量为8.5GB的单面双层DVD片。
*   ? 播放时有 Video/Audio/SubPicture三部分。解码后CC由Line21Dec处理，音频由DolbyDigital、LinerPCM解码
* DVR -- 数字摄像机
* DVR-MS(*.dvr-ms) -- 由 Windows XP Service Pack 1 引入的流缓冲引擎（Stream Buffer Engine，SBE）创建的媒体文件，Media Center 
*   用它存储录制的电视节目。
* Exif(Exchangeable Image File Format)--可交换图像文件格式。是专门为数码相机的照片设定的，可以记录
*   数字照片的属性信息和拍摄数据。http://www.exif.org
* Filmroll -- 将动画按缩略图展开。
* FOURCC(four-character code) -- 四字节码，用于表征多媒体的数据类型，如 "RIFF","AVI ","LIST" 等,通过宏 FCC('RIFF')来定义。
*   FOURCCMap fccMap = FCC('LIST'); pMediaType->subtype = static_cast<GUID>(fccMap); 是 mmioFOURCC ?
* GOP(Group Of Pictures) -- 由固定模式的一系列I帧、P帧、B帧组成。两个I帧之间形成一个GOP。
*   I-帧(即关键帧)采用帧内编码，通常是每个GOP的第一个帧，经过适度地压缩，做为随机访问的参考点，可以看成是压缩后的图像，可以直接解压成单独的完整图片。
*   P-帧采用预测编码，利用图像序列中前面已编码帧的时间冗余信息来压缩传输，需要参考前一个I-帧或B-帧来生成完整的图片
*   B-帧采用双向预测内插编码，既考虑与源图像序列前面已编码帧，也顾及源图像序列后面已编码帧之间的时间冗余信息来
*     压缩传输数据量的编码图像，
*   I帧编码是为了减少空间域冗余，P帧和B帧是为了减少时间域冗余。
*   在没有B帧存在的情况下DTS的顺序和PTS的顺序应该是一样的。对于存在双向预测帧的情况，通常要求解码器对图像重排序，以保证输出的图像顺序为显示顺序
* Handycam(Handy Camera) -- Sony的数码摄相机品牌，具有袖珍、轻巧、方便使用和携带的特点。存储介质包括磁带(Type)、
*   DVD光盘、硬盘(HDD)和闪存(Memore Stick)等，现在已经支持高清(SD使用M2PS格式，HD使用AVCHD格式[m2ts?]，静止画使用DCF)。
* HD DVD(High Definition DVD) -- 东芝主导的高清标准，已经失败给Blu-ray。
* HDMV(high definition movie mode) -- 
* HDV -- 可以在传统 Mini DV 磁带上记录 1080i(AVC) 的高清视频，并同时兼容标清 DV 拍摄和输出。使用 MPEG2-TS + MPEG-1 Audio Layer II 保存。
* HDTV  -- High Definition TV,高清电视，利用数字压缩技术可以把HDTV的地面广播谱压缩到现有的模拟电视频道范围内
*   (6MHz或8MHz)，是新世纪的电视。
* IPP(Intel Performance Primitive) -- 对应多核并且功能丰富的一个Library。包括一些面向多媒体、数据处理、
*   通信应用程序的，而且被优化的函数。
* ISO(International Standards Organization) -- 国际标准化组织。
* ISO 9660--当前唯一通用的光盘文件系统，任何类型的计算机都支持它，所有的烧录软件也都支持它。
* ITU(International Telecommunications Union)-- 国际电信联盟。
* ITU-T(TforITU Telecommunication Standardization Sector) -- 国际电信联盟远程通信标准化组。国际电信联盟管理下的
*   专门制定远程通信相关国际标准的组织。
* JVT()--联合视频组。有ISO/IEC 与 ITU-T联合组成，制定视频压缩编码标准。
* LAME -- 目前最好的MP3编码引擎,免费，可以设置VBR、ABR、CBR多种编码模式.
*   注意：默认情况下lame编码每次输出的数据不是刚好一帧，如果解码程序把每一个数据包都作为一帧进行解析，会无法解析。
*         可以在写AVI时对Lame数据进行缓存，查找每一帧再写入AVI。
* MasterContent -- 管理 BD Author时的源素材(PMB 或 Set内BDMV文件夹内)，主要用于UI显示等。对应于ConvertedContent。
* Mode -- 视频源的模式。分为VideMode和FilmMode。
*   Video Mode 通常指用摄像机拍摄的，NTSC(29.97fps) 和 PAL(25fps)。
*   Film Mode 是电影模式，按每秒24帧(fps)拍摄
* Modd(MOvie aDditional Data) -- 保存动画文件的附加信息的附属文件。使用SfCommonLib::SfModdXML类进行读写。
* Moff(MOvie Filmroll File) -- 动画缩略图文件。从MOFF文件中取得缩略图比直接从动画文件中取得要快，提高了效率。
* Motion JPEG -- 将多个AVI连起来？是AVI的一种
* MPEG(Motion Picture Experts Group) 
* MPLS(Movie PlayLists) -- (AVCHD 上的一种文件？ AVIndex 管理的一种Content)
* MultiLUN(Multiple Logical Unit Number)--逻辑单元号（LUN）指的是一个用于SCSI总线的唯一的识别号，它总线使得
*   能区别其他八个设备（它们每个都是一个逻辑单元）。每个LUN是一个能够识别特定逻辑单元的唯一识别号，
*   它可能是一个最终用户、一个文件或者一个程序。如一个USB硬盘可能会有N个分区。
* NTSC(National Television System Committee)--全国电视制式委员会。由美国国家电视标准委员会（NTSC）制定的
*   彩色电视广播标准。两大主要分支是NTSC-J与NTSC-US。美国、日本等地使用。
*   属于同时制，帧频为每秒29.97fps，525条扫描线，逐行扫描，画面比例为4：3，分辨率为720x480。
* OGG -- 一个庞大的多媒体开发计划的项目名称， 将涉及视频音频等方面的编码开发，提供完全免费多媒体编码方案
* PAL(Phase Alternating Line) -- 逐行倒相，电视广播中色彩调频的一种方法。625条扫瞄线，25fps，隔行扫瞄，PAL色彩调频。
*   兼容原有黑白电视广播格式的情况下加入彩色讯号。中国、英国、香港等地使用(欧洲标准)。
* PanoramaJPEG -- 360度全景摄影(パノラマ撮影)产生的JPEG
* PCM(Pulse Code Modulation) --  脉冲编码调制，音质好但体积大。
* PlayListMark -- (AVCHD中的手动分割点？)
* PMB(Picture Motion Brower) -- 也称为Foothold，PictureGear Studio
* PND(Personal Navigation Device) -- 
* Progressive() -- ?
* Progressive JPEG -- 一种高级JPEG格式，在装入图像时，开始只显示一个模糊的图像，随着数据的装入，图像逐步变得清晰。
* PSNR -- 峰值信噪比,表示信号最大可能功率和影响它的表示精度的破坏性噪声功率的比值的工程术语，常用对数分贝单位来表示。
* PTP(Picture Transfer Protocol) -- 图片传输协议，由柯达公司与微软协商制定的一种标准，符合这种标准的图像设备
*   在接入Windows XP系统之后可以更好地被系统和应用程序所共享，系统可以直接访问这些设备用于建立网络相册时图
*   片的上传、网上聊天时图片的传送等。
*   自动检出时启动的对话框(不要最后的GUID?)：HKLM\SYSTEM\ControlSet001\Control\StillImage\Events\Connected\{00F3CDFD-5D2E-439F-8900-3F56A0C1C8BA}
* PTS(Presentation Time Stamp) -- 显示时间戳，用于度量解码后的视频帧什么时候被显示出来，用于视频的同步和输出，（通常就是显示顺序？）
* PureVideo -- Nvidia 发布的显卡影片解码以及加速计算技术，GPU内建的可编程视频处理引擎可以对MEPG-2、HD Video视频播放进行硬件加速，
*   达成家庭影院级别的720p和1080i视频画质。需要安装nVIDIA DVD Decoder。支持格式为MEPG2、WMV、H.264、VC-1等。
*   软件方面有两部分：图形处理器驱动(免费) 和 PureVideo DVD解码器(收费)
* Re-Mux -- ？(MPEG-2 TS的文件的一部分存在Audio和Video没有同步的情况，需要进行Re-mux取得同步)
* RIFF(Resource Interchange File Format) -- 使用四字符码FOURCC(小端表示?)来表征数据类型。
*   文件格式为: "RIFF" + 四字节数据大小(实际数据长度加4) + FOURCC的类型(如 "AVI ") + 实际数据，
*     注意：数据大小不包括RIFF和数据大小区域，等于文件大小-8
*     List(列表) -- 列表可以嵌套子列表和块。格式为：
*       "LIST" + 四字节的listSize(listData长度加4) + listType(如 hdrl, strl等) + listData
*     Chunk(块) -- 格式为：
*       四字节的ckID + 四字节的ckSize(ckData的长度) + ckData
*
* RLE(Run-Length Encoded) -- 一种BMP的压缩方式。
* RODD(ROw aDditional Data) -- 保存RAW文件的附加信息的附属文件，格式同MODD完全一样。
* ROM Mark -- 是一个密码封锁资料，附加于蓝光光碟的内容之中，负责监控及阻止蓝光光碟的内容受到未得到授权的
*   播放程式进行解码内容。
* SBR(Spectral Band Replication) -- 频段复制,一种新的音频编码增强算法，在mp3PRO中使用，可以在指定的位率下增加音频的带宽或改善编码效率，
*   但 解码器的算法的优劣直接影响到音质的好坏(SBR编码的数据更像是一种产生高频的命令集)
* ScrapBook -- Sony相机中的剪贴簿功能，可将保存的照片自动创建成相册。激活后，可根据拍照日期和频率在屏幕上自动配上背景画面。
*   轻触剪贴簿中显示的某张小尺寸照片，即可回放该照片。相机提供16种背景，可以挑选背景画面来装点您的照片，或使用随机背景。
* SD -- 标准清晰
* SDTV -- 数字标准高清晰度电视，采用MPEG-2压缩(同DVD一样)，用于卫星和有线电视地面广播。
* Seamless -- （SD 和静止画时有效，具体是什么意思？)
* SOI(Start Of Image)--SOI在Exif中被定义为一个Marker Segments，每个Marker的单位是2个字节，即一个word类型
*   的数值，SOI Marker在文件起始的位置。JPEG格式文件的SOI值为FFD8，通常SOI!=FFD8的JPEG文件是不被支持的格式。
* SPU(Sony Picture Utility) -- 提供 Sony Digital Camera & HandyCam 媒体管理提供统一化管理
* STI(Still Image Architecture)--?
* Time-Shifting(时间平移) -- 即同步进行录、放的功能，如足球赛时离开电视，只需使用"Time-Shifting"功能键即可录制，然后回来继续播放
* Transfer JET -- Sony（专用的？）一种非接触时传输方式。JET设备链接PC，将DSC/CAM放在JET设备上（无需wire连接），
*  就可以跟PC端进行传输。目前：在Importer时，支持，Exproter时不允许。理由：不稳定，容易断开。
* UDF(Universal Disc Format)--采用标准的封装写入技术（PW，Packet Writing）将CD-R/CD-RW当作硬盘使用，
*   用户可以在光盘上修改和删除文件。利用UDF格式进行刻录时，刻录软件将数据打包，并在内存中临时建立
*   一个特殊文件目录表，同时接管系统对光盘的访问。
* VBR(Variable Bit Rate)-- 动态码率，压缩软件在压缩时根据音频数据即时确定使用什么比特率，这是以质量为前提兼顾文件大小的方式，
*   是推荐的编码模式，通常能够在质量与体积之间取得绝佳的平衡。
* VCD -- 数字激光视盘
* VCEG() -- 视频编码专家组，属于ITU-T
* VBI(Vertical Blanking Interval) -- 视频采集中使用的
* VFW(Video for Windows) -- 微软92年推出的关于数字视频的一个  VFW logo软件开发包，VFW的核心是AVI文件标准。
*    围绕AVI文件，VFW推出了一整套完整的视频采集、压缩、解压缩、回放和编辑的API。
* VHS(Video Home System) -- 家用录像系统。
* VOB(Video OBjects) -- 视频目标文件,DVD的视频文件，由一个视频(MPEG2)、多个声音(AC3/LPCM等)和字幕数据流组成。
*   一张DVD最多可以有9种伴音和32种字幕。
* VOD(Video-On-Demand) -- 视频点播。用户通过反向信道点播想要看得节目，从中央数据库检索到所需的节目，经压缩
*   后由正向信道传给用户。主要有三种VOD客户端系统：
*   TVOD(True VOD)--即点即播；
*   NVOD(Near VOD)--准实时点播；
*   IVOD(Interactive VOD)--交互式点播
* WDM(Windows Driver Model)--
* WIA(Windows Image Acquisition)--Windows 图象采集，WinME、WinXP等系统引入。提供广泛的支持以启用Imaging设备
*   (如扫描仪和数码相机)。与图像应用程序(如PhotoShop)对接。使用WIA，制造商只需要写 WIA microdriver(使用WIA DDI？)
*   以允许设备使用，而其他WIA组件由操作系统实现。WIA 是使用进程外服务实现的COM组件,提供自己的数据传送机制
*   (IWiaDataTransfer接口并使用共享内存)，避免了图像数据传送过程中的性能损失。
*   可用于：1.枚举可用的图像获取设备;2.同时连接多个设备;3.查询设备属性，获取数据;4.获取大量的设备事件通知消息 等。
*   http://msdn.microsoft.com/en-us/library/dd393559.aspx
* WMA(Windows Media Authoring) -- 微软出品的，在转换为WMV时的认证(由WMVCore.dll提供，该Dll还提供了Playback功能)
* WMT(Windows Media Technology)--
* WST(World Standard Teletext)
* x.v.Colour色域标准 -- 索尼提出的针对xvYCC国际色域标准的一种术语，该标准涉及的色彩范围比目前使用的广播电视标准（sRGB）更广
* xdb()--(Sony)实机的信息管理数据库，其中保存了Meta data、albums和favorite等信息。
*******************************************************************************************************/

Windows Media Player7 SDK
Windows Media Format 7.1.1 SDK 
Windows Media Encoder 7.1 SDK 
Windows Media Rights Manager 7.1 SDK 
Windows Media Services SDK 


#endif //MULTI_MEDIA_GLOSSARY_H