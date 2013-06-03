/*******************************************************************************
array--由 [n]type 定义，生成固定大小的容器，生成后即不能改变大小，是值类型，
  可使用方括号完成对元素赋值或索引，注意其序号是 zero-base(零基址)的
slice -- 由 make([]type, n) 或 array[n:m] 定义(从序号n到m-1,长度为m-n)，是引用类型
  底层是一个指向固定长度的array的指针，
  当使用append追加新的元素时会返回追加后的新的，与原来的slice相同类型的slice
map -- 由 map[Fromtype]ToType 定义
*******************************************************************************/

package gostudy

import (
	"fmt"
	"testing"
)

func TestArray(t *testing.T) {
	iSum := 0
	intArray := [...]int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10} //Go自动统计元素个数并初始化
	for _, val := range intArray {
		iSum += val
	}
	fmt.Printf("TestArray add array number[1~10]=%d\n", iSum)

	//多维数组 -- 等价的声明方式
	multiArray1 := [2][2]int{[2]int{1, 2}, [2]int{3, 4}}
	multiArray2 := [2][2]int{[...]int{1, 2}, [...]int{3, 4}}
	multiArray3 := [2][2]int{{1, 2}, {3, 4}} //如果元素复合声明的类型与外部一致，可以省略
	if multiArray1 != multiArray2 || multiArray1 != multiArray3 {
		t.Error("多维数组多种等价的声明方式")
	}
}

func TestSlice(t *testing.T) {
	//方法1：创建一个保存有10个整数的slice
	intSlice := make([]int, 10) //{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
	for idx := 0; idx < len(intSlice); idx++ {
		intSlice[idx] = idx + 1
	}

	//方法2:先创建一个array，然后基于该array创建slice
	var intArray [10]int // = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
	intSliceFromArray := intArray[0:5]

	_ = cap(intSlice) == cap(intArray)
	if len(intSlice) != cap(intSliceFromArray) {
		t.Error("Slice创建")
	}

	//下面代码会再运行时抛出"index out of range"的异常(panic) -- 因为该slice长度是5(即0~4)
	//intSliceFromArray[5] = 10

	newIntSliceFromArray := append(intSliceFromArray, 6, 7, 8)
	_ = len(newIntSliceFromArray) == len(intSliceFromArray)

	//在slice后追加slice，注意最后的三个点
	S3 := append(newIntSliceFromArray, intSliceFromArray...)
	_ = len(S3)

	//copy 函数
	var newS = make([]int, 5)
	nCopy := copy(newS, intSlice[0:5])
	if nCopy != len(newS) {
		t.Error("copy lenght")
	}
}

//定义 string -> int 的映射
func TestMap(t *testing.T) {
	//方法1
	m1 := make(map[string]int)

	//增加元素
	m1["one"] = 1

	//检查元素是否存在
	if _, bPresent := m1["one"]; !bPresent {
		t.Error("没有找到map中应该存在的元素")
	}

	if _, bPresent := m1["two"]; bPresent {
		t.Error("找到尚不存在的元素")
	}

	//删除元素 -- 旧的代码是 m1["one"] = 0, false, 已经被淘汰
	delete(m1, "one")
	if len(m1) > 0 {
		t.Error("m1 此时不应该有元素了")
	}

	//方法2
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
