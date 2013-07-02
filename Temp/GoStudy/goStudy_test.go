/*******************************************************************************
Go语言编程 P21 -- 2.1.3变量赋值

官方网站：
  https://code.google.com/p/go/
  Go语言的官方文档？：http://golang.org/doc/
  Go邮件组: http://groups.google.com/group/golang-nuts   -- 英文
           http://groups.google.com/group/golang-china  -- 中文讨论环境

特点(从 Limbo 语言继承) -- 完全开源的，但使用其的产品是否需要开源？
  并发(执行体有: 进程process、线程thread、协程coroutine)、分布式
  支持垃圾回收(GC, 参见 gc1.go)、 适合多核机器、网络、Web开发
  类型系统允许构建有弹性的模块化程序
  强大的运行时反射

调试
  不需要编译选项，Go语言编译的二进制程序直接支持GDB调试，调试信息格式为 DWARFv3(版本高于7.1的GDB都支持)
*******************************************************************************/

package gostudy

import (
	//"fmt"
	"testing"
)

func TestGoStudy(t *testing.T) {
	t.Log("some info in TestGoStudy")

}
