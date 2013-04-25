#ifndef FTL_MEM_H
#define FTL_MEM_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlMem.h requires ftlbase.h to be included first
#endif

#include "ftlBase.h"
#include "ftlThread.h"

/********************************************************************************************
* EM64T -- Intel 的延伸内存64技术
*
* 在C++中，内存分成5个区，他们分别是堆、栈、自由存储区、全局/静态存储区和常量存储区。
*   堆 -- 由new分配的内存块，需要delete，如未对应的释放，则是内存泄露；
*   自由存储区 -- 由malloc等分配的内存块，和堆十分相似，但用free来结束。
*   栈 -- 函数执行时，函数内局部变量的存储单元。函数执行结束时这些存储单元自动被释放。效率高，容量有限；
*      有静态分配(局部变量，编译器完成)和动态分配(alloca函数)两种方式
*   全局/静态存储区 -- 全局变量和静态变量被分配到同一块内存中（C语言中全局变量分为初始化的和未初始化的）；
*   常量存储区 -- 存放常量，不允许修改
*
* 重载new和delete
*   默认的全局操作符
*     void * operator new(size_t size);
*     void operator delete(void *p);
*   类特有的操作符重载(子类默认会使用该重载，除非它也进行了重载)
*     public:
*       void * operator new(size_t size);
*       void operator delete(void *p);
*       //默认情况下，请求对象数组时会被定向到全局的new[]和delete[]。如需要，也必须进行重载。
*       //注意：对于多数C++的实现，new[]操作符中的个数参数是数组的大小加上额外的存储对象数目的一些字节。
*       //      在内存分配机制重要考虑的这一点。应该尽量避免分配对象数组，从而使内存分配策略简单。
*       void * operator new[ ](size_t size);
*       void operator delete[](void *p);
* 
* 
* 内存泄露检查 -- CRT调试堆函数(只在 _DEBUG 宏定义的情况下有效)，按以下顺序进行声明和包含 宏和头文件
*   #define _CRTDBG_MAP_ALLOC     //会包含内存分配地方的文件路径和行号
*   #include <stdlib.h>
*   #include <crtdbg.h>
* 相关函数
*   _CrtDumpMemoryLeaks -- 输出目前为止尚未释放的内存信息
*   _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); -- 使得程序退出时，自动调用 _CrtDumpMemoryLeaks
*      一般在Main开始，#if defined(DEBUG) | defined(_DEBUG) ... #endif  但测试后觉得没什么用，系统好像会自动调用。
*   _CrtSetReportMode -- 重设输出模式(如目的地)
*   _CrtSetBreakAlloc -- 在指定的内存分配数值处设置断点，也可以在Watch窗口中输入 {,,msvcrtd.dll}_crtBreakAlloc , 回车后指定分配号
*   _CrtMemCheckpoint -- 对内存状态做快照，注意：有多个线程的时候
* CRT输出信息
*   {18}            normal block at 0x00780E80, 64 bytes long.
*   {内存分配数值}  模块的类型       内存地址       大小         
*   前十六字节的内容
* 内存模块类型
*   Free Blocks     -- 一种被释放的内存模块(不会见到？)
*   Normal Blocks   -- 由你的程序分配的普通内存(注意的重点)
*   CRT Blocks      -- 由CRT库提供自己使用而分配的内存模块,通常不需要注意(CRT崩溃时才会见到？)
*   Ignore Blocks   -- 特殊标记为忽略的内存块
*   Client Blocks   -- MFC 使用
********************************************************************************************/

namespace FTL
{
    //Class
    
    //注意：本宏只能在单线程的情况下进行检查，其他线程分配的内存也会被检测到，如果要检测，必须把其他的线程先暂停

#ifdef _DEBUG
# define CHECK_MEM_LEAK(bPauseOtherThread)\
    FTL::CFMemLeakChecker   memLeakChecker ## __LINE__ (TEXT(__FILE__),__LINE__, TEXT(__FUNCTION__), bPauseOtherThread)
#  define CHECK_NAME_MEM_LEAK(blockName, bPauseOtherThread)\
    FTL::CFMemLeakChecker   memLeakChecker ## __LINE__ (TEXT(__FILE__),__LINE__, blockName, bPauseOtherThread)
#else
#  define CHECK_MEM_LEAK(bPauseOtherThread)                     __noop
#  define CHECK_NAME_MEM_LEAK(blockName, bPauseOtherThread)     __noop
#endif 
    
    class CFMemLeakChecker
    {
    public:
        FTLINLINE CFMemLeakChecker(LPCTSTR pszFileName,DWORD line, LPCTSTR pBlockName, BOOL bPauseOtherThread);
        FTLINLINE ~CFMemLeakChecker();
    private:
        const TCHAR*    m_pszFileName;
        const TCHAR*    m_pszBlkName;
        const DWORD     m_Line;
        const BOOL      m_bPauseOtherThread;
        _CrtMemState    m_sOld;
        _CrtMemState    m_sNew;
        _CrtMemState    m_sDiff;
    };

    //用于检测程序在退出时，指定的类是否有内存泄漏的小工具类 -- 必须在认为应该释放完内存的地方(比如 main 返回的位置)进行调用检查宏
    FTLEXPORT class CFMemCheckBase
    {
    public:
        FTLINLINE CFMemCheckBase();
		FTLINLINE CFMemCheckBase(const CFMemCheckBase &other);
		FTLINLINE CFMemCheckBase& operator =( const CFMemCheckBase &ref );
		FTLINLINE virtual ~CFMemCheckBase();
    };

	class CFMemCheckManager
	{
	public:
		FTLINLINE static CFMemCheckManager& GetInstance();
		FTLINLINE static VOID ReleaseInstance();
	public:
		FTLINLINE BOOL AddObject(DWORD_PTR pObject, LPCTSTR pszPosition, LPCTSTR pszName = NULL);
		FTLINLINE BOOL RemoveObject(DWORD_PTR pObject);

		FTLINLINE VOID SetTrace(BOOL bTrace);
		FTLINLINE BOOL GetTrace();
		FTLINLINE VOID DumpLeakInfo();
	private:
		FTLINLINE CFMemCheckManager();
		FTLINLINE ~CFMemCheckManager();
		struct ObjectInfo
		{
			//UINT				m_nCount;		//o or 1
			//DWORD_PTR			pObjectAddr;
			CFStringFormater	m_strInfo;
		};
		CFCriticalSection		m_LockObj;
		typedef std::map<DWORD_PTR, ObjectInfo*>	ObjectPtrInfoContainer;
		ObjectPtrInfoContainer	m_allObjects;
		BOOL m_bTrace;

		static CFMemCheckManager*	s_pMemCheckMgr;
	};

	//#define CHECK_OBJ_MEM_LEAK()   \
	//if( 0 != CFDoubleFreeCheckMgr::Instance(). ) \
	//	{\
	//		FTLTRACEEX(FTL::tlWarning ,TEXT("%s(%d): Warning!!! Memory May Leak -- '%s' remain count is %d\n"), \
	//		TEXT(__FILE__), __LINE__, TEXT(#x), x::s_Count);\
	//	}

    //Function
}//namespace FTL

#ifndef USE_EXPORT
#  include "ftlMem.hpp"
#endif

#endif //FTL_MEM_H
