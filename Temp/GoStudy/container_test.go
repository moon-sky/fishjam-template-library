/*******************************************************************************
array(数组)--由 [n]type 定义，生成固定大小的容器，生成后即不能改变大小，是值类型(赋值和作为函数参数时会复制)，
  数组的长度必须是编译时常量或在编译时能计算出大小的
  可使用方括号完成对元素赋值或索引，注意其序号是 zero-base(零基址)的
  可定义多维数组

slice(切片)--是引用类型(赋值、函数参数、结果返回时元素不会被复制)，
  可随时动态增减元素,扩充时会重新分配并复制内存(类似于 std::vector)
  两种创建方式：
  1.make([]type, len[, cap])直接创建(没有长度限制的数组封装)
  2.contain[n:m] 基于数组或已有切片定义(从序号n到m-1,长度为m-n)
  其内部可抽象为三个变量(1.指向固定长度的array的指针;2.元素个数len;3.已分配的存储空间cap)。
  当使用append追加新的元素时会返回追加后的新的，与原来的slice相同类型的slice(新变量，地址是不同的?)

map -- 由 map[KeyType]ValueType 定义的一堆键值对的未排序集合
  创建：make( map[KeyType]ValueType [, cap] )

在container中提供了如下的容器
  heap
  list
  ring
*******************************************************************************/

package gostudy

import (
	"fmt"
	"testing"
)

func TestArray(t *testing.T) {
	intArray := [...]int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10} //Go自动统计元素个数并初始化
	//var intArray [10]int = [10]int{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
	//intArray := [10]int{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}

	GOUNIT_ASSERT(t, len(intArray) == 10, "计算数组的长度")

	iSum := 0
	for _, val := range intArray { //range返回两个值：下标, 元素值
		iSum += val
	}
	GOUNIT_ASSERT(t, iSum == 55, "通过range遍历数组进行计算")

	iSum = 0
	for i := 0; i < len(intArray); i++ {
		iSum += intArray[i]
	}
	GOUNIT_ASSERT(t, iSum == 55, "通过 len+[下标] 遍历数组进行计算")

	//多维数组 -- 等价的声明方式
	multiArray1 := [2][2]int{[2]int{1, 2}, [2]int{3, 4}}
	multiArray2 := [2][2]int{[...]int{1, 2}, [...]int{3, 4}}
	multiArray3 := [2][2]int{{1, 2}, {3, 4}} //如果元素复合声明的类型与外部一致，可以省略
	if multiArray1 != multiArray2 || multiArray1 != multiArray3 {
		t.Error("多维数组多种等价的声明方式")
	}
}

func TestSlice(t *testing.T) {
	//方法1：直接创建一个保存有10个整数的slice
	intSlice := make([]int, 10, 15) //{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
	for idx := 0; idx < len(intSlice); idx++ {
		intSlice[idx] = idx + 1
	}
	GOUNIT_ASSERT(t, len(intSlice) == 10, "make创建的slice长度")
	GOUNIT_ASSERT(t, cap(intSlice) == 15, "make创建的slice容量")

	//从尾端向数组切片中追加元素(容量够)
	newIntSlice := append(intSlice, 11, 12, 13)
	GOUNIT_ASSERT(t, len(intSlice) == 10, "append后原来的Slice不变")
	GOUNIT_ASSERT(t, len(newIntSlice) == 13, "append后新的Slice")
	GOUNIT_ASSERT(t, cap(newIntSlice) == 15, "cap(newIntSlice) == 15")
	GOUNIT_ASSERT(t, &newIntSlice != &intSlice, "append返回新的切片")
	GOUNIT_ASSERT(t, &newIntSlice[0] == &intSlice[0], "底层的数组是同一个")

	newIntSlice[0] = 99
	GOUNIT_ASSERT(t, newIntSlice[0] == intSlice[0], "未进行内存扩充的情况下新旧Slice指向同一个底层数组")

	//从尾端向数组切片中追加元素(容量不够，会造成扩充)
	newAddressIntSlice := append(intSlice, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20)
	GOUNIT_ASSERT(t, cap(newAddressIntSlice) == 30, "扩充后的长度，目前实现采用容量翻倍的方式")
	newAddressIntSlice[0] = 199
	GOUNIT_ASSERT(t, &newAddressIntSlice[0] != &intSlice[0], "进行了内存扩充，底层数组不再一样")
	GOUNIT_ASSERT(t, newAddressIntSlice[0] != intSlice[0], "进行了内存扩充，底层数组不再一样")

	//方法2:先创建一个array，然后基于该array创建slice
	intArray := [10]int{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
	intSliceFromArray := intArray[0:5] //[:]--基于全部元素, [5:]--从第5个元素开始的所有元素
	GOUNIT_ASSERT(t, len(intSliceFromArray) == 5, "array[n:m]创建的slice长度")
	GOUNIT_ASSERT(t, cap(intSliceFromArray) == 10, "array[n:m]创建的slice容量")
	GOUNIT_ASSERT(t, cap(intArray) == 10, "数组的容量")

	//下面代码会在运行时抛出"index out of range"的异常(panic) -- 因为该slice长度是5(即0~4)
	//intSliceFromArray[5] = 10

	//在slice后追加slice，注意最后的三个点
	S3 := append(newIntSlice, intSliceFromArray...)
	GOUNIT_ASSERT(t, len(S3) == 18 && cap(S3) == 30, "在slice后追加slice")

	t.Logf("Before Remove, S3=%v\n", S3)
	//TODO:从切片中删除指定元素 -- 似乎没有直接的方法，而需要自己根据 index/len 等来判断？
	//<Go语言编程>中第三章结束的例子有错 -- 这里省略了判断 delIndex 有效性的代码
	var delIndex int = 2
	if delIndex < len(S3)-1 { //中间元素
		S3 = append(S3[:delIndex], S3[delIndex+1:]...)
	} else if delIndex == 0 { //删除仅有的一个元素
		S3 = make([]int, 0)
	} else { //删除的是最后一个元素
		S3 = S3[:delIndex-1]
	}
	t.Logf("len(S3)=%d, cap(S3)=%d\n", len(S3), cap(S3))
	GOUNIT_ASSERT(t, len(S3) == 17 && cap(S3) == 30, "从slice中删除一个元素")
	t.Logf("After Remove, S3=%v\n", S3)

	//copy函数，如两个切片不一样大，会按照较小的切片的元素个数进行复制
	var newS = make([]int, 3)
	nCopy := copy(newS, intSlice[0:5])
	GOUNIT_ASSERT(t, nCopy == 3, "只复制3个元素")
}

func TestMap(t *testing.T) {
	//构造方法1
	//var m1 map[string]int = make(map[string]int, 5)
	m1 := make(map[string]int, 5) //定义 string -> int 的映射,初始存储能力为5

	//增加元素
	m1["one"] = 1
	GOUNIT_ASSERT(t, len(m1) == 1, "加入一个新元素")
	//fmt.Printf("cap(m1)=%d\n", cap(m1))
	//GOUNIT_ASSERT(t, cap(m1) == 5, "初始存储能力")

	//检查元素是否存在
	value, bPresent := m1["one"]
	GOUNIT_ASSERT(t, bPresent && (value == 1), "找到map中应该存在的元素")

	if _, bPresent := m1["two"]; bPresent {
		t.Error("找到尚不存在的元素")
	}

	//删除元素 -- 旧的代码是 m1["one"] = nil, false, 已经被淘汰
	delete(m1, "one")
	GOUNIT_ASSERT(t, len(m1) == 0, "删除元素后，m1此时不应该有元素了")

	//构造方法2
	m2 := map[string]int{
		"one":   1,
		"two":   2,
		"three": 3,
	}

	//遍历map容器，range 返回 下标,值 的多值
	for key, val := range m2 {
		fmt.Printf("%s => %d\n", key, val)
	}
}
