#include "StdAfx.h"
//#include <iosfwd>
//#include <sstream>
#include "AbstractTester.h"

union CallerFunctions
{
    InvokeFunc pFun;
};

const CAtlString  UNFOUND_PARAM_VALUE = TEXT("unknown");

CAbstractTester::CAbstractTester() //CORBA::Object_ptr pObject)
//:m_pObject(pObject)
{
}

CAbstractTester::~CAbstractTester(void)
{
    m_allInvokeFuncs.clear();
    m_allInvokeParams.clear();
}

int CAbstractTester::AppendInvokeFunInfo(const CAtlString& name,const InvokeFunc pFun ,const FuncParams& params)
{
    m_allInvokeFuncs.push_back(InvokeInfoPair(name, pFun));
    m_allInvokeParams.push_back(params);
    return m_allInvokeFuncs.size() - 1;
}

const CAtlString& CAbstractTester::GetParam(const NameValueMap& NVPair, const CAtlString& strParamName) const
{
    NameValueMap::const_iterator iter = NVPair.find(strParamName);
    if (iter != NVPair.end())
    {
        return iter->second;
    }
    ATLASSERT(FALSE);
    return UNFOUND_PARAM_VALUE;
}

//int CAbstractTester::AppendParamName(int indexFun, const std::wstring& strParamName)
//{
//    int nRet = -1;
//    if (indexFun >= 0 && indexFun < m_allInvokeFuncs.size())
//    {
//        FuncParams& params = m_allInvokeParams[indexFun];
//        params.push_back(strParamName);
//        nRet = params.size();
//    }
//    return nRet;    
//}

unsigned int CAbstractTester::GetFunctionCount()
{
    return m_allInvokeFuncs.size();
}

unsigned int CAbstractTester::GetParamCount(unsigned int indexFun )
{
    int nRet = 0;
    if (indexFun >= 0 && indexFun < m_allInvokeFuncs.size())
    {
        FuncParams& params = m_allInvokeParams[indexFun];
        nRet = params.size();
    }
    return nRet;
}

CAtlString CAbstractTester::GetFunctionName(unsigned int index)
{
    ATLASSERT(index >= 0);
    ATLASSERT(index < m_allInvokeFuncs.size() );
    if (index >= 0 && index < m_allInvokeFuncs.size() )
    {
        return m_allInvokeFuncs[index].first;
    }
    return TEXT("");
}

CAtlString CAbstractTester::GetParamName(unsigned int indexFun, unsigned indexParam)
{
    CAtlString strResult;
    if (indexFun >= 0 && indexFun < m_allInvokeFuncs.size())
    {
        FuncParams& params = m_allInvokeParams[indexFun];
        strResult = params[indexParam];
    }
    return strResult;
}


void CAbstractTester::Invoke(unsigned int index,const NameValueMap& NVPair, CAtlString& strResult) 
{
    ATLASSERT(index >= 0);
    ATLASSERT(index < m_allInvokeFuncs.size() );
    if (index >= 0 && index < m_allInvokeFuncs.size() )
    {
        InvokeFunc pFunc = m_allInvokeFuncs[index].second;
        ATLASSERT(pFunc);
        CallerFunctions cf;
        cf.pFun = pFunc;

        (this->*cf.pFun)(NVPair, strResult);
    }
}

void CAbstractTester::Release() 
{
    delete this;
}
