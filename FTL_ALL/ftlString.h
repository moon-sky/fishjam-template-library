#ifndef FTL_STRING_H
#define FTL_STRING_H
#pragma once

#include "ftlDefine.h"

namespace FTL
{
    //忽略大小写字符串比较的函数
    FTLINLINE int ciStringCompare(const std::string& s1, const std::string& s2);

    //用于忽略大小写字符串比较的仿函数类 -- 参见 Effective STL 中的条款19
    struct CIStringCompare : public std::binary_function<std::string, std::string, bool>
    {
        FTLINLINE bool operator()(const std::string& lhs, const std::string& rhs) const
        {
            return ciStringCompare(lhs, rhs);
        }

    };

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