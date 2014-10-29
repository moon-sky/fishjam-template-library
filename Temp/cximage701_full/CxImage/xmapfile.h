#if !defined(__xmapfile_h)
#define __xmapfile_h

#include "xfile.h"
//#include <TCHAR.h>

class DLL_EXP CxMapFile : public CxFile
{
public:
    CxMapFile();
    ~CxMapFile();
    bool Create(const TCHAR * filename, size_t size);
    PBYTE GetBuffer() { return m_pBase; }
    virtual bool Close();
    virtual size_t	Read(void *buffer, size_t size, size_t count);
    //////////////////////////////////////////////////////////
    virtual size_t	Write(const void *buffer, size_t size, size_t count);
    virtual bool Seek(int32_t offset, int32_t origin);
    virtual int32_t Tell();
    virtual int32_t	Size();
    virtual bool	Flush();
    virtual bool	Eof();
    virtual int32_t	Error();
    virtual bool PutC(uint8_t c);
    virtual int32_t	GetC();
    virtual char *	GetS(char *string, int32_t n);
    virtual int32_t	Scanf(const char *format, void* output);
protected:
    HANDLE m_hFile;
    HANDLE m_hFileMapping;
    PBYTE  m_pBase;
    size_t m_maxSize;
    size_t m_nCurPos;
};

#endif
