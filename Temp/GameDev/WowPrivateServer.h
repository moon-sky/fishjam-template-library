#pragma once

/**********************************************************************************************************************
* 说明
*   暴雪游戏的mpq(Mike O'Brien Pack--该人是暴雪的首席游戏程序开发人员和创始者)文件包，
*     可以使用 MPQE_1.2.rar 工具进行提取，可以将 *.dbc(游戏信息)提取到 mangos/dbc 中
*     mpqe.exe拷贝到data目录下，命令行执行 mpqe.exe /p dbc.MPQ  DBFilesClient/*.dbc, 最后宝贝到 mangos/dbc 中
*   地图提取工具ad.exe -- 拷贝到WOW安装目录中，建立空的maps子目录，然后运行ad.exe并等待其提取地图信息,
*     将解压地图拷贝到 mangos/maps 中
*   相关的数据库(MySQL)
*     mangos
*     realmd
*   配置文件
*     mangosd.conf -- 配置物品，金钱的掉落率，经验值的增长率等
*     realmd.conf -- 
*     
**********************************************************************************************************************/

/**********************************************************************************************************************
*
* 使用Trinity搭建魔兽私服 -- http://log4think.com/setup_wow_private_server/
*                            http://blog.csdn.net/xiadasong007/article/details/7522910
* 
* MaNGOS(Massive Network Game Object Server) -- 开源(GPL)的MMORPG游戏服务器源码框架
*   ( https://github.com/mangos 或 http://svn.code.sf.net/p/mangos/code/trunk )
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
*       MySQL GUI Tools ( http://dev.mysql.com/downloads/gui-tools/5.0.html )
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
