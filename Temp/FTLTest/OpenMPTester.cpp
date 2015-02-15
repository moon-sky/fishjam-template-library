#include "StdAfx.h"
#include "OpenMPTester.h"
#include <ftlOpenMP.h>
#include <ftlSystem.h>
#include <omp.h>

COpenMPTester::COpenMPTester()
{
    m_nOpenMPMaxThreadCount = omp_get_num_procs();

    m_nProcessorsCount = CFOSInfo::Instance()->GetNumberOfProcessors();
    FTLTRACE(TEXT("m_nOpenMPMaxThreadCount=%d, m_nProcessorsCount=%d\n"), m_nOpenMPMaxThreadCount, m_nProcessorsCount);

    //CPPUNIT_ASSERT(m_nOpenMPMaxThreadCount == m_nProcessorsCount);
}

COpenMPTester::~COpenMPTester()
{

}

void COpenMPTester::test_parallel()
{
    LONG nCount = 0;

    //分成N个线程并行执行，每个线程执行相同的逻辑
    #pragma omp parallel
    {
        InterlockedIncrement(&nCount);
    }

    CPPUNIT_ASSERT(nCount == m_nOpenMPMaxThreadCount);
}

void COpenMPTester::test_parallel_for()
{

    const int COUNT = 100;
    LONG nCalcCount = 0;
    LONG nThreadCount = 0;

    int x[COUNT] = { 0 };
    int y[COUNT] = { 0 };
    int sum[COUNT] = { 0 };
    for (int i = 0; i < COUNT; i++)
    {
        x[i] = i;
        y[i] = COUNT - i;
    }

    //注意: #pragma omp parallel + #pragma omp for 的写法可以简写为 #pragma omp parallel for，其后必须紧接 for 循环
    #pragma omp parallel
    {
        #pragma omp for     //通过 for 将工作分配到N个并行的线程中去
        for(int i = 0; i < COUNT; ++i)
        {
            //注意：当并行执行循环的时候必须确保没有循环依赖，如 x[i] = x[i-1] + y[i]; 就不行
            sum[i] = x[i] + y[i];
            InterlockedIncrement(&nCalcCount);
        }
        InterlockedIncrement(&nThreadCount);
    }

    //会在并行区域的结束处同步 -- 所有的线程将阻塞在并行区域结束处，直到所有线程都完成
    for (int i = 0; i < COUNT; i++)
    {
        CPPUNIT_ASSERT(COUNT == sum[i]);
    }
    CPPUNIT_ASSERT(COUNT == nCalcCount);    //并行的个数是 for 循环的个数
    CPPUNIT_ASSERT(m_nOpenMPMaxThreadCount == nThreadCount);
}

void COpenMPTester::test_parallel_cancel()
{
    const int COUNT = 10;
    LONG nCalcCount = 0;
    LONG nThreadCount = 0;
    int sum = 0;

    //因为 for 循环中不能使用 break 提前结束，所以一个可选的结束方式是通过 continue 进行空运算，而快速结束
    //  TODO: 有没有其他方法能快速结束 ? 
    #pragma omp parallel
    {
        #pragma omp for
        for(int i = 0; i < COUNT; ++i)
        {
            if (i > (COUNT/2))
            {
                continue;
                //break;  //error C3010: 'break' : jump out of OpenMP structured block not allowed

            }
            sum += i;
            InterlockedIncrement(&nCalcCount);
        }
        InterlockedIncrement(&nThreadCount);
    }

    CPPUNIT_ASSERT(15 == sum); //COUNT 为10时: 0 + 1 + 2 + 3 + 4 + 5
    CPPUNIT_ASSERT(nCalcCount == (COUNT /2 + 1)); 
    CPPUNIT_ASSERT(nThreadCount == m_nOpenMPMaxThreadCount);
}

void COpenMPTester::test_variable()
{
    //共享数据与私有数据

    const int COUNT = 5;
    LONG calcCount = 0;  //变量 calcCount 没有制定子句，默认是共享的
    int sum = 10;       //变量 sum 使用reduction 指定成私有变量， 在#pragma omp for代码块完成后，线程为所有的私有sum和原值做+操作(sum的原值在本例中是10),
                        //再把结果赋值给原本的共享的sum变量。
    int j = 5;          //变量j使用了 firstprivate 表示是私有的，且各线程中的变量要继承主线程中的初值(此处为5)
    int i;              //变量i作为循环变量，是私有的, 使用了 lastprivate 表示在并行处理结束后复制回主线程中的对应变量

    #pragma omp parallel
    {
        #pragma omp for firstprivate(j) lastprivate(i) reduction(+: sum)
        for(i = 0; i < COUNT; ++i)
        {
            int doubleI = 2 * i;
            for(; j < doubleI; ++j)  //j 通过 firstprivate 继承了初始值(5)，因此不需要初始化
            {
                sum += i * j;
                InterlockedIncrement(&calcCount);
            }
        }
    }

    CPPUNIT_ASSERT(sum == 97); //sum = 10(初始值) + 3*5(i为3时) + (4*5 + 4*6 + 4*7)(i为4时) = 10 + 15 + 20 + 24 + 28 = 97
    CPPUNIT_ASSERT(calcCount == 4);  // i为3的时候一次，i为4时3次
    CPPUNIT_ASSERT(COUNT == i);
}