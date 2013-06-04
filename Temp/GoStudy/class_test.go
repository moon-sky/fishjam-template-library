/*******************************************************************************
接口( type shape interface { 定义的方法原型， 如 Area() float64 }
  实现一个接口只需要定义和接口一样签名的方法，不需要从指定接口继承(duck typing),
    联合了接口值、静态类型检查、运行时动态转换、无需明确定义类型匹配一个接口--强大、灵活、高效、容易编写
  使用接口访问对象：s := []shape{ &实例1, &实例2 } for _, sh := range s { xxxx }
  空接口 -- 每个类型都能匹配到空接口 interface {}, 因此可以创建一个接受接口作为参数的普通函数
  检查接口： if ok := obj.(Shape); ok { obj实例实现了接口Shape }

Go不支持方法(运算法)重载

构造函数 -- 有时零值不能满足需求，必须有一个用于初始化的构造函数。
  如 func NewFile(fd int, name string) *File {
       f := File{ fd, name, nil, 0}; return &f} //注：可以返回本地变量的地址
复合声明 -- ？定义变量并且使用大括号"{}"包含初始化值？
   从复合声明获取分配的实例地址更好(告诉编译器在堆分配内存，而非栈中)
   return &File{fd,name,nil,0}或使用"字段:值"方式初始化 return &File{fd:fd,name:name}
*******************************************************************************/

package gostudy

import (
	"fmt"
	"math"
	"testing"
)

//接口 -- 定义方法原型的集合，共有的方法首字母需要大写
type Shape interface {
	Area() float64
}

func CalcArea(s Shape) {
	fmt.Printf("%v Area is %f\n", s, s.Area())
}

//结构体
type Rect struct {
	width  float64
	height float64
}

//定义Rect的成员函数 -- 注意：虽然 r 是指针，但引用其变量时仍然用点
func (r *Rect) Area() float64 {
	return r.width * r.height
}

//实现 Stringer 接口
func (r *Rect) String() string {
	var str string
	str = fmt.Sprintf("[Rect, width = %f, height = %f]", r.width, r.height)
	return str
}

type Circle struct {
	radius float64
}

func (c *Circle) Area() float64 {
	return math.Pi * c.radius * c.radius
}

func (c *Circle) String() string {
	var str string
	str = fmt.Sprintf("[Circle, radius = %f]", c.radius)
	return str
}

func TestShapeForInterface(t *testing.T) {
	var r Rect
	r.height = 10.0
	r.width = 20.0

	var c Circle
	c.radius = 5

	//利用接口来访问不同对象的方法
	s := []Shape{&r, &c}
	for idx, sh := range s {
		fmt.Printf("shape %d(%v) Area is %f\n", idx, sh, sh.Area())
	}

	CalcArea(&r)
	CalcArea(&c)
}
