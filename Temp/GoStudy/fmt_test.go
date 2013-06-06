/*******************************************************************************
fmt包实现了格式化的IO函数，支持的格式化类型:
  %b -- 二进制
  %o -- 八进制
  %d -- 十进制
  %X -- 十六进制
  %q -- ??
  %v -- 默认格式的值，当打印结构时，加号(%+v)会增加字段名
        %v 可以打印实现了 Stringer 接口(有 String() string 方法)的任何类
        %#v -- Go样式的值表达
  %T -- 类型，会打印出如 "func()" 等类型？

输入数据： var input string; fmt.Scanln(&input);
似乎直接 Println(xxx) 可以自动打出符合格式的内容，如 时间段变量会打出 "50ms"
*******************************************************************************/
package gostudy

import (
	"fmt"
	"testing"
)

func TestFmtType(t *testing.T) {

	fmt.Printf("Test Tools")
}

func TestFmtPackage(t *testing.T) {

	fmt.Printf("hello world\n") //等价于 print("hello world")
	fmt.Printf("some string value:%s\n", myFunReturnString(100))

	t.Log("some info in fmt package testing")
}
