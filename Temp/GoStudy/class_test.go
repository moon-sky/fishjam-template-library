/*******************************************************************************
结构体(struct)

接口( type shape interface { 定义的方法原型， 如 Area() float64 }
  实现一个接口只需要定义和接口一样签名的方法，不需要从指定接口继承(duck typing)--非侵入性。
    联合了接口值、静态类型检查、运行时动态转换、无需明确定义类型匹配一个接口--强大、灵活、高效、容易编写
	问题：无法通过检查继承列表就知道该类有哪些功能，必须检测其定义的方法？
  使用接口访问对象：s := []shape{ &实例1, &实例2 } for _, sh := range s { xxxx }
  空接口 -- 每个类型都能匹配到空接口 interface{}, 因此可以创建一个接受空接口作为参数的普通函数,
           然后使用 switch xxx.(type) 找到输入参数实际的类型(如 int, string, 等)
  检查接口： if ok := obj.(Shape); ok { obj实例实现了接口Shape }

Go不支持方法重载, 继承(通过组合方式实现类似继承的功能)、虚函数等常见的OO特性，没有构造和析构

构造函数 -- 有时零值不能满足需求，必须有一个用于初始化的构造函数。
  如 func NewFile(fd int, name string) *File {
       f := File{ fd, name, nil, 0}; return &f} //注：可以返回本地变量的地址
复合声明 -- ？定义变量并且使用大括号"{}"包含初始化值？
   从复合声明获取分配的实例地址更好(告诉编译器在堆分配内存，而非栈中)
   return &File{fd,name,nil,0}或使用"字段:值"方式初始化 return &File{fd:fd,name:name}

类型转换 -- 参见 TestErrorHandle 中将通用的 error 变量err转换成 divByZeroError 类型
*******************************************************************************/

package gostudy

import (
	"fmt"
	//"math"
	"testing"
)

////////////////////////////// 测试Shape接口 /////////////////////////////////////
func TestShapeForInterface(t *testing.T) {
	var r Rect //:= Rect{ 10.0, 20. 0 } //定义矩形
	r.Height = 10.0
	r.Width = 20.0

	var c Circle //定义圆形
	c.radius = 5

	//利用接口来访问不同对象的方法
	s := []Shape{&r, &c}
	for idx, sh := range s {
		fmt.Printf("shape %d(%v) Area is %f, Perimeter is %f\n",
			idx, sh, sh.Area(), sh.Perimeter())
	}

	PrintAreaInfo(&r)
	PrintAreaInfo(&c)
}

//使用空接口来实现类似模版的功能？ 其他类似的有？Sort( i [] interface{} )
//  但实际上更Go的方法：
//  1.定义一个有若干方法的接口类型，如 type Sorter interface{ Less(i,j int) bool; ... }
//  2.实现对应类型的接口方法，如 func (arr []int )Less(i, j int) bool { return arr[j] < arr[i] }
//  3.编写作用于该接口的通用函数，如 func Sort(s Sorter){ ... if(s.Less(i,j) { s.Swap(i,j) }}
func MyTemplateAdd(any1, any2 interface{}) (result interface{}) {
	switch any1.(type) {
	case int:
		return any1.(int) + any2.(int)
	case string:
		return any1.(string) + any2.(string)
	//对于自定义类型，如 Rect, 如果是new出来的，则应检查 *Rect, 否则应检查 Rect
	default:
	}
	return
}

func TestMyTemplateAdd(t *testing.T) {
	if 3 != MyTemplateAdd(1, 2) {
		t.Error("MyTemplateAdd for int")
	}
	if "hello world" != MyTemplateAdd("hello ", "world") {
		t.Error("MyTemplateAdd for string")
	}
}
