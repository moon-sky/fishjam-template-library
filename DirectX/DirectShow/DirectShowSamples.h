#ifndef DIRECTSHOW_SAMPLES_H
#define DIRECTSHOW_SAMPLES_H

#error(测试并放入FTL) -- SysEnum --
void CBaseDialog::RedrawControl(int nID)
{
    // Find the dialog rect and the control rect, both relative to the display
    RECT rcDlg, rcControl;
    GetWindowRect(m_hDlg, &rcDlg);
    GetWindowRect(GetDlgItem(nID), &rcControl);

    // Adjust the dialog rect by the size of the border and caption 
    rcDlg.top += NonClientTop();
    rcDlg.left += NonClientWidth();

    // Find the control rect relative to the dialog position
    OffsetRect(&rcControl, - rcDlg.left, - rcDlg.top);

    InvalidateRect(m_hDlg, &rcControl, TRUE);
    UpdateWindow(m_hDlg);
}

/************************************************************************************************
* 介绍DirectShow中的Sample
* Capture
*  +-AMCap      -- 音视频采集的例子(主要使用ICaptureGraphBuilder2)，支持视频预览，支持采集AVI文件，支持MPEG2流输出，支持热插拔。
*                  通过可重用的 CCrossbar 类来管理Crossbar -- 递归函数BuildRoutingList
*  +-AudioCap   -- 未看 -- 音频采集的例子，枚举音频采集设备，支持各种参数设置，能生成Wav文件，能边听边录。
*  +-CaptureSharedTimerDriven -- 使用Vista/Win7的WASAPI，在共享模式下使用Timer Driven编程模式的方式捕捉音频并存为文件。
* 
* Editing
*  +-CompressView -- 未看 -- 演示如何将媒体文件转换为AVI格式(通过系统枚举压缩器来完成)，哪个有效？
* 
* DMO
*  +-dmodemo    -- 使用各种 DirectSound Audio Effect DMOs 效果，通过 DirectSound 播放Wav文件
* 
* Filters
*  +-Ball       -- 使用未压缩格式的视频，显示运动小球的 Live Source Filter，每一个Sample都是一张BMP图片
*  +-Dump       -- 把接收到的Sample输出到Txt文件的 Render Filter(支持IFileSinkFilter接口，GE会自动创建SaveFile对话框)
*  +-EZRGB24    -- 对image进行过滤和颜色转换效果的Transform Filter(支持属性页调节颜色)，只支持RGB24
*  +-Gargle     -- 修改通过的声音频率的 In-Place Transform Filter(可以用属性页调节频率)
*  +-Grabber    -- 
*  +-InfTee     -- 未看 -- 从CBaseFilter继承的，将单输入分割为很多输出。
*  +-PushSource -- 三种 Video Source Filter(分别是图像、图像集 和 屏幕截图), 没有实现 暂停、帧率 控制的功能
*  +-Synth		-- 音频SourceFilter(Audio Synthesizer)
*  +-WavDest    -- 采集结束时，将PCM数据全部写入文件后，再在文件头位置插入一个Wave格式的数据块。
* 
* Misc
*  +- ASFCopy   -- 未看 -- 使用指定的系统配置讲文件转化为ASF格式的文件，包括认证服务、枚举ASF配置等。
*                  其中定义了ASF的错误码(ASFErr.h 和 nserror.h)。
*                  控制台方式，如：ASFCopy.exe /v /p 22 C:\WINDOWS\Media\tada.wav c:\testasf.asf
*  +- SysEnum   -- 枚举系统中注册的各种Filter，类似GraphEdt显示各种Filter时枚举的功能
*                  其中使用 CLSID_ActiveMovieCategories 得到了DShow相关的所有类型(如压缩、解压、Filter等)
*                  展示了使用自定义的 _SetWindowLongPtr 模板函数来解决 SetWindowLongPtr 函数编译错误的方法 
*                  注意：其RedrawControl方法演示了计算Control相对Dialog位置的方法
* Players
*  +-AudioBox   -- 使用MFC的音频播放程序，有播放列表，实现 IServiceProvider 接口 对WMF文件进行支持
*  +-BGMusic    -- 使用Win32播放MP3的音乐播放程序，其 SwapSourceFilter 方法具有动态改变 Source Filter 的能力
*                  (停止Graph -> 移除旧的SourceFilter -> 加入新的SourceFilter -> 对其OutputPin 进行Render)
*  +-Cutscene   -- 命令行模式的 Movie Player，通过命令行传入媒体文件，进行全屏播放(创建了一个0大小的"隐藏"窗体来处理键盘输入)
*  +-PlayWnd    -- 使用Win32的音频/视频媒体文件播放程序，只能播放一个文件
*  +-PlayWndASF -- 未看 -- 媒体播放器,可以播放受DRM保护的WMF类型的文件 -- 参考DRM的处理方式
*************************************************************************************************/

#endif //DIRECTSHOW_SAMPLES_H