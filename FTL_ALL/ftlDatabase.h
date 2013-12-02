
#ifndef FTL_DATABASE_H
#define FTL_DATABASE_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlDatabase.h requires ftlbase.h to be included first
#endif

//#define INCLUDE_SQLITE

namespace FTL
{
    FTLEXPORT class CFDatabase
    {
    public:
        FTLINLINE virtual ~CFDatabase(){};
    };

#ifdef INCLUDE_SQLITE
    #define SQLITE_VERIFY(x)\
        err = (x);\
        if(SQLITE_OK != err)\
        {\
            REPORT_ERROR_INFO(FTL::CFSQLiteErrorInfo, err, x);\
        }

    #define SQLITE_VERIFY_EXCEPT1(x, err1)\
        err = (x);\
        if(SQLITE_OK != err && err1 != err)\
        {\
            REPORT_ERROR_INFO(FTL::CFSQLiteErrorInfo, err, x);\
        }


    FTLEXPORT class CFSQLiteErrorInfo : public CFConvertInfoT<CFSQLiteErrorInfo, int>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFSQLiteErrorInfo);
    public:
        FTLINLINE explicit CFSQLiteErrorInfo(int err);
        FTLINLINE virtual LPCTSTR ConvertInfo();
    };

    class CFSQLite : public CFDatabase
    {
    public:
        FTLINLINE CFSQLite();
        FTLINLINE virtual ~CFSQLite();
    };
#endif //INCLUDE_SQLITE

}

#endif //FTL_DATABASE_H

#ifndef USE_EXPORT
#  include "ftlDatabase.hpp"
#endif
