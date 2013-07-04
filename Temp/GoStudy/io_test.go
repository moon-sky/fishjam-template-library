/*******************************************************************************
io
  EOF -- 提供提供的预定义错误，bufio.NewReader(file).ReadLine 等函数返回 err 时可以检查是否是该error
os
  打开文件 -- Open(文件名)/OpenFile(可指定模式)/Create， 注意需要 defer Close
  读写数据 -- Read/Write
  命令行参数 -- Args,其中使用了 flag,其有 Parse(解析标识，并填充变量),
    var port *string = flag.String("port", "80", "Set the web port")
	  //定义标识变量，变量必须是指针，否则package无法设置其值
    flag.Usage = func() { flag.PrintDefaults()} //重定义Usage函数，打印出每个参数的信息
  管道 -- Pipe() 函数创建读写管道，然后可以设置为子进程的输入输出，即可通信
  标准输入输出:Stdin/Stdout

bufio -- 缓冲读写(文件等)数据
  Reader
  Writer

缓冲读写文件的示例:
  f:=os.Open() ; defer f.Close()
  br:=bufio.NewReader(f); line,isPrefix,err:=br.ReadLine();
  w:=bufio.NewWriter(os.Stdout); defer w.Flush()
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
