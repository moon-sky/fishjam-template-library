#include "stdafx.h"
#include <cppunit/config/SourcePrefix.h>
#include <cppunit/extensions/HelperMacros.h>


//Foundation
#include "PocoFoundationTester.h"
#include "PocoThreadTester.h"
CPPUNIT_TEST_SUITE_REGISTRATION( CPocoFoundationTester );
CPPUNIT_TEST_SUITE_REGISTRATION( CPocoThreadTester );


#include "PocoDataTester.h"
#include "PocoDebugTester.h"
#include "PocoConfigurationTester.h"
#include "PocoFileTester.h"
#include "PocoNetTester.h"
#include "PocoXmlTester.h"

CPPUNIT_TEST_SUITE_REGISTRATION( CPocoConfigurationTester );
CPPUNIT_TEST_SUITE_REGISTRATION( CPocoDataTester );
CPPUNIT_TEST_SUITE_REGISTRATION( CPocoDebugTester );
CPPUNIT_TEST_SUITE_REGISTRATION( CPocoFileTester );
CPPUNIT_TEST_SUITE_REGISTRATION( CPocoNetTester );
CPPUNIT_TEST_SUITE_REGISTRATION( CPocoXMLTester );

