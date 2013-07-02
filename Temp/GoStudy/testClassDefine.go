package gostudy

import (
	"fmt"
	"math"
)

////////// 定义Shape接口及其方法原型的集合，公有的方法首字母需要大写///////////////////////
type Shape interface {
	Area() float64      //求面积
	Perimeter() float64 //求周长
}

//定义了一个使用Shape接口的方法
func PrintAreaInfo(s Shape) {
	fmt.Printf("%v Area is %f\n", s, s.Area())
}

///////////////////////// 矩形 Rect 及其方法 //////////////////////////////////////
type Rect struct {
	Width  float64
	Height float64
}

//定义Rect的成员函数 -- 注意：虽然 r 是指针，但引用其变量时仍然用点
func (r *Rect) Area() float64 {
	return r.Width * r.Height
}
func (r *Rect) Perimeter() float64 {
	return 2 * (r.Width + r.Height)
}

//实现 Stringer 接口
func (r *Rect) String() string {
	var str string
	str = fmt.Sprintf("[Rect, width = %f, height = %f]", r.Width, r.Height)
	return str
}

//////////////////////// 圆形 Circle 及其方法 ////////////////////////////////////
type Circle struct {
	radius float64
}

func (c *Circle) Area() float64 {
	return math.Pi * c.radius * c.radius
}
func (c *Circle) Perimeter() float64 {
	return 2 * math.Pi * c.radius
}
func (c *Circle) String() string {
	var str string
	str = fmt.Sprintf("[Circle, radius = %f]", c.radius)
	return str
}
