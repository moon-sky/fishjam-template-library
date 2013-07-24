#ifndef F_DRIVER_DEMO_DEFINE_H
#define F_DRIVER_DEMO_DEFINE_H

//通过 CreateFile(FDRIVER_DEMO_DOS_NAME,..., OPEN_EXISTING ) 方式打开驱动
#define FDRIVER_DEMO_DOS_NAME           TEXT("\\\\.\\Global\\FJDriverDemo")

#define FDRIVER_DEMO_FILENAME           TEXT("FJDriverDemo.sys")

#define FDRIVER_DEMO_SERVICE_NAME       TEXT("FJDriverDemo")

//设备名必须是 \\Device\\设备名 的形式， 如 磁盘分区的C盘： \\Device\\HarddiskVolume1
//  如不支持设备名，创建时 I/O 管理器会自动分配一个数字作为设备名
#define FDRIVER_DEMO_DEVICE_NAME        L"\\Device\\FJDriverDemoDevice"

//#define NT4_DRIVERNAME "\\\\.\\NTProcDrv"
//#define OTHER_DRIVERNAME "\\\\.\\Global\\NTProcDrv"
//#define PROCDRVPROCESSEVENT "NTProcDrvProcessEvent"


#endif //F_DRIVER_DEMO_DEFINE_H