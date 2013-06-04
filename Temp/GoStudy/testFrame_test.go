/*******************************************************************************
测试框架
  以 *_test.go 命名的文件,其中的测试函数有相同的标识
    func TestXxx(t * testing.T) { xxx }
	T::Fail -- 测试失败并继续执行后续测试
	T::FailNow -- 测试失败并中断当前文件的其他测试，而继续执行其他测试？
	T::Log -- 用默认格式对参数进行格式化，并记录文本到错误日志
	T::Fatal -- 等价于 Log + FailNow
*******************************************************************************/

package gostudy

import (
	//"fmt"
	"testing"
)

func TestGoTestFrame(t *testing.T) {
	//t.Errorf("some info in TestGoTestFrame")
}
