/*******************************************************************************
环境搭建 -- Go 1.1 介绍 -- http://blog.jobbole.com/37257/

 1.环境变量,设置GOROOT并将 %GOROOT%bin 加入 PATH 变量
   %GOROOT%=E:\Go\
   其他可设(实际上只要 GOROOT 即可? -- msi 安装程序只设置了这一个变量)
    %GOBIN%=C:\Go\Bin;  %GOARCH%=386|amd64;%GOOS%=mingw|windows;
    %GOPATH% -- 可指定各种package的路径(主要是第三方或自定义package?),go会查找 %GOPATH%/src/目录
	  go tool 的大部分命令都不是针对"当前目录"，而是针对包名(因此需要通过GOPATH指定相关包的路径)
	  Go1.1中go get命令必须有一个合法的GOPATH,并且会在GOPATH和GOROOT设置为相同值的时候报错
    %GOOS%、%GOARCH% -- 交叉编译时(比如GOARCH为ARM,GOOS为linux即为Android手机环境?)

 2.测试:
   命令行(cmd), 切到 %GOROOT%test 目录下，执行 "go run helloworld.go"

工程管理
  Go命令行工具彻底消除了工程文件的概念，完全用目录结构和包名来推导工程结构和构建顺序
  常见目录结构: <src>, <bin>, <pkg>

******************************************************************************/

/******************************************************************************
https://github.com/astaxie/build-web-application-with-golang/blob/9aa3e50854349188fb836e8a7981eea69855a92a/1.4.md

IDE(注意：.go 语言源码需要是 UTF-8 的编码方式，否则即使是注释也不能有中文等 )
 1.Notepad++(5.6.8), 并安装 NppExec 和 Explorer 插件( 插件 -> Plugin Manage -> Show Plugin Manager -> Available -> 选择"NppExec" 并 Install )
   $GOROOT/misc/notepadplus
 2.LiteID -- 基于QT专为Go语言开发的集成开发环境
   https://github.com/visualfc/liteide 或 https://code.google.com/p/liteide/
   注意：需要在 "查看->选项->LiteEnv->xxx.env" 中设置 GOROOT 的路径(自己不认环境变量中的)
   代码自动完成:
     a.设置 GOPATH 变量，必须设置为和 GOROOT不一样
	 b.安装gocode(就是 bin/gocode.exe ?): go get -u github.com/nsf/gocode
 3.Eclipse + GoClipse插件(https://goclipse.googlecode.com/svn/trunk/goclipse-update-site/)
 4.Sublime(http://www.sublimetext.com/)+GoSublime+gocode+MarGo
 5.vim + $GOROOT/misc/vim
   gocode 需要执行 github.com/nsf/gocode/vim/update.bash, 快捷键为 Ctrl+X+O

源码编译(TODO) -- 设置环境后，在 $GOROOT/src 里 ./all.bash

******************************************************************************/

package gostudy

import (
	"fmt"
	"testing"
)

func TestEnv(t *testing.T) {
	t.Log("some info in Env Study")
	fmt.Println("this is in TestEnv")
}
