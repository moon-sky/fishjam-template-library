#include "StdAfx.h"
#include "PocoConfigurationTester.h"
#include <Poco/Util/SystemConfiguration.h>
#include <sstream>

void CPocoConfigurationTester::test_SystemConfiguration()
{
    Poco::AutoPtr<Poco::Util::SystemConfiguration> pSysConfiguration(new Poco::Util::SystemConfiguration());
    std::string strosName = pSysConfiguration->getString("system.osName");
    std::string strosVersion = pSysConfiguration->getString("system.osVersion");
    std::string strosArchitecture = pSysConfiguration->getString("system.osArchitecture");
    std::string strnodeName = pSysConfiguration->getString("system.nodeName");
    std::string strnodeId = pSysConfiguration->getString("system.nodeId");
    std::string strcurrentDir = pSysConfiguration->getString("system.currentDir");
    std::string strhomeDir = pSysConfiguration->getString("system.homeDir");
    std::string strtempDir = pSysConfiguration->getString("system.tempDir");
    std::string strdateTime = pSysConfiguration->getString("system.dateTime");
    std::string strpid = pSysConfiguration->getString("system.pid");
    std::string strNumberOfProcessors = pSysConfiguration->getString("system.env.NUMBER_OF_PROCESSORS");

    std::ostringstream os;
    os  << "osName=" << strosName
        << ",osVersion=" << strosVersion
        << ",osArchitecture=" << strosArchitecture
        << ",nodeName=" << strnodeName
        << ",nodeId=" << strnodeId << std::endl
        << "currentDir=" << strcurrentDir
        << ",homeDir=" << strhomeDir
        << ",tempDir=" << strtempDir
        << ",dateTime=" << strdateTime
        << ",pid=" << strpid
        << ",NumberOfProcessors=" << strNumberOfProcessors << std::endl;
    
    const std::string& str = os.str();
    OutputDebugStringA(str.c_str());
}
