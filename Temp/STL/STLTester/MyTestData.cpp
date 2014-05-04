#include "StdAfx.h"
#include "MyTestData.h"
#include <cppunit/extensions/HelperMacros.h>

LONG CMyTestData::s_TestDataCount = 0;
LONG CMyTestData::s_ConstructCount = 0;
LONG CMyTestData::s_CopyConstructCount = 0;
LONG CMyTestData::s_DestructCount = 0;
LONG CMyTestData::s_CompareEqualCount = 0;
LONG CMyTestData::s_CompareLessCount = 0;
LONG CMyTestData::s_AssignmentCount = 0;

CMyTestData::CMyTestData()
{
    m_Index = -1;
    m_OtherInfo = GetTickCount();
    m_bDumpInfo = TRUE;
    InterlockedIncrement(&s_TestDataCount);
    InterlockedIncrement(&s_ConstructCount);

    if (m_bDumpInfo)
    {
        TRACE(_T("CMyTestData default constructor , m_Index =%d,m_OtherInfo=%d, s_TestDataCount =%d\n"),m_Index,m_OtherInfo,s_TestDataCount);
    }
}

CMyTestData::CMyTestData(LONG index,BOOL bDumpInfo)
{
    m_Index = index;
    m_bDumpInfo = bDumpInfo;
    m_OtherInfo = GetTickCount();

    InterlockedIncrement(&s_TestDataCount);
    InterlockedIncrement(&s_ConstructCount);

    //TODO: Thread May Error
    if (m_bDumpInfo)
    {
        TRACE(_T("CMyTestData LONG constructor, m_Index =%d,m_OtherInfo=%d, s_TestDataCount =%d\n"),m_Index,m_OtherInfo,s_TestDataCount);
    }
}

CMyTestData::CMyTestData( const CMyTestData &copy )
{
    m_Index = copy.m_Index;
    m_bDumpInfo = copy.m_bDumpInfo;
    m_OtherInfo = GetTickCount();  //获取当前的时间值
    
    InterlockedIncrement(&s_TestDataCount);
    InterlockedIncrement(&s_CopyConstructCount);

    if (m_bDumpInfo)
    {
        TRACE(_T("CMyTestData copy constructor , m_Index =%d,m_OtherInfo=%d, s_TestDataCount =%d, s_CopyConstructCount=%d\n"),
            m_Index,m_OtherInfo,s_TestDataCount, s_CopyConstructCount);
    }
}

CMyTestData::~CMyTestData(void)
{
    InterlockedIncrement(&s_DestructCount);
    InterlockedDecrement(&s_TestDataCount);
    if (m_bDumpInfo)
    {
        TRACE(_T("CMyTestData::Destrocutr , m_Index =%d, m_OtherInfo=%d,s_TestDataCount =%d, s_DestructCount =%d\n"),
            m_Index,m_OtherInfo, s_TestDataCount, s_DestructCount);
    }
}

CMyTestData& CMyTestData::operator =( const CMyTestData &copy )
{
    InterlockedIncrement(&s_AssignmentCount);

    if (m_bDumpInfo)
    {
        TRACE(_T("CMyTestData::operator = , m_Index =%d, copy.m_Index=%d, s_AssignmentCount =%d\n"),
            m_Index, copy.m_Index , s_AssignmentCount);
    }
    if (&copy == this)
    {
        return *this;
    }
    this->m_Index = copy.m_Index;
    this->m_OtherInfo = GetTickCount();

    return *this;
}

bool CMyTestData::operator == (const CMyTestData & other)
{
    InterlockedIncrement(&s_CompareEqualCount);

    if (m_bDumpInfo)
    {
        TRACE(_T("CMyTestData::operator == , m_Index =%d, other.m_Index=%d, s_CompareEqualCount =%d\n"),
            m_Index, other.m_Index, s_CompareEqualCount);
    }
    if (&other == this)  //same object
    {
        return true;
    }
    //只比较 Index (不管 OtherInfo)
    return other.m_Index == this->m_Index;
}

bool CMyTestData::operator < (const CMyTestData & other) const
{
    InterlockedIncrement(&s_CompareLessCount);
    if (m_bDumpInfo)
    {
        TRACE(_T("CMyTestData::operator < , m_Index =%d, other.m_Index=%d, s_CompareLessCount =%d\n"),
            m_Index, other.m_Index, s_CompareLessCount);
    }

    return m_Index < other.m_Index;
}

LONG CMyTestData::LongTimeIncreatment(DWORD dwMilliseconds)
{
    TRACE(_T("%d Enter LongTimeIncreatment\n"), GetCurrentThreadId());
    LONG lOldValue = m_Index;
    Sleep(dwMilliseconds);
    if (m_Index != lOldValue)
    {
        TRACE(_T("CMyTestData::LongTimeIncreatment Index Changed,old=%d,now=%d\n"), lOldValue, m_Index);
    }
    m_Index = (lOldValue+1);
    TRACE(_T("%d Leave LongTimeIncreatment\n"), GetCurrentThreadId());
    return m_Index;
}

LONG CMyTestData::LongTimeDecreatment(DWORD dwMilliseconds)
{
    TRACE(_T("%d Enter LongTimeDecreatment\n"), GetCurrentThreadId());
    LONG lOldValue = m_Index;
    Sleep(dwMilliseconds);
    if (m_Index != lOldValue)
    {
        TRACE(_T("CMyTestData::LongTimeDecreatment Index Changed,old=%d,now=%d\n"), lOldValue, m_Index);
    }
    m_Index = (lOldValue-1);
    TRACE(_T("%d Leave LongTimeDecreatment\n"), GetCurrentThreadId());
    return m_Index;
}

void CMyTestData::DumpDataInfo()
{
    TRACE(_T("CMyTestData::DumpDataInfo,m_Index =%d,m_OtherInfo=%d, s_TestDataCount =%d\n"),m_Index,m_OtherInfo,s_TestDataCount);
}

LONG CMyTestData::GetTestDataCount()
{
    return s_TestDataCount;
}

LONG CMyTestData::GetConstructCount()
{
    return s_ConstructCount;
}

LONG CMyTestData::GetCopyConstrcutCount()
{
    return s_CopyConstructCount;
}

LONG CMyTestData::GetAssignmentCount()
{
    return s_AssignmentCount;
}

LONG CMyTestData::GetCompareEqualCount()
{
    return s_CompareEqualCount;
}

LONG CMyTestData::GetCompareLessCount()
{
    return s_CompareLessCount;
}

LONG CMyTestData::GetDestructCount()
{
    return s_DestructCount;
}

CMyTestDataCountChecker::CMyTestDataCountChecker(CheckType checkType, LONG diff, 
                                                 LPCSTR pszFile /* = __FILE__ */, 
                                                 DWORD dwLine /* = __LINE__ */)
{
    m_checkType = checkType;
    m_diff = diff;
    m_pszFile = pszFile;
    m_dwLine = dwLine;
    switch(m_checkType)
    {
    case ctDataCount:
        m_OrignalCount = CMyTestData::GetTestDataCount();
        break;
    case ctConstructCount:
        m_OrignalCount = CMyTestData::GetConstructCount();
        break;
    case ctCopyConstructCount:
        m_OrignalCount = CMyTestData::GetCopyConstrcutCount();
        break;
    case ctAssignmentCount:
        m_OrignalCount = CMyTestData::GetAssignmentCount();
        break;
    case ctCompareEqualCount:
        m_OrignalCount = CMyTestData::GetCompareEqualCount();
        break;
    case ctCompareLessCount:
        m_OrignalCount = CMyTestData::GetCompareLessCount();
        break;
    case ctDestructCount:
        m_OrignalCount = CMyTestData::GetDestructCount();
        break;
    default:
        ASSERT(FALSE);
        m_OrignalCount = 0;
        break;
    }
}

CMyTestDataCountChecker::~CMyTestDataCountChecker()
{
    LONG curCount = 0;
    switch(m_checkType)
    {
    case ctDataCount:
        curCount = CMyTestData::GetTestDataCount();
        break;
    case ctConstructCount:
        curCount = CMyTestData::GetConstructCount();
        break;
    case ctCopyConstructCount:
        curCount = CMyTestData::GetCopyConstrcutCount();
        break;
    case ctAssignmentCount:
        curCount = CMyTestData::GetAssignmentCount();
        break;
    case ctCompareEqualCount:
        curCount = CMyTestData::GetCompareEqualCount();
        break;
    case ctCompareLessCount:
        curCount = CMyTestData::GetCompareLessCount();
        break;
    case ctDestructCount:
        curCount = CMyTestData::GetDestructCount();
        break;
    default:
        ASSERT(FALSE);
        curCount = 0;
        break;
    }
    std::ostringstream os;
    os << m_pszFile <<"(" << m_dwLine << "):" 
        << m_checkType << ", want diff=" << m_diff
        << ", real diff=" << curCount - m_OrignalCount << std::endl;
    const std::string& rStr = os.str();
    if (m_OrignalCount + m_diff != curCount)
    {
        OutputDebugStringA(rStr.c_str());
    }
    CPPUNIT_ASSERT_MESSAGE(os.str(), m_OrignalCount + m_diff == curCount);
}
