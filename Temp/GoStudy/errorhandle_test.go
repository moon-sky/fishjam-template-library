/*******************************************************************************
错误处理
内置的 error 接口，有一个 Error() string 方法
  defer -- 预设一个延迟调用的函数代码，在当前函数返回时自动执行，确保资源关闭
    (类似 ResourceGuard?)，按照 LIFO(后进先出)的顺序执行
    src, err := os.Open(srcFilePath);  defer src.Close();
  panic -- 不可恢复的错误,创建一个运行时错误并使程序停止，程序代码中应该避免
  recover -- 捕捉panic，仅在defer的函数内部才有效。在panic时重新获得Go程的控制权并恢复正常的执行
    正常的执行过程中，recover会返回nil且没有任何效果；但如果当前的goroutine在panic过程中，可以捕获
	到panic的输入值，并且恢复正常的执行

  if err := file.Chmod(0664); err != nil { xxx }

Go中常见的带错误处理的函数定义原型为：
  func (file *File) Write(b []bype)(n int, err Error)
*******************************************************************************/

package gostudy

import (
	"fmt"
	"testing"
)

func TestErrorHandle(t *testing.T) {
	fmt.Printf("TestErrorHandle\n")
}

func MyDeferFunc() {
	fmt.Printf("this is defer function\n")
}
func TestDefer(t *testing.T) {
	fmt.Printf("Enter Test Defer Function\n")
	//此处定义defer函数，会在整个程序退出(即打印 Leave)后自动执行
	defer MyDeferFunc()

	defer func() {
		//再加入一个匿名的defer函数(LIFO),此处甚至能改变返回值
		fmt.Printf("this is an anonymous defer function\n")
	}()

	fmt.Printf("Leave Test Defer Function\n")
}
