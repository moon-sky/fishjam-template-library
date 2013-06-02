package gostudy

import (
	"fmt"
	"testing"
)

//定义 string -> int 的映射
func TestMap(t *testing.T) {
	//方法1
	m1 := make(map[string]int)

	m1["one"] = 1
	delete(m1, "one")

	//方法2
	m2 := map[string]int{"one": 1, "two": 2, "three": 3}

	//遍历map容器，range 返回 下标,值 的多值
	for key, val := range m2 {
		fmt.Printf("%s => %d\n", key, val)
	}
}
