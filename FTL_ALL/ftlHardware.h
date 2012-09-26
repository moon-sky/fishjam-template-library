#ifndef FTL_HARDWARE_H
#define FTL_HARDWARE_H

#pragma once

#ifndef FTL_BASE_H
#  error FtlHardware.h requires ftlbase.h to be included first
#endif

namespace FTL
{
//Class
    class CFDeviceNotification;  //注册Device通知消息后，可以通过WM_DEVICECHANGE接收设备消息

//Function

}//namespace FTL


namespace FTL
{
    /*****************************************************************************************************
    * 存储器主要由：寄存器、高速缓存(Cache)、内存、外存 等部分组成。
    * CPU与外设之间交换数据的方式:
    *   1.直接程序控制：具体又分为 立即程序传送方式 和 程序查询方式(性能低，无法实时);
    *   2.中断方式：发生(高优先级)中断时，保存现场、中断处理、恢复现场。无法满足高速的批量数据传送要求。
    *   3.DMA：通过DMA控制器实现主存与I/O设备间的直接数据传送，不需要CPU的干预。对外设的管理和某些控制操作仍由CPU承担；
    *   4.通道控制方式：专用控制器，执行通道程序进行I/O操作的管理，为主机和I/O设备提供数据传输通道。常用于 大、中型机。
    *
    * 寻址方式：
    *   1.立即寻址   -- 操作数就包含在指令中，如 ADD AX,100 中的 100
    *   2.直接寻址   -- 操作数存放在内存单元中，指令中直接给出操作数所在存储单元的地址。如 ADD AX,(100) 中的 (100)
    *   3.寄存器寻址 -- 操作数存放在某一寄存器中，指令中给出寄存器名。如 ADD AX, 100 中的 AX
    *   4.寄存器间接寻址 -- 操作数存放在内存单元中，操作数所在存储单元的地址又在寄存器中。如 ADD AX,(BX) 中的 (BX)
    *   5.间接寻址   -- 指令中给出了操作数的地址的地址，需要进行两次访问内存的操作。如 ADD AX ((100))
    *   6.相对寻址   -- 指令地址码部分给出的是一个偏移量(可正可负)，操作数地址等于本条指令的地址加上该偏移量。
    *       如 ADD AX, *100。存储单元的地址 = 本条指令所在存储单元地址 + 100
    *   7.变址寻址   -- 操作数地址等于变址寄存器的内容加偏移量。如 ADD AX, (DI+100)，(DI+100)所在的存储单元是操作数
    *
    * 90年代起，Intel等公司开始把X86指令集翻译成短小固定的类RISC指令，使得现在的X86CPU开始具有先进的RISC核心、寄存器
    *   重命名、乱序执行等技术，具有了RISC的一些特点。
    *****************************************************************************************************/

    /*****************************************************************************************************
    * 即插即用：插上设备后，设备会自动寻找并安装合适的驱动，如果没有会提示用户安装。
    * 非即插即用：插上设备后，必须手动安装驱动，不会自动安装或提示。
    * 
    * //接收HID设备(Home Information Device)发送的数据
    * 1.注册：
    * UINT nDevices;
    * //取得连接到系统的raw input设备信息
    * ::GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST));
    * PRAWINPUTDEVICELIST pRawInputDeviceList = new RAWINPUTDEVICELIST[nDevices];
    * ::GetRawInputDeviceList(pRawInputDeviceList, &nDevices,sizeof(RAWINPUTDEVICELIST);
    * //依次对设备进行注册
    *  for(UINT i = 0;i < nDevices;i++)
    *  {
    *    RID_DEVICE_INFO stDeviceInfo = {sizeof(RID_DEVICE_INFO)};
    *    UINT nSize = sizeof(RID_DEVICE_INFO);
    *    ::GetRawInputDeviceInfo((pRawInputDeviceList + i)->hDevice,RIDI_DEVICEINFO,&stDeviceInfo,&nSize);
    *    switch(stDeviceInfo.dwType)
    *    {
    *      case RIM_TYPEHID:
    *        RAWINPUTDEVICE    stRawInputDevice;
    *        stRawInputDevice.usUsagePage = stDeviceInfo.hid.usUsagePage;
    *        stRawInputDevice.usUsage = stDeviceInfo.hid.usUsage;
    *        stRawInputDevice.dwFlags = RIDEV_INPUTSINK;
    *        stRawInputDevice.hwndTarget = m_hWnd;
    *        ::RegisterRawInputDevices(&stRawInputDevice,1,sizeof(RAWINPUTDEVICE));
    *         break;
    *      default:
    *        break;
    *    }
    *  }
    *  //释放RAWINPUTDEVICELIST内存
    *  delete []pRawInputDeviceList;
    * 2.处理 WM_INPUT 消息 -- ON_MESSAGE(WM_INPUT, OnWmInput)
    *   LRESULT CVcDlg::OnWmInput(WPARAM wParam, LPARAM lParam)
    *   {
    *     UINT uSize;
    *     //取得数据内容
    *     GetRawInputData((HRAWINPUT)lParam,RID_INPUT,NULL,&uSize,sizeof(RAWINPUTHEADER));
    *     char *szData = new char[uSize];
    *     GetRawInputData((HRAWINPUT)lParam,RID_INPUT,szData,&uSize,sizeof(RAWINPUTHEADER));
    *     PRAWINPUT pRawInput = (PRAWINPUT)szData;
    *     //对数据类型进行判断
    *     if(pRawInput->header.dwType == RIM_TYPEHID){ ... }
    *     //释放数据内存
    *     delete []szData;
    *     //交给系统进行默认处理
    *     DefWindowProc(WM_INPUT,wParam,lParam);
    *     return 0L;
    *   }
    *****************************************************************************************************/
    class CFDeviceNotification
    {
    public:
        CFDeviceNotification();
        ~CFDeviceNotification();
        BOOL Register(HANDLE hRecipient,LPVOID NotificationFilter,DWORD Flags);
        BOOL UnRegister();
    private:
        HDEVNOTIFY m_hDevNotify;
    };
}

#endif //FTL_HARDWARE_H

#ifndef USE_EXPORT
#  include "ftlHardware.hpp"
#endif