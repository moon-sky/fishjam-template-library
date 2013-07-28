#include "stdafx.h"

#ifdef ENABLE_CPPUNIT

#include <cppunit/config/SourcePrefix.h>
#include <cppunit/extensions/HelperMacros.h>

#pragma comment(lib, "cppunitd.lib")
#pragma comment(lib, "testrunnerud.lib")

#include "FDriverHelloTester.h"

CPPUNIT_TEST_SUITE_REGISTRATION(CFDriverHelloTester);

#endif //ENABLE_CPPUNIT
