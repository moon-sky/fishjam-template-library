#pragma once


/***********************************************************************************************
* 复合文档-- WORD文件就是复合文档（使用 VC6中的复合文件浏览器 DFView.exe 查看，不支持中文路径）：
* 
* 文件存储类型分为：
*   非结构化文件（如记事本），
*   标准结构化文件（如电子表格）
*   自定义结构化文件，
*   复合文件（使用类似文件系统的方式保存文件，根存储 -> 子存储 -> 流），其特点：
*     1.使用指针构造的一棵树进行管理，由于使用的是单向指针，因此当做定位操作的时候，向后定位比向前定位要快。
*     2.流对象是真正保存数据的空间，存储单位为512字节。
*     3.不同的进程，或同一个进程的不同线程可以同时访问一个复合文件的不同部分而互不干扰；
*     4.复合文件则提供了非常方便的“增量访问”能力 -- 能方便的增加、删除；
*     5.删除文件中的内容后需要进行整理；
*     6.多个程序可以共享同一个复合文件 -- 复合文档
*   命名规则：
*     根存储对象的名字遵守文件系统的命名约定
*     长度不超过32个字符
*     首字符使用大于32的字符，小于32的字符作为首字符有特殊意义
*     不能使用字符“\”、“/”、“:”和“!”
*     名字“.”和“..”被保留
*     名字保留大小写，但比较操作大小写无关
*   底层机制：ILockBytes对象，把存储介质描述成一般化的字节序列
* 
* 函数和接口
*   StgCreateDocfile    <== 建立一个复合文件，得到根存储对象
*   StgCreateDocfileOnILockBytes <== ?
*   StgOpenStorage      <== 打开一个复合文件，得到根存储对象 
*   StgIsStorageFile    <== 判断一个文件是否是复合文件 
*   WriteClassStg       <== 写CLSID到存储中，同IStorage::SetClass() 
*   ReadClassStg        <== 读出WriteClassStg()写入的CLSID，相当于简化调用IStorage::Stat() 
*   WriteClassStm       <== 写CLSID到流的开始位置 
*   ReadClassStm        <== 读出WriteClassStm()写入的CLSID 
*   WriteFmtUserTypeStg <== 写入用户指定的剪贴板格式和名称到存储中 
*   ReadFmtUserTypeStg  <== 读出WriteFmtUserTypeStg()写入的信息。方便应用程序快速判断是否是它需要的格式数据。 
*   CreateStreamOnHGlobal<== 内存句柄 HGLOBAL 转换为流对象 
*   GetHGlobalFromStream<== 取得CreateStreamOnHGlobal()调用中使用的内存句柄 
*
*   IStorage 
*     CreateStorage     <== 在当前存储中建立新存储，得到子存储对象 
*     CreateStream      <== 在当前存储中建立新流，得到流对象 
*     OpenStorage       <== 打开子存储，得到子存储对象 
*     OpenStream        <== 打开流，得到流对象 
*     CopyTo            <== 复制存储下的所有对象到目标存储中，该函数可以实现“整理文件，释放碎片空间”的功能 
*     MoveElementTo     <== 移动对象到目标存储中 
*     DestoryElement    <== 删除对象 
*     RenameElement     <== 重命名对象 
*     EnumElements      <== 枚举当前存储中所有的对象 
*     SetElementTimes   <== 修改对象的时间 
*     SetClass          <== 在当前存储中建立一个特殊的流对象，用来保存CLSID，以后可以根据CLSID查找关联的程序（如Word中保存Excel）
*     Stat              <== 取得当前存储中的系统信息 
*     Release           <== 关闭存储对象 
*  
*   IStream
*     Read              <== 从流中读取数据 
*     Write             <== 向流中写入数据 
*     Seek              <== 定位读写位置 
*     SetSize           <== 设置流尺寸。如果预先知道大小，那么先调用这个函数，可以提高性能 
*     CopyTo            <== 复制流数据到另一个流对象中 
*     Stat              <== 取得当前流中的系统信息 
*     Clone             <== 克隆一个流对象，方便程序中的不同模块操作同一个流对象 
*     Release           <== 关闭流对象 
*
*   IPersistStorage -- 用复合文件的存储(Storage)功能来保存/读取数据
*   IPersistStreamInit -- 用复合文件的流(Stream)功能来保存/读取数据
*
* 事务机制( Commit、Revert )
*   以STGM_TRANSACTED标志为基础，事务机制需要消耗较多系统资源
*   Commit参数：
*     STGC_DEFAULT
*     STGC_OVERWRITE
*     STGC_ONLYIFCURRENT
*     STGC_DANGEROUSLYCOMMITMERELYTODISKCACHE
***********************************************************************************************/
class CStorageFileUtil
{
public:
    CStorageFileUtil(void);
    ~CStorageFileUtil(void);
public:
    //将复合文档的结构显示出来，尚未实现完
    HRESULT DumpStorageFileInfo(LPCTSTR pszFilePath);
    HRESULT CreateDemoStorageFile(LPCTSTR pszFilePath);
};
