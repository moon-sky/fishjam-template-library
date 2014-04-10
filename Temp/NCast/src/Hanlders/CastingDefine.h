#ifndef _CASTING_DEFINE_H_
#define _CASTING_DEFINE_H_

#ifdef CASTING_HANDLER_EXPORTS
#define CASTING_HANDLER_API __declspec(dllexport)
#else
#define CASTING_HANDLER_API __declspec(dllimport)
#endif

//#include "../Neptune/Core/NptTypes.h"


typedef enum CastingDeviceType{
    cdtDNLA         = 0x0001,
    cdtChromeCast   = 0x0002,
    cdtAirplay      = 0x0004,
    cdtMiracast     = 0x0008,

    cdtAll          = 0xFFFF,
};


#endif //_CASTING_DEFINE_H