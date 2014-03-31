package com.fishjam.android.study;

import android.test.ActivityTestCase;

/***************************************************************************************************************************************
* 经验小结:
*   1.NDK中使用STL -- 在 Application.mk 中增加 APP_STL := stlport_static 或 gnustl_shared 等(有什么区别?)
***************************************************************************************************************************************/


/***************************************************************************************************************************************
 * NDK(Native Development Kit)--http://developer.android.com/tools/sdk/ndk/index.html
 *   官方提供的工具，可以开发 C/C++的动态库，提供 .mk 文件隔离平台、CPU、API等差异(指出编译文件、编译特性)，能自动将so和java应用一起打包成apk。
 *   NDK中提供的API：C标准库(libc), 标准数学库(libm),压缩库(libz),log库(liblog)，媒体库(libjnigraphics), OpenGL ES, JNI接口头文件 等
 *   应用场景：高性能应用逻辑、需要保密的应用逻辑
 *   缺点：程序更复杂(App需要通过JNI调用)，兼容性难以保证，无法访问Framework API，Debug难度大等
 * 
 * JNI(Java Native Interface) -- <jni.h>
 *   函数名 -- 直接供JNI调用的函数命名需要遵循 Java_包名_类名_函数名 的方式，Java中只需要import+直接使用函数名 即可
 *   类型 -- jlong, jobject,jstring 等
 *   JNIEnv* env-- 环境类,是JNI接口的第一个参数，
 *   注意：
 *     1.因为Java有回收机制，不能安全的获得和操作Java对象的指针(如String对应的字符串数组)
 *     2.JNIEnv* evn 的调用方式在 C/C++ 中不一样
 *       C中的调用方法为 (*env)->NewStringUTF(env, "xxx"); C++ 中的调用方法为 env->NewStringUTF("xxx");
 *     3.JNI 接口都是以C方式定义的，因此C++文件中导出C函数，需要用 #ifdef __cplusplus extern "C" { #endif 的方式
 ****************************************************************************************************************************************/
 
/****************************************************************************************************************************************
 * 预定义的编译宏
 *   运行环境(ARM CPU)-- 参见 hello-jni.c
 *     __arm__          -- 对应通用的 armeabi(ARMv5TE)
 *     __ARM_ARCH_7A__  -- 对应 armeabi-v7a(ARMv7)
 *     __ARM_NEON__     -- 对应 armeabi-v7a/NEON , 需要设置 LOCAL_ARM_NEON:=true
 *     __i386__         -- 对应 x86 ，通常是模拟器(i686)?
 *     __mips__         -- 对应 mips(mips32)
****************************************************************************************************************************************/

/****************************************************************************************************************************************
* 环境搭建(Windows 下) -- TODO:似乎不用 cygwin 也可以?
*   1.下载对应平台的SDK、NDK包和STL debug info包；
*   2.安装cygwin(1.7 以上)，http://www.cygwin.com, 需要 GNU Make, AWK, 
*     下载站点：http://mirrors.kernel.org 等比较快
*     选择安装包：autoconf2.1,automake1.10,binutils,gcc-core,gcc,g++,gcc4-core,gcc4-g++,gdb,pcre,pcre-devel,gawk,make 等12个包
*     测试：a.运行Cygwin后，输入 cygcheck -c cygwin 命令，会打印出当前cygwin的版本和运行状态;
*           b.gcc -v; g++ -v; make -v; gdb -v 等测试
*   3.配置NDK环境变量:<cygwin>\<user>\.bash_profile 中增加 NDK=/cygdrive/<ndk_path>; export NDK
*     将 ndk_build 命令所在目录加入 $PATH 变量:export PATH=${PATH}:$NDK
*     测试:  执行 ndk-build 确认
*   4.配置 Eclipse:
*     a.增加 CDT 插件;
*     b.导入项目后, Properties->Builders-> New一个编译器，选择Program -> 
*       名字为C_Builder,Location为<cygwin>/bin/bash.exe,Arguments为 --login -c "cd <projectpath> && $NDK/ndk-build";
*       切换到"Refresh"选项卡，勾选"Refresh resources upon completion";切换到"Build Options"勾选最后三项(具体?);
*       "Specify Resources"->勾选项目目录作为资源目录。
*
* 开发
*   0.Mac/Linux环境下需要执行 build/tools/make-standalone-toolchain.sh --platform=android-8 --install-dir=$HOME/my-android-toolchain
*   1.把原生代码放在 <project>/jni/ 目录下(对应的Java代码一般在 src 目录下),
*     供Java调用的函数名必须遵循 Java_包名_类名_函数名(JNIEnv* env, xxxx) 的格式
*   2.创建 <project>/jni/Android.mk 来定义编译信息(如需要编译的C/C++源文件),详细配置参见 <ndk>/docs/ANDROID-MK.html
*     注意：该文件可能会被分析几次，因此不能假设变量值以前没有定义过。
*     模块描述变量 -- 直接定义，通过 $(变量名) 的方式引用
*       LOCAL_PATH:=$(call my-dir) -- ★必须是第一行★，定义需要编译的C/C++源代码的位置，"my-dir"宏函数由NDK提供，表示当前目录
*       LOCAL_MODULE:=MyModule1 -- 定义模块名,NDK编译时会自动加 lib前缀 和 .so后缀
*       LOCAL_SRC_FILES:=jniSrc.cpp -- 定义需要编译的 C/C++ 文件列表，会自动计算依赖关系
*       LOCAL_CPP_EXTENSION := .cxx .cpp .cc -- 可以指定CPP文件的扩展名，默认是 .cpp
*       LOCAL_CPP_FEATURES := rtti exceptions -- 指定C++特性
*       LOCAL_C_INCLUDES:= $(LOCAL_PATH)/Poco -- 指定头文件的搜索路径
*       LOCAL_CFLAGS|LOCAL_CPPFLAGS -- 指定编译选项
*       LOCAL_STATIC_LIBRARIES|LOCAL_SHARED_LIBRARIES -- 依赖的静态或动态库
*       LOCAL_WHOLE_STATIC_LIBRARIES -- 参见GNU linker 的 --whole-archive，通常用于几个静态库的循环依赖时
*       LOCAL_LDLIBS -- 额外的链接库，如 -L搜索路径, -lz 表示需要链接到 libz.so , -lstdc++ 表示链接到标准C++支持库
*       LOCAL_LDFLAGS -- 额外的链接标志，如 -fuse-ld=gold 表示指定使用 ld.gold 链接器(默认)
*       LOCAL_ALLOW_UNDEFINED_SYMBOLS:=true -- 链接时忽视"undefined symbol"错误，虽然能链接通过，但可能在运行时错误
*       LOCAL_ARM_MODE:=thumb -- 指定ARM目标文件的模式，可选 thumb(缺省,指令为16位模式),arm(,32位指令模式)
*       LOCAL_DISABLE_FORMAT_STRING_CHECKS:=true -- 禁止NDK编译器在printf一类的函数中对"使用非常量的格式化字符串"报错
*       LOCAL_EXPORT_CFLAGS:=-DEXPORT_XXX=1 -- 定义导出时(即其他库会使用本库)会增加到使用方的 LOCAL_CFLAG 中的编译标志。
*         类似的有 LOCAL_EXPORT_CPPFLAGS|LOCAL_EXPORT_C_INCLUDES|LOCAL_EXPORT_LDFLAGS|LOCAL_EXPORT_LDLIBS 等
*       LOCAL_SHORT_COMMANDS:=true -- (常用于Windows,因为命令行只能接收8191个字符)，要求系统编译时内部使用较短的替代名字
*       MY_XXX:=xxxx -- 建议自定义变量的名字以 MY_ 开头(也可以用公司名)，NDK保留了 LOCAL_, PRIVATE_, NKD_, APP_, 小写 等的变量名
*     NDK提供的变量(通过 include $(xxxx) 的方式引用)
*       $(CLEAR_VARS) -- 清空除LOCAL_PATH外所有以"LOCAL_"开始的变量(如 LOCAL_MODULE/LOCAL_SRC_FILES 等)，防止多次分析时出错
*       $(BUILD_SHARED_LIBRARY) -- 表明编译成共享库方式，类似的有 $(BUILD_STATIC_LIBRARY)静态库, 
*         $(PREBUILT_SHARED_LIBRARY)预编译共享库, $(PREBUILT_STATIC_LIBRARY)预编译静态库
*       $(TARGET_ARCH) -- CPU体系架构，如 arm
*       $(TARGET_ARCH_ABI) -- 目录的 CPU+ABI,如 armeabi,armeabi-v7a,x86 等
*     NDK提供的函数宏 -- 通过 $(call xxx) 的方式调用
*       $(call my-dir) -- 返回当前 Android.mk 文件的路径,通常用在定义 LOCAL_PATH 
*       $(call all-subdir-makefiles) -- 返回当前目录下的所有 Android.mk 文件，每个文件可以定义一个module的信息
*       $(call this-makefile) -- 返回当前Makefile的路径，类似的有 parent-makefile, grand-parent-makefile
*       $(call import-module, <name>) -- 根据模块名字找对应的 Android.mk 
*     技巧：
*       a.条件编译(参见 Makefile ): ifneq ($(MY_CONFIG_BAR),) MY_SOURCES += bar.c endif
*   3.创建 <project>/jni/Application.mk，描述了应用程序中需要的原生模块的信息(如静态或共享库),
*     详细配置参见 <ndk>/docs/APPLICATION-MK.html
*     APP_ABI:=all -- 指定编译的ABI目标(即CPU体系架构)，其他的有[armeabi] armeabi-v7a x86
*     APP_CFLAGS:= -- 编译C代码时传递的编译器标识
*     APP_CXXFLAGS|APP_CPPFLAGS:= -- 编译C/C++代码时传递的编译器标识
*     APP_MODULES:=MyModule1 MyModule2 -- 表示Android工程需要调用的非托管模块，对应的动态库文件为 libMyModule1.so
*     APP_OPTIM:=release|debug -- 编译调试版本还是发布版本，缺省是release, 但如果App中android:debuggable为true，
*     APP_PROJECT_PATH:=$(call my-dir)/project -- 指定Android工程所在的目录，编译后APK会自动将库文件拷贝到<app>\libs\armeabi目录中
*   4.cd <project>, 使用 <ndk>/ndk-build 脚本编译, 编译成功后会生成 libs/libMyModule1.so 等文件
*   5.开发使用.so的App,有两种方式：
*     a.使用 JNI 访问(使用 native 声明函数原型)，该方式可以利用 Android framework 的优势;
*       System.loadLibrary("MyModule1"); -- 该函数可在 static 初始化函数中调用加载原生模块
*     b.从 NativeActivity 继承(Android 2.3 以后支持)
*     注意:App的Manifest中需要通过 <uses-sdk android:minSdkVersion="xx" /> 表明使用了native library
*   5.编译打包App，会自动将对应目录下的.so打包到.apk文件中,安装到 /data/data/com.xxx/lib/ 目录下
*
****************************************************************************************************************************************/

/****************************************************************************************************************************************
* Prebuilt -- 预编译库，程序能使用库的预编译版本
*   作用:
*     1.向第三方NDK开发人员发布你的共享库而不用提供源码
*     2.使用提前编译好预编译库来加速编译过程
*   使用方式:
*     1.include $(PREBUILT_SHARED_LIBRARY) 或 $(PREBUILT_STATIC_LIBRARY)
*     2.LOCAL_SRC_FILES:=libfoo.so -- 指定为你要提供的共享库的路径
****************************************************************************************************************************************/

/****************************************************************************************************************************************
* ndk-build 介绍
*   注意：windows下编译过后到cygwin下再编译或clean，会失败("多个目标匹配"), 这时需要手动删除 libs/obj 等编译结果目录后再次编译
*   clean -- 清理
*   NDK_DEBUG=1 -- 生成带调试信息的编码，在代码中会生成"xxx"宏
*   -B V=1 -- rebuild 并且显示详细的编译命令
****************************************************************************************************************************************/

/****************************************************************************************************************************************
* NDK使用的JNI语法
*   日志
*   #define LOGI(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__
****************************************************************************************************************************************/

/****************************************************************************************************************************************
* 远程gdb调试(ndk-gdb),2.2以后才支持
*   1.AndroidManifest.xml 中设置 android:debuggable 为"true";
*   2.编译并在模拟器中运行程序以后，在 应用程序目录运行 ndk-gdb ;
*   3.使用 gdb 命令进行调试
****************************************************************************************************************************************/
public class NdkTester extends ActivityTestCase {

}
