#ifndef CHROME_SRC_STUDY_H
#define CHROME_SRC_STUDY_H

/************************************************************************************************************
*Chrome源码使用 GYP(Generate Your Projects) 来生成工程文件
*  如果机器上安装了多个版本的VS，可以设置 GYP_MSVS_VERSION 环境变量(如 2010 )生成指定版本的工程
* 
*源码下载：
*  http://dev.chromium.org/developers/how-tos/get-the-code 然后在其中找 "source tarball"
*  解压时需要使用最新的解压工具(比如 http://www.7-zip.org/，但最好先测试是否能一次解压)？否则可能会报错？
*
* 编译(Windows) -- http://www.chromium.org/developers/how-tos/build-instructions-windows
*   0.编译机器：Win7_64 + 8G内存 + 60G可用空间
*   1.准备条件(可选cygwin，但选定后就不能更换):
*     ★先要禁用 病毒程序、Windows 索引服务(开始中搜索 "索引选项/Indexing Options") 等★
*     VS2010(选中X64编译器) + SP1(https://www.microsoft.com/en-us/download/details.aspx?id=23691)
*     Windows 8 SDK(http://msdn.microsoft.com/en-us/windows/hardware/hh852363.aspx)
*       如果没有安装在默认路径，需要设置两个环境变量指向安装的路径：GYP_DEFINES 和 windows_sdk_path
*       对安装的Windows 8 SDK 打补丁(去掉 Include\winrt\asyncinfo.h 66行的枚举类型 AsyncStatus 前的 class，详见官方说明)
*        ? http://download.microsoft.com/download/F/1/0/F10113F5-B750-4969-A255-274341AC6BCE/GRMSDKX_EN_DVD.iso
*     DirectX SDK 2010 June(http://www.microsoft.com/download/en/details.aspx?displaylang=en&id=6812)
*        ? http://download.microsoft.com/download/A/E/7/AE743F1F-632B-4809-87A9-AA1BB3458E31/DXSDK_Jun10.exe
*   可选：Google提供了自动的简单工具链安装脚本(自动安装上面的程序)
*   2.下载安装 depot_tools(命令行方式的自动化工具) -- http://www.chromium.org/developers/how-tos/install-depot-tools
*     TODO: gclient config https://src.chromium.org/chrome/trunk/src -- 什么时候执行(似乎缺省的就OK)？会创建一个 .gclient 文件来标记源码位置
*           svn ls https://src.chromium.org/chrome     -- 接受SSL
*           可手工更改 .gclient 文件，通过 custom_deps 项指定不同步的代码路径(比如 test )，从而减少空间
*     a.下载depot_tools.zip，解压并增加其路径到PATH的最后;
*     b.设置变量 GYP_GENERATORS='ninja,msvs' 来同时允许生成VS工程和ninja工程? -- 是设置环境变量还是 chromium.gyp_env ?
*     c.从命令行运行 gclient , 第一次运行时会自动在其目录下安装 svn,python,git 等
*     d.在 src 目录下晕更新 gclient sync [--force] 可以自动从服务器同步最新的代码, 可选的 --force 参数是什么意思？
*   3.可选的设置编译 Shared Library 等依赖组件(这样最后编译链接chrome时可以使用较小的文件,避免失败)，
*     注意更改以后一般都需要 clean 以清除之前的编译结果。有三种方法(此处只介绍一种)
*     a.在和 .gclient 同级的目录下增加 chromium.gyp_env 文件，其内容为：
*        {'GYP_DEFINES': 'component=shared_library'}
*     b.解决 MS manifest 的问题
*       编辑 src\build\common.gypi 文件，增加 _BIND_TO_CURRENT_CRT_VERSION=1 -- TODO:怎么增加，文档不一样
*     c.打开预编译头(VS2010已经默认打开) -- 
*   4.生成工程文件：gclient runhooks --force
*   5.设置Google API keys(暂时不进行) -- 可在编译时指定，也可在运行时通过环境变量指定
*   6.1.用VS2010打开 chrome/chrome.sln 就可以编译，不过非常慢
*   6.2.加速编译：
*     a.修改 src/build/some.gyp ，只加入关注的工程 -- 参考 all.gyp 和 all_android.gyp
*     b.减少并行数，从而减少同时运行时的内存消耗。
*       MSVC中：Tools\Options\Projects and Solutions\Build and Run -> 减少 "maximum number of parallel project builds"
*       linxu或Cygwin下可以更改 ~/include.gypi 
*       gclient中: -j=N
*     c1.使用 ninja 编译 -- ? ninja -C out\Debug chrome , 但编译的结果不能在VS中调试
*     c2.使用 VS的命令行编译: 在 src\chrome 目录下执行 devenv.exe /build Debug chrome.sln
*   7.运行
*     src/build/Debug/chrome.exe 或 src/build/Release/chrome.exe
*
* Chrome源码分析
*   http://blog.csdn.net/milado_nju/article/details/7216067
* 
* 各种极速浏览器是直接在Chrome上面扩展 -- 可扩展性强？
************************************************************************************************************/

/************************************************************************************************************
* CEF(Chromium Embedded Framework) -- 开源Web browser控件，详见 ftlWeb.h
*
************************************************************************************************************/


#endif	//CHROME_SRC_STUDY_H