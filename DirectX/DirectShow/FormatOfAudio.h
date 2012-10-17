#pragma once

//编码技术中有 PCM(Pluse Coding Modulation) -- 脉冲编码调制，和LPCM的关系？
//MP3一般是1152个采样为一帧来编码的，知道采样率，声道，很容易计算出1秒多少帧
//  帧/秒 = SampleRate/1152/chanel ? 如 44100/1152/2 ?

/*********************************************************************************************
* AAC(Advanced Audio Coding) -- 高级音频编码，AC3的升级，有2.0和5.1音频的区别。
* AC3 -- DOLBY_AC3
* DTS
* LPCM
* ??? -- MPEG-1 Audio Layer II(HDV中保存音频时的格式)
* MP3 -- 类似于波表文件的文件格式(进行大量的压缩，但音质基本不变)
*   MPEG-1 Layer III，似乎功能有限制(最高只能输出22050Hz)，且在Vista/Win7下有license问题
*      http://support.microsoft.com/kb/937141/en-us
*   需要(这种方式是否算破解，设置后有其他问题, VFW_E_START_TIME_AFTER_END(0x80040228))在
*     "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Drivers32" 下增加 
*       "msacm.l3codec"="l3codecp.acm" 和 "msacm.l3acm"="l3codecp.acm"(注意：已有?)
*     "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\drivers.desc] 下增加 
*       "l3codecp.acm"="Fraunhofer IIS MPEG Layer-3 Codec (professional)"
*     注意：（默认只有 "l3codeca.acm"="Fraunhofer IIS MPEG Layer-3 Codec"）
*   更好的方式是使用 "LAME MP3 Encoder" (http://lame.sourceforge.net/),其中包含了可直接使用的 DShow Filter.
* MPEG1/MPEG2 -- MEDIASUBTYPE_MPEG1Audio/MEDIASUBTYPE_MPEG2Audio
* Ogg -- 
* Vorbis
* WMA -- Windows Media Audio
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

