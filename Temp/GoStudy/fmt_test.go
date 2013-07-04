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
	"strconv"
	"strings"
	"testing"
)

func ExampleFmtType() {
	fmt.Printf("hello world\n") //等价于 print("hello world")
	x := 10
	y := 20
	fmt.Println("x =", x, ", y =", y)

	//Output:
	//hello world
	//x = 10 , y = 20
}

func TestConv(t *testing.T) {
	valInt, _ := strconv.Atoi("123")
	GOUNIT_ASSERT(t, valInt == 123, "Atoi")
}

func TestStrings(t *testing.T) {
	strParam := "spm=2013.1.0.103.ZJ8LEG&id=18630698467&source=superboss&appId=13"
	params := strings.Split(strParam, "&")
	GOUNIT_ASSERT(t, len(params) == 4, "字符串Split")
	GOUNIT_ASSERT(t, params[3] == "appId=13", "字符串Split")
}
