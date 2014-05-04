#pragma once
#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 分配器最初是为抽象内存模型而开发的，允许库开发者忽略在某些16位操作系统上near和far指针的区别 -- 但努力失败了
* 因此被设计成促进全功能内存管理器的发展，负责分配（和回收）原始内存
* 标准明确地允许库实现假设每个分配器的 pointer typedef 是 T* 的同义词，每个分配器的 reference typedef 与 T& 相同
* 分配器不能有状态 -- 即不能有任何非静态数据成员
* 相同类型的所有分配器都一定等价
* 
* 如果想要写自定义分配器，必须按如下做法进行(大部分事情是重现大量样板代码，然后修补一些成员函数,特别是allocate和deallocate)：
*   1.把分配器做成一个模板，带有模板参数T，代表要分配内存的对象类型。 
*   2.提供pointer和reference的typedef，但是总是让pointer是T*，reference是T&。 
*   3.决不要给分配器对象状态。通常，分配器不能有非静态的数据成员。 
*   4.应该传给分配器的allocate成员函数需要分配的对象个数而不是字节数。也应该记得这些函数返回T*指针（通过pointer typedef），
*     即使还没有T对象被构造。 
*   5.一定要提供标准容器依赖的内嵌rebind模板。
*
************************************************************************/

#if 0
    template<typename T>
    class SharedMemoryANocator 
    {
    public:
        typedef  T* pointer;
        typedef T& reference;
        pointer allocate(size_type numObiects, const void *localityHint = 0)
        {
            return static_cast<pointer>(mallocShared(numObiects * sizeof(T)));
        }

        void deallocate(pointer ptrToMemory, size_type numObjects)
        {
            freeShared(ptrToMiemory);
        } 
    };
    //使用自定义的 Allocator
    typedef vector<double, SharedMemoryAllocator<double> >  SharedDoubleVec;

#endif 

class CSTLAllocatorTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLAllocatorTester );
    //CPPUNIT_TEST( testAppend );
    CPPUNIT_TEST_SUITE_END();

    //void testAppend();

    DECLARE_DEFAULT_TEST_CLASS(CSTLAllocatorTester);
};
