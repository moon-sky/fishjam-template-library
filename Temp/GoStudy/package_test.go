/*******************************************************************************
bufio -- 实现缓冲的io，提供了 Reader/Writer 对象，在提供缓冲的同时实现一些文本IO的功能
  r* bufio.Reader; buf,e := r.ReadBytes('\n'); if e == os.EOF{ break }
exec -- 执行外部命令，Run(xxx)
flag -- 实现了命令行解析
  var numberFlag = flag.Bool("n", false, "number each line")
  flag.Parse()
  flag.NArg() -- 返回参数个数, 可在循环中获取 flag.Arg(i)

http -- 实现了HTTP请求、响应和URL的解析，并且提供了可扩展的HTTP服务和基本的HTTP客户端
io -- 原始的IO操作接口，对os包这样的原始IO进行封装
json -- 实现了编解码 RFC4627 定义的JSON对象
os -- 提供了与平台无关的操作系统功能接口，其设计是Unix形式的
reflect -- 实现了运行时反射
sort -- 提供对数组和用户定义集合的原始的排序功能
strconv -- 提供将字符串转换成基本数据类型，或从基本数据类型转换为字符串的功能
sync -- 多线程同步,包括 atomic, Cond,Mutex,RWMutex, Locker 等
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
