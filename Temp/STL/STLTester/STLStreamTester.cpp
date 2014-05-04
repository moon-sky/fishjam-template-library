#include "StdAfx.h"
#include "STLStreamTester.h"

#include <string>
#include <vector>
#include <fstream>
#include <strstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CSTLStreamTester::test_ifstream()
{
    const char* fileNames[] = 
    {
        //无论是Windows格式还是Unix格式，结果都一样
        "ReadMe.txt",
        "ReadMe_Unix.txt"
    };
    for (int i = 0; i < _countof(fileNames) ; ++i)
    {
        std::string strtmp;
        //std::vector<std::string> vect;
        int TotalLineCount = 0;
        int NotEmptyCount = 0;
        std::ifstream in(fileNames[i]);//打开文件

        while(getline(in, strtmp, '\n'))  //循环读取每一行
        {
            ++TotalLineCount;
            if (!strtmp.empty())
            {
                ++NotEmptyCount;
            }

            // 将每一行中空格以前的信息放到 vector<string>中
            //vect.push_back(strtmp.substr(0, strtmp.find(' ')));
        }
        //ReadMe.txt 文件的行数 -- 注意最后一行如果是空行的话将不会被统计(和没有最后一样一样)
        CPPUNIT_ASSERT(93 == TotalLineCount);
        CPPUNIT_ASSERT(68 == NotEmptyCount);

        //数出来空行是25行
        CPPUNIT_ASSERT(25 + NotEmptyCount == TotalLineCount);

        //输出到屏幕中
        //std::copy(vect.begin(), vect.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
    }
}

void CSTLStreamTester::test_ostringstream()
{
    std::ostringstream os;
    os << "Hello" << " World " << 100;
    std::string s = os.str();
    CPPUNIT_ASSERT(s == "Hello World 100");
}

void CSTLStreamTester::test_istreambuf_iterator()
{
    //把一个文本文件拷贝到一个字符串对象中
    std::ifstream inputFile("STLStreamTester.h");
    
    //istream_iterator<char>对象使用operator>>来从输入流中读取单个字符。
    //istreambuf_iterator<char>对象进入流的缓冲区并直接读取下一个字符。
    //即：一个istreambuf_iterator<char> 对象从一个istream s中读取会调用s.rdbuf()->sgetc()来读s的下一个字符

#if 0
    inputFile.unset(ios::skipws);				// 关闭inputFile的忽略空格标志 -- 可以读入空格
    std::string fileData((istream_iterator<char>(inputFile)), istream_iterator<char>());    //性能差
#endif
    //istreambuf_iterator 不需要设置空格标志
    std::string fileData((std::istreambuf_iterator<char>(inputFile)),std::istreambuf_iterator<char>()); //性能好

    TRACE("**********************************************************************\n");
    TRACE("%s\n",fileData.c_str());
    TRACE("**********************************************************************\n");
}

void CSTLStreamTester::test_ostrstream()
{

}