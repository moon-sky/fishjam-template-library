#include "StdAfx.h"
#include "STLStringTester.h"
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CSTLStringTester::test_reserve()
{
    
}

void CSTLStringTester::test_string()
{
    std::string ss = "test string";
    std::string::value_type v = 'a';
    std::string::pointer p = "this is a const string"; //char* p = ...
    std::string::reference r = *p;
    
    // npos = -1(0xFFFFFFFF),max_size = -2(0xFFFFFFFE), capacity = 
    TRACE("string::npos = 0x%08X, max_size = 0x%08X, capacity = %d\n",std::string::npos, ss.max_size(),ss.capacity());  

    CPPUNIT_ASSERT(ss.length() == ss.size());   //string 中两者相等

    //insert,c_str,data,reserve,swap,append,push_back,erase,clear,resize,assign,replace,copy
    //find,rfind,find_first_of,find_first_not_of,find_last_of,find_last_not_of,substr,compare
    
}

void CSTLStringTester::test_swap()
{

}