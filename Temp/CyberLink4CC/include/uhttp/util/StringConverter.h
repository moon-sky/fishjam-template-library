#ifndef _CUTIL_STRING_CONVERTER_H_
#define _CUTIL_STRING_CONVERTER_H_

#include <string>
#if !defined(BTRON) && !defined(ITRON) && !defined(TENGINE) 
#include <iostream>
#endif

namespace uHTTP {
////////////////////////////////////////////////////////////////////////////////////////////////
//  String Class
////////////////////////////////////////////////////////////////////////////////////////////////

class  StringConverter {
public:
    static bool toUTF16(const std::string& utf8String, std::wstring& utf16String);	
    static bool toUTF8(const std::wstring& utf16String, std::string& utf8String);

    static bool LocalToUTF16(const std::string& localString, std::wstring& utf16String, int codePage); //charset
    static bool LocalToUTF8(const std::string& localString, std::string& utf8String, int codePage);

    static bool UTF16ToLocal(const std::wstring& utf16String, std::string& localString, int codePage);
    static bool UTF8ToLocal(const std::string& utf8String, std::string& localString, int codePage);
};

}

#endif


