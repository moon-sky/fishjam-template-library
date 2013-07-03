/*******************************************************************************
错误处理
内置的 error 接口 -- type error interface{ Error() string; }
  可以自定义包含额外信息的错误结构( myError{ arg int; errMsg string; })，并实现 Error 方法
  返回系统错误:   return -1, errors.New("Some bad information")
  返回自定义错误: return -1, &myError{ arg, "some other info"}
  正确返回时:    return xxx, nil
  调用示例： if nResult, err := obj.Chmod(0664); err != nil { 错误处理 }

defer -- 预设一个延迟调用的函数代码，在当前函数返回时自动执行，确保资源关闭
  (类似 ResourceGuard?)，按照LIFO(后进先出)的顺序执行
  src, err := os.Open(srcFilePath);  defer src.Close(); 或 defer func(){ 匿名函数进行清理 }()

panic(interface{}) -- 不可恢复的错误,创建一个运行时错误并使程序停止，程序代码中应该避免
recover() interface{} -- 捕捉panic，仅在defer的函数内部才有效。在panic时重新获得Go程的控制权并恢复正常的执行
  正常的执行过程中，recover会返回nil且没有任何效果；但如果当前的goroutine在panic过程中，可以捕获
  到panic的输入值，并且恢复正常的执行, 如 if r := recover(); r != nil{ recovered in funciton }


Go中常见的带错误处理的函数定义原型为：
  func (file *File) Write(b []bype)(n int, err Error)
*******************************************************************************/

package gostudy

import (
	//"errors"
	"fmt"
	"testing"
)

type divByZeroError struct {
	dividend int //被除数
}

func (e *divByZeroError) Error() string {
	return fmt.Sprintf("can not div zero for %d", e.dividend)
}

//多重返回 + 错误处理机制
func divWithError(a, b int) (ret int, err error) {
	if b == 0 {
		err = &divByZeroError{a} // errors.New("")
		return
	}
	return a / b, nil
}

func TestErrorHandle(t *testing.T) {
	_, err := divWithError(10, 0)
	GOUNIT_ASSERT(t, err != nil, "除数为零的除法会设置error")
	if err != nil {
		e, ok := err.(*divByZeroError)
		GOUNIT_ASSERT(t, ok && e.dividend == 10, "详细错误的类型转换")
		fmt.Printf("TestErrorHandle err is \"%v\"\n", err)
	}
	ret, err := divWithError(10, 5)
	GOUNIT_ASSERT(t, ret == 2 && err == nil, "除数不为零")
}

func TestDefer(t *testing.T) {
	fmt.Printf("Enter Test Defer Function\n")
	//此处定义defer函数，会在整个程序退出(即打印 Leave)后自动执行
	defer func() {
		fmt.Printf("this is defer function\n")
	}()

	defer func() {
		//再加入一个匿名的defer函数(LIFO),此处甚至能改变返回值
		fmt.Printf("this is an anonymous defer function\n")
	}()

	fmt.Printf("Leave Test Defer Function\n")
}
