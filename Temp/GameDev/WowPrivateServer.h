#pragma once

/**********************************************************************************************************************
* 说明(相关工具在mangos的 contrib 中 )
*   1.mpqe.exe客户端dbc文件 -- https://svn.code.sf.net/p/mangoscms/code/trunk/tools/MPQE/
*     暴雪游戏的mpq(Mike O'Brien Pack--该人是暴雪的首席游戏程序开发人员和创始者)文件包，
*     可以使用 MPQE_1.2.rar 工具进行提取，可以将 *.dbc(游戏信息)提取到 mangos/dbc 中
*     mpqe.exe拷贝到data(还是 /data/zhCN ？)目录下，命令行执行 mpqe.exe /p dbc.MPQ  DBFilesClient/*.dbc, 最后拷贝到 mangos/dbc 中
*     TODO:命令应该是? mpqe /p local-zhCN.mpq DBFilesClient/*.dbc
*   2.地图提取工具ad.exe -- https://svn.code.sf.net/p/mangos/code/trunk/contrib/extractor
*     为支持中文，需要修改system.cpp中的语言代码 char* langs[]={"zhCN", ... }
*     拷贝到WOW安装目录中，建立空的maps子目录，然后运行ad.exe并等待其提取地图信息,
*     将解压地图拷贝到 mangos/maps 中
*   3.处理Vmap(处理"视线"问题) -- vmap_extract_assembler_bin
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
*     1.sql\create_mysql.sql -- 创建所有的空数据库
*       characters
*       mangos -- 通过导入YTDB的方式恢复， 需要约 5、6 个小时?
*                 然后按顺序逐步执行YTDB中名字带 mangos 的升级补丁，
*                 查看 db_version 表，看最后一列的列名前面的编号(如 11169_02)，然后再导入sql\updates文件夹下对应编号后的所有sql文件
*       realmd -- login 信息的数据库，main中循环处理 ACE_Reactor::run_reactor_event_loop，处理 AuthSocket 的接入?
*     2.src\bindings\ScriptDev2\sql\scriptdev2_create_database.sql -- 创建 scriptdev2 库
*       scriptdev2 -- scriptdev2_script_full.sql 脚本创建表
*   配置文件
*     mangosd.conf -- 配置物品，金钱的掉落率，经验值的增长率等
*       [list]
*         DataDir = "./"
*         WorldDatabaseInfo = "127.0.0.1;3306;root;[密码];mangos"
*         LoginDatabaseInfo = "127.0.0.1;3306;root;[密码];realmd"
*         CharacterDatabaseInfo = "127.0.0.1;3306;root;[密码];characters"
*     realmd.conf -- 
*       LoginDatabaseInfo="127.0.0.1;3306;root;[密码];realmd"
**********************************************************************************************************************/

/**********************************************************************************************************************
*
* 搭建魔兽私服 -- http://blog.csdn.net/lodger007/article/details/2279422 -- 私服的搭建流程
*                 http://log4think.com/setup_wow_private_server/
*                 http://blog.csdn.net/xiadasong007/article/details/7522910
*                            
* 中文论坛(芒果中国) -- http://www.mangoscn.com/forum.php?mod=viewthread&tid=6864
* 
* MaNGOS(Massive Network Game Object Server) -- 开源(GPL)的MMORPG游戏服务器源码框架, 理论上支持任何客户端的网络游戏, 目前常用于魔兽私服。
*   新版本 -- https://github.com/mangos , 其上可以找到对应各个Wow版本的程序( Zero -> Four )
*     0.Mangos Zero  -- For Vanilla World of Warcraft
*     1.Mangos One   -- The Burning Crusade(燃烧的远征)
*     2.Mangos Two   -- The Wrath of the Lich King(巫妖王之怒)
*     3.Mangos Three -- The Cataclysm(大灾变)
*     4.Mangos Four  -- Mists of Pandaria(潘达利亚的迷雾)
*
*     
*   
*  
*   核心部分是个和特定游戏没有关系的核心框架程序 -- 进行进程调度，创造世界，建立心跳机制，处理网络接入等。
*   而游戏内容数据库，游戏人物，时间，世界脚本，都是由这个核心程序所支持的扩展脚本来实现。
*    
*     游戏内容数据库，需要对应不同的Mangos版本(CompleteSDB156-Mangos3462.rar)
* 
*   Linux 下编译前需要(如代码是通过SVN下载的) autoreconf --install --force
*   doc -- 可以生成 Doxygen 格式的帮助文件
* 
* 脚本系统(scriptdev2)，编译生成 scriptdev2.dll， 针对WoW的游戏脚本，增加了额外的怪物及BOSS的AI，小怪提供框架由数据库提供支持
*   https://github.com/scriptdev2/scriptdev2 -- 最新 ?
*   https://svn.code.sf.net/p/scriptdev2/code/, 需要下载到 mangos\trunk\src\bindings\ScriptDev2 目录中(会通过相对路径使用mangos中的文件)
*   https://scriptdev2.svn.sourceforge.net/svnroot/scriptdev2
* 
* YTDB 数据库 -- 整合了ACID和scriptdev2脚本的数据库， 如下载 YTDB_0.14.1_R580_MaNGOS_R11000_SD2_R1946_ACID_R307_RuDB_R38.6.7z
*   http://svn2.assembla.com/svn/ytdbase/  -- 无法访问， 但可以访问论坛 http://ytdb.ru/
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
*     3.MySQL ( http://dev.mysql.com/downloads/mysql/ ) , 创建数据库时，其 root 密码最好为 mangos(.conf 文件中的默认值)
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

/**********************************************************************************************************************
* 魔兽目录分析
*   Wow.exe -- 原版登录器
*   Launcher.exe -- 登录器，集成网站公告，更新检查，修复等多项功能 
*   Data -- 核心版本数据，包括3D模型等
*   WTK  -- 游戏的配置数据文件夹
**********************************************************************************************************************/
