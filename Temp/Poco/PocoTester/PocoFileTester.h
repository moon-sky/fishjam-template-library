#pragma once

#include <cppunit/extensions/HelperMacros.h>

/**********************************************************************************************
与平台无关的文件，目录的操作类。  
a. File类，读取文件的相关信息，比如权限，大小，日期，文件的删除，拷贝等 
b. Path 类， 文件目录的解析，目录创建等  
c. DirectoryIterator类，遍历指定目录下的文件列表  
d. TemporaryFile 类，临时文件创建，删除等
e. 
**********************************************************************************************/

class CPocoFileTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPocoFileTester );
    CPPUNIT_TEST( test_container_holder );
    CPPUNIT_TEST_SUITE_END();

    DECLARE_DEFAULT_TEST_CLASS(CPocoFileTester);
private:
    void test_container_holder();
};
