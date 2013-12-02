#pragma once

TODO:命令行(批处理) 中执行 color 0D 可以设置控制台的颜色(值范围 >= 01 )
	 MODE con: COLS=71 LINES=25 -- 设置高宽
	echo %date:~0,10% -- 当前年月日
	
setlocal enabledelayedexpansion -- ???
	
/*****************************************************************************************************
* MySQL目前属于Oracle，采用双授权政策，分为社区版和商业版，开放源码。
* 数据库引擎: MyISAM(较高的插入，查询速度，但不支持事务) -- 5.5之前默认使用
*              InnoDB(支持ACID事务、行级锁定) -- 5.5之后默认使用;
*              Memory -- 所有数据置于内存，拥有极高的效率。但占用内存空间，且内容会在重启时丢失。
*              Archive -- 适合存储大量的独立的，作为历史记录的数据，拥有很高的插入速度，但其查询支持较差。
*              Federated -- 将不同的Mysql服务器联合起来，逻辑上组成一个完整的数据库，适合分布式应用
*
* 支持AIX、FreeBSD、HP-UX、Linux、Mac OS、Solaris、Windows 等多种操作系统。
* 为多种编程语言(C、C++、Python、Java、Perl、PHP 等)提供了API
*
* 数据库配置(my.ini) -- 超级用户默认名是 root ?
*   1.字符编码--通常使用utf8或gbk;使用mysql.exe时,在执行数据操作命令之前运行一次"SET NAMES GBK;"
*   2.
*   
*
*   create 类型 名字 -- 创建新的数据，如 database, table,
*   delete from 表名 [where xxx] -- 删除表中的数据，如无条件，则删除全部
*   describe 表名; -- 显示表的属性结构, 可缩写为 desc
*   drop 类型 [if exists] 名字 -- 删除数据，如 database, table
*   insert into 表名 (字段名列表) values (对应的值列表); -- 在表中插入数据
*   load data local infile "文件名" into table 表名 -- 将文本文件中的表数据加载到指定表(格式: 字段数据之间用tab键隔开，null值用\n来代替)
*   show databases/tables; -- 显示所有数据库/当前数据库中的表
*   show errors; -- 显示最后一个执行语句所产生的错误
*   show warnings; -- 显示最后一个执行的语句所产生的错误、警告和通知
*   use 数据库名称; -- 使用指定数据库
*   
* 数据库优化
*   1.WHERE子句的查询条件里有不等号(!=), 通常无法使用索引
*   2.如果WHERE子句的查询条件里使用比较操作符LIKE和REGEXP,只有在搜索模板的第一个字符不是通配符的情况下才能使用索引
*   3.在ORDERBY操作中，MySQL只有在排序条件不是一个查询条件表达式的情况下才使用索引
*
* 性能分析
*   1.使用 EXPLAINSELECT 关键字执行查询，则会以表格的形式把查询的执行过程和用到的索引等信息列出来。
*     type列指定了本数据表与其它数据表之间的关联关系(JOIN),
*     效率由高到低依次为：system > const > eq_ref > ref > range > index > All
*
* 系统数据库和表
*   mysql -- 维护系统信息
*****************************************************************************************************/

/*****************************************************************************************************
* 使用SQL脚本文件
*   方法1:mysql -uroot -p密码 < sql文件
*   方法2:进入 mysql 环境后，source sql文件

* 命令行工具
*   mysql.exe, 以分号结尾，使用完毕后通过 exit 命令退出
*     命令行参数: -h主机 -u用户名 -p密码 -- 指定连接的主机、用户名和密码
*     
*     MySql环境中的命令( 如有特殊单词或符号，则需要用引号扩起来 ):
*       允许任意主机远程连接: 
*         GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIEDBY "密码" WITH GRANT OPTION;
*         FLUSH PRIVILEGES;
*       控制用户权限
*         grant select,insert,update,delete on 数据库名.表名 to 用户名@登录主机 identified by "密码"
*   mysqladmin -u用户名 -p旧密码 password 新密码
*   mysqldump -- 备份，恢复?
*     mysqldump --opt school>school.bbb -- 测试失败(unknown option '--no-beep')
*
* 管理工具
*   MySQL Administrator
*   MySQL Query Browser
*   MySQL Workbench
*   phpMyAdmin -- 用Web界面管理MySQL资料库
*****************************************************************************************************/

/*****************************************************************************************************
* 常见错误及解决方案
*   1.中文排序错误 -- 进行中文排序和查找的时候，对汉字的排序和查找结果是错误的
*	  原因:缺省以ISO-8859字符集作为默认的字符集，
*     解决:a.对于包含中文的字段加上"binary"属性，使之作为二进制比较, 如 name char(10) => name char(10)binary
*          b.源码编译时使用 --with--charset=gbk 参数, TODO: 使用 UTF8 ?
*****************************************************************************************************/	
#if 0
class CMySqlTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CMySqlTester );
    //CPPUNIT_TEST( test_Time );
    CPPUNIT_TEST_SUITE_END();

private:
    DECLARE_DEFAULT_TEST_CLASS(CMySqlTester);

    //void test_Time();

};
#endif 