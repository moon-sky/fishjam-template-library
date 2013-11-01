#ifndef FTL_REG_HPP
#define FTL_REG_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlReg.h"
#endif

namespace FTL
{
    typedef std::pair<HKEY, LPCTSTR> HKeyRoot2StringPair;
    /*__declspec(selectany) */ static HKeyRoot2StringPair s_RegRootStringPairs[] = {
        std::make_pair(HKEY_CLASSES_ROOT, TEXT("HKEY_CLASSES_ROOT")),
        std::make_pair(HKEY_CURRENT_USER, TEXT("HKEY_CURRENT_USER")),
        std::make_pair(HKEY_LOCAL_MACHINE, TEXT("HKEY_LOCAL_MACHINE")),
        std::make_pair(HKEY_USERS, TEXT("HKEY_USERS")),
        std::make_pair(HKEY_CURRENT_CONFIG, TEXT("HKEY_CURRENT_CONFIG")),
    };

    LPCTSTR CFRegUtil::ConvertRegRootKeyToString(HKEY hKeyRoot)
    {
        for (int i = 0; i < _countof(s_RegRootStringPairs); i++)
        {
            if (hKeyRoot == s_RegRootStringPairs[i].first)
            {
                return s_RegRootStringPairs[i].second;
            }
        }
        FTLTRACEEX(FTL::tlError, TEXT("Unknown RootKey:0x%x\n"), hKeyRoot);
        FTLASSERT(FALSE);
        return NULL;
    }

    HKEY CFRegUtil::ConvertStringToRegRootKey(LPCTSTR pszKeyRoot)
    {
        for (int i = 0; i < _countof(s_RegRootStringPairs); i++)
        {
            if (lstrcmpi(pszKeyRoot, s_RegRootStringPairs[i].second) == 0)
            {
                return s_RegRootStringPairs[i].first;
            }
        }
        FTLTRACEEX(FTL::tlError, TEXT("Unknown Root String:%s\n"), pszKeyRoot);
        FTLASSERT(FALSE);
        return NULL;
    }

    LONG CFRegUtil::GetRegValueExportString(HKEY hKey, LPCTSTR pszValueName, CAtlString& strResult, DWORD* pRegType)
    {
        LONG lRet = ERROR_SUCCESS;
        BOOL bRet= FALSE;
        DWORD dwRegType = REG_NONE;
        DWORD nBytesCount = 0;
        REG_VERIFY_EXCEPT1(::RegQueryValueEx(hKey, pszValueName, NULL, &dwRegType, NULL, &nBytesCount), ERROR_FILE_NOT_FOUND);
        if (ERROR_SUCCESS == lRet)
        {
            CFMemAllocator<BYTE> memValue(nBytesCount > 0 ? nBytesCount : 1);
            REG_VERIFY(::RegQueryValueEx(hKey, pszValueName, NULL, &dwRegType, memValue, &nBytesCount));
            if (ERROR_SUCCESS == lRet)
            {
                if (pRegType)
                {
                    *pRegType = dwRegType;
                }
                strResult.Empty();
                CAtlString strBinary;
                LONG nStrBinaryCount = 0;

                switch (dwRegType)
                {
                case REG_NONE:
                    strResult = _T("hex(0):");
                    break;
                case REG_SZ:
                    {
                        CAtlString strTemp;
                        strTemp.Format(TEXT("%s"), (LPCTSTR)memValue.GetMemory());
                        strTemp.Replace(TEXT("\\"), TEXT("\\\\"));  // \ => \\  
                        strTemp.Replace(TEXT("\""), TEXT("\\\""));  // " => \" 
                        strResult.Format(TEXT("\"%s\""), strTemp);
                    }
                    break;
                case REG_EXPAND_SZ:
                case REG_BINARY:
                case REG_MULTI_SZ:
                case REG_QWORD_LITTLE_ENDIAN:  //REG_QWORD
                    {
                        API_VERIFY(CFConvUtil::HexFromBinary(memValue.GetMemory(), nBytesCount, NULL, &nStrBinaryCount, _T(',')));
                        CFMemAllocator<TCHAR> bufString(nStrBinaryCount);
                        API_VERIFY(CFConvUtil::HexFromBinary(memValue.GetMemory(), nBytesCount, bufString.GetMemory(), &nStrBinaryCount, _T(',')));

                        if (REG_BINARY == dwRegType)
                        {
                            strResult = _T("hex:");
                        }
                        else
                        {
                            strResult.Format(TEXT("hex(%x):"), dwRegType);
                        }
                        strResult.Append(bufString.GetMemory());

                        break;
                    }
                case REG_DWORD_LITTLE_ENDIAN: //REG_DWORD:
                    {
                        strResult.Format(TEXT("dword:%08x"), *((DWORD*)memValue.GetMemory()));
                        break;
                    }
                case REG_DWORD_BIG_ENDIAN:
                case REG_LINK:
                case REG_RESOURCE_LIST:
                case REG_FULL_RESOURCE_DESCRIPTOR:
                case REG_RESOURCE_REQUIREMENTS_LIST:
                default:
                    FTLTRACEEX(tlWarning, TEXT("%s Unsupport RegType\n"), __FILE__LINE__);
                    FTLASSERT(FALSE);
                    lRet = ERROR_UNSUPPORTED_TYPE;
                    break;
                }
            }
        }
        return lRet;
    }

    LONG CFRegUtil::ExportRegValueToFile(LPCTSTR pszFullKey, 
        LPCTSTR pszValueName, 
        CFUnicodeFile* pRegFile, 
        DWORD flags,
        REGSAM samDesired)
    {
        FTLASSERT(pszFullKey);  //pszValue can be NULL
        FTLASSERT(pRegFile);
        
        LONG lRet = ERROR_FILE_NOT_FOUND;
        BOOL bRet = FALSE;
        //BOOL bWritePath = ((flags & WRITE_PATH_AUTO) != 0); 

        TCHAR szKeyRoot[256] = {0};  //255 + 1
        TCHAR szSubKey[256] = {0};

        lstrcpyn(szKeyRoot,  pszFullKey, _countof(szKeyRoot));
        LPTSTR pszSubKeyDiv = StrChr(szKeyRoot, TEXT('\\'));
        FTLASSERT(pszSubKeyDiv);
        if (pszSubKeyDiv)
        {
            lstrcpyn(szSubKey, pszSubKeyDiv + 1, _countof(szKeyRoot));
            pszSubKeyDiv[0] = NULL;

            HKEY hKeyRoot = ConvertStringToRegRootKey(szKeyRoot);
            FTLASSERT(hKeyRoot);
            if (hKeyRoot)
            {
                HKEY hkResult = NULL;
                REG_VERIFY(::RegOpenKeyEx(hKeyRoot, szSubKey, 0, samDesired, &hkResult));
                if (ERROR_SUCCESS == lRet)
                {
                    CAtlString strRegExport;
                    //if (bWritePath)
                    {
                        strRegExport.Format(TEXT("\r\n[%s]\r\n"), pszFullKey);
                        API_VERIFY(pRegFile->WriteString(strRegExport, NULL));
                    }

                    CAtlString strValueResult;
                    DWORD regType;
                    if (pszValueName && lstrlen(pszValueName) == 1 && *pszValueName == _T('*'))
                    {
                        if(ERROR_SUCCESS == GetRegValueExportString(hkResult, pszValueName, strValueResult, &regType))
                        {
                            FTLTRACE(TEXT("Export for %s\\%s\n"), pszFullKey, pszValueName);
                            return lRet;
                        }

                        DWORD    cSubKeys = 0;               // number of subkeys 
                        DWORD    cbMaxSubKeyLen = 0;         // longest subkey size 
                        //DWORD    cchMaxClass = 0;            // longest class string 
                        DWORD    cValues = 0;                // number of values for key 
                        DWORD    cchMaxValueLen = 0;         // longest value name 
                        DWORD    cbMaxValueLen = 0;          // longest value data 
                        //DWORD    cbSecurityDescriptor = 0;   // size of security descriptor 

                        REG_VERIFY(RegQueryInfoKey(hkResult, NULL, NULL, NULL, &cSubKeys, 
                            &cbMaxSubKeyLen, NULL,&cValues, &cchMaxValueLen, &cbMaxValueLen, 
                            NULL, NULL));
                        if (ERROR_SUCCESS == lRet)
                        {
                            DWORD dwValueNameLen = cchMaxValueLen + 1;
                            CFMemAllocator<TCHAR> valueName(dwValueNameLen);
                            //CFMemAllocator<BYTE>  valueData(cbMaxValueLen);
                            DWORD subFlags = flags;
                            for (DWORD indexValue = 0; indexValue < cValues; indexValue++)
                            {
                                dwValueNameLen = valueName.GetCount();
                                REG_VERIFY(RegEnumValue(hkResult, indexValue, valueName, &dwValueNameLen, NULL, 
                                    NULL, NULL, NULL));
                                    //&regType, valueData, &cbMaxValueLen));
                                if (ERROR_SUCCESS == lRet)
                                {
                                    //if (indexValue == 0)
                                    //{
                                    //    subFlags |= WRITE_PATH_AUTO;
                                    //}
                                    //else {
                                    //    subFlags &= ~WRITE_PATH_AUTO;
                                    //}
                                    REG_VERIFY(ExportRegValueToFile(pszFullKey, valueName, pRegFile, subFlags, samDesired));
                                }
                            }

                            if (flags & EXPORT_SUB_KEY)
                            {
                                DWORD dwSubKeyBufferLen = cbMaxSubKeyLen + 1;
                                FTL::CFMemAllocator<TCHAR> subKeyName(dwSubKeyBufferLen);
                                for (DWORD indexSubKey = 0; indexSubKey < cSubKeys; indexSubKey++)
                                {
                                    dwSubKeyBufferLen = subKeyName.GetCount();
                                    CAtlString strSubKeyFullPath = pszFullKey;
                                    REG_VERIFY(RegEnumKeyEx(hkResult, indexSubKey, subKeyName.GetMemory(), &dwSubKeyBufferLen, NULL, NULL, NULL, NULL));
                                    if (ERROR_SUCCESS == lRet)
                                    {
                                        strSubKeyFullPath.AppendFormat(TEXT("\\%s"), subKeyName.GetMemory());
                                        REG_VERIFY(ExportRegValueToFile(strSubKeyFullPath, _T("*"), pRegFile, flags, samDesired));
                                    }
                                }
                            }
                        }
                        //enum
                    }
                    else
                    {
                        REG_VERIFY(GetRegValueExportString(hkResult, pszValueName, strValueResult, &regType));
                        if (ERROR_SUCCESS == lRet)
                        {
                            if (pszValueName && lstrlen(pszValueName) > 0)
                            {
                                strRegExport.Format(TEXT("\"%s\"=%s\r\n"), pszValueName, strValueResult);
                            }
                            else
                            {
                                strRegExport.Format(TEXT("@=%s\r\n"), strValueResult);
                            }
                            API_VERIFY(pRegFile->WriteString(strRegExport, NULL));
                        }
                    }
                }
            }
        }
        return lRet;
    }

    CFRegSerialize::CFRegSerialize()
    {
        m_bWillQuit = FALSE;
        m_pRegFile = NULL;
    }
    CFRegSerialize::~CFRegSerialize()
    {
        CloseRegFile();
    }
    LONG CFRegSerialize::CreateRegFile(LPCTSTR pszFilePath)
    {
        FTLASSERT(NULL == m_pRegFile);

        BOOL bRet = TRUE;
        LONG lRet = ERROR_SUCCESS;
        if (NULL == m_pRegFile)
        {
            m_pRegFile = new CFUnicodeFile(tfeUnicode);
            API_VERIFY(m_pRegFile->Create(pszFilePath));
            if (!bRet)
            {
                lRet = GetLastError();
            }
            else
            {
                API_VERIFY(m_pRegFile->WriteFileHeader());
                API_VERIFY(m_pRegFile->WriteString(TEXT("Windows Registry Editor Version 5.00\r\n")));
            }
        }
        return lRet;
    }
    LONG CFRegSerialize::CloseRegFile()
    {
        LONG lRet = ERROR_SUCCESS;
        BOOL bRet = TRUE;
        if (m_pRegFile)
        {
            m_pRegFile->WriteString(TEXT("\r\n"));
            API_VERIFY(m_pRegFile->Flush());
            API_VERIFY(m_pRegFile->Close());
            delete m_pRegFile;
            m_pRegFile = NULL;
        }
        return lRet;
    }

    LONG CFRegSerialize::ExportReg(LPCTSTR pszFullKey, 
        LPCTSTR pszValueName, 
        DWORD flags, 
        REGSAM samDesired)
    {
        FTLASSERT(pszFullKey);  //pszValue can be NULL

        LONG lRet = ERROR_INVALID_PARAMETER;
        //BOOL bRet = FALSE;

        TCHAR szKeyRoot[256] = {0};  //255 + 1
        TCHAR szSubKey[256] = {0};

        lstrcpyn(szKeyRoot,  pszFullKey, _countof(szKeyRoot));
        LPTSTR pszSubKeyDiv = StrChr(szKeyRoot, TEXT('\\'));
        FTLASSERT(pszSubKeyDiv);
        if (pszSubKeyDiv)
        {
            lstrcpyn(szSubKey, pszSubKeyDiv + 1, _countof(szKeyRoot));
            pszSubKeyDiv[0] = NULL;

            HKEY hKeyRoot = CFRegUtil::ConvertStringToRegRootKey(szKeyRoot);
            FTLASSERT(hKeyRoot);
            if (hKeyRoot)
            {
                m_strRootKey = szKeyRoot;
                //m_strSubKey = szSubKey;

                REG_VERIFY(_ExportReg(hKeyRoot, szSubKey, pszValueName, flags, samDesired));
            }
        }

        return lRet;
    }

    LONG CFRegSerialize::_ExportReg(HKEY hKeyRoot, LPCTSTR pszSubKey, 
        LPCTSTR pszValueName, DWORD flags, REGSAM samDesired)
    {
        BOOL bRet = FALSE;
        LONG lRet = ERROR_SUCCESS;
        CAtlString strRegExport;

        HKEY hkResult = NULL;
        REG_VERIFY(::RegOpenKeyEx(hKeyRoot, pszSubKey, 0, samDesired, &hkResult));
        if (ERROR_SUCCESS == lRet)
        {
            strRegExport.Format(TEXT("\r\n[%s\\%s]\r\n"), m_strRootKey, pszSubKey);
            API_VERIFY(m_pRegFile->WriteString(strRegExport, NULL));

            if (_WillExportMultiValue(pszValueName, flags))
            {
                //需要导出多个值，进行枚举
                DWORD    cSubKeys = 0;               // number of subkeys 
                DWORD    cbMaxSubKeyLen = 0;         // longest subkey size 
                //DWORD    cchMaxClass = 0;            // longest class string 
                DWORD    cValues = 0;                // number of values for key 
                DWORD    cchMaxValueLen = 0;         // longest value name 
                DWORD    cbMaxValueLen = 0;          // longest value data 
                //DWORD    cbSecurityDescriptor = 0;   // size of security descriptor 

                REG_VERIFY(RegQueryInfoKey(hkResult, NULL, NULL, NULL, &cSubKeys, 
                    &cbMaxSubKeyLen, NULL,&cValues, &cchMaxValueLen, &cbMaxValueLen, 
                    NULL, NULL));
                if (ERROR_SUCCESS == lRet)
                {
                    if ((flags & EXPORT_MULTI_VALUE)
                        && (*pszValueName == _T('*'))
                        && (cValues > 0))
                    {
                        DWORD dwValueNameLen = cchMaxValueLen + 1;
                        CFMemAllocator<TCHAR> valueName(dwValueNameLen);
                        //CFMemAllocator<BYTE>  valueData(cbMaxValueLen);

                        //DWORD subFlags = flags;
                        for (DWORD indexValue = 0; 
                            indexValue < cValues && !m_bWillQuit; 
                            indexValue++)
                        {
                            dwValueNameLen = valueName.GetCount();
                            REG_VERIFY(RegEnumValue(hkResult, indexValue, valueName, &dwValueNameLen, NULL, 
                                NULL, NULL, NULL));
                            if (ERROR_SUCCESS == lRet)
                            {
                                _ExportReg(hkResult, valueName);
                            }
                        }
                    }

                    if ((flags & EXPORT_SUB_KEY) 
                        && (cSubKeys > 0))
                    {
                        DWORD dwSubKeyBufferLen = cbMaxSubKeyLen + 1;
                        FTL::CFMemAllocator<TCHAR> subKeyName(dwSubKeyBufferLen);
                        for (DWORD indexSubKey = 0; indexSubKey < cSubKeys; indexSubKey++)
                        {
                            dwSubKeyBufferLen = subKeyName.GetCount();
                            CAtlString strSubKeyFullPath = pszSubKey;
                            REG_VERIFY(RegEnumKeyEx(hkResult, indexSubKey, subKeyName.GetMemory(), &dwSubKeyBufferLen, NULL, NULL, NULL, NULL));
                            if (ERROR_SUCCESS == lRet)
                            {
                                strSubKeyFullPath.AppendFormat(TEXT("\\%s"), subKeyName.GetMemory());
                                REG_VERIFY(_ExportReg(hKeyRoot, strSubKeyFullPath, pszValueName, flags, samDesired));
                                //REG_VERIFY(_ExportReg(hKeyRoot, strSubKeyFullPath, flags, samDesired));
                                //REG_VERIFY(ExportRegValueToFile(strSubKeyFullPath, _T("*"), pRegFile, flags, samDesired));
                            }
                        }
                    }
                }
            }
            else
            {
                //export just current reg key
                REG_VERIFY(_ExportReg(hkResult, pszValueName));
            }

            REG_VERIFY(RegCloseKey(hkResult));
        }
        return lRet;
    }

    LONG CFRegSerialize::_ExportReg(HKEY hKey, LPCTSTR pszValueName)
    {
        BOOL bRet = FALSE;
        LONG lRet = ERROR_INVALID_PARAMETER;
        DWORD regType = REG_NONE;
        CAtlString strValueResult;
        CAtlString strRegExport;

        REG_VERIFY(CFRegUtil::GetRegValueExportString(hKey, pszValueName, strValueResult, &regType));
        if (ERROR_SUCCESS == lRet)
        {
            REG_VERIFY(_ConstructExportString(strRegExport, pszValueName, strValueResult, regType));
        }

        return lRet;
    }

    LONG CFRegSerialize::_ConstructExportString(CAtlString& strRegExport, LPCTSTR pszValueName, 
        const CAtlString& strValueResult, DWORD regType)
    {
        BOOL bRet = FALSE;
        INT nValueLen = 0;
        if (pszValueName && lstrlen(pszValueName) > 0)
        {
            CAtlString strValueName = pszValueName;
            strValueName.Replace(TEXT("\\"), TEXT("\\\\"));  // \ => \\  
            strValueName.Replace(TEXT("\""), TEXT("\\\""));  // " => \" 

            nValueLen = strValueName.GetLength();

            strRegExport.Format(TEXT("\"%s\"=%s\r\n"), strValueName, strValueResult);
        }
        else
        {
            strRegExport.Format(TEXT("@=%s\r\n"), strValueResult);
        }

        _LineWrapperBinaryString(strRegExport, nValueLen, regType);
        API_VERIFY(m_pRegFile->WriteString(strRegExport, NULL));
        return ERROR_SUCCESS;
    }

    BOOL CFRegSerialize::_LineWrapperBinaryString(CAtlString& strRegExport, INT nValueLen, DWORD regType)
    {
        BOOL bChange = TRUE;
        switch (regType)
        {
        case REG_NONE:
        case REG_EXPAND_SZ:
        case REG_BINARY:
        case REG_MULTI_SZ:
        case REG_QWORD_LITTLE_ENDIAN:  //REG_QWORD
            {
                const INT MAX_LINE_WRAPPER_COUNT = 78;
                const INT FIRST_LONG_VALUE_OFFSET = 12; // lstrlen(_T("=hex(7):4e,")) + 1

                BOOL bWrapped = FALSE;
                //INT nWrapperSize = nValueLen + FIRST_LONG_VALUE_OFFSET > MAX_LINE_WRAPPER_COUNT 
                //    ? nValueLen + FIRST_LONG_VALUE_OFFSET - 1
                //    : MAX_LINE_WRAPPER_COUNT;
                INT nWrapperSize = MAX_LINE_WRAPPER_COUNT;
                if (nValueLen + FIRST_LONG_VALUE_OFFSET > MAX_LINE_WRAPPER_COUNT)
                {
                    nWrapperSize = nValueLen + FIRST_LONG_VALUE_OFFSET - 1;
                }
                else if (nValueLen + FIRST_LONG_VALUE_OFFSET == MAX_LINE_WRAPPER_COUNT)
                {
                    nWrapperSize = MAX_LINE_WRAPPER_COUNT + 2;
                }

                CAtlString strTemp;
                while (strRegExport.GetLength() > nWrapperSize) //回车换行
                {
                    BOOL bSameLine = FALSE;
                    TCHAR chLast = _T('\0');
                    while (nWrapperSize > 0 && ((chLast = strRegExport.GetAt(nWrapperSize)) != _T(',')))
                    {
                        if (chLast == _T('\r') || chLast == _T('\n'))
                        {
                            bSameLine = TRUE;
                            break;
                        }
                        nWrapperSize--;
                    }
                    strTemp += strRegExport.Left(nWrapperSize + 1);
                    if (!bSameLine)
                    {
                        strTemp += _T("\\\r\n  ");
                    }
                    strRegExport.Delete(0, nWrapperSize + 1);
                    nWrapperSize = MAX_LINE_WRAPPER_COUNT - 2;
                    
                    bWrapped = TRUE;
                }
                if (strRegExport.GetLength() > 0)
                {
                    strTemp += strRegExport.Left(nWrapperSize);
                }
                if (bWrapped)
                {
                    strRegExport = strTemp;
                }

                break;
            }
        }
        return bChange;
    }

    BOOL CFRegSerialize::_WillExportMultiValue(LPCTSTR pszValueName, DWORD flags)
    {
        BOOL bRet = FALSE;
        
        //if (EXPORT_SUB_KEY == (flags & EXPORT_SUB_KEY))
        //{
        //    bRet = TRUE;
        //}

        //if (!bRet)
        {
            if (pszValueName 
                && (_T('*') == *pszValueName)
                && (EXPORT_MULTI_VALUE ==(flags & EXPORT_MULTI_VALUE))
                )
            {
                bRet = TRUE;
            }
        }


        return bRet;
    }
}
#endif //FTL_REG_HPP