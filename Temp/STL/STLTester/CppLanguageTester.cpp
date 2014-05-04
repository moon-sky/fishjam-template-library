#include "StdAfx.h"
#include "CppLanguageTester.h"

//结构体中如果有指针,会直接将值拷贝过去 -- 通常是错误的
//  因此必须写拷贝赋值函数和拷贝构造函数
struct BadStructPointerAssign
{
    //什么都不写，采用的是浅拷贝，拷贝后指针指向相同的地址
    int iValue;
    CMyTestData*    pData;
};

struct GoodStructPointrAssign
{
    int iValue;
    CMyTestData*    pData;

    GoodStructPointrAssign()
    {
        iValue = 0;
        pData = NULL;
    }

    GoodStructPointrAssign(const GoodStructPointrAssign& other)
        :pData(NULL)
    {
        *this = other;
    }

    GoodStructPointrAssign& operator= (const GoodStructPointrAssign& other)
    {
        if(this != &other)
        {
            this->iValue = other.iValue;
            SAFE_DELETE(this->pData);
            if (other.pData)
            {
                this->pData = new CMyTestData(*other.pData);
            }
        }
        return *this;
    }
    ~GoodStructPointrAssign()
    {
        SAFE_DELETE(pData);
    }
};

//结构体总如果是类变量，在赋值时会自动调用其拷贝构造 -- 通常是正确的
struct GoodStructObjectAssign
{
    int iValue;
    CMyTestData    data;
};

//使用初始化列表的方式
class UseConstrcutInitList
{
public:
    UseConstrcutInitList(LONG i)
        :m_Data(i)
    {

    }
    ~UseConstrcutInitList()
    {
        
    }
private:
    CMyTestData m_Data;
};

#ifdef NEED_OPERATOR_FUNC

//使用构造函数中赋值的方式
class UseConstrcutAssignment
{
public:
    UseConstrcutAssignment(BOOL bAssigneFromLONG, LONG i)
    {
        //两个的写法虽然不同，但实质一致
        //此处直接使用了整型赋值给类的方式，会有 1次缺省构造 + 1次 LONG 的构造 + 1次赋值操作 + 2次析构
        if(bAssigneFromLONG)
        {
            m_Data = i;
        }
        else
        {
            //如果构造函数中有 excplit 修饰的话，只能用这种方式：会有 1次缺省构造 + 1次 LONG 的构造 + 1次赋值操作 + 2次析构
            m_Data = CMyTestData(1);
        }
    }
    ~UseConstrcutAssignment()
    {
        
    }
private:
    CMyTestData m_Data;
};
#endif

//////////////////////////////////////////////////////////////////////////

CCppLanguageTester::CCppLanguageTester()
{
    m_bCallGetMyTestDataInstance = FALSE;
}

CCppLanguageTester::~CCppLanguageTester()
{

}

void CCppLanguageTester::test_bad_struct_pointer_assign()
{
    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker, ctDataCount, 0, __FILE__, __LINE__);
    LONG curTestDataCount = CMyTestData::GetTestDataCount();
    
    BadStructPointerAssign badStructSrc = {0};
    badStructSrc.iValue = 100;
    badStructSrc.pData = new CMyTestData(badStructSrc.iValue);
    
    BadStructPointerAssign badStructDest = badStructSrc;
    //结构体赋值时没有重载 = 符号，是按照位拷贝的方式进行,因此有两个 CMyTestData 的指针，但指向的是同一个实例
    CPPUNIT_ASSERT(badStructSrc.iValue == badStructDest.iValue);
    CPPUNIT_ASSERT(badStructSrc.pData == badStructDest.pData);

#pragma TODO ( " 只能释放一次，否则就会出现 double free 的错误" )
    SAFE_DELETE(badStructSrc.pData);
    //SAFE_DELETE(badStructDest.pData);
    badStructDest.pData = NULL;

    CPPUNIT_ASSERT(curTestDataCount == CMyTestData::GetTestDataCount());
}

void CCppLanguageTester::test_construct_init_list()
{
    {
        //使用初始化列表的方式
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker, ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCountChecker, ctCopyConstructCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker, ctConstructCount, 1, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCheck, ctDestructCount, 1, __FILE__, __LINE__);
        UseConstrcutInitList  useInitListObj(10);
    }

#ifdef NEED_OPERATOR_FUNC
    {
        //使用构造函数中赋值的方式一：整型直接赋值给类
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker, ctDataCount, 0, __FILE__, __LINE__);

        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 2, __FILE__, __LINE__);         //一次缺省构造、一次带LONG参数的构造
        DECLARE_MYTEST_DATA_COUNT_CHECKER(assignmentCountChecker,ctAssignmentCount, 1, __FILE__, __LINE__);       //一次赋值操作
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCheck,ctDestructCount, 2, __FILE__, __LINE__);                  //两次析构
        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCountChecker,ctCopyConstructCount, 0, __FILE__, __LINE__); //注意：没有拷贝构造
        UseConstrcutAssignment  useAssignmentObj(TRUE, 11);
    }

    {
        //使用构造函数中赋值的方式二：拷贝构造(构造函数中有 explict 修饰
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 2, __FILE__, __LINE__);         //一次缺省构造、一次带LONG参数的构造
        DECLARE_MYTEST_DATA_COUNT_CHECKER(assignmentCountChecker,ctAssignmentCount, 1, __FILE__, __LINE__);       //一次赋值操作
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCheck,ctDestructCount, 2, __FILE__, __LINE__);                  //两次析构
        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCountChecker,ctCopyConstructCount, 0, __FILE__, __LINE__); //注意：没有拷贝构造
        UseConstrcutAssignment  useAssignmentObj(FALSE, 11);
    }
#endif

}

void CCppLanguageTester::test_exception_lifecycle()
{
    {
        //捕获抛出 new 后的异常，必须 catch 指针才行
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCheck,ctDestructCount, 1, __FILE__, __LINE__);
        try
        {
            throw new CMyTestData(1);
            ASSERT(FALSE);      //不会到这里来
        }
        catch (/*const*/ CMyTestData* e) //是否加const都可以
        {
            //new出来的异常必须手工delete,否则就是内存泄漏
            delete e;
        }
    }


    {
        //特别注意: 使用 catch(...) 捕获异常的话可能造成内存泄漏 
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker,ctDataCount, 1, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCheck,ctConstructCount, 1, __FILE__, __LINE__);

        //new出来的异常没有被释放，造成内存泄漏 -- 也说明了抛出异常时，不要使用 new 的方式
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCheck,ctDestructCount, 0, __FILE__, __LINE__);  
        try
        {
            throw new CMyTestData(1);
            ASSERT(FALSE);      //不会到这里来
        }
        catch (...)//由点点点捕获异常
        {
            TRACE(TEXT("in catch ...\n"));
        }
    }

    {
        //捕获局部变量的异常，必须 catch 引用才行 -- 如果是值？
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructChecker,ctConstructCount, 1, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCheck,ctDestructCount, 1, __FILE__, __LINE__);
        try
        {
            throw CMyTestData(2);
            ASSERT(FALSE);      //不会到这里来
        }
        catch (/*const*/ CMyTestData& e) //是否加const都可以
        {
            TRACE(TEXT("In catch (CMyTestData& e), e.GetCurrentIndex=%d\n"), e.GetCurrentIndex());
        }
    }

}

void CCppLanguageTester::test_good_struct_pointer_assign()
{
    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker,ctDataCount, 0, __FILE__, __LINE__);
    DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCheck,ctDestructCount, 2, __FILE__, __LINE__);
    DECLARE_MYTEST_DATA_COUNT_CHECKER(constructChecker,ctConstructCount, 1, __FILE__, __LINE__);
    DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCheck,ctCopyConstructCount, 1, __FILE__, __LINE__);
    {
        GoodStructPointrAssign goodStructSrc;
        goodStructSrc.iValue = 100;
        goodStructSrc.pData = new CMyTestData(goodStructSrc.iValue);

        CPPUNIT_ASSERT(goodStructSrc.pData->GetCurrentIndex() == 100);

        //重载了赋值操作，对指针进行了深拷贝
        GoodStructPointrAssign goodStructDest = goodStructSrc ;
        CPPUNIT_ASSERT(goodStructSrc.pData->GetCurrentIndex() == goodStructDest.pData->GetCurrentIndex());

        //两个的地址不一样
        CPPUNIT_ASSERT(goodStructSrc.pData != goodStructDest.pData);
    }
}

void CCppLanguageTester::test_good_struct_object_assign()
{
    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker,ctDataCount, 0, __FILE__, __LINE__);
    DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCheck,ctDestructCount, 2, __FILE__, __LINE__);
    DECLARE_MYTEST_DATA_COUNT_CHECKER(constructChecker,ctConstructCount, 1, __FILE__, __LINE__);
    DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCheck,ctCopyConstructCount, 1, __FILE__, __LINE__);
    {
        GoodStructObjectAssign goodStructSrc;
        goodStructSrc.iValue = 100;

        //调用的是缺省构造函数
        CPPUNIT_ASSERT(goodStructSrc.data.GetCurrentIndex() == -1);
        goodStructSrc.data.SetCurrentIndex(10);

        //虽然结构体没有重载拷贝构造或赋值操作，但C++会对其中的类自动调用拷贝构造
        GoodStructObjectAssign goodStructDest = goodStructSrc;
        CPPUNIT_ASSERT(goodStructSrc.data.GetCurrentIndex() == goodStructDest.data.GetCurrentIndex());

        //两个的地址不一样
        CPPUNIT_ASSERT(&(goodStructSrc.data) != &(goodStructDest.data));
    }
}

class BigClass
{
public: 
   BigClass()   {} 
   ~BigClass(){} 
   char   BigArray[0x7FFFFFFF]; 
};

int my_new_handler(size_t) {
    throw std::bad_alloc();
    return 0;
}

void unfunction( ) 
{
    throw std::bad_alloc();
}


void CCppLanguageTester::test_cpp_alloc_fail()
{
    //分配BigClass的空间失败，且设置了 nothrow,因此会返回 NULL
    {
        BigClass* pFailObj = new (std::nothrow) BigClass();
        CPPUNIT_ASSERT(NULL == pFailObj);
    }

    {
        BOOL bThrowException = FALSE;
        _PNH _old_new_handler = NULL;
        //set_unexpected(unfunction);
        _old_new_handler = _set_new_handler(my_new_handler);
        BOOL bCatchInDot = FALSE;
        try
        {
            BigClass* pTemp = new BigClass();
            SAFE_DELETE(pTemp);
        }
        catch (std::bad_alloc& e)
        {
            bThrowException = TRUE;
        }
        catch(std::exception& e)
        {
            bThrowException = TRUE;
        }
        catch(...)
        {
            FTLTRACE(TEXT("In Catch new Faild\n"));
            bCatchInDot = TRUE;
        }
        _set_new_handler(_old_new_handler);

#pragma TODO(为什么 bad_alloc 捕获不了)
        CPPUNIT_ASSERT(bCatchInDot);

        //CPPUNIT_ASSERT(bThrowException);
    }

    {
        //BigClass* pTemp = new( std::nothrow ) BigClass();
        //CPPUNIT_ASSERT(NULL == pTemp);
    }
}

void CCppLanguageTester::test_cpp_alloc_physical_memory()
{
    MEMORYSTATUS startMemoryStatus = {0}; 
    MEMORYSTATUS afterNewMemoryStatus = {0};
    MEMORYSTATUS afterAccessMemoryStatus = {0};

    GlobalMemoryStatus(&startMemoryStatus);
    
    //申请分配10M的内存空间
    const int nAllocSize = 10 * 1024 * 1024;
    BYTE *pBuff = new BYTE[ nAllocSize ];
    GlobalMemoryStatus(&afterNewMemoryStatus);

    ZeroMemory(pBuff, nAllocSize);
    GlobalMemoryStatus(&afterAccessMemoryStatus);

    delete [] pBuff;

    FTLTRACE(TEXT("Memory Status: Start -> AfterNew -> AfterAccess\n")), 
    FTLTRACE(TEXT("  AvailPhys: %fM -> %fM -> %fM\n"), 
        (float)startMemoryStatus.dwAvailPhys/1024/1024,
        (float)afterNewMemoryStatus.dwAvailPhys/1024/1024,
        (float)afterAccessMemoryStatus.dwAvailPhys/1024/1024);
    FTLTRACE(TEXT("  AvailVirtual: %fM -> %fM -> %fM\n"), 
        (float)startMemoryStatus.dwAvailVirtual/1024/1024,
        (float)afterNewMemoryStatus.dwAvailVirtual/1024/1024,
        (float)afterAccessMemoryStatus.dwAvailVirtual/1024/1024);

    //调用new以后内存即会分配
    CPPUNIT_ASSERT(startMemoryStatus.dwAvailPhys - afterNewMemoryStatus.dwAvailPhys >= nAllocSize);
    
}

void CCppLanguageTester::test_anonymous_temp_objcet_Life()
{

    {
        LONG oldCount = CMyTestData::GetTestDataCount();

        //匿名对象，尚未出生命周期，立刻就释放了
        CMyTestData(1);

        LONG newCount = CMyTestData::GetTestDataCount();
        CPPUNIT_ASSERT(newCount == oldCount);
    }

    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker,ctDataCount, 0, __FILE__, __LINE__);
    {
        LONG oldCount = CMyTestData::GetTestDataCount();
        //定义的局部对象，在括号限制的生命周期内
        CMyTestData  tmpData(1);
        LONG newCount = CMyTestData::GetTestDataCount();

        //此时尚未出生命周期，因此还有一个
        CPPUNIT_ASSERT(newCount == oldCount + 1);
    }
}

CMyTestData& GetMyTestDataInstance()
{
    //函数中的静态变量所占用的内存在程序启动时分配，但直到第一次调用时才初始化(构造)，并直到程序退出时才释放
    static CMyTestData s_MyTestData;
    return s_MyTestData;
}
void CCppLanguageTester::test_static_var_life()
{
    if(!m_bCallGetMyTestDataInstance)
    {
        m_bCallGetMyTestDataInstance = TRUE;
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker,ctDataCount, 1, __FILE__, __LINE__);            //第一次调用时，会生成实例对象
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructChecker,ctConstructCount, 1, __FILE__, __LINE__);  //此处调用了一次缺省构造
        CMyTestData& instanceData = GetMyTestDataInstance();

    }
    else
    {
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker, ctDataCount, 0, __FILE__, __LINE__);  //不会生成新的实例
        CMyTestData& instanceData = GetMyTestDataInstance();
    }
}

//枚举值的作用域为枚举的外围作用域。两个不同的枚举不能包含名字相同的成员。
enum Enum1
{
    someType,   //缺省是从0开始
};
enum Enum2
{
#if 0
    someType /*= 1*/,  //这句话会因为和 Enum1::someType 冲突，而造成编译错误 -- C2365
#endif 
    someTypeInEnum2,
};
void CCppLanguageTester::test_enum()
{
    Enum1 myEnum = someType;
    CPPUNIT_ASSERT(myEnum == 0);
    CPPUNIT_ASSERT(Enum2::someTypeInEnum2 == 0);
}