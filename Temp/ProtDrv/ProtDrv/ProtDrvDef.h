#pragma once

#if defined(_M_IX86)
#  define PROTECT_DRIVER_FILE_NAME           TEXT("ProtDrv32.sys")
#elif defined(_M_AMD64)
#  define PROTECT_DRIVER_FILE_NAME           TEXT("ProtDrv64.sys")
#elif defined(_M_IA64)
#  error NOT SUPPORT IA64 now
#endif 

#define PROTECT_SERVICE_NAME        TEXT("ProtectService")
#define PROTECT_DEVICE_NAME         L"\\Device\\ProtDrvDevice"

//客户端使用的驱动符号链接名，通过 CreateFile(FDRIVER_DEMO_DOS_DEVICE_NAME,..., OPEN_EXISTING ) 方式打开驱动
//#define PROTECT_NT_DEVICE_NAME		L"\\\\.\\Global\\ProtDev"
#define PROTECT_DOS_DEVICE_NAME			L"\\DosDevices\\ProtDev"
#define PROTECT_NT_CONTROL_FILE_NAME	L"\\\\.\\Global\\ProtDev"



//#  define METHOD_BUFFERED 0
//#  define FILE_WRITE_ACCESS         ( 0x0002 )    // file & pipe


//自定义的 DEVICE_TYPE , 给用户保留可以使用的范围是 0x8000(32768)~0xFFFF(65535)
#define PROTDRV_DEVICE_TYPE 0xA934

//定义可用于 IOCTL 或 FSCTL 的编码，给用户保留可以使用的function codes范围是 0x800(2048)-0xFFF(4095)
//CTL_CODE( 设备类型, 功能, I/O访问内存使用方式, 访问限制 )

#define IOCTL_PROTDRV_INSTALL_HOOK		(ULONG) CTL_CODE(PROTDRV_DEVICE_TYPE, 0x800, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_PROTDRV_UNINSTALL_HOOK	(ULONG) CTL_CODE(PROTDRV_DEVICE_TYPE, 0x801, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_PROTDRV_SET_INFO      	(ULONG) CTL_CODE(PROTDRV_DEVICE_TYPE, 0x802, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_PROTDRV_CLEAR_INFO      	(ULONG) CTL_CODE(PROTDRV_DEVICE_TYPE, 0x803, METHOD_BUFFERED, FILE_WRITE_ACCESS)


typedef struct _PROTECT_WND_INFO
{
    HANDLE  hProcess;
    HWND    hWndDesktop;
    HWND    hTargetWindow;
    HDC     hFillDC;
    HBITMAP hFillBitmap;
    RECT    rcWindow;
} PROTECT_WND_INFO, *PPROTECT_WND_INFO;

