#pragma once

#include <ftlBase.h>
#include <ftlThread.h>
#include <ftlGdi.h>
#include <ftlDebug.h>
#include <ftlAlgorithm.h>

#define GIF_VERIFY(x, err) \
    nRet = (x);\
    if(nRet == GIF_ERROR){\
        FTLTRACE(TEXT("%s(%d) :\t Gif Error at %s, Reason:%d(%s)\n"), TEXT(__FILE__), __LINE__, TEXT(#x), (err), GifErrorString(err)); \
    }

