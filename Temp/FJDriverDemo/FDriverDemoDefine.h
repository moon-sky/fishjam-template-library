#ifndef F_DRIVER_DEMO_DEFINE_H
#define F_DRIVER_DEMO_DEFINE_H

//#ifdef __cplusplus
//extern "C" 
//{
//#endif
//
//#include <ntddk.h>
//
//#ifdef __cplusplus
//}
//#endif


#define FDRIVER_DEMO_FILENAME           TEXT("FJDriverDemo.sys")

#define FDRIVER_DEMO_SERVICE_NAME       TEXT("FJDriverDemo")

//设备名必须是 \\Device\\设备名 的形式， 如 磁盘分区的C盘： \\Device\\HarddiskVolume1
//  如不支持设备名，创建时 I/O 管理器会自动分配一个数字作为设备名
#define FDRIVER_DEMO_NT_DEVICE_NAME        L"\\Device\\FJDriverDemoDevice"

//客户端使用的驱动符号链接名，通过 CreateFile(FDRIVER_DEMO_DOS_DEVICE_NAME,..., OPEN_EXISTING ) 方式打开驱动
#define FDRIVER_DEMO_DOS_DEVICE_NAME       L"\\DosDevices\\FJDriverDemo"			//L"\\\\.\\Global\\FJDriverDemo"
#define FDRIVER_DEMO_WIN2K_DEVICE_NAME		L"\\\\.\\Global\\FJDriverDemo"

#  define METHOD_BUFFERED 0
#  define FILE_WRITE_ACCESS         ( 0x0002 )    // file & pipe


//在  IRP_MJ_DEVICE_CONTROL 的分发函数中 switch(irpStack->Parameters.DeviceIoControl.IoControlCode)

//通信方面，有 METHOD_NEITHER 方法(不好，有问题)，更好的应该是 BUFFERED 模式

//Notice: SDK 的 <winioctl.h> 有 CTL_CODE 的定义

//自定义的 DEVICE_TYPE ? 系统已经预定很多类型( FILE_DEVICE_BEEP 等), 给用户保留可以使用的范围是 0x8000(32768)~0xFFFF(65535)
#define FDRIVER_DEV_DRV 0xA812

//定义可用于 IOCTL 或 FSCTL 的编码，给用户保留可以使用的function codes范围是 0x800(2048)-0xFFF(4095)
//CTL_CODE( 设备类型, 功能, I/O访问内存使用方式, 访问限制 )

#define IOCTL_FDRIVER_INSTALL_HOOK		(ULONG) CTL_CODE(FDRIVER_DEV_DRV, 0x800, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_FDRIVER_UNINSTALL_HOOK	(ULONG) CTL_CODE(FDRIVER_DEV_DRV, 0x801, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_FDRIVER_FILTER_DESKTOP	(ULONG) CTL_CODE(FDRIVER_DEV_DRV, 0x802, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_FDRIVER_UNFILTER_DESKTOP	(ULONG) CTL_CODE(FDRIVER_DEV_DRV, 0x803, METHOD_BUFFERED, FILE_WRITE_ACCESS)

typedef struct _PROTECT_WND_INFO
{
	HWND hWndDeskTop;
	//HWND hTargetWindow;
	//HANDLE hSelfProcess;
	HANDLE hTargetProcess;
	//HDC	   hDCProtect;
	//RECT   rcProtectWindow;
} PROTECT_WND_INFO, *PPROTECT_WND_INFO;

//#define NT4_DRIVERNAME "\\\\.\\NTProcDrv"
//#define OTHER_DRIVERNAME "\\\\.\\Global\\NTProcDrv"
//#define PROCDRVPROCESSEVENT "NTProcDrvProcessEvent"


#endif //F_DRIVER_DEMO_DEFINE_H