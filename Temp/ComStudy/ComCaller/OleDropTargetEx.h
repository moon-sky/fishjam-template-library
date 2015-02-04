#pragma once
/**************************************************************************************************
* OLE拖放(Drag and Drop)--MFC为实现对象拖放提供了如下类：
*   COleDataObject
*   COleDataSource
*   COleDropSource
*   COleDropTarget -- 支持拖放操作的接收,成员函数Register在系统中注册窗口为拖放接收窗口。重载其
*     OnDragEnter、OnDragOver、OnDropEx、OnDrop等成员函数
* 启动拖放
* 接收拖放
**************************************************************************************************/

class COleDropTargetEx
{
public:
    COleDropTargetEx(void);
    ~COleDropTargetEx(void);
};
