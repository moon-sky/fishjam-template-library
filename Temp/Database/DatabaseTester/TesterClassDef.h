#ifndef _FJUTILITY_TEST_TESTER_CLASS_DEF_H
#define _FJUTILITY_TEST_TESTER_CLASS_DEF_H

#pragma once

//声明默认的测试类
//  1.空的默认构造、默认析构
//  2.私有的拷贝构造和复制操作符
//  3.空的 CPPUNIT重载函数 setUp 和 tearDown

#define DECLARE_DEFAULT_TEST_CLASS(className) \
public:\
	className(){};\
	virtual ~className(){};\
public:\
	virtual void setUp(){};\
	virtual void tearDown(){};\
private:\
	className( const className &copy );\
	void operator =( const className &copy );

#endif //_FJUTILITY_TEST_TESTER_CLASS_DEF_H