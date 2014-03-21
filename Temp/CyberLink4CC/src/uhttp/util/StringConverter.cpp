#include <uhttp/util/StringConverter.h>

#include <string.h>

using namespace std;
using namespace uHTTP;
#if defined(WIN32) && !defined(ITRON)
#include <windows.h>

//#define CP_UTF8 65001

bool StringConverter::toUTF16(const std::string& utf8String, std::wstring& utf16String)
{
    int wcharsize = MultiByteToWideChar( CP_UTF8, 0,  utf8String.c_str(), -1, NULL, 0 );
    if (wcharsize > 0)
    {
        utf16String.resize(wcharsize - 1);
        MultiByteToWideChar( CP_UTF8, 0,  utf8String.c_str(), -1, &utf16String[0], wcharsize );
        return true;
    }
    return false;
}

bool StringConverter::toUTF8(const std::wstring& utf16String, std::string& utf8String)
{
    int charsize = WideCharToMultiByte( CP_UTF8, 0, utf16String.c_str(), -1, NULL, 0, NULL, NULL );
    if (charsize > 0)
    {
        utf8String.resize(charsize - 1);
        WideCharToMultiByte( CP_UTF8, 0, utf16String.c_str(), -1, &utf8String[0], charsize, NULL, NULL );
        return true;
    }
    return false;
}

bool StringConverter::LocalToUTF16(const std::string& localString, std::wstring& utf16String, int codePage)
{
    int wcharsize = MultiByteToWideChar( codePage, 0,  localString.c_str(), -1, NULL, 0 );
    if (wcharsize > 0)
    {
        utf16String.resize(wcharsize - 1);
        MultiByteToWideChar( codePage, 0,  localString.c_str(), -1, &utf16String[0], wcharsize );
        return true;
    }
    return false;
}

bool StringConverter::LocalToUTF8(const std::string& localString, std::string& utf8String, int codePage)
{
    std::wstring utf16String;
    if(LocalToUTF16(localString, utf16String, codePage))
    {
        return toUTF8(utf16String, utf8String);
    }
    return false;    
}

bool StringConverter::UTF16ToLocal(const std::wstring& utf16String, std::string& localString, int codePage)
{
    int charsize = WideCharToMultiByte( codePage, 0, utf16String.c_str(), -1, NULL, 0, NULL, NULL );
    if (charsize > 0)
    {
        localString.resize(charsize - 1);
        WideCharToMultiByte( codePage, 0, utf16String.c_str(), -1, &localString[0], charsize, NULL, NULL );
        return true;
    }
    return false;
}

bool StringConverter::UTF8ToLocal(const std::string& utf8String, std::string& localString, int codePage)
{
    std::wstring utf16String;
    if(toUTF16(utf8String, utf16String))
    {
        return UTF16ToLocal(utf16String, localString, codePage);
    }
    return false;
}


#else

//other system

#endif

