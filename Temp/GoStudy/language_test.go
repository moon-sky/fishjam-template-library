//Go 语言简介（下）— 特性 -- http://coolshell.cn/articles/8489.html
//JavaScript中的匿名函数及函数的闭包 -- http://www.cnblogs.com/rainman/archive/2009/05/04/1448899.html

//TODO:
//1.go是静态类型的语言 -- 什么意思(强类型语言)? 对应的还有动态类型的语言(如JavaScript)?

/*******************************************************************************
定义变量(两种方式)
  1.var 变量名 [类型][= 初始值] -- 语法很像javascript(可不指明类型，通过初始化值来推导)
    可通过小括号()来将多个需要声明的变量放在一起，免得重复写 var 关键字
  2.变量名 := 初始值

常见类型(各类型全部独立，混用这些类型向变量赋值会引起编译器错误 -- 强制类型转换: type(value) )：
  bool -- 布尔类型不能接受其他类型的赋值，不支持自动或强制的类型转换
  byte -- uint8 的别名(UTF-8字符串的单个字节)
  complex64(32位虚数部分)/complex128(64位虚数部分) -- 复数
  error -- 错误类型
  int/uint/uintptr(指针) -- 32位系统是32位，但64位系统是64位的?
  int8/int16/int32/uint32/int64/uint64
  float32(等价于C中的float)/float64(等价于C中的double)
  rune -- 单个Unicode字符类型(range通过Unicode字符方式遍历字符串时每一个字符的类型)
  string -- 字符串的内容在初始化后不能修改。即string[0]='S' 是非法的(编译错误)
常量 const ( 常量名 [类型] = 常量值 ),可通过 iota 来定义枚举，如定义时不指定类型则是无类型常量(同字面常量)
匿名变量(_, 下划线)，任何赋给它的值都被丢弃，通常用于不关心的变量，否则会出现编译错误(声明了变量却不使用)

编码支持(https://github.com/qiniu/iconv -- 七牛的)
  Go语言标准库只支持UTF-8和Unicode编码，没有其他的内置编码转换支持，可基于iconv库用Cgo包装
  注意：处于简化语言的考虑，Go语言的很多API都假设字符串为UTF-8编码，很少使用Unicode(但可通过 unicode/utf8包转换)

[]定义了slice, [size]定义了数组，*定义了指针

执行程序的入口点 func 是 main
import "路径" -- import指定路径里的所有go文件(排除 _test.go)

注意：
 1.无论任何时候，你都不应该将一个控制结构（(if、for、switch或select）的左大括号放在下一行。
   如果这样做，将会在大括号的前方插入一个分号，这可能导致出现不想要的结果。
 2.Go的正式的语法使用分号来终止语句，但这些分号由词法分析器在扫描源代码过程中使用简单的规则自动插入分号，源码中就不需要分号了。
 3.通常Go程序仅在for循环语句中使用分号，以此来分开初始化器、条件和增量单元
 4.没有 public/protected/private 的关键字，如果想让方法可以被别的包访问的话，第一个字母必须大写。

内存分配原语 -- 两种内存分配方式区别的原因是 slice,map,chan 使用前必须初始化内部数据结构(非零)
  new(T)--分配内存的内建函数,返回指针，只是将内存清零，而不是初始化内存，
    new(T)返回一个指向新分配的类型为T的零值的指针。
	即使用者可用new创建一个数据结构的实例并且可以直接工作
  make(T, args)--仅用于创建内建的slice、map和chan(消息管道),
    并返回一个被初始化了的(不是零)类型T(不是*T)实例

指针 -- Go有指针，但没有指针运算(从使用上来说，更像引用)，取址操作符(&)，取值操作符(*)

切片 -- 一个具有三项内容的描述符，包括指向数据（在一个数组内部）的指针、长度以及容量
函数 -- func [p 特定类型] 函数名(变量名 变量类型 , ...) (返回值 返回类型,...) { 函数体 }
 可以随意安排函数的顺序 -- 不需要前向声明函数原型
 可选的"特定类型"表示这是特定类型的成员方法(参见 Rect/Circle 成员函数的定义 )
 返回多值 -- 函数中允许返回多个值，由逗号分开，可根据位置或变量名进行赋值返回(未明确赋值的返回值将保持默认的空值)，
   通常很多函数都会返回两个值，一个正常值，一个是错误
 函数不定参数(变参) -- func sum(args ...int)，类型 ...type 本质是一个数组切片，即[]type
   如要支持任意类型的不定参数，可以指定类型为 interface{}, 即 (xxx, args ...interface{}),
     然后使用 switch arg.(type) 来区分处理
   当使用数组、切片等作为参数时，需要在后面增加三个点(...)表示将其中的所有元素打散后作为参数传入

 函数作为值赋值给变量(相当于函数指针),
   pFun := func(param)(returnvalue){xxx}  //定义匿名函数并赋值给变量
   pFun()		      //通过变量调用函数

闭包--可以包含自由(未绑定到特定对象)变量的代码块，这些变量不在这个代码块内或者任何全局上下文中定义，
  而是在定义代码块的环境中定义。要执行的代码块为自由变量提供绑定的计算环境(作用域)。
  闭包的价值在于可以作为函数对象或者匿名函数。支持闭包的多数语言可以将函数存储到变量中作为参数传递
  给其他函数，最重要的是能够被函数动态创建和返回。
  闭包的实现确保只要闭包还被使用，那么被闭包引用的变量会一直存在


   闭包可以毫不费力的将局部变量传递到回调

关键字
  break --可指定标签(label)
  fallthrough -- 用于switch...case中，当匹配失败后自动向下尝试(类似C中没有break时?)
  for -- 循环语句，其后的条件表达式不需要使用圆括号包含，(另外注意:Go不支持while和do-while)
  range--迭代器，可以从array,slice,string,map,channel等需要循环的内容中返回一个键值对
  select(选择不同类型的通讯),chan,
  switch -- 不需要用break来明确退出一个case，case中可以有多个用逗号分开的值，可以通过fallthrough关键字继续尝试

内建函数
  append -- 追加slice
  len/cap -- 返回字符串、slice、数组等的长度/最大容量
  copy -- 复制slice, 并且返回赋值的元素的个数
  print/println -- 底层打印函数，可以在不引入fmt包的情况下使用，主要用于调试
  close/closed,,make,real,
  complex,real,imag -- 处理复数

利用多赋值(是这个名字吗？)，要交换两个变量的值非常简单，不需要定义中间变量
  x, y = y , x

package -- 包是Go语言你最基本的分发单位
定义包时，报名的约定是使用小写字符(不应有下划线或混合大小写)。 import [替代名] "包名"
  导入时(import)可以指定路径(如 "./mypackage")，并可选替代名(通常用于解决冲突)
  包中函数首字母大写时表示是导出的(包外部可见)，私有函数以小写字母开头(只在本包内可见)

定时器：time.NewTimer(只通知一次)或time.NewTicker(持续通知)，可通过Stop()方法停止
  绑定在当前channel中，通过channel的阻塞通知机制来通知程序
  timer := time.NewTimer(2 * time.Second); <-timer.C; {定时满足}
  ticker := time.NewTicker(time.Second); for t := range ticker.C { xxxx }
计算耗时： t0 := time.Now(); xxx; elapsed:=time.Since(t0);
   或 t1:=time.Now(); elapsed:=t1.Sub(t0);
   计算时间段(可用于Sleep等)：time.Duration(nSec * time.Second)
随机数： rand.Seed(time.Now().Unix()); randNum:= rand.Intn(100); //0~99的随机数

*******************************************************************************/
package gostudy

import (
	"fmt"
	//"math"
	"testing"
	//"utf8"
)

//使用 iota 定义枚举,可认为iota是一个可被编译器修改的常量,在每一个const关键字出现时重置为0，否则每次出现都会递增
const (
	//如指定 Monday = 0，是无法定义递增的枚举的，其后的 Sunday 会是默认值(0)
	Monday  = iota //默认值是0，其后的定义会自动递增，也可以手动指定值
	Tuesday        //等价于 Tuesday = iota
	Wednesday
	Thursday
	Friday
	Saturday
	Sunday

	numberOfDays //首字母小写，不被导出的常量，内部使用
)

func TestEnum(t *testing.T) {
	//fmt.Printf("Sunday Number=%d\n", Sunday)
	GOUNIT_ASSERT(t, Sunday == 6, "通过iota定义枚举值")
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

	GOUNIT_ASSERT(t, ^uint32(0) == 0xffffffff, "取反运算, Go中的取反是 ^x, C语言中是 ~x")

	//匿名结构体赋值 -- 问题：定义值得部分似乎不能断行？
	strValue := []struct {
		name string
		age  int
	}{{"fishjam", 30}, {"fuj", 10}}
	GOUNIT_ASSERT(t, strValue[0].name == "fishjam", "匿名结构体")
	GOUNIT_ASSERT(t, strValue[1].age == 10, "匿名结构体")
}

func TestForLanguage(t *testing.T) {
	//通过for对数组进行倒序
	intArray := []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
	for i, j := 0, len(intArray)-1; i < j; i, j = i+1, j-1 {
		intArray[i], intArray[j] = intArray[j], intArray[i]
	}
	fmt.Printf("通过for对数组进行倒序:%v\n", intArray)

	//TODO:有没有方法可以对数组、Slice直接比较其中的值?
	intArrayResult := []int{10, 9, 8, 7, 6, 5, 4, 3, 2, 1}
	for i := 0; i > len(intArray); i++ {
		GOUNIT_ASSERT(t, intArray[i] == intArrayResult[i], "通过for对数组进行倒序")
	}
}
func TestSwitchLanguage(t *testing.T) {
	var iSelNumber int = 3
	var strSelNumber string = ""
	switch iSelNumber {
	case 0:
		strSelNumber = "0"
	case 1:
		strSelNumber = "1"
	case 2:
		fallthrough
	case 3:
		strSelNumber = "2/3"
	case 4, 5, 6:
		strSelNumber = "4/5/6"
	default:
		strSelNumber = "Unknown"
	}
	GOUNIT_ASSERT(t, strSelNumber == "2/3", "Switch Test")
}

//复数
func TestComplex(t *testing.T) {
	var com complex64 = 5.1 + 5.2i
	fmt.Printf("复数：5.1+5.2i = %v\n", com)
	if (real(com) != 5.1) || (imag(com) != 5.2) {
		t.Error("获取实部和虚部")
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
	fmt.Printf("MySum for 1-10 number is %d\n", iSum)

	nums := []int{1, 2, 4}
	iSum = MySum(nums...) //在传递数组作为不定参数时，注意最后需要增加三个点
	fmt.Printf("MySum for 1-4 Array is %d\n", iSum)
}

//测试指针的用法
func TestPointer(t *testing.T) {
	var intValue int = 10
	var pIntValue = &intValue
	(*pIntValue) += 15
	fmt.Printf("intValue=%d, pIntValue=%p, *pIntValue=%d\n", intValue, pIntValue, *pIntValue)

	var v []int = make([]int, 10, 100) // -- 分配一个整型数组，长度为10，容量为100，并返回前10个数组的切片
	for i := 0; i < len(v); i++ {
		v[i] = i
	}

	//定义指针数组
	var pIntArray [10]*int
	for i := 0; i < 10; i++ {
		pIntArray[i] = &v[i]
		GOUNIT_ASSERT(t, *pIntArray[i] == v[i], "比较指针数组中的数据")
	}
}

func TestRange(t *testing.T) {
	//从 array,slice,函数的不定参数 返回 int序号 + 值 的键值对
	list := []string{"a", "b", "c", "d", "e", "f"} //创建字符串的slice
	for idx, val := range list {
		fmt.Printf("Range from List[%d]=%s\n", idx, val)
	}

	//测试switch...case
	if UnHex('E') != 14 {
		t.Error("Switch case for UnHex Fail")
	}
}

func TestString(t *testing.T) {
	var strveryLong = `可以断行
的
多行字符串
`

	strLen := len(strveryLong)
	//10个汉字 * 3 + 三个换行符
	GOUNIT_ASSERT(t, strLen == 33, "通过反斜号可以定义很长的多行字符串")

	strUTF8Info := "Go语言测试"
	var nLen = len(strUTF8Info) //计算字节(byte)数组长度
	GOUNIT_ASSERT(t, nLen == 14, "字符串字节数组长度")
	GOUNIT_ASSERT(t, strUTF8Info[2] == 232, "按索引获取字符串中的字节元素")

	byteStrSlice := make([]byte, nLen)
	for i := 0; i < nLen; i++ {
		byteStrSlice[i] = strUTF8Info[i]
	}

	//TODO: 似乎没有方法能获取到UTF-8字符串中字符的个数？ 只能通过 range ?

	//n := strUTF8Info.Length
	//runeStrSlice := make
	//var cpyStr string = ""
	//从字符串string中返回 int序号 + 字符 的键值对
	//  注意：返回的pos是UTF8的字节位置，而非字符位置; char 是独立的Unicode字符
	//       即输出的 pos 为 { 0, 1, 2,5, 8, 11 } -- 术语为 rune
	for pos, char := range strUTF8Info {
		fmt.Printf("Character '%c' at position %d\n", char, pos)
		//cpyStr += (string)char
	}
	//GOUNIT_ASSERT(t, strUTF8Info == cpyStr,
	//	"通过range遍历字符串中的每一个Unicode字符(rune)")

	byteSilce := []byte(strUTF8Info)
	//[71 111 232 175 173 232 168 128 230 181 139 232 175 149]
	fmt.Printf("byteSilce for strUTF8Info is(%d) %v\n", len(byteSilce), byteSilce)
	//GOUNIT_ASSERT(t, byteStrSlice == byteSilce, "字节数组的两种获取方式")
	covByteStr := string(byteSilce)
	GOUNIT_ASSERT(t, covByteStr == strUTF8Info, "使用string将byte数组转换回字符串")

	//可以转换成[]int32, 每个int32保存Unicode编码
	//[71 111 35821 35328 27979 35797]
	intSilce := []int32(strUTF8Info)
	fmt.Printf("intSlice for strUTF8Info is(%d) %v\n", len(intSilce), intSilce)

	covIntStr := string(intSilce)
	GOUNIT_ASSERT(t, covIntStr == strUTF8Info, "使用string将rune数组转换回字符串")

	GOUNIT_ASSERT(t, fmt.Sprintf("%d", 2012) == "2012", "字符串格式化")
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

//回调函数 -- 能否 typedef ?
func MYCallback(num int, f func(int) int) int {
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

	//传入函数指针，通过回调的方式进行调用
	numTwice := MYCallback(10, twiceFunc)
	fmt.Printf("twiceFun for 10 is %d\n", numTwice)
}

//定义一个返回函数的函数
func plusX(x int) func(int) int {
	return func(y int) int {
		return x + y
	}
}

//测试闭包
func TestClosure(t *testing.T) {
	pFun := plusX(10)
	GOUNIT_ASSERT(t, pFun(20) == 30, "使用闭包")
	GOUNIT_ASSERT(t, plusX(20)(10) == 30, "使用闭包")
}
