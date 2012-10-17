#ifndef DIRECT_SHOW_WINDOWS_SHOW_H
#define DIRECT_SHOW_WINDOWS_SHOW_H
#pragma once

/**********************************************************************************************
* Windows Media Files--WMF(ASF,WMV,WMA)，由 WMVCore.dll 提供Playback和Authoring的功能，该Dll在
*   Windows Media Player 9 或 Windows Media Format Runtime 中提供。
*  
* 
* 通常需要使用 Windows Media Format SDK(WMFS) 开发，DirectShow中提供了部分Filter进行支持
*   1.Windows Media Player 6.4 中，有名为 Windows Media Source 的Filter支持ASF文件的播放
*   2.DirectShow 8.0 中增加了两个Filter：WM ASF Reader 和 WM ASF Write，但其内部使用 WMFS
*
* 许可认证 -- 播放经过 DRM 保护的文件,通过带IServiceProvider接口的COM组件实现(创建一个密钥提供器来解锁)。 
*   Windows Media 文件如果使用了DRM保护，播放器就必须提供DRM存根库(WMStub.lib)，
*   WMF9以前需要使用 Windows Media certificate handler来 unkey(IServiceProvider子类), 
*   需要使用 Windows Media Format SDK (WMStub.LIB -- 注：DirectShow 中提供的版本不支持DRM),
*   参见 DS Sample AudioBox 和 PlayWndASF
* 
*   CertHandler :
*     #include <dshowasf.h>，在 IServiceProvider::QueryService 的实现中 
*     if( siid == __uuidof(IWMReader) && riid == IID_IUnknown ) 时，
*     调用 WMCreateCertificate 创建certificate并返回。
*   播放时使用 WM ASF Reader filter（CLSID_WMAsfReader),获得GraphBuilder的IObjectWithSite，
*     通过 SetSite((IUnknown *) (IServiceProvider *) &prov); 设置需要使用的 Cert Handler
*   加载媒体文件时可能遇到NS_E_LICENSE_REQUIRED(0xC00D0BBEL,等待认证)/NS_E_PROTECTED_CONTENT(0xC00D0BBDL,认证失败)等错误， 
*     表明媒体文件被DRM保护，并且需要License才能播放，(must acquire a DRM stub library from Microsoft
*     and link it with this application. the default version of WMStub.lib does not support DRM)
*     IMediaEventEx::GetEvent 为 EC_WMT_EVENT 时需要进行 DRM 认证处理
* 
* 配置(Profile，SDK中有ProfileEnum.exe工具) -- 生成ASF文件时用于设定 WM ASF Writer(CLSID_WMAsfWriter) 
*   有几个输入Pin、生成文件的比特率、压缩质量等。
*   IConfigAsfWriter -- 注意：必须先加入FilterGraph，然后配置，最后再进行连接
*   IWMProfileManager(2) -- 枚举ASF配置
*
* 注意：
*   1.ASF是容器
*   2.ASF文件必须以音频为基准来播放，所以ASFWriter总是需要一个音频的输入
**********************************************************************************************/

#endif //DIRECT_SHOW_WINDOWS_SHOW_H