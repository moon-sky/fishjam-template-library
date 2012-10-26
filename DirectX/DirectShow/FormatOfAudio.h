#pragma once

//编码技术中有 PCM(Pluse Coding Modulation) -- 脉冲编码调制，和LPCM的关系？
//MP3一般是1152个采样为一帧来编码的，知道采样率，声道，很容易计算出1秒多少帧
//  帧/秒 = SampleRate/1152/chanel ? 如 44100/1152/2 ?

/*********************************************************************************************
* AAC(Advanced Audio Coding) -- 高级音频编码，AC3的升级，有2.0和5.1音频的区别。
* AC3(Audio Coding3) -- 音频编码3,是 DOLBY_AC3 的同义词，最多可以对6个比特率最高为448kbps的单独声道进行编码
* APE -- 一种无损压缩格式，压缩比远低于其他格式(50-70%的压缩比)，但能够做到真正无损，最高品质的音乐欣赏及收藏.
* DTS
* LPCM -- 线性脉冲编码调制，是一种非压缩音频数字化技术，无损编码。
*         其码率为：采样率值x采样大小值x声道数 bps， 如 44100 x 16 x 2(双声道) ==1411.2 Kbps,等价于128Kbps的MP3.
* ??? -- MPEG-1 Audio Layer II(HDV中保存音频时的格式)
* MP3(MPEG-1 Audio Layer-3, 是MPEG1的衍生编码方案) -- 进行有损压缩，音质基本不变，但如果多次编辑后，音质会急剧下降，
*   最多只能编码2个声道，具有流媒体的基本特征，可以做到在线播放。
*   DS中有 MPEG-1 Layer III，似乎功能有限制(最高只能输出22050Hz)，且在Vista/Win7下有license问题
*      http://support.microsoft.com/kb/937141/en-us
*   需要(这种方式是否算破解，设置后有其他问题, VFW_E_START_TIME_AFTER_END(0x80040228))在
*     "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Drivers32" 下增加 
*       "msacm.l3codec"="l3codecp.acm" 和 "msacm.l3acm"="l3codecp.acm"(注意：已有?)
*     "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\drivers.desc] 下增加 
*       "l3codecp.acm"="Fraunhofer IIS MPEG Layer-3 Codec (professional)"
*     注意：（默认只有 "l3codeca.acm"="Fraunhofer IIS MPEG Layer-3 Codec"）
*   更好的方式是使用 "LAME MP3 Encoder" (http://lame.sourceforge.net/),其中包含了可直接使用的 DShow Filter.
* mp3PRO -- 基于mp3编码技术的改良方案，其最大的技术亮点就在于SBR。mp3PRO其实是一种mp3信号流和SBR信号流的混合数据流编码。
*   官方声称64kbps的mp3PRO可以媲美128kbps的mp3。
* MPC -- 音质优势主要表现在高频部分,是目前最适合用于音乐欣赏的有损编码。
* MPEG1/MPEG2 -- MEDIASUBTYPE_MPEG1Audio/MEDIASUBTYPE_MPEG2Audio
* Ogg Vorbis -- 完全免费，号称可以在相对较低的数据速率下实现比MP3更好的音质（MP3杀手），可以支持多声道，
*   具有流媒体的基本特征.
* RA(RealAudio) -- 针对网络(支持边读边放),而且可以根据听众的带宽来控制自己的码率，在保证流畅的前提下尽可能提高音质。
*   可以支持多种音频编码，包括ATRAC3。支持使用特殊协议来隐匿文件的真实网络地址(只在线试听而不提供下载)
* WAV -- WAV是一种文件格式，复合RIFF规范，Windows平台下，所有音频软件都能够提供其的支持
* WMA -- Windows Media Audio编码后的文件格式，主要针对网络，支持流技术(边读边放)，微软开发，有很好的软硬件支持。
*   支持防复制功能(可通过Windows Media Rights Manager加入保护，可以限制播放时间和播放次数甚至于播放的机器等)
*   支持在线试听
*
*********************************************************************************************/

/*********************************************************************************************
* Audio 的媒体类型（AM_MEDIA_TYPE::pbFormat）
* formattype为FORMAT_WaveFormatEx 时采用 WAVEFORMATEX 格式
*   PCM文件每秒的数据量大小: nSamplesPerSec * (wBitsPerSample / 8) * nChannels, 如 44100 * (16/8) * 2
*     注意： wBitsPerSample 可能为 0
*   typedef struct tWAVEFORMATEX
*   {
*     WORD        wFormatTag;         // format type，如 WAVE_FORMAT_PCM, WAVE_FORMAT_EXTENSIBLE
*     WORD        nChannels;          // 声道：number of channels (i.e. monaural-1, stereo-2...)
*     DWORD       nSamplesPerSec;     // 每秒的采样率，通常为 44100,22050,11025 等，
*       由于范围太大，通常需要使用 log10(freq)/pow(10.0,pos) 方法进行对数转换（方便显示和调整）
*     DWORD       nAvgBytesPerSec;    // 每秒的平均字节数，nSamplesPerSec * nBlockAlign
*     WORD        nBlockAlign;        // (nChannels * wBitsPerSample) / 8，表示每一个采样的字节大小，常见值有 1,2,4
*     WORD        wBitsPerSample;     // 每个采样的位数，如8或16等, 但测试出来有 0 ?
*     WORD        cbSize;             // the count in bytes of the size of extra information (after cbSize)
*   }WAVEFORMATEX, *PWAVEFORMATEX, NEAR *NPWAVEFORMATEX, FAR *LPWAVEFORMATEX;
* 
* 媒体数据(IMediaSample::GetPointer)
*   
*********************************************************************************************/

