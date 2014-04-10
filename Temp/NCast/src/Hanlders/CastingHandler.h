#ifndef _CASTING_HANDLER_H_
#define _CASTING_HANDLER_H_

//JNI 接口
//public static native int startMediaRender(byte[] friendname ,byte[] uuid);
//public static native int stopMediaRender();  
//1.将外部的action事件通过反射机制抛给java层处理 -- JNIEnv::CallStaticVoidMethod
//2.通过jni将一些事件状态值更新至所在服务列表 -- 
//  FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1") => SetStateVariable
//
//在jni层转换C++类型与java类型时一定要注意及时释放资源，不要造成内存泄漏了


#include "CastingDefine.h"

//#if defined(__cplusplus)
//extern "C" {
//#endif 

class CASTING_HANDLER_API INMediaObject{
public:

};

class CASTING_HANDLER_API INDevice{
public:
    virtual const char* GetFriendlyName() = 0;
};

class CASTING_HANDLER_API INCastingEventCallback{
public:
    virtual void OnDeviceAdd(INDevice* pDevice) = 0;
    virtual void OnDeviceRemove(INDevice* pDevice) = 0;
};


class CASTING_HANDLER_API INCastingHandler{
public:
    virtual int Init(INCastingEventCallback* pCallback) = 0;
    virtual int Release() = 0;
    virtual int RefreshDevice() = 0;
    virtual int PlayMediaObjectOnDevice(INMediaObject* pMediaObject, INDevice* pDevice) = 0;

public:
    virtual ~INCastingHandler() {}
};



//#if defined(__cplusplus)
//}
//#endif 



#endif //_CASTING_HANDLER_H_