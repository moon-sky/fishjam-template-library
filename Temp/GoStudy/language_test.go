/*******************************************************************************
基础语法：
  http://www.open-open.com/lib/view/open1352201112438.html
  http://www.open-open.com/lib/view/open1352264498172.html

执行程序的入口点 func 是 main

注意：
 1.无论任何时候，你都不应该将一个控制结构（(if、for、switch或select）的左大括号放在下一行。
   如果这样做，将会在大括号的前方插入一个分号，这可能导致出现不想要的结果。
 2.Go的正式的语法使用分号来终止语句，但这些分号由词法分析器在扫描源代码过程中使用简单的规则自动插入分号，源码中就不需要分号了。
 3.通常Go程序仅在for循环语句中使用分号，以此来分开初始化器、条件和增量单元
 4.没有 public/protected/private 的关键字，如果想让方法可以被别的包访问的话，第一个字母必须大写。

内存分配
  new(T) -- 分配内存的内建函数, 只是将内存清零，而不是初始化内存，new(T)返回一个指向新分配的类型为T的零值的指针。
  make(T, args)--仅用于创建内建的slice、map和channel（消息管道）,并返回类型T（不是*T）的一个被初始化了的（不是零）实例

切片 -- 一个具有三项内容的描述符，包括指向数据（在一个数组内部）的指针、长度以及容量
函数 -- func [p 特定类型] 函数名(变量名 变量类型 , ...) (返回值 返回类型,...) { 函数体 }
 可以随意安排函数的顺序 -- 不需要前向声明函数原型
 可选的"特定类型"表示这是特定类型的成员方法(参见 class_test )
 返回多值 -- 很多函数都会返回两个值，一个正常值，一个是错误
 函数不定参数(变参) -- func sum(nums ...int)
 匿名函数
 函数作为值赋值给变量(相当于函数指针),
   pFun:=func(){xxx}  //定义匿名函数并赋值给变量
   pFun()		      //通过变量调用函数

函数闭包 -- 返回匿名函数的函数？ func nextNum() func() int { xxx := 100; return func() int { return xxx; }

关键字
  select(选择不同类型的通讯),chan,
  fallthrough -- 用于switch...case中，当匹配失败后自动向下尝试(类似C中没有break时?)
  range--迭代器，可以从array,slice,string,map,channel等需要循环的内容中返回一个键值对

内建函数
  append -- 追加slice
  len/cap -- 返回字符串、slice、数组等的长度/最大容量
  copy -- 复制slice, 并且返回赋值的元素的个数
  print/println -- 底层打印函数，可以在不引入fmt包的情况下使用，主要用于调试
  close/closed,,make,real,
  complex,real,imag -- 处理复数

利用多赋值(是这个名字吗？)，要交换两个变量的值非常简单，不需要定义中间变量
  x, y = y , x
*******************************************************************************/
package gostudy

import (
	"fmt"
	//"math"
	"testing"
)

/*******************************************************************************
定义变量：
  var 变量名 [类型][= 初始值] 等价于 "变量名 := 初始值"
go是静态类型的语言？语法很像 javascript(可不指明类型，通过初始化值来推导),
常见类型(各类型全部独立，混用这些类型向变量赋值会引起编译器错误 -- 强制转换: type(value)?)：
  bool --
  byte -- uint8 的别名
  complex64(32位虚数部分)/complex128(64位虚数部分) -- 复数
  int/uint -- 32位系统是32位，但64位系统是64位的?
  int32/uint32/int64/uint64/float32/float64
  string -- 字符串是常量，是不能修改的？ 即 string[0] = 'S' 是非法的？
*
常量 const ( 常量定义 ),可通过 iota 来定义枚举
特殊变量名(_, 下划线)，任何赋给它的值都被丢弃，通常用于不关心的变量，否则会出现编译错误(声明了变量却不使用)
*******************************************************************************/

//使用 iota 定义枚举
const (
	Monday = iota //默认值是0，其后的定义会自动递增，也可以手动指定值
	Tuesday
	Wednesday
	Thursday
	Friday
	Saturday
	Sunday
)

func TestEnum(t *testing.T) {
	fmt.Printf("Sunday Number=%d\n", Sunday)
	if Sunday != 6 {
		t.Error("Enum 失败")
	}
}

func TestLanguage(t *testing.T) {
	var (
		iValue int = 100 //定义变量而不赋初始值，默认为其类型的null值(整数是0，字符串是零长度字符串)
	)
	iOtherValue := 100 //只可用在函数内的方式(变量类型由初始值推演出来)
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
	str := "this is string" //字符串是常量
	arr := []byte(str)      //转换为字节数组
	arr[0] = 'T'
	fmt.Printf("str is \"%s\"\n", str)   //打印出来的仍是 ”this ..."
	str2 := string(arr)                  //创建新的字符串
	fmt.Printf("str2 is \"%s\"\n", str2) //打印出来的是 "This ..."

	//复数
	var com complex64 = 5 + 5i
	fmt.Printf("复数：5+5i = %v\n", com)
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

func TestMySums(t *testing.T) {
	var iSum int = MySum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
	fmt.Printf("sum is %d\n", iSum)
}

func myFunReturnString(val int) string {
	return fmt.Sprintf("%d", val) //返回格式化的string
}

func TestRange(t *testing.T) {
	//从slice或array返回 int序号 + 值 的键值对
	list := []string{"a", "b", "c", "d", "e", "f"} //创建字符串的slice
	for idx, val := range list {
		fmt.Printf("Range from List[%d]=%s\n", idx, val)
	}

	//从字符串string中返回 int序号 + 字符 的键值对
	//  注意：返回的pos是UTF8的字节位置，而非字符位置; char 是独立的Unicode字符
	//       即输出的 pos 为 { 0, 1, 2,5, 8, 11 } -- 术语为 rune
	for pos, char := range "Go语言测试" {
		fmt.Printf("Character '%c' at position %d\n", char, pos)
	}

	//测试switch...case
	if UnHex('E') != 14 {
		t.Error("Switch case for UnHex Fail")
	}
}

func TestFmtPackage(t *testing.T) {
	//%T -- 打印类型，如 func(),
	fmt.Printf("hello world\n") //等价于 print("hello world")
	fmt.Printf("some string value:%s\n", myFunReturnString(100))

	t.Log("some info in fmt package testing")
}

func TestNumberConverter(t *testing.T) {
	var strTwo string = NumberConverter(2)
	if strTwo != "two" {
		t.Error("在map中存储和查找函数失败")
	}
}

func twiceFunc(num int) (result int) {
	result = num * 2
	return
}

func tripleFunc(num int) (result int) {
	result = num * 3
	return
}

func callback(num int, f func(int) int) int {
	return f(num)
}

func TestCallbackFunc(t *testing.T) {
	//var funs [...]func(int)(int) = {   //这种语法为何编译不过?
	funs := [...]func(int) int{ //定义函数指针数组
		twiceFunc,
		tripleFunc,
	}
	for i := 0; i < len(funs); i++ {
		fmt.Printf("funs[%d]=%d\n", i, funs[i](10))
	}

	numTwice := callback(10, twiceFunc)
	fmt.Printf("twiceFun for 10 is %d\n", numTwice)
}
