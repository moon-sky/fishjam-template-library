/*******************************************************************************
Cgo -- 在Go语言中可以使用特定语法混合编写C语言代码，然后Cgo工具可以将这些混合的C代码提取并生成
       对于C功能的调用包装代码。

类型映射：
  C语言中的void*指针类型在Go语言中用特殊的 unsafe.Pointer 类型来对应
  C语言中的 struct,union,enum 类型对应到Go语言中都会变成带前缀(struct_,union_,enum_)的类型名称
  字符串映射: C.CString/C.GoString/C.GoStringN -- 每次转换都会导致一次内存复制，使用完毕后
    必须通过 C.fre 显式释放( defer C.free(unsafe.Pointer(cstr))

#cgo -- 伪C语法，可以指定第三方依赖库或编译选项等，如


TODO: can't load package: package .: use of cgo in test cgo_test.go not supported
*******************************************************************************/

package gostudy

import (
	"fmt"
	"testing"
	//"unsafe"
)

/*
#include <stdlib.h>

//任何合法的C代码
void hello(){ printf("Hello, Cgo\n"); }

#cgo CFLAGS: -DPNG_DEBUG=1
#cgo LDFLAGS: -lpng
#include <png.h>  或 #cgo pkg-config: png

*/
//import "C"

//注意:不存在名为C的包，import C 只是通知Cgo:
// 对应本import语句前的块注释的C源代码，自动生成包装性质的Go代码

func TestUseCInGo(t *testing.T) {
	fmt.Printf("in TestUseCInGo\n")

	/*
		var iSeed = C.srandom(C.uint(100))
		var iRandom = C.random()

		cstr := C.CString("Hello world")
		defer C.free(unsafe.Pointer(cstr))
		C.puts(cstr)

		C.hello()
	*/
}
