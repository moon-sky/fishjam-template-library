#ifndef F_DRIVER_THREAD_H
#define F_DRIVER_THREAD_H

/******************************************************************************************************************
* 异步IO
*   CreateFile 时指定 FILE_FLAG_OVERLAPPED,然后在 DeviceIoControl 等函数中使用 OVERLAPPED 参数或 ReadFileEx 函数中使用APC
******************************************************************************************************************/

/******************************************************************************************************************
* 内核对象
*   应用程序中的内核对象句柄是与进程相关的，需要使用ObReferenceObjectByHandle等函数将句柄转化为内核对象的指针才能在内核中使用,
*   注意使用完毕后需要 ObDereferenceObject 减少引用计数
* 
* 内核线程
*   PsCreateSystemThread -- 通过 ProcessHandle 参数来区分是创建新的 用户线程(属于当前进程) 还是 系统线程(属于System系统进程)
*   ★PsTerminateSystemThread★ -- 必须在线程函数内部调用该函数并设置返回值，否则线程无法退出。直接 return 是不行的？
* 
* KSPIN_LOCK -- 自旋锁,被锁住时如果再次申请获取该锁，程序会处于自旋状态(不停的轮询是否可以获取该锁，锁区域不能过长),
*   必须在低于或等于 DISPATCH_LEVEL 的IRQL级别中使用。一般用于各派遣函数间同步(如多线程同时读写同一个设备)。
*   KeInitializeSpinLock -- 初始化
*   KeAcquireSpinLock/KeReleaseSpinLock -- 获取和释放自旋锁，在获取锁时会改变为 DISPATCH_LEVEL
*   KeAcquireSpinLockAtDpcLevel/KeReleaseSpinLockFromDpcLevel -- 在 DISPATCH_LEVEL 级别申请和释放锁，不会改变级别
*
* 内核模式的同步对象(程序员可以获得真实同步对象的指针)
*   等待函数 -- KeWaitForSingleObject/KeWaitForMultipleObjects
*   FAST_MUTEX -- 快速互斥对象(速度快但不能递归获取释放)
*     ExInitializeFastMutex
*     ExAcquireFastMutex/ExReleaseFastMutex --
*   KEVENT -- 事件对象，分为 通知事件(NotificationEvent, 需要手动重置) 和 同步事件(SynchronizationEvent,Wait后自动重置)
*     KeInitializeEvent/KeSetEvent
*     IoCreateNotificationEvent/IoCreateSynchronizationEvent -- 创建或打开"有名"的通知事件/同步事件对象，多个驱动可以交互
*   KMUTEX -- 互斥对象(可递归获取释放)
*     KeInitializeMutex -- 初始化
*     KeRleaseMutex -- 释放互斥对象，然后可通过 KeWait 等待
*   KSEMAPHORE -- 信号量
*     KeInitializeSemaphore -- 初始化
*     KeReadStateSemaphore -- 读取信号量当前的计数
*     KeReleaseSemaphore -- 释放信号量，然后可通过 KeWait 等待
*   变量同步
*     InterlockedIncrement 系列 -- 不使用自旋锁，内部不会提升 IRQL，可操作非分页数据或分页数据
*     ExInterlockedIncrementLong 系列 -- 通过自旋锁实现，会提升IRQL，该系列函数不能操作分页内存数据
******************************************************************************************************************/

/******************************************************************************************************************
* 多线程同步
*   ExAcquireFastMutex/ExReleaseFastMutex -- 获取/释放一个快速互斥体
*
* 考虑代码的调用者在哪里 -- 处理函数可重入性 和 考虑运行中断级别
*   DriverEntry/DriveUnload -- 由系统进程的单一线程调用，不会出现多线程同时调用的情况
*   各种分发函数、完成函数、NDIS回调函数 -- 可能被多线程同时调用
* 
* 中断级(IRQL，中断请求级) -- 正在运行的线程随时可以被中断打断，进入到中断处理程序，当优先级高的中断来临时，
*   处在优先级低的中断处理程序会被打断，进入到更高级别的中断处理函数。
*   IRQL中规定了32个中断请求级别，0~2 为软件中断，3~31 为硬件中断(包括APIC的24个中断)，数字从0到31优先级别依次递增。
*   将24个IRQ映射到从 DISPATCH_LEVEL 到 PROFILE_LEVEL 之间，通常运行在软件中断级别中，当设备中断来临时，提升至硬件中断请求级(DIRQL)，处理结束后降低到原来的级别。
*   通常具有比较复杂功能的内核API(如 IoCreateFile)都要求必须在Passive级执行。
*   PASSIVE_LEVEL(0), DISPATCH_LEVEL(2), 设备中断请求级别 DIRQL(4)，
*   规则：
*     0.执行在高于或等于 DISPATCH_LEVEL 级的代码不可以引发页故障，否则会蓝屏 ( PAGED_CODE 宏 可以帮助检查本函数所在的中断请求级是否超过APC_LEVEL)
*     1.如果在调用路径上没有特殊的情况(导致中断级的提高或降低)，则一个函数执行时的中断级和它的调用源的中断级相同
*     2.如果在调用路径上有获取自旋锁，则中断级随之升高；如果调用路径上有释放自旋锁，则中断级随之下降
*     3.当前代码运行的中断级(如DISPATCH_LEVEL) 和 需要调用函数能运行的中断级(如PASSIVE_LEVEL)不一样时，不能使用API改变当前中断级(为什么?)。
*       但可以：
*       a.生成一个线程专门去执行指定中断级的代码
*     4.对于等于或高于 DISPATCH_LEVEL 级别的函数不能使用分页内存，必须使用非分页内存
*   主要调用源的运行中断级
*     用户模式的应用程序        -- PASSIVE_LEVEL
*     DriverEntry/DriveUnload/各种分发函数/AddDevice等	-- PASSIVE_LEVEL，需要时可申请进入 DISPATCH_LEVEL
*     完成函数、NDIS回调函数	-- DISPATCH_LEVEL
*     StartIO/DPC				-- DISPATCH_LEVEL
*     中断服务例程				-- 提升到设备对应的IRQL级别
*     内核中的线程调度模块      -- DISPATCH_LEVEL, 所以可以中断用户线程的执行进行调度切换
*   相关API
*     KeGetCurrentIrql       -- 得到当前的IRQL级别
*     KeRaiseIrql/KeRaiseIrqlToDpcLevel  -- 提高IRQL级别
*     KeLowerIrql            -- 降低恢复到原来的级别
******************************************************************************************************************/


/******************************************************************************************************************
* 中断：IRQ 和 INT
*   如 _asm int 3    -- 激发一个中断号为3的软件中断(即设置一个断点)
*   中断处理函数的首地址保存在 IDT(中断描述符表) 中，每个中断号只有最后设置的中断处理函数有效
*   在IOAIPC出现后，可以修改IRQ号到中断号的对应关心，
******************************************************************************************************************/

#endif //F_DRIVER_THREAD_H