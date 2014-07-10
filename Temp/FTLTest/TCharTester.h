#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <tchar.h>

/***************************************************************************************************
* VS2005 中提供的字符串安全函数(_s) -- 采用调试版本中使用断言错误，Release版本中直接崩溃的方式来“防止”
*    缓冲区溢出。而且无法用异常处理捕获(C++异常 和 结构化异常)。
*    而非安全函数版本是否崩溃则依赖于缓冲区是否覆盖返回地址。
*
* _tcsncpy_s 给的缓冲大小 包含了  结束NULL，会   自动填充最后的NULL
* _tcsncpy   给的缓冲大小 不包含  结束NULL，不会 自动填充最后的NULL，需要手工填充确保以NULL结束
***************************************************************************************************/

class CTCharTester : public CPPUNIT_NS::TestFixture
{

    CPPUNIT_TEST_SUITE( CTCharTester );

    CPPUNIT_TEST( test_tcscpy );

    CPPUNIT_TEST_SUITE_END();


    void test_tcscpy();

    DECLARE_DEFAULT_TEST_CLASS(CTCharTester);

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