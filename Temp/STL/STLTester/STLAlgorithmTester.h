#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <string>

/**********************************************************************************************************
* 算法头文件：由一大堆模版函数组成，其中常用到的功能范围涉及到比较、交换、查找、遍历操作、复制、修改、
*   移除、反转、排序、合并等等。
* 
* 稳定排序：如果一个区间中的两个元素有等价的值，它们的相对位置在排序后不改变。
* 排序有多种方法 -- 需要随机访问迭代器，对标准关联容器排序元素没有意义
*   性能(由高到低)：partition > stable_partition > nth_element > partial_sort > sort > stable_sort
*   nth_element -- 找出满足需要的[第]N个元素（不按顺序排列）-- 可以找到指定百分点的元素
*      //找到质量值为75%的Widget,begin + goalOffset现在指向质量等级为75%的Widget
*      vector<Widget>::size_type goalOffset = 0.25 * widgets.size();
*      nth_element(begin, begin + goalOffset,end, qualityCompare); 
*   sort -- 将元素进行完全排序
*   stable_sort -- sort 的稳定排序版本
*   partition -- 重排区间中的元素以使所有满足某个标准的元素都在区间的开头
*   stable_partition -- partition 的稳定排序版本
*   partial_sort -- 不稳定的部分排序（如找出最大的10个元素并按顺序排列）
*   
* 对不支持的容器间接使用排序算法的方法：
*   1.把元素拷贝到一个支持随机访问迭代器的容器中再使用算法；
*   2.建立一个迭代器的容器，对那个容器使用算法，然后通过迭代器访问元素；
*   3.使用有序的迭代器容器的信息来迭代地把元素接合到需要的位置。
*   
* 只能操作有序数据（需要先用 sort 排序）的算法：
*   binary_search，lower_bound，upper_bound，equal_range -- 需要使用二分法查找来搜索值
*   set_union，set_intersection，set_difference，set_symmetric_difference -- 在有序数据上线性时间设置指定操作
*   merge，inplace_merge -- 执行了有效的单遍合并排序算法(读取两个有序区间，然后产生一个包含了两个源区间所有元素的新有序区间)
*   includes -- 检测一个区间的所有对象是否也在另一个区间中
*  通常需要有序数据（会更快，但不强求）的算法：
*   unique，unique_copy -- 从每个相等元素的连续组中去除第一个以外所有的元素
*  注意：对数据进行 排序的算法的比较函数行为 应该和 要使用的算法的比较函数行为 一致（如必须都是升序） -- 否则结果未定义
* 在未排序的容器中线性时间的算法：count、count_if、find和find_if
*   注意：count/find 等无序区间算法使用“相等”来判断，而 binary_search/lower_bound 等有序区间算法用“等价”来判断 -- 具体需要看文档
* 
* copy 算法 -- 标准STL中带copy名字的算法有11个，但没有 copy_if -- ftl 中实现了一个
*   copy，copy_backward，replace_copy reverse_copy，replace_copy_if unique_copy，remove_copy rotate_copy，
*   remove_copy_if，partial_sort_copy，uninitialized_copy 
*                                      
* 注意：<algorithm> 中的很多算法函数都不需要std名字空间？
*
* 使用 replace_if 进行字符替换
*   replace_if(str.begin(), str.end(), bind2nd(equal_to<char>(),'.',',');  <== 把 '.'(点)替换成 ','(逗号)
*
* 泛型堆算法(C++ 标准库提供的是 max-heap)
*   make_heap -- 把范围内的元素生成一个堆
*   pop_heap -- 并不是真正的把最大元素从堆中弹出，而是重新排序堆。它把 first 和 last-1 交换，然后重新做成一个堆，
*     可以使用容器的 back 来访问被“弹出“的元素或者使用 pop_back 来真正的删除。
*   push_heap -- 假设 first 到 last-1 是一个有效的堆，要被加入堆的元素在位置 last-1 ，重新生成堆。
*     在指向该函数前，必须先把元素插入容器后。
*   sort_heap -- 对范围内的序列重新排序，它假设该序列是个有序的堆
*   
**********************************************************************************************************/

class CSTLAlgorithmTester : public CPPUNIT_NS::TestFixture
{
public:
    CSTLAlgorithmTester();
    ~CSTLAlgorithmTester();

    virtual void setUp();
    virtual void tearDown();


    CPPUNIT_TEST_SUITE( CSTLAlgorithmTester );
    CPPUNIT_TEST( test_adjacent_find );
    CPPUNIT_TEST( test_adjacent_difference );
	CPPUNIT_TEST( test_binary_search );

    CPPUNIT_TEST( test_copy_if );
    CPPUNIT_TEST( test_count );
    CPPUNIT_TEST( test_equal );
    CPPUNIT_TEST( test_equal_range );
    CPPUNIT_TEST( test_fill );
    CPPUNIT_TEST( test_find );
    CPPUNIT_TEST( test_find_if );
    CPPUNIT_TEST( test_find_end );
    CPPUNIT_TEST( test_find_first_of );
    CPPUNIT_TEST( test_for_each );
    CPPUNIT_TEST( test_generate );
    CPPUNIT_TEST( test_includes );
    CPPUNIT_TEST( test_inner_product );
    CPPUNIT_TEST( test_inner_merge );
    CPPUNIT_TEST( test_iter_swap );
    CPPUNIT_TEST( test_lexicographical_compare );
    CPPUNIT_TEST( test_nth_element );
    CPPUNIT_TEST( test_permutation );
    CPPUNIT_TEST( test_lower_bound );
    CPPUNIT_TEST( test_min_max_element );
    CPPUNIT_TEST( test_mismatch );
    CPPUNIT_TEST( test_search );
    CPPUNIT_TEST( test_set_difference );
    CPPUNIT_TEST( test_set_intersection );
    CPPUNIT_TEST( test_random_shuffle );
    CPPUNIT_TEST( test_remove );
    CPPUNIT_TEST( test_remove_copy );
    CPPUNIT_TEST( test_remove_copy_if );
    CPPUNIT_TEST( test_replace_if );
    CPPUNIT_TEST( test_reverse );
    CPPUNIT_TEST( test_rotate );
    CPPUNIT_TEST( test_partition );
    CPPUNIT_TEST( test_partial_sort );
    CPPUNIT_TEST( test_partial_sum );
    CPPUNIT_TEST( test_set_union );
    CPPUNIT_TEST( test_stable_sort );
    CPPUNIT_TEST( test_stable_partition );
    CPPUNIT_TEST( test_swap );
    CPPUNIT_TEST( test_swap_range );
    CPPUNIT_TEST( test_transform );
    CPPUNIT_TEST( test_unique );
    CPPUNIT_TEST( test_unique_copy );
    CPPUNIT_TEST( test_upper_bound );
    CPPUNIT_TEST( test_merge );

    CPPUNIT_TEST( test_heap );
    CPPUNIT_TEST_SUITE_END();

private:
    //!搜索相等或满足特定条件的临近元素
    void test_adjacent_find();  

    //序列中的后一个减去与他相邻的前一个得到新的序列
    //创建一个新序列，该序列的每个新值都代表了当前元素与上一个元素的差
    void test_adjacent_difference();
              
    //使用二分法在一个已经★排好序★的Range中查找满足指定条件的值
    void test_binary_search();  //if (binary_search(vw.begin(), vw.end(), w))

    //复制序列, 对应的有 copy, copy_backward(反序拷贝), copy_if 
    //使用 copy_XXX 时，必须确保目标容器有足够大的空间，或者容器本身是自动扩展的，通常用 back_inserter 等进行自动扩展
    void test_copy_if();

    //@ count()和count_it 用来统计 等于某个值 或 满足特定条件 的对象的个数，其中 count_if 需要使用 函数指针或函数对象
    void test_count();

    //如果两个序列在范围内的元素都相等，则 equal 返回 true 
    void test_equal();

    //返回一对迭代器，第一个等于lower_bound返回的迭代器，第二个等于upper_bound返回值 -- 即划分出了和要搜索的值等价的区间
    //如果这两个迭代器相同，就意味着对象的区间是空的；这个值没有找到
    //pair<vector<Widget>::iterator,vector<Widget>::iterator> range = equal_range(vw.begin(),vw.end(), w);
    //if (range.first != range.second)  //找到了,range.first指向第一个而range.second指向最后一个的下一个
    //对first 和 second 作 distance 就等于区间中对象的数
    void test_equal_range();  

    //将输入的值的拷贝赋给范围内的每个元素, 对应的有 fill_n 
    void test_fill();

    //find,find_if -- 在容器中查找指定元素,如果没有找到指定的对象，就会返回 end() 的值，要是找到了就返回一个指着找到的对象的 iterator
    //注意：每次最多只能查找一个，如果要查找全部的，必须更改起始位置，循环查找
    void test_find();
    void test_find_if();

    //在范围内查找“由输入的另外一个 iterator 对标志的第二个序列”的最后一次出现
    void test_find_end();

    //在范围内查找“由输入的另外一个 iterator 对标志的第二个序列”中的任意一个元素的第一次出现。
    void test_find_first_of();

    /**
    *   for_each 遍历一个iterator的范围，该函数不会修改序列中的任何元素 -- 不需要初始化、比较和给iterator增量
    *   起始iterator指出操作由哪里开始，末尾iterator指明到哪结束，但是它不包括在这个范围内
    *   注意：如果for_each要传入成员函数，必须使用 bind1st和mem_fun(参见test_bind1st_this)
    *     对应的仿函数类型为 void operator ( ) ( Type& elem ) const
    **/
    void test_for_each();

    //使用特定的算法生成数据,对应的还有 generate_n
    void test_generate();

    //判断 [first1, last1) 的一个元素是否被包含在另外一个序列中。使用底层元素的 <= 操作符
    void test_includes();

    //对两个序列做内积 ( 对应的元素相乘，再求和 ) ，并将内积加到一个输入的的初始值上
    void test_inner_product();

    //合并两个排过序的连续序列，结果序列覆盖了两端范围
    void test_inner_merge();

    //交换两个 ForwardIterator 的值
    void test_iter_swap();

    //交换存储在两个对象中的值
    void test_swap();

    //将在范围内的元素与另外一个序列的元素值进行交换
    void test_swap_range();

    //比较两个序列
    void test_lexicographical_compare();

    //检查“它在吗？如果是，第一个拷贝在哪里？如果不是，它将在哪里？” -- 同时检测是否等于end 和 是否是想要的值(必须使用“等价”)
    //返回一个 iterator ，它指向在范围内的有序序列中可以插入指定值而不破坏容器顺序的第一个位置
    void test_lower_bound();  //vector<Widget>::iterator i = lower_bound(vw.begin(), vw.end(), w);if (i != vw.end() && !(w < *i))

    //返回一个 iterator ，它指向在范围内的有序序列中插入 value 而不破坏容器顺序的最后一个位置，该位置标志了一个大于 value 的值。
    void test_upper_bound();

    //合并两个有序序列，并存放到另外一个序列中
    void test_merge();

    //获取区间中的最小和最大值,
    //对应的还有 max_element(返回一个 iterator ，指出序列中最大的元素), min_element(返回最小的元素)
    void test_min_max_element();

    //并行的比较两个序列，指出第一个不匹配的位置，它返回一对 iterator ，标志第一个不匹配的元素位置。如果都匹配，返回每个容器的 last 
    void test_mismatch();

    // search -- 在一个序列中找另一个序列的第一次出现的位置(如字符串搜索)，使用operator == 
    //   检查第二个序列是否在第一个序列中出现，且顺序相同
    //   注意：★如果容器中是对象(CMyTestData)，则调用其operator==，但如果是对象指针（CMyTestData*），则使用地址进行比较★
    //   类似的还有 search_n -- 查找出现 n 次的子序列
    void test_search();

    
    //构造一个排过序的序列，其中的元素出现在第一个序列中，但是不包含在第二个序列中。
    void test_set_difference();

    //构造一个排过序的序列，其中的元素在第一个序列中出现，但是不出现在第二个序列中
    //  问题：如果是这样的话，那和 set_difference 有什么区别
    void test_set_symmetric_difference();

    //构造一个排过序的序列，其中的元素在两个序列中都存在
    void test_set_intersection();

    //构造一个排过序的序列，它包含两个序列中的所有的不重复元素
    void test_set_union();

    //next_permuttion -- 取出当前范围内的排列，并将其重新排序为下一个排列
    //prev_permutation -- 取出范围内的序列并将它重新排序为上一个序列。如果不存在上一个序列则返回 false
    void test_permutation();

    //将范围内的序列重新排序，使所有小于第 n 个元素的元素都出现在它前面，而大于它的都出现在后面
    void test_nth_element();

    //随机打乱原先的顺序 -- 需要randomiter，因此只适用于 list/set 等，只适用于 vector ?
    void test_random_shuffle();

    //   remove -- 通用算法remove()使用和容器的成员函数使用不同的方式工作 -- 不改变容器的大小，剩下的元素保持不变。
    //     返回一个指向新的list的结尾的iterator(区间的“新逻辑终点”)。
    //     从开始到这个新的结尾（不含新结尾元素）的范围 包含了remove后剩下所有元素 -- 注意：没有把删除后的元素移到最后
    //     可以用list成员函数erase函数来删除从新结尾到老结尾的部分。
    //   需要前向迭代器和可以通过这些迭代器赋值的能力
    //   见 Effective STL 规则 23, 注意必须调用 erase() 来真正删除
    //   类似的有 remove_if
    void test_remove();

    //将所有不匹配的元素都复制到一个指定容器，返回的 OutputIterator 指向被拷贝的末元素的下一个位置
    void test_remove_copy();

    //remove_copy_if 将谓次返回false的元素拷贝到 目的地中（即保留需要删除的）
    //利用 remove_copy_if 可以用于从容器(比如map)中删除指定的数据，从而避免使用 erase 时的注意
    //  但是由于copy操作会执行拷贝构造，因此性能可能受损失？
    void test_remove_copy_if();

    //检查一个范围内的每个值，如果满足指定的条件，就用特定的值进行替换 -- 原来的值怎么办？直接被替换？指针时？
    //对应的有 replace_copy, replace_copy_if   
    void test_replace_if();

    //对容器进行倒置, 对应的有 reverse_copy
    void test_reverse();

    //将范围内的元素移到容器末尾，由 middle 指向的元素成为容器第一个元素
    //类似地有 rotate_copy 
    void test_rotate();

    //以指定的规则对序列进行稳定性排序 -- 保留相等元素之间的顺序关系
    void test_stable_sort();

    //stable_partition 重新排列元素，使得满足指定条件的元素排在 不满足条件的元素前面。它维持着两组元素的顺序关系 -- 部分排序？
    //不保证保留容器中的相对顺序
    void test_stable_partition();

    //对范围内元素重新排序，使用输入的函数，把计算结果为 true 的元素都放在结果为 false 的元素之前
    void test_partition();

    //对整个序列做部分排序，被排序元素的个数正好可以被放到范围内,
    //对应的有 partial_sort_copy，经过排序的序列复制到另外一个容器
    void test_partial_sort();

    //创建一个新的元素序列，其中每个元素的值代表了范围内该位置之前所有元素之和
    void test_partial_sum();

    //以升序重新排列范围内的元素 -- 需要随机存取的iterator(vector/deque等)
    void test_sort();
    
    //对区间内的每个元素都施加某个函数，而结果将被写到另外一个地方
    //将输入的操作作用在范围内的每个元素上，并产生一个新的序列。重载版本将操作作用在一对元素上，
    //另外一个元素来自输入的另外一个序列，结果输出到指定的容器
    void test_transform();

    //清除序列中重复的元素（从一个区间删除东西（邻近的重复值）而不用访问持有区间元素的容器），之后必须调用成员函数 erase -- 同remove
    //从每个相等元素的连续组中去除第一个以外所有的元素 
    //删除相邻重复元素，最好先排序
    void test_unique();
    void test_unique_copy();

    //测试泛型堆算法相关的函数：make_heap、pop_heap、push_heap、sort_heap
    void test_heap();

    //DECLARE_DEFAULT_TEST_CLASS(CSTLAlgorithmTester);
private:
    typedef std::map<std::string, CMyTestData*>  String2MyTestDataMap;
    typedef std::pair<std::string, CMyTestData*> String2MyTestDataPair;

    String2MyTestDataMap    m_string2MyTestDataMap;

    typedef std::list<CMyTestData* >    MyTestDataList;
    MyTestDataList                      m_MyTestDataList;
};
