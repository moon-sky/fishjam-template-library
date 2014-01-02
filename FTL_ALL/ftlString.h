#ifndef FTL_STRING_H
#define FTL_STRING_H
#pragma once

#include "ftlDefine.h"

namespace FTL
{
    class CFStringUtil
    {
    public:
        FTLINLINE static BOOL IsMatchMask(LPCTSTR Name, LPCTSTR Mask, BOOL bCaseSensitive = TRUE);
    };
}
#endif //FTL_STRING_H

#ifndef USE_EXPORT
#  include "ftlString.hpp"
#endif