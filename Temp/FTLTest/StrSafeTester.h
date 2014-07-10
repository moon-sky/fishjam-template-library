#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <strsafe.h>

/***************************************************************************************************
* Strsafe.h：更安全的C语言字符串处理函数(C++可以使用 CString、CComBSTR、stl::string 等类)
*  标准的C语言运行时函数要么在返回值和参数上缺乏一致性，要么隐含着所谓的“截断误差”(truncation errors) 
*  错误，容易产生“内存溢出”等问题。
* 注意：
*  1.当引用 strsafe 系列函数时，原有的 C 语言字符串处理函数都将被自动进行 #undef 处理。
*  2.相关的宏(需要在 #include <strsafe.h> 之前)
*    #define STRSAFE_LIB -- 按 library form 方式使用(会自动使用 #pragma comment(lib, "strsafe.lib") )
*    #define STRSAFE_NO_DEPRECATE -- 防止将旧的函数被定义为 deprecated 的(避免编译警告)
*    #define STRSAFE_NO_CB_FUNCTIONS -- allow only character count functions(只允许“字符”函数，不用Byte计数？)
*    STRSAFE_NO_CCH_FUNCTIONS -- allow only byte count functions(只允许Byte函数？)
*    STRSAFE_MAX_CCH -- 支持的最大的字符串长度(2,147,483,647)
*
* 有问题的代码：没有检查返回值；strncat中错误使用cchPath(应是剩余空间长度，而不是总长度)
* void UnsafeFunc(LPTSTR szPath,DWORD cchPath) 
* {
*   TCHAR szCWD[MAX_PATH];
*   GetCurrentDirectory(ARRAYSIZE(szCWD), szCWD);
*   strncpy(szPath, szCWD, cchPath);
*   strncat(szPath, TEXT("\\"), cchPath);
*   strncat(szPath, TEXT("desktop.ini"),cchPath);
* }
* 
* 好的代码：StringCchCat中使用的 cchPath 代表目标缓冲区的总长度。
* bool SaferFunc(LPTSTR szPath,DWORD cchPath) 
* {
*   TCHAR szCWD[MAX_PATH];
*   if (GetCurrentDirectory(ARRAYSIZE(szCWD), szCWD) &&
*     SUCCEEDED(StringCchCopy(szPath, cchPath, szCWD)) &&
*     SUCCEEDED(StringCchCat(szPath, cchPath, TEXT("\\"))) &&
*     SUCCEEDED(StringCchCat(szPath, cchPath, TEXT("desktop.ini")))) {
*       return true;
*   }
*   return false;
* }	
*
* strsafe 系列函数的Ex版本可以实现更加高级的功能：
*   获取目标缓冲区的当前指针。
*   获取目标缓冲区的剩余空间长度。
*   以某个特定字符填充空闲缓冲区。
*   一旦字符串处理函数失败，就把用特定值填充字符串。
*   一旦字符串处理函数失败，就把目标缓冲区设成 NULL 。
***************************************************************************************************/

class CStrSafeTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CStrSafeTester );

    CPPUNIT_TEST(test_StringCchCat );
    //CPPUNIT_TEST(test_StringCchCatEx );
    //CPPUNIT_TEST(test_StringCchCatN );
    //CPPUNIT_TEST(test_StringCchCatNEx );
    CPPUNIT_TEST(test_StringCchCopy );
    //CPPUNIT_TEST(test_StringCchCopyEx );
    //CPPUNIT_TEST(test_StringCchCopyN );
    //CPPUNIT_TEST(test_StringCchCopyNEx );
    //CPPUNIT_TEST(test_StringCchGets );
    //CPPUNIT_TEST(test_StringCchGetsEx );
    //CPPUNIT_TEST(test_StringCchLength );
    CPPUNIT_TEST(test_StringCchPrintf );
    CPPUNIT_TEST(test_StringCchPrintfEx );
    //CPPUNIT_TEST(test_StringCchVPrintf );
    //CPPUNIT_TEST(test_StringCchVPrintfEx );

    CPPUNIT_TEST_SUITE_END();

    void test_StringCchCat();
    //void test_StringCchCatEx();
    //void test_StringCchCatN();
    //void test_StringCchCatNEx();
    void test_StringCchCopy();      //范围是Buffer的大小，会自动填充最后的NULL
    //void test_StringCchCopyEx();
    //void test_StringCchCopyN();
    //void test_StringCchCopyNEx();
    //void test_StringCchGets();
    //void test_StringCchGetsEx();
    //void test_StringCchLength();
    void test_StringCchPrintf();    //范围是Buffer的大小，会自动填充最后的NULL
    void test_StringCchPrintfEx();
    //void test_StringCchVPrintf();
    //void test_StringCchVPrintfEx();
    DECLARE_DEFAULT_TEST_CLASS(CStrSafeTester);
private:
    void ResetBuf(LPTSTR pbuf,DWORD cCount)
    {
#ifdef UNICODE
        wmemset(pbuf,TEXT('*'),cCount);
#else
        memset(pbuf,TEXT('*'),cCount);
#endif
    }
};
