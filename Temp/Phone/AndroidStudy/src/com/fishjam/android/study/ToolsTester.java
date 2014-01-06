package com.fishjam.android.study;
import android.test.AndroidTestCase;

/***************************************************************************************************************************************
 * aapt.exe(Android Asset Packaging Tool) -- 资源(音视频、图片、xml配置等)打包工具? (旧: 可以获取log，安装应用程序，复制文件等等)
 *    aapt p -A assets -S res -M AndroidManifest.xml -F \bin\resources.ap_ -- 将工程下的资源打包为 \bin\resources.ap_ 压缩文件
 * activityCreator.py -- 一个Python脚本，用于为项目创建所有的源代码和目录的编译环境(即产生可用于ant编译的build.xml)
 *    activityCreator.py --out HelloAndroid com.google.android.hello.HelloAndroid  -- 产生 build.xml
 * adb.exe(Android Debug Bridge) -- 提供一个简单的shell环境,可以登录到手机/模拟器上进行各种命令行操作，就像在一台Linux电脑里一样。可以安装一个ARM版的busybox,使用起来更加方便
 *    adb logcat --  可以查看系统log，用于调试。
 *    adb install myapp.apk -- 在CMD环境下将应用程序安装到模拟器 /data/app 目录下，安装前需要先复制到设备上。可以在不退出模拟器的情况下通过安装、删除的方式进行调试。
 *    adb shell -- 进入shell交互模式，可以使用 ls、cd、rm、dmsg 等简单命令；还可以使用 sqlite3 命令访问数据库
 *    adb forward tcp:5555 tcp:8000 -- 设置发布端口，做为主机向模拟器或设备的请求端口
 *    adb push test.txt /sdcard/test.txt -- 复制一个文件或目录到设备或模拟器上
 *    adb pull /android/lib/libwebcore.os .\ -- 从设备或模拟器上复制一个文件或目录到当前目录
 *    adb devices -- 取得当前运行的模拟器、设备的实例列表及每个实例的状态
 *    adb bugreport -- 查看Bug报告
 * ant -- 使用Ant编译，会在bin目录下生成 HelloAndroid.apk 包文件，然后可以用 adb 安装进模拟器。
 *
 * apkbuilder.bat -- 将dx工具制作的.dex 和 appt命令制作的资源文件 打包成 .apk 文件
 * AVD(Android Virtual Device) -- Android虚拟设备，是模拟器的配置，让用户可以更好地模拟真实设备。
 *    包含:硬件配置(如是否有照相机、键盘类型、内存大小等)、版本选择、设备的屏幕尺寸、SD卡大小等。
 *    创建的配置文件保存在%ANDROID_SDK_HOME%环境变量中(如无则是%HOMEPATH%? 中)的 .android\avd 目录下
 * Dalvik VM -- Android中的虚拟机机制，和Java VM类似，但不兼容。专门为移动设备做了优化(基于寄存器的)，相对Java虚拟机速度快很多,执行.dex的Dalvik可执行文件
 * ddms.bat(Dalvik Debug Monitor Service) -- Dalvik 调试监控服务。主要对系统运行后台日志、系统线程、虚拟机状态等的监控? 还可模拟发送短信、拨打电话、发送GPS位置信息等。
 *     (旧: 手机/模拟器的屏幕截图或log)
 * dx.bat -- 将Java编译后的类文件(.class字节码文件)转换成Dalvik虚拟机可执行的.dex(Dalvik Executable Format)文件
 *    如: dx --dex --dump-to=D:\MyAndroid\testProject.dex --core-library D:\MyAndroid\Bin
 * emulator.exe 模拟器软件，几乎提供了大多数物理硬件设备的特性，但不能 接打电话、拍照 等
 *    -wipe-data  -- 把模拟器的设置恢复到初始状态
 *    -sdcard SD.file -- 模拟插入sd卡的情景
 * logcat  -- Debug 工具，显示 android.util.Log 的日志输出。
 *   abd shell 进入交互后，通过 logcat 命令执行。常用参数：
 *     -s -- 设置默认的过滤级别及过滤信息，如 -s "Module1:i"  -- 显示Tag为 "Module1" 中大于等于I(nfo)的信息
 *     -b -- 打开不同的log buffer(位于 /dev/log/ 目录下，有三个：main、radio、events)
 *     -c -- 清除已有的log信息。如：#logcat -cb events #logcat -b events
 *     -d -- 在屏幕上显示log信息，并在信息结束后退出。
 *     -f -- 将log信息作为文件保存起来，可以将log信息较为长久的保存在手机设备中。
 *       # mount -o remount,rw rootfs /      -- 将文件系统改为可读写
 *       # logcat -b events -f /tmp/events   --  将events buffer中的log信息存入 tmp文件夹下的events文件中
 *     -g -- 查看三个log buffer 的大小
 *     -n -- 设置日志的最大数目<count>，默认是4。和 -f 命令配套使用
 *     -r  -- 每 <kbytes>时输出日志，默认值为16。和 -f 命令配套使用
 *     -v -- 设置log的输出格式。brief(默认)、process、tag、thread、raw、time、long
 *            TODO: log级别： V(erbose) 、D(ebug)、I(nfo)、W(arning)、E(rror)、F(atal)、S(ilent) 
 * mksdcard.exe -- 创建sd卡影像文件
 *   mksdcard.exe -l SDCard 512M "E:\Android_SDK\sdcard.img"
 * 随机测试 ： adb shell monkey -p com.fishjam.android.study  -v 500  -- 使用缺省配置，想应用发送500个随机事件(包括按键、touch事件、系统事件等)
 **************************************************************************************************************************************/

public class ToolsTester  extends AndroidTestCase {

}
