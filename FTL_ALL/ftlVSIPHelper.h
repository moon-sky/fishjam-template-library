#ifndef FTL_VSIP_HELPER_H
#define FTL_VSIP_HELPER_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlVSIPHelper.h requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlDTEHelper requires ftlCom.h to be included first
#endif

/********************************************************************************************

********************************************************************************************/

namespace FTL
{
    class CFVSIPUtils
    {
    public:

        //获取 cmdidcmd.h 中定义的 CmdID 对应的字符串，在 IOleCommandTarget::QueryStatus 中判断 cmds[n].cmdID
        FTLINLINE static LPCTSTR GetStdIdCommandtring(ULONG cmdID);
    };

}//namespace FTL

#ifndef USE_EXPORT
#  include "ftlVSIPHelper.hpp"
#endif

#endif //FTL_VSIP_HELPER_H
