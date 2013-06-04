/*******************************************************************************
工具程序 -- 为什么文档中有 8g/8l 等，而我下载的就没有
  老版本中 6g 表示64位Intel，8g表示32位Intel，现在似乎统一成了 go.exe ?

查看帮助：命令 --help

  go.exe
    build xxx.go [-o output] [build flags] [packages] -- 将 .go 文件编译成二进制文件，然后可直接运行
    run xxx.go -- 编译并运行 go 文件
  godoc.exe -- 文档工具(会显示 %GOROOT%src\pkg 下各个包的文档)
    如: godoc fmt Printf
  gofix --
  gofmt -- 格式化源码为官方认可的格式
*******************************************************************************/
package gostudy

import (
	"fmt"
	"testing"
)

func TestTools(t *testing.T) {
	fmt.Printf("Test Tools")
}
