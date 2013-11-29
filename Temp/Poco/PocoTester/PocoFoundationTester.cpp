#include "StdAfx.h"
#include "PocoFoundationTester.h"

#include <Poco/Path.h>

#include "Poco/LocalDateTime.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeParser.h"

#include "Poco/SharedLibrary.h"
#include "Poco/ClassLibrary.h"

#include "Poco/ClassLoader.h"
#include "Poco/Manifest.h"

void CPocoFoundationTester::test_DateTime()
{
    Poco::LocalDateTime now;

    std::string str = Poco::DateTimeFormatter::format(now, Poco::DateTimeFormat::ISO8601_FORMAT);
    Poco::DateTime dt;

    int tzd = 0;
    Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FORMAT, str, dt, tzd);
    dt.makeUTC(tzd);
    Poco::LocalDateTime ldt(tzd, dt);

    //CPPUNIT_ASSERT(y == 2013);
}


void CPocoFoundationTester::test_PluginLoader()
{
#if 0
    typedef Poco::ClassLoader<AbstractPlugin> PluginLoader;  
    typedef Poco::Manifest<AbstractPlugin> PluginManifest;  

    PluginLoader loader;  
    std::string libName("PluginLibrary");  
    libName += Poco::SharedLibrary::suffix(); // append .dll or .so  
    loader.loadLibrary(libName);  
    PluginLoader::Iterator it(loader.begin());  
    PluginLoader::Iterator end(loader.end());  
    for (; it != end; ++it)  
    {  
        std::cout << "lib path: " << it->first << std::endl;  
        PluginManifest::Iterator itMan(it->second->begin());  
        PluginManifest::Iterator endMan(it->second->end());  
        for (; itMan != endMan; ++itMan)  
            std::cout << itMan->name() << std::endl;  
    }  
    AbstractPlugin* pPluginA = loader.create("PluginA");  
    AbstractPlugin* pPluginB = loader.create("PluginB");  
    std::cout << pPluginA->name() << std::endl;  
    std::cout << pPluginB->name() << std::endl;  
    loader.classFor("PluginA").autoDelete(pPluginA);  
    delete pPluginB;  
    loader.unloadLibrary(libName);  

#endif
}

typedef void (WINAPI *OutputDebugStringWPtr)(LPCWSTR lpOutputString);
void CPocoFoundationTester::test_SharedLibrary()
{
    std::string kernel32Path(TEXT("C:\\Windows\\System32\\Kernel32"));
    kernel32Path.append(Poco::SharedLibrary::suffix()); // adds ".dll" or ".so"

    Poco::SharedLibrary shareLib(kernel32Path);  // will also load the library  
    OutputDebugStringWPtr pOutputDebugString = (OutputDebugStringWPtr)shareLib.getSymbol("OutputDebugStringW");
    if (pOutputDebugString)
    {
        pOutputDebugString(L"Some Info for test SharedLibrary\n");
    }
    shareLib.unload();
}