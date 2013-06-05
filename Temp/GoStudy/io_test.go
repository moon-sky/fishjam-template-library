/*******************************************************************************
os
  读写文件 -- Open/Create(defer Close), Read/Write
  命令行参数 -- Args,其中使用了 flag,其有 Parse(解析标识，并填充变量),
    var port *string = flag.String("port", "80", "Set the web port")
	  //定义标识变量，变量必须是指针，否则package无法设置其值
    flag.Usage = func() { flag.PrintDefaults()} //重定义Usage函数，打印出每个参数的信息
  管道 -- Pipe() 函数创建读写管道，然后可以设置为子进程的输入输出，即可通信

bufio
  f:=os.Open(defer Close)->r:=bufio.NewReader(f); w:=bufio.NewWriter(os.Stdout)
    ->defer w.Flush ->
*******************************************************************************/
package gostudy

import (
	"fmt"
	"os"
	"testing"
)

func TestReadWriteFile(t *testing.T) {
	fmt.Printf("Will Read/Write This file")

	buf := make([]byte, 1024)
	if f, _ := os.OpenFile("/etc/passwd", os.O_RDONLY, 0666); f != nil {
		defer f.Close()
		for {
			n, _ := f.Read(buf)
			if n == 0 {
				break
			}
			os.Stdout.Write(buf[0:n])
		}
	}

}
