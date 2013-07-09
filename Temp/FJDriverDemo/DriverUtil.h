#ifndef DRIVER_UTIL_H
#define DRIVER_UTIL_H

#if DBG
# define REPORT_ERROR_INFO(s, x)   \
    DbgPrint("ERROR:%d, %s", s, #x);

# define NT_VERIFY(x)   \
    status = (x);\
    if(STATUS_SUCCESS != status)\
    {\
        REPORT_ERROR_INFO(status, x);\
    }

#else

# define NT_VERIFY(x)   \
    nRet = (x);\

#endif 



#endif //DRIVER_UTIL_H