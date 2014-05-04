#include "StdAfx.h"
#include "STLTupleTester.h"
#include <tuple>
using std::tr1::tuple;

void CSTLTupleTester::test_tuple_init()
{
	typedef std::tr1::tuple<int, std::string, std::wstring>	NumberTransfers;
	//NumberTransfers numbers[] = 
	//{
	//	(1, "one", L"一"),
	//	(2, "two", L"二"),
	//	
	//};

	//申明2个元素元组并进行初始化 -- 如果要初始化，则必须全部初始化，否则会报错
	NumberTransfers one(1, "one", L"一" );


//#if SUPPORT_CPP2011
//	//NumberTransfers two = make_tuple(2, "two", L"二");
//#endif //SUPPORT_CPP2011
}