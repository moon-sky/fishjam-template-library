#ifndef FTL_MEM_H
#define FTL_MEM_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlMem.h requires ftlbase.h to be included first
#endif

#include "ftlBase.h"
#include "ftlThread.h"
#include "ftlFunctional.h"

#ifndef ENABLE_CALL_STACK
# define ENABLE_CALL_STACK 1
#endif

#if ENABLE_CALL_STACK
#include "ftlStackWalker.h"
#endif

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
*
* 字节对齐 --  #pragma pack(1)
*
* HGLOBAL -- 兼容16位内存分配API的数据，表示一个内存区，GlobalAlloc -> GlobalLock -> 读写内存 -> GlobalUnlock -> GlobalFree 
*   GlobalAlloc 时的参数
*     GMEM_FIXED: 分配固定位置的内存，返回值就是内存指针(GlobalLock 会返回同样的值)，可以直接使用，但推荐还是通过 Lock/Unlock 后使用?
*     GMEM_MOVEABLE: 分配可移动的内存(内存块不会在物理内存区域移动，但会在缺省 heap 内移动? ), 需要通过 GlobalLock 后才能使用
*                    实测没有发现地址移动 -- Win32 采用了高级的内存管理方案，因此不会移动？只有在16位时才会移动 ?
*     GMEM_DISCARDABLE：分配可丢弃内存块
*     GMEM_NODISCARD： 函数调用期间不丢弃任何内存块
*     GMEM_ZEROINIT：新分配的内存块全部初始化成零，可和其他标志组合
*   GlobalUnlock -- 解除被锁定的内存对象，如返回值为TRUE(通常是1)，表指定的内存对象仍处于被锁定状态；如返回值为FALSE且GetLastError为0，表内存对象已经解锁
*     实质是对内存对象的锁定计数器减一
*
* CreateStreamOnHGlobal -- 使用 HGLOBAL 句柄指向的内存创建流对象(IStream),
*   TODO: 1.其 HGGLOBAL 句柄必须是 moveable and nondiscardable 的 ?
*         2.使用 SHCreateMemStream 性能更好
********************************************************************************************/

namespace FTL
{
    //check GlobalAlloc memory status,
    //h -- HGLOBAL return GlobalAlloc
    //n -- lock count by GlobalLock
    //a -- allocation values of the memory object, It can be zero or GMEM_DISCARDED
    //ATLASSERT(((nTmpFlags & 0xFF00) >> 8) == (a));
    #ifndef GLOBAL_FLAGS_CHECK
    # ifdef _DEBUG
    #   define GLOBAL_FLAGS_CHECK(h, n, a) \
        { \
        INT nTmpFlags = GlobalFlags(h); \
        ASSERT(GMEM_INVALID_HANDLE != nTmpFlags); \
        ATLASSERT((nTmpFlags & GMEM_LOCKCOUNT) == (n)); \
    }
    # else
    #  define GLOBAL_FLAGS_CHECK(h, n, a) __noop
    # endif 
    #endif 

    //Class
    
    //模板内存池
    //  注意：Get后返回的指针将是上一次的值，需要自行进行初始化
    //        Get和Release都从list前部进行，减少page交换的次数
    template <typename T>  //, typename THREADLOCK = CFCriticalSection
    class CFMemoryPoolT
    {
    private:
        template <typename T>
        struct CFMemoryObjectT{
        public:
            LONG index;
            T    obj;
            FTLINLINE bool operator < (const CFMemoryObjectT<T> & other) const
            {
                COMPARE_MEM_LESS(index, other);
                return false;
            }
        };
        typedef CFMemoryObjectT<T>    CFMemoryObject;
    public:
        FTLINLINE CFMemoryPoolT( INT nInitSize = 0 , INT nMaxSize = INT_MAX);
        FTLINLINE ~CFMemoryPoolT();
        FTLINLINE T* Get();
        FTLINLINE void Retrun( T* t );

        //真正的释放，如果 nCount = -1， 表示将 m_freeBlocks 中的全部释放掉
        INT Free(INT nCount = -1);
    private:
        //typename THREADLOCK	m_lockObject;
        CFCriticalSection	m_lockObject;
        
        typedef std::set< CFMemoryObject*, UnreferenceLess<CFMemoryObject*> > ObjectContainer;
        ObjectContainer     m_allBlocks;
        ObjectContainer     m_freeBlocks;
        INT                 m_nMaxSize;
        INT                 m_nCurCount;
        LONG                m_nNextIndex;
        FTLINLINE LONG      _GetNextIndex(){ return InterlockedIncrement(&m_nNextIndex); }
    };

    //从内存池中获取对象的辅助类，构造中获取，释放中放回内存池
    template <typename T>
    class CFMemoryPoolObjectHelper
    {
    private:
        DISABLE_COPY_AND_ASSIGNMENT(CFMemoryPoolObjectHelper);
        typedef CFMemoryPoolT<T>        MemoryPoolType;
        MemoryPoolType* m_pMemPool;
    public:
        T*  pObject;
        CFMemoryPoolObjectHelper(MemoryPoolType* pMemPool);
        ~CFMemoryPoolObjectHelper();
    };

    //TODO: 参考 .NET 的 System.Web.Caching.Cache 类的特点
    //  如果缓存项已经过期，或者缓存项所依赖的某个文件或对象发生了改变，或内存紧张时，自动移除缓存项 -- 使用前需要检查缓存项是否存在
    //  缓存项支持缓存依赖性(CacheDependency) -- 链接到的文件、DB中的表或其他任何类型的资源，如发生变化，自动视为无效并被移除
    FTLEXPORT template<typename KEY, typename VALUE, typename CHILDCLASS>
    class CFItemCacheT
    {
    public:
        CFItemCacheT(DWORD nMaxCacheSize,DWORD clearPercent = 30);
        virtual ~CFItemCacheT();
    public:
        enum ClearMode
        {
            //clearByOptimal,     //最佳置换算法(需要“估计”不使用或很长时间内不使用的的项，很难实现)
            //clearByLFU,         //Least Frequently Used(最不经常使用), TODO -- 实现
            clearByLRU,         //Least recently used(最近最少使用算法，通常只置换一个)
            clearByMRU,         //Most recently used(最近最常使用算法，性能非常差) 
            //clearByNUR,         //Not Used Recently(最近未用置换算法,是LRU的近似算法，置换所有从上次计算后未使用过的)
            clearByFIFO,        //First In First Out(先入先出算法，最直观，但性能最差)
            clearByRandom,      //随机清除，不排序(可能是按照 map 中 KEY 的大小顺序排序)
        };
        BOOL GetItemByKey(const KEY key,VALUE& value);
        //BOOL GetItmeByKeyAsync(const KEY key); //TODO:异步获取，如果有则马上返回，否则通过指定的异步方法调用后，通过回调返回
        void SetClearMode(ClearMode mode);
        BOOL SetClearPercent(DWORD clearPercent);    //使用百分比进行清除
    protected:
        //virtual BOOL GetRealItem(KEY key,VALUE& value) = 0;
        //virtual void FreeItem(KEY key,VALUE& value) = 0;
    protected:
        struct CItemData
        {
            KEY key;
            VALUE value;
            LARGE_INTEGER lastAccess;   //上一次访问的时间
            DWORD index;
        };
        typedef std::map<KEY,CItemData*>  CACHE_MAP;
        CACHE_MAP m_ItemCacheMaps;
        static bool LastAccessGreater(CItemData* p, CItemData* q);
        static bool LastAccessSmaller(CItemData* p, CItemData* q);
        static bool IndexGreater(CItemData* p, CItemData* q);
        static bool IndexSmaller(CItemData* p, CItemData* q);
        void ClearCache(DWORD percent);     
    private:
        void Init(DWORD nMaxCacheSize,DWORD clearPercent);

        CFCriticalSection   m_LockObject;
        ClearMode           m_ClearMode;
        DWORD               m_MaxCacheSize;
        DWORD               m_ClearPercent;
        DWORD               m_CurrentIndex;
    };

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
			CAtlString	        m_strInfo;
		};
		CFCriticalSection		m_LockObj;
		typedef std::map<DWORD_PTR, ObjectInfo*>	ObjectPtrInfoContainer;
		ObjectPtrInfoContainer	m_allObjects;
		BOOL m_bTrace;

		static CFMemCheckManager*	s_pMemCheckMgr;
	};

    class CFMemCheckManagerHelper
    {
    public:
        FTLINLINE CFMemCheckManagerHelper();
        FTLINLINE ~CFMemCheckManagerHelper();
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
