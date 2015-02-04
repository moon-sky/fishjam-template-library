#ifndef COM_THREADING_MODEL_H_
#define COM_THREADING_MODEL_H_

/***********************************************************************************************
* COM多线程同步
*   实例数据的同步:对象级的锁(方便安全但并发性低) 和 细粒度的数据锁(允许高并发但需要小心使用防止死锁)
*     CComCriticalSection -- 需要手动 Init/Term 的 CRITICAL_SECTION, Init->Lock->Unlock->Term
*       用于需要一个全局或者静态的临界区，并且C运行库(CRT)不能自动完成构造和析构工作时（为什么会这样?不使用CRT?）
*     CComAutoCriticalSection -- 构造中自动初始化，析构中自动终止， Lock->Unlock
*     CComAutoDeleteCriticalSection -- 从 CComCriticalSection 继承，可以防止多次调用 Init/Term ?
*     CComFakeCriticalSection -- 通常用于单线程模型时，避免锁定的开销
*   自动的范围锁：CComObjectLockT<>，在 CComObjectRootEx 中typedef 为 ObjectLock，使用方法：ObjectLock lock(this);
*
* CComXXXThreadModel -- 定义了静态的 Increment/Decrement，用于抽象出管理一个对象的生命周期计算器在单线程和多线程方式下的差异
*   CComSingleThreadModel
*   CComMultiThreadModel
*   CComMultiThreadModelNoCS -- 引用计数时采用线程安全的方式，但不使用对象级锁，即没有除引用计数外需要保护的成员变量
*      或使用细粒度的数据锁
*   
* 服务器的“缺省”线程模型(没有看到对应的宏),ATL使用相关的宏来typedef CComObjectThreadModel/CComGlobalsThreadModel，
*   ATL在内部使用 CComObjectThreadModel 保护实例数据， CComGlobalsThreadModel 来保护全局和静态数据。
*   为了保证程序运行正确，哪怕服务器中只有一个多线程类，也必须把向导生成德尔 _ATL_APARTMENT_THREADED 改为 _ATL_FREE_THREADED
*     _ATL_SINGLE_THREADED
*     _ATL_APARTMENT_THREADED  -- ftlConfigDetect.h 中有对其的判断
*     _ATL_FREE_THREADED
*
* 一个进程只能有一个MTA，或一个也没有。所有使用MTA MODULE的COM OBJECT在同一个MTA里边（互相调用时不通过stubs，proxies ）
* 
* 使用 IComThreadingInfo 接口可以获得当前的COM线程模型 -- 问题：CLSID_???
* COM的线程模式指的是两个方面：
*   客户程序的线程模式 -- STA、MTA
*   组件所支持的线程模式 -- Single(单线程)、Apartment(STA)、Free(MTA)、Both(STA+MTA)、Neutral
* 如果客户程序和组件都支持同样的线程模式，那么COM就允许客户程序直接调用对象，这样将产生最佳性能。
*
* Apartment(套间/单元) -- 用于帮助组件的开发者在实现多线程下调用组件时候的同步问题
*   提供了三种线程模型，有三种不同的套间分别对应。
* COM 提供三种线程模型
*   0.STA发布前，所有组件访问都通过应用程序的主线程(消息循环)进行，性能最差 -- 主STA
*   1.Single-Threaded Apartment（STA 单线程套间）-- 每一个COM线程有自己的STA。
*     适合于拥有界面的组件，如ActiveX控件、OLE文档服务器等。
*     套间和1个线程、0~n个对象关联。通过创建一个具有OleMainThreadWndClass窗口类的隐藏窗口，其他线程通过发送
*     消息的方式进行调用(COM库自动进行同步)。STA中的多个组件都要串行，互相会影响。最安全，但性能很差。
*   2.Multithread Apartment（MTA 多线程套间）-- 允许不同线程的CLIENT同时访问COM OBJECT，适用于性能要求较高的业务逻辑组件或干后台服务的组件,
*     套间和多个线程,0~n个对象关联，只有该套间中的线程能访问对象，其他线程对套间内对象的调用将
*     使用RPC管理的线程池 进行远程调用，并且该对象的代码在自己的方法中实现了线程保护。
*     调用系统的方法时线程安全，但其他全局变量、静态变量需要进行线程同步，性能中等。
*     需要自己同步对象访问代码，如 Release 时的冲突考虑
*       DWORD temp = InterlockedDecreament( &m_RefCount ); if(!temp) delete this;   //判断时不能使用m_RefCount
*     编写MTA时不能保存任何记录着TLS内存的指针或窗口句柄（没有任何线程范围的成员变量）
*       -- 由于MFC底层大量使用TLS，因此MFC不能编写在MTA中运行的组件。
*   3.Neutral Apartment（NTA 中立线程套间，由COM+提供，只能用于进程内服务器）
*     套间不关联线程，只关联 0~n 个对象，任何线程都可以直接访问里面的对象 -- 性能最佳，但需要用户同步,
*     COM+中的每个对象都有一个环境和其相绑定，环境记录了必要的信息，并监听对对象的每一次调用，
*       以保证当将对象的接口指针成员变量进行传递或回调时其操作的正确性.
*   3.1.Both -- 这种组件很像NA，哪个套间都可能直接访问它，但只是可能，而NA组件是可以。
*       支持一种称作 自由线程调度器（FTM——Free Threaded Marshaler）的技术，可以获得最佳性能。
*       由COM提供的系统组件，CoCreateFreeThreadedMarshaler，
*       FTM实现了接口 IMarshal，当调度接口指针的函数时，内部调用IMarshal内的相关函数，并判断如果调度发生
*       在一个进程内的公寓之间则直接返回接口指针；如果调度发生在进程之间或者远程计算机间，则调用标准的调度器，
*       并返回指向代理对象的指针。所以可见使用FTM，可以跨过公寓间的调度而直接调用接口方法 -- 最佳性能。
*
* 向导中选项(表明组件可以适用于哪种线程模型)
*   Apartment   -- 组件只能以STA模式运行,单元模式，当在线程中调用组件函数的时候，系统通过隐藏的
*                  窗口消息来排队这些调用，因此通常不用考虑同步的问题。
*                  对象是单线程模型，全局是多线程模型。
*   Free        -- MTA，对象和全局都是多线程模型
*   Both        -- STA+MTA，和 自由线程调度器(FreeThreaded Marshaler)结合
*     1.增加 CComPtr<IUnknown> m_pUnkMarshaler 的成员变量
*     2.在 FinalConstruct 中 return CoCreateFreeThreadedMarshaler(GetControllingUnknown,&m_pUnkMarshaler.p);
*        
*   Single(None)-- 只能在一个单一的线程中执行(主STA),对象和全局线程模型都是单线程
*    
* 列集（Marshaling）和散集（Unmarshaling） 
*     -- 进程间和远程Marshal是因为处于不同的地址空间并且为了将参数打包；本地进程Marshal是为了线程同步。
*   列集 -- 将信息以某种格式存为流（IStream*）形式的操作；
*   散集 -- 列集的反操作，将信息从流形式中反还出来；
*     列集可以通过函数CoMarshalInterface函数来完成,当调用CoMarshalInterface第一次为对象进行列集时会为对象建
*     立一个存根管理器,而以后对同一个对象的其他接口列集时,就不会再建一个存根管理器。
*     CoUnmarshalInterface又是根据什么来确定是否要建立代理管理器的????
*
* 一般有两种代理对象的实现方式 -- （标准列集法以接口为基础，自定义列集法以对象为基础？）
*   自定义Marshal(custom marshaling) -- 组件需实现IMarshal接口和一个代理组件（即完全实现真正组件所有接口的一个副本，
*     实现了汇集方法参数及线程模型的规则[如接口指针用CoMarshalInterface/CoUnmarshalInterface]，
*     也必须实现IMarshal接口），并将这个代理组件在客户机上注册，以保证代理对象的正确生成。 
*   标准Marshal(standard marshaling) -- 组件需要为自己生成一个代理(Proxy)/存根(Stub)组件(可通过MIDL由IDL文件自动生成),
*     并通过聚合方式，由COM提供的标准代理对象的实现调用。
*
*  组件调用机制: Client [ -> 本地代理Proxy -> 通道 -> 远程存根Stub ] -> Server，调用 进程外或远程组件、
*   以及多线程环境中传输接口指针(为了同步访问)时 使用 代理/存根。进程外组件必须注册代理存根DLL，否则连接口都无法创建。
*   COM+中代理存根DLL需要注册到所有需要列集(Marshal)和散集(Unmarshal)自定义接口的COM客户和服务器所在的机器上。
* 可以使用标准的自动化调度器(Standard Automation Marshaler，类型库调度器)实现调度，以免每个组件都需要
*   单独的proxy/stub DLL。（使用OLEView查看 proxy/stub 入口为 oleaut32.dll）要求：
*     1.接口方法中智能使用与自动化兼容的类型库;
*     2.在IDL中加入 oleautomation 属性（会使用 RegisterTypeLibrary 函数注册类型库）
* Marshaling 方法在Marshal进程外组件时使用LPC，分布式网络中使用RPC(如果实现自定义调度，需要实现 IMarshal 接口，
*   如：可以实现为使用 TCP/IP 实现DCOM )
*  
* 全局接口表(GIT)
*   可以把接口指针存进表中，然后在别的 Apartment 内把其取出，GIT自动执行公寓间的调度，所以很方便。
*   通过IGlobalInterfaceTable访问的。通过创建CLSID为 CLSID_StdGlobalInterfaceTable 的对象可调用它。
*
* 注：
*   1.当MTA调STA或STA调MTA，都会发生线程切换，消耗大量时间(内核/用户模式切换)，因此产生了NTA。
*   2.套间的规则都是通过对代理对象而非组件对象发起调用以截取对组件对象的调用由代理对象来实现的。
*   3.跨套间调用时，被调用的对象指针是指向一个代理对象，不是组件对象本身，如果直接传递接口指针到另外的线程中调用，
*     不能保证调用结果正确(无法使用COM的线程模型)。需要通过 CoMarshalInterface/CoUnmarshalInterface 来使得传送的
*     接口指针指向代理对象。同一进程内的线程可以使用CoMarshalInterThreadInterfaceInStream/CoGetInterfaceAndReleaseStream 进行简化）。
*     问题：怎么验证在进程内组件中直接传递接口指针会出现错误？？？
*           怎么知道线程具体所在的是哪个套间(STA/MTA 等)？？？
*
* CoInitialize -- 告诉COM生成一个新的STA套间，并将当前的调用线程和这个套间相关联。
* CoInitializeEx:
*   COINIT_APARTMENTTHREADED -- 同 CoInitialize，多线程执行时，将调用“串行化”到创建对象的线程。
*   COINIT_MULTITHREADED(也叫自由线程) -- 告诉COM检查是否已经有了一个MTA套间，没有则生成一个MTA套间，然后将那个套间和调用线程相关联。
*     多线程执行时，各个函数的调用★可能★（还依赖于COM组件的ThreadingModel）同时发生，需要COM作者自行同步。
*
* 进程内的主STA套间 -- 进程中第一个调用 COINIT_APARTMENTTHREADED 的线程所关联的套间（即进程中的第一个STA套间）
* 
* 进程内组件时，根据注册表 InprocServer32\ThreadingModel 和 线程的不同，组合生成最后的套间结果
*   创建线程关联的套间种类          ThreadingModel键值              组件对象最后所在套间  
*            STA                        Apartment                      创建线程的套间(A)  
*            STA                           Free                        进程内的MTA套间(B) 
*            STA                           Both                        创建线程的套间(A)
*            STA                        ""或Single                    进程内的主STA套间(C)  
*            STA                          Neutral                       进程内的NTA套间(D)
*            MTA                         Apartment                     新建的一个STA套间(E)
*            MTA                            Free                        进程内的MTA套间(B)
*            MTA                            Both                        进程内的MTA套间(B)
*            MTA                        ""或Single                     进程内的主STA套间(C)
*            MTA                          Neutral                       进程内的NTA套间(D)
*   创建线程的套间(A)   -- 
*   进程内的MTA套间(B)  --
*   进程内的主STA套间(C)--
*   进程内的NTA套间(D)  --
*   新建的一个STA套间(E)--
*
* 最佳实例：
*   1.（除ActiveX等UI组件外？）创建同时支持 STA/MTA 的 Both 组件 -- 性能最好，灵活性最高，但需要注意
*     a.必须选中 Free Threaded Marshaler 支持，否则会出错？ -- 为什么？
*       限制：使用FTM的对象不能直接拥有没有实现FTM的对象的接口指针；使用FTM的对象不能拥有其他公寓对象代理的引用。
*     b.注意线程访问的互斥安全（使用 Lock/Unlock 函数进行保护，更好的是用 ObjectLock lock(this) 自动锁定）
*   2.线程间传递接口必须进行Marshal(CoMarshalInterThreadInterfaceInStream) -- 如果不需要(如MTA中)，COM库也能正确处理
*   3.选中DLL 中 Proxy/Stub Dll 的代码，防止需要Marshal(是否真的需要这样？)
***********************************************************************************************/

#endif //COM_THREADING_MODEL_H_