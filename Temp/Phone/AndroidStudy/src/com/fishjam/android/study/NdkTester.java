package com.fishjam.android.study;

import android.test.ActivityTestCase;

/***************************************************************************************************************************************
 * NDK(Native Development Kit)--http://developer.android.com/tools/sdk/ndk/index.html
 *   官方提供的工具，可以开发 C/C++的动态库，提供 .mk 文件隔离平台、CPU、API等差异(指出编译文件、编译特性)，能自动将so和java应用一起打包成apk。
 *   NDK中提供的API：C标准库(libc), 标准数学库(libm),压缩库(libz),log库(liblog)，媒体库(libjnigraphics), OpenGL ES, JNI接口头文件 等
 *   应用场景：高性能应用逻辑、需要保密的应用逻辑
 *   缺点：程序更复杂(App需要通过JNI调用)，兼容性难以保证，无法访问Framework API，Debug难度大等
 * 
 * JNI -- 
****************************************************************************************************************************************/
/****************************************************************************************************************************************
* 环境搭建(Windows 下) -- TODO:似乎不用 cygwin 也可以?
*   1.下载对应平台的SDK、NDK包和STL debug info包；
*   2.安装cygwin(1.7 以上)，http://www.cygwin.com, 需要 GNU Make, AWK, 
*     下载站点：http://mirrors.kernel.org 等比较快
*     选择安装包：autoconf2.1,automake1.10,binutils,gcc-core,gcc,g++,gcc4-core,gcc4-g++,gdb,pcre,pcre-devel,gawk,make 等12个包
*     测试：a.运行Cygwin后，输入 cygcheck -c cygwin 命令，会打印出当前cygwin的版本和运行状态;
*           b.gcc -v; g++ --version; make --version; gdb --version 等测试
*   3.配置NDK环境变量:<cygwin>\<user>\.bash_profile 中增加 NDK=/cygdrive/<ndk_path>; export NDK
*     测试: cd $NDK<CR> 确认是否到对应的目录
*     TODO: 将 ndk_build 命令所在目录加入 $PATH 变量? export PATH=${PATH}:$NDK
*   4.配置 Eclipse:
*     a.增加 CDT 插件;
*     b.导入项目后, Properties->Builders-> New一个编译器，选择Program -> 
*       名字为C_Builder,Location为<cygwin>/bin/bash.exe,Arguments为 --login -c "cd <projectpath> && $NDK/ndk-build";
*       切换到"Refresh"选项卡，勾选"Refresh resources upon completion";切换到"Build Options"勾选最后三项(具体?);
*       "Specify Resources"->勾选项目目录作为资源目录。
*
* 开发
*   0.Mac/Linux环境下需要执行 build/tools/make-standalone-toolchain.sh --platform=android-8 --install-dir=$HOME/my-android-toolchain
*   1.把原生代码放在 <project>/jni/ 目录下
*   2.创建 <project>/jni/Android.mk 来定义编译信息,详细配置参见 <ndk>/docs/ANDROID-MK.html
*   3.[可选]创建 <project>/jni/Application.mk，详细配置参见 <ndk>/docs/ANDROID-MK.html
*   4.cd <project>, 使用 <ndk>/ndk-build 脚本编译, 编译成功后会生成 libs/Xxx.so 
*   5.开发使用.so的App,有两种方式：
*     a.使用 JNI 访问，该方式可以利用 Android framework 的优势;
*     b.从 NativeActivity 继承(Android 2.3 以后支持)
*     注意:App的Manifest中需要通过 <uses-sdk android:minSdkVersion="xx" /> 表明使用了native library
*   5.编译打包App，会自动将对应目录下的 .so 打包到 .apk 文件中
****************************************************************************************************************************************/
public class NdkTester extends ActivityTestCase {

}
