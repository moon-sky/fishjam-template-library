#include "StdAfx.h"
#include "UuidFinderTester.h"
#include "../UuidFinder/UuidFinder.h"


void CUuidFinderTester::test_UuidFinder()
{
     // {D34BB8A7-A08D-45e6-BA9C-EB822B854F54}
     static const GUID Test_CLSID_MediaSubTypeH264 = 
    { 0x34363268, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };

     LPCTSTR pszH264ClsidName = FindNameByClsid(Test_CLSID_MediaSubTypeH264);
     CPPUNIT_ASSERT(0 == lstrcmp(TEXT("MEDIASUBTYPE_h264"), pszH264ClsidName));

     // {BFE4A724-00E9-40b9-859A-4170706916D9}
     static const GUID CLSID_GUID_Generate = 
     { 0xbfe4a724, 0xe9, 0x40b9, { 0x85, 0x9a, 0x41, 0x70, 0x70, 0x69, 0x16, 0xd9 } };

     LPCTSTR pszCannotFindClsid = FindNameByClsid(CLSID_GUID_Generate);
     CPPUNIT_ASSERT(NULL == pszCannotFindClsid);
}