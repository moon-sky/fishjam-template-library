/*******************************************************************************
* 错误处理
* 内置的 error 接口，有一个 Error() string 方法
*   defer -- 确保资源关闭(预设一个函数调用，在函数返回式自动执行， 类似 ResourceGuard ?)
*     src, err := os.Open(srcFilePath);  defer src.Close();
*   panic -- 不可恢复的错误,创建一个运行时错误并使程序停止，程序代码中应该避免
*   recover -- 捕捉panic，必须在被 defer的函数内部才有效？ 在 panic 时重新获得Go程的控制权并恢复正常的执行
*******************************************************************************/

package gostudy

import (
	"fmt"
	"testing"
)

func TestErrorHandle(t *testing.T) {
	fmt.Printf("TestErrorHandle\n")
}
