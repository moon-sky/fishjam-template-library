#ifndef F_DRIVER_DEMO_DEFINE_H
#define F_DRIVER_DEMO_DEFINE_H

//符号链接是  \\??\\FJDriverDemo  这种？

#define FDRIVER_DEMO_NAME               TEXT("\\\\.\\Global\\FJDriverDemo")
#define FDRIVER_DEMO_FILENAME           TEXT("FJDriverDemo.sys")

#define FDRIVER_DEMO_SERVICE_NAME       TEXT("FJDriverDemo")

#define FDRIVER_DEMO_DEVICE_NAME        L"\\Device\\FJDriverDemoDevice"

//#define NT4_DRIVERNAME "\\\\.\\NTProcDrv"
//#define OTHER_DRIVERNAME "\\\\.\\Global\\NTProcDrv"
//#define PROCDRVPROCESSEVENT "NTProcDrvProcessEvent"


#endif //F_DRIVER_DEMO_DEFINE_H