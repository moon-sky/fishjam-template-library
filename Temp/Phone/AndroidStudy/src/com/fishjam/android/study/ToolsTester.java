package com.fishjam.android.study;
import android.test.AndroidTestCase;

/***************************************************************************************************************************************
 * aapt.exe(Android Asset Packaging Tool) -- 资源(音视频、图片、xml配置等)打包工具? (旧: 可以获取log，安装应用程序，复制文件等等)
 *    子命令:
 *       a -- add, 向压缩包中添加指定文件
 *       d --dump 导出APK包内的指定内容
 *       l -- list, 列出资源压缩包内的内容
 *       p -- package,打包生成资源压缩文件
 *       v -- version, 打印aapt的版本 
 *    如：aapt p -A "附件资源路径如assets" -S "资源路径如res" -M AndroidManifest.xml -I "额外的包如 android-9\android.jar" 
 *       -F \bin\resources.ap_ -- 将工程下的 assets子目录、res子目录、AndroidManifest.xml 等资源打包为 \bin\resources.ap_ 压缩文件
 * activityCreator.py -- 一个Python脚本，用于为项目创建所有的源代码和目录的编译环境(即产生可用于ant编译的build.xml)
 *    activityCreator.py --out HelloAndroid com.google.android.hello.HelloAndroid  -- 产生 build.xml
 * adb.exe(Android Debug Bridge) -- 提供一个简单的shell环境,可以登录到手机/模拟器上进行各种命令行操作，就像在一台Linux电脑里一样。
 *  可以安装一个ARM版的busybox,使用起来更加方便
 *    adb bugreport -- 查看Bug报告
 *    adb devices -- 取得当前运行的模拟器、设备的实例列表及每个实例的状态
 *    adb forward tcp:5555 tcp:8000 -- 设置发布端口，做为主机向模拟器或设备的请求端口
 *    adb install [-r] [-s] myapp.apk -- 在CMD环境下将应用程序安装到模拟器 /data/app 目录下，安装前需要先复制到设备上。
 *      可以在不退出模拟器的情况下通过安装、删除的方式进行调试。
 *      -r -- 重新安装该APK包; -s -- 将APK包安装到SDK卡上(默认安装到内部存储器上)
 *    adb logcat --  可以查看系统log，用于调试。
 *    adb pull /android/lib/libwebcore.os .\ -- 从设备或模拟器上复制一个文件或目录到当前目录
 *    adb push D:/test.txt /sdcard/test.txt -- 复制一个文件或目录到设备或模拟器上
 *    adb remount -- TODO: 加载指定设备，可以解决 adb shell 后 rm 等的权限问题? 如 rm /system/app/Launcher.apk
 *    adb shell -- 进入shell交互模式(会进入设备的根目录 / ?)，可以使用 ls、cd、rm、dmsg 等简单命令；还可以使用 sqlite3 命令访问数据库
 *    adb uninstall [-k] <包名> -- 从系统中删除指定软件包。 -k 表示只删除该应用程序，但保留其使用的数据和缓存目录
 * android.bat -- 直接运行会启动Android SDK管理器(TODO: 官方有帮助?)
 *   list [avds|targets]-- 列出机器上所有已经安装的 AVD设备 和|或 Android版本
 *   create|move|delete avd  -- 创建|移动或重命名|删除 一个AVD设备
 *     --name Android20 --target 5 <== create 时的参数
 *   create|update project -- 创建|更新  一个新的Android项目，会提供Ant生成文件(build.xml)，然后可通过Ant来生成、安装项目。
 *   create|update test-project -- 创建|更新  一个新的Android测试项目   
 * ant.bat -- 使用Ant编译，会在bin目录下生成 HelloAndroid.apk 包文件，然后可以用 adb 安装进模拟器。
 *   build.xml 中包含的 target: clean,debug,release,test,install,uninstall
 * apkbuilder.bat -- 将dx工具制作的.dex 和 appt命令制作的资源文件 打包成 .apk 文件
 * AVD(Android Virtual Device) -- Android虚拟设备，是模拟器的配置，让用户可以更好地模拟真实设备。
 *    包含:硬件配置(如是否有照相机、键盘类型、内存大小等)、版本选择、设备的屏幕尺寸、SD卡大小等。
 *    创建的配置文件保存在%ANDROID_SDK_HOME%环境变量中(如无则是%HOMEPATH%? 中)的 .android\avd 目录下
 *    允许DPad键：打开 .android\avd\xxxx.avd\config.ini 文件，将 hw.dPad=no 改为 hw.dPad=yes
 * Dalvik VM -- Android中的虚拟机机制，和Java VM类似，但不兼容。专门为移动设备做了优化(基于寄存器的)，相对Java虚拟机速度快很多,执行.dex的Dalvik可执行文件
 * ddms.bat(Dalvik Debug Monitor Service) -- Dalvik 调试监控服务。主要对系统运行后台日志、系统线程、虚拟机状态等的监控? 还可模拟发送短信、拨打电话、发送GPS位置信息等。
 *     (旧: 手机/模拟器的屏幕截图或log)
 * dmtracedump -- 从trace文件中生成函数调用图(似乎是个失败的工具？替换工具：http://blog.csdn.net/zjujoe/article/details/6080738)
 * draw9patch.bat-- 制作9Patch图片的工具，http://blog.csdn.net/xiaominghimi/article/details/6107837
 * dx.bat -- 将Java编译后的类文件(.class字节码文件)转换成Dalvik虚拟机可执行的.dex(Dalvik Executable Format)文件
 *      dx --dex [--dump-to=<目的.dex文件>] [--core-library <file>.class | f<file>.{zip,jar,apk} | <directory> ]
 *      如: dx --dex --dump-to=D:\MyAndroid\testProject.dex --core-library D:\MyAndroid\Bin
 * emulator.exe 模拟器软件，几乎提供了大多数物理硬件设备的特性，但不能 接打电话、拍照 等
 *    -avd <AVD名> -- 运行指定名字的AVD设备
 *    -data <镜像文件路径> -- 直接使用指定镜像文件来运行AVD
 *    -wipe-data  -- 把模拟器的设置恢复到初始状态
 *    -sdcard sdcard.img -- 加载SD卡的镜像文件
 * jarsigner.exe -- 对未签名的APK安装包进行签名，会以交互方式让用户输入密码(能否自动化?)
 *   -verbose -- 指定生成详细输出
 *   -keystore -- 指定数字证书的存储路径
 *   -signedjar <签名后的APK包> <未签名的APK包> <数字证书的别名> -- 
 *    签名: jarsigner -verbose -keystore 证书文件(如fishjam.keystore) -signedjar HelloWorld_Signed.apk HelloWorld.apk 别名(如fishjam)
 * keytool -- 数字证书工具
 *   -genkeypair -- 指定生成数字证书
 *   -alias -- 指定生成数字证书的别名
 *   -keyalg -- 指定生成证书的算法，如 RSA
 *   -validity -- 指定有效期
 *   -keystore -- 指定证书文件的存储路径
 *    生成证书: keytool -genkeypair -alias 别名(如fishjam) -keyalg RSA -validity 400 -keystore 文件路径(如 fishjam.keystore) -- 执行后向导模式输入公司等信息
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
 * mksdcard.exe -- 创建sd卡镜像文件
 *   mksdcard.exe [-l "Label"] <大小如512M> <路径，如 E:\Android_SDK\sdcard.img>
 * monitor.bat -- Android Debug Monitor,用于调试监控(代替ddms.bat)
 * traceview.bat -- 用于对Android的应用程序以及Framework层的代码进行性能分析。
 *   1.修改代码，在需要调试的起始位置加入调试函数( Debug.startMethodTracing/stopMethodTracing )
 *   2.运行程序，会在SD的根目录下产生*.trace文件来保存运行时的数据，
 *   3.将*.trace拷贝到PC上，然后 traceview.bat xxxx.trace 文件进行分析(TODO: 路径必须用绝对路径？)
 * zipalign.exe -- 档案整理工具，可用于优化APK安装包,从而提升Android应用与系统之间的交互效率和运行速度
 *   -f -- 指定强制覆盖已有的文件
 *   -v -- 指定生成详细输出
 *   4 -- 指定档案整理所基于的字节数，通常指定为4(即基于32位进行整理)
 *   zipalign.exe -f -v 4 HelloWorld_Signed.apk HelloWorld_Signed_zip.apk
 * 
 * 随机测试 ： adb shell monkey -p com.fishjam.android.study  -v 500  -- 使用缺省配置，想应用发送500个随机事件(包括按键、touch事件、系统事件等)
 * 查看应用内存占用情况：
 *   1.使用Eclipse插件MAT
 *   2.adb shell dumpsys meminfo <package_name>
 **************************************************************************************************************************************/

public class ToolsTester  extends AndroidTestCase {

}
