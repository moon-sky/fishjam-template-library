#include "StdAfx.h"
#include "Cpp2011Tester.h"
#include <vector>
//#include <regex>

#if SUPPORT_CPP2011  //VS2012

void CCpp2011Tester::test_auto()
{
	std::vector<int> myIntVector;
	
	
	for (auto iter = myIntVector.begin(); iter != myIntVector.end(); ++iter)
	{
		//可以避免写成如下的新式:
		//for(std::vector<int>::const_iterator iter = myIntVector.begin(); iter != myIntVector.end(); ++itr)
	}
	
}

//constexpr int GetFive() 
//{ 
//	//constexpr 使得 编译器理解并确认 GetFive 是个编译期常量
//	return 5;
//}

void CCpp2011Tester::test_constexpr()
{
//	int num[GetFive()] = {1, 2, 3, 4, 5};
}

void CCpp2011Tester::test_decltype()
{
	LONGLONG someTypeValue = 1;
	decltype(someTypeValue) otherIntegerVariable = 5;

	//TODO:C++ 中如何判断类型相等？
	CPPUNIT_ASSERT(sizeof(otherIntegerVariable) == sizeof(LONGLONG));  //otherIntegerVariable 的类型是someTypeValue的类型
}

//class SequenceClass
//{
//public:
//	//初始化列表构造器
//	SequenceClass(std::initializer_list< int >list)
//	{
//
//	}
//	//使用初始化列表的普通函数
//	void SomeFunctionName(std::initializer_list<float> list)
//	{
//
//	}
//};

void CCpp2011Tester::test_initializer_list()
{
	////初始化列表是常量，一旦构建，组成列表的成员以及其成员所包含的数据便无法改变
	//SequenceClass myObject = {1, 2, 3, 4};
	//myObject.SomeFunctionName({3, 4, 5, 6});
}

void CCpp2011Tester::test_lambda()
{
    std::vector<int>  intVect(10);
    //使用 generate 在已知大小的 vector 中生成 [1,10]
    std::generate(intVect.begin(), intVect.end(), FTL::sequence_generator<int>(1,1));

    int total = 0;
    std::for_each(intVect.begin(), intVect.end(), [&total](int x) {
        //闭包变量total是栈变量total的引用，使用前者可以改变后者的值 -- TODO:闭包变量名是否有要求
        total += x
    });
    CPPUNIT_ASSERT(total == 55);

    //如果一个lambda函数由一个类的某个成员函数定义，那么此lambda函数便被认定为该类的友元。
    //只有当lambda函数在SomeType的某个成员函数中创建时这段代码才能工作。
    //[](SomeType *typePtr) { typePtr->SomePrivateMemberFunction() };


    //如果用户希望把lambda函数当作参数，那么要么参数相应类型为模板，要么创建一个std::function用于保存lambda函数
    //使用auto关键字则可以将lambda函数保存在局部变量中
    auto myLambdaFunc = [this]() {
        this->SomePrivateMemberFunction()
    };
}

//定义名为 LessThanComparable 的约束 -- 表明需要有小于操作符
auto concept LessThanComparable < typename T >
{
    bool operator<(T, T);
}

//使用 LessThanComparable 约束来定义模板函数
template<typename T> requires LessThanComparable<T>
const T& MyMin(const T &x, const T &y)
{
    return y < x ? y : x;
}

void CCpp2011Tester::test_requires()
{

}

//std::vector<CMyTestData>&& GetTempVector()
//{
//	std::vector<CMyTestData>	tempVector;
//	tempVector.push_back(CMyTestData(1));
//	return tempVector;
//}

void CCpp2011Tester::test_right_reference()
{
	//有了右值引用，一个参数为指向某个vector的右值引用的std::vector的转移构造器就能够简单地将该右值中C式数组的指针复制到新的vector，然后将该右值清空。
	//这样没有数组拷贝，并且销毁被清空的右值也不会销毁保存数据的内存 -- 需要 转移构造器 的支持
	//std::vector<CMyTestData>	myDataVector = GetTempVector();
}

void CCpp2011Tester::test_scope_for()
{
	int my_array[5] = { 1, 2, 3, 4, 5 };	
	for(int &x:			//定义了用于在区间上循环的变量,该变量的作用域也仅限于循环之内
		my_array)		//表示将进行循环的区间, 进行循环的区间还可以是std::vector，或任何符合区间概念的对象
	{
		x *= 2;
	}
	CPPUNIT_ASSERT(x == 120);
}
#endif //SUPPORT_CPP2011
