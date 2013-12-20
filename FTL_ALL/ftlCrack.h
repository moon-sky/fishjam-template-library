
#ifndef FTL_CRACK_H
#define FTL_CRACK_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlCrack.h requires ftlbase.h to be included first
#endif

//代码页转换: http://bianma.911cha.com/
//Cryptoapi说明: http://www.cnblogs.com/lzjsky/archive/2010/09/21/1832239.html
//Cryptopp 库

/*********************************************************************************************************************************
* 签名证书工具
*  Cert2Spc -- 将Cert转换成Spc
*  Pvk2Pfx.exe -- 将证书私钥文件转换为签名工具可以使用的 pfx(个人信息交换) 格式
*    -pvk 指定密钥文件 
*    -spc 指定双证书文件 
*    -pfx 输出的pfx证书文件
*  SignTool.exe -- 签名工具，主要分为 catdb/sign/verify/timestamp 等命令
*    sign 
*      /v 执行成功、执行失败或产生警告消息时生成详细输出。 
*      /a 自动选择最佳的签名证书。 
*      /c 指定用于对证书进行签名的证书模板名（一个 Microsoft 扩展），交叉签名证书。 
*      /f 指定签名证书文件（pfx格式）。 
*      /p 指定签名证书的私钥保护密码。 
*      /t 指定时间戳服务器的 URL， 确保证书过期还能正常使用
*********************************************************************************************************************************/

/*********************************************************************************************************************************
* PE 文件格式：
*   Dos Header(IMAGE_DOS_HEADER) -- DOS兼容的文件头，其 e_magic 应该为 IMAGE_DOS_SIGNATURE
*   PE Header(IMAGE_NT_HEADERS  -- (IMAGE_NT_HEADERS *)((PBYTE)pDosHeader + pDosHeader->e_lfanew)，其Signature 应为 IMAGE_NT_SIGNATURE
*   .text Section(Program Code) -- 包含了程序代码
*   .data Section(Initialized Data) -- 保存了初始化数据的数据节表
*   .idata Section(Import Table) -- 列出导入的DLL和函数的导入表
*   .edata Section（Export Table） -- 列出导出函数代码的导出表
*   Debug Symbols -- 调试符号
*  
*********************************************************************************************************************************/

/*********************************************************************************************************************************
* CA(Certificate Authority) -- 受信任的证书颁发机构
*   certificate -- 证书
*   certificate revocation list -- 证书吊销列表
*   certificate store -- 证书存储,有三种类型：User Account store、Service Account store、Local Computer store
*   certificate trust list -- 证书信任列表
* DRM(Digital Rights Management) -- 数字权限管理，主要用于数字版权加密保护。出版者用来控制被保护对象的使用权
* PEM(Privacy Enhanced Mail)
* PKI -- 公钥基础设施，是使用非对称加密理论，提供数字签名、加密、数字证书等服务的体系，一般包括CA、数字证书库、密钥备份及恢复系统、
*   证书作废系统、应用接口(API)等
*********************************************************************************************************************************/

/*********************************************************************************************************************************
* 对可执行程序(.exe/.dll/.sys等)进行签名 -- 各种参数需要对应更改
*   1.创建根证书：
*     MakeCert.exe -r -pe -ss "FujieCertStore" -n CN="FujieCert" "Fujie.cer"
*       -$ individual
*   2.信任该根证书，之后可以通过 certmgr.msc 查看和删除等
*     CertMgr.exe /add "Fujie.cer" /s /r localMachine root
*   3.对可执行文件进行签名
*     signtool.exe sign /v /s "FujieCertStore" /n "FujieCert" /t http://timestamp.verisign.com/scripts/timestamp.dll drivername.sys
*   4.[可选]验证签名 -- TODO: /kp ?
*     signtool.exe verify /pa /v drivername.sys
*
* 64位Vista/Win7驱动签名 (http://msdn.microsoft.com/en-us/windows/hardware/gg487317.aspx)
*   1.向CA证书机构(VerSign、GlobalSign 等)购买驱动签名证书(特殊种类的?)
*     a.MyCert.spc(双证书文件，包含一个CA证书和一个签名证书) + myCert.pvk(私钥文件)
*     b.转换为签名工具可以使用的pfx(个人信息交换)格式：Pvk2Pfx -pvk myCert.pvk -spc myCert.spc -pfx myCert.pfx
*   2.从微软网站下载交叉认证根证书，交叉认证根证书和驱动签名证书的CA必须是相同的，如 "VeriSign Class 3 Public Primary Certification Authority"
*     http://www.microsoft.com/whdc/driver/install/drvsign/crosscert.mspx
*   3.使用 SignTool 同时使用 交叉证书和PFX文件 进行签名
*     signtool sign /v /ac VeriSignG5.cer /f MyCert.pfx /p 密码 
*        /t http://timestamp.verisign.com/scripts/timestamp.dll EasyHook64Drv.sys
*********************************************************************************************************************************/


/*********************************************************************************************************************************
* 权限检查工具：AccessChk -- 可知道指定账号对特定目录的权限和完整性级别等
*   AccessChk -i 目录 -- 查看目录下文件的完整性级别
*   AccessChk -d -q %AppData% 
*
* IsDebuggerPresent/CheckRemoteDebuggerPresent -- 判断是否存在调试器，通常是为了防止被破解
*********************************************************************************************************************************/

/*********************************************************************************************************************************
* 网络身份认证(Authentication) -- 通过分析 netds\http\httpauth 例子而来，可能不正确，其采用了 NTLM 认证
*   0.服务器需要 HttpSetUrlGroupProperty(, HttpServerAuthenticationProperty, ); -- 允许认证
*   1.客户端HTTP 初次连接时，循环查找到 HTTP_REQUEST::pRequestInfo[nIndex].InfoType == HttpRequestInfoTypeAuth，可通过 
*     HTTP_REQUEST::pRequestInfo[nIndex].pInfo 转换为 PHTTP_REQUEST_AUTH_INFO，
*     其 AuthStatus 为 HttpAuthStatusNotAuthenticated(表示尚未认证);
*   2.Server发送 HTTP_STATUS_DENIED(401) 的 HTTP_RESPONSE 响应，其中需要通过 HttpHeaderWwwAuthenticate 指定认证方式的内容。
*   3.客户端接到 401 响应后，设置认证信息(如用户名、密码），重新发送；
*   4.再次连接时，判断 AuthStatus == HttpAuthStatusSuccess， 进行验证，如（流程是否这样？）：
*       ImportSecurityContext -> ImpersonateLoggedOnUser -> RevertToSelf -> DeleteSecurityContext 等
*   5.如果验证通过，则发送 HTTP_STATUS_OK(200) 的响应
*********************************************************************************************************************************/

/*********************************************************************************************************************************
* 二进制转换十进制：将二进制数的每一位数乘以它的权，然后相加。
*   (100110.101)2 = 1*2^5 + 1*2^2 + 1*2^1 + 1*2^(-1) + 1*2^(-3) = 32+4+2+0.5+0.125 = 38.625
* 十进制转换二进制：整数部分(除2取余)和小数部分(乘2取整)分别转换，然后再合并。
*   或 写成按二进制数权的大小展开的多项式，按权值从高到低依次取各项的系数。
*   (175.71875)10 = 128+32+8+4+2+1+0.5+0.125+0.0625+0.3125 
*                 = 2^7+2^5+2^3+2^2+2^1+2^0+2^(-1)+2^(-3)+2^(-4)+2^(-5)
*                 = (10101111.10111)2
*   
* 原码：最高位是符号位(0表示正号，1表示负号)，其余的n-1位表示数值的绝对值。
*   0的原码表示有两种：[0]原 = 0 000 0000, [-0]原 = 1 000 0000
* 反码：最高位是符号位(0表示正号，1表示负号)，正数的反码和原码相同，负数的反码则是其绝对值按位求反。
*   0的反码表示有两种：[0]反 = 0 000 0000, [-0]反 = 1 111 1111
* 补码：最高位是符号位(0表示正号，1表示负号)，正数的补码和原码相同，负数的补码则等于其反码+1。
*   0有唯一的补码 [+0]补=[-0]补=0 000 0000。利用补码，可以将减法运算转换为加法运算。
* 移码：在数X上增加一个偏移量，常用于表示浮点数中的阶码。如果机器字长为n，在偏移2^(n-1)的情况下，
*   将补码的符号位取反就可获得相应的移码表示（也可以是原码+127,1023或16383）
*   [-45]原=1 010 1101， [-45]反=1 101 0010， [-45]补=1 101 0011，[-45]补=0 101 0011
* 
* 模2运算加减法：按位运算，不发生借位和进位。
* 
* 定点数：小数点的位置固定不变的数。通常有两种：定点整数 和 定点小数，能表示的数值范围较小。
* 浮点数：小数点的位置不固定的数，能表示更大范围，使用阶码和尾数来表示。二进制数N = 2^E * F (其中E称为 阶码， F称为 尾数)
*   表示格式为：阶符|阶码|数符|尾数，其表示不唯一。浮点数能表示的数值范围由阶码决定，所表示数值的精度由尾数决定。
*   为了充分利用尾数来表示更多的有效数字，通常采用规格化浮点数(将尾数的绝对值限定在[0.5,1]的区间内)。
*   通常阶码用移码表示，尾数用原码表示。IEEE 754 标准中对各种类型小数的 指数长度规定分别是：float(8),double(11),扩充精度(15)
*   按IEEE 754标准将 176.0625 表示为单精度浮点数：(176.0625)10 = (1011 0000.0001)2 = 1.011 0000 0001 * 2^7 
*     =>(将小数部分扩展为单精度浮点数所规定的23位尾数，双精度为52,扩充精度为64) 011 0000 0001 0000 0000 0000
*     =>(计算阶码，前面指数为7，加上单精度偏移 127，双精度为1023，扩充精度为16383)E=7+127=134,则指数的移码表示为 1000 0110
*     最后可得：(176.0625)10 的单精度浮点数表示形式为 0 10000110 011 0000 0001 0000 0000 0000(0x43301000)
*     float f = 176.0625;
*     DWORD d = *((DWORD*)(&f));
*     CPPUNIT_ASSERT(d == 0x43301000);
* 
* DBCS -- 多字节编码
* 
* 汉字编码：
*   西文基本符号较少，编码容易(ASCII)，在计算机系统中，输入、内部处理、存储和输出都是用同一代码（输出应该也有字形码？）；
*   汉字种类繁多，输入、内部处理、存储和输出对汉字代码的要求不尽相同，采用的编码也不同。
*     输入码：是用西文标准键盘输入汉字时的编码，常用的有三类：数字编码(如国标区位码--94x94)、拼音码(以汉语读音为基础)、字形码(以汉字的形状确定，如五笔)
*     内部码：汉字在设备或信息处理系统内部的表达形式，是存储、处理、传输用的代码。如GB2312的汉字国标码，将GB2312两个字节的高位置1，
*       就得到了机内码(GBK内码或ASCII码)，如“大”的GB2312为3473H，机内码为B4F3H
*       UCS(Universal Character Set,也叫ISO 10646) -- 为了统一地表示世界各国的文字，为包括中日韩等国文字在内的
*         各种正在使用的文字规定了统一的编码方案。用4个字节指定 组、平面、行和字位( 2^7 x 2^8 x 2^8 x 2^8)。
*         Unicode 标准对应于 ISO 10646 实现级别 3(即支持所有的 UCS 组合字符)；
*         UTF-8 即 UCS 变形格式 8，是一种兼容于 ASCII 编码和所有 POSIX 文件系统的可变长编码
*     字形码：用于汉字输出，表示汉字字形的字模数据，通常用点阵、矢量函数等方式表示。当显示或打印输出时，检索字库，输出字模点阵得到字形。
*       根据输出的要求不同，点阵的多少也不同。如：简易型的 16x16点阵，高精度型的 24x24点阵、32x32点阵、48x48点阵等。
*       矢量表示法是将汉字看做由笔画组成的图形，提取每个笔画的坐标值，将每一个汉字的所有坐标值组合起来就是该汉字字形的矢量信息。
*
* 校验码 -- 把数据可能出现的编码分为两类：合法编码和错误编码。合法编码用于传送数据，错误编码是不允许在数据中出现的编码。
*   码距 -- 一个编码系统中任意两个合法编码之间至少有多少个二进制位不同,码距为1时无差错功能（如8421码）
*   常用的校验码：
*   1.奇偶校验码(parity code)：在编码中增加一位校验位来使编码中1的个数为奇数(奇校验)或偶数(偶校验)，码距为2，能发现错误，但不能校正错误。
*   2.汉明码(hamming code)：也是利用奇偶性来检错和纠错。在数据位之间插入k个校验位，通过扩大码距来实现检错和纠错。如 8位数据 中插入 4位校验。
*   3.CRC：广泛应用于数据通信领域和存储系统中。利用生成多项式为k个数据位产生r个校验位来进行编码，其编码长度为k+r。编码后共n个数据，其中有
*      k个信息码，则校验码占 n - k 个，称为 (n ,k)码。校验码由信息码产生，其位数越长，校验能力就越强。在求CRC编码时，采用的是模2运算。
*
* 逻辑运算
*   与(逻辑乘) -- AND
*   或(逻辑加) -- OR
*   非(逻辑求反) -- NOT
*   异或(半加) -- XOR
*   
**********************************************************************************************************************************/

/**********************************************************************************************************************************
* 编码
*   Base64 -- 网络上最常见的用于传输8Bit字节代码的编码方式，主要用于在HTTP环境下传递较长的标识信息，电邮的附件编码等。
*     2^6=64，将各种各样的字符转换为 [A-Z a-z 0-9]、"+" 和 "/" 共64个可见的字符，方便传递( 二进制 => ASCII码 )
*     算法(转换后的字符串理论上将要比原来的长1/3)：
*       1.把每三个8Bit的字节转换为四个6Bit的字节（3*8 = 4*6 = 24），然后把6Bit再添两位高位0，组成四个8Bit的字节
*       2.每76个字符加一个换行符
*       3.处理最后的结束符，原文的字节不够3的倍数则用全0来补足，转换时Base64编码用=号来代替(有些Base64编码会以一或两个等号结束)
*     分类：
*             标准Base64 -- 其中可以包含"/"和"+"字符，不适合直接放在URL中传输(URL编码器会把"/"和"+"变为形如"%XX"的形式)
*       URL 的改进Base64 -- 将标准Base64中的"+"和"/"分别改成了"*"和"-"，不在末尾填充"="号，统一了数据库、表单等处对象标识符的格式
*       正则的改进Base64 -- 将"+"和"/"改成了"!"和"-"
**********************************************************************************************************************************/

/**********************************************************************************************************************************
* 注意：
*   a.为减少计算量，常采用传统加密方法与公钥加密方法相结合的方式
*     对话密钥 等数据量小但重要的内容通过非对称加密(速度慢)
*     信息内容 等数据量大，但相对不重要的数据通过 对称密钥加密(速度快)
*   b.公钥/私钥对 -- 使用单向函数的数学原理，即公钥加密后只能用私钥解密；私钥加密后只能用公钥解密
* 
* 加密模式(不同的加密模式会影响 加解密速度 和 被破解的难度)
*     ECB(Electronic Code Book) -- 电子密码本模式
*     CBC(Cipher Block Chaining) -- 加密块链模式
*     CFB(Cipher Feedback Mode) -- 加密反馈模式
*     OFB(Output Feedback Mode) -- 输出反馈模式
*  算法模式?
*     ede -- 该加密算法采用了加密、解密、加密的方式，第一个密钥和最后一个密钥是相同的 
*     ede3——该加密算法采用了加密、解密、加密的方式，但是三个密钥都不相同
* 编码方法
*   流式编码 -- 在明码文本的每一位上创建编码位，速度较快，但安全性较低
*   块编码   -- 在一个完整的块上(如64位)上工作，需要使用填充的方法对要编码的数据进行舍入，以组成多个完整的块。
*               这种算法速度较慢，但更安全。
*   
* 1.对称密钥(对称加密算法，常用的是 AES/DES 等)：私钥加密和解密，速度快，但密钥管理困难
*   常见对称加密算法
*     [AES](Advanced Encryption Standard) -- 高级加密标准，是美国联邦政府采用的一种区块加密标准，用于替代原先的DES，
*       目前最流行的私钥数据(对称）加密算法之一。加密强度有 128/192/256 位等。
*     Blowfish
*     CAST
*     DES
*     IDEA
*     RC2
*     RC4 -- 流式编码
*     RC5 -- OpenSSL中 缺省的是128位密钥，加密次数为12次
*
* 2.非对称密钥(非对称加密算法)：公钥(PK -- Public Key)，私钥(SK -- )，加密算法(E)和解密算法(D)也是公开的，机制灵活，但速度慢。
*   基于“由已知加密密钥推导出解密密钥在计算上是不可行的”的密码体制。
*   常见用途：
*     加密发送：发送者使用接受者的公钥加密并发送，一旦加密，只能由接受方的私钥解密，但因为速度慢，往往只用于 密钥交换
*       如 数字证书就是包含对方的公钥，发送数据时通过公钥加密，然后发送
*     数字签名：用户用自己的私钥加密，可通过其公钥解密进行验证，拥有标记文件身份、分发的不可抵赖性等作用
*   常见非对称加密算法
*     DH -- 
*     DSA
*     [RSA](开发的三个人的名字首字母)-- ISO推荐的公钥数据（非对称）加密标准
*     ECC(椭圆曲线)
*
* 3.信息摘要算法(Hash) -- 一种能产生特殊输出格式的算法，无论输入数据如何，输出的密文长度固定，常用于数据完整性校验、不可逆向解密的场景
*   常见算法(MD5/SHA-1)
*     DSS/DSS1
*     MD2
*     [MD5] -- 输出16字节
*     MDC2
*     SHA
*     [SHA-1]/256/383/512 -- 输出20字节
*     RIPEMD
* 
* 4.MAC 算法 -- 这是什么？
*     HMAC
* 
* 5.密钥和证书(Certificate)管理
*   ASN.1
*   X.509
*   PKCS#12
* 数字证书：有公钥信息，从而确认了拥有密钥对的用户的身份
*    
* 公钥位于 ~/.ssh/ 目录中，该目录有以下限制：除本账户的其他所有账户(包括root)都只读，否则无法登录。特殊需求时可在 sshd_config 配置
*   文件中用 StrictMode no 来取消该限制(man sshd_config)
*
**********************************************************************************************************************************/

/**********************************************************************************************************************************
* Crypto++(http://www.eskimo.com/~weidai/cryptlib.html) -- 开源的密码类库。实现了各种公开密钥算法、对称加密算法、数字签名算法、
*   信息摘要等算法。eMule用于实现RSA签名，支持独有的积分机制
*   1.生成随机数(16字节的数组)：
*     AutoSeededRandomPool rng;
*     byte byRandomBytes[16] = { 0 };
*     rng.GenerateBlock(byRandomBytes, 16);
**********************************************************************************************************************************/

/**********************************************************************************************************************************
* Crypto API -- 微软的加解密API(wincrypt.h), AtlServer中的 AtlCrypt.h 进行了封装
*   Crypto API 的配置信息存储在注册表中( HKLM\SOFTWARE\Microsoft\Cryptography )
*   支持：
*     会话密钥(对称算法) -- 
*     公钥/私钥对 -- 
*   编程
*     CSP(Cryptographic Service Provider)
*     CryptAcquireContext -- 连接缺省的CSP，如果最后的参数是 CRYPT_NEWKEYSET 则会创建缺省CSP，使用完毕后需要通过 CryptReleaseContext 释放，
*     CryptCreateHash -- 产生一个空的HASH对象
*     CryptDecodeObject -- 解码对象 ?
*     CryptDecrypt/CryptEncrypt -- 
*     CryptDeriveKey(..,CRYPT_EXPORTABLE) -- 从某一固定数据(如密码)产生会话密钥，
*     CryptExportKey -- 导出密钥，其中 hExpKey 表示将待导出密钥用交换密钥进行加密，如导出公钥，则设置为 NULL
*     CryptGetUserKey -- 返回所获取密钥类型的句柄，可以指定 AT_KEYEXCHANGE(交换密钥，可导出会话密钥) 或 AT_SIGNATURE(签名密钥)
*     CryptGenRandom -- 为空间产生随机字节,生成的随即数据可用于 CryptSetKeyParam
*     CryptGenKey -- 产生一个随机的会话密钥或者公钥/私钥对，ALG_ID 表明产生私钥所使用的算法(如 CALG_AES、CALG_RSA_KEYX 等)
*     CryptHashData -- 对密码进行HASH处理
*     CryptImportKey -- 将密钥从BLOB转换到CSP中
*     CryptMsgGetParam -- 从加密信息中获取特定参数的信息(比如签名证书信息 CMSG_SIGNER_INFO_PARAM)
*     CryptMsgUpdate -- 增加内容到加密信息中
*     CryptQueryObject -- 可以从文件、内存中 获取信息(如 证书/证书信任列表等)
*       如：从文件中获取签名的证书信息 -- CryptQueryObject(CERT_QUERY_OBJECT_FILE, L"FilePath", CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
*         CERT_QUERY_FORMAT_FLAG_BINARY, 0, &encodingType, ...) -- 
*     CryptSetKeyParam/CryptGetKeyParam -- 设置/获取 会话密钥的参数
*     证书(Certificate)
*       CertCreateCertificateContext/ -- 创建证书上下文
*       CertFindCertificateInStore  -- 从certificate store(证书存储)中查找证书?
*       CertFreeCertificateContext -- 释放证书上下文
*       CertGetNameString -- 从证书上下文中获取主题或者签发者，并且将他转化为易读的字符串(文档中未写的：dwFlags为0时表获取publisherName)
*       CryptUIDlgCertMgr -- 可以显示系统提供的证书管理对话框(WinXp后才提供?)
*       
*   常见密码相关流程(流程中不包括异常和资源清理)
*     1.通过密码生成会话密钥: CryptAcquireContext=>CryptCreateHash=>CryptHashData
*     2.加解密消息: 
* .NET的加解密类： System.Security.Cryptography
**********************************************************************************************************************************/

/**********************************************************************************************************************************
* TODO:
*   1.似乎支持的媒体类型有限制(视频：.wma,.wmv,.asf 等)？
* DRM数字版权保护流程：
*   1.支持DRM的格式 -> 加密(Key ID + LKSeed) -> 播放时请求许可证(LAURL) -> 数字节目授权中心发放许可证 -> 获取到权限后才能播放
*     加密的数字节目头部存放着KeyID和节目授权中心的URL
**********************************************************************************************************************************/

/**********************************************************************************************************************************
* SSL(Secure Socket Layer)--安全套接层协议，可使用户/服务器之间的通信不被窃听，始终对服务器进行认证，可选择对用户进行认证
*   采用公钥加密技术，是Internet上保密通讯的工业标准。
*   优点：与应用层协议独立无关，高层应用层协议(HTTP，FTP，TELNET等)能透明地建立于SSL协议之上
* TLS(Transport Layer Security)-- 建立于SSL V3.0之上的兼容协议，主要区别在于所支持的加密算法
*
* OpenSSL -- 基于密码学的安全开发包，为网络通信提供安全及数据完整性的一种安全协议，囊括了主要的密码算法、常用的密钥和证书封装管理功能以及SSL协议.
*   电子文档:赵春平 的 OpenSSL编程
*   编译(参见其中的 INSTALL.W32)：
*     1.下载并安装 ActivePerl(http://www.activestate.com/ActivePerl);
*     2.[可选]下载 汇编编译器 NASM(Netwide Assembler) http://www.nasm.us/ 或 http://nasm.sourceforge.net/
*     3.编译时指定 --prefix=安装后的目录， 编译完毕后设置 OPENSSL_HOME 的环境变量，指向该目录，然后可在程序中使用
*
*  实现内容：
*   1~3.各种对称加密、非对称加密、信息摘要等算法
*   4.密钥和证书管理
*   5.SSL和TLS协议 -- OpenSSL实现的SSL协议是纯粹的SSL协议，没有跟其它协议(如HTTP)协议结合在一起，澄清了SSL协议的本来面目
*     实现了SSL协议的SSLv2和SSLv3，支持了其中绝大部分算法协议，也实现了TLSv1.0
*
*   6.应用程序 -- 主要包括密钥生成、证书管理、格式转换、数据加密和签名、SSL测试以及其它辅助配置功能
*     openssl.exe -- bf -in 指定的
*
*   7.Engine机制 -- 使OpenSSL能够透明地使用第三方提供的软件加密库或者硬件加密设备进行加密，提供了一个通用的加密接口，能够与绝大部分加密库或者加密设备协调工作。
*     和微软的CSP功能基本相同
*   
*   8.其他
*     BIO -- OpenSSL提供的一种高层IO接口,封装了几乎所有类型的IO接口，如内存访问、文件访问以及Socket等。这使得代码的重用性大幅度提高
*
* 编程API
*   EVP(evp.h) -- OpenSSL自定义的一组高层算法封装函数，是对具体算法的封装。可以在同一类加密算法框架下，通过相同的接口去调用不同的加密算法，且方便扩展。
*     通过 struct evp_cipher_st 中的回调函数来调用对应的各种算法函数( init -> [ctrl -> ] do_cipher -> cleanup )?
*
*     EVP_Encrypt -- 对称加密算法
*     EVP_Seal/EVP_Open -- 非对称加密算法
*     EVP_Digest  -- 信息摘要算法
*     EVP_Sign/EVP_Verify -- 签名算法
*     EVP_Encode/EVP_Decode -- BASE64(等?)编码算法
*     EVP_PKEY/EVP_BytesToKey -- 不同类型密钥结构的管理/加密密钥和IV值提取功能
* 
*   加解密：
*     1.EVP_CIPHER_CTX_init
*        E.1. EVP_EncryptInit(ctx, EVP_aes_128_cbc(), pKey, pIV);
*        E.2. EVP_EncryptUpdate
*        E.3. EVP_EncryptFinal
*        D.1. EVP_DecryptInit(ctx, EVP_aes_128_cbc(), pKey, pIV);
*        D.2. EVP_DecryptUpdate
*        D.3. EVP_DecryptFinal
*     3.EVP_CIPHER_CTX_cleanup
**********************************************************************************************************************************/


namespace FTL
{
    class CFCrackUtility
    {
    public:
        enum CrackStringType
        {
            //csLowerCase,
            //csUpperCase,
            //csNumber,
            csKiloString,
            //csPlaceString,
        };
        FTLINLINE CFCrackUtility();
        FTLINLINE virtual ~CFCrackUtility();

        FTLINLINE LPCTSTR GetCrackString(CrackStringType csType,DWORD dwPlaceStart = 0);
    private:
        LPTSTR  m_pszCrackString;
    private:
        static LPCTSTR s_csKiloString;
        static LPCTSTR s_csPlaceString;
    };
}

#endif //FTL_CRACK_H

#if 0
void Base64_Demo()
{
	std::string strUtf8 = "xxxxx";
	int iDestLen = ATL::Base64EncodeGetRequiredLength(strUtf8.size());
	char *pBase64 = new char[iDestLen + 1]; pBase64[0] = '\0';
	if (ATL::Base64Encode((LPBYTE)strUtf8.c_str(), strUtf8.size(), pBase64, &iDestLen))
	{
		pBase64[iDestLen] = '\0';
		CString wstrBase64;
		CUtil::Utf8ToUcs2(pBase64, wstrBase64);

		//问题：是否需要这些转换?
		//wstrBase64.Replace(_T("%"), _T("%25"));
		//wstrBase64.Replace(_T("+"), _T("%2B"));
		//wstrBase64.Replace(_T("/"), _T("%2F"));
		//wstrBase64.Replace(_T("?"), _T("%3F"));
		//wstrBase64.Replace(_T("#"), _T("%23"));
		//wstrBase64.Replace(_T("&"), _T("%26"));
		wstrBase64.Replace(_T("="), _T("-"));	

		strURL.format("xxxx=%s,", wstrBase64);
	}
	delete []pBase64;
}
#endif 


#ifndef USE_EXPORT
#  include "ftlCrack.hpp"
#endif
