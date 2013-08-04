#pragma once
#include "TesterBase.h"

#include <vector>
//#include <string>
//#include <cassert>
//using namespace std;

class CAbstractTester : public ITesterBase
{
public:
    CAbstractTester(); //CORBA::Object_ptr pObject);
    ~CAbstractTester(void);
public:
    virtual unsigned int GetFunctionCount();
    virtual unsigned int GetParamCount(unsigned int indexFun );
    virtual CAtlString GetFunctionName(unsigned int index);
    virtual CAtlString GetParamName(unsigned int indexFun, unsigned indexParam);

    virtual void Invoke(unsigned int index,const NameValueMap& NVPair, CAtlString& strResult);
    virtual void Release();
protected:
    typedef std::vector<CAtlString>            FuncParams;
    int AppendInvokeFunInfo(const CAtlString& name, const InvokeFunc pFun, const FuncParams& params );
    const CAtlString& GetParam(const NameValueMap& NVPair, const CAtlString& strParamName) const;
    //int AppendParamName(int indexFun, const std::wstring& strParamName);
protected:
    //CORBA::Object_ptr   m_pObject;
private:
    typedef std::pair<CAtlString, InvokeFunc> InvokeInfoPair;
    std::vector<InvokeInfoPair>                 m_allInvokeFuncs;
    std::vector<FuncParams>                     m_allInvokeParams;
};

