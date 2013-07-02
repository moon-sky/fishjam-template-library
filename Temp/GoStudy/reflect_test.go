/*******************************************************************************
注意：反射是把双刃剑，功能强大但可读性不强，如非必要，不推荐使用(最常见的使用场景是对象序列化)
  encoding下的json/xml/gob/binary 等

reflect实现了运行时反射，允许程序通过抽象类型操作对象，通常用于处理静态类型interface{}的值，
  并且通过 Typeof 解析出其动态类型信息，通常会返回一个有接口类型Type的对象,
  没有实现类似Java语言那样的内置类型工厂(即不能通过类型字符串创建对象实例)
  定义结构(struct)时，在每一个变量后可增加
  知道实际的类型，使用 type switch：
    switch f.(type) { case int: f.(int)* 2; case string: f.(string) + f.(string); }
*******************************************************************************/
package gostudy

import (
	"fmt"
	"reflect"
	"testing"
)

func ShowTag(i interface{}) {
	/*
		switch t := reflect.NewValue(i).(type) { //在reflect值上的类型断言
		case *reflect.PtrValue:
			//Elem() 返回v指向的值
			//在Value使用函数Type()返回reflect.Type
			//(*reflect.StructType) 返回reflect.Type
			//通过 StructType.Field(n)返回结构的第n个字段,类型为 StructField
			//通过 StructField.Tag 返回字符串类型的标签名
			tag := t.Elem().Type().(*reflect.StructType).Field(0).Tag

		}
	*/
}

//利用反射打印出定义的矩形(Rect)对象的公有变量?
func TestReflect(t *testing.T) {
	rect := &Rect{20, 30}
	fmt.Printf("TestReflect, rect=%v\n", rect)
	e := reflect.ValueOf(rect).Elem()
	typeofT := e.Type()
	for i := 0; i < e.NumField(); i++ {
		f := e.Field(i)
		fmt.Printf("%d: %s %s = %v\n", i, typeofT.Field(i).Name, f.Type(),
			f.Interface())
	}

}
