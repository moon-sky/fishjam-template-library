#include "StdAfx.h"
#include "PocoLoggerTester.h"
#include "Poco/AutoPtr.h"
#include "Poco/AsyncChannel.h"
#include "Poco/Channel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/WindowsConsoleChannel.h"
#include "Poco/SimpleFileChannel.h"
#include "Poco/SplitterChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/LogStream.h"

CPocoLoggerTester::CPocoLoggerTester()
:m_logger(Poco::Logger::get("default")) //inherits root channel
{

}

CPocoLoggerTester::~CPocoLoggerTester()
{

}

void CPocoLoggerTester::setUp()
{

}

void CPocoLoggerTester::tearDown()
{

}

void CPocoLoggerTester::test_AsyncLogger()
{
    Poco::AutoPtr<Poco::WindowsConsoleChannel> pCons(new Poco::WindowsConsoleChannel);  
    Poco::AutoPtr<Poco::AsyncChannel> pAsync(new Poco::AsyncChannel(pCons));  

    pCons->open();

    Poco::Logger::root().setChannel(pAsync);  
    Poco::Logger& logger = Poco::Logger::get("TestLogger");  
    for (int i = 0; i < 10; ++i)  
        logger.information("This is a test");  

    Poco::Thread::sleep(1 * 1000);

}

void CPocoLoggerTester::test_Logger()
{
    Poco::AutoPtr<Poco::PatternFormatter> formatter;
    Poco::AutoPtr<Poco::Channel> channel;
    Poco::AutoPtr<Poco::FormattingChannel> formattingChannel;

    formatter =new Poco::PatternFormatter;
    formatter->setProperty("times", "local");
    formatter->setProperty("pattern", "%Y-%m-%d %H:%M:%S [%p] %T %t %U(%u)");

    channel = new Poco::FileChannel;
    channel->setProperty("path", "C:\\PotoTester.log");

    formattingChannel = new Poco::FormattingChannel(formatter, channel);
    m_logger.setChannel(formattingChannel);
    //Logger::root().setChannel(formattingChannel);  


    m_logger.information("This is an informational message");
    m_logger.warning("This is a warning message");
}

void CPocoLoggerTester::test_SplitterLogger()
{
    Poco::AutoPtr<Poco::ConsoleChannel> pCons(new Poco::ConsoleChannel);  
    Poco::AutoPtr<Poco::SimpleFileChannel> pFile(new Poco::SimpleFileChannel("test.log"));  
    Poco::AutoPtr<Poco::SplitterChannel> pSplitter(new Poco::SplitterChannel);  
    pSplitter->addChannel(pCons);  
    pSplitter->addChannel(pFile);  
    Poco::Logger::root().setChannel(pSplitter);  
    Poco::Logger::root().information("This is a test");
}

void CPocoLoggerTester::test_LogStream()
{
    Poco::Logger& logger = Poco::Logger::get("test_LogStream");  
    Poco::LogStream lstr(logger);  
    lstr << "This is a test" << std::endl;  
}