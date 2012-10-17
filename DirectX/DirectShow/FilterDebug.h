#ifndef FILTER_DEBUG_H
#define FILTER_DEBUG_H

#pragma once


/***************************************************************************************************
* DbgLog -- 系统提供的日志输出，可以设置和检测 Type/Level(DbgSetModuleLevel/DbgCheckModuleLevel)
*   Type -- LOG_TIMING, LOG_TRACE, LOG_MEMORY, LOG_LOCKING , LOG_ERROR
*   Level:DWORD, 0 最重要(总是输出)，每种类型都有对应的Level,如 CONNECT_TRACE_LEVEL
* 
* 注册表的 HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\DirectShow\Debug 记录了每个模块每种类型的日志输出等级，可以分别设置,
*   而 GLOBAL 子项下是全局设置，设置的值必须大于对应的Level才能有效
*   CUSTOMx -- 
*   ERROR
*   LogToFile -- 设置默认输出的目的地
*     1.为NULL使用 OutputDebugString 输出
*     2.为"Console"时，在控制台输出（好像有问题？）
*     3.为文件路径时，使用文件方式输出
*   TIMEOUT -- 设置 DbgWaitForSingleObject 等函数的超时值，可以方便发现死锁(注意用户暂停问题)
*     也可以通过 DbgSetWaitTimeout 函数设定
*
*
*   DbgDumpObjectRegister -- 显示当前活动对象的信息（可以检查内存泄漏),Debug状态下自动调用 ?
*   DbgSetModuleLevel(LOG_TRACE|LOG_MEMORY , 5);  -- 设置日志输出的类型 和 等级
*   DbgSetAutoRefreshLevels -- 设置是否自动检查注册表的变化(默认不检查，检查的话会影响性能)
*   DisplayTypeInfo -- 显示媒体类型,Terse at level 2, verbose at level 5
*   DisplayPinInfo -- 显示Pin信息
*   DumpGraph(pGraph, 1) -- 显示 Filter Graph 的信息
*   系统提供了 DbgLog 宏用于输出信息
*   CAutoTrace -- 自动对每个函数进行跟踪，应该使用 AMTRACEFN / AMTRACE 宏
*
*   Sample 的 NullInPlace 中定义了 CTextMediaType 类，可以将媒体类型的GUID转变为易读的 CLSID_XXX 
*     -- 使用全局的GuidNames[yourGUID]数组变量
***************************************************************************************************/

#endif //FILTER_DEBUG_H