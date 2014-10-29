#include "xmapfile.h"

//////////////////////////////////////////////////////////
CxMapFile::CxMapFile()
{
    m_hFile = INVALID_HANDLE_VALUE;
    m_hFileMapping = NULL;
    m_pBase = NULL;
    m_maxSize = 0;
    m_nCurPos = 0;
}

CxMapFile::~CxMapFile()
{
    Close();
}

bool CxMapFile::Create(const TCHAR * filename, size_t size)
{
    if (m_hFileMapping) return false;	// Can't re-open without closing first

    if (filename)
    {
        m_hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    }

    m_hFileMapping = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, size, NULL);
    if (m_hFileMapping)
    {
        m_pBase = (PBYTE)::MapViewOfFile(m_hFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
    }
    
    if (!m_pBase)
    {
        return false;
    }
    m_maxSize = size;
    m_nCurPos = 0;
    return true;
}

bool CxMapFile::Close()
{
    if (m_pBase)
    {
        ::UnmapViewOfFile(m_pBase);
        m_pBase = NULL;
    }
    if (m_hFileMapping)
    {
        CloseHandle(m_hFileMapping);
        m_hFileMapping = NULL;
    }
    if (INVALID_HANDLE_VALUE != m_hFile)
    {
        CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
    }
    m_nCurPos = 0;
    m_maxSize = 0;
    return true;
}
//////////////////////////////////////////////////////////
size_t CxMapFile::Read(void *buffer, size_t size, size_t count)
{
    if (!m_pBase) return 0;
    size_t sizeRead = size * count;
    if (sizeRead > m_maxSize - m_nCurPos)
    {
        sizeRead = m_maxSize - m_nCurPos;
    }
    if (sizeRead != 0)
    {
        CopyMemory(buffer, m_pBase + m_nCurPos, sizeRead);
        m_nCurPos += sizeRead;
        return sizeRead;
    }
    else
    {
        return EOF;
    }
}
//////////////////////////////////////////////////////////
size_t CxMapFile::Write(const void *buffer, size_t size, size_t count)
{
    if (!m_pBase) return 0;
    size_t sizeWrite = size * count;
    if (sizeWrite > m_maxSize - m_nCurPos)
    {
        sizeWrite = m_maxSize - m_nCurPos;
    }
    if (sizeWrite != 0)
    {
        CopyMemory(m_pBase + m_nCurPos, buffer, sizeWrite);
        m_nCurPos += sizeWrite;
        return sizeWrite;
    }
    else
    {
        return EOF;
    }
}
//////////////////////////////////////////////////////////
bool CxMapFile::Seek(int32_t offset, int32_t origin)
{
    if (!m_pBase) return false;
    size_t newPos = m_nCurPos;

    switch (origin)
    {
    case SEEK_SET:
        newPos = offset;
    case SEEK_CUR:
        m_nCurPos += offset;
        break;
    case SEEK_END:
        newPos += offset;
        break;
    default:
        return false;
    }
    if (newPos >= 0 && newPos < m_maxSize)
    {
        m_nCurPos = newPos;
        return true;
    }

    return false;
}
//////////////////////////////////////////////////////////
int32_t CxMapFile::Tell()
{
    if (!m_pBase) return 0;
    return m_nCurPos;
}
//////////////////////////////////////////////////////////
int32_t CxMapFile::Size()
{
    return m_maxSize;
}
//////////////////////////////////////////////////////////
bool CxMapFile::Flush()
{
    return true;
}
//////////////////////////////////////////////////////////
bool CxMapFile::Eof()
{
    if (m_nCurPos == m_maxSize)
    {
        return true;
    }
    return false;
}
//////////////////////////////////////////////////////////
int32_t CxMapFile::Error()
{
    if (!m_pBase) return -1;
    return 0;
}
//////////////////////////////////////////////////////////
bool CxMapFile::PutC(uint8_t c)
{
    return (Write(&c, 1, 1) != EOF);
}
//////////////////////////////////////////////////////////
int32_t CxMapFile::GetC()
{
    char chRead = EOF;
    Read(&chRead, 1, 1);
    return chRead;
}
//////////////////////////////////////////////////////////
char *	CxMapFile::GetS(char *string, int32_t n)
{
    Read(string, 1, n);
    return string;
}
//////////////////////////////////////////////////////////
int32_t	CxMapFile::Scanf(const char *format, void* output)
{
    cx_throw("Not Impl");
    //if (!m_fp) return EOF;
    //return fscanf(m_fp, format, output);
}
