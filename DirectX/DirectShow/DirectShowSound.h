#ifndef DIRECTSHOW_SOUND_H
#define DIRECTSHOW_SOUND_H
/************************************************************************************************
* WinXPI 音频采集 --  Main Sound Capture Drive
* Vista/Win7 音频采集 -- WASAPI
*
* APO(Audio Processing Objects) -- WASAPI中的音频处理对象，主要用来处理音量增益、格式转换及混音等功能。
* ASIO(Audio Stream Input Output) -- Windows XP 中与WASAPI类似的通道，由Steinberg公司开发的个人电脑声卡专业驱动，
*   可以减少系统对音频流信号的延迟(MME驱动时延迟 750 毫秒 => ASIO驱动时延迟 40 毫秒内)，增强声卡硬件的处理能力。
*   和微软的DirectSound功能类似，但更高效，但ASIO需要硬件芯片和驱动的支持。
*   编码：http://blog.csdn.net/changpei/article/details/2537080
* 
* SRC(Sample Rate Conversion) -- 取样率转换器
* UAA（Universal Audio Architecture Bus Driver for High Definition Audio) -- 是通用音频架构高清晰度音频类驱动程序。
*   其驱动补丁为(KB888111补丁，2003不发音的原因)， WinXP SP3的 "设备管理器->系统设备" 中有
*   "Microsoft 用于 High Definition Audio 的 UAA 总线驱动程序"
*   Vista时代微软要求所有的音频设备厂商必须遵循UAA的架构来开发驱动程序,可以在用户模式(User Mode)下实现声卡功能。
*   频流程处理的改变，可以使得每个应用程式有自己的音量控制。
*
* WASAPI(Windows Audio Session API) -- Windows音频会话API。从Vista后引入UAA的API。允许传输未经修改的
*   比特流到音频设备，从而避开SRC的干扰，可以无损输出各种采样率和精度(如CD的 44.1kHz 16bit )的音频信号。
*   采用以“Session”为概念的思路，当不同的应用程序调用WASAPI，它们的需求会被各自独立成不同的Session进行音讯处理 
*   两种模式访问内核模式(Kernel Mode)的功能
*     1.Shared Mode(共享模式，默认) -- 不同的应用程序对WASAPI的调用请求被各自独立成不同的Session进行音讯处理,
*       所有的声音讯号都会转送至Audio Engine（音效引擎）部分，使得或多或少被改变了原始内容。
*       例子 -- CaptureSharedTimerDriven
*     2.Exclusive Mode(独占模式) -- 扬声器属性等中启用“允许应用程序独占控制该设备”设置，
*       系统会切断共享模式这一条路径，声音讯号就会直接送达Kernel Mode最后到达底层的音频设备后输出，
*       程序启用独占模式后，可以让声卡直接处理最原始的数字讯号(避免中间处理失真)，
*       缺点：
*         1.无法听到其他程序的声音;
*         2.无法从系统音量那边调整音量大小
*   

************************************************************************************************/


#endif //DIRECTSHOW_SOUND_H