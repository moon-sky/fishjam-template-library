/*******************************************************************************
TODO:在需要的时候，可以给任何类型(包括内置类型，但不包括指针类型)增加新方法, 如 http/Header 给 map 增加Add等方法
//定义了以和int本质相同的 Integer，并未其增加了新方法：Less。使用: var a Integer = 1 if a.Less(2){xx}
type Integer int
func (i Integer) Less(other Integer) bool {
	return i < other
}

结构体(struct)

接口( type shape interface { 定义的方法原型， 如 Area() float64 }
  实现一个接口只需要定义和接口一样签名的所有函数，不需要从指定接口继承(duck typing)--非侵入性。
    联合了接口值、静态类型检查、运行时动态转换、无需明确定义类型匹配一个接口--强大、灵活、高效、容易编写
	只要两个接口拥有相同的方法列表(次序无所谓)，则他们是相同的，可以相互赋值；如是方法子集时可以单向赋值
	问题：无法通过检查继承列表就知道该类有哪些功能，必须检测其定义的方法？
  使用接口访问对象：s := []shape{ &实例1, &实例2 } for _, sh := range s { xxxx }
  空接口(interface{})--每个类型都能匹配到空接口(即Any类型), 因此可以创建一个接受空接口作为参数的普通函数,
           然后使用 switch xxx.(type) 找到输入参数实际的类型(如 int, string, 等)
  接口查询： if [shape,?] ok := obj.(IShape|*Rect); ok { obj实例实现了接口IShape|是*Rect实例 }
  接口组合：如 ReadWriter 中 组合了 Reader 和 Writer

Go不支持方法重载, 继承(通过组合方式实现类似继承的功能)、虚函数等常见的OO特性，没有构造和析构

创建函数 -- 有时零值不能满足需求，必须有一个全局的用于初始化的创建函数(通常命名为 NewXxx)。
  如 func NewFile(fd int, name string) *File {
       f := File{ fd, name, nil, 0}; return &f} //注：可以返回本地变量的地址
复合声明 -- ？定义变量并且使用大括号"{}"包含初始化值？
   从复合声明获取分配的实例地址更好(告诉编译器在堆分配内存，而非栈中)
   return &File{fd,name,nil,0}或使用"字段:值"方式初始化 return &File{fd:fd,name:name}

匿名组合(类似继承)：比如 testing.T 继承至 testing.common
 子类可以改写父类的同名方法(如需要调用基类的同名方法，需要手动调用)

类型转换 -- 参见 TestErrorHandle 中将通用的 error 变量err转换成 divByZeroError 类型
*******************************************************************************/

package gostudy

import (
	//"fmt"
	//"math"
	"testing"
)

////////////////////////////// 测试Shape接口 /////////////////////////////////////
func TestShapeForInterface(t *testing.T) {
	var r Rect //:= Rect{ 10.0, 20. 0 } //定义矩形
	r.Height = 10.0
	r.Width = 20.0

	//两种查询类型的方法：
	var typeString string = ""

	var v interface{} = &r
	//1.通过 switch...case可以定位出任意的类型 -- 会以找到的第一个复合的为结果，所以父类应该放在后面
	//  TODO: 为什么检测不出 Rect? 查看Printf 等的源码，看是如何实现查询多种类型的
	//        内置类型(穷举并转换为字符串),一般对象(判断是否实现了String方法),最后利用反射来遍历所有成员变量
	switch v.(type) {
	case Rect:
		typeString += "Rect,"
		GOUNIT_ASSERT(t, false, "RectType")
	case IShape:
		typeString += "IShape,"
		GOUNIT_ASSERT(t, true, "Rect的实例实现了IShape接口")
	case interface{}:
		typeString += "interface{},"
	default:
		typeString += "default,"
		GOUNIT_ASSERT(t, false, "default")
	}
	t.Log("typeString=", typeString)
	GOUNIT_ASSERT(t, typeString == "IShape,", "检查出的类型")

	//方法2：直接查询指定的类型
	pShape, vIsShape := v.(IShape)
	GOUNIT_ASSERT(t, vIsShape, "Rect的实例实现了IShape接口")
	pRect, sIsRectInstance := pShape.(*Rect) //接口查询
	GOUNIT_ASSERT(t, sIsRectInstance, "这里的shape接口指针是*Rect的对象实例")
	GOUNIT_ASSERT(t, pShape == &r, "类型转换,指向同一个Rect对象")
	GOUNIT_ASSERT(t, pRect == &r, "类型转换,指向同一个Rect对象")

	var c Circle //定义圆形
	c.radius = 5

	//利用接口来访问不同对象的方法
	resultData := [2][2]float64{
		{200.0, 60.0},          //矩形的面积和周长
		{78.539816, 31.415927}, //圆的面积和周长
	}
	shapeArr := []IShape{&r, &c}
	for idx, sh := range shapeArr {
		//fmt.Printf("shape %d(%v) Area is %f, Perimeter is %f, result[0]=%f, result[1]=%f\n",
		//	idx, sh, sh.Area(), sh.Perimeter(), resultData[idx][0], resultData[idx][1])

		GOUNIT_ASSERT(t, isEqual(sh.Area(), resultData[idx][0], 0.001), "面积")
		GOUNIT_ASSERT(t, isEqual(sh.Perimeter(), resultData[idx][1], 0.001), "周长")
	}

	//PrintAreaInfo(&r)
	//PrintAreaInfo(&c)

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
