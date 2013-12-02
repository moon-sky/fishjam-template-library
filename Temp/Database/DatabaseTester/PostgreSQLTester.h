#pragma once

/*****************************************************************************************************
* 问题：
*   1.OID -- 只要创建表时没有使用 WITHOUT OIDS，PG创建的每一行记录都会获得一个唯一的OID(4字节整数，溢出后重复),
*     内部系统表里使用 OID 在表之间建立联系
*   2.查询 work_mem -- 每个进程可使用的工作内存空间，类似swap
*
* PostgreSQL -- 完全免费，开发源码的对象关系型数据库管理系统，遵从BSD版权。支持多种语言(C/C++/Java/Perl/python 等。
*   以BSD开发的 POSTGRES 版本 4.2为基础开发。
*   支持大部分SQL标准并且提供了许多其他现代特性：复杂查询、外键、触发器、视图、事务完整性、多版本并发控制。
*   多线程支持（--enable-thread-safety，默认值）：不同的线程可以同时使用 不同 的连接(PGConn) -- 一个连接不支持多线程
*   支持全文检索，允许对列中的单词进行搜索
* 
* 编译安装(./configure –help来查看可用的参数)：
*   1../configure --prefix=/usr/local/pgsql && make && make install
*   2.增加 postgresql 数据库的用户 postgres 并设置密码
*     useradd postgres -m -d /export/postgres/ 
*     chown postgres:users postgres/ -R
*     passwd 
*   3.设置环境变量
*     PGHOME，
*   常见错误:
*     a.configure: error: readline library not found   <== 配置时 --without-readline 或 安装 yum install readline-devel
*     
*   需要设置 POSTGRES_HOME，PGLIB，PGDATA 等环境变量？
*
* 使用：
*   初始化：initdb -D <数据库地址>。初始化成功后，其下会有一个 postgresql.conf 的配置文件
*       在做 initdb 时必须采用缺省的本地设置 C locale -- 否则使用like时可能无法使用索引？创建一个特殊的text_pattern_ops索引来规避？
*     postgresql.conf -- 设置 listen_address='*' 、port，使得可以远程访问(否则只在 127.0.0.1 上监听)
*     pg_hba.conf -- 设置能访问的用户、数据库、客户端IP等， 如 host all all 192.168.1.0/24 password
* 
*   启动：pg_ctl -D <数据库地址--如/export/home/tsdata）> -l logfile start
*     实际运行的数据库程序是 postgres
*   停止：pg_ctl stop -m 关闭模式(有smart、fast、immediate)
*   创建DB：createdb <DB名字>
*   用 pgpool 同步
*   
* postgresql.conf
*   日志目录通过参数 log_directory 设置，如"/var/postgres/pg_log" 
*   shared_buffers <== 共享内存，查询数据时，顺序为 共享内存 -> OS缓存 -> 硬盘，因此共享内存与OS的缓存一致较安全
*   checkpoint <== 把共享内存里更新国的数据写到磁盘的时机，通过检查日志中的 "checkpoint"来确认
*     checkpoint_segments -- 定量大小，一个是16M，默认为3，即48M
*     checkpoint_timeout -- 定时，默认5分钟
*
* 类型
*   类型	    内部名称	说明
*   SERIAL                  序列，会自动转换为 INT4 NOT NULL DEFAULT nextval('表名_字段_seq'),
*   VARCHAR(n)	varchar	    指定了最大长度，变长字符串，不足定义长度的部分不补齐，通常存储限制了最大长度的变长字符
*   CHAR(n)	    bpchar	    定长字符串，实际数据不足定义长度时，以空格补齐，适合于存储长度相同的字符串
*   TEXT	    text	    没有特别的上限限制（仅受行的最大长度限制），适用于存储最大可达1G左右但未定义限制长度的字符串
*   BYTEA	    bytea	    变长字节序列，适用于存储二进制数据，尤其是包含 NULL 字节的值
*   "char"	    char	    单个字符
*
* 
* 客户端
*   Lin：psql -p 端口号 -U 用户名 -d 数据库名
*     带 -E 选项启动 psql,将打印出你在psql中所给出的命令执行时的内部实际使用的SQL查询语句
*     \q 退出
*     \? 帮助
*     \dt -- 显示数据表的定义；  \dn -- 显示Schema
*     创建用户：  create user fujie password '123456';
*     创建数据库：create database mydata owner fujie; 
*   Win：pgAdmin ,目前是 3-1.12，主要配置项：listen_address, port
*   为了方便客户端的使用，可以安装 phpPgAdmin，使用 B/S 结构进行管理，需要apache、php4、php4-pgsql等软件的支持
*
* 并发控制
*   PG使用多版本并发控制机制(Multiversion Concurrency Control, MVCC) -- 每个事务在查询数据时，看到的是数据的快照.
*   只有在两个事务试图同时更新同一个数据行时，才会有等待出现。
*   PostgreSQL只提供了两种事务隔离级别(命令SET TRANSACTION来设置)
*     Read Committed(默认) -- 事务发出的SELECT命令只能看见在SELECT命令开始执行以前提交的数据，使用简单，速度较快
*     Serializable -- 最严格，所有并发执行的事务的执行结果和单个事务一个一个地执行的结果是一样的
*
* 常见的提升性能方法：
*   1.使用COPY语句代替多个Insert语句(需要是admin用户)；
*   2.将多个SQL语句组成一个事务以减少提交事务的开销；
*   3.从一个索引中提取多条记录时使用CLUSTER；
*   4.从一个查询结果中取出部分记录时使用 LIMIT n；如果在执行查询功能时不知道确切的记录数， 可使用游标(cursor)和FETCH功能。
*   5.使用ANALYZE以保持精确的优化统计；
*   6.定期使用 VACUUM 或 pg_autovacuum -- 可以使用 VACUUM ANALYZE，或 ANALYZE 获得有关表的统计值
*   7.进行大量数据更改时先删除索引（然后重建索引）
*
* 查询：
*   正则表达式搜索  <== 操作符 ~ 处理正则表达式匹配，而 ~* 处理大小写无关的正则表达式匹配，大小写无关的 LIKE 变种成为 ILIKE
*     大小写无关的等式比较通常写做 WHERE lower(col) = 'abc';
*   连接 <== 
*     普通连接(内连接)：
*     左外连接：SELECT * FROM t1 LEFT OUTER JOIN t2 ON (t1.col = t2.col); 返回 t1 中所有未连接的行, 
*     右外连接：SELECT * FROM t1 RIGHT OUTER JOIN t2 ON (t1.col = t2.col); 返回 t2 中未连接的行
*     完全外连接（FULL OUTER JOIN）,返回 t1 和 t2 中未连接的行
*
* 性能测试
*   更改配置中的 log_xxx 服务器配置选项
* 
* DDL语言
*   CREATE INDEX 索引名 ON 表名(字段)
*   CREATE SEQUENCE 序列号名 -- 通常命名为 表名_字段_seq
*   INSERT INTO 表名 (字段) VALUES ('值') RETURNING id;  -- 可以返回主键的值
*
* 约束
*   UNIQUE
*   CHECK
*
* 编程
*   DB同步连接：PQconnectdb 或 PQsetdblogin，返回的 PGConn* 用 PQfinish 释放
*   DB异步连接：PQConnectStart 和 PQconnectPool -- 用空格间隔的参数（关键字 = 数值 -- 注意等号两边得有空格）
*     关键字有：host(主机)，hostaddr(IP地址)，port(端口)，dbname、user、password、connect_timeout、
*               options(给服务器的命令行选项)、sslmode(是否使用SSL连接，缺省是prefer)
*   错误检查  ：PQstatus(连接)、PQerrorMessage
*   状态值获取：PQSocket(可获取数据库链接下层的套接字描述符)
*               PQparameterStatus(检查服务器参数，如 server-version 等)
*               PQServerVersion
*   事务处理  ：PQtransactioStatus
*   命令执行  ：PQexec，多个分号分隔的命令，自动在一个事务中，结果需要用 PQclear 释放
*               PQExecParams，可传参数，可要求查询结果是文本(0)或二进制(1)格式，参数必须是 $1,$2 等格式
*               PQprepare/PQexecPrepared，准备一个执行语句，可避免每次执行都分析，返回的结果不要直接访问其中的字段，应该用 PQresultStatus 来访问
*               PQntuples(返回行个数)，PQnfields(返回列个数)，PQfname(字段名称)，PQftable(返回字段所在的表的OID)
*   锁操作：支持表级锁、行级锁、建议锁(pg_advisory_lock) -- 应用程序能够完全自主地控制得到和释放锁的时间
*****************************************************************************************************/

class CPostgreSQLTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPostgreSQLTester );
    //CPPUNIT_TEST( test_Time );
    CPPUNIT_TEST_SUITE_END();

private:
    DECLARE_DEFAULT_TEST_CLASS(CPostgreSQLTester);

    //void test_Time();

};