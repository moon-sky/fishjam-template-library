// ProtDrvTester.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ProtDrvTester.h"
#include "AbstractTester.h"

class CProtDrvTester : public CAbstractTester
{
public:
    CProtDrvTester()
        :CAbstractTester()
    {
        FuncParams  params;

        params.push_back(TEXT("userName"));
        params.push_back(TEXT("dummy"));

        AppendInvokeFunInfo(TEXT("sayHello"), 
            static_cast<InvokeFunc>(&CProtDrvTester::invokeSayHello), params);

        AppendInvokeFunInfo(TEXT("testUSList"), 
            static_cast<InvokeFunc>(&CProtDrvTester::invokeTestUSList), params);


        params.clear();
    }

    virtual CAtlString GetInterfaceName()
    {
        return TEXT("Hello");
    }

    void CProtDrvTester::invokeSayHello(const NameValueMap& NVPair, CAtlString& strResult)
    {
        MessageBox(NULL, TEXT("invokeSayHello"), TEXT("Caption"), MB_OK);
    }

    void CProtDrvTester::invokeTestUSList(const NameValueMap& NVPair, CAtlString& strResult)
    {
        MessageBox(NULL, TEXT("invokeTestUSList"), TEXT("Caption"), MB_OK);
    }
};

extern "C" TESTER_API 
ITesterBase* GetTester()
{
    return new CProtDrvTester();
}
