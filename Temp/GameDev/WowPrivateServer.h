#pragma once

/**********************************************************************************************************************
* http://blog.csdn.net/lodger007/article/details/2279422 -- 私服的搭建流程
* 说明(相关工具在mangos的 contrib 中 )
*   1.mpqe.exe客户端dbc文件 -- https://svn.code.sf.net/p/mangoscms/code/trunk/tools/MPQE/
*     暴雪游戏的mpq(Mike O'Brien Pack--该人是暴雪的首席游戏程序开发人员和创始者)文件包，
*     可以使用 MPQE_1.2.rar 工具进行提取，可以将 *.dbc(游戏信息)提取到 mangos/dbc 中
*     mpqe.exe拷贝到data(还是 /data/zhCN ？)目录下，命令行执行 mpqe.exe /p dbc.MPQ  DBFilesClient/*.dbc, 最后拷贝到 mangos/dbc 中
*     TODO:命令应该是? mpqe /p local-zhCN.mpq DBFilesClient/*.dbc
*   2.地图提取工具ad.exe -- https://svn.code.sf.net/p/mangos/code/trunk/contrib/
*     为支持中文，需要修改system.cpp中的语言代码 char* langs[]={"zhCN", ... }
*     拷贝到WOW安装目录中，建立空的maps子目录，然后运行ad.exe并等待其提取地图信息,
*     将解压地图拷贝到 mangos/maps 中
*   3.处理Vmap(处理"视线"问题)
*     执行 mangos 中的 makevmaps_SIMPLE.bat，将生成目录 vmaps/*.vmap(?) 文件拷贝到mangos/vmaps 目录中
*   4.使用UDB数据丰富游戏世界 -- (网站不可访问)http://www.udbforums.org/
*     其数据导入 mangos 数据库
*   5.配置MaNGOS和客户端
*     5.1.在客户端的wtf子目录下创建或打开 realmlist.wtf 文件，写入"set realmlist 127.0.0.1"
*     5.2.修改 mangosd.conf 和 realmd.conf 配置文件
*   6.运行服务器端
*     start realmd.exe + start mangosd.exe
*   7.直接运行客户端 Wow.exe 
*   8.如出现问题，则查看log文件，用sql/update中的数据脚本更新出现问题的数据表
* 
*   相关的数据库(MySQL), Restore from SQL Dump -> 选择sql目录下对应的sql文件
*     characters
*     mangos 
*     realmd
*     scriptdev2 -- 针对WoW的游戏脚本，增加了额外的怪物及BOSS的AI
*   配置文件
*     mangosd.conf -- 配置物品，金钱的掉落率，经验值的增长率等
*       [list]
*         DataDir = ""
*         WorldDatabaseInfo = "127.0.0.1;3306;root;[密码];mangos"
*         LoginDatabaseInfo = "127.0.0.1;3306;root;[密码];realmd"
*         CharacterDatabaseInfo = "127.0.0.1;3306;root;[密码];characters"
*     realmd.conf -- 
*       LoginDatabaseInfo="127.0.0.1;3306;root;[密码];realmd"
**********************************************************************************************************************/

/**********************************************************************************************************************
*
* 使用Trinity搭建魔兽私服 -- http://log4think.com/setup_wow_private_server/
*                            http://blog.csdn.net/xiadasong007/article/details/7522910
* 
* MaNGOS(Massive Network Game Object Server) -- 开源(GPL)的MMORPG游戏服务器源码框架
*   ( https://github.com/mangos 或 https://svn.code.sf.net/p/mangos/code/trunk )
*   理论上支持任何客户端的网络游戏, 目前常用于魔兽私服。
*  
*   核心部分是个和特定游戏没有关系的核心框架程序 -- 进行进程调度，创造世界，建立心跳机制，处理网络接入等。
*   而游戏内容数据库，游戏人物，时间，世界脚本，都是由这个核心程序所支持的扩展脚本来实现。
*    
*     游戏内容数据库，需要对应不同的Mangos版本(CompleteSDB156-Mangos3462.rar)
* 
*   Linux 下编译前需要(如代码是通过SVN下载的) autoreconf --install --force
*   doc -- 可以生成 Doxygen 格式的帮助文件
* 
* 脚本系统(scriptdev2)，编译生成 scriptdev2.dll
*   https://svn.code.sf.net/p/scriptdev2/code/
*  
*   任务系统
*   技能/法术系统
*   脚本系统
*   工会系统
*
* 
* Trinity -- 基于MaNGOS的代码开发(https://github.com/TrinityCore/TrinityCore.git)
*   编译安装
*     0.Git (安装完毕后需要加入Path) + TortoiseGid，并下载更新源码
*     1.OpenSSL ( http://slproweb.com/products/Win32OpenSSL.html ,16M左右的, 为何不能下载官网的?)
*     2.CMake ( http://www.cmake.org/cmake/resources/software.html ),
*     3.MySQL ( http://dev.mysql.com/downloads/mysql/ ) 
*       MySQL GUI Tools ( http://dev.mysql.com/downloads/gui-tools/5.0.html )，管理工具也可以用 SQLyog
*       注意:a.下载时登录账号的话，需要是IE核心的浏览器。
*            b.不要用最新的 5.0(有问题), 用 4.0.2 即可?
*     4.通过 CMake-Gui 来生成VS的工程文件：
*       Configure 中选中 “Use default native compilers” -> Visual Studio 9 2008 -> Finish 后会自动分析。
*       分析结束后选择编译 SCRIPTS、SERVERS、TOOLS、USE_COREPCH、USE_SCRIPTPCH, 不要选择 USE_MYSQL_SOURCES。
*       点Generate，就会在指定目录下生成VS的工程文件
*     5.使用VS打开工程文件，Rebuild。
*     6.会自动拷贝 libmySQL.dll(MySQL)和 libeay32.dll,ssleay32.dll(OpenSSL)到编译输出目录，如版本不对，则自行拷贝
**********************************************************************************************************************/


/**********************************************************************************************************************
* 模拟魔兽世界的项目 -- 
* 
**********************************************************************************************************************/
