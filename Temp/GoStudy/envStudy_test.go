/*******************************************************************************
环境搭建
 1.环境变量,设置GOROOT并将 %GOROOT%bin 加入 PATH 变量
   %GOROOT%=E:\Go\
   其他可设(实际上只要 GOROOT 即可? -- msi 安装程序只设置了这一个变量)
    %GOBIN%=C:\Go\Bin;  %GOARCH%=386|amd64;%GOOS%=mingw|windows;
    %GOPATH% -- 可指定各种package的路径(主要是第三方或自定义package?),go会查找 %GOPATH%/src/目录
 2.测试:

   命令行(cmd), 切到 %GOROOT%test 目录下，执行 "go run helloworld.go"
IDE(注意：.go 语言源码需要是 UTF-8 的编码方式，否则即使是注释也不能有中文等 )
 1.Notepad++(5.6.8), 并安装 NppExec 插件( 插件 -> Plugin Manage -> Show Plugin Manager -> Available -> 选择"NppExec" 并 Install )
 2.LiteID -- https://code.google.com/p/liteide/
   注意：需要在 "查看->选项->LiteEnv->xxx.env" 中设置 GOROOT 的路径(自己不认环境变量中的)

源码编译(TODO) -- 设置环境后，在 $GOROOT/src 里 ./all.bash

工具
 go -
******************************************************************************/

package gostudy

import (
	"testing"
)

func TestEnv(t *testing.T) {
	t.Log("some info in Env Study")
}
