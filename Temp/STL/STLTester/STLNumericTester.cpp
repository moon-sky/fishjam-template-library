#include "StdAfx.h"
#include "STLNumericTester.h"

#include <numeric>
#include <list>
#include <set>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//统计时使用的函数：带有目前的和与新的字符串，而且返回更新的和
std::string::size_type stringLengthSum(std::string::size_type sumSoFar, const std::string& s)
{
    return sumSoFar + s.size();
}

void CSTLNumericTester::test_accumulate()
{
    //统计 list 中 double 数的和
    std::list<double> ld;
    for (int i = 1; i< 10; i++)
    {
        ld.push_back(i + 0.1*i);
    }
    //{ 1.1 + 2.2 + 3.3 + 4.4 + 5.5 + 6.6 + 7.7 + 8.8 + 9.9 }
    double sumDouble = std::accumulate(ld.begin(), ld.end(), 0.0);  //初始值是 0.0 (double型)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sumDouble,49.5,0.1);


    //计算数值区间的积, 1.1 * 2.2 * 3.3 * 4.4 * 5.5 * 6.6 * 7.7 * 8.8 * 9.9
    double multiDouble = std::accumulate(ld.begin(),ld.end(),1.0,
        std::multiplies<double>()); //使用标准的 multiplies 仿函数类
    CPPUNIT_ASSERT_DOUBLES_EQUAL(multiDouble,855652.06 , 0.01); //855652.05811008
    
#if 0
    //accumulate只需要输入迭代器，所以你甚至可以使用istream_iterator和istreambuf_iterator
    // 打印cin中那些int的和
    cout << "The sum of the ints on the standard input is"		
        << accumulate(istream_iterator<int>(cin),istream_iterator<int>(),0);
#endif

    //使用accumulate来计算容器中的字符串的长度和
    std::set<std::string> strList;
    strList.insert("fishjam ");
    strList.insert("study ");
    strList.insert("STL");
    
    std::string::size_type sumString = accumulate(strList.begin(),strList.end(),0,stringLengthSum);
    CPPUNIT_ASSERT(sumString == 17);// 总长度应该是17


}