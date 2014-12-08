#include "StdAfx.h"
#include "STLMemoryTester.h"
#include <memory>
#include <iterator>
using std::tr1::shared_ptr;
using std::tr1::enable_shared_from_this;
using std::tr1::dynamic_pointer_cast;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CEnableSharedFromThis : public enable_shared_from_this<CEnableSharedFromThis>{
public:
    static LONG s_Count;
public:
    CEnableSharedFromThis(){
        InterlockedIncrement(&s_Count);
        FTLTRACE(TEXT("CEnableSharedFromThis  Construct\n"));
    }
    virtual ~CEnableSharedFromThis(){
        InterlockedDecrement(&s_Count);
        FTLTRACE(TEXT("CEnableSharedFromThis  Destruct\n"));
    }
    shared_ptr<CEnableSharedFromThis> getSharedThis(){
        return shared_from_this();
    }
    virtual int GetIdentify(){
        return 1;
    }
};
LONG CEnableSharedFromThis::s_Count = 0;

class CEnableSharedFromThisChild : public CEnableSharedFromThis{
public:
    CEnableSharedFromThisChild(){
        FTLTRACE(TEXT("CEnableSharedFromThisChild  Construct\n"));
    }
    virtual ~CEnableSharedFromThisChild(){
        FTLTRACE(TEXT("CEnableSharedFromThisChild  Destruct\n"));
    }
    virtual int GetIdentify(){
        return 2;
    }
    int CallChildMethod(){return 3;}
};


void CSTLMemoryTester::test_auto_ptr()
{
    DECLARE_MYTEST_DATA_COUNT_CHECKER(ConstructCountChecker,ctDataCount, 0, __FILE__, __LINE__);
    {
        //测试超出范围后自动释放
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 1, __FILE__, __LINE__);
        {
            std::auto_ptr<CMyTestData> pMyTestData(new CMyTestData(1));
            CPPUNIT_ASSERT(pMyTestData.get()->GetCurrentIndex() == 1);
        }//此处以后自动释放
    }

    {
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 1, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructChecker,ctCopyConstructCount, 0, __FILE__, __LINE__); //只有指针的拷贝，不会有拷贝构造函数

        //测试auto_ptr进行赋值时的所有权转移 -- 因为赋值时所有权转移的问题，所以不能用于标准容器
        std::auto_ptr<CMyTestData> pMyData(new CMyTestData(1));
        CPPUNIT_ASSERT(pMyData->GetCurrentIndex() == 1);

        std::auto_ptr<CMyTestData> pMyDataAssign = pMyData;
        CPPUNIT_ASSERT(NULL == pMyData.get()); //所有权转移后，原来的智能指针变为NULL
    }

    {
        //测试reset方法
        LONG lCount = CMyTestData::GetTestDataCount();
        {
            std::auto_ptr<CMyTestData> pMyTestData(new CMyTestData(1));
            CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() == lCount + 1);
            CPPUNIT_ASSERT(pMyTestData.get()->GetCurrentIndex() == 1);

            //使用reset方法后，以前的会释放掉，产生新的
            pMyTestData.reset(new CMyTestData(2));
            CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() == lCount + 1);
            CPPUNIT_ASSERT(pMyTestData.get()->GetCurrentIndex() == 2);

            //使用reset方法，同时不带参数的话，会释放
            pMyTestData.reset();

            CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() == lCount);
            CPPUNIT_ASSERT(pMyTestData.get() == NULL);

        }
        CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() == lCount);
    }

    {
        //测试手工使用 release 释放 -- 不好的使用方式
        LONG lCount = CMyTestData::GetTestDataCount();

        std::auto_ptr<CMyTestData> pMyTestData(new CMyTestData(1));
        CPPUNIT_ASSERT(NULL != pMyTestData.get());
        CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() == lCount + 1);
        CPPUNIT_ASSERT(pMyTestData.get()->GetCurrentIndex() == 1);

        //由于release函数返回以前的指针，因此必须手工释放
        CMyTestData* pTestData = pMyTestData.release();
        CPPUNIT_ASSERT(NULL == pMyTestData.get());
        CPPUNIT_ASSERT(NULL != pTestData);
        CPPUNIT_ASSERT(pTestData->GetCurrentIndex() == 1);
        delete pTestData;  //必须手工释放

        CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() == lCount);
    }
}

void CSTLMemoryTester::test_contain_assign()
{
#ifdef NEED_OPERATOR_FUNC
    typedef std::vector<CMyTestData> DataContainer;
    DataContainer srcDataContainer;
    
    //除了一开始的时候是构造函数，其他时候都是调用拷贝构造
    DECLARE_MYTEST_DATA_COUNT_CHECKER(ConstructCheck,ctConstructCount, 5, __FILE__, __LINE__);
    {
        for (long i = 0; i < 5; i++)
        {
            srcDataContainer.push_back(CMyTestData(i, 0));
        }

        {
            DECLARE_MYTEST_DATA_COUNT_CHECKER(ConstructCheck,ctConstructCount, 0, __FILE__, __LINE__);
            //赋值时会对容器中的每一个元素调用拷贝构造函数 -- 
            DECLARE_MYTEST_DATA_COUNT_CHECKER(CopyConstructCheck,ctCopyConstructCount, 5, __FILE__, __LINE__);
            //超出dstDataContainer的生存周期后释放
            DECLARE_MYTEST_DATA_COUNT_CHECKER(DestructCheck,ctDestructCount, 5, __FILE__, __LINE__);
            DataContainer dstDataContainer;
            dstDataContainer = srcDataContainer;
        }

        {
            DECLARE_MYTEST_DATA_COUNT_CHECKER(ConstructCheck,ctConstructCount, 0, __FILE__, __LINE__);
            //assign时会对容器中的每一个元素调用拷贝构造函数 -- 
            DECLARE_MYTEST_DATA_COUNT_CHECKER(CopyConstructCheck,ctCopyConstructCount, 5, __FILE__, __LINE__);
            //超出dstDataContainer的生存周期后释放
            DECLARE_MYTEST_DATA_COUNT_CHECKER(DestructCheck,ctDestructCount, 5, __FILE__, __LINE__);
            DataContainer dstDataContainer;
            dstDataContainer.assign(srcDataContainer.begin(), srcDataContainer.end());
        }

        {
            DECLARE_MYTEST_DATA_COUNT_CHECKER(ConstructCheck,ctConstructCount, 0, __FILE__, __LINE__);
            DECLARE_MYTEST_DATA_COUNT_CHECKER(CopyConstructCheck,ctCopyConstructCount, 5, __FILE__, __LINE__);
            DECLARE_MYTEST_DATA_COUNT_CHECKER(DestructCheck,ctDestructCount, 5, __FILE__, __LINE__);

            DataContainer dstDataContainer;
            dstDataContainer.reserve(srcDataContainer.size());
            std::copy(srcDataContainer.begin(), srcDataContainer.end(), 
                std::back_insert_iterator<DataContainer>(dstDataContainer));
        }
    }
#endif
}

void CSTLMemoryTester::test_shared_ptr()
{
    typedef shared_ptr<CMyTestData>           CMyTestDataSharePtr;
    typedef shared_ptr<CMyTestDataChild>      CMyTestDataChildSharePtr;

    {
	    //可以将 shared_ptr 的指针放入容器，并且在容器超出范围或 clear 后会自动释放
	    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
	    DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 4, __FILE__, __LINE__);
	    {
		    typedef std::vector<CMyTestDataSharePtr>		CMyTestDataSharePtrContainer;
		    CMyTestDataSharePtrContainer myTestDataSharedPtrs;
		    for (int i = 0; i < 4; i++)
		    {
			    myTestDataSharedPtrs.push_back(CMyTestDataSharePtr(new CMyTestData(i)));
		    }
	    }//此处以后自动释放
    }

    {
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 2, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataDestructCountChecker,ctDestructCount, 2, __FILE__, __LINE__);
        CMyTestDataSharePtr     ptr1(new CMyTestData(1));
        CMyTestDataSharePtr     ptr2(new CMyTestData(2));
        {
            DECLARE_MYTEST_DATA_COUNT_CHECKER(dataDestructCountChecker,ctDestructCount, 1, __FILE__, __LINE__);
            ptr1 = ptr2;        //一个只能指针赋值给已有的智能指针，自动释放
        }
    }

    {
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 2, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataDestructCountChecker,ctDestructCount, 2, __FILE__, __LINE__);

        //智能指针保存 父子 类型 -- 并相互转换：
        CMyTestDataChildSharePtr    pChild1(new CMyTestDataChild(1));
        CMyTestDataSharePtr         pParent1(new CMyTestData(10));


        //pChild1 = pParent1;       //直接这样写，无法编译通过
        CMyTestDataChildSharePtr pParentToChildNULL =  dynamic_pointer_cast<CMyTestDataChild>(pParent1);
        CPPUNIT_ASSERT(pParentToChildNULL == NULL);  //由于 pParent1 实际指向父类型的变量，因此无法动态转换到子类型

        pParent1 = pChild1;         //将父类型的指针指向子类型的变量 -- 可以直接赋值
        CMyTestDataChildSharePtr pChildNoNull =  dynamic_pointer_cast<CMyTestDataChild>(pParent1);
        CPPUNIT_ASSERT(pChildNoNull != NULL);

#if 0
        //错误的用法 -- 会造成引用计数的混乱
        CMyTestDataSharePtr         pParent2;
        pParent2.reset((CMyTestDataChild*)pChild1.get());
#endif
    }

    {
        //测试通过 shared_from_this() + dynamic_pointer_cast<> 将父类指针特例化为 子类指针并调用子类的 特定方法

        CPPUNIT_ASSERT(CEnableSharedFromThis::s_Count == 0);
        {
            shared_ptr<CEnableSharedFromThis>   pEnableSharedFromThis(new CEnableSharedFromThisChild());
            CPPUNIT_ASSERT(pEnableSharedFromThis->GetIdentify() == 2);

            //转换成子类
            shared_ptr<CEnableSharedFromThisChild> pEnableShaedChild(
                dynamic_pointer_cast<CEnableSharedFromThisChild>(pEnableSharedFromThis->getSharedThis()));
            
            CPPUNIT_ASSERT(CEnableSharedFromThis::s_Count == 1);
            CPPUNIT_ASSERT(pEnableShaedChild->GetIdentify() == 2);
            CPPUNIT_ASSERT(pEnableShaedChild->CallChildMethod() == 3);  //调用子类特有的方法
        }
        CPPUNIT_ASSERT(CEnableSharedFromThis::s_Count == 0);
    }
}