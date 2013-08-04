#ifndef TESTER_BASE_H_
#define TESTER_BASE_H_

#ifdef TESTER_EXPORTS
#define TESTER_API __declspec(dllexport)
#else
#define TESTER_API __declspec(dllimport)
#endif

//#include <string>
#include <atlbase.h>
#include <atlstr.h>
#include <map>

class ITesterNotify
{
    virtual void OnInfo(const std::wstring& info);
};

typedef std::map<const CAtlString, CAtlString>  NameValueMap;

class ITesterBase;
typedef void (ITesterBase::*InvokeFunc)(const NameValueMap& NVPair, CAtlString& strResult );

class TESTER_API ITesterBase
{
public:
    virtual CAtlString GetInterfaceName() = 0;
    virtual unsigned int GetFunctionCount() = 0;
    virtual unsigned int GetParamCount(unsigned int indexFun ) = 0;
    virtual CAtlString GetFunctionName(unsigned int index) = 0;
    virtual CAtlString GetParamName(unsigned int indexFun, unsigned indexParam) = 0;

    virtual void Invoke(unsigned int index, const NameValueMap& NVPair, CAtlString& strResult) = 0;
	virtual void Release() = 0;
};

typedef ITesterBase* (*GetTesterFunc)(); //CORBA::Object_ptr pObject);

#endif //TESTER_BASE_H_