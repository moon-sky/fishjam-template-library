/*******************************************************************************
工具程序 -- 为什么文档中有 8g/8l 等，而我下载的就没有
  老版本中 6g/6l 表示64位Intel，8g/8l表示32位Intel，现在似乎统一成了 go.exe ?

查看帮助：命令 --help

  go.exe
    build xxx.go [-o output] [build flags] [packages] -- 将 .go 文件编译成二进制文件，然后可直接运行
	doc -- 运行 godoc 生成文档
	fmt -- 以参数"-l -w"运行 gofmt 来进行代码格式化
	get -- 下载或安装指定包
	help [具体的命令]
	  testfunc -- 测试相关的帮助(test,benchmark,example)
	install <module|bin> -- 安装第三方或自己开发的模块，然后才可使用？安装到%GOPATH%pkg目录下？
    run xxx.go -- 编译、链接并运行go文件(编译到是临时目录下，运行后就删除了?)
	test Xxx|xxx_test.go -- 运行指定目录或_test.go中的单元测试(func TestXxx(*testing.T))
	  -bench regexp -- 进行指定的性能测试(Benchmark)
	version -- 查看当前Go的版本，比如 "go1.1 windows/amd64"
  godoc.exe -- 文档工具(会显示 %GOROOT%src\pkg 下各个包的文档)
    如: godoc fmt Printf
	    godoc -http=:8080 <== 会在本地8080端口上建立帮助的Web服务器
  gofix --
  gofmt -- 格式化源码为官方认可的格式
    -w=true 文件 -- 直接格式化并保存到文件(而不是输出到stdout)

*******************************************************************************/

/*******************************************************************************
文档注释的基本规则
  1.注释需要紧贴在对应的包声明和函数之前，不能有空行
  2.注释如果要新起一个段落，应该用一个空白注释行隔开，因为直接换行书写会被认为是正常的段内折行。
  3.可以直接在代码内用 "//BUG(autho):" 等方式记录额外信息，这些信息也会被抽取到文档中。
*******************************************************************************/
package gostudy

import (
	"fmt"
	"testing"
)

func TestTools(t *testing.T) {
	fmt.Printf("Test Tools")
}
