/******************************************************************************************************************************
* 其他类似的安装程序制作工具有：Inno Setup,  wix(微软)
* NSIS(Nullsoft Scriptable Install System) -- http://nsis.sf.net, 开源的 Windows 系统下免费安装程序制作程序, 通过脚本配置
* 最佳实践
*   1.加入 !define MUI_VERBOSE 4 , 这样在编译时会输出详细的编译信息(本质是控制!verbose), 方便查找问题和学习。但发布时最好取消, 以免产生过多编译信息
*   2.
* TODO：
*   0.怎么查看 !insertmacro 后展开的结果?
*   1.Unicode版本？
*   2.MUI_ABORTWARNING -- TODO: 在 System.nsh 中是个宏, 但很多地方都写 !define MUI_ABORTWARNING , 是为了取消宏定义还是说两个正好同名?
*   3.卸载信息(一般都写在 HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\ 子目录下)
*
*   NSIS帮助文档(蓝色网际)：通过查看 System.nsh 等系统源码文件, 可以学到很多用法
*   编译方式：
*     makensis.exe [option | script.nsi ] -- 命令行的编译器
*     makensisw.exe -- GUI的编译器
*     可选参数:
*       /Vx -- 控制编译时输出信息的详细程度(等价于 !define MUI_VERBOSE 常量). x具体值有 4(all),3(no script),2(no info),1(no warnings),0(none).
*       /Ofile -- 指定文本文件记录日志, 如 /OResult.log
*   源码编译：使用 python + scons 编译
*   注意：
*     1.函数没有参数传递。
*       参数传递是通过 全局变量 或 堆栈操作 Pop, Push, Exch 和 20 个寄存器变量 $0～$9, $R0～$R9 进行
*     2.三种压缩算法(ZLib, BZip2, LZMA) -- LZMA 压缩具有比其它通用压缩方法更好的效果, 通过 SetCompressor 设置
*     3.可通过"XXXX"实现自定义对话框和界面
*   常见问题:
*     1.SetOverwrite -- 设置是否覆盖已经存在的文件。可选值:[on(直接覆盖,如不能覆盖的话提示)],off(不覆盖),try(尽力覆盖),ifnewer(较新)
*     2.设置网页链接的快捷方式 -- WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
*     3.若要自定义界面，可以查看 Contrib\Graphics 下的 Graphics 和 Language files 目录内容，确认使用了哪个资源，
*       然后在 System.nsh 中查找用法并对应定义相关的常量或宏
*   静默安装(silent installations) -- 程序全部按照NSIS给予的默认配置进行安装, 安装过程也不显示, 全部后台运行
*     启动参数 /silent ? 或 /M ?
*
*   脚本(.nsh/.nsi -- 如果 makensis.exe 同目录下有 nsisconf.nsh 头文件, 会自动包含)
*     第三方的脚本开发集成环境：
*       HM NIS EDIT(有创建基础脚本的向导, F9预览结果, nisedit2.0.3.exe) -- http://sourceforge.net/projects/hmne/ (http://hmne.sourceforge.net/ -- 旧?)
*         TODO: 安装好以后执行 NSIS-> 配置 -> 设置 nsis 的安装和执行路径
*         使用时非常方便, 可通过 Ctrl + 鼠标 进行跳转; 直接查看命令的帮助
*         开源(尝试修改代码来支持): http://ftp.jaist.ac.jp/pub/sourceforge/h/hm/hmne/HM%20NIS%20Edit/2.0.3/nisedit203-src.zip
*           1.不支持 Unicode 或 UTF-8 格式的文件
*           2.SaveAs 时会更改默认的扩展名
*           3.增加打开目录并的功能
*       Venis IX -- http://www.spaceblue.com/products/venis/
*       HM VNISEdit --
*       makensisw -- NSIS提供的GUI编译程序, 似乎只在 Unicode 版本下有?
*     语法(基本结构包括 安装程序属性属性, 页面, 区段, 函数 )
*       0.基本语法
*         注释 -- 以分号(;) 开始的行部分
*         转义字符 -- $前缀, 如 $\n 表示换行,  $$ 表示美元符"$", $\"表示双引号
*         命令很长时, 可以用 "\" 来换行继续写, 如 Messagebox MB_OK \<CR> "This is line breaks demo"
*         !define 常量名 [常量值]   -- 引用时：${常量名}, TODO: 如果二次定义会如何, 后面的会覆盖?
*            可以通过 !insertmacro MUI_SET Xxx 的方式来定义常量且防止二次定义
*         !macro 宏名 参数列表 XXX !macroend -- 定义宏,实现中可通过 ${参数名} 的方式引用, 然后通过 !insertmacro 宏名 参数 的方式引用
*         !ifdef|!ifndef MY_DEFINE  xxxxx !endif -- 判断是否定义了特殊的宏, 可通过 !define 或 makensis.exe 命令时通过 /D"MY_DEFINE" 的方式定义
*         !include 头文件名 -- 包含指定的头文件, 然后即可使用其中的常量, 函数, 宏等。如 !include "LogicLib.nsh" 和 !include "MUI.nsh" 等
*         !insertmacro 宏名 -- 在当前位置插入定义的宏（如系统预订的 MUI_PAGE_WELCOME 等各个Page页面都是宏 -- \Modern UI\System.nsh 文件中）
*         !verbose -- 定义输出信息的等级, 默认为3(no script), 为了得到更详细的信息, 可以通过 !define MUI_VERBOSE 4 的方式定义为 all
*         var [/GLOBAL] 变量名, 引用时 $变量名, 变量以字符串方式存储？有1024字节的限制, 定义的变量都是全局的(即使在区段或函数类定义？)
*             颜色 -- 类似HTML中的RGB表示法, 但不用井号"#"
*         常用的预定义系统变量：
*           $INSTDIR(安装目录), $OUTDIR(目标系统的工作路径), $CMDLINE, $LANGUAGE
*           $PROGRAMFILES[64], $COMMONFILES, $DESKTOP(当前用户桌面), $STARTMENU(开始菜单目录), $SMPROGRAMS(开始菜单里的Programs目录),
*           $MUSIC, $FONTS, $APPDATA, $TEMP(系统临时目录)
*           $RESOURCES, $RESOURCES_LOCALIZED
*         64位系统安装判断: !include x64.nsh => ${If} ${RunningX64} xxxxx ${Else} yyyyy ${EndIf}
*    安装脚本
*       0.5安装程序属性（如 , InstallDir 等)
*          确定安装程序的性能, 外观和习惯, 控制显示的文本, 安装类型的数量等, 大多数在运行时只读,
*          InstallDir "$PROGRAMFILES\${COMPANYNAME}\${APPNAME_INNER}"  -- 设置缺省安装目录
*          InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" "" <==
*          Name "MyPrgramName" -- 设置安装程序运行时显示的标题栏名字等。然后在脚本里可通过 "$(^Name)" 访问?
*          OutFile "MyProgramSetup.exe" -- 设置编译后的安装程序名字
*          ShowInstDetails|ShowUnInstDetails show -- 设置是否允许用户查看 安装|卸载 时的详细信息
*          RequestExecutionLevel admin -- 在Vista后安装时需要管理员权限
*       1.页面 -- 非静默安装所需要的向导页面, 用于指导用户进行安装。
*         Page/UninstPage/PageEx 页面名 -- 指定特定的安装/卸载页面, 可通过回调函数进行验证,如没有回调, 则对应的位置用 ""
*         内置界面(??) -- uninstConfirm
*           Page <页面名> [pre_function] [show_function] [leave_function]
*         使用预定义的页面(一般是通过 !insertmacro 的方式插入):
*           安装|卸载 相关
*             MUI_PAGE_INIT <== TODO: 测试时没有看到具体的页面
*             MUI_PAGE_WELCOME|MUI_UNPAGE_WELCOME <== 欢迎页面
*             MUI_PAGE_LICENSE|MUI_UNPAGE_LICENSE licenseData <== 版权页面, 可通过参数指定 license 文件(如 ".\license.txt" )
*             MUI_PAGE_COMPONENTS | MUI_UNPAGE_COMPONENTS <== 选择需要安装的组件(TODO: 组件定义?)
*             MUI_PAGE_DIRECTORY|MUI_UNPAGE_DIRECTORY <== 选择安装路径
*             MUI_PAGE_STARTMENU ID $Var <== 用户选择菜单项的位置。参数:
*               ID  <== 如 Application,
*               Var <== 为通过 var 定义的变量(如前面通过 var ICONS_GROUP 定义后, $ICONS_GROUP 使用), 用于获取返回值进行后续操作?
*               另外, 在插入宏前, 可 !define 控制常量:
*                 a.MUI_STARTMENUPAGE_NODISABLE
*                 b.MUI_STARTMENUPAGE_DEFAULTFOLDER
*                 c.MUI_STARTMENUPAGE_REGISTRY_ROOT
*             MUI_PAGE_INSTFILES | MUI_UNPAGE_INSTFILES -- 显示安装文件进度, 要显示这个页面时, 其前一个页面的"Next"按钮会变为"Install"
*               TODO: 这是一个长时间的操作, 是否能控制能否取消, 以及具体的操作?
*             MUI_PAGE_FINISH | MUI_UNPAGE_FINISH -- 结束页面。控制常量:
*               MUI_FINISHPAGE_NOREBOOTSUPPORT -- 不重启系统(Delete,RMDir 等指令带上 /REBOOTOK 时, 如果不能删除会提示重启, 并在重启后删除)
*                 重启标记 <== 判断(IfRebootFlag), 设置(SetRebootFlag)
*               MUI_FINISHPAGE_RUN <== 指向结束后会自动运行的程序, 一般为 "$INSTDIR\xxx.exe"
*           安装特有
*
*           卸载特有
*              MUI_UNPAGE_CONFIRM <== 确认是否卸载
*           定制界面
*             Page <页面名> [creator_function] [leave_function] [caption]
*             MUI_PAGE_FUNCTION_CUSTOM TYPE -- 自定义页面的宏, TYPE可为: PRE, SHOW, LEAVE,
*             0.通过 Function Xxx 定义指定的页面回调函数
*             1.用户 !define MUI_PAGE_CUSTOMFUNCTION_PRE, MUI_PAGE_CUSTOMFUNCTION_SHOW, MUI_PAGE_CUSTOMFUNCTION_LEAVE 等常量为自定义的函数
*
*       2.区段 -- 一般把属于一组功能或者相互关联的功能, 放在一个区段中, 并可以给区段取个名字,
*                 这样在组建页面就可以显示这个名字供用户选择(组件？）。如果没有名字或有前缀"-",表示是一个隐藏的段(用户不能更改).
*                 如果有前缀 "un." 则表示是卸载时的段
*         Section "[un.]区段名"
*            区段内容, 对应某种安装/卸载选项的处理逻辑, 如 解压文件, 读取和写入注册表, INI文件或普通文件, 创建目录, 快捷方式等
*         SectionEnd
*         区段有修饰符, 如 RO
*         子区段：SubSection
*       3.用户自定义函数(使用 Call 指定调用) 和 回调函数（ 如 .onInit )
*         a.用户自定义函数：
*           Function <函数名>
*             函数体
*           FunctionEnd
*         b.NSIS 预定义的回调函数(安装和卸载) -- 注意:插入一些系统页面宏以后会自动生成对应的回调函数
*             .onGUIInit <== TODO: 似乎默认已有(插入了什么宏造成的?)
*             .onInit -- 安装时显示窗体前执行
*             .onInstFailed
*             .onInstSuccess <== 安装成功后执行
*             .onGUIEnd <== 安装程序的GUI结束后执行
*             .onMouseOverSection -- 目前实测,在 MUI_PAGE_COMPONENTS 页面中,鼠标在Section上移动时会执行(可进行效果设置?)
*             .onRebootFailed
*             .onSelChange
*             .onUserAbort
*             .onVerifyInstDir -- MUI_PAGE_DIRECTORY 页面每次用户输入或更改安装目录时执行，一般用于检测目录合法性
*             un.onGUIInit
*             un.onInit <== 卸载时初始化，一般用于向用户确认是否要卸载
*             un.onUninstFailed
*             un.onUninstSuccess <== 卸载成功
*             un.onGUIEnd
*             un.onRebootFailed
*             un.onUserAbort
*       4.指令 -- 可带参数, 如 /REBOOTOK . TODO: 错误处理?
*           基本指令：Delete, Exec, ExecShell, ExecWait, Rename, RMDir
*             Abort -- 中断当前操作, 如在 .onInit 中表示停止安装并退出
*             File -- 释放文件到当前输出路径($OUTDIR) , 通常需要配合SetOutPath使用
*               参数: /r 递归,  /x 排除指定文件,  如 File /r *.dll
*             SetOutPath -- 设置输出路径(即更改$OUTDIR的值?), 当路径不存在时会自动创建。常用的路径有  $INSTDIR, $SYSDIR,
*             SetRegView -- 设置注册表函数操作的路径(64位系统上的32位WOW64程序只能访问 32位的View，需要通过 SetRegView 64 来允许访问64位的View)
*           标号：使用Goto, IfErrors等语句进行程序控制, 语法为"标号: 语句", 标号必须定义在函数和区段中, 其作用范围仅限于定义它的区段或函数。
*              以点号"."开头的标号是全局标号（如 .onInit 等预定义的回调函数?）
*           相对跳转：语法为 "[+-][1-9]", 加号表示从当前位置(为0)往前跳转, 减号则表示从当前位置往后跳转。数字表示跳转的语句条数。
*              代码改动的时候通常需要修改相对跳转, 容易出错。因此推荐使用 LogicLib.nsh 代替。
*              如 Goto +4
*           分支判断：IfErrors, IfSilent, IfFileExists, StrCmp,
*                     LogicLib.nsh 文件中提供很多简单的宏便于构造复杂的逻辑结构(条件或联合逻辑判断),不需要格外的跳转标记(★推荐用法★), 如：
*                       ${If} $0 == 'some string' ${ElseIf} xxx ${Else} yyy {EndIf}
*                       ${Switch} $0 ${Case} 'some string' xxx ${Break} ${Default} yyy ${Break} ${EndSwitch}
*           注册表：ReadRegStr
*           调试函数：
*              MessageBox MB_YESNO "提示信息" IDYES 跳转标号 或 MessageBox MB_OK "提示信息". 可通过 /SD 返回值 指定silent模式时的返回值
*              DetailPrint 字符串信息或变量 -- 在安装的进度窗体显示信息, 如 DetailPrint $R0
*              Dumpstate
*
*       6.插件(扩展NSIS安装程序的DLL, 系统预安装的在 Plugins 目录下, 用户可用 !addplugindir 增加目录位置)
*         使用语法： DllName::FunctionName [选项] "参数1" 参数2" "参数3"
*           插件中使用参数可通过 popstring/popint 等获取, 处理结果通过 pushstring/pushint 等再放入堆栈
*           插件放入堆栈的处理结果, 在脚本中可通过 Pop $0 后进行访问
*         如（下载文件）： NSISdl::download http://download.nullsoft.com/winamp/client/winamp291_lite.exe $R0
*            (调用DLL函数): System::Call '不带扩展名的Dll名::函数() 参数'
*         可用选项:
*           /NOUNLOAD -- 调用完插件后不卸载, 可用于在插件内保存了数据的情况(坏的设计？)
*       6.1自己编写插件(示例参见 NSIS\Examples\Plugin ) -- 插件函数的原型声明如下：
*          extern "C" __declspec(dllexport) void mypluginfunc(HWND hWndParent, int string_size, TCHAR *variables, stack_t **stacktop, extra_parameters *extra);
*            hWndParent -- 整个安装的根窗口句柄
*            stacktop -- 参数堆栈, 可通过 popstring/pushint 等函数操作, 从而和脚本交互
*            extra -- 插件里调用script函数时使用, 使用示例:
*            a.脚本中获取脚本函数DllInit的地址, 然后传递给插件:
*              Function DllInit 脚本的函数实现 FunctionEnd
*              GetFunctionAddress $2 DllInit
*              myPlugin::Init /NOUNLOAD $2
*            b.插件中通过堆栈获取到地址后, 使用 ExecuteCodeSegment 进行回调:
*              EXDLL_INIT(); int nFunc=popint(); extra->ExecuteCodeSegment(nFunc-1,g_hwndParent);
*          通过堆栈或全局变量($0~$9,$R0~$R9 等)传递参数, 返回结果
*
*       7.更改默认的UI -- 在 "MUI Settings" 后通过 !define MUI_XXXX_YYY "文字或位图地址" 的方式指定特定时刻所使用的文字或位图等信息
*         MUI_WELCOMEPAGE_TITLE -- 安装包标题(字符串)
*         MUI_WELCOMEFINISHPAGE_BITMAP --
*         MUI_WELCOMEPAGE_TEXT --
*       8.多语言控制 -- 如果需要重新定义文字或语言,查看 %NSIS%\Contrib\Language files\ 下的文件，定义对应的常量即可
*         !insertmacro MUI_LANGUAGE "语言(如English, SimpChinese) <== 需要对应的资源文件(如 Contrib\Language files\English.nsh)
*
*       9.系统控制(一般是通过 !define 定义的一些宏 ?  查看System.nsh 等系统文件源码)
*         MUI_ABORTWARNING -- 控制安装时取消的话是否有提示
*       10.系统提供的常用 .nsh 文件
*          FileFunc.nsh -- 文件相关的辅助函数,如获取大小、扩展名等
*          Library.nsh -- 一些dll, tlb 等相关的辅助函数
*          LogicLib.nsh -- 逻辑控制宏
*          WinMessages.nsh -- Windows 消息定义
*          x64.nsh -- 控制在64位系统上安装逻辑
******************************************************************************************************************************/
