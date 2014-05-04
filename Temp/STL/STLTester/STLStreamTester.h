#pragma once

#include <cppunit/extensions/HelperMacros.h>

/*******************************************************************************************************************
* << (插入器) -- 向流中输出数据
* >> (析取器) -- 从流中输入数据
* 将输入/输出流作为容器看待 -- 任何接受迭代器参数的算法都可以和流一起工作
*
* ios_base
*   <== basic_ios -- ios<char>, 其中定义了打开方式,如 app/ate/binary/in/out 等
*     <== basic_istream -- istream<char>
*       <== basic_ifstream -- ifstream<char>,从文件读入 
*       <== basic_istringstream -- istringstream<char>
*     <== basic_ostream -- ostream<char>
*       <== basic_ofstream -- ofstream<char>, 输出到文件
*       <== basic_ostringstream -- ostringstream<char>
*       <== ostrstream -- 已经被淘汰了(使用C格式的字符串,返回char*,不会自己释放内存),最好用 ostringstream 
*
*      <== basic_iostream(同时从 basic_istream 和 basic_ostream 继承,同时支持输入/输出) -- iostream<char>
*         <== basic_fstream -- fstream<char>
*         <== basic_stringstream -- stringstream<char>
*         <== strstream<char>
*
* basic_streambuf -- streambuf<char>
* basic_stringbuf -- stringbuf<char>
*
* eof -- 检测是否到达文件尾
*
* C++ I/O系统管理两个与一个文件相联系的指针，每次执行输入或输出时，相应的指针自动变化。
*   读指针 -- 输入操作在文件中的位置
*   写指针 -- 下次写操作的位置
*******************************************************************************************************************/


/*******************************************************************************************************************
*
* ifstream in("name.txt", ios::binary|ios::in, 打开文件属性[0-普通,1-只读,2-隐含,4-系统]); -- 打开文件
* while(getline(in, strtmp, '\n'))  //循环读取每一行
*   vect.push_back(strtmp.substr(0, strtmp.find(' '))); 将每一行中空格以前的信息放到 vector<string>中
*   //copy(istream_iterator<string>(ifs), istream_iterator<string>(), back_inserter(vec_str));
* copy(vect.begin(), vect.end(), ostream_iterator<string>(cout, "\n"));  //输出到屏幕中 -- 怎么去掉最后一个符号?
* 
* 可以设置 cout 输出的格式(如16进制输出hex,10进制输出dec,8进制输出oct), endl-空行, ends-空字符
*   setpxecision(n) -- 设置浮点数的精度位数输出, 如 cout << setpxecision(5) << PI 
*   cout.setf(ios::hex,ios::basefield);
*   cout<<"hr="<<hr<<endl;  //或者 cout << std::hex << hr
*
* 使用宽字符的 wcout
* 
* 格式化
*   cout << setiosflags(ios::left) << setw(15) << "CString:" << setw(50) << strResult.GetBuffer(0) << endl;
*   cout << setiosflags(ios::left) << setw(15) << "LPCTSTR:" << setw(50) << szResult << endl;
*   cout << setiosflags(ios::left) << setw(15) << "LPCTSTR1:" << setw(50) << szResult1 << endl;
*
* 二进制读写:
*   put/get
*   read -- 通过 gcount 取得实际读取的字符数
*   write
*   seekg -- 定位读指针
*   seekp -- 定位写指针
*   
* 注意：
*   1.ostringstream 虽然在格式化输出时很不错，但因为其构造函数中会调用 locale 的构造，造成全局锁。因此在多线程环境下，
*     如果频繁创建 ostringstream 的实例，会造成性能的急剧下降 -- 除了换用 snprintf 等外，还有别的方法吗？是否可以尝试重用 ostringstream
*     basic_istringstream => locale::locale => pthread_mutex_lock
*   2.ostringstream 的 str().c_str() 返回的是临时对象，表达式结尾会析构，c_str() 返回的将是野指针，
*     应该保存临时对象然后进行使用: const string& strTemp = ostream.str(); const char* cValue = strTemp.c_str();
*     
*******************************************************************************************************************/
class CSTLStreamTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLStreamTester );
    CPPUNIT_TEST( test_ifstream );
    CPPUNIT_TEST( test_istreambuf_iterator );
    CPPUNIT_TEST( test_ostringstream );
    CPPUNIT_TEST( test_ostrstream );
    CPPUNIT_TEST_SUITE_END();

    void test_ifstream();

    //需要一个一个字符输入时考虑使用 istreambuf_iterator -- Effective STL 的规则29
    void test_istreambuf_iterator();

    void test_ostringstream();

    //调用 str 方法后必须调用 freeze(false) 方法，否则会造成内存泄漏 -- 建议用 ostringstream 
    void test_ostrstream();

    DECLARE_DEFAULT_TEST_CLASS(CSTLStreamTester);
};