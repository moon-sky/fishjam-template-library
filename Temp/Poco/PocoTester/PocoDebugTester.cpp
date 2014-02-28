#include "StdAfx.h"
#include "PocoDebugTester.h"
#include "Poco/Exception.h"
#include "Poco/Bugcheck.h"
#include "Poco/NestedDiagnosticContext.h"


void CPocoDebugTester::test_NDC()
{
    poco_ndc(test_NDC); 
    Poco::NDC::current().dump(std::cout); 
}
