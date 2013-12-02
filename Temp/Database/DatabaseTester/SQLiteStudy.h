#pragma once

/*****************************************************************************************************
* http://www.sqlite.org 、http://www.sqlite.com.cn
*   注意：由于 sqlite3.c 是C语言的代码，如果直接关联在工程里的话，应该采用 extern "C" { #include "sqlite3.h" }; 的方式
*
* SQLite是一款轻型的、开源的、遵守 ACID 的关联式文件型数据库管理系统，其设计目标是嵌入式，占用资源非常低，
*   储存在单一磁盘文件中的一个完整的数据库,支持数据库大小至2TB
* SQLite是无类型(Typelessness)的，但仍然建议在Create Table语句中指定数据类型
*   支持的数据类型有：VARCHAR、NVARCHAR、TEXT、INTEGER、FLOAT、BOOLEAN、CLOB、BLOB、TIMESTAMP、NUMERIC、
* 
* 支持的SQL包括
*   comment、expression 
*   TRANSACTION：BEGIN、COMMIT、END、ROLLBACK 
*   CREATE：INDEX、TABLE、TRIGGER、VIEW
*     创建表：create table table_name( ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, field2 类型2, ...);
*       自增列在插入NULL值时会自动增加，通过 sqlite3_last_insert_rowid 函数获取加入的ID值
*     创建索引：create index index_name on table_name(field_to_be_indexed);
*   DATABASE：ATTACH、DETACH
*   COPY、DELETE、INSERT、SELECT、UPDATE、EXPLAIN、PRAGMA、REPLACE、
*     插入数据：insert into 表名 [(字段列表)] values(data1, data2, data3, ...); 
*       字符串用单引号(')包含，如果字段没有内容，可以填入NULL
*     查询数据：select 字段 from 表名 where 条件 order by 字段名 (desc) limit 条数;
*       like '部分字段%' -- 通用条件
*       count(*) -- 统计条数
*     更改数据：update 表名 set 字段名='值' where 条件;
*     删除数据：delete from 表名 where 条件;
*   DROP：INDEX、TABLE、TRIGGER、VIEW
*   ON CONFLICT clause
*     

* 系统表
*   sqlite_master、
*
* 大量插入资料时，使用"begin;"，然后insert数据后，通过"commit;"命令进行提交
* 
* 命令 ( sqlite3 数据库名.db -- 打开或创建数据库，如果之前没有数据库，输入 分号";" 后确认即可生成数据库)
*   SQL指令以分号(;)结束，两个减号(--)则代表注释，Ctrl+C 可以结束当前命令
*   .help -- 显示帮助信息
*   .quit -- 退出
*   .backup [数据库名，默认是main] FILE -- 备份数据库到文件，可用 .restore 进行恢复
*   .bail ON|OFF -- 遇到错误是继续还是终止选项,默认是终止.
*   .databases -- 显示文件中数据库列表,如果没有附加数据库的情况下，文件中只有main数据库
*   .dump [表名] -- 按SQL语句的方式输出表中的数据
*   .echo ON|OFF -- 控制是否回显操作命令，默认隐藏
*   .explain ON|OFF -- 设置输出格式为column 
*   .header(s) ON|OFF -- 控制是否 输出|隐藏字段名
*   .import FILE TABLE -- 把文件中的数据导入到表中，各字段用separator（默认是"|"）的值为分隔符
*   .indices TABLE -- 列出特定表的所有的索引
*   .mode MODE ?TABLE? 输出格式选项：有 csv(逗号分隔)、column(指定宽度)、html、insert(SQL语句)、
*      line(field=value 格式)、list(由.separator分隔)、tabs、tcl 等
*   .output FILENAME -- 把输出结果(如 select、)输出到文件，默认是 stdout(输出到屏幕)
*   .read 文件名 -- 执行文件中的SQL语句,文件中的语句一定要带上分号(;).
*   .restore [DB] FILE -- 从备份文件还原数据
*   .schema ?TABLE? -- 显示用于创建当前数据库的 CREATE TABLE 和 CREATE INDEX 语句
*   .show -- 显示配置信息(echo、explain、headers、mode、nullvalue、output 等)
*   .tables ?PATTERN? -- 显示库中的表
*   
* 函数说明
*   sqlite3_exec -- 可以通过 sqlite3_callback 指定在sql语句执行后调用的回调。通常只在 select 时才使用，
*     如回调：int LoadMyInfo(void *param, int n_column, char** column_value, char** column_name)在每查到一条记录就调用一次。
*     参数分别代表 传入的参数，一条记录有多少字段，字段内容的一维数组，字段名的一维数组
*
* 常用的免费管理工具
*   SQLite Expert –- http://www.sqliteexpert.com/features.html
*   Sqliteadmin Administrator -- http://sqliteadmin.orbmu2k.de/
*   SQLite Database Browser -- http://www.oschina.net/p/sqlitebrowser
*   SQLiteSpy -- http://www.oschina.net/p/sqlitespy
*   SQLite Manager 0.8.0 Firefox Plugin -- https://addons.mozilla.org/en-US/firefox/addon/sqlite-manager/
*****************************************************************************************************/
