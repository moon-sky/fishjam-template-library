#ifndef FTL_OPENMP_H
#define FTL_OPENMP_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlOpenMP.h requires ftlbase.h to be included first
#endif


/*************************************************************************************************************************
* TODO:
*   1.OpenMP启动的线程如何提前取消? 似乎不能 break ?
*     会报如下错误: error C3010: 'break' : jump out of OpenMP structured block not allowed
*   2.默认时 并行区域的所有变量都是共享的，除非:
*     a.在并行for循环中，循环变量是私有的;
*     b.并行区域代码块里的本地变量是私有的;
*     c.通过 private, firstprivate, lastprivate, reduction 子句声明的变量为私有变量
*      
*
* VC中启用 OpenMP
*   C/C++ -> Language -> OpenMP Support, 会增加 /openmp 编译链接项，会增加 _OPENMP 宏,并导入 vcomp.lib
* 
* OpenMP 介绍  -- 自动适应多核CPU的多线程编程方式
*   一种面向共享内存以及分布式共享内存的多处理器多线程并行编程语言
*   一种能够被用于显示指导多线程、共享内存并行的应用程序编程接口（API）
*   具有良好的可移植性，支持多种编程语言 ( VS2005 完全支持 OpenMP 2.0标准，C/C++->Language->OpenMP Support，其他语言 ？)
*   能够支持多种平台，包括大多数的类UNIX系统以及Windows NT系统（Windows 2000，Windows XP，Windows Vista等）
*   以线程为编程模型的基础，通过 编译指导语句(#pragma omp) 来显示地指导并行化，为编程人员提供了对并行化的完整的控制。
*   采用 Fork-Join 的形式 -- 并行区域 -> 等待 -> 串行区域
*   标准并行模式执行代码的基本思想是，程序开始时只有一个主线程，程序中的串行部分都由主线程执行，
*     并行的部分是通过派生其他线程来执行，但是如果并行部分没有结束时是不会执行串行部分的
*   将串行的程序 逐步地 改造成一个并行程序，达到 增量 更新程序的目的，减少程序编写人员一定的负担。
*   但for循环必须满足并行执行的条件：
*     1.★ 每次循环互不相干，后一次循环不依赖于前面的循环 ★; 如 x[i] = x[i-1] + y[i]; 就不行
*     2.for 循环语句必须是最简单的循环语句：(初始化; 大[等]于/小[等]于某个数; 增/减变化)
*
* OpenMP 线程数：
*   1. 函数 设置的值;
*   2. 环境变量 OMP_NUM_THREADS 的值;
*   3. 检测出的 CPU 个数 (omp_get_num_procs)
* 
* OpenMP 两个基本构成部分
*   运行时库函数
*   编译器指令 -- #pragma omp xxx , 编译器不支持时OpenMP指令将被忽略
* 
* 
* OpenMP 的运行时库函数     #include <omp.h>
*   OpenMP运行时函数库原本用以设置和获取执行环境相关的信息，它们当中也包含一系列用以同步的API。
*   支持运行时对并行环境的改变和优化，给编程人员足够的灵活性来控制运行时的程序运行状况。
*     omp_get_num_procs  -- 返回运行本线程的多处理机的处理器个数
*     omp_get_thread_num -- 获取当前线程的 线程标识号（OpenMP中使用，从 0 开始）
*     omp_set_num_threads-- 设置并行执行代码时的线程个数
*     omp_get_num_threads-- 返回当前并行区域中的活动线程个数
*     //互斥锁
*     omp_init_lock      -- 初始化一个互斥锁
*     omp_set_lock       -- 获得一个互斥锁
*     omp_unset_lock     -- 释放一个互斥锁
*     omp_test_lock      -- 试图获得一个互斥锁，并在成功是返回真（true），失败是返回假（false）
*     omp_destroy_lock   -- 结束一个互斥锁的使用并释放内存
*
*
* OpenMP支持两种不同类型的线程同步机制(默认情况下自动互斥？)：
*   互斥锁 -- 三种互斥锁机制对一块内存进行保护，分别是临界区（critical），原子操作（atomic）以及由库函数来提供同步操作。
*     编译指导语句进行的互斥锁支持只能放置在一段代码之前，作用在这段代码之上。
*     a.临界区（critical） -- 在程序需要访问可能产生竞争的内存数据的时候，都需要插入相应的临界区代码。
*       #pragma omp critical [(name)]
*     b.原子操作（atomic） -- OpenMP编程方式给同步编程带来的特殊的编程功能。
*       通过编译指导语句的方式直接获取了现在多处理器计算机体系结构的功能。只能作用在语言内建的基本数据结构。
*       #pragma omp atomic --
*     c.库函数 -- 程序员必须自己保证在调用相应锁操作之后释放相应的锁，否则就会造成多线程程序的死锁。
*       
* 
*   事件通知机制
*  
* OpenMP 的 Directives （指令）
*   #pragma omp <directive> [clause[ [,] clause]…] 语句等
*   其中 directive 部分就包含了具体的编译指导语句，包括 
*     parallel{代码}  -- 开始并行执行，线程个数由 OPENMP 线程数 确定, 每个线程都执行一样的指令(如果后面加 for ，则是 循环并行化 )
*     parallel for for(...){循环代码} -- for循环的代码将被多个线程并行执行(工作共享，每个线程执行执行一部分for循环的内容)
*     sections/section -- 工作分区编码(每个线程执行一个分区，上限为 OPENMP 线程数)
*     single -- 只有一个线程能执行指定代码
*     master -- 只有主线程才能执行指定代码（如主线程进行 读/写，多个线程进行 计算时）
*     critical [(name)]{ 代码 } -- 临界区，线程间对代码的互斥访问
*     flush [(var)] -- 更新共享变量的值，使共享变量的值在各线程中保持一致 
*     ordered -- 指定for循环的执行要按顺序执行
*     atomic -- 用于指定一块内存区域被自动更新(多个线程同时写一个全局变量时)
*     barrier -- 用于并行区内代码的线程同步，所有线程执行到barrier时停止，直到所有线程都执行到barrier，才继续往下执行。
*     threadprivate(变量名) -- 线程私有数据,在线程组的线程都拥有一份私有变量的拷贝，某线程中的改变对其他线程是不可见的
* 
* OpenMP 的Clauses(子句), 注意: 五个指令(master，critical，flush，ordered 和 atomic)不能使用子句
*    private, 指定每个线程都有它自己的变量私有副本，其初始值为该类型的默认值(即使用默认构造设置初始值)。
*    firstprivate，指定每个线程都有它自己的变量私有副本，并且变量要被继承主线程中的初值(即使用拷贝构造设置初始值)。
*    lastprivate，主要是用来指定将线程中的私有变量的值在并行处理结束后复制回主线程中的对应变量(使用拷贝赋值操作符)
*      (TODO: 多个线程，哪个值被复制回去 -- 最后一次迭代的值?)
*    reduce，用来指令一个或多个变量是私有的，初始值为类型的默认值，并且在并行处理结束后这些变量要执行指定的运算，
*      在代码块的结束处，为变量的私有拷贝和变量原值一起应用reduction操作符。
*      其变量必须为标量变量，可用运算符为 +,*,-,&,|,&& 等
*    nowait，忽略指令中暗含的等待
*    num_threads，指定线程的个数
*    schedule，指定如何调度for循环迭代
*    shared，指定一个或多个变量为多个线程间的共享变量, 
*    ordered，用来指定for循环的执行要按顺序执行
*    copyprivate，用于single指令中的指定变量为多个线程的共享变量
*    copyin，用来指定一个threadprivate的变量的值要用主线程的值进行初始化。
*    default，用来指定并行处理区域内的变量的使用方式，缺省是shared
*
*
* 循环并行化(一段代码能够在多个线程内部同时执行) -- 限制：必须使用标准的 for 循环；循环语句块应该是单出口与单入口的
*  循环并行化编译指导语句可以加在任意一个循环之前，则对应的最近的循环语句被并行化，其它部分保持不变 -- 如要嵌套，需要写多个？
*  #pragma omp parallel for [clause[clause…]]   //生成 CPU个数 个线程，循环 for规定 的次数
*     for(index = first ; test_expression ; increment_expr)
*     {
*        body of the loop;
*     }
*
* 线程私有数据
*   使用 threadprivate 子句用来标明某一个变量是线程私有数据，在程序运行的过程中，不能够被其他线程访问到。
*   每个线程都有一份拷贝，每个线程都可以更改，但全局时以 thread_num 为 0(主线程) 的线程值为准
*   使用copyin子句对线程私有的全局变量进行初始化
*    int g_counter=0;                       //定义全局变量
*    #pragma omp threadprivate(g_counter)   //将该变量设为私有
*    #pragma omp parallel copyin(g_counter){ 初始化代码 } //初始化为每个线程独立的变量值，之后每个线程有各自的值
*
* 工作队列 -- 为维持一个工作的队列，线程在并行执行的时候，不断从这个队列中取出相应的工作完成，直到队列为空为止。
*     1.使用线程标识号分配任务 -- 由于每一个线程在执行的过程中的线程标识号 (omp_get_thread_num) 是不同的，可以根据这个线程标识号来分配不同的任务。
*     2.使用循环语句分配任务
*
* 工作分区编码
*     #pragma omp parallel sections
*     {
*       #pragma omp section
*           printf("section 1 thread=%d\n",omp_get_thread_num());
*
*     }
*************************************************************************************************************************/

namespace FTL
{
}
#endif //FTL_OPENMP_H

#ifndef USE_EXPORT
#  include "ftlOpenMP.hpp"
#endif 