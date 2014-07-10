#include "StdAfx.h"
#include "FTLCrackTester.h"
#include <ftlCrack.h>

void CFTLCrackTester::test_GetCrackString()
{
    CFCrackUtility crackUtil;
    CString strTemp = crackUtil.GetCrackString(CFCrackUtility::csKiloString,0);
    strTemp = crackUtil.GetCrackString(CFCrackUtility::csKiloString,16);
    strTemp = crackUtil.GetCrackString(CFCrackUtility::csKiloString,32);

}
