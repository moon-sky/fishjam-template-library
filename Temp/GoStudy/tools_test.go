/*******************************************************************************
工具程序 -- 为什么文档中有 8g/8l 等，而我下载的就没有
  老版本中 6g/6l 表示64位Intel，8g/8l表示32位Intel，现在似乎统一成了 go.exe ?

查看帮助：命令 --help

  go.exe
    build xxx.go [-o output] [build flags] [packages] -- 将 .go 文件编译成二进制文件，然后可直接运行
	help [具体的命令]
	  testfunc -- 测试相关的帮助(test,benchmark,example)
	install <module|bin> -- 安装第三方或自己开发的模块，然后才可使用？安装到%GOPATH%pkg目录下？
    run xxx.go -- 编译、链接并运行go文件(编译到是临时目录下，运行后就删除了?)
	test Xxx|xxx_test.go -- 运行指定目录或_test.go中的单元测试(func TestXxx(*testing.T))
	  -bench regexp -- 进行指定的性能测试(Benchmark)
	version -- 查看当前Go的版本，比如 "go1.1 windows/amd64"
  godoc.exe -- 文档工具(会显示 %GOROOT%src\pkg 下各个包的文档)
    如: godoc fmt Printf
  gofix --
  gofmt -- 格式化源码为官方认可的格式
    -w=true 文件 -- 直接格式化并保存到文件(而不是输出到stdout)
*******************************************************************************/
package gostudy

import (
	"fmt"
	"testing"
)

func TestTools(t *testing.T) {
	fmt.Printf("Test Tools")
}
