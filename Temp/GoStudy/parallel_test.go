/*******************************************************************************
channel -- 通讯序列化过程(Communicating Sequential Processes - CSP)
           用于向系统中其他代理发送和接收特定类型对象的通信机制。
           可用于本地进程间通信(是否可用于分布式通信？)，用于连接到命名的目的地的库方法。

使用 close 关闭，使用 closed 判断关闭状态？
*******************************************************************************/
package gostudy

import (
	"fmt"
	"testing"
)

func TestParallel(t *testing.T) {
	fmt.Printf("Test Parallel")
}
