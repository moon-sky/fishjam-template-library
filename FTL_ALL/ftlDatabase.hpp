#ifndef FTL_DATABASE_HPP
#define FTL_DATABASE_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlDatabase.h"
#endif

#ifdef INCLUDE_SQLITE
#  include "sqlite3.h"
#endif

namespace FTL
{
#ifdef INCLUDE_SQLITE
    CFSQLiteErrorInfo::CFSQLiteErrorInfo(int err) : CFConvertInfoT<CFSQLiteErrorInfo,int>(err)
    {
    }

    LPCTSTR CFSQLiteErrorInfo::ConvertInfo()
    {
        if (NULL == m_bufInfo[0])
        {
            switch (m_Info)
            {
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_ERROR);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_INTERNAL);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_PERM);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_ABORT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_BUSY);       //数据库文件被锁？
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_LOCKED);     //数据库中一个表被锁？
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_NOMEM);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_READONLY);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_INTERRUPT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_IOERR);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_CORRUPT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_NOTFOUND);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_FULL);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_CANTOPEN);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_PROTOCOL);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_EMPTY);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_SCHEMA);     //数据库模式改变
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_TOOBIG);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_CONSTRAINT); //由于约束冲突而终止
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_MISMATCH);   //数据类型不匹配
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_MISUSE);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_NOLFS);      //使用主机操作系统不支持的特性
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_AUTH);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_FORMAT);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_RANGE);      //2nd parameter to sqlite3_bind out of range
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_ROW);        //sqlite3_step还有查出的行
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),SQLITE_DONE);       //sqlite3_step执行结束
                //HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo),CPPSQLITE_ERROR);
            default:
                StringCchPrintf(m_bufInfo,_countof(m_bufInfo),TEXT("UnKnown[%d]"), m_Info);
                break;
            }
        }
        return m_bufInfo;
    }

#endif //INCLUDE_SQLITE
}

#endif //FTL_DATABASE_HPP