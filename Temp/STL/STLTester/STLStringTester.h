#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* string并不是一个单独的容器，只是basic_string 模板类的一个typedef 而已,相对应的还有wstring -- 没有tstring，
* basic_string 是基于字符序列容器(Sequence)的模板类, 包含了所有序列容器的常用操作，同时也包含了字符串的标准操作，
*   通过字符类型和类型的特征( Character Traits )来实例化
* 词典字符串比较建立在字符比较之上。一旦你有了一个忽略大小写的字符比较函数对象，问题就解决了。
*  标准未规定string必须使用连续内存和带空字符的结束标志
*  c_str() -- 获得以空字符结束的C风格字符串
*  data() -- 不保证以空字符结束
* 判断字符串是否为空，应用 empty 函数，而不要用常串比较
************************************************************************/

#if 0
//忽略大小写字符串比较的函数
int ciStringCompare(const string& s1, const string& s2);

//用于忽略大小写字符串比较的仿函数类 -- 参见 Effective STL 中的条款19
struct CIStringCompare : public binary_function<string, string, bool>
{
    bool operator()(const string& lhs, const string& rhs) const
    {
        return ciStringCompare(lhs, rhs);
    }
};
//以忽略大小写的方式排序字符串的vector.
// std::sort(v.begin(),v.end(),CIStringCompare());

//另外一种忽视大小写德字符串比较类 -- 参见 Effective STL 中的附录 A
struct lt_str_1 : public std::binary_function<std::string, std::string, bool> {
        struct lt_char {
            const std::ctype<char>& ct;
            lt_char(const std::ctype<char>& c) : ct(c) {}
            bool operator()(char x, char y) const {
                return ct.toupper(x) < ct.toupper(y);
            }
        };
        std::locale loc;
        const std::ctype<char>& ct;
        lt_str_1(const std::locale& L = std::locale::classic())
            : loc(L), ct(std::use facet<std::ctype<char> >(loc)) {}

        bool operator()(const std::string& x, const std::string& y) const{
            return std::lexicographical_compare(x.begin(), x.end(),
                y.begin(), y.end(),
                lt_char(ct));
        }
};
#endif 

class CSTLStringTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLStringTester );
    CPPUNIT_TEST( test_reserve );
    CPPUNIT_TEST( test_string );
    CPPUNIT_TEST( test_swap );
    CPPUNIT_TEST_SUITE_END();

    void test_reserve();
    void test_string();
    void test_swap();

    DECLARE_DEFAULT_TEST_CLASS(CSTLStringTester);
};
