/*******************************************************************************
* 基础语法：
*   http://www.open-open.com/lib/view/open1352201112438.html
*   http://www.open-open.com/lib/view/open1352264498172.html
*
* 执行程序的入口点 func 是 main
*
* 注意：
*  1.无论任何时候，你都不应该将一个控制结构（(if、for、switch或select）的左大括号放在下一行。
*    如果这样做，将会在大括号的前方插入一个分号，这可能导致出现不想要的结果。
*  2.Go的正式的语法使用分号来终止语句，但这些分号由词法分析器在扫描源代码过程中使用简单的规则自动插入分号，源码中就不需要分号了。
*  3.通常Go程序仅在for循环语句中使用分号，以此来分开初始化器、条件和增量单元
*  4.没有 public/protected/private 的关键字，如果想让方法可以被别的包访问的话，第一个字母必须大写。
*
* 内存分配
*  new(T) -- 分配内存的内建函数, 只是将内存清零，而不是初始化内存，new(T)返回一个指向新分配的类型为T的零值的指针。
*  make(T, args) -- 仅用于创建切片、map和chan（消息管道）,并返回类型T（不是*T）的一个被初始化了的（不是零）实例
*
* 切片 -- 一个具有三项内容的描述符，包括指向数据（在一个数组内部）的指针、长度以及容量
* 函数 -- func 函数名(变量名 变量类型 , ...) 返回值 { 函数体 }
*  返回多值 -- 很多函数都会返回两个值，一个正常值，一个是错误
*  函数不定参数 -- func sum(nums ...int)
* 函数闭包 -- 返回匿名函数的函数？ func nextNum() func() int { xxx := 100; return func() int { return xxx; }
*******************************************************************************/
package gostudy

import (
	"fmt"
	//"math"
	"testing"
)

/*
var 变量名 [类型][= 初始值] 等价于 "变量名 := 初始值"
go是静态类型的语言？语法很像 javascript(可不指明类型，通过初始化值来推导),
常见类型有 bool, int, string 等
*/
func TestLanguage(t *testing.T) {
	var iValue int = 100
	iOtherValue := 100
	if iValue != iOtherValue {
		t.Error("定义变量")
	}

	//for 语句没有括号 -- 使用循环计算 1~100 的和
	var iSum int = 0
	for i := 1; i <= 100; i++ {
		iSum += i
	}
	fmt.Printf("iSum=%d\n", iSum)
	//t.Error(iSum == 5050)

	//数组
	str := "this is string"
	arr := []bytes(str)
	arr[0] = 'T'
	fmt.Printf("str is %s\n", str)
}

//数组切片(类似 Python) -- 英文是不是这个?
func TestArraySplit(t *testing.T) {

}

func TestLanguage2(t *testing.T) {
	t.Log("some info in language 2")
	//t.Errorf("some info in language 2")
}

func TestPointer(t *testing.T) {
	var intValue int = 10
	var pIntValue = &intValue
	(*pIntValue) += 15
	fmt.Printf("intValue=%d, pIntValue=%p, *pIntValue=%d\n", intValue, pIntValue, *pIntValue)

	var v []int = make([]int, 10, 100) // -- 分配一个整型数组，长度为10，容量为100，并返回前10个数组的切片
	for i := 0; i < len(v); i++ {
		v[i] = i
	}

}

func myMulti_ret(key string) (int, bool) {
	var err bool = false
	var val int = 0
	if key == "one" {
		val = 1
		err = true
	} else if key == "two" {
		val = 2
		err = true
	}
	return val, err
}

func TestMyMultiRet(t *testing.T) {
	if _, e := myMulti_ret("one"); e { // 注意 if 的分号后面有 e
		fmt.Printf("正常返回\n")
	} else {
		fmt.Printf("错误返回\n")
	}
}

//函数不定参数
func MySum(nums ...int) {
	var sum int = 0
	for _, num := range nums {
		sum += num
	}
	fmt.Printf("sum for %d num is %d\n", len(nums), sum)
}
func TestMySums(t *testing.T) {
	MySum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
}

func myFunReturnString(val int) string {
	return fmt.Sprintf("%d", val) //返回格式化的string
}
func TestFmtPackage(t *testing.T) {
	fmt.Printf("hello world\n") //等价于 print("hello world")
	fmt.Printf("some string value:%s\n", myFunReturnString(100))

	t.Log("some info in fmt package testing")
}
