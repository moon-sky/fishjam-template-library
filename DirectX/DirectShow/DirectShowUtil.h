#pragma once

/*************************************************************************************************
* CAMSchedule -- 其实此类就是一个List，Node是类型CAdvisePacket, 做具体解析的
* CAutoLock + CCritSec -- 范围锁，进行资源保护
* CBaseAllocator -- 管理IMediaSample列表的抽象基类，子类实现Alloc和Free函数,Add时是加在列表的最前面(使用效率高?)
*    其实现时使用了一些小技巧(lazy signalling mechanism),只在必要时才进入OS等待模式，从而提高多线程时的效率
* +- CMemAllocator -- 默认的Sample管理器(使用new在主存储器中分配内存)，包括分配/释放Sample的内存，维持Sample队列等
*     通过全局函数 CreateMemoryAllocator 创建 CLSID_MemoryAllocator
* CBaseReferenceClock -- 继承了 IReferenceClock 接口，
* CBaseWindow -- 可以用作 Render 的窗体，其中有自己的工作线程。
* CDrawImage -- 
* CEnumPins -- 枚举Filter上的PIn
* CEnumMediaTypes -- Pin上的首选媒体类型枚举
* CGenericList -- 通用的List模板基类
* CImageDisplay -- Video Renderer 使用的工具类，可以获取显示媒体的格式等信息？
* CMediaSample -- 封装了指向数据内存的指针及相关的属性。Sample由Pin上的Sample管理器创建，并维持
*   在一个空闲Sample队列中(CBaseAllocator管理?)，需要使用时，通过GetBuffer获得一个空Sample(如果当前没有空Sample的话，等待)。
*   使用完毕后，Release，如果引用计数变为0，会自动调用IMemAllocator::ReleaseBuffer，把自己重新加入
*   空闲Sample队列中。直到调用IMemAllocator::Decommit时，Sample对象才会真正析构。
* CPosPassThru -- 在 Render 或 Transform Filter 中处理 IMediaPosition IMediaSeeking 的类，可以用于位置控制？
*    CreatePosPassThru 函数创建 ISeekingPassThru 接口实例
* CRefTime
*   COARefTime -- 在 Double/REFERENCE_TIME 等格式中转换的辅助类
*************************************************************************************************/
class CDirectShowUtil
{
public:
    CDirectShowUtil(void);
    ~CDirectShowUtil(void);
    static BOOL SetFilterMerit(LPCTSTR inClsid, DWORD inMerit);
};
 