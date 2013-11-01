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
        EXPORT_SUB_KEY       = 0x0010,          //递归遍历子树
        EXPORT_MULTI_VALUE   = 0x0020,          //支持 pszValueName 指定 * 导出多个值

        EXPORT_LINEWAPPER_BINARY = 0x0100,      //对二进制进行格式化
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

        //获取键值导出时对应的格式字符串
        FTLINLINE static LONG GetRegValueExportString(HKEY hKey, LPCTSTR pszValueName, CAtlString& strResult, DWORD* pRegType);
    };

    class IRegSerializeCallback
    {
    public:
        //TODO
    };

    //以 .reg 的文本格式 导出注册表的内容
    //  注意：不能采用 CRegKey 的子类方式实现(利用m_hKey) -- 其没有保存注册表路径
    class CFRegSerialize
    {
    public:
        FTLINLINE CFRegSerialize();
        FTLINLINE ~CFRegSerialize();
        FTLINLINE LONG CreateRegFile(LPCTSTR pszFilePath);
        FTLINLINE LONG CloseRegFile();

        FTLINLINE LONG ExportReg(LPCTSTR pszFullKey, LPCTSTR pszValueName = _T("*"), 
            DWORD flags = EXPORT_SUB_KEY | EXPORT_MULTI_VALUE,
            REGSAM samDesired = KEY_READ | KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE);
        
    protected:
        FTLINLINE LONG _ExportReg(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValueName, DWORD flags, REGSAM samDesired);
        FTLINLINE LONG _ExportReg(HKEY hKey, LPCTSTR pszValueName, DWORD flags);
        FTLINLINE LONG _ConstructExportString(CAtlString& strRegExport, LPCTSTR pszValueName, 
            const CAtlString& strValueResult, DWORD regType, DWORD flags);
        BOOL _LineWrapperBinaryString(CAtlString& strRegExport, INT nValueLen, DWORD regType);

        FTLINLINE BOOL _WillExportMultiValue(LPCTSTR pszValueName, DWORD flags);

        volatile BOOL  m_bWillQuit;
        CAtlString  m_strRootKey;
        //CAtlString  m_strSubKey;
        CFUnicodeFile* m_pRegFile;
    };
}
#endif //FTL_REG_H

#ifndef USE_EXPORT
#  include "ftlReg.hpp"
#endif 