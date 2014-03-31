#ifndef _POCO_STUDY_H
#define _POCO_STUDY_H

#pragma once

//http://blog.csdn.net/arau_sh/article/details/8620810

//快速的跨平台开发 -- STL + boost + Poco + Qt + Mysql 是一个不错的组合

#include "Poco/Config.h"        //编译定义宏，用来指定编译版本中一些特性
#include "Poco/Foundation.h"    //编译版本的输出类型,如编译的Dll的输出属性
#include "Poco/Platform.h"      //支持平台定义，操作系统与硬件结构关系定义
//#include "Poco/Platform_WIN32.h"   //特定平台的下平台特性定义


/****************************************************************************************************
* Poco(POrtable COmponents), http://pocoproject.org/ -- 开源，侧重于互联网应用，
*   使用Boost Software License发布，完全免费
*   提供跨平台(Windows/WinCE/Linux/Mac OS X/Solaris/Android 等)的基础类库，可认为是一个小型的ACE?
*   包括：Foundation、Crypto、XML、Util、Net、Zip、NetSSL、Data（支持 SQLite、ODBC、MySQL）、
*          Tools & Utilities、Application 等
* 
* 跨平台库的生成
*   1.VC下编译采用没有预编译头文件的形式, 没有stdafx.h 、stdafx.cpp
*   2.所有的Dll都是没有资源、没有dllmain存在的工程
*
* 编译
*   TODO: 编译时的日志可以通过重定向保存到文本中吗？
*   0.Poco库分成很多类型 -- (Debug/Release) * (Share/Static) * (MultiThread/MultiThreadDLL), 其中：静态库需要定义 POCO_STATIC 宏
*   1.更改 buildwin.cmd 批处理，设置其中的 OPENSSL_DIR,OPENSSL_LIB
*   1.5. -- TODO: MySQL 怎么弄，需要加到VS的配置路径中？或更改 buildwin.cmd ?
*     set MYSQL_DIR=D:\MaNGOS\MaNGOSfour\dep
*     set MYSQL_INCLUDE=%MYSQL_DIR%\include\mysql
*     set MYSQL_LIB=%MYSQ_DIR%\lib\win32_debug;%MYSQ_DIR%\lib\win32_release;
*     set INCLUDE=%INCLUDE%;%OPENSSL_INCLUDE%;%MYSQL_INCLUDE%
*     set LIB=%LIB%;%OPENSSL_LIB%;%MYSQL_LIB%
*   2.执行对应 build_vs90.cmd 等批处理
* 
* 问题
*   1.Poco中定义了一些宏，会造成标准API(如 OutputDebugString) C2065(undeclared identifier), C3861(identifier not found)的错误。
*     解决：a.更改头文件的包含顺序?
*            b.使用编译堆栈控制宏，如 verify
*              #pragma push_macro("verify")
*              #undef verify
*              include各种Poco头文件
*              #pragma pop_macro("verify")

* 编译宏 -- 更改 Poco/Config.h 文件? 可以控制线程数最大数量、是否支持UTF8(默认支持)等
****************************************************************************************************/


/****************************************************************************************************
* 基本的数据类型在 Poco/Types.h 中，定义了跨平台的类型定义(如 Poco::Int8, Poco::Int16, Poco::IntPtr 等)
*   
* 编译
*   需要设置 $(POCO_BASE) 环境变量 ?
*
* 跨平台编译( $(POCO_BASE)/build )
*   Linux/Unix -- 在 Makefile 中需要 include $(POCO_BASE)/build/rules/global ?
*   Android/MacOS -- 客户端使用时必须定义 POCO_ANDROID 宏，编译配置文件为 <POCO>/build/config/Android
*     参考文档: http://www.appinf.com/docs/poco/99300-AndroidPlatformNotes.html
*                <POCO>/doc/99300-AndroidPlatformNotes.page
*     限制：Android下不支持以下的类: NamedEvent/NamedMutex/SharedMemory/RWLock 等
*     0.在执行脚本时可能会报 "$'\r': command not found" 的错误 ，需要使用 dos2unix 命令先将脚本转换成unix格式
*       需要更改的文件为 /<poco> 目录下的 build/* 和 rules/* 等
*     1.生成自定义的 Standalone Toolchain
*       $NDK/build/tools/make-standalone-toolchain.sh --platform=android-8 --install-dir=$HOME/my-android-toolchain
*       注意：可能需要增加 "--system=windows-x86_64" ，代表什么意思？
*     2.将自定义toolchain的bin目录加入PATH环境变量(.bash_profile): export PATH=$PATH:$HOME/my-android-toolchain/bin
*     3.编译: ./configure --config=Android --no-samples --no-tests 
*              make -- 可以通过 ANDROID_ABI=armeabi-v7a 改变缺省的 platform ABI
*       TODO:也可以通过定义 Android.mk 后使用NDK来编译，如何?
*     4.Android 中使用Poco
*       Android.mk 中:
*         LOCAL_CPP_FEATURES := rtti exceptions
*       Application.mk 中:
*         APP_STL := gnustl_shared
****************************************************************************************************/

/***************************************************************************************************
特性列表:

DynamicAny与Any类
Cache框架
基于OpenSSL的密码系统
日期，时间类库
事件和通知框架
FTP客户端
跨平台的文件系统类库
HTML表单类库
HTTP客户端和服务端（支持SSL），C++ 服务器页面编译器
日志框架
多线程框架（线程池，活动对象，工作队列等）
POP3客户端类库
跨平台，一次编写，多平台编译和运行
进程管理和进程间通信类库
反射框架
基于PCRE的正则表达式
SMTP客户端类库
数据库访问类库（SQLite,MySQL, ODBC)
支持SSL/TLS，基于OpenSSL
动态类库加载
先进的内存和指针管理（Buffer, Pool）
Socket类库
网络数据流类库，支持Base64,HexBinary编解码，压缩等等
字符串格式化和其它字符串工具类库
TCP服务器框架（多线程）
文本编码和转换
Tuples
URI支持
支持UTF8和Unicode编码
UUID生成器
XML生成和解析器
Zip文件操作类库
****************************************************************************************************/

#endif _POCO_STUDY_H