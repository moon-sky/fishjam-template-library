/*******************************************************************************
TODO: 定义结构时，成员变量后面的字符串是干什么的？是json等的标签？(参见 ipc\server.go::Request 等的定义)

Go语言编程 P178 -- 2读取输入文件
https://github.com/astaxie/build-web-application-with-golang/blob/9aa3e50854349188fb836e8a7981eea69855a92a/preface.md

官方网站：
  https://code.google.com/p/go/
  Go语言的官方文档？：http://golang.org/doc/
  Go邮件组: http://groups.google.com/group/golang-nuts   -- 英文
           http://groups.google.com/group/golang-china  -- 中文讨论环境
  第三方库?：https://code.google.com/p/go-wiki/wiki/Projects

特点(从 Limbo 语言继承) -- 完全开源的，BSD License，但使用其的产品是否需要开源？
  Go试图成为结合解释型编程的轻松、动态类型语言的高效以及静态类型语言的安全的编译型语言。
  并发(执行体有: 进程process、线程thread、协程coroutine)、分布式
  支持垃圾回收(GC, 参见 gc1.go)、 适合多核机器、网络、Web开发
  类型系统允许构建有弹性的模块化程序
  强大的运行时反射


调试
  不需要编译选项，Go语言编译的二进制程序直接支持GDB调试，调试信息格式为 DWARFv3(版本高于7.1的GDB都支持)
*******************************************************************************/

/*******************************************************************************
init() -- 在main函数之前执行的初始化函数
main() -- 可执行文件的入口函数
*******************************************************************************/

/******************************************************************************
go help testfunc -- 查看相关帮助

符合规范的测试代码会在运行测试时被自动识别并执行。

单元测试 -- xxx.go 文件对应的单元测试文件为 xxx_test.go, 通过 go test [xxx] 运行测试
  函数签名为：func TestXxx(t *testing.T)
  testing.T
    Error("错误信息")
    Errorf("格式化错误信息")
    Skip("跳过测试的原因")

性能测试 -- 命令行中通过 -bench regexp 运行匹配的性能测试(如 -bench . 表示匹配所有)
  函数签名: func BenchmarkXxx(b *testing.B) {
    for i := 0; i < b.N; i++ { 运行目标代码N次 }
  }
  testing.B
	ResetTimer() -- 重设计时器，通常用于初始化后来排除初始化的影响

示例(Examples) -- 测试时会比较 示例语句在标准输出中输出的内容 和 注释Output:后的内容
 func ExampleXxx(){
  示例语句
  //Output: 示例输出
 }

*******************************************************************************/

package gostudy

import (
	"fmt"
	"math/rand"
	"testing"
	"time"
)

func TestGoStudy(t *testing.T) {
	if 1+1 != 2 {
		t.Error("1 + 1 != 2")
	}
	//t.Log("some info in TestGoStudy")
}

func funOfLongTimeRun() {
	time.Sleep(time.Duration(rand.Intn(10)) * time.Millisecond)
}

//go test goStudy_test.go -bench .
func BenchmarkFunOfLongTimeRun(b *testing.B) {
	for i := 0; i < b.N; i++ {
		funOfLongTimeRun()
	}
	//b.ReportAllocs()
}

//示例:Example
func ExampleHello() {
	fmt.Println("Hello World") //尝试更改其中一个的字符串，测试会输出"got"和"want"的错误信息
	//Output: Hello World
}
