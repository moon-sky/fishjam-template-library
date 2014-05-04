#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* STL 中没有包含标准的散列(Hash)容器，但兼容STL的散列关联容器可以从多个来源获得
*   hash_set、hash_multiset、hash_map和hash_multimap -- 在 stdext 的名称空间中
* 注意：不同的实现有不同的接口、能力、数据结构和效率。
*
* 自定义 hash 函数(如 string )
* struct str_hash: public unary_function<string,bool> {
*    size_t operator()(const string& str) const
*    {return ...}  //MS预定义的 hash 函数见 <xhash>
*  }
************************************************************************/
class CSTLHashContainerTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLHashContainerTester );
    CPPUNIT_TEST( test_hash_map );
    CPPUNIT_TEST( test_hash_multimap );
    CPPUNIT_TEST( test_has_multiset );
    CPPUNIT_TEST( test_hash_set );
    CPPUNIT_TEST_SUITE_END();

    
    /***********************************************
     * @brief 
     *   hash_map基于hash table（哈希表）。把数据的存储和查找消耗的时间大大降低，几乎可以看成是常数时间；
     *   而代价仅仅是消耗比较多的内存 -- 空间换时间
     * 基本原理：使用一个下标范围比较大的数组来存储元素。可以设计一个函数（哈希函数，也叫做散列函数），
     *   使得每个元素的关键字都与一个函数值（即数组下标，hash值）相对应，于是用这个数组单元来存储这个元素 -- "直接定址"；
     * 不能够保证每个元素的关键字与函数值是一一对应的，不同的元素可能计算出相同的函数值 -- "解决冲突"；
     * 要实现哈希表, 和用户相关的是：hash函数 和 比较函数 -- 在使用hash_map时指定的参数
     ***********************************************/
    
    
    //
    void test_hash_map();

    void test_hash_multimap();

    void test_has_multiset();

    void test_hash_set();

    
    DECLARE_DEFAULT_TEST_CLASS(CSTLHashContainerTester);
};
