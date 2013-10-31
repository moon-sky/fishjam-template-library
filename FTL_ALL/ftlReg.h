#ifndef FTL_REG_H
#define FTL_REG_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlReg.h requires ftlbase.h to be included first
#endif

#include "ftlFile.h"
/*************************************************************************************************************************
*************************************************************************************************************************/


namespace FTL
{
    enum RegExportFlags
    {
        WRITE_PATH_NONE     = 0x0000,
        WRITE_PATH_AUTO     = 0x0001,       //用于 pszValueName 为 *时
        
        WRITE_SUB_KEY       = 0x0010
    };

    class CFRegUtil
    {
    public:
        FTLINLINE static LPCTSTR ConvertRegRootKeyToString(HKEY hKeyRoot);
        FTLINLINE static HKEY ConvertStringToRegRootKey(LPCTSTR pszKeyRoot);

        //RegNotifyChangeKeyValue -- 检测注册表的变化，可以快速响应
        //RegCreateKeyEx -- 创建注册表项，如果存在的话则打开
        //RegQueryValueEx -- 获取 字符串(REG_SZ)、DWORD(REG_DWORD) 注册表的 类型 和值
        //RegSetValueEx -- 设置值，如果lpSubKey为NULL或""，则设置默认值
        //RegOpenKeyEx -- 打开注册表
        //RegEnumKeyEx -- 枚举子键，可用于删除、查找子键等，没有更多时返回 ERROR_NO_MORE_ITEMS
        //RegDeleteKey -- 删除没有下级子键的子键,如果要递归删除，可以使用 CRegKey::RecurseDeleteKey 
        //RegSaveKey/RegSaveKeyEx -- 以二进制方式导出注册表内容
        FTLINLINE static LONG GetRegValueExportString(HKEY hKey, LPCTSTR pszValueName, CAtlString& strResult, DWORD* pRegType);

        //以 .reg 的文本格式 导出注册表的内容
        //  注意：不能采用 CRegKey 的子类方式实现(利用m_hKey) -- 其没有保存注册表路径
        FTLINLINE static LONG ExportRegValueToFile(LPCTSTR pszFullKey, 
            LPCTSTR pszValueName, 
            CFUnicodeFile* pRegFile, 
            DWORD flags = WRITE_PATH_AUTO | WRITE_SUB_KEY,
            REGSAM samDesired = KEY_READ | KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE);

        //FTLINLINE static LONG EnumRegKeyHandle(HKEY hKey, )
    };

}
#endif //FTL_REG_H

#ifndef USE_EXPORT
#  include "ftlReg.hpp"
#endif 