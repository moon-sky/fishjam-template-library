#ifndef EventParamDefine_H_
#define EventParamDefine_H_

//typedef int (*F_EventNotify)(int nCode, void *pParam);

// EVENT CODE

//#define  FRAME_TIME (EC_USER + 100)  // this msg's param is ST_FRAME_TIME
//struct ST_FRAME_TIME 
//{
//    LONGLONG llStartTime;
//    LONGLONG llStopTime;
//    
//    int nFrameIndex;
//};


//lParam1 is HBITMAP
//lParam2 is NULL
#define  FIRST_FRAME (EC_USER + 101)

//lParam1 is BSTR, indicate video FullPath, 
//Lparam2 -- TODO:(struct?)
#define  INIT_VIDEO_INFO  (EC_USER + 102)

//lParam1 is void
//lParam2 is void
#define TIME_OVER	(EC_USER + 103)

//ERROR_NOTIFY

//lParam1 is HRESULT
//  0x80070070() -- Disk Full
//lParam2 is void
#define ERROR_ABORT	(EC_USER + 104)


#endif 