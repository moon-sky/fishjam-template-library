/*******************************************************************************
Cgo -- 在Go语言中可以使用特定语法混合编写C语言代码，然后Cgo工具可以将这些混合的C代码提取并生成
       对于C功能的调用包装代码。

TODO: can't load package: package .: use of cgo in test cgo_test.go not supported
*******************************************************************************/

package gostudy

import (
	"fmt"
	//"C"
	"testing"
	//"unsafe"
)

func TestUseCInGo(t *testing.T) {
	fmt.Printf("in TestUseCInGo")

	/*
		cstr := C.CString("Hello world")
		C.puts(cstr)
		C.free(unsafe.Pointer(cstr))
	*/
}
