/*******************************************************************************
bufio -- 实现缓冲的io，提供了 Reader/Writer 对象，在提供缓冲的同时实现一些文本IO的功能
  r* bufio.Reader; buf,e := r.ReadBytes('\n'); if e == os.EOF{ break }
exec -- 执行外部命令，Run(xxx)
flag -- 定义(全局的?)命令行参数规范、并可获取和解析命令行参数, 运行时: -n=8080
  port := flag.Bool("n", 80, "port number") //参数名,默认值,说明 -- 返回类型的指针(*bool，*string 等)
    flag中类似的方法有 String,
  flag.Parse(); fmt.Println("port is:", *port);
  flag.NArg() -- 返回参数个数, 可在循环中获取 flag.Arg(i)

http -- 实现了HTTP请求、响应和URL的解析，并且提供了可扩展的HTTP服务和基本的HTTP客户端
log -- 基础的日志功能
io -- 原始的IO操作接口，对os包这样的原始IO进行封装
json -- 实现了编解码 RFC4627 定义的JSON对象
os -- 提供了与跨平台的操作系统功能接口，其设计是Unix形式的，有三个子包 Exec/Signal/User
  读写环境变量: Getenv/Setenv, 枚举全部环境变量: range of os.Environ()
  处理命令行参数: os.Args, 参数中没有运行的命令，即[0]为第一个用户输入参数，而非运行的命令
reflect -- 实现了运行时反射
sort -- 提供对数组和用户定义集合的原始的排序功能
strconv -- 提供将字符串转换成基本数据类型，或从基本数据类型转换为字符串的功能
sync -- 多线程同步,包括 atomic, Cond,Mutex,RWMutex, Locker 等
syscall -- (主要是类Unix系统?)提供了 Chroot/Chmod/Chmod/Getgid/Getpid/Getgroups/Getpid 等
           甚至有 Inotify/Ptrace/Epoll/Socket 等系统调用
template -- 数据驱动的模版，用于生成文本输出，如 HTML。
  将模版关联到某个数据结构上进行解析，模版内容指向数据结构的元素(通常结构的字段或者map的键)，控制
  解析并且决定某个值会被现实，模版扫描结构以便解析，而"游标"决定了当前位置在结构中的值。
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
