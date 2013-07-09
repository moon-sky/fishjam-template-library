/*******************************************************************************
bufio -- 实现缓冲的io，提供了 Reader/Writer 对象，在提供缓冲的同时实现一些文本IO的功能
  r* bufio.Reader; buf,e := r.ReadBytes('\n'); if e == os.EOF{ break }
bytes -- 提供了对字节切片操作的函数
  NewBuffer(nil) -- 分配动态内存? 然后往里面Write时会自动扩展，写完全部内容后通过Bytes方法转换成切片
exec -- 执行外部命令，Run(xxx)
expvar -- 为公共变量提供了一个标准的接口，如服务器中的运算计数器
debug
  dwarf -- 提供了对从执行文件加载的DWARF调试信息的访问，对实现GO语言的调试器非常有价值
  elf -- 实现了对ELF对象文件的访问
  gosym -- 访问Go语言二进制程序中的调试信息，对于可视化调试很有价值
  pe -- 实现了对PE文件的访问
flag -- 定义(全局的?)命令行参数规范、并可获取和解析命令行参数, 运行时: -n=8080
  port := flag.Bool("n", 80, "port number") //参数名,默认值,说明 -- 返回类型的指针(*bool，*string 等)
    flag中类似的方法有 String,
  flag.Parse(); fmt.Println("port is:", *port);
  flag.NArg() -- 返回参数个数, 可在循环中获取 flag.Arg(i)

http -- 实现了HTTP请求、响应和URL的解析，并且提供了可扩展的HTTP服务和基本的HTTP客户端
image -- 实现了一个基本的二维图像库
index\suffixarray --通过构建内存索引实现的高速字符串匹配查找算法
io -- 通过io包(而不是fmt包)来输出字符串，源文件编译成可执行文件后，体积会小很多，而且运行起来也更省资源
log -- 基础的日志功能
io -- 原始的IO操作接口，对os包这样的原始IO进行封装
mime -- 实现了部分的MIME规范
json -- 实现了编解码 RFC4627 定义的JSON对象
os -- 提供了与跨平台的操作系统功能接口，其设计是Unix形式的，有三个子包 Exec/Signal/User
  读写环境变量: Getenv/Setenv, 枚举全部环境变量: range of os.Environ()
  处理命令行参数: os.Args, ?参数中没有运行的命令，即[0]为第一个用户输入参数，而非运行的命令?
reflect -- 实现了运行时反射
regexp -- 实现了一个简单的正则表达式库
sort -- 提供对数组和用户定义集合的原始的排序功能
strconv -- 提供将字符串转换成基本数据类型，或从基本数据类型转换为字符串的功能
strings -- 实现了操作字符串的简单函数
sync -- 多线程同步,包括 atomic, Cond,Mutex,RWMutex, Locker 等
syscall -- (主要是类Unix系统?)提供了 Chroot/Chmod/Chmod/Getgid/Getpid/Getgroups/Getpid 等
           甚至有 Inotify/Ptrace/Epoll/Socket 等系统调用
template -- 数据驱动的模版，用于生成文本输出，如 HTML。
  将模版关联到某个数据结构上进行解析，模版内容指向数据结构的元素(通常结构的字段或者map的键)，控制
  解析并且决定某个值会被现实，模版扫描结构以便解析，而"游标"决定了当前位置在结构中的值。
text
  scanner -- 为UTF-8文本提供了一个扫描器和分词器
unsafe -- 包含了Go程序中数据类型上所有不安全的操作，通常无需使用这个包
*******************************************************************************/
package gostudy

import (
	"fmt"
	"testing"
)

func TestPackages(t *testing.T) {
	fmt.Printf("Test Tools")
}
