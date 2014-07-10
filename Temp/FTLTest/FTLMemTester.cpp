#include "StdAfx.h"
#include "FTLMemTester.h"
#include "ftlMem.h"

class CMyMemLeak : public FTL::CFMemCheckBase
{
public:
    CMyMemLeak()
    {
        m_SomeValue = 10;
    }
private:
    int m_SomeValue;
    
};

void CFTLMemTester::test_MemLeakBase()
{
    CMyMemLeak* pLeak = new CMyMemLeak();

    //此处还没有释放，因此会检测到内存泄漏
    //CHECK_OBJ_MEM_LEAK(CMyMemLeak);
    
    SAFE_DELETE(pLeak);
}