/*******************************************************************************
* 工具程序(go tool、gofmt、godoc、gofix) -- 为什么文档中有 8g 等，而我下载的就没有 -- 现在似乎统一成了 go.exe ?
*
* go.exe
*    build xxx.go [-o output] [build flags] [packages] -- 将 .go 文件编译成二进制文件，然后可直接运行
*    run xxx.go -- 编译并运行 go 文件
*******************************************************************************/
package gostudy

import (
	"fmt"
)

func TestTools(t *testing.T) {
	fmt.Printf("Test Tools")
}
