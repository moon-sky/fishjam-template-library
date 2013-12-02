#include "stdafx.h"
#include "SQLiteTester.h"
#include <process.h>

#include <ftlBase.h>
#define INCLUDE_SQLITE
#include <ftlDatabase.h>
#include <ftlFunctional.h>
using namespace FTL;


const char* CSQLiteTester::s_DBName = "testDB.db";

CSQLiteTester::CSQLiteTester()
{
    m_pSqliteDB = NULL;
};

CSQLiteTester::~CSQLiteTester()
{
    ASSERT(NULL == m_pSqliteDB);
};

void CSQLiteTester::setUp()
{
    openDatabase();
}

void CSQLiteTester::tearDown()
{
    closeDatabase();
}

void CSQLiteTester::openDatabase()
{
    ASSERT(NULL == m_pSqliteDB);
    int err = SQLITE_OK;
    SQLITE_VERIFY(sqlite3_open("testDB.db", &m_pSqliteDB));
}

void CSQLiteTester::closeDatabase()
{
    ASSERT(NULL != m_pSqliteDB);

    int err = SQLITE_ERROR;
    SQLITE_VERIFY(sqlite3_close(m_pSqliteDB));
    m_pSqliteDB = NULL;
}

void CSQLiteTester::createTable(sqlite3* pSqliteDB)
{
    FUNCTION_BLOCK_TRACE(500);
    //CFElapseCounter counter;

    int err = SQLITE_OK;

    //如果出错，表明表没有被删掉
    SQLITE_VERIFY_EXCEPT1(sqlite3_exec(pSqliteDB,
        "CREATE TABLE tlb_Students(ID INTEGER, name varchar(10), sex varchar(6), age varchar(2));",
        NULL, NULL, NULL), SQLITE_ERROR);

    SQLITE_VERIFY_EXCEPT1(sqlite3_exec(pSqliteDB,
        "CREATE TABLE tbl_BlobInfo(ID INTEGER, blobValue BLOB);",
        NULL, NULL, NULL), SQLITE_ERROR);

    //SQLITE_VERIFY_EXCEPT1(sqlite3_exec(pSqliteDB,
    //    "CREATE TABLE teachers(ID INTEGER, name varchar(10), sex varchar(6), age varchar(2));",
    //    NULL, NULL, NULL), SQLITE_ERROR);

    //return (counter.GetElapseTime() / NANOSECOND_PER_MILLISECOND);
}

void CSQLiteTester::dropTable(sqlite3* pSqliteDB)
{
    FUNCTION_BLOCK_TRACE(500);
    //CFElapseCounter counter;

    int err = SQLITE_OK;
    //SQLITE_VERIFY(sqlite3_exec(pSqliteDB, "DROP table tlb_Students", NULL, NULL, NULL));
    //SQLITE_VERIFY(sqlite3_exec(pSqliteDB, "DROP table tbl_BlobInfo", NULL, NULL, NULL));
    //SQLITE_VERIFY(sqlite3_exec(pSqliteDB, "DROP table teachers", NULL, NULL, NULL));

    //return (counter.GetElapseTime() / NANOSECOND_PER_MILLISECOND);
}

void CSQLiteTester::createIndex(sqlite3* pSqliteDB)
{
    FUNCTION_BLOCK_TRACE(500);
    //CFElapseCounter counter;

    int err = SQLITE_OK;
    SQLITE_VERIFY(sqlite3_exec(pSqliteDB, "create index idxStudentId on tlb_Students( ID )", NULL, NULL, NULL));
    SQLITE_VERIFY(sqlite3_exec(pSqliteDB, "create index idxStudentName on tlb_Students( Name )", NULL, NULL, NULL));
    //SQLITE_VERIFY(sqlite3_exec(pSqliteDB, "create index idxTeacherId on teachers( ID )", NULL, NULL, NULL));

    //return (counter.GetElapseTime() / NANOSECOND_PER_MILLISECOND);
}

LONG CSQLiteTester::insertRecord(sqlite3* pSqliteDB, bool useTransact, int nStartIndex, int nEndIndex)
{
    int err = SQLITE_OK;

    FUNCTION_BLOCK_TRACE(3000);
    CFElapseCounter counter;
    if (useTransact)
    {
        SQLITE_VERIFY(sqlite3_exec(pSqliteDB, "begin", NULL, NULL, NULL));
    }
    std::vector<int> randArray;
    FTL::GetRandomArray(nStartIndex, nEndIndex, randArray);

    for (int i = nStartIndex; i <= nEndIndex; i++)
    {
        std::ostringstream os;
        os << "INSERT INTO tlb_Students VALUES(" << randArray[i - nStartIndex ] 
            << ",'Name" << randArray[i - nStartIndex] <<"','male'," << randArray[i - nStartIndex] % 100 <<")";
        std::string strSQL = os.str();
        SQLITE_VERIFY(sqlite3_exec(pSqliteDB, strSQL.c_str(), NULL, NULL, NULL));
    }
    if (useTransact)
    {
        SQLITE_VERIFY(sqlite3_exec(pSqliteDB, "commit", NULL, NULL, NULL));
    }
    return (counter.GetElapseTime() / NANOSECOND_PER_MILLISECOND);
}

LONG CSQLiteTester::insertBlobRecord(sqlite3* pSqliteDB, bool useTransact, int nStartIndex, int nEndIndex)
{
    int err = SQLITE_OK;

    unsigned char blobBuf[256] = {0};
    for (unsigned char i = 1; i < 255; i++)
    {
        blobBuf[i] = i;
    }

    FUNCTION_BLOCK_TRACE(3000);
    CFElapseCounter counter;
    if (useTransact)
    {
        SQLITE_VERIFY(sqlite3_exec(pSqliteDB, "begin", NULL, NULL, NULL));
    }
    sqlite3_stmt* pStmt = NULL;
    SQLITE_VERIFY(sqlite3_prepare( pSqliteDB, "INSERT INTO tbl_BlobInfo( ID, blobValue) values( ?, ? )", -1, &pStmt,NULL )); 

    for (int i = nStartIndex; i < nEndIndex; i++)
    {
        //char *sql = "select @pa1, @pa2, @pa3 where 0>1; update qin_country set values1 = @pa1 where group_name = @pa2; 
        //  update qin_country set values2 = @pa3 where group_name = @pa2; ";

        //index1 = sqlite3_bind_parameter_index(stmt, "@pa1");
        //ret = sqlite3_bind_blob(stmt, index1, value, strlen((char*)value), SQLITE_STATIC);

        SQLITE_VERIFY(sqlite3_bind_int(pStmt, 0, i ));
        SQLITE_VERIFY(sqlite3_bind_blob(pStmt, 1, blobBuf, _countof(blobBuf), NULL));
        SQLITE_VERIFY_EXCEPT1(sqlite3_step( pStmt ), SQLITE_DONE);
    }
    if (useTransact)
    {
        SQLITE_VERIFY(sqlite3_exec(pSqliteDB, "commit", NULL, NULL, NULL));
    }
    return (counter.GetElapseTime() / NANOSECOND_PER_MILLISECOND);

}


LONG CSQLiteTester::selectRecord(sqlite3* pSqliteDB, int id)
{
    FUNCTION_BLOCK_TRACE(100);

    int err = SQLITE_OK;
    std::ostringstream os;
    os << "SELECT ID,name,sex,age from tlb_Students where ID=" << id;
    std::string strSQL = os.str();
    sqlite3_stmt* pStmp = NULL;

    CFElapseCounter counter;

    SQLITE_VERIFY(sqlite3_prepare(pSqliteDB, strSQL.c_str(), -1, &pStmp, NULL));
    ASSERT(pStmp);
    if (pStmp)
    {
        while((err = sqlite3_step(pStmp)) == SQLITE_ROW)
        {
            int queryId = sqlite3_column_int(pStmp,0);
            CPPUNIT_ASSERT(queryId == id);
        }
        sqlite3_finalize(pStmp);
        pStmp = NULL;
    }

    return (counter.GetElapseTime() / NANOSECOND_PER_MILLISECOND);
}

LONG CSQLiteTester::selectRecord(sqlite3* pSqliteDB, const std::string& name)
{
    FUNCTION_BLOCK_TRACE(100);

    int err = SQLITE_OK;
    std::ostringstream os;
    os << "SELECT ID,name,sex,age from tlb_Students where name ='" << name << "'";
    std::string strSQL = os.str();
    sqlite3_stmt* pStmp = NULL;

    CFElapseCounter counter;
    SQLITE_VERIFY(sqlite3_prepare(pSqliteDB, strSQL.c_str(), -1, &pStmp, NULL));
    ASSERT(pStmp);
    if (pStmp)
    {
        while((err = sqlite3_step(pStmp)) == SQLITE_ROW)
        {
            std::string queryValue = (const char*)sqlite3_column_text(pStmp,1);
            CPPUNIT_ASSERT(queryValue == name);
        }
        sqlite3_finalize(pStmp);
        pStmp = NULL;
    }

    return (counter.GetElapseTime() / NANOSECOND_PER_MILLISECOND);
}


void CSQLiteTester::test_InsertRecord()
{
    //是否使用事务，在插入时有天壤之别：
    //不加索引时
    //  1000条记录，调试版本：useTimeWithNoTran=138707(ms), useTimeWithTran=469(ms)
    //  1000条记录，发布版本：useTimeWithNoTran=95219(ms), useTimeWithTran=210(ms)
    //  100 条记录，发布版本：useTimeWithNoTran=12584(ms), useTimeWithTran=104(ms)

    //加索引时 -- 似乎差不多(10W条 无索引插入9451ms，有索引插入 10958ms)
    //  100 条记录，发布版本：useTimeWithIndex=139(ms)

    const int INSERT_COUNT = 10000;
    LONG tInsertWithNoTran = 0;
    LONG tInsertWithTran = 0;
    LONG tInsertWithTranIndex = 0;

    {
        ////不加索引，不用事务
        //createTable(m_pSqliteDB);
        //tInsertWithNoTran = insertRecord(m_pSqliteDB, false, 1, INSERT_COUNT);
        //tInsertWithNoTran += insertRecord(m_pSqliteDB, false, 1 + INSERT_COUNT, 2 * INSERT_COUNT);
        //dropTable(m_pSqliteDB);

        //不加索引，使用事务
        createTable(m_pSqliteDB);
        tInsertWithTran = insertRecord(m_pSqliteDB, true, 1, INSERT_COUNT);
        tInsertWithTran += insertRecord(m_pSqliteDB, true, 1 + INSERT_COUNT, 2 * INSERT_COUNT);
        dropTable(m_pSqliteDB);

        //加索引，使用事务
        createTable(m_pSqliteDB);
        createIndex(m_pSqliteDB);
        tInsertWithTranIndex = insertRecord(m_pSqliteDB, true,1, INSERT_COUNT);
        tInsertWithTranIndex += insertRecord(m_pSqliteDB, true,1 +INSERT_COUNT, 2 * INSERT_COUNT);
        dropTable(m_pSqliteDB);


        FTLTRACE(TEXT("INSERT_COUNT=%d, tInsertWithNoTran=%d(ms), tInsertWithTran=%d(ms), tInsertWithTranIndex=%d(ms)\n"), 
            INSERT_COUNT,
            tInsertWithNoTran, tInsertWithTran, tInsertWithTranIndex);

        //不加事务时的用时远远高于加上事务的用时
        if(tInsertWithNoTran > 0)
        {
            //加判断是为了方便测试大数据量时直接屏蔽掉 不用事务的代码
            CPPUNIT_ASSERT( tInsertWithNoTran / 10 > tInsertWithTran);
        }
    }
}

void CSQLiteTester::test_InsertBlobRecord()
{
    createTable(m_pSqliteDB);
    insertBlobRecord(m_pSqliteDB,true, 1, 100);
    dropTable(m_pSqliteDB);

    //sqlite3_bind_blob
}


void CSQLiteTester::test_selectRecord()
{
    //是否添加索引，在查询时似乎没有多大差别 -- 1w条数据，查询中间的5000，时间在几毫秒内
    const int RECORD_COUNT = 10000;

    LONG tSelWithNoIndexInteger = 0;
    LONG tSelWithNoIndexString = 0;
    LONG tSelWithIndexInteger = 0;
    LONG tSelWithIndexString = 0;
    std::ostringstream os;
    os << "name" << (RECORD_COUNT / 2);
    std::string strQueryName = os.str();


    //不加索引
    createTable(m_pSqliteDB);
    insertRecord(m_pSqliteDB, true, 1, RECORD_COUNT);
    tSelWithNoIndexInteger = selectRecord(m_pSqliteDB, RECORD_COUNT / 2);
    tSelWithNoIndexString = selectRecord(m_pSqliteDB, strQueryName);
    dropTable(m_pSqliteDB);

    //加索引
    createTable(m_pSqliteDB);
    createIndex(m_pSqliteDB);
    insertRecord(m_pSqliteDB, true, 1, RECORD_COUNT);
    tSelWithIndexInteger = selectRecord(m_pSqliteDB, RECORD_COUNT / 2);
    tSelWithIndexString = selectRecord(m_pSqliteDB, strQueryName);
    dropTable(m_pSqliteDB);

    FTLTRACE(TEXT("RECORD_COUNT=%d, tSelWithNoIndexInteger=%d(ms),tSelWithNoIndexString=%d(ms),tSelWithIndexInteger=%d(ms),tSelWithIndexString=%d\n"), 
        RECORD_COUNT,tSelWithNoIndexInteger, tSelWithNoIndexString, tSelWithIndexInteger, tSelWithIndexString);
}

unsigned int __stdcall CSQLiteTester::InsertDBThreadProc(void *pParam)
{
    int err = SQLITE_OK;

    CSQLiteTester* pThis = static_cast<CSQLiteTester*>(pParam);
    sqlite3* pSqlitDB = pThis->m_pSqliteDB;

    BOOL bCreateDB = FALSE;
    if (NULL == pSqlitDB)
    {
        SQLITE_VERIFY(sqlite3_open(s_DBName, &pSqlitDB));
        bCreateDB = TRUE;
    }
    int Type = (int)pParam;
    //SQLITE_VERIFY();

    for (int i = 0; i < 5; ++i)
    {
        if (Type == 0)
        {
            err = sqlite3_exec(pSqlitDB, "INSERT INTO tlb_Students VALUES('1111', 'Mary', 'female', '15')", 
                NULL,NULL,NULL);
        }
        else
        {
            ASSERT(FALSE);
            //err = sqlite3_exec(pSqlitDB, "INSERT INTO teachers VALUES('1111', 'Mary', 'female', '15')", 
            //    NULL,NULL,NULL);
        }
        if (err != SQLITE_OK)
        {
            TRACE("insert error :%d\n", err);
        }
    }
    //SQLITE_VERIFY(sqlite3_close(g_pSqlitDB));
    return 0;
}

void CSQLiteTester::test_MultiThreadRead()
{

}

void CSQLiteTester::test_MultiThreadWrite()
{
    openDatabase();

    const int THREAD_COUNT = 2;
    HANDLE hThreads[THREAD_COUNT] = {0};

    for (int i = 0; i < _countof(hThreads); ++i)
    {
        hThreads[i] = (HANDLE)_beginthreadex(NULL, 0, InsertDBThreadProc, this, 0, NULL);
    }

    WaitForMultipleObjects(_countof(hThreads), hThreads, TRUE, INFINITE);
    //SQLITE_VERIFY(sqlite3_prepare(pDB,"select * from tbl_test", -1, &pSTMT, NULL));

    closeDatabase();
}


