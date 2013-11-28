#include "StdAfx.h"
#include "PocoFoundationTester.h"
#include <Poco/DateTime.h>

void CPocoFoundationTester::test_DateTime()
{
    Poco::DateTime now;
    int y = now.year();

    CPPUNIT_ASSERT(y == 2013);
}

