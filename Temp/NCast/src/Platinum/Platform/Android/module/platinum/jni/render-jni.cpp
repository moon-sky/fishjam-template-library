#include <assert.h>
#include <jni.h>
#include <string.h>
#include <sys/types.h>

#include "platinum-jni.h"
#include "Platinum.h"

#include <android/log.h>


void ActionInflect(int cmd, const char* value, const char* data)  
{  
    if (g_vm == NULL)  
    {  
        UpnpPrintInfo("g_vm = NULL!!!");  
        return ;  
    }  
  
  
    int status;  
    JNIEnv *env = NULL;  
    bool isAttach = false;  
    status = g_vm->GetEnv((void **) &env, JNI_VERSION_1_6);  
    if(status != JNI_OK)   
    {  
        status = g_vm->AttachCurrentThread(&env, NULL);  
        if(status < 0) {  
            UpnpPrintInfo("callback_handler: failed to attach , current thread, status = %d", status);  
            return;  
        }  
        isAttach = true;  
    }  
  
    jstring valueString = NULL;  
    jstring dataString = NULL;  
    jclass inflectClass = g_inflectClass;  
    jmethodID inflectMethod = g_methodID;  
  
    if (inflectClass == NULL || inflectMethod == NULL)  
    {  
        goto end;  
    }  
  
  
//  UpnpPrintInfo("CMD = %d\nVALUE = %s\nDATA = %s",cmd, value, data);  
      
    valueString = env->NewStringUTF(value);  
    dataString = env->NewStringUTF(data);  
  
    env->CallStaticVoidMethod(inflectClass, inflectMethod, cmd, valueString, dataString);  
  
    env->DeleteLocalRef(valueString);  
    env->DeleteLocalRef(dataString);  
  
end:  
    if (env->ExceptionOccurred())  
    {  
        env->ExceptionDescribe();  
        env->ExceptionClear();  
    }  
    if (isAttach)  
    {  
        g_vm->DetachCurrentThread();  
    }  
}  