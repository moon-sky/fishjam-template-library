#pragma once


/**********************************************************************************************************************
* 使用Trinity搭建魔兽私服 -- http://log4think.com/setup_wow_private_server/
*                            http://blog.csdn.net/xiadasong007/article/details/7522910
* 
* MaNGOS（Massive Network Game Object Server） -- 开源(GPL)的MMORPG游戏服务器源码框架(https://github.com/mangos)，常用于魔兽私服
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