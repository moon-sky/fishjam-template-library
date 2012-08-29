#ifndef FTL_FILE_H
#define FTL_FILE_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlfile.h requires ftlbase.h to be included first
#endif

namespace FTL
{
//Class
    class CFStructuredStorageFile;
//Function

}//namespace FTL

#include <WinIoctl.h>
#include "ftlsystem.h"

namespace FTL
{
    #define BYTES_PER_KILOBYTE      (1024)
    #define BYTES_PER_MEGABYTE      (BYTES_PER_KILOBYTE * 1000)
    #define BYTES_PER_GIGABYTE      (BYTES_PER_MEGABYTE * 1000)
    #define BYTES_PER_TERABYTE      (BYTES_PER_GIGABYTE * 1000)
    #define BYTES_PER_PETABYTE      (BYTES_PER_TERABYTE * 1000)


    /**************************************************************************************************
    * 共享内存分为两种(注意多线程、进程访问时的同步)：
    *   1.本地共享内存 -- linux 下是 shmget, Windows下是 内存映射文件
	*     ATL 通过 CAtlFileMapping 对文件映射进行了封装
    *   2.分布式共享内存(DSM) -- 在网络上扩展了虚拟内存的概念，通过全局/共享内存中的数据进行透明的进程间通信。
    *     如多台计算机被群集(cluster)为一套逻辑系统，系统中的内存在群集中共享应用程序通过共享内存通信。
    *  
    * CreateFile 可以用来打开各式各样的资源，包括文件(硬盘，软盘，光盘或其他)，串口和并口，Named pipes，console等，
    *   可以在 dwFlagsAndAttributes 变量上设置 FILE_FLAG_OVERLAPPED 属性。
    *   OVERLAPPED结构执行两个功能：
    *     1.识别每一个目前正在进行的overlapped操作；
    *     2.在代码和系统之间提供一个共享区域，参数可以在该区域中双向传递。
    *   GetOverlappedResult
    * 
    *   虽然你要求一个overlapped操作，但它不一定就是overlapped，如果数据已经被放进cache中，或操作系统认为它可以很快地
    *   取得那份数据，那么文件操作就会在ReadFile或WriteFile返回之前完成，而这两个函数将返回TRUE。这种情况下，文件handle
    *   处于激发状态，而对文件的操作可被视为overlaped一样。另外，如果你要求一个文件操作为overlapped，而操作系统把
    *   这个“操作请求”放到队列中等待执行，那么ReadFile和WriteFile都会返回FALSE，这时你必须调用GetLastError并确定
    *   它传回ERROR_IO_PENDING，那意味着“overlapped I/O请求”被放进队列中等待执行
    * 
    * Console 文件
    *   m_hOutput = GetStdHandle (STD_OUTPUT_HANDLE);
    *   if (m_hOutput == INVALID_HANDLE_VALUE) {
    *     API_VERIFY(AllocConsole ());
    *     m_hOutput = GetStdHandle (STD_OUTPUT_HANDLE);
    *   }
    *   SetConsoleTitle (TEXT("ActiveX Debug Output"));  
    *   
    * 通过文件句柄获取文件信息 GetFileInformationByHandle
	*
	* CreateFile 时如果有 FILE_FLAG_SEQUENTIAL_SCAN， 表示优化Cache，适用于从头到尾顺序访问，不会随机跳
    **************************************************************************************************/
	
#if 0
    class CFConsoleFile
    {
    public:
        FTLINLINE CFConsoleFile();
        virtual ~CFConsoleFile();
    };
#endif 

    class CFFile
    {
	public:
		enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };
		// Constructors, destructor
		FTLINLINE CFFile();
		FTLINLINE CFFile(HANDLE hFile);
		FTLINLINE virtual ~CFFile();

		// Attributes
		HANDLE m_hFile;

		// Operations
		FTLINLINE virtual DWORD GetPosition() const;
		FTLINLINE virtual CString GetFileName() const;
		FTLINLINE virtual CString GetFilePath() const;
		FTLINLINE virtual BOOL SetFilePath(CString strNewName);

		FTLINLINE virtual BOOL Open(LPCTSTR pszFileName, 
			DWORD dwAccess = GENERIC_WRITE | GENERIC_READ,
			DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE,
			LPSECURITY_ATTRIBUTES lpSA = NULL,
			DWORD dwCreationDisposition = CREATE_ALWAYS,
			DWORD dwAttributes = FILE_ATTRIBUTE_NORMAL,
			HANDLE hTemplateFile = NULL
			);

		FTLINLINE static BOOL Rename(CString strOldName, CString strNewName);
		FTLINLINE static BOOL Remove(CString strFileName);

		FTLINLINE DWORD SeekToEnd();
		FTLINLINE DWORD SeekToBegin();

		FTLINLINE virtual CFFile * Duplicate() const;

		FTLINLINE virtual DWORD Seek(LONG lOff, UINT nFrom);
		FTLINLINE virtual BOOL SetLength(DWORD dwNewLen);
		FTLINLINE virtual DWORD GetLength() const;

		FTLINLINE virtual DWORD Read(void* lpBuf, DWORD nCount, LPOVERLAPPED lpOverlapped = NULL);
		FTLINLINE virtual BOOL Write(const void* lpBuf, DWORD nCount);
		FTLINLINE BOOL WriteEndOfLine();

		FTLINLINE virtual BOOL LockRange(DWORD dwPos, DWORD dwCount);
		FTLINLINE virtual BOOL UnlockRange(DWORD dwPos, DWORD dwCount);

		FTLINLINE virtual BOOL Flush();
		FTLINLINE virtual BOOL Close();

		enum BufferCommand { 
			bufferRead, 
			bufferWrite, 
			bufferCommit, 
			bufferCheck, 
			bufferBlocking,
			bufferDirect
		};
		FTLINLINE virtual UINT GetBufferPtr(UINT nCommand, UINT nCount = 0, void** ppBufStart = NULL, void** ppBufMax = NULL);

	protected:
		CString m_strFileName;	// stores the file name for the current file
    };

    //有 ATLPath 实现了很多功能, 如 RemoveFileSpec(删除路径最后的文件名)
    class CFPath
    {
    public:
		FTLINLINE static BOOL CreateDir(LPTSTR szPath);
    private:
        
    };

    class CFStructuredStorageFile
    {
    public:
        const static DWORD STRUCTURED_STORAGE_FILE_DEFAULT_MODE = STGM_READWRITE | STGM_SHARE_EXCLUSIVE;
        FTLINLINE CFStructuredStorageFile();
        FTLINLINE virtual ~CFStructuredStorageFile();
        FTLINLINE HRESULT CreateDocFile(LPCTSTR pszFilePath, DWORD mode = STGM_CREATE | STRUCTURED_STORAGE_FILE_DEFAULT_MODE);
        FTLINLINE HRESULT OpenDocFile(LPCTSTR pszFilePath,DWORD mode = STRUCTURED_STORAGE_FILE_DEFAULT_MODE);
        FTLINLINE void Close();
        FTLINLINE IStorage * Attach(IStorage* pNews);
        FTLINLINE IStorage * Detach();
        // storage-level access:
        FTLINLINE HRESULT CreateStorage(LPCTSTR pName,BOOL bEnter = TRUE, DWORD mode = STRUCTURED_STORAGE_FILE_DEFAULT_MODE);
        FTLINLINE HRESULT EnterStorage(LPCTSTR pName,DWORD mode = STRUCTURED_STORAGE_FILE_DEFAULT_MODE);
        FTLINLINE HRESULT ExitStorage();
        //bool CreateStream(const CString & name, COleStreamFile &sf, DWORD mode = CFile::modeReadWrite | CFile::shareExclusive);
        //bool OpenStream(const CString & name, COleStreamFile &sf, DWORD mode = CFile::modeReadWrite | CFile::shareExclusive);
        FTLINLINE HRESULT CreateStream(LPCTSTR pName,IStream** ppChildStream,DWORD mode = STRUCTURED_STORAGE_FILE_DEFAULT_MODE);
        FTLINLINE HRESULT OpenStream(LPCTSTR pName,IStream** ppChildStream,DWORD mode = STRUCTURED_STORAGE_FILE_DEFAULT_MODE);
        FTLINLINE HRESULT DestroyElement(LPCTSTR pName);
        // status info:
        FTLINLINE IStorage* GetRootStorage() const;
        FTLINLINE IStorage* GetCurrentStorage() const;
        FTLINLINE BOOL IsOpen() const;
        //CString GetPath(const CString & SepChar) const;
        //CString GetFilename() const;
    private:
        struct StorageData
        {
            IStorage *Stg;
            StorageData *ParentStg;
            StorageData()
            {
                Stg = NULL;
                ParentStg = NULL;
            }
        };
        IStorage    *m_pRootStg;
        StorageData *m_pCurrentStg;
    };

    class CFileSystemFinder
    {

    };

}//namespace FTL

#endif //FTL_FILE_H

#ifndef USE_EXPORT
#  include "ftlFile.hpp"
#endif