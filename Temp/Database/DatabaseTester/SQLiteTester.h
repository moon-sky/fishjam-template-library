#pragma once
#include "sqlite3.h"
//#include "sqlite3ext.h"

/*****************************************************************************************************
* 
*****************************************************************************************************/

class CSQLiteTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CSQLiteTester );
    CPPUNIT_TEST( test_InsertRecord );
    CPPUNIT_TEST( test_InsertBlobRecord );
    CPPUNIT_TEST( test_selectRecord );
    CPPUNIT_TEST( test_MultiThreadRead );
    CPPUNIT_TEST( test_MultiThreadWrite );

    CPPUNIT_TEST_SUITE_END();

public:
    CSQLiteTester();
    ~CSQLiteTester();
    virtual void setUp();
    virtual void tearDown();
private:
    void openDatabase();
    void closeDatabase();
    void createTable(sqlite3* pSqliteDB);
    void createIndex(sqlite3* pSqliteDB);
    void dropTable(sqlite3* pSqliteDB);
    

    LONG insertRecord(sqlite3* pSqliteDB, bool useTransact, int nStartIndex, int nEndIndex);
    LONG insertBlobRecord(sqlite3* pSqliteDB, bool useTransact, int nStartIndex, int nEndIndex);

    //select * from tabel_name limit 30 offset 30; //跳过前面30条，读取后面30条记录
    LONG selectRecord(sqlite3* pSqliteDB, int id);
    LONG selectRecord(sqlite3* pSqliteDB, const std::string& name);

    //DECLARE_DEFAULT_TEST_CLASS(CSQLiteTester);
    static unsigned int __stdcall InsertDBThreadProc(void *pParam);
    static const char*  s_DBName;

    void test_InsertRecord();
    void test_InsertBlobRecord();
    void test_selectRecord();

    void test_MultiThreadRead();
    void test_MultiThreadWrite();

    sqlite3*    m_pSqliteDB;
};