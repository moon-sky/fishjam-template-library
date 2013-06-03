/*******************************************************************************
接口( type shape interface { 定义的方法原型， 如 Area() float64 }
实现一个接口只需要定义和接口一样签名的方法，不需要从指定接口继承
使用接口访问对象：s := []shape{ &实例1, &实例2 } for _, sh := range s { xxxx }
Go不支持方法(运算法)重载
*******************************************************************************/

package gostudy

import (
	"fmt"
	"math"
	"testing"
)

//接口 -- 定义方法原型，共有的方法首字母需要大写
type Shape interface {
	Area() float64
}

//结构体
type Rect struct {
	width  float64
	height float64
}

//Rect的方法
func (r *Rect) Area() float64 {
	return r.width * r.height
}

type Circle struct {
	radius float64
}

func (c *Circle) Area() float64 {
	return math.Pi * c.radius * c.radius
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
		fmt.Printf("shape %d Area is %f\n", idx, sh.Area())
	}
}
